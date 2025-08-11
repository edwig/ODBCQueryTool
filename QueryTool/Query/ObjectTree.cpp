/* 
  Copyright (C) 2017 Edwig Huisman

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/
#include "pch.h"
#include "ObjectTree.h"
#include "QueryTool.h"
#include "SQLInfoDB.h"
#include "Query\NativeSQLDlg.h"
#include <StyleToast.h>

BEGIN_MESSAGE_MAP(ObjectTree,StyleTreeCtrl)
  ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
  ON_NOTIFY_REFLECT(NM_DBLCLK,         OnItemDoubleClicked)
END_MESSAGE_MAP()

ObjectTree::ObjectTree()
           :m_busy(false)
{
}

ObjectTree::~ObjectTree()
{
}

// Setting the image list for the tree control
// Containing all object type images for the nodes
void
ObjectTree::CreateImageList()
{
  // Create image list (16 x 16 in 32 bits depth, 10 images default, grow by 2)
  m_imageList.Create(16,16,ILC_COLOR32,10,2);

  // Load bitmap from resources
  CBitmap bitmap;
  bitmap.LoadBitmap(MAKEINTRESOURCE(IDB_RDBMS_OBJECTS));

  // Set bitmap in image list, white is background color
  m_imageList.Add(&bitmap,RGB(0,0,0));

  // Set the image list in the tree control
  SetImageList(nullptr,     TVSIL_NORMAL);
  SetImageList(&m_imageList,TVSIL_NORMAL);
  // SetImageList(&m_imageList,TVSIL_STATE);
}

// Each node gets a 'no info' extra node
// to show the "+" in the tree control
void
ObjectTree::InsertNoInfo(HTREEITEM p_item)
{
  HTREEITEM noinfo = InsertItem(_T("No information"),p_item);
  SetItemImage(noinfo,IMG_INFO,IMG_INFO);
}

// If we DO have info, we want the 
// 'No info' marker to go away :-)
void
ObjectTree::RemoveNoInfo(HTREEITEM p_item)
{
  // We now have information. Remove the 'No information' marker
  HTREEITEM child = GetChildItem(p_item);
  if(child)
  {
    CString title = GetItemText(child);
    if(title.Compare(_T("No information")) == 0)
    {
      DeleteItem(child);
    }
  }
}

// Start the tree all over.
// EG. when logging out, or in,
//     or when changing the filter for the tree
void
ObjectTree::ClearTree()
{
  // Stop actions on the tree
  m_busy = true;

  StyleToast* toast = CreateToast(STYLE_TOAST_WARNING,STYLE_POS_MIDMIDDLE,_T("JUST A MOMENT!"),_T("Clearing the object tree!"),_T(""),0);

  // De-allocating large tree's can take a lot of time in the next step
  CWaitCursor takeADeepSigh;

  DeleteAllItems();

  // Rebuild the basic tree
  HTREEITEM root       = GetRootItem();
  HTREEITEM tables     = InsertItem(_T("Tables"),    root,TREE_TABLES);
  HTREEITEM views      = InsertItem(_T("Views"),     root,TREE_TABLES);
  HTREEITEM sequences  = InsertItem(_T("Sequences"), root,TREE_SEQUENCES);
  HTREEITEM usertypes  = InsertItem(_T("UserTypes"), root,TREE_USERTYPE);
  HTREEITEM catalogs   = InsertItem(_T("Catalog"),   root,TREE_TABLES);
  HTREEITEM synonyms   = InsertItem(_T("Synonyms"),  root,TREE_TABLES);
  HTREEITEM triggers   = InsertItem(_T("Triggers"),  root,TREE_TRIGGERS);
  HTREEITEM procedures = InsertItem(_T("Procedures"),root,TREE_PROCEDURES);

  // Setting images on all nodes
  SetItemImage(tables,    IMG_TABLES,     IMG_TABLES);
  SetItemImage(views,     IMG_VIEWS,      IMG_VIEWS);
  SetItemImage(sequences, IMG_SEQUENCE,   IMG_SEQUENCE);
  SetItemImage(usertypes, IMG_USERTYPE,   IMG_USERTYPE);
  SetItemImage(catalogs,  IMG_CATALOGS,   IMG_CATALOGS);
  SetItemImage(synonyms,  IMG_SYNONYMS,   IMG_SYNONYMS);
  SetItemImage(triggers,  IMG_TRIGGER,    IMG_TRIGGER);
  SetItemImage(procedures,IMG_PROCEDURES, IMG_PROCEDURES);

  // Setting a no-info message on all nodes
  InsertNoInfo(tables);
  InsertNoInfo(views);
  InsertNoInfo(sequences);
  InsertNoInfo(usertypes);
  InsertNoInfo(catalogs);
  InsertNoInfo(synonyms);
  InsertNoInfo(triggers);
  InsertNoInfo(procedures);

  // Free all trigger and procedure source code
  m_source.clear();

  DestroyToast(toast);

  // Actions allowed again
  m_busy = false;
}

// See if a node is a schema, or if the schema
// is missing, if it is the node above
// BEWARE: Removes the count from the text
bool
ObjectTree::IsSpecialNode(CString& p_name)
{
  // The node name can contain the SQL comment field!
  int pos = p_name.Find('(');
  if(pos > 0)
  {
    p_name = p_name.Left(pos);
  }
  p_name = p_name.TrimRight();

  // See if it is a type node instead of a schema name
  if(p_name.Compare(_T("Tables"))     == 0)  return true;
  if(p_name.Compare(_T("Views"))      == 0)  return true;
  if(p_name.Compare(_T("Sequences"))  == 0)  return true;
  if(p_name.Compare(_T("UserTypes"))  == 0)  return true;
  if(p_name.Compare(_T("Catalog"))    == 0)  return true;
  if(p_name.Compare(_T("Synonyms"))   == 0)  return true;
  if(p_name.Compare(_T("Triggers"))   == 0)  return true;
  if(p_name.Compare(_T("Procedures")) == 0)  return true;

  return false;
}

ObjectImage
ObjectTree::TypeToImage(TCHAR p_type)
{
  if(p_type == 'T') return IMG_TABLE;
  if(p_type == 'V') return IMG_VIEW;
  if(p_type == 'U') return IMG_USERTYPE;
  if(p_type == 'C') return IMG_CATALOG;
  if(p_type == 'S') return IMG_SYNONYM;
  if(p_type == 'R') return IMG_TRIGGER;
  if(p_type == 'P') return IMG_PROCEDURE;

  return IMG_TABLES;
}

void      
ObjectTree::SetItemCount(HTREEITEM p_theItem,int p_size)
{
  // Get current text
  CString text = GetItemText(p_theItem);

  // Find primary text of the node name
  int pos = text.Find(' ');
  if(pos > 0)
  {
    text = text.Left(pos);
  }
  
  // Append count if not zero
  if(p_size > 0)
  {
    text.AppendFormat(_T(" (%d)"),p_size);
  }

  // Reformat the item text
  SetItemText(p_theItem,text);
}


// Basic item insert. Also inserts a data item, to keep track
// of the type of lookup we should do for a node
// It's in essence a 'enum treeType' pointer
HTREEITEM 
ObjectTree::InsertItem(CString p_string,HTREEITEM p_item,int p_data /*=0*/)
{
  HTREEITEM item = StyleTreeCtrl::InsertItem(p_string.GetString(),p_item);
  SetItemData(item,(DWORD_PTR) p_data);
  return item;
}

// Expanding a node, if not already done
void 
ObjectTree::OnItemExpanding(NMHDR* pNMHDR,LRESULT* pResult)
{
  // Guard against re-entrance in the UI thread
  if(m_busy)
  {
    return;
  }
  m_busy = true;

  // See if database is open
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  if(app->DatabaseIsOpen())
  {
    // See which action to take
    CWaitCursor sighAndWait;
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM theItem = pNMTreeView->itemNew.hItem;

    DWORD_PTR data = GetItemData(theItem);
    if(data)
    {
      DispatchTreeAction(data,theItem);

      Expand(theItem,TVE_EXPAND);
    }
  }
  // Ready
  m_busy = false;
}

void
ObjectTree::OnItemDoubleClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
  // Guard against re-entrance in the UI thread
  if(m_busy)
  {
    return;
  }
  m_busy = true;

  // See if database is open
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  if(app->DatabaseIsOpen())
  {
    // See which action to take
    HTREEITEM theItem = GetSelectedItem();
    DWORD_PTR data    = GetItemData(theItem);
    if(data)
    {
      DispatchTreeAction(data,theItem);
    }
  }
  // Ready
  m_busy = false;
}

bool
ObjectTree::OnTableOrView(HTREEITEM p_theItem)
{
  HTREEITEM itemSchema  = GetParentItem(p_theItem);
  HTREEITEM itemSpecial = GetParentItem(itemSchema);

  CString table   = GetItemText(p_theItem);
  CString schema  = GetItemText(itemSchema);
  CString special = GetItemText(itemSpecial);
  table.Trim();
  schema.Trim();
  special.Trim();
  if(IsSpecialNode(special))
  {
    if(special == _T("Tables") || 
       special == _T("Views")  ||
       special == _T("Catalog"))
    {
      return true;
    }
  }
  return false;
}

// From pop-up menu
void
ObjectTree::OnTableFirst(HTREEITEM p_theItem)
{
  HTREEITEM itemSchema  = GetParentItem(p_theItem);
  HTREEITEM itemSpecial = GetParentItem(itemSchema);

  CString table   = GetItemText(p_theItem);
  CString schema  = GetItemText(itemSchema);
  CString special = GetItemText(itemSpecial);
  table.Trim();
  schema.Trim();
  special.Trim();
  if(IsSpecialNode(special))
  {
    if(special == _T("Tables") || special == _T("Views") || special == _T("Catalog"))
    {
      SQLInfoDB* info = theApp.GetDatabase().GetSQLInfoDB();

      // Get quoted or unquoted names
      table = info->QueryIdentifierQuotation(table);

      if(info->GetSchemaNameUsage() & SQL_CU_DML_STATEMENTS)
      {
        if(!IsSpecialNode(schema))
        {
          schema = info->QueryIdentifierQuotation(schema);
          table  = schema + _T(".") + table;
        }
      }
      theApp.SelectFirst100(table);
    }
  }
}

void      
ObjectTree::OnTableDDL(HTREEITEM p_theItem)
{
  HTREEITEM itemSchema  = GetParentItem(p_theItem);
  HTREEITEM itemSpecial = GetParentItem(itemSchema);

  CString table   = GetItemText(p_theItem);
  CString schema  = GetItemText(itemSchema);
  CString special = GetItemText(itemSpecial);
  table.Trim();
  schema.Trim();
  special.Trim();
  if(IsSpecialNode(special))
  {
    if(special == _T("Tables") || special == _T("Views"))
    {
      theApp.TableDDL(table);
    }
  }
}

// After a 'expand' or a double-click action do this!
void
ObjectTree::DispatchTreeAction(DWORD_PTR p_action,HTREEITEM theItem)
{
  StyleToast* toast{nullptr};

  if(p_action != TREE_SOURCECODE)
  {
    toast = CreateToast(STYLE_TOAST_WARNING,STYLE_POS_MIDMIDDLE,_T("JUST A MOMENT!"),_T("Populating the object tree!"),_T(""),0);
  }

  CString text = GetItemText(theItem);
  CString type = GetObjectType(text);
  TCHAR   tt   = type.GetAt(0);

  switch(p_action)
  {
    case TREE_TABLES:       FindTables(theItem);          break;
    case TREE_TABLE:        PrepareTable(theItem,tt);     break;
    case TREE_COLUMNS:      FindColumns(theItem);         break;
    case TREE_PRIMARY:      FindPrimary(theItem);         break;
    case TREE_FOREIGN:      FindForeign(theItem);         break;
    case TREE_STATISTICS:   FindStatistics(theItem);      break;
    case TREE_SPECIALS:     FindSpecials(theItem);        break;
    case TREE_REFERENCEDBY: FindReferenced(theItem);      break;
    case TREE_TABTRIGGERS:  FindTabTriggers(theItem);     break;
    case TREE_TABSEQUENCES: FindTabSequences(theItem);    break;
    case TREE_PRIVILEGES:   FindPrivileges(theItem);      break;
    case TREE_COLPRIVILEGES:FindColumnPrivileges(theItem);break;
    case TREE_SEQUENCES:    FindSequences(theItem);       break;
    case TREE_USERTYPE:     FindUserTypes(theItem);       break;
    case TREE_TRIGGERS:     FindTriggers(theItem);        break;
    case TREE_PROCEDURES:   FindProcedures(theItem);      break;
    case TREE_PARAMETERS:   FindParameters(theItem);      break;
    case TREE_SOURCECODE:   FindSourcecode(theItem);      break;
    default:                /* NOTHING */                 break;
  }

  // Resetting the item, so we do this only **ONCE**
  if(p_action != TREE_SOURCECODE)
  {
    SetItemData(theItem,0);
    DestroyToast(toast);
  }
}

CString
ObjectTree::GetObjectType(CString p_type)
{
  switch(p_type.GetAt(0))
  {
    case _T('T'): return _T("TABLE");   break;
    case _T('V'): return _T("VIEW");    break;
    case _T('C'): return _T("CATALOG"); break;
    case _T('S'): return _T("SYNONYM"); break;
  }
  return _T("");
}

// Expand the first table in the tree
// Special case for the F12 key on a table in the editor
void
ObjectTree::ExpandFirstTable(CString p_table)
{
  // Setting the filter for a table
  SetFilter(p_table);

  HTREEITEM tables = GetRootItem();
  if(tables)
  {
    // The "Expand" below already does a 'FindTables' on basis of the set filter
    // FindTables(tables); 
    Expand(tables,TVE_EXPAND);
    HTREEITEM table = GetChildItem(tables);
    if(table)
    {
      Expand(table,TVE_EXPAND);
      HTREEITEM cols = GetChildItem(table);
      if(cols)
      {
        FindColumns(cols);
        Expand(cols,TVE_EXPAND);
      }
    }
  }
}

// Finding all table, or all tables in the filter
// Adding them to an elementary tree node 
// tables/views/synonyms etc
void
ObjectTree::FindTables(HTREEITEM p_theItem)
{
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();

  CString text = GetItemText(p_theItem);
  CString type = GetObjectType(text);

  CString catalog;
  CString schema;
  CString table;
  info->GetObjectName(m_filter,catalog,schema,table);

  MTableMap tables;
  CString   errors;
  CString   lastSchema;
  int       schemaCount = 0;
  HTREEITEM schemaItem  = 0;

  // Go  find it
  switch(type.GetAt(0))
  {
    case _T('T'): info->MakeInfoTableTable   (tables,errors,catalog,schema,table); break;
    case _T('V'): info->MakeInfoTableView    (tables,errors,catalog,schema,table); break;
    case _T('C'): info->MakeInfoTableCatalog (tables,errors,catalog,schema,table); break;
    case _T('S'): info->MakeInfoTableSynonyms(tables,errors,catalog,schema,table); break;
    default:      info->MakeInfoTableObject  (tables,errors,catalog,schema,table); break;
  }

  if(!errors.IsEmpty())
  {
    InsertItem(errors,p_theItem);
    return;
  }

  if(!tables.empty())
  {
    RemoveNoInfo(p_theItem);
  }

  // Set item count text
  SetItemCount(p_theItem,(int)tables.size());

  for(auto& theTable : tables)
  {
    HTREEITEM table = NULL;
    CString schema = theTable.m_schema;
    CString object = theTable.m_table;
    schema.Trim();
    object.Trim();

    if(!theTable.m_remarks.IsEmpty())
    {
      object.AppendFormat(_T(" (%s)"),theTable.m_remarks);
    }

    if(theTable.m_objectType.Find(_T("TEMPORARY")) >= 0)
    {
      object.AppendFormat(_T(" (%s)"),theTable.m_objectType);
    }

    if(schema.IsEmpty())
    {
      // No schema found, just add the item
      table = InsertItem(object,p_theItem);
    }
    else
    {
      if((schema.Compare(lastSchema) == 0) && schemaItem)
      {
        table = InsertItem(object,schemaItem);
        SetItemCount(schemaItem,++schemaCount);
      }
      else
      {
        lastSchema  = schema;
        schemaItem  = InsertItem(schema,p_theItem);
        table       = InsertItem(object,schemaItem);
        schemaCount = 1;
        SetItemCount(schemaItem,schemaCount);
        SetItemImage(schemaItem,IMG_SCHEMA,IMG_SCHEMA);
      }
    }
    ObjectImage image = TypeToImage(type.GetAt(0));
    SetItemImage(table,image,image);
    PrepareTable(table,type.GetAt(0));
  }
}

// Add what we can find in a table
void
ObjectTree::PrepareTable(HTREEITEM p_theItem,TCHAR p_type)
{
  HTREEITEM columns = InsertItem(_T("Columns"),p_theItem,TREE_COLUMNS);
  SetItemImage(columns,IMG_COLUMN,IMG_COLUMN);
  InsertNoInfo(columns);

  if(p_type != 'V')
  {
    HTREEITEM primary    = InsertItem(_T("Primary key"),      p_theItem,TREE_PRIMARY);
    HTREEITEM foreigns   = InsertItem(_T("Foreign keys"),     p_theItem,TREE_FOREIGN);
    HTREEITEM indices    = InsertItem(_T("Statistics"),       p_theItem,TREE_STATISTICS);
    HTREEITEM specials   = InsertItem(_T("Special columns"),  p_theItem,TREE_SPECIALS);
    HTREEITEM referenced = InsertItem(_T("Referenced by"),    p_theItem,TREE_REFERENCEDBY);
    HTREEITEM triggers   = InsertItem(_T("Triggers"),         p_theItem,TREE_TABTRIGGERS);
    HTREEITEM sequences  = InsertItem(_T("Sequences"),        p_theItem,TREE_TABSEQUENCES);

    SetItemImage(primary,   IMG_PRIMARY, IMG_PRIMARY);
    SetItemImage(foreigns,  IMG_FOREIGN, IMG_FOREIGN);
    SetItemImage(indices,   IMG_INDEX,   IMG_INDEX);
    SetItemImage(specials,  IMG_COLUMN,  IMG_COLUMN);
    SetItemImage(referenced,IMG_FOREIGN, IMG_FOREIGN);
    SetItemImage(triggers,  IMG_TRIGGER, IMG_TRIGGER);
    SetItemImage(sequences, IMG_SEQUENCE,IMG_SEQUENCE);

    InsertNoInfo(primary);
    InsertNoInfo(foreigns);
    InsertNoInfo(indices);
    InsertNoInfo(specials);
    InsertNoInfo(referenced);
    InsertNoInfo(triggers);
    InsertNoInfo(sequences);
  }

  HTREEITEM tabaccess  = InsertItem(_T("Table privileges"), p_theItem,TREE_PRIVILEGES);
  HTREEITEM colaccess  = InsertItem(_T("Column privileges"),p_theItem,TREE_COLPRIVILEGES);

  SetItemImage(tabaccess,IMG_ACCESS,IMG_ACCESS);
  SetItemImage(colaccess,IMG_ACCESS,IMG_ACCESS);

  InsertNoInfo(tabaccess);
  InsertNoInfo(colaccess);
}

// Before expanding a node, find the table again
bool
ObjectTree::PresetTable(HTREEITEM p_theItem)
{
  HTREEITEM itemTable  = GetParentItem(p_theItem);
  HTREEITEM itemSchema = GetParentItem(itemTable);

  CString table  = GetItemText(itemTable);
  CString schema = GetItemText(itemSchema);
  table.Trim();
  schema.Trim();
  IsSpecialNode(table);
  if(!IsSpecialNode(schema))
  {
    m_schema = schema;
  }
  m_table = table;
  return true;
}

// Before expanding a node, find the procedure again
bool
ObjectTree::PresetProcedure(HTREEITEM p_theItem,MProcedureMap& p_procedures)
{
  HTREEITEM itemProcedure = GetParentItem(p_theItem);
  HTREEITEM itemSchema    = GetParentItem(itemProcedure);

  CString procedure = GetItemText(itemProcedure);
  CString schema    = GetItemText(itemSchema);
  procedure.Trim();
  schema.Trim();
  CString findproc(procedure);
  if(!IsSpecialNode(schema))
  {
    findproc = schema + _T(".") + procedure;
    m_schema = schema;
  }
  m_procedure = procedure;

  return true;
}

// Find all the columns in a table
void
ObjectTree::FindColumns(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    // Go find the columns
    MColumnMap columns;
    CString    errors;
    CString    catalog;
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableColumns(columns,errors,catalog,m_schema,m_table);
    ColumnListToTree(columns,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Find all the primary keys in a table
// 0 or 1 is all we can find :-)
void
ObjectTree::FindPrimary(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MPrimaryMap primaries;
    CString     errors;
    CString     catalog;

    // Go find the primary key
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTablePrimary(primaries,errors,catalog,m_schema,m_table);
    PrimariesToTree(primaries,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Find all foreign keys on this table
// pointing to other tables
void
ObjectTree::FindForeign(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MForeignMap foreigns;
    CString     errors;
    CString     catalog;

    // Go find the foreign keys
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableForeign(foreigns,errors,catalog,m_schema,m_table);
    ForeignsToTree(foreigns,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Finding table statistics
// and indices for the table
void
ObjectTree::FindStatistics(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MIndicesMap statistics;
    CString errors;
    CString catalog;

    // Go find the indices and statistics
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableStatistics(statistics,errors,catalog,m_schema,m_table,nullptr);
    StatisticsToTree(statistics,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Finding special columns like 'ROWID' (oracle) or 'OID' (PostgreSQL)
void      
ObjectTree::FindSpecials(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MSpecialsMap specials;
    CString errors;
    CString catalog;

    // Go find the special columns
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableSpecials(specials,errors,catalog,m_schema,m_table);
    SpecialsToTree(specials,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Find all foreign keys from OTHER tables
// pointing to this table
void
ObjectTree::FindReferenced(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MForeignMap foreigns;
    CString     errors;
    CString     catalog;

    // Go find the referencing tables
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableForeign(foreigns,errors,catalog,m_schema,m_table,true);
    ReferencedToTree(foreigns,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Find all triggers on the table
void
ObjectTree::FindTabTriggers(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MTriggerMap triggers;
    CString     errors;
    CString     catalog;

    // Go find the triggers
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableTriggers(triggers,errors,catalog,m_schema,m_table);
    TriggersToTree(triggers,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Finding all sequences with a name that looks alike the tablename
// Search is done on a LIKE '%tablename%'
void
ObjectTree::FindTabSequences(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MSequenceMap sequences;
    CString      errors;
    CString     catalog;

    // Go find the sequences
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableSequences(sequences,errors,catalog,m_schema,m_table + _T("%"));
    SequencesToTree(sequences,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Finding all table privileges on the table
void
ObjectTree::FindPrivileges(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MPrivilegeMap privileges;
    CString errors;
    CString     catalog;

    // Go find the privileges on the table
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTablePrivileges(privileges,errors,catalog,m_schema,m_table);
    PrivilegesToTree(privileges,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Finding all column privileges on the table columns
void
ObjectTree::FindColumnPrivileges(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MPrivilegeMap privileges;
    CString errors;
    CString     catalog;

    // Go find the privileges on the table
    app->GetDatabase().GetSQLInfoDB()->MakeInfoColumnPrivileges(privileges,errors,catalog,m_schema,m_table);
    ColumnPrivilegesToTree(privileges,p_theItem);
    AddErrorsToTree(errors,p_theItem);
  }
}

// Finding all sequences
void
ObjectTree::FindSequences(HTREEITEM p_theItem)
{
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();
  HTREEITEM    schemaItem  = NULL;
  int          schemaCount = 0;
  MSequenceMap sequences;
  CString      errors;
  CString      lastSchema;

  // Find filtered name
  CString catalog;
  CString schema;
  CString table;
  info->GetObjectName(m_filter,catalog,schema,table);

  // Get relevant sequences
  info->MakeInfoTableSequences(sequences,errors,catalog,schema,table);
  if(!errors.IsEmpty())
  {
    StyleMessageBox(this,errors,_T("Querytool"),MB_OK | MB_ICONERROR);
    return;
  }

  // See if we have something
  if(!sequences.empty())
  {
    RemoveNoInfo(p_theItem);
  }

  // Setting count of objects
  SetItemCount(p_theItem,(int)sequences.size());

  for(auto& seq : sequences)
  {
    HTREEITEM sequence = NULL;
    CString schema = seq.m_schemaName;
    CString object = seq.m_sequenceName;
    schema.Trim();
    object.Trim();

    if(schema.IsEmpty())
    {
      // No schema found, just add the item
      sequence = InsertItem(object,p_theItem);
    }
    else
    {
      if((schema.Compare(lastSchema) == 0) && schemaItem)
      {
        sequence = InsertItem(object,schemaItem);
        SetItemCount(schemaItem,++schemaCount);
      }
      else
      {
        lastSchema = schema;
        schemaItem = InsertItem(schema,p_theItem);
        sequence   = InsertItem(object,schemaItem);
        schemaCount = 1;
        SetItemCount(schemaItem,schemaCount);
        SetItemImage(schemaItem,IMG_SCHEMA,IMG_SCHEMA);
      }
    }
    SetItemImage(sequence,IMG_SEQUENCE,IMG_SEQUENCE);

    // All info on the item
    CString line;
    line.Format(_T("Minimum value: %-12.0f"),seq.m_minimalValue);
    HTREEITEM item = InsertItem(line,sequence);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Current value: %-12.0f"),seq.m_currentValue);
    item = InsertItem(line,sequence);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Increment with: %d"),seq.m_increment);
    item = InsertItem(line,sequence);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Caching size: %d"),seq.m_cache);
    item = InsertItem(line,sequence);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Re-Cycling of sets: %s"),seq.m_cycle ? _T("yes") : _T("no"));
    item = InsertItem(line,sequence);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Cluster ordering: %s"),seq.m_order ? _T("yes") : _T("no"));
    item = InsertItem(line,sequence);
    SetItemImage(item,IMG_INFO,IMG_INFO);
  }
}

void
ObjectTree::FindUserTypes(HTREEITEM p_theItem)
{
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB*   info = app->GetDatabase().GetSQLInfoDB();
  HTREEITEM    schemaItem = NULL;
  int          schemaCount = 0;
  MUserTypeMap types;
  CString      errors;
  CString      lastSchema;

  // Find filtered name
  CString catalog;
  CString schema;
  CString type;
  info->GetObjectName(m_filter,catalog,schema,type);

  // Find relevant user types
  info->MakeInfoUserTypes(types,errors,catalog,schema,type);
  if(!errors.IsEmpty())
  {
    StyleMessageBox(this,errors,_T("Querytool"),MB_OK | MB_ICONERROR);
    return;
  }

  // See if we have something
  if(!types.empty())
  {
    RemoveNoInfo(p_theItem);
  }

  int numberOfTypes = 0;
  for(unsigned item = 0;item < (unsigned) types.size();++item)
  {
    MetaUserType* typ = &types[item];
    if(typ->m_ordinal == 1)
    {
      ++numberOfTypes;

      HTREEITEM usertype = NULL;
      CString schema = typ->m_schemaName;
      CString object = typ->m_typeName;
      schema.Trim();
      object.Trim();

      if(schema.IsEmpty())
      {
        // No schema found, just add the item
        usertype = InsertItem(object,p_theItem);
      }
      else
      {
        if((schema.Compare(lastSchema) == 0) && schemaItem)
        {
          usertype = InsertItem(object,schemaItem);
          SetItemCount(schemaItem,++schemaCount);
        }
        else
        {
          lastSchema = schema;
          schemaItem = InsertItem(schema,p_theItem);
          usertype   = InsertItem(object,schemaItem);
          schemaCount = 1;
          SetItemCount(schemaItem,schemaCount);
          SetItemImage(schemaItem,IMG_SCHEMA,IMG_SCHEMA);
        }
      }
      SetItemImage(usertype,IMG_USERTYPE,IMG_USERTYPE);

      // Set in the tree
      UserTypeToTree(types,usertype,item);
    }
  }
  // Setting count of objects
  SetItemCount(p_theItem,numberOfTypes);
}

void
ObjectTree::FindTriggers(HTREEITEM p_theItem)
{
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();
  HTREEITEM   schemaItem = NULL;
  int         schemaCount = 0;
  MTriggerMap triggers;
  CString     errors;
  CString     lastSchema;

  // Find filtered name
  CString catalog;
  CString schema;
  CString table;
  info->GetObjectName(m_filter,catalog,schema,table);

  // Find relevant triggers
  info->MakeInfoTableTriggers(triggers,errors,schema,table,_T(""));
  if(!errors.IsEmpty())
  {
    StyleMessageBox(this,errors,_T("Querytool"),MB_OK | MB_ICONERROR);
    return;
  }

  // See if we have something
  if(!triggers.empty())
  {
    RemoveNoInfo(p_theItem);
  }

  // Setting count of objects
  SetItemCount(p_theItem,(int)triggers.size());

  for(auto& trig : triggers)
  {
    HTREEITEM trigger = NULL;
    CString schema = trig.m_schemaName;
    CString object = trig.m_triggerName;
    schema.Trim();
    object.Trim();

    if(schema.IsEmpty())
    {
      // No schema found, just add the item
      trigger = InsertItem(object,p_theItem);
    }
    else
    {
      if((schema.Compare(lastSchema) == 0) && schemaItem)
      {
        trigger = InsertItem(object,schemaItem);
        SetItemCount(schemaItem,++schemaCount);
      }
      else
      {
        lastSchema = schema;
        schemaItem = InsertItem(schema,p_theItem);
        trigger    = InsertItem(object,schemaItem);
        schemaCount = 1;
        SetItemCount(schemaItem,schemaCount);
        SetItemImage(schemaItem,IMG_SCHEMA,IMG_SCHEMA);
      }
    }
    SetItemImage(trigger,IMG_TRIGGER,IMG_TRIGGER);

    // Set in the tree
    TriggerToTree(trig,trigger);
  }
}

// Finding all procedures according to the filter, or all
void
ObjectTree::FindProcedures(HTREEITEM p_theItem)
{
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();
  MProcedureMap procedures;
  CString   errors;
  CString   lastSchema;
  int       schemaCount = 0;
  HTREEITEM schemaItem  = NULL;

  // Find filtered name
  CString catalog;
  CString schema;
  CString table;
  info->GetObjectName(m_filter, catalog, schema, table);

  try
  {
    // Go find the procedures
    info->MakeInfoPSMProcedures(procedures,errors,catalog,schema,table);
  }
  catch(CString& er)
  {
    StyleMessageBox(this,er,_T("Querytool"),MB_OK | MB_ICONERROR);
    return;
  }
  // See if we have something
  if(!procedures.empty())
  {
    RemoveNoInfo(p_theItem);
  }

  // Setting count of objects
  SetItemCount(p_theItem,(int)procedures.size());
  
  for(auto& proc : procedures)
  {
    HTREEITEM procedure = NULL;
    CString schema = proc.m_schemaName;
    CString object = proc.m_procedureName;
    schema.Trim();
    object.Trim();

    if(schema.IsEmpty())
    {
      // No schema found, just add the item
      procedure = InsertItem(object,p_theItem);
    }
    else
    {
      if((schema.Compare(lastSchema) == 0) && schemaItem)
      {
        procedure = InsertItem(object,schemaItem);
        SetItemCount(schemaItem,++schemaCount);
      }
      else
      {
        lastSchema  = schema;
        schemaItem  = InsertItem(schema,p_theItem);
        procedure   = InsertItem(object,schemaItem);
        schemaCount = 1;
        SetItemCount(schemaItem,schemaCount);
        SetItemImage(schemaItem,IMG_SCHEMA,IMG_SCHEMA);
      }
    }
    SetItemImage(procedure,IMG_PROCEDURE,IMG_PROCEDURE);

    // Insert the info and parameters nodes
    HTREEITEM information = InsertItem(_T("Information"),procedure,TREE_PARAMETERS);
    HTREEITEM parameters  = InsertItem(_T("Parameters"), procedure,TREE_PARAMETERS);

    SetItemImage(information,IMG_INFO,     IMG_INFO);
    SetItemImage(parameters, IMG_PARAMETER,IMG_PARAMETER);

    InsertNoInfo(information);
    InsertNoInfo(parameters);
  }
}

// Finding the procedure parameters for the procedure
void
ObjectTree::FindParameters(HTREEITEM p_theItem)
{
  MProcedureMap procedures;
  if(PresetProcedure(p_theItem,procedures))
  {
    // Go find the procedures
    QueryToolApp* app = (QueryToolApp*)AfxGetApp();
    MProcedureMap procedures;
    CString   errors;
    CString     catalog;

    app->GetDatabase().GetSQLInfoDB()->MakeInfoPSMProcedures(procedures,errors,catalog,m_schema,m_procedure);
    if(procedures.empty())
    {
      return;
    }
    MetaProcedure& procedure = procedures.front();
    CString   line;
    HTREEITEM proc  = GetParentItem(p_theItem);
    HTREEITEM info  = GetChildItem(proc);
    HTREEITEM param = GetNextItem(info,TVGN_NEXT);

    // procedure type + return mechanism
    if(!procedures.empty())
    {
      RemoveNoInfo(info);
    }

    line = _T("PSM type: ");
    switch(procedure.m_procedureType)      
    {
      case SQL_PT_PROCEDURE:  line += _T("PROCEDURE"); break;
      case SQL_PT_FUNCTION:   line += _T("FUNCTION");  break;
      case SQL_PT_UNKNOWN:    // Fall through
      default:                line += _T("UNKNOWN");   break;
    }
    HTREEITEM item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Input parameters: %d"), procedure.m_inputParameters);
    item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);
    
    line.Format(_T("Output parameters: %d"),procedure.m_outputParameters);
    item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);
    
    line.Format(_T("Result sets: %d"),      procedure.m_resultSets);
    item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Remarks: %s"),procedure.m_remarks);
    item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    // Source
    CString name = procedure.m_schemaName + _T(".") + m_procedure;
    m_source.insert(std::make_pair(name, procedure.m_source));
    line.Format(_T("SQL Source: %d characters"), procedure.m_source.GetLength());
    item = InsertItem(line,info,TREE_SOURCECODE);
    SetItemImage(item,IMG_SOURCE,IMG_SOURCE);

    // Go find the parameters for the procedure
    MParameterMap parameters;
    app->GetDatabase().GetSQLInfoDB()->MakeInfoPSMParameters(parameters,errors,catalog,procedure.m_schemaName,m_procedure);
    ParametersToTree(parameters,param);
    AddErrorsToTree(errors,p_theItem);

    // Reset BOTH node data pointers
    SetItemData(info,0);
    SetItemData(param,0);
  }
}

void
ObjectTree::FindSourcecode(HTREEITEM p_theItem)
{
  CString tableproc;
  MProcedureMap procedures;
  if (PresetProcedure(p_theItem,procedures))
  {
    // For triggers it's the first node
    if(ShowSourcecodeTrigger(m_schema,m_procedure))
    {
      return;
    }
    tableproc = m_procedure;
    // For procedures, it's a node deeper in the tree
    HTREEITEM par = GetParentItem(p_theItem);
    if(PresetProcedure(par,procedures))
    {
      if(ShowSourcecodeModule(m_schema,m_procedure))
      {
        return;
      }
    }
    // For user types
    if(ShowSourcecodeUserType(m_schema,tableproc))
    {
      return;
    }
    // For trigger under tables, it's even a node deeper
    par = GetParentItem(par);
    if(PresetProcedure(par,procedures))
    {
      if(ShowSourcecodeTrigger(m_schema,tableproc))
      {
        return;
      }
    }
  }
}

bool
ObjectTree::ShowSourcecodeTrigger(CString p_schema,CString p_trigger)
{
  CString errors;
  CString object;
  CString source;
  CString catalog;
  MTriggerMap triggers;
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();

  if(info->MakeInfoTableTriggers(triggers,errors,catalog,m_schema,object,p_trigger))
  {
    source = info->GetCATALOGTriggerCreate(triggers[0]);
  }
  else
  {
    source.Format(_T("Internal error reading trigger source of %s.%s\n"),m_schema.GetString(),p_trigger.GetString());
    return false;
  }
  // Prepare for MFC edit control
  source.Replace(_T("\n"),_T("\r\n"));

  // Show in source code popup
  CNativeSQLDlg dlg(this,source,_T("Persistent Stored Module"));
  dlg.DoModal();
  return true;
}

bool
ObjectTree::ShowSourcecodeModule(CString p_schema, CString p_procedure)
{
  CString name = p_schema + _T(".") + p_procedure;
  SourceList::iterator it = m_source.find(name);
  if (it != m_source.end())
  {
    CString source = it->second;
    if(source == _T("<@>") || source.IsEmpty())
    {
      CString catalog;
      QueryToolApp* app = (QueryToolApp*)AfxGetApp();
      SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();
      source = info->MakeInfoPSMSourcecode(catalog,p_schema,p_procedure);
    }

    // Prepare for MFC edit control
    source.Replace(_T("\n"),_T("\r\n"));

    // Show in source code popup
    CNativeSQLDlg dlg(this,source,_T("Persistent Stored Module"));
    dlg.DoModal();
    return true;
  }
  return false;
}

bool
ObjectTree::ShowSourcecodeUserType(CString p_schema,CString p_typename)
{
  CString name = p_schema + _T(".") + p_typename;
  SourceList::iterator it = m_source.find(name);
  if(it != m_source.end())
  {
    CString source = it->second;

    // Prepare for MFC edit control
    source.Replace(_T("\n"),_T("\r\n"));

    // Show in source code popup
    CNativeSQLDlg dlg(this,source,_T("User defined type"));
    dlg.DoModal();
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// FILLING THE TREE
//
//////////////////////////////////////////////////////////////////////////

void
ObjectTree::ColumnDetailsToTree(MetaColumn& p_column,HTREEITEM p_item)
{
  CString text;

  text.Format(_T("Datatype SQL = %d\n"),      p_column.m_datatype);      InsertItem(text,p_item);
  text.Format(_T("Name datatype = %s\n"),     p_column.m_typename);      InsertItem(text,p_item);
  text.Format(_T("Column size = %d\n"),       p_column.m_columnSize);    InsertItem(text,p_item);
  text.Format(_T("Buffer length = %u\n"),     p_column.m_bufferLength);  InsertItem(text,p_item);
  text.Format(_T("Decimal digits = %d\n"),    p_column.m_decimalDigits); InsertItem(text,p_item);
  text.Format(_T("radix = %d\n"),             p_column.m_numRadix);      InsertItem(text,p_item);
  text.Format(_T("nullable = %d\n"),          p_column.m_nullable);      InsertItem(text,p_item);
  text.Format(_T("remarks = %s\n"),           p_column.m_remarks);       InsertItem(text,p_item);
  text.Format(_T("Default value = %s\n"),     p_column.m_default);       InsertItem(text,p_item);
  text.Format(_T("Datatype version 3 = %d\n"),p_column.m_datatype3);     InsertItem(text,p_item);
  text.Format(_T("Datatype sub = %d\n"),      p_column.m_sub_datatype);  InsertItem(text,p_item);
  text.Format(_T("Octet length = %d\n"),      p_column.m_octet_length);  InsertItem(text,p_item);
  text.Format(_T("Ordinal position = %d\n"),  p_column.m_position);      InsertItem(text,p_item);
  text.Format(_T("Is nullable = %s\n"),       p_column.m_isNullable);    InsertItem(text,p_item);
}

void
ObjectTree::ColumnListToTree(MColumnMap& p_columns,HTREEITEM p_item)
{
  // We now have information. Remove the 'No information' marker
  if(!p_columns.empty())
  {
    RemoveNoInfo(p_item);
  }

  // Process columns
  for(auto& column : p_columns)
  {
    CString line  = column.m_column + _T(" ") + column.m_typename;
    if((column.m_columnSize > 0) && (column.m_typename.Find('(') < 0))
    {
      line.AppendFormat(_T(" (%d"),column.m_columnSize);
      if(column.m_decimalDigits)
      {
        line.AppendFormat(_T(",%d)"),column.m_decimalDigits);
      }
      else
      {
        line += _T(")");
      }
    }
    switch(column.m_nullable)
    {
      case SQL_NO_NULLS:          line += _T(" NOT NULL"); 
                                  break;
      case SQL_NULLABLE:          break;  
      default:                    // Fall through
      case SQL_NULLABLE_UNKNOWN:  line += _T(" NULLABLE UNKNOWN");
                                  break;
    }

    if(!column.m_remarks.IsEmpty())
    {
      line.AppendFormat(_T(" (%s)"),column.m_remarks);
    }
    HTREEITEM item = InsertItem(line,p_item);
    SetItemImage(item,IMG_COLUMN,IMG_COLUMN);

    ColumnDetailsToTree(column,item);
  }
}

void
ObjectTree::PrimariesToTree(MPrimaryMap& p_primaries,HTREEITEM p_item)
{
  // Check for a primary key
  if(p_primaries.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  // Insert primary constraint name
  CString name(_T("Constraint name: "));
  name += p_primaries.front().m_constraintName;
  HTREEITEM nameItem = InsertItem(name,p_item);
  SetItemImage(nameItem,IMG_PRIMARY,IMG_PRIMARY);
  
  // Insert primary column names
  for(auto& prim : p_primaries)
  {
    HTREEITEM colItem = InsertItem(prim.m_columnName,nameItem);
    SetItemImage(colItem,IMG_COLUMN,IMG_COLUMN);
  }
}

CString
ObjectTree::ForeignRuleToString(int p_rule)
{
  CString text;
  switch(p_rule)
  {
    case SQL_CASCADE:     text = _T("CASCADE");     break;
    case SQL_NO_ACTION:   text = _T("NO ACTION");   break;
    case SQL_SET_NULL:    text = _T("SET NULL");    break;
    case SQL_SET_DEFAULT: text = _T("SET DEFAULT"); break;
    default:              text = _T("No info");     break;
  }
  return text;
}

CString
ObjectTree::DeferrableToString(int p_defer)
{
  CString text;
  switch(p_defer)
  {
    case SQL_INITIALLY_DEFERRED:  text = _T("INITIALLY DEFERRED");  break;
    case SQL_INITIALLY_IMMEDIATE: text = _T("INITIALLY IMMEDIATE"); break;
    case SQL_NOT_DEFERRABLE:      text = _T("NOT DEFERRABLE");      break;
    default:                      text = _T("No Info");             break;
  }
  return text;
}

void
ObjectTree::ForeignsToTree(MForeignMap& p_foreigns,HTREEITEM p_item)
{
  // Check for a foreign keys
  if(p_foreigns.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  // Currently dealing with foreign key item
  HTREEITEM keyItem = NULL;
  HTREEITEM info    = NULL;
  HTREEITEM primkey = NULL;
  CString   line;

  // Walk through all the foreign-key columns
  for(auto& foreign : p_foreigns)
  {
    if(foreign.m_keySequence == 1)
    {
      // Start a new foreign key
      line  = _T("Constraint name: ");
      line += foreign.m_foreignConstraint;
      keyItem = InsertItem(line,p_item);
      SetItemImage(keyItem,IMG_FOREIGN,IMG_FOREIGN);

      // Update rule
      line = _T("Update rule: ") + ForeignRuleToString(foreign.m_updateRule);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Delete rule
      line = _T("Delete rule: ") + ForeignRuleToString(foreign.m_deleteRule);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // deferrable
      line = _T("Deferrable: ") + DeferrableToString(foreign.m_deferrable);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Primary key + columns
      line.Format(_T("Primary key: %s (%s)")
                  ,foreign.m_pkTableName
                  ,foreign.m_primaryConstraint);
      primkey = InsertItem(line,keyItem);
      SetItemImage(primkey,IMG_FOREIGN,IMG_FOREIGN);
    }
    // Add the columns
    line.Format(_T("%s -> %s"),foreign.m_fkColumnName,foreign.m_pkColumnName);
    HTREEITEM col = InsertItem(line,primkey);
    SetItemImage(col,IMG_COLUMN,IMG_COLUMN);
  }
}

void
ObjectTree::ReferencedToTree(MForeignMap& p_foreigns,HTREEITEM p_item)
{
  // Check for a referenced tables
  if(p_foreigns.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  // Currently dealing with foreign key item
  HTREEITEM keyItem = NULL;
  HTREEITEM info    = NULL;
  HTREEITEM primkey = NULL;
  CString   line;

  // Walk through all the foreign-key columns
  for(auto& foreign : p_foreigns)
  {
    if(foreign.m_keySequence == 1)
    {
      // Start a new table
      line  = _T("Table: ");
      line += foreign.m_fkTableName;
      keyItem = InsertItem(line,p_item);
      SetItemImage(keyItem,IMG_FOREIGN,IMG_FOREIGN);

      // Update rule
      line = _T("Update rule: ") + ForeignRuleToString(foreign.m_updateRule);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Delete rule
      line = _T("Delete rule: ") + ForeignRuleToString(foreign.m_deleteRule);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Deferrable
      line = _T("Deferrable: ") + DeferrableToString(foreign.m_deferrable);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Primary key + columns
      line.Format(_T("Foreign key: %s"),foreign.m_foreignConstraint);
      primkey = InsertItem(line,keyItem);
      SetItemImage(primkey,IMG_FOREIGN,IMG_FOREIGN);
    }
    // Add the columns
    line.Format(_T("%s -> %s"),foreign.m_fkColumnName,foreign.m_pkColumnName);
    HTREEITEM col = InsertItem(line,primkey);
    SetItemImage(col,IMG_COLUMN,IMG_COLUMN);
  }
}

void
ObjectTree::StatisticsToTree(MIndicesMap& p_statistics,HTREEITEM p_item)
{
  // Check for a indices
  if(p_statistics.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  HTREEITEM next = NULL;
  CString   line;
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();

  // Process statistics
  for(auto& stat : p_statistics)
  {
    if(stat.m_position <= 1)
    {
      // Start new index/table node
      if(stat.m_indexType == SQL_TABLE_STAT)
      {
        // Process whole table statistics
        next = InsertItem(_T("[Table statistics]"),p_item);
      }
      else
      {
        // New index on a table
        line.Format(_T("Index name: %s"),stat.m_indexName);
        if(stat.m_nonunique == false)
        {
          line += _T(" (Unique)");
        }
        if(stat.m_indexType == SQL_INDEX_CLUSTERED)
        {
          line += _T(" (Clustered)");
        }
        if(stat.m_indexType == SQL_INDEX_HASHED)
        {
          line += _T(" (Hashed)");
        }
        next = InsertItem(line,p_item);
      }
      SetItemImage(next,IMG_INDEX,IMG_INDEX);

      // Table/index nodes have cardinality and pages
      line.Format(_T("Cardinality: %d"),stat.m_cardinality);
      HTREEITEM item = InsertItem(line,next);
      SetItemImage(item,IMG_INDEX,IMG_INDEX);
      line.Format(_T("Data pages: %d"),stat.m_pages);
      item = InsertItem(line,next);
      SetItemImage(item,IMG_INDEX,IMG_INDEX);
      line.Format(_T("Index filter: %s"),stat.m_filter);
      item = InsertItem(line,next);
      SetItemImage(item,IMG_INDEX,IMG_INDEX);
    }
    if(stat.m_indexType != SQL_TABLE_STAT)
    {
      // Optionally get the filter
      if(stat.m_filter.IsEmpty())
      {
        stat.m_filter = info->GetCATALOGIndexFilter(stat);
      }

      // Extra column on an index
      if(!stat.m_filter.IsEmpty())
      {
        line.Format(_T("%d: %s"),stat.m_position,stat.m_filter);
      }
      else
      {
        line.Format(_T("%d: %s"),stat.m_position,stat.m_columnName);
      }
      line += (stat.m_ascending == _T("A")) ? _T(" (Ascending)") : _T(" (Descending)");
      HTREEITEM item = InsertItem(line,next);
      SetItemImage(item,IMG_COLUMN,IMG_COLUMN);
    }
  }
}

void
ObjectTree::SpecialsToTree(MSpecialsMap& p_specials,HTREEITEM p_item)
{
  // Check for special columns
  if(p_specials.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  HTREEITEM next = NULL;
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();

  // Inserts the special columns
  for(auto& special : p_specials)
  {
    CString line = _T("Columnname: ") + special.m_columnName;
    next = InsertItem(line,p_item);
    SetItemImage(next,IMG_COLUMN,IMG_COLUMN);

    line.Format(_T("RDBMS Special type: %s"),special.m_typeName);
    HTREEITEM item  = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("ODBC data type: %s"),info->ODBCDataType(special.m_datatype));
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Column size: %d"),special.m_columnSize);
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Buffer size: %d"),special.m_bufferSize);
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Decimal digits: %d"),special.m_decimalDigits);
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line = _T("Column scope: ");
    switch(special.m_scope)
    {
      case SQL_SCOPE_CURROW:      line += _T("Current row"); break;
      case SQL_SCOPE_TRANSACTION: line += _T("Transaction"); break;
      case SQL_SCOPE_SESSION:     line += _T("Session");     break;
      default:                    line += _T("Unknown");     break;
    }
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line = _T("Column scope: ");
    switch(special.m_pseudo)
    {
      case SQL_PC_NON_PSEUDO: line += _T("Non-pseudo");    break;
      case SQL_PC_PSEUDO:     line += _T("Pseudo column"); break;
      default:                line += _T("No info");       break;
    }
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);
  }
}

void
ObjectTree::TriggersToTree(MTriggerMap& p_triggers,HTREEITEM p_item)
{
  // Check if we have triggers
  if(p_triggers.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  for(auto& trigger : p_triggers)
  {
    // Name of the trigger + position in firing (0 = no order)
    HTREEITEM trigItem = InsertItem(trigger.m_triggerName,p_item);
    SetItemImage(trigItem,IMG_TRIGGER,IMG_TRIGGER);
    TriggerToTree(trigger,trigItem);
  }
}

void
ObjectTree::UserTypeToTree(MUserTypeMap& p_types,HTREEITEM p_item,unsigned p_index)
{
  // Get the main item (ordinal = 1)
  MetaUserType* type = &p_types[p_index];
  CString line;

  CString usageText;
  TCHAR usage = type->m_usage[0];
  switch(usage)
  {
    case 'D': usageText = _T("Domain value type"); break;
    case 'C': usageText = _T("Composite type");    break;
    case 'E': usageText = _T("Enumerator type");   break;
  }

  // Type of user-type
  line = _T("UserType is a: ") + usageText;
  HTREEITEM item = InsertItem(line,p_item);
  SetItemImage(item,IMG_USERTYPE,IMG_USERTYPE);

  // Remarks
  line = _T("Comment on type is: ") + type->m_remarks;
  item = InsertItem(line,p_item);
  SetItemImage(item,IMG_USERTYPE,IMG_USERTYPE);

  if(usage == 'D')
  {
    // Datatype
    line = _T("Domain base datatype: ") + type->m_datatype;
    item = InsertItem(line,p_item);
    SetItemImage(item,IMG_USERTYPE,IMG_USERTYPE);
    // NOT NULL
    line.Format(_T("Domain forces not-null: %s"),type->m_notnull ? _T("true") : _T("false"));
    item = InsertItem(line,p_item);
    SetItemImage(item,IMG_USERTYPE,IMG_USERTYPE);
    // DEFAULT
    line = _T("Domain default value: ") + type->m_default;
    item = InsertItem(line,p_item);
    SetItemImage(item,IMG_USERTYPE,IMG_USERTYPE);
    // CHECK
    line = _T("Domain check value: ") + type->m_domaincheck;
    item = InsertItem(line,p_item);
    SetItemImage(item,IMG_USERTYPE,IMG_USERTYPE);
  }
  else
  {
    line.Format(_T("Usertype %s"),(usage == 'C') ? _T("COLUMNS") : _T("ENUMERATOR VALUES"));
    item = InsertItem(line,p_item);
    SetItemImage(item,IMG_USERTYPE,IMG_USERTYPE);

    if(usage == 'C')
    {
      UserColumnsToTree(p_types,item,p_index);
    }
    if(usage == 'E')
    {
      UserEnumeratorsToTree(p_types,item,p_index);
    }
  }
  // Source
  CString name = type->m_schemaName + _T(".") + type->m_typeName;
  m_source.insert(std::make_pair(name,type->m_source));
  line.Format(_T("SQL Source: %d characters"),type->m_source.GetLength());
  item = InsertItem(line,p_item,TREE_SOURCECODE);
  SetItemImage(item,IMG_SOURCE,IMG_SOURCE);
}

void
ObjectTree::UserColumnsToTree(MUserTypeMap& p_types,HTREEITEM p_item,unsigned p_index)
{
  bool next(true);
  MetaUserType* type = &p_types[p_index];
  CString line;

  do 
  {
    // Attribute
    HTREEITEM colitem = InsertItem(type->m_attribute,p_item);
    SetItemImage(colitem,IMG_COLUMN,IMG_COLUMN);
    // Datatype
    line = _T("Domain base datatype: ") + type->m_datatype;
    HTREEITEM item = InsertItem(line,colitem);
    // NOT NULL
    line.Format(_T("Domain forces not-null: %s"),type->m_notnull ? _T("true") : _T("false"));
    item = InsertItem(line,colitem);
    // DEFAULT
    line = _T("Domain default value: ") + type->m_default;
    item = InsertItem(line,colitem);
    // CHECK
    line = _T("Domain check value: ") + type->m_domaincheck;
    item = InsertItem(line,colitem);

    // Last item in the collection reached
    if(p_index == unsigned (p_types.size() - 1))
    {
      break;
    }
    type = &p_types[++p_index];
    if(type->m_ordinal == 1)
    {
      next = false;
    }
  } 
  while(next);
}

void
ObjectTree::UserEnumeratorsToTree(MUserTypeMap& p_types,HTREEITEM p_item,unsigned p_index)
{
  bool next(true);
  MetaUserType* type = &p_types[p_index];
  CString line;

  do 
  {
    line = _T("Enum: ") + type->m_attribute;
    InsertItem(line,p_item);

    // Last item in the collection reached
    if(p_index == unsigned(p_types.size() - 1))
    {
      break;
    }
    type = &p_types[++p_index];
    if(type->m_ordinal == 1)
    {
      next = false;
    }
  } 
  while(next);
}

void
ObjectTree::TriggerToTree(MetaTrigger& p_trigger,HTREEITEM p_item)
{
  CString line;

  // Active trigger
  line.Format(_T("Trigger is: %s"),p_trigger.m_enabled ? _T("enabled") : _T("disabled"));
  HTREEITEM item = InsertItem(line,p_item);
  SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);

  // Type before/after
  line.Format(_T("Trigger fires: %s"),p_trigger.m_before ? _T("before") : _T("after"));
  item = InsertItem(line,p_item);
  SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);

  // Trigger currently enabled
  line.Format(_T("Trigger enabled: %s"),p_trigger.m_enabled ? _T("yes") : _T("no"));
  item = InsertItem(line,p_item);
  SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);

  // Ordering of firing
  if(p_trigger.m_position > 0)
  {
    line.Format(_T("Trigger order: %d"),p_trigger.m_position);
  }
  else
  {
    line = _T("Trigger order: indetermined");
  }
  item = InsertItem(line,p_item);
  SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);

  // Remarks
  line = _T("Remarks on trigger: ") + p_trigger.m_remarks;
  item = InsertItem(line,p_item);
  SetItemImage(item, IMG_TRIGGER, IMG_TRIGGER);

  // Fires on INSERT/UPDATE/DELETE/SELECT
  line = _T("Trigger on DML: ");
  if(p_trigger.m_insert)      line += _T("insert,");
  if(p_trigger.m_update)      line += _T("update,");
  if(p_trigger.m_delete)      line += _T("delete,");
  if(p_trigger.m_select)      line += _T("select,");
  if(p_trigger.m_session)     line += _T("session,");
  if(p_trigger.m_transaction) line += _T("transaction,");
  if(p_trigger.m_rollback)    line += _T("rollback,");
  line.TrimRight(',');
  item = InsertItem(line,p_item);
  SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);

  // Referencing statement (if any)
  if(!p_trigger.m_referencing.IsEmpty())
  {
    line.Format(_T("Referencing: %s"),p_trigger.m_referencing);
    item = InsertItem(line,p_item);
    SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);
  }
  // Source
  CString name = p_trigger.m_schemaName + _T(".") + p_trigger.m_triggerName;
  m_source.insert(std::make_pair(name,p_trigger.m_source));
  line.Format(_T("SQL Source: %d characters"),p_trigger.m_source.GetLength());
  item = InsertItem(line,p_item,TREE_SOURCECODE);
  SetItemImage(item,IMG_SOURCE,IMG_SOURCE);
}

void      
ObjectTree::SequencesToTree(MSequenceMap& p_sequences,HTREEITEM p_item)
{
  // Check if we have sequences
  if(p_sequences.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  CString line;
  for(auto& seq : p_sequences)
  {
    // The main sequence item
    line.Format(_T("Sequence name: %s"),seq.m_sequenceName);
    HTREEITEM seqItem = InsertItem(line,p_item);
    SetItemImage(seqItem,IMG_SEQUENCE,IMG_SEQUENCE);

    // All info on the item
    line.Format(_T("Minimum value: %-12.0f"),seq.m_minimalValue);
    HTREEITEM item = InsertItem(line,seqItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Current value: %-12.0f"),seq.m_currentValue);
    item = InsertItem(line,seqItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Increment with: %d"),seq.m_increment);
    item = InsertItem(line,seqItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Caching size: %d"),seq.m_cache);
    item = InsertItem(line,seqItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Re-Cycling of sets: %s"),seq.m_cycle ? _T("yes") : _T("no"));
    item = InsertItem(line,seqItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Cluster ordering: %s"),seq.m_order ? _T("yes") : _T("no"));
    item = InsertItem(line,seqItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);
  }
}

void
ObjectTree::PrivilegesToTree(MPrivilegeMap& p_privileges,HTREEITEM p_item)
{
  // Check for privileges
  if(p_privileges.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  // Do all privileges records
  for(auto& priv : p_privileges)
  {
    CString line;
    line.Format(_T("%s was granted %s by %s")
                ,priv.m_grantee
                ,priv.m_privilege
                ,priv.m_grantor);
    if(priv.m_grantable)
    {
      line += _T(" (With GRANT OPTION)");
    }
    HTREEITEM item = InsertItem(line,p_item);
    SetItemImage(item,IMG_ACCESS,IMG_ACCESS);
  }
}

void
ObjectTree::ColumnPrivilegesToTree(MPrivilegeMap& p_privileges,HTREEITEM p_item)
{
  // Check for privileges
  if(p_privileges.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  // Do all privileges records
  for(auto& priv : p_privileges)
  {
    CString line;
    line.Format(_T("%s was granted %s on column %s by %s")
                ,priv.m_grantee
                ,priv.m_privilege
                ,priv.m_columnName
                ,priv.m_grantor);
    if(priv.m_grantable)
    {
      line += _T(" (With GRANT OPTION)");
    }
    HTREEITEM item = InsertItem(line,p_item);
    SetItemImage(item,IMG_ACCESS,IMG_ACCESS);
  }
}

void
ObjectTree::ParametersToTree(MParameterMap& p_parameters,HTREEITEM p_item)
{
  // Check for parameters
  if(p_parameters.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();

  // Do all parameters
  for(auto& param : p_parameters)
  {
    CString line;
    line.Format(_T("%d: %s"),param.m_position,param.m_parameter);
    switch(param.m_nullable)
    {
      case SQL_NO_NULLS:         line += _T(" (NOT NULL)"); break;
      case SQL_NULLABLE:         line += _T(" (NULLABLE)"); break;
      case SQL_NULLABLE_UNKNOWN: // Fall through
      default:                   line += _T(" (NULLS unknown)");
                                 break;
    }
    HTREEITEM paramItem = InsertItem(line,p_item);
    SetItemImage(paramItem,IMG_PARAMETER,IMG_PARAMETER);

    line = _T("Parameter type: ");
    switch(param.m_columnType)
    {
      case SQL_PARAM_INPUT:               line += _T("input");              break;
      case SQL_PARAM_OUTPUT:              line += _T("output");             break;
      case SQL_PARAM_INPUT_OUTPUT:        line += _T("input/output");       break;
      case SQL_RETURN_VALUE:              line += _T("return value");       break;
      case SQL_RESULT_COL:                line += _T("result column");      break;
      case SQL_PARAM_INPUT_OUTPUT_STREAM: line += _T("input/output stream");break;
      case SQL_PARAM_OUTPUT_STREAM:       line += _T("output stream");      break;
      case SQL_PARAM_TYPE_UNKNOWN:        // fall through
      default:                            line += _T("UNKNOWN");            break;
    }
    HTREEITEM item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("ODBC Datatype: %s"),info->ODBCDataType(param.m_datatype));
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("RDBMS Datatype: %s"),param.m_typeName);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Column size: %d"),param.m_columnSize);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Buffer size: %d"),param.m_bufferLength);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Decimal digits: %d"),param.m_decimalDigits);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Radix: %d"),param.m_numRadix);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Default value: %s"),param.m_default);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format(_T("Comments: %s"),param.m_remarks);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);
  }
}

void
ObjectTree::AddErrorsToTree(CString p_errors,HTREEITEM p_theItem)
{
  if(!p_errors.IsEmpty())
  {
    CString line(_T("ERROR: "));
    line += p_errors;
    InsertItem(line,p_theItem);
  }
}


