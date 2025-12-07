////////////////////////////////////////////////////////////////////////
//
// File: StyleTexts.cpp
// Function: Styled progress control
//
//   _____ _         _ _             ______                                           _    
//  / ____| |       | (_)           |  ____|                                         | |   
// | (___ | |_ _   _| |_ _ __   __ _| |__ _ __ __ _ _ __ ___   _____      _____  _ __| | __
//  \___ \| __| | | | | | '_ \ / _` |  __| '__/ _` | '_ ` _ \ / _ \ \ /\ / / _ \| '__| |/ /
//  ____) | |_| |_| | | | | | | (_| | |  | | | (_| | | | | | |  __/\ V  V / (_) | |  |   < 
// |_____/ \__|\__, |_|_|_| |_|\__, |_|  |_|  \__,_|_| |_| |_|\___| \_/\_/ \___/|_|  |_|\_\
//              __/ |           __/ |                                                      
//             |___/           |___/                                                       
//
//
// Author: ir. W.E. Huisman
// For license: See the file _T("LICENSE.txt" in the root folder
//
#include "stdafx.h"
#include "StyleTexts.h"
#include <map>

using StyleTexts = std::map<int,CString>;
using StyleLTMap = std::map<StyleLanguage,StyleTexts>;

// Default language at startup is English
static StyleLanguage g_FXLanguage = StyleLanguage::SL_ENGLISH;
static StyleLTMap    g_languages;
static bool          g_loaded = false;

// Forward declaration
static void LoadFXTexts();

// Sets the currently used (default) language for the style framework
void  
SetStyleFxLanguage(StyleLanguage p_language /*= StyleLanguage::SL_ENGLISH*/)
{
  if(p_language > StyleLanguage::SL_DEFAULT && p_language < StyleLanguage::SL_LAST)
  {
    g_FXLanguage = p_language;
  }
  else
  {
    g_FXLanguage = StyleLanguage::SL_ENGLISH;
  }
}

// And retrieving the currently set language
StyleLanguage 
GetStyleFxLanguage()
{
  return g_FXLanguage;
}

bool
AddStyleText(int p_id,CString p_text,StyleLanguage p_language /*= StyleLanguage::SL_DEFAULT*/)
{
  // Check if a valid language
  if(p_language == StyleLanguage::SL_DEFAULT)
  {
    p_language = g_FXLanguage;
  }
  if(p_language <= StyleLanguage::SL_DEFAULT || p_language >= StyleLanguage::SL_LAST)
  {
    return false;
  }

  // Getting the language map
  StyleLTMap::iterator lan = g_languages.find(p_language);
  if(lan == g_languages.end())
  {
    g_languages[p_language] = StyleTexts();
    lan = g_languages.find(p_language);
  }

  // Check for duplicates
  StyleTexts& texts = lan->second;
  StyleTexts::iterator word = texts.find(p_id);
  if(word != texts.end())
  {
    // Word already added
    return false;
  }

  // Keep the text
  texts[p_id] = p_text;
  return true;
}

CString 
GetStyleText(int p_id,StyleLanguage p_language /*= StyleLanguage::SL_DEFAULT*/)
{
  // Check if a valid language
  if(p_language == StyleLanguage::SL_DEFAULT)
  {
    p_language = g_FXLanguage;
  }
  if(p_language <= StyleLanguage::SL_DEFAULT || p_language >= StyleLanguage::SL_LAST)
  {
    return _T("");
  }

  // Load if not loaded earlier
  LoadFXTexts();

  // Getting the language map
  StyleLTMap::iterator lan = g_languages.find(p_language);
  if(lan == g_languages.end())
  {
    // Not a valid registered language
    return _T("");
  }

  StyleTexts& texts = lan->second;
  StyleTexts::iterator word = texts.find(p_id);
  if(word != texts.end())
  {
    // Word found
    return word->second;
  }

  // ID not found
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
//
// Standard texts needed by the StyleFramework itself
//
//////////////////////////////////////////////////////////////////////////

struct _fxtext
{
  StyleLanguage m_language;
  int           m_id;
  LPCTSTR       m_word;
}
g_fxtext[] =
{
  { StyleLanguage::SL_ENGLISH, TXT_OK,            _T("ok") }
 ,{ StyleLanguage::SL_DUTCH,   TXT_OK,            _T("ok") }
 ,{ StyleLanguage::SL_GERMAN,  TXT_OK,            _T("ok") }
 ,{ StyleLanguage::SL_FRENCH,  TXT_OK,            _T("ok") }

 ,{ StyleLanguage::SL_ENGLISH, TXT_OK_CANCEL,     _T("ok cancel$can")     }
 ,{ StyleLanguage::SL_DUTCH,   TXT_OK_CANCEL,     _T("ok annuleren$can")  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_OK_CANCEL,     _T("ok abbrechen$can")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_OK_CANCEL,     _T("ok annuler$can")    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_ABORT_RETRY,   _T("cancel$can again$ok ignore$rem")          }
 ,{ StyleLanguage::SL_DUTCH,   TXT_ABORT_RETRY,   _T("afbreken$can nogmaals$ok negeren$rem")    }
 ,{ StyleLanguage::SL_GERMAN,  TXT_ABORT_RETRY,   _T("abbrechen$can nochmal$ok ignorieren$rem") }
 ,{ StyleLanguage::SL_FRENCH,  TXT_ABORT_RETRY,   _T("annuler$can encore$ok ignore$rem")        }

 ,{ StyleLanguage::SL_ENGLISH, TXT_YES_NO,        _T("yes$ok no$can")   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_YES_NO,        _T("ja$ok nee$can")   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_YES_NO,        _T("ja$ok nein$can")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_YES_NO,        _T("oui$ok non$can")  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_YES_NO_CANCEL, _T("yes$ok no$can cancel$rem")      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_YES_NO_CANCEL, _T("ja$ok nee$can annuleren$rem")   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_YES_NO_CANCEL, _T("ja$ok nein$can abbrechen$rem")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_YES_NO_CANCEL, _T("oui$ok non$can annuler$rem")    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_RETRY_CANCEL,  _T("retry$ok cancel$can")           }
 ,{ StyleLanguage::SL_DUTCH,   TXT_RETRY_CANCEL,  _T("nogmaals$ok annuleren$can")     }
 ,{ StyleLanguage::SL_GERMAN,  TXT_RETRY_CANCEL,  _T("nochmal$ok abbrechen$can")      }
 ,{ StyleLanguage::SL_FRENCH,  TXT_RETRY_CANCEL,  _T("encore$ok annuler$can")         }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NO_REPEAT,     _T("Do not repeat")       }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NO_REPEAT,     _T("Niet meer herhalen")  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NO_REPEAT,     _T("Nicht wiederholen")   }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NO_REPEAT,     _T("Ne répète pas")       }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_OK_CANCEL,   _T("ok cancel")     }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_OK_CANCEL,   _T("ok annuleren")  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_OK_CANCEL,   _T("ok abbrechen")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_OK_CANCEL,   _T("ok annuler")    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_ABORT_RETRY, _T("cancel again ignore")          }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_ABORT_RETRY, _T("afbreken nogmaals negeren")    }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_ABORT_RETRY, _T("abbrechen nochmal ignorieren") }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_ABORT_RETRY, _T("annuler encore ignore")        }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_YES_NO,      _T("yes no")   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_YES_NO,      _T("ja nee")   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_YES_NO,      _T("ja nein")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_YES_NO,      _T("oui non")  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_YES_NO_CANCEL, _T("yes no cancel")      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_YES_NO_CANCEL, _T("ja nee annuleren")   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_YES_NO_CANCEL, _T("ja nein abbrechen")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_YES_NO_CANCEL, _T("oui non annuler")    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_RETRY_CANCEL,  _T("retry cancel")           }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_RETRY_CANCEL,  _T("nogmaals annuleren")     }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_RETRY_CANCEL,  _T("nochmal abbrechen")      }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_RETRY_CANCEL,  _T("encore annuler")         }

 ,{ StyleLanguage::SL_ENGLISH, TXT_ERROR,         _T("ERROR")   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_ERROR,         _T("FOUT")    }
 ,{ StyleLanguage::SL_GERMAN,  TXT_ERROR,         _T("FEHLER")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_ERROR,         _T("ERREUR")  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_VALUE_BETWEEN, _T("Enter a value between %d and %d (inclusive)")                   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_VALUE_BETWEEN, _T("Geef een waarde op tussen %d en %d (inclusief)")                }
 ,{ StyleLanguage::SL_GERMAN,  TXT_VALUE_BETWEEN, _T("Geben Sie einen Wert zwischen %d und %d (einschließlich) ein")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_VALUE_BETWEEN, _T("Entrez une valeur entre %d et %d (inclus)")                     }

 ,{ StyleLanguage::SL_ENGLISH, TXT_PRO_LIST,      _T("Complete list")       }
 ,{ StyleLanguage::SL_DUTCH,   TXT_PRO_LIST,      _T("Volledige lijst")     }
 ,{ StyleLanguage::SL_GERMAN,  TXT_PRO_LIST,      _T("Vollständige Liste")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_PRO_LIST,      _T("Liste complète")      }

 ,{ StyleLanguage::SL_ENGLISH, TXT_SEL_LIST,      _T("List of selection")       }
 ,{ StyleLanguage::SL_DUTCH,   TXT_SEL_LIST,      _T("Lijst van de selectie")   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_SEL_LIST,      _T("Auswahlliste")            }
 ,{ StyleLanguage::SL_FRENCH,  TXT_SEL_LIST,      _T("Liste de sélection")      }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NUM_ONLY,      _T("You can only enter numbers in a numeric text field.")                     }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NUM_ONLY,      _T("In een nummeriek tekstveld mogen alleen getallen ingetoetst worden.")     }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NUM_ONLY,      _T("Sie können nur Zahlen in ein numerisches Textfeld eingeben.")             }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NUM_ONLY,      _T("Vous ne pouvez saisir des nombres que dans un champ de texte numérique.") }

 ,{ StyleLanguage::SL_ENGLISH, TXT_MANDATORY_FLD, _T("Mandatory field **must** be filled in!")   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_MANDATORY_FLD, _T("Verplicht veld **moet** ingevuld worden!") }
 ,{ StyleLanguage::SL_GERMAN,  TXT_MANDATORY_FLD, _T("Pflichtfeld **muss** ausgefüllt werden!")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_MANDATORY_FLD, _T("Le champ obligatoire doit être rempli!")   }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NO_BITMAP,     _T("Bitmap resource is not a MS-Windows bitmap file!")            }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NO_BITMAP,     _T("De bitmap resource is geen MS-Windows bitmap bestand!")       }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NO_BITMAP,     _T("Die Bitmap-Ressource ist keine MS-Windows-Bitmap-Datei!")     }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NO_BITMAP,     _T("La ressource Bitmap n'est pas un fichier Bitmap MS-Windows!") }

 ,{ StyleLanguage::SL_ENGLISH, TXT_FILE_NOT_OPEN, _T("Cannot open the file: %s")                  }
 ,{ StyleLanguage::SL_DUTCH,   TXT_FILE_NOT_OPEN, _T("Kan het bestand niet openen: %s")           }
 ,{ StyleLanguage::SL_GERMAN,  TXT_FILE_NOT_OPEN, _T("Die Datei kann nicht geöffnet werden: %s")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_FILE_NOT_OPEN, _T("Impossible d'ouvrir le fichier: %s")        }

 ,{ StyleLanguage::SL_ENGLISH, TXT_FILE_NOT_CLOSED, _T("Cannot close the file: %s")                   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_FILE_NOT_CLOSED, _T("Kan het bestand niet sluiten: %s")            }
 ,{ StyleLanguage::SL_GERMAN,  TXT_FILE_NOT_CLOSED, _T("Die Datei kann nicht geschlossen werden: %s") }
 ,{ StyleLanguage::SL_FRENCH,  TXT_FILE_NOT_CLOSED, _T("Impossible de fermer le fichier: %s")         }

 ,{ StyleLanguage::SL_ENGLISH, TXT_CALENDAR_TITLE,_T("Select a date")         }
 ,{ StyleLanguage::SL_DUTCH,   TXT_CALENDAR_TITLE,_T("Selecteer een datum")   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_CALENDAR_TITLE,_T("Wählen Sie ein Datum")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_CALENDAR_TITLE,_T("Sélectionnez une date") }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NO_SPACE,      _T("Not enough space on the paper, or header/footers are too large to print!")                          }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NO_SPACE,      _T("Het papier is niet hoog genoeg, of de kop- / voetregels zijn te groot om te kunnen printen!")       }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NO_SPACE,      _T("Nicht genügend Platz auf dem Papier oder Kopf-/Fußzeilen sind zu groß zum Drucken!")                }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NO_SPACE,      _T("Espace insuffisant sur le papier ou en-tête/pied-de-page sont trop volumineux pour être imprimés!") }

 ,{ StyleLanguage::SL_ENGLISH, TXT_X_ARGUMENTS,   _T("More escape characters($) in printer string than arguments!")                       }
 ,{ StyleLanguage::SL_DUTCH,   TXT_X_ARGUMENTS,   _T("Meer escape tekens($) in de printerstring dan argumenten!")                         }
 ,{ StyleLanguage::SL_GERMAN,  TXT_X_ARGUMENTS,   _T("Mehr Escapezeichen ($) in der Druckerzeichenfolge als Argumente!")                  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_X_ARGUMENTS,   _T("Plus de caractères d'échappement ($) dans la chaîne d'imprimante que d'arguments!") }

 ,{ StyleLanguage::SL_ENGLISH, TXT_U_ARGUMENTS,   _T("Unknown escape characters (%c) in printer string: %s")                      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_U_ARGUMENTS,   _T("Onbekend escape teken (%c) in the printerstring: %s")                       }
 ,{ StyleLanguage::SL_GERMAN,  TXT_U_ARGUMENTS,   _T("Unbekannte Escapezeichen (%c) in der Druckerzeichenfolge: %s")              }
 ,{ StyleLanguage::SL_FRENCH,  TXT_U_ARGUMENTS,   _T("Caractères d'échappement inconnus (%c) dans la chaîne de l'imprimante: %s") }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NO_BMP_FILE,   _T("No bitmap loaded from file: _T(")              }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NO_BMP_FILE,   _T("Geen bitmap geladen uit bestand: _T(")         }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NO_BMP_FILE,   _T("Keine Bitmap aus Datei geladen: _T(")          }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NO_BMP_FILE,   _T("Aucun bitmap chargé à partir du fichier: _T(") }

 ,{ StyleLanguage::SL_ENGLISH, TXT_END_LISTING,   _T("*** End of listing ***")      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_END_LISTING,   _T("*** Einde van de lijst ***")  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_END_LISTING,   _T("*** Ende der Auflistung ***") }
 ,{ StyleLanguage::SL_FRENCH,  TXT_END_LISTING,   _T("*** Fin de la liste ***")     }

 ,{ StyleLanguage::SL_ENGLISH, TXT_PRINT_PAGE,    _T("Page")    }
 ,{ StyleLanguage::SL_DUTCH,   TXT_PRINT_PAGE,    _T("Pagina")  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_PRINT_PAGE,    _T("Seite")   }
 ,{ StyleLanguage::SL_FRENCH,  TXT_PRINT_PAGE,    _T("Page")    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_PRINT_DATE,    _T("Date")  }
 ,{ StyleLanguage::SL_DUTCH,   TXT_PRINT_DATE,    _T("Datum") }
 ,{ StyleLanguage::SL_GERMAN,  TXT_PRINT_DATE,    _T("Datum") }
 ,{ StyleLanguage::SL_FRENCH,  TXT_PRINT_DATE,    _T("Date")  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_BOOLEAN_TRUE,  _T("TRUE")  }
 ,{ StyleLanguage::SL_DUTCH,   TXT_BOOLEAN_TRUE,  _T("WAAR")  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_BOOLEAN_TRUE,  _T("WAHR")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_BOOLEAN_TRUE,  _T("VRAI")  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_BOOLEAN_FALSE, _T("FALSE")   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_BOOLEAN_FALSE, _T("ONWAAR")  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_BOOLEAN_FALSE, _T("FALSCH")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_BOOLEAN_FALSE, _T("FAUX")    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_WIZARD,        _T("Wizard")      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_WIZARD,        _T("Wizard")      }
 ,{ StyleLanguage::SL_GERMAN,  TXT_WIZARD,        _T("Assistenten") }
 ,{ StyleLanguage::SL_FRENCH,  TXT_WIZARD,        _T("Assistant")   }

 ,{ StyleLanguage::SL_ENGLISH, TXT_CANCEL_WIZARD, _T("Do you want to leave the wizard?")      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_CANCEL_WIZARD, _T("Wilt u deze wizard verlaten?")          }
 ,{ StyleLanguage::SL_GERMAN,  TXT_CANCEL_WIZARD, _T("Möchten Sie den Assistenten beenden?")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_CANCEL_WIZARD, _T("Voulez-vous quitter l'assistant ?")     }

 ,{ StyleLanguage::SL_ENGLISH, TXT_TOAST_TOOMUCH, _T("More than 25 toasts")             }
 ,{ StyleLanguage::SL_DUTCH,   TXT_TOAST_TOOMUCH, _T("Meer dan 25 meldingen")           }
 ,{ StyleLanguage::SL_GERMAN,  TXT_TOAST_TOOMUCH, _T("Mehr als 25 Benachrichtigungen")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_TOAST_TOOMUCH, _T("Plus de 25 notifications")        }

 ,{ StyleLanguage::SL_ENGLISH, TXT_TOAST_WAITTODIE, _T("Wait until all toasts die out...")                              }
 ,{ StyleLanguage::SL_DUTCH,   TXT_TOAST_WAITTODIE, _T("Wacht totdat alle meldingen zijn verdwenen...")                 }
 ,{ StyleLanguage::SL_GERMAN,  TXT_TOAST_WAITTODIE, _T("Warten Sie, bis alle Benachrichtigungen sind verschwunden...")  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_TOAST_WAITTODIE, _T("Attendez que toutes les notifications disparaissent...")        }

 ,{ StyleLanguage::SL_ENGLISH, TXT_TOAST_FALLBACK, _T("Application message")          }
 ,{ StyleLanguage::SL_DUTCH,   TXT_TOAST_FALLBACK, _T("Applicatie melding")           }
 ,{ StyleLanguage::SL_GERMAN,  TXT_TOAST_FALLBACK, _T("Bewerbungsbenachrichtigung")   }
 ,{ StyleLanguage::SL_FRENCH,  TXT_TOAST_FALLBACK, _T("Notification de candidature")  }
};

static void LoadFXTexts()
{
  // See if already done
  if(g_loaded)
  {
    return;
  }

  // Load internal texts
  for(int index = 0;index < sizeof(g_fxtext) / sizeof(struct _fxtext);++index)
  {
    AddStyleText(g_fxtext[index].m_id,g_fxtext[index].m_word,g_fxtext[index].m_language);
  }

  // Load external texts: Plain ASCII file!
  FILE* file = nullptr;
  if(_tfopen_s(&file,_T("StyleTexts.txt"),_T("r")) == 0 && file != nullptr)
  {
    char buffer[1024];
    while(fgets(buffer,1024,file))
    {
      int len = (int) strlen(buffer);
      if(len > 0 && buffer[len-1] == '\n')
      {
        buffer[len - 1] = 0;
      }
      if(buffer[0] == '#')
      {
        continue;
      }
      char* pos      = strchr(buffer,' ');
      int   id       = atoi(buffer);
      char  language = *(++pos);
      char* word     = pos + 2;

      StyleLanguage lan = StyleLanguage::SL_DEFAULT;
      switch(language)
      {
        case 'E': lan = StyleLanguage::SL_ENGLISH; break;
        case 'D': lan = StyleLanguage::SL_DUTCH;   break;
        case 'G': lan = StyleLanguage::SL_GERMAN;  break;
        case 'F': lan = StyleLanguage::SL_FRENCH;  break;
      }
      if(lan != StyleLanguage::SL_DEFAULT)
      {
        AddStyleText(id,word,lan);
      }
    }
    fclose(file);
  }
  g_loaded = true;
}
