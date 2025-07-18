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
#pragma once
#include "SQLInfo.h"
#include <StyleTreeCtrl.h>
#include <map>

using namespace SQLComponents;

typedef enum treeTypes
{
  TREE_NOTHING = 0  
 ,TREE_TABLES
 ,TREE_PROCEDURES
 ,TREE_TABLE
 ,TREE_COLUMNS
 ,TREE_PRIMARY
 ,TREE_FOREIGN
 ,TREE_STATISTICS
 ,TREE_SPECIALS
 ,TREE_REFERENCEDBY
 ,TREE_TABTRIGGERS
 ,TREE_PRIVILEGES
 ,TREE_COLPRIVILEGES
 ,TREE_PARAMETERS
 ,TREE_TABSEQUENCES
 ,TREE_SEQUENCES
 ,TREE_TRIGGERS
 ,TREE_SOURCECODE
}
TreeTypes;

typedef enum objectImage
{
  IMG_TABLES = 0
 ,IMG_TABLE 
 ,IMG_VIEWS
 ,IMG_VIEW
 ,IMG_CATALOGS
 ,IMG_CATALOG
 ,IMG_PROCEDURES
 ,IMG_PROCEDURE
 ,IMG_SYNONYMS
 ,IMG_SYNONYM
 ,IMG_COLUMN
 ,IMG_PRIMARY
 ,IMG_FOREIGN
 ,IMG_PARAMETER
 ,IMG_TRIGGER
 ,IMG_INFO
 ,IMG_ACCESS
 ,IMG_SCHEMA
 ,IMG_INDEX
 ,IMG_SEQUENCE
 ,IMG_SOURCE
}
ObjectImage;

using SourceList = std::map<CString,CString>;

class ObjectTree: public StyleTreeCtrl
{
public:
  ObjectTree();
 ~ObjectTree();

  void      ClearTree();
  void      SetFilter(CString p_filter);
  void      ExpandFirstTable(CString p_table);
  CString   GetFilter();
  void      CreateImageList();

  HTREEITEM InsertItem(CString p_string,HTREEITEM p_item,int p_data = 0);

  // From pop-up menu
  void      OnTableFirst (HTREEITEM p_item);
  void      OnTableDDL   (HTREEITEM p_item);
  bool      OnTableOrView(HTREEITEM p_item);

private:
  ObjectImage TypeToImage(CString p_type);
  void      TriggerToTree(MetaTrigger& trigger,HTREEITEM trigItem);

protected:
  DECLARE_MESSAGE_MAP();

  void      InsertNoInfo    (HTREEITEM p_item);
  void      RemoveNoInfo    (HTREEITEM p_item);
  CString   GetObjectType   (CString   p_type);
  bool      IsSpecialNode   (CString&   p_name);
  void      SetItemCount    (HTREEITEM p_theItem,int p_size);
  bool      PresetTable     (HTREEITEM p_theItem);
  bool      PresetProcedure (HTREEITEM p_theItem,MProcedureMap& p_procedures);
  void      FindTables      (HTREEITEM p_theItem);
  void      PrepareTable    (HTREEITEM p_theItem);

  void      DispatchTreeAction(DWORD_PTR p_action, HTREEITEM p_theItem);

  void      FindColumns     (HTREEITEM p_theItem);
  void      FindPrimary     (HTREEITEM p_theItem);
  void      FindForeign     (HTREEITEM p_theItem);
  void      FindStatistics  (HTREEITEM p_theItem);
  void      FindSpecials    (HTREEITEM p_theItem);
  void      FindReferenced  (HTREEITEM p_theItem);
  void      FindTabTriggers (HTREEITEM p_theItem);
  void      FindTabSequences(HTREEITEM p_theItem);
  void      FindPrivileges  (HTREEITEM p_theItem);
  void      FindSequences   (HTREEITEM p_theItem);
  void      FindTriggers    (HTREEITEM p_theItem);
  void      FindProcedures  (HTREEITEM p_theItem);
  void      FindParameters  (HTREEITEM p_theItem);
  void      FindSourcecode  (HTREEITEM p_theItem);
  void      FindColumnPrivileges(HTREEITEM p_theItem);

  void      ColumnListToTree(MColumnMap&    p_columns,   HTREEITEM p_item);
  void      PrimariesToTree (MPrimaryMap&   p_primaries, HTREEITEM p_item);
  void      ForeignsToTree  (MForeignMap&   p_foreigns,  HTREEITEM p_item);
  void      ReferencedToTree(MForeignMap&   p_foreigns,  HTREEITEM p_item);
  void      TriggersToTree  (MTriggerMap&   p_triggers,  HTREEITEM p_item);
  void      SequencesToTree (MSequenceMap&  p_sequences, HTREEITEM p_item);
  void      StatisticsToTree(MIndicesMap&   p_statistics,HTREEITEM p_item);
  void      SpecialsToTree  (MSpecialsMap&  p_specials,  HTREEITEM p_item);
  void      PrivilegesToTree(MPrivilegeMap& p_privileges,HTREEITEM p_item);
  void      ParametersToTree(MParameterMap& p_parameters,HTREEITEM p_item);
  void      ColumnPrivilegesToTree(MPrivilegeMap& p_privileges,HTREEITEM p_item);
  void      ColumnDetailsToTree(MetaColumn& p_column,    HTREEITEM p_item);
  void      AddErrorsToTree(CString p_errors,HTREEITEM p_theItem);

  CString   ForeignRuleToString(int p_rule);
  CString   DeferrableToString(int p_defer);
  bool      ShowSourcecodeTrigger(CString p_schema, CString p_trigger);
  bool      ShowSourcecodeModule (CString p_schema, CString p_procedure);

  // Data
  CString     m_filter;
  bool        m_busy;
  CImageList  m_imageList;
  // Currently searching for...
  CString     m_schema;
  CString     m_table;
  CString     m_procedure;
  // Expanding source code
  SourceList  m_source;
public:
  afx_msg void OnItemExpanding    (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnItemDoubleClicked(NMHDR* pNMHDR, LRESULT* pResult);
};

inline void
ObjectTree::SetFilter(CString p_filter)
{
  m_filter = p_filter;
}

inline CString
ObjectTree::GetFilter()
{
  return m_filter;
}


