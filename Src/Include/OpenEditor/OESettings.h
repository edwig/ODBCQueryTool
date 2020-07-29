/* 
    Copyright (C) 2002 Aleksey Kochetov

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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __OESettings_h__
#define __OESettings_h__

/*
    28.06.02 new properties have been added
    01.10.02 new properties have been added
    10.01.03 new properties have been added
    29.06.2003 improvement, "Restrict cursor" has been replaced with "Cursor beyond EOL" and "Cursor beyond EOF"
*/

#include <string>
#include <vector>
#include <map>
#include <common/arg_shared.h>
#include "COMMON/VisualAttributes.h"
#include "OpenEditor/OETemplates.h"


#define OES_DECLARE_ABSTR_PROPERTY(T,N) \
    public: \
        virtual const T& Get##N () const = 0; \
        virtual void  Set##N (const T& val, bool notify = true) = 0;

#define OES_DECLARE_PROPERTY(T,N) \
    protected: \
        T m_##N; \
    public: \
        const T& Get##N () const { return m_##N; }; \
        void  Set##N (const T& val, bool notify = true) \
            { m_##N = val; if (notify) NotifySettingsChanged(); }

#define OES_DECLARE_GLOBAL_PROPERTY(T,N) \
    public: \
        const T& Get##N () const { return m_globalSettings.Get##N(); }; \
        void  Set##N (const T& val, bool notify = true) \
            { m_globalSettings.Set##N (val, notify); if (notify) NotifySettingsChanged(); }

#define OES_AGGREGATE_PROPERTY(T,N) \
    public: \
        const T& Get##N () const {  _ASSERTE(m_classSettings); return m_classSettings->Get##N(); }; \
    private: \
        void  Set##N (const T&, bool = true) { _ASSERTE(0); }

#define OES_OVERRIDE_PROPERTY(T,N) \
    protected: \
        T m_##N; \
        bool m_overriden##N; \
    public: \
        const T& Get##N () const \
            { _ASSERTE(m_classSettings); \
              return (m_overriden##N) ? m_##N : m_classSettings->Get##N(); }; \
        void  Set##N (const T& val, bool notify = true) \
            { _ASSERTE(m_classSettings); \
              m_##N = val; m_overriden##N = true; if (notify) NotifySettingsChanged(); }



namespace OpenEditor
{
    using std::string;
    using std::vector;
    using std::map;
    using arg::counted_ptr;

    using Common::VisualAttribute;
    using Common::VisualAttributesSet;
    using Common::VisualAttributeOption;

    class SyntaxAnalizer;
    class Highlighter;
    class Language;

    class SettingsSubscriber;
    class BaseSettings;
    class GlobalSettings;
    class ClassSettings;
    class SettingsManager;
    class SettingsManagerReader;
    class SettingsManagerWriter;

    enum EFileFormat { effDefault = -1, effDos = 0, effUnix = 1, effMac = 2 };
    enum EBackupMode { ebmNone = 0, ebmCurrentDirectory = 1, ebmBackupDirectory = 2 };

    class SettingsSubscriber
    {
        friend class BaseSettings;
    protected:
        virtual ~SettingsSubscriber () {};
    private:
        virtual void OnSettingsChanged () = 0;
    };


    class BaseSettings
    {
    public:
        virtual void AddSubscriber (SettingsSubscriber*) const;
        virtual void RemoveSubscriber (SettingsSubscriber*) const;
        virtual void NotifySettingsChanged ();

        // dont copy subscribers
        BaseSettings () {} 
        BaseSettings (const BaseSettings&) {}
        BaseSettings& operator = (const BaseSettings&) { return *this; }

        virtual ~BaseSettings () {};
    private:
        mutable vector<SettingsSubscriber*> m_subscribers;
    };


    //
    // it's a global part of a settings implementation
    // it will be used by a text class settings
    //
    class GlobalSettings : public BaseSettings
    {
        friend class SettingsManagerReader;
        friend class SettingsManagerWriter;
    public:
        GlobalSettings ();
        static int GetSysPrintMeasuremnt ();

        OES_DECLARE_PROPERTY(bool,   PrintBlackAndWhite);
        OES_DECLARE_PROPERTY(string, PrintHeader);
        OES_DECLARE_PROPERTY(string, PrintFooter);
        OES_DECLARE_PROPERTY(int,    PrintMarginMeasurement);
        OES_DECLARE_PROPERTY(double, PrintLeftMargin);
        OES_DECLARE_PROPERTY(double, PrintRightMargin);
        OES_DECLARE_PROPERTY(double, PrintTopMargin);
        OES_DECLARE_PROPERTY(double, PrintBottomMargin);

        OES_DECLARE_PROPERTY(string, DefFileExtension);
        OES_DECLARE_PROPERTY(int,  UndoLimit);
        OES_DECLARE_PROPERTY(int,  UndoMemLimit);
        OES_DECLARE_PROPERTY(bool, UndoAfterSaving);
        OES_DECLARE_PROPERTY(bool, TruncateSpaces);
        OES_DECLARE_PROPERTY(bool, TabExpand);
        OES_DECLARE_PROPERTY(bool, CursorBeyondEOL);
        OES_DECLARE_PROPERTY(bool, CursorBeyondEOF);
        OES_DECLARE_PROPERTY(bool, LineNumbers);
        OES_DECLARE_PROPERTY(bool, VisibleSpaces);
        OES_DECLARE_PROPERTY(bool, NormalizeKeywords);

        OES_DECLARE_PROPERTY(bool, BlockKeepMarking);
        OES_DECLARE_PROPERTY(bool, BlockKeepMarkingAfterDragAndDrop);
        OES_DECLARE_PROPERTY(bool, BlockDelAndBSDelete);
        OES_DECLARE_PROPERTY(bool, BlockTypingOverwrite);
        OES_DECLARE_PROPERTY(bool, BlockTabIndent);
        OES_DECLARE_PROPERTY(bool, ColBlockDeleteSpaceAfterMove);
        OES_DECLARE_PROPERTY(bool, ColBlockCursorToStartAfterPaste);
        OES_DECLARE_PROPERTY(string, MouseSelectionDelimiters);

        OES_DECLARE_PROPERTY(bool,   FileLocking);
        OES_DECLARE_PROPERTY(bool,   FileMemMapForBig);
        OES_DECLARE_PROPERTY(int,    FileMemMapThreshold);
        OES_DECLARE_PROPERTY(bool,   FileSaveOnSwith);
        OES_DECLARE_PROPERTY(bool,   FileDetectChanges);
        OES_DECLARE_PROPERTY(bool,   FileReloadChanges);
        OES_DECLARE_PROPERTY(bool,   FileAutoscrollAfterReload);
        OES_DECLARE_PROPERTY(bool,   FileOverwriteReadonly);
        OES_DECLARE_PROPERTY(int,    FileBackup);
        OES_DECLARE_PROPERTY(string, FileBackupName);
        OES_DECLARE_PROPERTY(string, FileBackupDirectory);

        OES_DECLARE_PROPERTY(bool,   FileWholeWord);
        OES_DECLARE_PROPERTY(bool,   FileMatchCase);
        OES_DECLARE_PROPERTY(bool,   FileUseRegEx);
        OES_DECLARE_PROPERTY(bool,   FileSubfolders);
        OES_DECLARE_PROPERTY(bool,   FileSaveBefore);
        OES_DECLARE_PROPERTY(bool,   FileCollapse);
        OES_DECLARE_PROPERTY(string, FileWhatToFind);
        OES_DECLARE_PROPERTY(string, FileFileTypes);
        OES_DECLARE_PROPERTY(string, FileStartDirectory);

        OES_DECLARE_PROPERTY(int,    SQLPrefetchLines);
        OES_DECLARE_PROPERTY(string, SQLQueryTerminator);
        OES_DECLARE_PROPERTY(string, SQLQueryFont);

        OES_DECLARE_PROPERTY(bool,   PreferODBCMetaSQL);
        OES_DECLARE_PROPERTY(bool,   AllowMultipleInstances);
        OES_DECLARE_PROPERTY(bool,   NewDocOnStartup);
        OES_DECLARE_PROPERTY(bool,   MaximizeFirstDocument);
        OES_DECLARE_PROPERTY(bool,   WorkDirFollowsDocument);
        OES_DECLARE_PROPERTY(bool,   SaveCurPosAndBookmarks);
        OES_DECLARE_PROPERTY(bool,   SaveMainWinPosition);
        OES_DECLARE_PROPERTY(string, Locale);
        OES_DECLARE_PROPERTY(string, KeymapLayout);
    };


    //
    //  interface
    //
    class Settings : public BaseSettings
    {
        OES_DECLARE_ABSTR_PROPERTY(string, Name);
        OES_DECLARE_ABSTR_PROPERTY(string, Extensions);
        OES_DECLARE_ABSTR_PROPERTY(string, Language);

        OES_DECLARE_ABSTR_PROPERTY(string, Delimiters);
        OES_DECLARE_ABSTR_PROPERTY(int,    TabSpacing);
        OES_DECLARE_ABSTR_PROPERTY(int,    IndentSpacing);
        OES_DECLARE_ABSTR_PROPERTY(int,    IndentType);

        OES_DECLARE_ABSTR_PROPERTY(int,    FileCreateAs); // Dos, Unix, Mac
        OES_DECLARE_ABSTR_PROPERTY(int,    FileSaveAs);   // Default, Dos, Unix, Mac

        OES_DECLARE_ABSTR_PROPERTY(VisualAttributesSet, VisualAttributesSet);

        OES_DECLARE_ABSTR_PROPERTY(bool,   PrintBlackAndWhite);
        OES_DECLARE_ABSTR_PROPERTY(string, PrintHeader);
        OES_DECLARE_ABSTR_PROPERTY(string, PrintFooter);
        OES_DECLARE_ABSTR_PROPERTY(int,    PrintMarginMeasurement);
        OES_DECLARE_ABSTR_PROPERTY(double, PrintLeftMargin);
        OES_DECLARE_ABSTR_PROPERTY(double, PrintRightMargin);
        OES_DECLARE_ABSTR_PROPERTY(double, PrintTopMargin);
        OES_DECLARE_ABSTR_PROPERTY(double, PrintBottomMargin);

        OES_DECLARE_ABSTR_PROPERTY(string, DefFileExtension);
        OES_DECLARE_ABSTR_PROPERTY(int,    UndoLimit);
        OES_DECLARE_ABSTR_PROPERTY(int,    UndoMemLimit);
        OES_DECLARE_ABSTR_PROPERTY(bool,   UndoAfterSaving);
        OES_DECLARE_ABSTR_PROPERTY(bool,   TruncateSpaces);
        OES_DECLARE_ABSTR_PROPERTY(bool,   TabExpand);
        OES_DECLARE_ABSTR_PROPERTY(bool,   CursorBeyondEOL);
        OES_DECLARE_ABSTR_PROPERTY(bool,   CursorBeyondEOF);
        OES_DECLARE_ABSTR_PROPERTY(bool,   LineNumbers);
        OES_DECLARE_ABSTR_PROPERTY(bool,   VisibleSpaces);
        OES_DECLARE_ABSTR_PROPERTY(bool,   NormalizeKeywords);

        OES_DECLARE_ABSTR_PROPERTY(bool,   BlockKeepMarking);
        OES_DECLARE_ABSTR_PROPERTY(bool,   BlockKeepMarkingAfterDragAndDrop);
        OES_DECLARE_ABSTR_PROPERTY(bool,   BlockDelAndBSDelete);
        OES_DECLARE_ABSTR_PROPERTY(bool,   BlockTypingOverwrite);
        OES_DECLARE_ABSTR_PROPERTY(bool,   BlockTabIndent);
        OES_DECLARE_ABSTR_PROPERTY(bool,   ColBlockDeleteSpaceAfterMove);
        OES_DECLARE_ABSTR_PROPERTY(bool,   ColBlockCursorToStartAfterPaste);
        OES_DECLARE_ABSTR_PROPERTY(string, MouseSelectionDelimiters);

        OES_DECLARE_ABSTR_PROPERTY(bool,   FileLocking);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileMemMapForBig);
        OES_DECLARE_ABSTR_PROPERTY(int,    FileMemMapThreshold);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileSaveOnSwith);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileDetectChanges);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileReloadChanges);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileAutoscrollAfterReload);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileOverwriteReadonly);
        OES_DECLARE_ABSTR_PROPERTY(int,    FileBackup);
        OES_DECLARE_ABSTR_PROPERTY(string, FileBackupName);
        OES_DECLARE_ABSTR_PROPERTY(string, FileBackupDirectory);

        OES_DECLARE_ABSTR_PROPERTY(bool,   FileWholeWord);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileMatchCase);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileUseRegEx);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileSubfolders);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileSaveBefore);
        OES_DECLARE_ABSTR_PROPERTY(bool,   FileCollapse);
        OES_DECLARE_ABSTR_PROPERTY(string, FileWhatToFind);
        OES_DECLARE_ABSTR_PROPERTY(string, FileFileTypes);
        OES_DECLARE_ABSTR_PROPERTY(string, FileStartDirectory);

        OES_DECLARE_ABSTR_PROPERTY(int,    SQLPrefetchLines);
        OES_DECLARE_ABSTR_PROPERTY(string, SQLQueryTerminator);
        OES_DECLARE_ABSTR_PROPERTY(string, SQLQueryFont);

        OES_DECLARE_ABSTR_PROPERTY(bool,   PreferODBCMetaSQL);
        OES_DECLARE_ABSTR_PROPERTY(bool,   AllowMultipleInstances);
        OES_DECLARE_ABSTR_PROPERTY(bool,   NewDocOnStartup);
        OES_DECLARE_ABSTR_PROPERTY(bool,   MaximizeFirstDocument);
        OES_DECLARE_ABSTR_PROPERTY(bool,   WorkDirFollowsDocument);
        OES_DECLARE_ABSTR_PROPERTY(bool,   SaveCurPosAndBookmarks);
        OES_DECLARE_ABSTR_PROPERTY(bool,   SaveMainWinPosition);
        OES_DECLARE_ABSTR_PROPERTY(string, Locale);
        OES_DECLARE_ABSTR_PROPERTY(string, KeymapLayout);

        // class/language template support
        virtual const TemplatePtr GetTemplate () const = 0;
    };

    //
    // it's text class settings
    //
    class ClassSettings : public Settings, public SettingsSubscriber
    {
        friend class SettingsManagerReader;
        friend class SettingsManagerWriter;
    public:
        // class properties
        OES_DECLARE_PROPERTY(string, Name);
        OES_DECLARE_PROPERTY(string, Extensions);
        OES_DECLARE_PROPERTY(string, Language);

        OES_DECLARE_PROPERTY(string, Delimiters);
        OES_DECLARE_PROPERTY(int,    TabSpacing);
        OES_DECLARE_PROPERTY(int,    IndentSpacing);
        OES_DECLARE_PROPERTY(int,    IndentType);
        OES_DECLARE_PROPERTY(bool,   TabExpand);
        OES_DECLARE_PROPERTY(bool,   LineNumbers);
        OES_DECLARE_PROPERTY(bool,   VisibleSpaces);
        OES_DECLARE_PROPERTY(bool,   NormalizeKeywords);

        OES_DECLARE_PROPERTY(int,    FileCreateAs);
        // always default
        const int& GetFileSaveAs () const { return m_FileSaveAsDefault; }

        OES_DECLARE_PROPERTY(VisualAttributesSet, VisualAttributesSet);

        VisualAttributesSet& GetVisualAttributesSet () { return m_VisualAttributesSet; }

        // global properties
        OES_DECLARE_GLOBAL_PROPERTY(bool,   PrintBlackAndWhite);
        OES_DECLARE_GLOBAL_PROPERTY(string, PrintHeader);
        OES_DECLARE_GLOBAL_PROPERTY(string, PrintFooter);
        OES_DECLARE_GLOBAL_PROPERTY(int,    PrintMarginMeasurement);
        OES_DECLARE_GLOBAL_PROPERTY(double, PrintLeftMargin);
        OES_DECLARE_GLOBAL_PROPERTY(double, PrintRightMargin);
        OES_DECLARE_GLOBAL_PROPERTY(double, PrintTopMargin);
        OES_DECLARE_GLOBAL_PROPERTY(double, PrintBottomMargin);

        OES_DECLARE_GLOBAL_PROPERTY(string, DefFileExtension);
        OES_DECLARE_GLOBAL_PROPERTY(int,    UndoLimit);
        OES_DECLARE_GLOBAL_PROPERTY(int,    UndoMemLimit);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   UndoAfterSaving);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   TruncateSpaces);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   CursorBeyondEOL);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   CursorBeyondEOF);

        OES_DECLARE_GLOBAL_PROPERTY(bool,   BlockKeepMarking);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   BlockKeepMarkingAfterDragAndDrop);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   BlockDelAndBSDelete);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   BlockTypingOverwrite);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   BlockTabIndent);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   ColBlockDeleteSpaceAfterMove);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   ColBlockCursorToStartAfterPaste);
        OES_DECLARE_GLOBAL_PROPERTY(string, MouseSelectionDelimiters);

        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileLocking);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileMemMapForBig);
        OES_DECLARE_GLOBAL_PROPERTY(int,    FileMemMapThreshold);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileSaveOnSwith);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileDetectChanges);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileReloadChanges);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileAutoscrollAfterReload);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileOverwriteReadonly);
        OES_DECLARE_GLOBAL_PROPERTY(int,    FileBackup);
        OES_DECLARE_GLOBAL_PROPERTY(string, FileBackupName);
        OES_DECLARE_GLOBAL_PROPERTY(string, FileBackupDirectory);

        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileWholeWord);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileMatchCase);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileUseRegEx);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileSubfolders);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileSaveBefore);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   FileCollapse);
        OES_DECLARE_GLOBAL_PROPERTY(string, FileWhatToFind);
        OES_DECLARE_GLOBAL_PROPERTY(string, FileFileTypes);
        OES_DECLARE_GLOBAL_PROPERTY(string, FileStartDirectory);

        OES_DECLARE_GLOBAL_PROPERTY(int,    SQLPrefetchLines);
        OES_DECLARE_GLOBAL_PROPERTY(string, SQLQueryTerminator);
        OES_DECLARE_GLOBAL_PROPERTY(string, SQLQueryFont);

        OES_DECLARE_GLOBAL_PROPERTY(bool,   PreferODBCMetaSQL);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   AllowMultipleInstances);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   NewDocOnStartup);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   MaximizeFirstDocument);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   WorkDirFollowsDocument);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   SaveCurPosAndBookmarks);
        OES_DECLARE_GLOBAL_PROPERTY(bool,   SaveMainWinPosition);
        OES_DECLARE_GLOBAL_PROPERTY(string, Locale);
        OES_DECLARE_GLOBAL_PROPERTY(string, KeymapLayout);


        // class/language template support
        virtual const TemplatePtr GetTemplate () const;

    public:
        // constructors, destructor
        ClassSettings (SettingsManager&);
        ClassSettings (SettingsManager&, const ClassSettings&);
        ~ClassSettings ();

        // assign operator
        ClassSettings& operator = (const ClassSettings&);
    private:
        // not support because it is always default
        virtual void SetFileSaveAs (const int&, bool = true) {};

        ClassSettings (const ClassSettings&);
        SettingsManager& m_owner;
        GlobalSettings& m_globalSettings;


        virtual void OnSettingsChanged () { NotifySettingsChanged(); };

        const static int m_FileSaveAsDefault;
    };


    //
    // it's text document settings
    //
    class InstanceSettings : public Settings, public SettingsSubscriber
    {
        friend class SettingsManagerReader;
        friend class SettingsManagerWriter;
    public:
        // class properties
        OES_AGGREGATE_PROPERTY(string, Name);
        OES_AGGREGATE_PROPERTY(string, Extensions);
        OES_AGGREGATE_PROPERTY(string, Language);

        OES_AGGREGATE_PROPERTY(string, Delimiters);

        // overridden class properties
        OES_OVERRIDE_PROPERTY(int,     TabSpacing);
        OES_OVERRIDE_PROPERTY(int,     IndentSpacing);
        OES_OVERRIDE_PROPERTY(int,     IndentType);
        OES_OVERRIDE_PROPERTY(bool,    TabExpand);
        OES_OVERRIDE_PROPERTY(bool,    LineNumbers);
        OES_OVERRIDE_PROPERTY(bool,    VisibleSpaces);
        OES_OVERRIDE_PROPERTY(bool,    NormalizeKeywords);

        OES_OVERRIDE_PROPERTY(int,     FileCreateAs); // Dos, Unix, Mac
        OES_OVERRIDE_PROPERTY(int,     FileSaveAs); // Default, Dos, Unix, Mac

        OES_AGGREGATE_PROPERTY(VisualAttributesSet, VisualAttributesSet);

        // global properties
        OES_AGGREGATE_PROPERTY(bool,   PrintBlackAndWhite);
        OES_AGGREGATE_PROPERTY(string, PrintHeader);
        OES_AGGREGATE_PROPERTY(string, PrintFooter);
        OES_AGGREGATE_PROPERTY(int,    PrintMarginMeasurement);
        OES_AGGREGATE_PROPERTY(double, PrintLeftMargin);
        OES_AGGREGATE_PROPERTY(double, PrintRightMargin);
        OES_AGGREGATE_PROPERTY(double, PrintTopMargin);
        OES_AGGREGATE_PROPERTY(double, PrintBottomMargin);

        OES_AGGREGATE_PROPERTY(string, DefFileExtension);
        OES_AGGREGATE_PROPERTY(int,    UndoLimit);
        OES_AGGREGATE_PROPERTY(int,    UndoMemLimit);
        OES_AGGREGATE_PROPERTY(bool,   UndoAfterSaving);
        OES_AGGREGATE_PROPERTY(bool,   TruncateSpaces);
        OES_AGGREGATE_PROPERTY(bool,   CursorBeyondEOL);
        OES_AGGREGATE_PROPERTY(bool,   CursorBeyondEOF);

        OES_AGGREGATE_PROPERTY(bool,   BlockKeepMarking);
        OES_AGGREGATE_PROPERTY(bool,   BlockKeepMarkingAfterDragAndDrop);
        OES_AGGREGATE_PROPERTY(bool,   BlockDelAndBSDelete);
        OES_AGGREGATE_PROPERTY(bool,   BlockTypingOverwrite);
        OES_AGGREGATE_PROPERTY(bool,   BlockTabIndent);
        OES_AGGREGATE_PROPERTY(bool,   ColBlockDeleteSpaceAfterMove);
        OES_AGGREGATE_PROPERTY(bool,   ColBlockCursorToStartAfterPaste);
        OES_AGGREGATE_PROPERTY(string, MouseSelectionDelimiters);

        OES_AGGREGATE_PROPERTY(bool,   FileLocking);
        OES_AGGREGATE_PROPERTY(bool,   FileMemMapForBig);
        OES_AGGREGATE_PROPERTY(int,    FileMemMapThreshold);
        OES_AGGREGATE_PROPERTY(bool,   FileSaveOnSwith);
        OES_AGGREGATE_PROPERTY(bool,   FileDetectChanges);
        OES_AGGREGATE_PROPERTY(bool,   FileReloadChanges);
        OES_AGGREGATE_PROPERTY(bool,   FileAutoscrollAfterReload);
        OES_AGGREGATE_PROPERTY(bool,   FileOverwriteReadonly);
        OES_AGGREGATE_PROPERTY(int,    FileBackup);
        OES_AGGREGATE_PROPERTY(string, FileBackupName);
        OES_AGGREGATE_PROPERTY(string, FileBackupDirectory);

        OES_AGGREGATE_PROPERTY(bool,   FileWholeWord);
        OES_AGGREGATE_PROPERTY(bool,   FileMatchCase);
        OES_AGGREGATE_PROPERTY(bool,   FileUseRegEx);
        OES_AGGREGATE_PROPERTY(bool,   FileSubfolders);
        OES_AGGREGATE_PROPERTY(bool,   FileSaveBefore);
        OES_AGGREGATE_PROPERTY(bool,   FileCollapse);
        OES_AGGREGATE_PROPERTY(string, FileWhatToFind);
        OES_AGGREGATE_PROPERTY(string, FileFileTypes);
        OES_AGGREGATE_PROPERTY(string, FileStartDirectory);

        OES_AGGREGATE_PROPERTY(int,    SQLPrefetchLines);
        OES_AGGREGATE_PROPERTY(string, SQLQueryTerminator);
        OES_AGGREGATE_PROPERTY(string, SQLQueryFont);

        OES_AGGREGATE_PROPERTY(bool,   PreferODBCMetaSQL);
        OES_AGGREGATE_PROPERTY(bool,   AllowMultipleInstances);
        OES_AGGREGATE_PROPERTY(bool,   NewDocOnStartup);
        OES_AGGREGATE_PROPERTY(bool,   MaximizeFirstDocument);
        OES_AGGREGATE_PROPERTY(bool,   WorkDirFollowsDocument);
        OES_AGGREGATE_PROPERTY(bool,   SaveCurPosAndBookmarks);
        OES_AGGREGATE_PROPERTY(bool,   SaveMainWinPosition);
        OES_AGGREGATE_PROPERTY(string, Locale);
        OES_AGGREGATE_PROPERTY(string, KeymapLayout);

        // class/language template support
        const TemplatePtr GetTemplate () const;

    public:
        // constructors, destructor
        InstanceSettings ();
        ~InstanceSettings ();

        // assign operator
        InstanceSettings& operator = (const InstanceSettings& other);
        void SetClassSetting (const ClassSettings* settings, bool notify = true);

    private:
        const ClassSettings* m_classSettings;

        virtual void OnSettingsChanged () { NotifySettingsChanged(); };
    };


    typedef counted_ptr<ClassSettings> ClassSettingsPtr;
    typedef std::vector<ClassSettingsPtr> ClassSettingsVector;

    /*
      An open document has a pointer to a ClassSettings instance
      so we will support removing only if a reference counter is equal 0.
      For GUI edit operations we will create a copy of SettingsManager
      and copy state to an original object after a successful validation.
    */

    class SettingsManager
    {
        friend class SettingsManagerReader;
        friend class SettingsManagerWriter;
        friend class TemplateCollectionReader;
        friend class TemplateCollectionWriter;
    public:
        // constructors, destructor
        SettingsManager ();
        SettingsManager (const SettingsManager& cloned);
        ~SettingsManager ();

        // assign operator
        SettingsManager& operator = (const SettingsManager&);

        // create, destroy a class (for GUI)
        void CreateClass (const char* name);
        void DestroyClass (const char* name);

        // global settings
        GlobalSettings&  GetGlobalSettings ()               { return m_globalSettings; }
        const GlobalSettings&  GetGlobalSettings () const   { return m_globalSettings; }

        // find methods
        const ClassSettings& FindByExt (const char*, bool _default) const;
        const ClassSettings& FindByName (const char*) const;
        const ClassSettings& GetDefaults () const;

        // enumeration
        int GetClassCount () const;
        ClassSettings& GetClassByPos (int i);
        const ClassSettings& GetClassByPos (int i) const;

        // class/language template support
        const TemplateCollection& GetTemplateCollection () const;

    private:
        GlobalSettings      m_globalSettings;
        ClassSettingsVector m_classSettingsVector;
        TemplateCollection m_templateCollection;
    };



    inline
    InstanceSettings::InstanceSettings ()
    {
        m_classSettings = 0;
        SetClassSetting(0, false);
    }

    inline
    InstanceSettings::~InstanceSettings ()
    {
        if (m_classSettings)
            m_classSettings->RemoveSubscriber(this);
    }

    inline
    InstanceSettings& InstanceSettings::operator = (const InstanceSettings& other)
    {
        SetClassSetting(other.m_classSettings, false);

        m_overridenTabSpacing    = other.m_overridenTabSpacing;
        m_overridenIndentSpacing = other.m_overridenIndentSpacing;
        m_overridenIndentType    = other.m_overridenIndentType;
        m_overridenTabExpand     = other.m_overridenTabExpand;
        m_overridenFileCreateAs  = other.m_overridenFileCreateAs;
        m_overridenFileSaveAs    = other.m_overridenFileSaveAs;
        m_overridenLineNumbers   = other.m_overridenLineNumbers;
        m_overridenVisibleSpaces = other.m_overridenVisibleSpaces;
        m_overridenNormalizeKeywords = other.m_overridenNormalizeKeywords;
        
        m_TabSpacing    = other.m_TabSpacing;
        m_IndentSpacing = other.m_IndentSpacing;
        m_IndentType    = other.m_IndentType;
        m_TabExpand     = other.m_TabExpand;
        m_FileCreateAs  = other.m_FileCreateAs;
        m_FileSaveAs    = other.m_FileSaveAs;
        m_LineNumbers   = other.m_LineNumbers;
        m_VisibleSpaces = other.m_VisibleSpaces;
        m_NormalizeKeywords = other.m_NormalizeKeywords;

        NotifySettingsChanged();

        return *this;
    }

    inline
    void InstanceSettings::SetClassSetting (const ClassSettings* settings, bool notify)
    {
        if (m_classSettings)
            m_classSettings->RemoveSubscriber(this);

        m_classSettings = settings;

        if (m_classSettings)
            m_classSettings->AddSubscriber(this);

        m_overridenTabSpacing    =
        m_overridenIndentSpacing =
        m_overridenIndentType    =
        m_overridenTabExpand     =
        m_overridenFileCreateAs  =
        m_overridenFileSaveAs    =
        m_overridenLineNumbers   =
        m_overridenVisibleSpaces = 
        m_overridenNormalizeKeywords = false;


        if (notify && settings)
            NotifySettingsChanged();
    }

    inline
    const TemplatePtr InstanceSettings::GetTemplate () const
    {
        return m_classSettings->GetTemplate();
    }

    inline
    int SettingsManager::GetClassCount () const
    {
        return (int)m_classSettingsVector.size();
    }

    inline
    const ClassSettings& SettingsManager::GetDefaults () const
    {
        return FindByExt(GetGlobalSettings().GetDefFileExtension().c_str(), true);
    }

    inline
    ClassSettings& SettingsManager::GetClassByPos (int i)
    {
        return *m_classSettingsVector.at(i);
    }

    inline
    const ClassSettings& SettingsManager::GetClassByPos (int i) const
    {
        return *m_classSettingsVector.at(i);
    }

    inline
    const TemplateCollection& SettingsManager::GetTemplateCollection () const
    {
        return m_templateCollection;
    }

};//namespace OpenEditor


#endif//__OESettings_h__
