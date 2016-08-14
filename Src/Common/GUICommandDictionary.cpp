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

// 28.08.2002 improvement, double key accelerator, for example "Ctrl+K,B"
// 07.04.2003 bug fix, no menu shortcut labels on Win95,... because of SDK incompatibility

#include "stdafx.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <strstream>
#include "COMMON/AppUtilities.h"
#include "COMMON/GUICommandDictionary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Common
{
    using namespace std;

    const int MENU_ITEM_TEXT_SIZE   = 80;
    const int MAX_MENU_ITEMS_NUMBER = 1024;
    const int MENUITEMINFO_OLD_SIZE = 44;

    Command GUICommandDictionary::m_firstDblKeyAccelCommandId = 0;


    typedef map<string, Command>    DescToCmdMap;
    typedef map<Command, string>    CmdToDescMap;
    typedef map<VKey, Command>      KeyToCmdMap;
    typedef map<string, VKey>       NameToKeyMap;
    typedef map<VKey, string>       KeyToNameMap;

    static bool findCommand (const string&, Command&);
    static bool readAcceleratorSeq (istream& istr, bool& control, bool& shift, bool& alt, VKey vkKeys[2]);
    static bool addDblKeyAccel (ACCEL&, VKey vKey, Command commandId);

    static DescToCmdMap g_descToCmdMap;

    struct DblKeyAccel 
    {
        ACCEL       m_firstKeyAccel;
        KeyToCmdMap m_keyToCmdMap;
    };
    typedef vector<DblKeyAccel> DblKeyAccelVector;

    static HACCEL g_accelTable = NULL;
    
    static DblKeyAccelVector g_dblKeyAccels;

    static struct VkMap
    {
        NameToKeyMap m_nameToKey;
        KeyToNameMap m_keyToName;

        VkMap ()
        {
            insert("Backspace", VK_BACK);
            insert("Tab",       VK_TAB);
            insert("Enter",     VK_RETURN);
            insert("Return",    VK_RETURN);
            insert("Shift",     VK_SHIFT);
            insert("Ctrl",      VK_CONTROL);
            insert("Alt",       VK_MENU);
            insert("Escape",    VK_ESCAPE);
            insert("Space",     VK_SPACE);
            insert("PgUp",      VK_PRIOR);
            insert("PgDown",    VK_NEXT);
            insert("End",       VK_END);
            insert("Home",      VK_HOME);
            insert("Left",      VK_LEFT);
            insert("Up",        VK_UP);
            insert("Right",     VK_RIGHT);
            insert("Down",      VK_DOWN);
            insert("Insert",    VK_INSERT);
            insert("Delete",    VK_DELETE);
            insert("F1",        VK_F1);
            insert("F2",        VK_F2);
            insert("F3",        VK_F3);
            insert("F4",        VK_F4);
            insert("F5",        VK_F5);
            insert("F6",        VK_F6);
            insert("F7",        VK_F7);
            insert("F8",        VK_F8);
            insert("F9",        VK_F9);
            insert("F10",       VK_F10);
            insert("F11",       VK_F11);
            insert("F12",       VK_F12);
            insert("[",         219);
            insert("]",         221);
            insert("<",         VK_OEM_COMMA);
            insert(">",         VK_OEM_PERIOD);
            insert(".",         VK_OEM_COMMA);
            insert(",",         VK_OEM_PERIOD);
            insert("+",         VK_OEM_PLUS);
            insert("-",         VK_OEM_MINUS);
            insert("Num5",      VK_CLEAR);
        }

        void insert (const char* name, VKey key)
        {
            string common(name);
            m_nameToKey.insert(NameToKeyMap::value_type(common, key));
            m_keyToName.insert(KeyToNameMap::value_type(key, common));
        }

        bool find (const char* key_name, VKey& vk_key)
        {
            NameToKeyMap::const_iterator it = m_nameToKey.find(key_name);

            if (it != m_nameToKey.end())
            {
                vk_key = it->second;
                return true;
            }

            return false;
        }

        bool find (VKey vk_key, string& key_name)
        {
            KeyToNameMap::const_iterator it = m_keyToName.find(vk_key);

            if (it != m_keyToName.end())
            {
                key_name = it->second;
                return true;
            }

            return false;
        }

        void make_accel_desc (ACCEL accel, string& desc, VKey vKey = 0)
        {
            desc.erase();

            if (accel.fVirt & FCONTROL)
                desc += "Ctrl+";
            if (accel.fVirt & FALT)
                desc += "Alt+";
            if (accel.fVirt & FSHIFT)
                desc += "Shift+";

            string key_name;
            if (find(accel.key, key_name))
                desc += key_name;
            else
                desc += (char)accel.key;
            
            if (vKey)
            {
                desc += ", ";
                if (find(vKey, key_name))
                    desc += key_name;
                else
                    desc += (char)vKey;
            }

        }

    } g_vkMap;


void GUICommandDictionary::InsertCommand (const char* desc, Command id)
{
    g_descToCmdMap.insert(pair<string,Command>(desc, id));
}

bool findCommand (const string& cmd_name, Command& cmd_id)
{
    DescToCmdMap::const_iterator it = g_descToCmdMap.find(cmd_name);

    if (it != g_descToCmdMap.end())
    {
        cmd_id = it->second;
        return true;
    }

    return false;
}

bool GUICommandDictionary::BuildAcceleratorTable (const char* filename, HACCEL& accelTable)
{
    if (g_accelTable) 
    {
        DestroyAcceleratorTable(g_accelTable);
        g_accelTable = NULL;
        g_dblKeyAccels.clear();
    }

    string buffer;
    ifstream is(filename);
    ACCEL accels[MAX_MENU_ITEMS_NUMBER];

    int i;
    for (i = 0; i < MAX_MENU_ITEMS_NUMBER && getline(is, buffer); /**/)
    {
        string token;
        VKey vk_key[2];
        Command command_id;
        bool control, shift, alt;

        istrstream istr(buffer.c_str(), buffer.size());

        // skip space
        char ch;
        while (isspace(istr.peek()) && istr)
            istr.get(ch);

        while (istr.get(ch) && (isalnum(ch) || ch == '_' || ch == '.'))
            token += ch;

        if (!token.empty() && token[0] != '#') // # it's not a comment line
        {
            if (findCommand(token, command_id))
            {
                if (readAcceleratorSeq(istr, control, shift, alt, vk_key))
                {
                    accels[i].fVirt = FVIRTKEY
                                    | (control ? FCONTROL : 0)
                                    | (shift ? FSHIFT : 0)
                                    | (alt ? FALT : 0);
                    accels[i].key   = vk_key[0];

                    // single key acceleration
                    if (vk_key[1] == 0)
                    {
                        accels[i++].cmd = command_id;
                    }
                    // double key acceleration 
                    else
                    {
                        if (addDblKeyAccel(accels[i], vk_key[1], command_id))
                            i++;
                    }
                }
            }
            else
            {
                TRACE1("\tGUICommandDictionary::BuildAcceleratorTable: \"%s\" not found!\n", token.c_str());
                _ASSERTE(0);
            }
        }
    }

    g_accelTable = CreateAcceleratorTable(accels, i);
    TRACE1("\tGUICommandDictionary::BuildAcceleratorTable: accelerator table size is %d\n", i);

    accelTable = g_accelTable;

    return accelTable != NULL ? true : false;
}

//
// recognaze only [<Control>+][<Alt>+][<Shift>+]('Char'|<Key>)[,'Char']
//
bool readAcceleratorSeq (istream& istr, bool& control, bool& shift, bool& alt, VKey vk_key[2])
{
    char ch;
    char delim[2] = { '+', ',' };
    int  index = 0;

    shift   = false;
    control = false;
    alt     = false;
    vk_key[0] = vk_key[1] = 0;

    // skip space
    while (isspace(istr.peek()) && istr)
        istr.get(ch);

    if (istr.eof())
        return false; // it's ok

    for (int i(0); istr; i++)
    {
        if (i) // second token & all next
        {
            while (isspace(istr.peek()) && istr)
                istr.get(ch);

            // expect '+' or ','
            if(istr.peek() == delim[index] && istr)
                istr.get(ch);

            while (isspace(istr.peek()) && istr)
                istr.get(ch);
        }

        string token;

        // read word
        for (int j(0); 
            !isspace(istr.peek()) 
            && !istr.eof() 
            && (!j || isalnum(istr.peek()))
            ; j++)
        {
            istr.get(ch);
            token += ch;
        }

        // recognise
        if (g_vkMap.find(token.c_str(), vk_key[index]))
        {
            switch (vk_key[index])
            {
            default:
                index++;
                break;

            case VK_SHIFT:
                shift = true;
                break;
            case VK_CONTROL:
                control = true;
                break;
            case VK_MENU:
                alt = true;
                break;
            }
            token.erase();
        }
        else
        {
            // no ckeck of line end
            if (token.size() == 1)
                vk_key[index++] = token[0];
            else if (token.size() == 0)
                break;
            else
                _ASSERTE(0);
        }
    }

    return index ? true : false;
}

    struct MenuItem
    {
        HMENU   m_hmenu;
        Command m_command;
        string  m_text;
    };

    typedef vector<MenuItem> MenuItemVector;

static void walk_menu (HMENU& hMenu, MenuItemVector& menuitems)
{
    MENUITEMINFO mii;
    memset(&mii, 0, sizeof mii);
    mii.cbSize = MENUITEMINFO_OLD_SIZE;  // 07.04.2003 bug fix, no menu shortcut labels on Win95,... because of SDK incompatibility
    mii.fMask  = MIIM_SUBMENU | MIIM_DATA | MIIM_ID | MIIM_TYPE;

    int nitems = GetMenuItemCount(hMenu);
    menuitems.reserve(menuitems.size() + nitems);

    for (int i = 0; i < nitems; i++)
    {
        char buffer[MENU_ITEM_TEXT_SIZE];
        mii.dwTypeData = buffer;
        mii.cch = sizeof buffer;

        if (GetMenuItemInfo(hMenu, i, TRUE, &mii))
        {
            _ASSERTE(mii.cch < sizeof buffer);

            buffer[sizeof buffer - 1] = '\0';
            const char* text = (const char*)mii.dwTypeData;

            if (mii.hSubMenu != NULL)
            {
                //TRACE1("\t%s\n", text);

                walk_menu(mii.hSubMenu, menuitems);
            }
            else if (mii.fType == MFT_STRING)
            {
                    //TRACE1("\t\t%s\n", text);
                MenuItem menuitem;

                menuitem.m_hmenu   = hMenu;
                menuitem.m_command = static_cast<Command>(mii.wID);
                menuitem.m_text    = text;

                menuitems.push_back(menuitem);
            }
        }
    }
}

void GUICommandDictionary::AddAccelDescriptionToMenu (HMENU& hMenu)
{
    MenuItemVector menuitems;
    TRACE("Walk menu\n");
    walk_menu(hMenu, menuitems);
    TRACE1("\tGUICommandDictionary::AddAccelDescriptionToMenu: number of menuitems is %d\n", menuitems.size());

    // allocate and get acceleration table by handle
    ACCEL AccelTable[MAX_MENU_ITEMS_NUMBER];
    memset(AccelTable, 0, sizeof(AccelTable));
    int size = CopyAcceleratorTable(g_accelTable, AccelTable, sizeof AccelTable/ sizeof AccelTable[0]);

    CmdToDescMap accelDesc;
    TRACE("\tGUICommandDictionary::AddAccelDescriptionToMenu: Walk accelerator table, size is %d\n", size);
    for (int i(0); i < size; i++)
    {
        string desc;
        g_vkMap.make_accel_desc(AccelTable[i], desc);

        CmdToDescMap::iterator it = accelDesc.find(AccelTable[i].cmd);
        
        if (it == accelDesc.end())
            accelDesc.insert(CmdToDescMap::value_type(AccelTable[i].cmd, desc));
        else
        {
            it->second += "; "; 
            it->second += desc.c_str();
        }
    }

    DblKeyAccelVector::const_iterator it = g_dblKeyAccels.begin();
    for (; it != g_dblKeyAccels.end(); it++)
    {
        KeyToCmdMap::const_iterator it2 = it->m_keyToCmdMap.begin();
        for (; it2 != it->m_keyToCmdMap.end(); it2++)
        {
            string desc;
            g_vkMap.make_accel_desc(it->m_firstKeyAccel, desc, it2->first);

            if (accelDesc.find(it2->second) == accelDesc.end())
                accelDesc.insert(map<int, string>::value_type(it2->second, desc));
            else
                TRACE1("\tWARNING: entry \"%s\" already exists!!!\n", desc.c_str());
        }
    }
    {
    //TRACE("Change menu labels\n");
    for (MenuItemVector::iterator it = menuitems.begin(); it != menuitems.end(); ++it)
        {
        MenuItem& menuitem = *it;
            // for reinitalizing
        string::size_type pos = menuitem.m_text.find('\t');
            if (pos != string::npos) 
            menuitem.m_text.resize(pos);

        CmdToDescMap::const_iterator descIt = accelDesc.find(menuitem.m_command);
        if (descIt != accelDesc.end())
            menuitem.m_text += '\t' + descIt->second;

            MENUITEMINFO mii;
            memset(&mii, 0, sizeof mii);
            mii.cbSize     = MENUITEMINFO_OLD_SIZE; // 07.04.2003 bug fix, no menu shortcut labels on Win95,... because of SDK incompatibility
            mii.fMask      = MIIM_TYPE;
        mii.dwTypeData = const_cast<char*>(menuitem.m_text.c_str());
        mii.cch        = (int)menuitem.m_text.size();

        VERIFY(SetMenuItemInfo(hMenu, menuitem.m_command, FALSE, &mii));
    }
    }

}

bool addDblKeyAccel (ACCEL& accel, VKey vKey, Command commandId)
{
    bool retCode = false;
    DblKeyAccelVector::iterator it = g_dblKeyAccels.begin();

    for (; it != g_dblKeyAccels.end(); it++)
        if (it->m_firstKeyAccel.fVirt == accel.fVirt 
            && it->m_firstKeyAccel.key == accel.key)
            break;

    if (it == g_dblKeyAccels.end())
    {
        g_dblKeyAccels.resize(g_dblKeyAccels.size()+1);
        g_dblKeyAccels.rbegin()->m_firstKeyAccel = accel;
        g_dblKeyAccels.rbegin()->m_keyToCmdMap[vKey] = commandId;
        accel.cmd = static_cast<Command>(GUICommandDictionary::m_firstDblKeyAccelCommandId 
            + g_dblKeyAccels.size() - 1);
        retCode = true;
    }
    else
    {
        it->m_keyToCmdMap[vKey] = commandId;
        accel.cmd = static_cast<WORD>(GUICommandDictionary::m_firstDblKeyAccelCommandId 
            + (it - g_dblKeyAccels.begin()));
    }

    return retCode;
}

bool GUICommandDictionary::GetDblKeyDescription (int dblKeyAccelInx, string& desc, VKey vKey)
{
    desc.empty();
    if (dblKeyAccelInx < static_cast<int>(g_dblKeyAccels.size()))
    {
        g_vkMap.make_accel_desc(g_dblKeyAccels[dblKeyAccelInx].m_firstKeyAccel, desc, vKey);
        return true;
    }
    return false;
}

bool GUICommandDictionary::GetDblKeyAccelCmdId (int dblKeyAccelInx, VKey vKey, Command& commandId)
{
    if (dblKeyAccelInx < static_cast<int>(g_dblKeyAccels.size()))
    {
        KeyToCmdMap::const_iterator it = g_dblKeyAccels[dblKeyAccelInx].m_keyToCmdMap.find(vKey);
        if (it != g_dblKeyAccels[dblKeyAccelInx].m_keyToCmdMap.end())
        {
            commandId = it->second;
            return true;
        }
    }
    return false;
}

bool GUICommandDictionary::GetCommandAccelLabel (Command command, string& label)
{
    ACCEL AccelTable[MAX_MENU_ITEMS_NUMBER];
    memset(AccelTable, 0, sizeof(AccelTable));

    int size = CopyAcceleratorTable(g_accelTable, AccelTable, sizeof AccelTable/ sizeof AccelTable[0]);

    int i;
    for (i = 0; i < size; i++)
    {
        if (AccelTable[i].cmd == command)
        {
            g_vkMap.make_accel_desc(AccelTable[i], label);
            return true;
        }
    }

    size = (int)g_dblKeyAccels.size();
    for (i = 0; i < size; i++)
    {
        KeyToCmdMap::const_iterator 
            it = g_dblKeyAccels[i].m_keyToCmdMap.begin(),
            end = g_dblKeyAccels[i].m_keyToCmdMap.end();

        for (; it != end; ++it)
        {
            if(it->second == command)
            {
                GetDblKeyDescription(i, label, it->first);
                return true;
            }
        }
    }
    
    return false;
}

};//namespace Common
