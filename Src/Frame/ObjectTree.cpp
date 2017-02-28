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
#include "StdAfx.h"
#include "ObjectTree.h"
#include "OpenEditorApp.h"
#include "SQLInfoDB.h"

IMPLEMENT_DYNAMIC(ObjectTree,CTreeCtrl)

BEGIN_MESSAGE_MAP(ObjectTree,CTreeCtrl)
  ON_NOTIFY_REFLECT(TVN_SELCHANGED,   OnSelChanged)
  ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING,OnSelChanged)
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
  bitmap.LoadBitmapA(MAKEINTRESOURCE(IDB_RDBMS_OBJECTS));

  // Set bitmap in image list, white is background color
  m_imageList.Add(&bitmap,RGB(0,0,0));

  // Set the image list in the treecontrol
  SetImageList(&m_imageList,TVSIL_NORMAL);
}

// Each node gets a 'no info' extra node
// to show the "+" in the tree control
void
ObjectTree::InsertNoInfo(HTREEITEM p_item)
{
  HTREEITEM noinfo = InsertItem("No information",p_item);
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
    if(title.Compare("No information") == 0)
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
  m_busy = true;

  DeleteAllItems();
  HTREEITEM root = GetRootItem();

  HTREEITEM tables     = InsertItem("Tables",    root,TREE_TABLES);
  HTREEITEM views      = InsertItem("Views",     root,TREE_TABLES);
  HTREEITEM catalogs   = InsertItem("Catalog" ,  root,TREE_TABLES);
  HTREEITEM aliasses   = InsertItem("Aliasses",  root,TREE_TABLES);
  HTREEITEM synonyms   = InsertItem("Synonyms",  root,TREE_TABLES);
  HTREEITEM procedures = InsertItem("Procedures",root,TREE_PROCEDURES);

  SetItemImage(tables,    IMG_TABLES,     IMG_TABLES);
  SetItemImage(views,     IMG_VIEWS,      IMG_VIEWS);
  SetItemImage(catalogs,  IMG_CATALOGS,   IMG_CATALOGS);
  SetItemImage(aliasses,  IMG_ALIASSES,   IMG_ALIASSES);
  SetItemImage(synonyms,  IMG_ALIASSES,   IMG_ALIASSES);
  SetItemImage(procedures,IMG_PROCEDURES, IMG_PROCEDURES);

  InsertNoInfo(tables);
  InsertNoInfo(views);
  InsertNoInfo(catalogs);
  InsertNoInfo(aliasses);
  InsertNoInfo(synonyms);
  InsertNoInfo(procedures);

  m_busy = false;
}

// See if a node is a schema, or if the schema
// is missing, if it is the node above
// BEWARE: Removes the count from the text
bool
ObjectTree::IsSpecialNode(CString& p_name)
{
  // Find first word of the node name
  int pos = p_name.Find(' ');
  if(pos > 0)
  {
    p_name = p_name.Left(pos);
  }

  // See if it is a type node instead of a schema name
  if(p_name.Compare("Tables")     == 0)  return true;
  if(p_name.Compare("Views")      == 0)  return true;
  if(p_name.Compare("Catalog")    == 0)  return true;
  if(p_name.Compare("Aliases")    == 0)  return true;
  if(p_name.Compare("Synonyms")   == 0)  return true;
  if(p_name.Compare("Procedures") == 0)  return true;

  return false;
}

ObjectImage
ObjectTree::TypeToImage(CString p_type)
{
  if(p_type == "T") return IMG_TABLE;
  if(p_type == "V") return IMG_VIEW;
  if(p_type == "C") return IMG_CATALOG;
  if(p_type == "A") return IMG_ALIAS;
  if(p_type == "S") return IMG_ALIAS;
  if(p_type == "P") return IMG_PROCEDURE;

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
    text.AppendFormat(" (%d)",p_size);
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
  HTREEITEM item = CTreeCtrl::InsertItem(p_string.GetString(),p_item);
  SetItemData(item,(DWORD_PTR) p_data);
  return item;
}

// Expanding a node, if not already done
void 
ObjectTree::OnSelChanged(NMHDR* pNMHDR,LRESULT* pResult)
{
  // Guard against re-entrance in the UI thread
  if(m_busy)
  {
    return;
  }
  m_busy = true;

  // See if database is open
  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  if(app->DatabaseIsOpen())
  {
    // See which action to take
    CWaitCursor sighAndWait;
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM theItem = pNMTreeView->itemNew.hItem;

    DWORD_PTR data = GetItemData(theItem);
    if(data)
    {
      switch(data)
      {
        case TREE_TABLES:       FindTables(theItem);     break;
        case TREE_TABLE:        PrepareTable(theItem);   break;
        case TREE_COLUMNS:      FindColumns(theItem);    break;
        case TREE_PRIMARY:      FindPrimary(theItem);    break;
        case TREE_FOREIGN:      FindForeign(theItem);    break;
        case TREE_STATISTICS:   FindStatistics(theItem); break;
        case TREE_SPECIALS:     FindSpecials(theItem);   break;
        case TREE_REFERENCEDBY: FindReferenced(theItem); break;
        case TREE_TRIGGERS:     FindTriggers(theItem);   break;
        case TREE_PRIVILEGES:   FindPrivileges(theItem); break;
        case TREE_PROCEDURES:   FindProcedures(theItem); break;
        case TREE_PARAMETERS:   FindParameters(theItem); break;
        default:                /* NOTHING */            break;
      }
      Expand(theItem,TVE_EXPAND);

      // Resetting the item, so we do this only **ONCE**
      SetItemData(theItem,0);
    }
  }
  // Ready
  m_busy = false;
}

// Move the string list to the tree
// 1) Remove the 'no info' marker if the list is filled
// 2) Add items, with respect to the '+' and '-' marker
// 3) '+' -> One extra level in the tree
// 4) '-' -> One level back in the tree
void
ObjectTree::WordListToTree(WordList& p_list,HTREEITEM p_item,ObjectImage p_image)
{
  // We now have information. Remove the 'No information' marker
  if(!p_list.empty())
  {
    RemoveNoInfo(p_item);
  }

  HTREEITEM item = p_item;
  HTREEITEM last = NULL;

  // Add the contents of the list
  for(auto& line : p_list)
  {
    if(line == "+" && last)
    {
      // Indent one level
      item = last;
      continue;
    }
    if(line == "-")
    {
      // Reset to original
      item = p_item;
    }
    else
    {
      last = InsertItem(line,item);
      SetItemImage(last,p_image,p_image);
    }
  }
  p_list.clear();
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
  CString text = GetItemText(p_theItem);
  CString type = text.Left(1);
  CString find = type + ":";
  if(m_filter.IsEmpty())
  {
    find += "%";
  }
  else
  {
    find += m_filter;
    if(find.Right(1) != "%")
    {
      find += "%";
    }
  }

  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  MTableMap tables;
  CString   errors;
  CString   lastSchema;
  int       schemaCount = 0;
  HTREEITEM schemaItem  = 0;

  // Go  find it
  app->GetDatabase().GetSQLInfoDB()->MakeInfoTableTablepart(find,tables,errors);

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
      object.AppendFormat(" (%s)",theTable.m_remarks);
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
    ObjectImage image = TypeToImage(type);
    SetItemImage(table,image,image);
    PrepareTable(table);
  }
}

// Add what we can find in a table
void
ObjectTree::PrepareTable(HTREEITEM p_theItem)
{
  HTREEITEM columns    = InsertItem("Columns",        p_theItem,TREE_COLUMNS);
  HTREEITEM primary    = InsertItem("Primary key",    p_theItem,TREE_PRIMARY);
  HTREEITEM foreigns   = InsertItem("Foreign keys",   p_theItem,TREE_FOREIGN);
  HTREEITEM indices    = InsertItem("Statistics",     p_theItem,TREE_STATISTICS);
  HTREEITEM specials   = InsertItem("Special columns",p_theItem,TREE_SPECIALS);
  HTREEITEM referenced = InsertItem("Referenced by",  p_theItem,TREE_REFERENCEDBY);
  HTREEITEM triggers   = InsertItem("Triggers",       p_theItem,TREE_TRIGGERS);
  HTREEITEM access     = InsertItem("Privileges",     p_theItem,TREE_PRIVILEGES);

  SetItemImage(columns,   IMG_COLUMN,  IMG_COLUMN);
  SetItemImage(primary,   IMG_PRIMARY, IMG_PRIMARY);
  SetItemImage(foreigns,  IMG_FOREIGN, IMG_FOREIGN);
  SetItemImage(indices,   IMG_INDEX,   IMG_INDEX);
  SetItemImage(specials,  IMG_COLUMN,  IMG_COLUMN);
  SetItemImage(referenced,IMG_FOREIGN, IMG_FOREIGN);
  SetItemImage(triggers,  IMG_TRIGGER, IMG_TRIGGER);
  SetItemImage(access,    IMG_ACCESS,  IMG_ACCESS);

  InsertNoInfo(columns);
  InsertNoInfo(primary);
  InsertNoInfo(foreigns);
  InsertNoInfo(indices);
  InsertNoInfo(specials);
  InsertNoInfo(referenced);
  InsertNoInfo(triggers);
  InsertNoInfo(access);
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
  CString findtable(table);
  if(!IsSpecialNode(schema))
  {
    findtable = schema + "." + table;
  }

  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  // Set table to use
  MTableMap tables;
  CString   errors;
  return app->GetDatabase().GetSQLInfoDB()->MakeInfoTableTablepart(findtable,tables,errors);
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
    findproc = schema + "." + procedure;
  }

  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  CString errors;
  // Set procedure
  return app->GetDatabase().GetSQLInfoDB()->MakeInfoProcedureProcedurepart(findproc,p_procedures,errors);
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
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableColumns(columns,errors);
    ColumnListToTree(columns,p_theItem);
  }
}

// Find all the primary keys in a table
// 0 or 1 is all we can find :-)
void
ObjectTree::FindPrimary(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    MPrimaryMap primaries;
    CString     errors;

    // Go find the primary key
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTablePrimary(primaries,errors);
    PrimariesToTree(primaries,p_theItem);
  }
}

// Find all foreign keys on this table
// pointing to other tables
void
ObjectTree::FindForeign(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    MForeignMap foreigns;
    CString     errors;

    // Go find the foreign keys
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableForeign(foreigns,errors);
    ForeignsToTree(foreigns,p_theItem);
  }
}

// Finding table statistics
// and indices for the table
void
ObjectTree::FindStatistics(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    MStatisticsMap  statistics;
    CString         errors;

    // Go find the indices and statistics
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableStatistics(statistics,nullptr,errors);
    StatisticsToTree(statistics,p_theItem);
  }
}

// Finding special columns like 'ROWID' (oracle) or 'OID' (PostgreSQL)
void      
ObjectTree::FindSpecials(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    MSpecialColumnMap specials;
    CString errors;

    // Go find the special columns
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableSpecials(specials,errors);
    SpecialsToTree(specials,p_theItem);
  }
}

// Find all foreign keys from OTHER tables
// pointing to this table
void
ObjectTree::FindReferenced(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    MForeignMap foreigns;
    CString     errors;

    // Go find the referencing tables
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableForeign(foreigns,errors,true);
    ReferencedToTree(foreigns,p_theItem);
  }
}

// Find all triggers on the table
void
ObjectTree::FindTriggers(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    MTriggerMap triggers;
    CString     errors;

    // Go find the triggers
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableTriggers(triggers,errors);
    TriggersToTree(triggers,p_theItem);
  }
}

// Finding all table privileges on the table
void
ObjectTree::FindPrivileges(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    MPrivilegeMap privileges;
    CString errors;

    // Go find the privileges on the table
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTablePrivileges(privileges,errors);
    PrivilegesToTree(privileges,p_theItem);
  }
}

// Finding all procedures according to the filter, or all
void
ObjectTree::FindProcedures(HTREEITEM p_theItem)
{
  CString find = m_filter.IsEmpty() ? "%" : m_filter;
  if(find.Right(1) != "%")
  {
    find += "%";
  }

  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  MProcedureMap procedures;
  CString   errors;
  CString   lastSchema;
  int       schemaCount = 0;
  HTREEITEM schemaItem  = NULL;

  // Go find the procedures
  app->GetDatabase().GetSQLInfoDB()->MakeInfoProcedureProcedurepart(find,procedures,errors);

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
    HTREEITEM information = InsertItem("Information",procedure,TREE_PARAMETERS);
    HTREEITEM parameters  = InsertItem("Parameters", procedure,TREE_PARAMETERS);

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

    line = "SPL type: ";
    switch(procedure.m_procedureType)      
    {
      case SQL_PT_PROCEDURE:  line += "PROCEDURE"; break;
      case SQL_PT_FUNCTION:   line += "FUNCTION";  break;
      case SQL_PT_UNKNOWN:    // Fall through
      default:                line += "UNKNOWN";   break;
    }
    HTREEITEM item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Input parameters: %d", procedure.m_inputParameters);
    item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);
    
    line.Format("Output parameters: %d",procedure.m_outputParameters);
    item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);
    
    line.Format("Result sets: %d",      procedure.m_resultSets);
    item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Remarks: %s",procedure.m_remarks);
    item = InsertItem(line,info);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    // Now getting the parameter list
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    WordList params;

    // Go find the parameters for the procedure
    CString errors;
    MProcColumnMap parameters;
    app->GetDatabase().GetSQLInfoDB()->MakeInfoProcedureParameters(parameters,errors);
    ParametersToTree(parameters,param);

    // Reset BOTH node data pointers
    SetItemData(info,0);
    SetItemData(param,0);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// FILLING THE TREE
//
//////////////////////////////////////////////////////////////////////////

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
    CString line  = column.m_column + " " + column.m_typename;
    if(column.m_precision > 0)
    {
      line.AppendFormat(" (%d",column.m_precision);
      if(column.m_scale)
      {
        line.AppendFormat(",%d)",column.m_scale);
      }
      else
      {
        line += ")";
      }
    }
    switch(column.m_nullable)
    {
      case SQL_NO_NULLS:          line += " NOT NULL"; 
                                  break;
      case SQL_NULLABLE:          break;  
      default:                    // Fall through
      case SQL_NULLABLE_UNKNOWN:  line += " NULLABLE UNKNOWN";
                                  break;
    }

    if(!column.m_remarks.IsEmpty())
    {
      line.AppendFormat(" (%s)",column.m_remarks);
    }
    HTREEITEM item = InsertItem(line,p_item);
    SetItemImage(item,IMG_COLUMN,IMG_COLUMN);
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
  CString name("Constraint name: ");
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
    case SQL_CASCADE:     text = "CASCADE";     break;
    case SQL_NO_ACTION:   text = "NO ACTION";   break;
    case SQL_SET_NULL:    text = "SET NULL";    break;
    case SQL_SET_DEFAULT: text = "SET DEFAULT"; break;
    default:              text = "No info";     break;
  }
  return text;
}

CString
ObjectTree::DeferrableToString(int p_defer)
{
  CString text;
  switch(p_defer)
  {
    case SQL_INITIALLY_DEFERRED:  text = "INITIALLY DEFERRED";  break;
    case SQL_INITIALLY_IMMEDIATE: text = "INITIALLY IMMEDIATE"; break;
    case SQL_NOT_DEFERRABLE:      text = "NOT DEFERRABLE";      break;
    default:                      text = "No Info";             break;
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
      line  = "Constraint name: ";
      line += foreign.m_foreignConstraint;
      keyItem = InsertItem(line,p_item);
      SetItemImage(keyItem,IMG_FOREIGN,IMG_FOREIGN);

      // Update rule
      line = "Update rule: " + ForeignRuleToString(foreign.m_updateRule);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Delete rule
      line = "Delete rule: " + ForeignRuleToString(foreign.m_deleteRule);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // deferrable
      line = "Deferrable: " + DeferrableToString(foreign.m_deferrable);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Primary key + columns
      line.Format("Primary key: %s (%s)"
                  ,foreign.m_pkTableName
                  ,foreign.m_primaryConstraint);
      primkey = InsertItem(line,keyItem);
      SetItemImage(primkey,IMG_FOREIGN,IMG_FOREIGN);
    }
    // Add the columns
    line.Format("%s -> %s",foreign.m_fkColumnName,foreign.m_pkColumnName);
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
      line  = "Table: ";
      line += foreign.m_fkTableName;
      keyItem = InsertItem(line,p_item);
      SetItemImage(keyItem,IMG_FOREIGN,IMG_FOREIGN);

      // Update rule
      line = "Update rule: " + ForeignRuleToString(foreign.m_updateRule);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Delete rule
      line = "Delete rule: " + ForeignRuleToString(foreign.m_deleteRule);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Deferable
      line = "Deferrable: " + DeferrableToString(foreign.m_deferrable);
      info = InsertItem(line,keyItem);
      SetItemImage(info,IMG_INFO,IMG_INFO);

      // Primary key + columns
      line.Format("Primary key: %s (%s)"
                  ,foreign.m_pkTableName
                  ,foreign.m_primaryConstraint);
      primkey = InsertItem(line,keyItem);
      SetItemImage(primkey,IMG_FOREIGN,IMG_FOREIGN);
    }
    // Add the columns
    line.Format("%s -> %s",foreign.m_fkColumnName,foreign.m_pkColumnName);
    HTREEITEM col = InsertItem(line,primkey);
    SetItemImage(col,IMG_COLUMN,IMG_COLUMN);
  }
}

void
ObjectTree::StatisticsToTree(MStatisticsMap& p_statistics,HTREEITEM p_item)
{
  // Check for a indices
  if(p_statistics.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  HTREEITEM next = NULL;
  CString   line;
  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
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
        next = InsertItem("[Table statistics]",p_item);
      }
      else
      {
        // New index on a table
        line.Format("Index name: %s",stat.m_indexName);
        if(stat.m_unique)
        {
          line += " (Unique)";
        }
        if(stat.m_indexType == SQL_INDEX_CLUSTERED)
        {
          line += " (Clustered)";
        }
        if(stat.m_indexType == SQL_INDEX_HASHED)
        {
          line += " (Hashed)";
        }
        next = InsertItem(line,p_item);
      }
      SetItemImage(next,IMG_INDEX,IMG_INDEX);

      // Table/index nodes have cardinality and pages
      line.Format("Cardinality: %d",stat.m_cardinality);
      HTREEITEM item = InsertItem(line,next);
      SetItemImage(item,IMG_INDEX,IMG_INDEX);
      line.Format("Data pages: %d",stat.m_pages);
      item = InsertItem(line,next);
      SetItemImage(item,IMG_INDEX,IMG_INDEX);
    }
    if(stat.m_indexType != SQL_TABLE_STAT)
    {
      // Optionally get the filter
      if(stat.m_filter.IsEmpty())
      {
        stat.m_filter = info->GetIndexFilter(stat);
      }

      // Extra column on an index
      if(!stat.m_filter.IsEmpty())
      {
        line.Format("%d: %s",stat.m_position,stat.m_filter);
      }
      else
      {
        line.Format("%d: %s",stat.m_position,stat.m_columnName);
      }
      line += (stat.m_ascending == "A") ? " (Ascending)" : " (Descending)";
      HTREEITEM item = InsertItem(line,next);
      SetItemImage(item,IMG_COLUMN,IMG_COLUMN);
    }
  }
}

void
ObjectTree::SpecialsToTree(MSpecialColumnMap& p_specials,HTREEITEM p_item)
{
  // Check for special columns
  if(p_specials.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  HTREEITEM next = NULL;
  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();

  // Inserts the special columns
  for(auto& special : p_specials)
  {
    CString line = "Columnname: " + special.m_columnName;
    next = InsertItem(line,p_item);
    SetItemImage(next,IMG_COLUMN,IMG_COLUMN);

    line.Format("RDBMS Special type: %s",special.m_typeName);
    HTREEITEM item  = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("ODBC data type: %s",info->ODBCDataType(special.m_datatype));
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Column size: %d",special.m_columnSize);
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Buffer size: %d",special.m_bufferSize);
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Decimal digits: %d",special.m_decimalDigits);
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line = "Column scope: ";
    switch(special.m_scope)
    {
      case SQL_SCOPE_CURROW:      line += "Current row"; break;
      case SQL_SCOPE_TRANSACTION: line += "Transaction"; break;
      case SQL_SCOPE_SESSION:     line += "Session";     break;
      default:                    line += "Unknown";     break;
    }
    item = InsertItem(line,next);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line = "Column scope: ";
    switch(special.m_pseudo)
    {
      case SQL_PC_NON_PSEUDO: line += "Non-pseudo";    break;
      case SQL_PC_PSEUDO:     line += "Pseudo column"; break;
      default:                line += "No info";       break;
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
    CString line;
    line.Format("%d: %s",trigger.m_position,trigger.m_triggerName);
    if(trigger.m_remarks)
    {
      line.AppendFormat(" (%s)",trigger.m_remarks);
    }
    HTREEITEM trigItem = InsertItem(line,p_item);
    SetItemImage(trigItem,IMG_TRIGGER,IMG_TRIGGER);

    // Active trigger
    line.Format("Trigger is: %s",trigger.m_enabled ? "enabled" : "disabled");
    HTREEITEM item = InsertItem(line,trigItem);
    SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);

    // Type before/after
    line.Format("Trigger fires: %s",trigger.m_before ? "before" : "after");
    item = InsertItem(line,trigItem);
    SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);

    // Fires on INSERT/UPDATE/DELETE/SELECT
    line = "Trigger on DML: ";
    if(trigger.m_insert) line += "insert,";
    if(trigger.m_update) line += "update,";
    if(trigger.m_delete) line += "delete,";
    if(trigger.m_select) line += "select,";
    line.TrimRight(',');
    item = InsertItem(line,trigItem);
    SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);

    // Referencing statement (if any)
    if(!trigger.m_referencing.IsEmpty())
    {
      line.Format("Referencing: %s",trigger.m_referencing);
      item = InsertItem(line,trigItem);
      SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);
    }
    // Source
    line.Format("SQL Source: %d characters",trigger.m_source.GetLength());
    item = InsertItem(line,trigItem);
    SetItemImage(item,IMG_TRIGGER,IMG_TRIGGER);
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
    line.Format("%s was granted %s by %s"
                ,priv.m_grantee
                ,priv.m_privilege
                ,priv.m_grantor);
    if(priv.m_grantable)
    {
      line += " (With GRANT OPTION)";
    }
    HTREEITEM item = InsertItem(line,p_item);
    SetItemImage(item,IMG_ACCESS,IMG_ACCESS);
  }
}

void
ObjectTree::ParametersToTree(MProcColumnMap& p_parameters,HTREEITEM p_item)
{
  // Check for parameters
  if(p_parameters.empty())
  {
    return;
  }
  RemoveNoInfo(p_item);

  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();

  // Do all parameters
  for(auto& param : p_parameters)
  {
    CString line;
    line.Format("%d: %s",param.m_ordinalPosition,param.m_columnName);
    switch(param.m_nullable)
    {
      case SQL_NO_NULLS:         line += " (NOT NULL)"; break;
      case SQL_NULLABLE:         line += " (NULLABLE)"; break;
      case SQL_NULLABLE_UNKNOWN: // Fall through
      default:                   line += " (NULLS unknown)";
                                 break;
    }
    HTREEITEM paramItem = InsertItem(line,p_item);
    SetItemImage(paramItem,IMG_PARAMETER,IMG_PARAMETER);

    line = "Parameter type: ";
    switch(param.m_columnType)
    {
      case SQL_PARAM_INPUT:               line += "input";              break;
      case SQL_PARAM_OUTPUT:              line += "output";             break;
      case SQL_PARAM_INPUT_OUTPUT:        line += "input/output";       break;
      case SQL_RETURN_VALUE:              line += "return value";       break;
      case SQL_RESULT_COL:                line += "result column";      break;
      case SQL_PARAM_INPUT_OUTPUT_STREAM: line += "input/output stream";break;
      case SQL_PARAM_OUTPUT_STREAM:       line += "output stream";      break;
      case SQL_PARAM_TYPE_UNKNOWN:        // fall through
      default:                            line += "UNKNOWN";            break;
    }
    HTREEITEM item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("ODBC Datatype: %s",info->ODBCDataType(param.m_dataType));
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("RDBMS Datatype: %s",param.m_typeName);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Columnsize: %d",param.m_columnSize);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Buffersize: %d",param.m_bufferSize);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Decimal digits: %d",param.m_decimalDigits);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Radix: %d",param.m_radix);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Default value: %s",param.m_defaultValue);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);

    line.Format("Comments: %s",param.m_remarks);
    item = InsertItem(line,paramItem);
    SetItemImage(item,IMG_INFO,IMG_INFO);
  }
}
