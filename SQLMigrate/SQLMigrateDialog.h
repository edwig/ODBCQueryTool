////////////////////////////////////////////////////////////////////////
//
// File: SQLMigrateDialog.h
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version number: See SQLComponents.h
//
#pragma once
#include "MigrateParameters.h"
#include "SQLMigrate.h"
#include "Logging.h"
#include "resource.h"
#include "version.h"

class SQLMigrateDialog : public StyleDialog
{
public:
  SQLMigrateDialog(CWnd* pParent = NULL);
 ~SQLMigrateDialog();
  BOOL  AddLogLine(XString msg);
  void  SetTableGauge (int num,int maxnum);
  void  SetTablesGauge(int num,int maxnum);
  void  SetSourceType(XString p_type);
  void  SetTargetType(XString p_type);
  void  SetStatus(XString status);
  void  LoadProfile();
  void  SaveProfile();
  void  Closing(bool p_success);

  enum { IDD = IDD_MIGRATE_WIZARD};

  XString       m_directory;
  XString       m_sourceDSN;
  XString       m_sourceUser;
  XString       m_sourcePassword;
  XString       m_sourceType;
  XString       m_targetDSN;
  XString       m_targetUser;
  XString       m_targetPassword;
  XString       m_targetType;
  XString       m_sourceSchema;
  XString       m_targetSchema;

  MigrateType   m_directMigration;
  XString       m_dropscript;
  XString       m_createscript;

  XString       m_tablespace;
  XString       m_logPerRow;
  XString       m_table;
  XString       m_where;
  XString       m_status;
  XString       m_estimated;
  XString       m_log;

  BOOL          m_toLogfile;
  BOOL          m_allTables;

  BOOL          m_do_tables;
  BOOL          m_do_views;
  BOOL          m_do_data;
  BOOL          m_do_truncate;
  BOOL          m_do_deletes;
  BOOL          m_do_primarys;
  BOOL          m_do_indices;
  BOOL          m_do_foreigns;
  BOOL          m_do_sequences;
  BOOL          m_do_triggers;
  BOOL          m_do_access;

  // Controls
  StyleEdit     m_editDirectory;
  StyleButton   m_buttonDirectory;

  StyleComboBox m_comboSourceDSN;  
  StyleEdit     m_editSourceUser;
  StyleEdit     m_editSourcePassword;
  StyleEdit     m_editSourceType;

  StyleComboBox m_comboTargetDSN;
  StyleEdit     m_editTargetUser;
  StyleEdit     m_editTargetPassword;
  StyleEdit     m_editTargetType;

  StyleEdit     m_editSourcSchema;
  StyleEdit     m_editTargetSchema;
  StyleEdit     m_editTablespace;

  StyleComboBox m_comboDirectMigration;
  StyleEdit     m_editDropscript;
  StyleEdit     m_editCreatescript;
  StyleEdit     m_editTable;
  StyleEdit     m_editWhere;

  StyleCheckbox m_checkToLogfile;
  StyleCheckbox m_checkAllTables;

  StyleProgressCtrl m_table_gauge;
  StyleProgressCtrl m_tables_gauge;
  StyleEdit         m_editStatus;
  StyleEdit         m_editLogPerRow;
  StyleEdit         m_editEstimated;
  StyleEdit         m_editLog;

  StyleCheckbox     m_check_do_tables;
  StyleCheckbox     m_check_do_views;
  StyleCheckbox     m_check_do_data;
  StyleCheckbox     m_check_do_truncate;
  StyleCheckbox     m_check_do_deletes;
  StyleCheckbox     m_check_do_primarys;
  StyleCheckbox     m_check_do_indices;
  StyleCheckbox     m_check_do_foreigns;
  StyleCheckbox     m_check_do_sequences;
  StyleCheckbox     m_check_do_triggers;
  StyleCheckbox     m_check_do_access;

  StyleButton       m_buttonExport;
  StyleButton       m_buttonClose;

  // Working objects
  XString           m_iniFile;
  clock_t           m_start;
  MigrateParameters m_parameters;
  Logging           m_logfile;

  void HandleMessages();

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

  void FindProfile();
  bool StartFromCommandLine();
  void SetComboBoxes();
  void SetDirectory();
  void EstimateRemainingTime(int p_num,int p_maxnum);
  void InitTaskbar();

  // Migration
  void GetMigrationParameters();
  void PerformMigration();
  void PostMigration();

  afx_msg void    OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();

  afx_msg void OnAboutBox();
  afx_msg void OnDirectory();
  afx_msg void OnClosingButton();
  afx_msg bool OnClosing();
  afx_msg void OnAllTables();
  afx_msg void OnDirectMigration();
  afx_msg void OnToLogfile();
  afx_msg void OnEnKillfocusSourceUser();
  afx_msg void OnEnKillfocusTargetUser();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnMigrate();
  afx_msg void OnCancel();

  DECLARE_MESSAGE_MAP()

private:
  CFont*  m_font;
  XString m_profile;

  bool    m_exportRunning;
  bool    m_commandLineMode;
  bool    m_exportResult;
};
