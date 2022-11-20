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
// For license: See the file "LICENSE.txt" in the root folder
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
    return "";
  }

  // Load if not loaded earlier
  LoadFXTexts();

  // Getting the language map
  StyleLTMap::iterator lan = g_languages.find(p_language);
  if(lan == g_languages.end())
  {
    // Not a valid registered language
    return "";
  }

  StyleTexts& texts = lan->second;
  StyleTexts::iterator word = texts.find(p_id);
  if(word != texts.end())
  {
    // Word found
    return word->second;
  }

  // ID not found
  return "";
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
  const char*   m_word;
}
g_fxtext[] =
{
  { StyleLanguage::SL_ENGLISH, TXT_OK,            "ok" }
 ,{ StyleLanguage::SL_DUTCH,   TXT_OK,            "ok" }
 ,{ StyleLanguage::SL_GERMAN,  TXT_OK,            "ok" }
 ,{ StyleLanguage::SL_FRENCH,  TXT_OK,            "ok" }

 ,{ StyleLanguage::SL_ENGLISH, TXT_OK_CANCEL,     "ok cancel$can"     }
 ,{ StyleLanguage::SL_DUTCH,   TXT_OK_CANCEL,     "ok annuleren$can"  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_OK_CANCEL,     "ok abbrechen$can"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_OK_CANCEL,     "ok annuler$can"    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_ABORT_RETRY,   "cancel$can again$ok ignore$rem"          }
 ,{ StyleLanguage::SL_DUTCH,   TXT_ABORT_RETRY,   "afbreken$can nogmaals$ok negeren$rem"    }
 ,{ StyleLanguage::SL_GERMAN,  TXT_ABORT_RETRY,   "abbrechen$can nochmal$ok ignorieren$rem" }
 ,{ StyleLanguage::SL_FRENCH,  TXT_ABORT_RETRY,   "annuler$can encore$ok ignore$rem"        }

 ,{ StyleLanguage::SL_ENGLISH, TXT_YES_NO,        "yes$ok no$can"   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_YES_NO,        "ja$ok nee$can"   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_YES_NO,        "ja$ok nein$can"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_YES_NO,        "oui$ok non$can"  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_YES_NO_CANCEL, "yes$ok no$can cancel$rem"      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_YES_NO_CANCEL, "ja$ok nee$can annuleren$rem"   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_YES_NO_CANCEL, "ja$ok nein$can abbrechen$rem"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_YES_NO_CANCEL, "oui$ok non$can annuler$rem"    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_RETRY_CANCEL,  "retry$ok cancel$can"           }
 ,{ StyleLanguage::SL_DUTCH,   TXT_RETRY_CANCEL,  "nogmaals$ok annuleren$can"     }
 ,{ StyleLanguage::SL_GERMAN,  TXT_RETRY_CANCEL,  "nochmal$ok abbrechen$can"      }
 ,{ StyleLanguage::SL_FRENCH,  TXT_RETRY_CANCEL,  "encore$ok annuler$can"         }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NO_REPEAT,     "Do not repeat"       }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NO_REPEAT,     "Niet meer herhalen"  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NO_REPEAT,     "Nicht wiederholen"   }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NO_REPEAT,     "Ne répète pas"       }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_OK_CANCEL,   "ok cancel"     }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_OK_CANCEL,   "ok annuleren"  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_OK_CANCEL,   "ok abbrechen"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_OK_CANCEL,   "ok annuler"    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_ABORT_RETRY, "cancel again ignore"          }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_ABORT_RETRY, "afbreken nogmaals negeren"    }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_ABORT_RETRY, "abbrechen nochmal ignorieren" }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_ABORT_RETRY, "annuler encore ignore"        }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_YES_NO,      "yes no"   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_YES_NO,      "ja nee"   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_YES_NO,      "ja nein"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_YES_NO,      "oui non"  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_YES_NO_CANCEL, "yes no cancel"      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_YES_NO_CANCEL, "ja nee annuleren"   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_YES_NO_CANCEL, "ja nein abbrechen"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_YES_NO_CANCEL, "oui non annuler"    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_S_RETRY_CANCEL,  "retry cancel"           }
 ,{ StyleLanguage::SL_DUTCH,   TXT_S_RETRY_CANCEL,  "nogmaals annuleren"     }
 ,{ StyleLanguage::SL_GERMAN,  TXT_S_RETRY_CANCEL,  "nochmal abbrechen"      }
 ,{ StyleLanguage::SL_FRENCH,  TXT_S_RETRY_CANCEL,  "encore annuler"         }

 ,{ StyleLanguage::SL_ENGLISH, TXT_ERROR,         "ERROR"   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_ERROR,         "FOUT"    }
 ,{ StyleLanguage::SL_GERMAN,  TXT_ERROR,         "FEHLER"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_ERROR,         "ERREUR"  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_VALUE_BETWEEN, "Enter a value between %d and %d (inclusive)"                   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_VALUE_BETWEEN, "Geef een waarde op tussen %d en %d (inclusief)"                }
 ,{ StyleLanguage::SL_GERMAN,  TXT_VALUE_BETWEEN, "Geben Sie einen Wert zwischen %d und %d (einschließlich) ein"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_VALUE_BETWEEN, "Entrez une valeur entre %d et %d (inclus)"                     }

 ,{ StyleLanguage::SL_ENGLISH, TXT_PRO_LIST,      "Complete list"       }
 ,{ StyleLanguage::SL_DUTCH,   TXT_PRO_LIST,      "Volledige lijst"     }
 ,{ StyleLanguage::SL_GERMAN,  TXT_PRO_LIST,      "Vollständige Liste"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_PRO_LIST,      "Liste complète"      }

 ,{ StyleLanguage::SL_ENGLISH, TXT_SEL_LIST,      "List of selection"       }
 ,{ StyleLanguage::SL_DUTCH,   TXT_SEL_LIST,      "Lijst van de selectie"   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_SEL_LIST,      "Auswahlliste"            }
 ,{ StyleLanguage::SL_FRENCH,  TXT_SEL_LIST,      "Liste de sélection"      }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NUM_ONLY,      "You can only enter numbers in a numeric text field."                     }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NUM_ONLY,      "In een nummeriek tekstveld mogen alleen getallen ingetoetst worden."     }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NUM_ONLY,      "Sie können nur Zahlen in ein numerisches Textfeld eingeben."             }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NUM_ONLY,      "Vous ne pouvez saisir des nombres que dans un champ de texte numérique." }

 ,{ StyleLanguage::SL_ENGLISH, TXT_MANDATORY_FLD, "Mandatory field **must** be filled in!"   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_MANDATORY_FLD, "Verplicht veld **moet** ingevuld worden!" }
 ,{ StyleLanguage::SL_GERMAN,  TXT_MANDATORY_FLD, "Pflichtfeld **muss** ausgefüllt werden!"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_MANDATORY_FLD, "Le champ obligatoire doit être rempli!"   }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NO_BITMAP,     "Bitmap resource is not a MS-Windows bitmap file!"            }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NO_BITMAP,     "De bitmap resource is geen MS-Windows bitmap bestand!"       }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NO_BITMAP,     "Die Bitmap-Ressource ist keine MS-Windows-Bitmap-Datei!"     }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NO_BITMAP,     "La ressource Bitmap n'est pas un fichier Bitmap MS-Windows!" }

 ,{ StyleLanguage::SL_ENGLISH, TXT_FILE_NOT_OPEN, "Cannot open the file: %s"                  }
 ,{ StyleLanguage::SL_DUTCH,   TXT_FILE_NOT_OPEN, "Kan het bestand niet openen: %s"           }
 ,{ StyleLanguage::SL_GERMAN,  TXT_FILE_NOT_OPEN, "Die Datei kann nicht geöffnet werden: %s"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_FILE_NOT_OPEN, "Impossible d'ouvrir le fichier: %s"        }

 ,{ StyleLanguage::SL_ENGLISH, TXT_FILE_NOT_CLOSED, "Cannot close the file: %s"                   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_FILE_NOT_CLOSED, "Kan het bestand niet sluiten: %s"            }
 ,{ StyleLanguage::SL_GERMAN,  TXT_FILE_NOT_CLOSED, "Die Datei kann nicht geschlossen werden: %s" }
 ,{ StyleLanguage::SL_FRENCH,  TXT_FILE_NOT_CLOSED, "Impossible de fermer le fichier: %s"         }

 ,{ StyleLanguage::SL_ENGLISH, TXT_CALENDAR_TITLE,"Select a date"         }
 ,{ StyleLanguage::SL_DUTCH,   TXT_CALENDAR_TITLE,"Selecteer een datum"   }
 ,{ StyleLanguage::SL_GERMAN,  TXT_CALENDAR_TITLE,"Wählen Sie ein Datum"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_CALENDAR_TITLE,"Sélectionnez une date" }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NO_SPACE,      "Not enough space on the paper, or header/footers are too large to print!"                          }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NO_SPACE,      "Het papier is niet hoog genoeg, of de kop- / voetregels zijn te groot om te kunnen printen!"       }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NO_SPACE,      "Nicht genügend Platz auf dem Papier oder Kopf-/Fußzeilen sind zu groß zum Drucken!"                }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NO_SPACE,      "Espace insuffisant sur le papier ou en-tête/pied-de-page sont trop volumineux pour être imprimés!" }

 ,{ StyleLanguage::SL_ENGLISH, TXT_X_ARGUMENTS,   "More escape characters($) in printer string than arguments!"                       }
 ,{ StyleLanguage::SL_DUTCH,   TXT_X_ARGUMENTS,   "Meer escape tekens($) in de printerstring dan argumenten!"                         }
 ,{ StyleLanguage::SL_GERMAN,  TXT_X_ARGUMENTS,   "Mehr Escapezeichen ($) in der Druckerzeichenfolge als Argumente!"                  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_X_ARGUMENTS,   "Plus de caractères d'échappement ($) dans la chaîne d'imprimante que d'arguments!" }

 ,{ StyleLanguage::SL_ENGLISH, TXT_U_ARGUMENTS,   "Unknown escape characters (%c) in printer string: %s"                      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_U_ARGUMENTS,   "Onbekend escape teken (%c) in the printerstring: %s"                       }
 ,{ StyleLanguage::SL_GERMAN,  TXT_U_ARGUMENTS,   "Unbekannte Escapezeichen (%c) in der Druckerzeichenfolge: %s"              }
 ,{ StyleLanguage::SL_FRENCH,  TXT_U_ARGUMENTS,   "Caractères d'échappement inconnus (%c) dans la chaîne de l'imprimante: %s" }

 ,{ StyleLanguage::SL_ENGLISH, TXT_NO_BMP_FILE,   "No bitmap loaded from file: "              }
 ,{ StyleLanguage::SL_DUTCH,   TXT_NO_BMP_FILE,   "Geen bitmap geladen uit bestand: "         }
 ,{ StyleLanguage::SL_GERMAN,  TXT_NO_BMP_FILE,   "Keine Bitmap aus Datei geladen: "          }
 ,{ StyleLanguage::SL_FRENCH,  TXT_NO_BMP_FILE,   "Aucun bitmap chargé à partir du fichier: " }

 ,{ StyleLanguage::SL_ENGLISH, TXT_END_LISTING,   "*** End of listing ***"      }
 ,{ StyleLanguage::SL_DUTCH,   TXT_END_LISTING,   "*** Einde van de lijst ***"  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_END_LISTING,   "*** Ende der Auflistung ***" }
 ,{ StyleLanguage::SL_FRENCH,  TXT_END_LISTING,   "*** Fin de la liste ***"     }

 ,{ StyleLanguage::SL_ENGLISH, TXT_PRINT_PAGE,    "Page"    }
 ,{ StyleLanguage::SL_DUTCH,   TXT_PRINT_PAGE,    "Pagina"  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_PRINT_PAGE,    "Seite"   }
 ,{ StyleLanguage::SL_FRENCH,  TXT_PRINT_PAGE,    "Page"    }

 ,{ StyleLanguage::SL_ENGLISH, TXT_PRINT_DATE,    "Date"  }
 ,{ StyleLanguage::SL_DUTCH,   TXT_PRINT_DATE,    "Datum" }
 ,{ StyleLanguage::SL_GERMAN,  TXT_PRINT_DATE,    "Datum" }
 ,{ StyleLanguage::SL_FRENCH,  TXT_PRINT_DATE,    "Date"  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_BOOLEAN_TRUE,  "TRUE"  }
 ,{ StyleLanguage::SL_DUTCH,   TXT_BOOLEAN_TRUE,  "WAAR"  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_BOOLEAN_TRUE,  "WAHR"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_BOOLEAN_TRUE,  "VRAI"  }

 ,{ StyleLanguage::SL_ENGLISH, TXT_BOOLEAN_FALSE, "FALSE"   }
 ,{ StyleLanguage::SL_DUTCH,   TXT_BOOLEAN_FALSE, "ONWAAR"  }
 ,{ StyleLanguage::SL_GERMAN,  TXT_BOOLEAN_FALSE, "FALSCH"  }
 ,{ StyleLanguage::SL_FRENCH,  TXT_BOOLEAN_FALSE, "FAUX"    }
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

  // Load external texts
  FILE* file = nullptr;
  if(fopen_s(&file,"StyleTexts.txt","r") == 0 && file != nullptr)
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
