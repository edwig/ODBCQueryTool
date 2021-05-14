/** \file arg_compiler.h
*   
*   Identify compiler and define work-arounds
* 
*  The following work-arounds are defined:
* 
*   o  Some messages relating to compiler deficiencies are suppressed
* 
*   o 	Macros "ARG_CSTDHDR", "ARG_CPPSTDHDR" and "ARG_STLHDR" are defined
*   	to address variations in the names used for header files.
*       (I use a separate macros for the STL subset of the standard library
*   	as this may be sourced separately.)
* 
*   Modification History
*<PRE>
*   06-Apr-00 : Added ARG_COMPILER_NO_COVARIENT_RETURNS		    Alan Griffiths
*   21-Mar-00 : Removed ARG_SMEYER_VIRTUAL						Alan Griffiths
*   04-Nov-99 : Revised copyright notice						Alan Griffiths
*   18-May-99 : Port to M$VC++ v5                               Alan Griffiths
*   15-May-99 : Added ARG_SMEYER_VIRTUAL						Alan Griffiths
*   24-Apr-99 : Original version                                Alan Griffiths
*</PRE>
**/
/*
* -----------------------------------------------------------------------------
*  arglib: A collection of utilities. (E.g. smart pointers).
*  Copyright (C) 1999, 2000 Alan Griffiths.
* 
*   This code is part of the 'arglib' library. The latest version of
*   this library is available from:
* 
*       http:* www.octopull.demon.co.uk/arglib/
* 
*   This code may be used for any purpose provided that:
* 
*     1. This complete notice is retained unchanged in any version
*        of this code.
* 
*     2. If this code is incorporated into any work that displays
*        copyright notices then the copyright for the 'arglib'
*        library must be included.
* 
*   This library is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* 
*   You may contact the author at: alan@octopull.demon.co.uk
* -----------------------------------------------------------------------------	
*/

#ifndef ARG_COMPILER_H
#define ARG_COMPILER_H

#if defined(_MSC_VER) && (_MSC_VER < 1100)
	
	#error There are too many problems with language support in M$VC++ 4.x

#elif defined(_MSC_VER)     // M$VC++ 5 or later
	// C++ Exception Specification ignored
	#pragma warning(disable : 4290)	

	// copy constructor could not be generated
	#pragma warning(disable : 4511)	

	// assignment operator could not be generated
	#pragma warning(disable : 4512)	

	// typdef used as synonym for class name
	#pragma warning(disable : 4097)	

	// unreferenced inline function has been removed
	#pragma warning(disable : 4514)

    // multiple copy constructors specified
	#pragma warning(disable : 4521)
    
    // multiple assignment operators specified
	#pragma warning(disable : 4522)

    // identifier was truncated to '255' characters in the debug information
	#pragma warning(disable : 4786)
    
    // forcing value to 'true' or 'false' (performance warning)
	#pragma warning(disable : 4800)
    
	// Standard headers for C library only exist in <*.h> form
	#define ARG_CSTDHDR(name) <name##.h>

    // Reduce problems with M$VC++'s crazed name lookup    
    namespace arg {}
    namespace std { using namespace arg; }
    
    #define ARG_COMPILER_NO_COVARIENT_RETURNS

#elif defined(__GNUC__)		// egcs
    
#elif defined(__BORLANDC__)		// Borland
	#define ARG_CSTDHDR(name) <name##.h>
#endif

// Default definitions for "ARG_CSTDHDR", "ARG_CPPSTDHDR" and "ARG_STLHDR".
#ifndef ARG_CSTDHDR
	#define ARG_CSTDHDR(name) <c##name>
#endif

#ifndef ARG_CPPSTDHDR
	#define ARG_CPPSTDHDR(name) <name>
#endif

#ifndef ARG_STLHDR
	#define ARG_STLHDR(name) <name>
#endif

#endif
