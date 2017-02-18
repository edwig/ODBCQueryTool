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
  // Find first word of the nodename
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

  // Find primary text of the nodename
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
  WordList  list;
  CString   lastSchema;
  int       schemaCount = 0;
  HTREEITEM schemaItem  = 0;

  // Go  find it
  app->GetDatabase().GetSQLInfoDB()->MakeInfoTableTablepart(&list,nullptr,find);

  if(!list.empty())
  {
    RemoveNoInfo(p_theItem);
  }

  // Set item count text
  SetItemCount(p_theItem,(int)list.size());

  for(auto& str : list)
  {
    int pos = str.Find(':');
    if(pos > 0)
    {
      HTREEITEM table = NULL;
      CString toAdd = str.Mid(pos + 1);
      pos = toAdd.Find('.');
      if(pos > 0)
      {
        CString schema = toAdd.Left(pos);
        CString object = toAdd.Mid(pos + 1);
        schema.Trim();
        object.Trim();

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
      else
      {
        // No schema found, just add the item
        table = InsertItem(toAdd,p_theItem);
      }
      ObjectImage image = TypeToImage(type);
      SetItemImage(table,image,image);
      PrepareTable(table);
    }
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
  HTREEITEM access     = InsertItem("Privileges",     p_theItem,TREE_PRIVILEGES);

  SetItemImage(columns,   IMG_COLUMN,  IMG_COLUMN);
  SetItemImage(primary,   IMG_PRIMARY, IMG_PRIMARY);
  SetItemImage(foreigns,  IMG_FOREIGN, IMG_FOREIGN);
  SetItemImage(indices,   IMG_INDEX,   IMG_INDEX);
  SetItemImage(specials,  IMG_COLUMN,  IMG_COLUMN);
  SetItemImage(referenced,IMG_FOREIGN, IMG_FOREIGN);
  SetItemImage(access,    IMG_ACCESS,  IMG_ACCESS);

  InsertNoInfo(columns);
  InsertNoInfo(primary);
  InsertNoInfo(foreigns);
  InsertNoInfo(indices);
  InsertNoInfo(specials);
  InsertNoInfo(referenced);
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
  CString findtable(table);
  if(!IsSpecialNode(schema))
  {
    findtable = schema + "." + table;
  }

  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  // Set table to use
  return app->GetDatabase().GetSQLInfoDB()->MakeInfoTableTablepart(NULL,NULL,findtable);
}

// Before expanding a node, find the procedure again
bool
ObjectTree::PresetProcedure(HTREEITEM p_theItem,WordList& p_list)
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
  // Set table to use
  return app->GetDatabase().GetSQLInfoDB()->MakeInfoProcedureProcedurepart(&p_list,findproc);
}

// Find all the columns in a table
void
ObjectTree::FindColumns(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    WordList list;

    // Go find the columns
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableColumns(&list);
    WordListToTree(list,p_theItem,IMG_COLUMN);
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
    WordList   list;
    PrimaryMap keymap;
    CString    primName;

    // Go find the primary key
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTablePrimary(&list,primName,keymap);
    WordListToTree(list,p_theItem,IMG_PRIMARY);
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
    WordList   list;

    // Go find the foreign keys
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableForeign(&list);
    WordListToTree(list,p_theItem,IMG_FOREIGN);
  }
}

// Finding table statistics
// and indeces for the table
void
ObjectTree::FindStatistics(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    WordList   list;
    CString    keyname;
    PrimaryMap primary;

    // Go find the indices and statistics
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableStatistics(&list,keyname,primary);
    WordListToTree(list,p_theItem,IMG_INDEX);
  }
}

// Finding special columns like 'ROWID' (oracle) or 'OID' (PostgreSQL)
void      
ObjectTree::FindSpecials(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    WordList   list;

    // Go find the special columns
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableSpecials(&list);
    WordListToTree(list,p_theItem,IMG_COLUMN);
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
    WordList   list;

    // Go find the referencing tables
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTableForeign(&list,true);
    WordListToTree(list,p_theItem,IMG_FOREIGN);
  }
}

// Finding all table privileges on the table
void
ObjectTree::FindPrivileges(HTREEITEM p_theItem)
{
  if(PresetTable(p_theItem))
  {
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    WordList   list;

    // Go find the privileges on the table
    app->GetDatabase().GetSQLInfoDB()->MakeInfoTablePrivileges(&list);
    WordListToTree(list,p_theItem,IMG_ACCESS);
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
  WordList  list;
  CString   lastSchema;
  int       schemaCount = 0;
  HTREEITEM schemaItem  = NULL;

  // Go find the procedures
  app->GetDatabase().GetSQLInfoDB()->MakeInfoProcedureProcedurepart(&list,find);

  if(!list.empty())
  {
    RemoveNoInfo(p_theItem);
  }

  // Setting count of objects
  SetItemCount(p_theItem,(int)list.size());
  
  for(auto& str : list)
  {
    int pos = str.Find(':');
    if(pos > 0)
    {
      HTREEITEM procedure = NULL;
      CString toAdd = str.Mid(pos + 1);
      pos = toAdd.Find('.');
      if(pos > 0)
      {
        CString schema = toAdd.Left(pos);
        CString object = toAdd.Mid(pos + 1);
        schema.Trim();
        object.Trim();

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
      else
      {
        // No schema found, just add the item
        procedure = InsertItem(toAdd,p_theItem);
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
}

// Finding the procedure parameters for the procedure
void
ObjectTree::FindParameters(HTREEITEM p_theItem)
{
  WordList list;
  if(PresetProcedure(p_theItem,list))
  {
    HTREEITEM proc  = GetParentItem(p_theItem);
    HTREEITEM info  = GetChildItem(proc);
    HTREEITEM param = GetNextItem(info,TVGN_NEXT);

    // procedure type + return mechanism
    if(!list.empty())
    {
      RemoveNoInfo(info);
      for(auto& type : list)
      {
        HTREEITEM node = InsertItem(type,info);
        SetItemImage(node,IMG_INFO,IMG_INFO);
      }
    }

    // Now getting the parameter list
    COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
    WordList params;

    // Go find the parameters for the procedure
    app->GetDatabase().GetSQLInfoDB()->MakeInfoProcedureParameters(&params);
    WordListToTree(params,param,IMG_PARAMETER);

    // Reset BOTH node data pointers
    SetItemData(info,0);
    SetItemData(param,0);
  }
}
