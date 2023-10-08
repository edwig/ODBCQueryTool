//  File. . . . : arg_shared.cpp
//  Description : Various utility classes supporting shared ownership idioms.
//  Rev . . . . :
//
//  Classes . . : 
//
//  Modification History
//
//	04-Nov-99 : Revised copyright notice                        Alan Griffiths
//	01-Sep-99 : Correct decrement_references() reptd. egcs 2.95 Alan Griffiths
//  03_Jun-99 : Refactored to remove unnecessary class          Alan Griffiths
//  28-May-99 : Original version                                Alan Griffiths
//
	/*-------------------------------------------------------------------**
	** arglib: A collection of utilities (e.g. smart pointers).          **
	** Copyright (C) 1999 Alan Griffiths.                                **
	**                                                                   **
	**  This code is part of the 'arglib' library. The latest version of **
	**  this library is available from:                                  **
	**                                                                   **
	**      http://www.octopull.demon.co.uk/arglib/                      **
	**                                                                   **
	**  This code may be used for any purpose provided that:             **
	**                                                                   **
	**    1. This complete notice is retained unchanged in any version   **
	**       of this code.                                               **
	**                                                                   **
	**    2. If this code is incorporated into any work that displays    **
	**       copyright notices then the copyright for the 'arglib'       **
	**       library must be included.                                   **
	**                                                                   **
	**  This library is distributed in the hope that it will be useful,  **
  **  but WITHOUT ANY WARRANTY; without even the implied warranty of   **
  **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.             **
	**                                                                   **
	**  You may contact the author at: alan@octopull.demon.co.uk         **
	**-------------------------------------------------------------------*/
	

#include "pch.h"
#include <Common/arg_shared.h>
#include <functional>
//#include ARG_STLHDR(functional)

namespace arg
{
	/*
	* 	Factored code for shared ownership management.  This manages
    *   ownership of the pointee and weak & strong reference counts.
    *   It manages its own deletion and needs to be specialized to
    *   delete the pointee type.
	*
	* @author Alan Griffiths
	**/
    class generic_reference::counted_ptr_body
    {
    public:
        typedef generic_reference::delete_ftn delete_ftn;

        void increment_references()         { ++refs;   }
        
        void decrement_references() throw()
        {
    	    if (0 == --refs)
    	    {
    		    do_delete(p);
    		    delete this;
    	    }
        }

        void increment_strong_references()  { ++strong; }
        
        void decrement_strong_references() throw()
        {
    	    if (0 == --strong)
    	    {
    		    do_delete(p);
			    p = 0;
    	    }
        }
        
        void* get() const                   { return p; }

        counted_ptr_body(void* pointee,     delete_ftn del) 
        : p(pointee), refs(0), strong(0), do_delete(del) {}
        
        virtual ~counted_ptr_body() throw() {}

    private:
        counted_ptr_body(const counted_ptr_body&);
        counted_ptr_body& operator=(const counted_ptr_body&);
        
        void*        p;
        unsigned int refs;
        unsigned int strong;            
        delete_ftn   do_delete; 
    };

	void generic_reference::
	swap(generic_reference& with) throw()
	{
        counted_ptr_body* tmp = with.body;
		with.body = body;
	    body = tmp;
	}


    generic_reference::
    generic_reference(void* pointee, delete_ftn do_delete)
    : body(0) 
    { 
        try
        {
            body = new counted_ptr_body(pointee, do_delete);
            body->increment_references(); 
        }
        catch(...)
        {
            do_delete(pointee);
            throw;
        }
    }


    generic_reference::
	generic_reference(const generic_reference& rhs)
    : body(rhs.body) 
    {
        body->increment_references(); 
    }

    
    generic_reference& generic_reference::
	operator=(const generic_reference& rhs)
    {
        rhs.body->increment_references();
        body->decrement_references();
        body = rhs.body;
		return *this;
    }
    

    void* generic_reference::get() const { return body->get(); }
    

    void generic_reference::
	increment_strong_references() const
    {
        body->increment_strong_references();
    }


    void generic_reference::
	decrement_strong_references() const
    {
        body->decrement_strong_references();
    }
    

    generic_reference::
	~generic_reference() throw()
    {
        body->decrement_references();
    }

    bool generic_reference::less(void* rhs) const throw()
    {
    	return std::less<void*>()(body->get(), rhs);
    }
}
