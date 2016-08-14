/** \file arg_shared.h
*   
*   Various utility classes supporting shared ownership idioms.
* 
*   Modification History
*<PRE>
*   06-Apr-00 : Reworked documentation following user feedback  Alan Griffiths
*   04-Nov-99 : Revised copyright notice						Alan Griffiths
*   20-Oct-99 : Disable unsafe conversions see ARG_UNSAFE_XVERT	Alan Griffiths
*   26-Aug-99 : Provided overload of std::swap()				Alan Griffiths
*   28-May-99 : removed factored code from inlining             Alan Griffiths
*   18-May-99 : Port to M$VC++ v5                               Alan Griffiths
*   15-May-99 : Partially corrected type safety					Alan Griffiths
*   14-May-99 : Original version                                Alan Griffiths
*</PRE>
**/

#ifndef ARG_SHARED_H
#define ARG_SHARED_H

#ifndef ARG_COMPILER_H
#include "arg_compiler.h"
#endif

/**
*   arglib: A collection of utilities. (E.g. smart pointers).
*   Copyright (C) 1999, 2000 Alan Griffiths.
*
*   This code is part of the 'arglib' library. The latest version of
*   this library is available from:
*
*      http:*www.octopull.demon.co.uk/arglib/
*
*   This code may be used for any purpose provided that:
*
*   1. This complete notice is retained unchanged in any version
*       of this code.
*
*   2. If this code is incorporated into any work that displays
*       copyright notices then the copyright for the 'arglib'
*       library must be included.
*
*   This library is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*   @author alan@octopull.demon.co.uk
**/
namespace arg
{
	/**
    *   Implementation class supporting counted_ptr/uncounted_ptr templates.
    *
    *   Factored code for shared ownership management.  This manages the
    *   shared body (containing pointer to referenced body and the 
    *   strong reference count).
    *
    *   @author Alan Griffiths
    **/
    class generic_reference
    {
	public:
        typedef void (*delete_ftn)(void* pointee);
        
    protected:
        
		/// Swap state with another reference
        void swap(generic_reference& with) throw();

        /// Create a reference to pointee & record the delete function
        generic_reference(void* pointee, delete_ftn do_delete);
        
        /// Copy rhs
        generic_reference(const generic_reference& rhs);
        
		/// Destructor
        virtual ~generic_reference() throw();
		
        /// Take a share in what rhs references (delete old if appropriate)
        generic_reference& operator=(const generic_reference& rhs);
        
        /// Increment the strong reference count
        void increment_strong_references() const;

        /// Decrement the strong reference count
        void decrement_strong_references() const;
        
        /// Compare with pointer to referenced object
		bool less(void*) const throw();

        /// Get with pointer to referenced object
        void* get() const;
        
        /// Get with pointer to object referenced by rhs
        static void* get(const generic_reference& rhs) { return rhs.get(); }
	
    private:
        class counted_ptr_body;
        counted_ptr_body* body;
    };


	/**
    *   Implementation class supporting counted_ptr/uncounted_ptr templates.
    *
    *   Typesafe version of generic_reference (i.e. it does the casts). 
    *
    *   @author Alan Griffiths
    **/
	template<typename pointee_type>
    class typed_reference : public generic_reference
    {
	public:

        typed_reference(pointee_type* pointee) 
        : generic_reference(
            pointee, 
            &typed_reference::do_delete)
        { 
        }

// The following constructor and assignment lead to the pointer conversion
// chain "derived" -> "void*" -> "base*" which isn't guaranteed to work.
// (In practice it works in many circumstances.)
//
// Pending inspiration on a safe way to provide this facility together
// with the counted/uncounted support I've disabled them. {arg 20-Oct-99}
#ifdef ARG_UNSAFE_XVERT
		// Templated copy construction - for upcasting/const conversion
		template<typename rhs_pointee_type>
        typed_reference(
			const typed_reference<rhs_pointee_type>& rhs)
		: generic_reference(rhs)
		{
			// compile time type compatability check - does nothing at runtime
			{ rhs_pointee_type* pr= 0; pointee_type* pl= 0; pl = pr; }
		}
		
		// Templated assignment - for upcasting/const conversion
		template<typename rhs_pointee_type>
        typed_reference& operator=(
			const typed_reference<rhs_pointee_type>& rhs)
		{
			// compile time type compatability check - does nothing at runtime
			{ rhs_pointee_type* pr= 0; pointee_type* pl= 0; pl = pr; }
			generic_reference::operator=(rhs);
			return *this;
		}
#endif		

    	void swap(typed_reference& with) throw()
    	{
    		generic_reference::swap(with);
    	}


		///	equality
		template<typename rhs_pointee_type>
		bool operator==(
			const typed_reference<rhs_pointee_type>& rhs) const
		{
		    return get() == get(rhs);
		}

		///	inequality
		template<typename rhs_pointee_type>
		bool operator!=(
			const typed_reference<rhs_pointee_type>& rhs) const
		{
		    return get() != rhs.get();
		}

		///	Comparison
		template<typename rhs_pointee_type>
		bool operator<(
			const typed_reference<rhs_pointee_type>& rhs) const
		{
		    return less(rhs.get());
		}

	protected:

        void increment_strong_references() const
        {
        	generic_reference::increment_strong_references();
        }
		
	private:

        // Provide a type-safe deletion function
        static void do_delete(void* pointee) throw()
        {
            delete static_cast<pointee_type*>(pointee);
        }
    };
    

	/**
	*   "Smart pointer" template which owns the referenced object and shares
    *   ownership on copy/assignment - the last owner is responsible for
    *   destroying the referenced object.
	*
	*	"counted_ptr" gives non-intrusive strong reference counting and 
    *   is copy compatable with "uncounted_ptr" (which gives weak reference 
    *   counting).
	*
    *   <code>counted_ptr</code> and <code>uncounted_ptr</code> are described
    *   in an article:
    *   <a href="http://www.octopull.demon.co.uk/arglib/uncounted_ptr.html">
    *   Uncounted pointers in C++</a>.
	*
	*	All methods support the strong exception safety guarantee with the
    *   proviso that the referenced type must have a non-throwing destructor.
	*
	*                                                   @author Alan Griffiths
	**/
	template<typename pointee_type>
	class counted_ptr : 
		public  typed_reference<pointee_type>
	{
        typedef typed_reference<pointee_type> base_type;
        
	public:
	
	//  Construction

		/// "0 initialised" pointer
		counted_ptr() 
        : base_type(static_cast<pointee_type*>(0)) 
        {
            increment_strong_references();
        }
		
		/// Takes ownership of p
		explicit counted_ptr(pointee_type* p) 
        : base_type(p) 
        {
            increment_strong_references();
        }
		
		/// Take value of rhs
		counted_ptr(const counted_ptr& rhs)
        : base_type(rhs)
		{
            increment_strong_references();
		}

#ifdef ARG_UNSAFE_XVERT
		/// Share ownership of rhs
		template<typename rhs_pointee_type>
        counted_ptr(
			const typed_reference<rhs_pointee_type>& rhs)
        : base_type(rhs)
		{
            increment_strong_references();
		}
#else
		/// Acquire share of ownership from rhs
        counted_ptr(const base_type& rhs)
        : base_type(rhs)
		{
            increment_strong_references();
		}
#endif

		~counted_ptr() throw()
		{
            decrement_strong_references();
		}

	
	//  Accessors

		/// Returns contents
		pointee_type* get() const 
		{
		    return static_cast<pointee_type*>(base_type::get());
		}
	
		/// Indirection operator
		pointee_type* operator->() const
		{
		    return static_cast<pointee_type*>(base_type::get());
		}
	
		///	Dereference operator
		pointee_type& operator*() const
		{
		    return *static_cast<pointee_type*>(base_type::get());
		}

	
	//  Mutators

#ifdef ARG_UNSAFE_XVERT
		template<typename rhs_pointee_type>
        counted_ptr& operator=(
			const typed_reference<rhs_pointee_type>& rhs)
		{
            rhs.increment_strong_references();
            decrement_strong_references();
            base_type::operator=(rhs);
			return *this;
		}
#endif

		/**
        *   Acquire share of ownership from rhs 
        *   (and destroy old referent if last owner)
        **/
        counted_ptr& operator=(const base_type& rhs)
		{
            rhs.increment_strong_references();
            decrement_strong_references();
            base_type::operator=(rhs);
			return *this;
		}

		/**
        *   Acquire share of ownership from rhs 
        *   (and destroy old referent if last owner)
        **/
        counted_ptr& operator=(const counted_ptr& rhs)
		{
            rhs.increment_strong_references();
            decrement_strong_references();
            base_type::operator=(rhs);
			return *this;
		}

		/// Delete existing contents (and take ownership of p)
		void reset(pointee_type* p)
		{
		    counted_ptr tmp(p);
		    base_type::swap(tmp);
		}
		
		/// Swaps contents with "with"
		void swap(base_type& with) throw()
		{
		    base_type::swap(with);
		}

	};


	/**
	*   "Smart pointer" template which does not own the referenced object (a
    *   counted_ptr is needed to own the object).  When there are no owners of
    *   the referenced object the pointer becomes zero.
    *
    *   The copy semantics are to copy the reference - "uncounted_ptr" gives 
    *   weak reference counting (the references do not participate in the
    *   ownership) and is copy compatable with "counted_ptr".
	*
    *   <code>counted_ptr</code> and <code>uncounted_ptr</code> are described
    *   in an article:
    *   <a href="http://www.octopull.demon.co.uk/arglib/uncounted_ptr.html">
    *   Uncounted pointers in C++</a>.
	*
	*	All methods support the strong exception safety guarantee with the
    *   proviso that the referenced type must have a non-throwing destructor.
	*
	*                                           @author Alan Griffiths
	**/
	template<typename pointee_type>
	class uncounted_ptr : public typed_reference<pointee_type>
	{
        typedef typed_reference<pointee_type> base_type;
        
	public:
	
	//  Construction

		/// "0 initialised" pointer
		uncounted_ptr() 
        : base_type(static_cast<pointee_type*>(0)) 
        {
        }

		
#ifdef ARG_UNSAFE_XVERT
		/**
        *   Acquire share of ownership from rhs 
        **/
		template<typename rhs_pointee_type>
        uncounted_ptr(
			const typed_reference<rhs_pointee_type>& rhs)
        : base_type(rhs)
		{
		}
#else
		/**
        *   Acquire share of ownership from rhs 
        **/
        uncounted_ptr(const base_type& rhs)
        : base_type(rhs)
		{
		}
#endif

		~uncounted_ptr() throw()
		{
		}

	
	//  Accessors

		/// Returns contents
		pointee_type* get() const 
		{
		    return static_cast<pointee_type*>(base_type::get());
		}
	
		/// Indirection operator
		pointee_type* operator->() const
		{
		    return static_cast<pointee_type*>(base_type::get());
		}
	
		///	Dereference operator
		pointee_type& operator*() const
		{
		    return static_cast<pointee_type&>(*base_type::get());
		}

	
	//  Mutators

#ifdef ARG_UNSAFE_XVERT
		///
		template<typename rhs_pointee_type>
        uncounted_ptr& operator=(
			const typed_reference<rhs_pointee_type>& rhs)
		{
            base_type::operator=(rhs);
			return *this;
		}
#else
		/**
        *   Acquire share of ownership from rhs 
        *   (and destroy old referent if last owner)
        **/
        uncounted_ptr& operator=(const base_type& rhs)
		{
            base_type::operator=(rhs);
			return *this;
		}
#endif

		/// Delete existing contents (and take ownership of p)
		void reset(pointee_type* p)
		{
		    base_type tmp(p);
		    base_type::swap(tmp);
		}
		
		/// Swaps contents with "with"
		void swap(base_type& with) throw()
		{
		    base_type::swap(with);
		}

	};
}

namespace std
{
	/// Overload swap algorithm to provide an efficient, non-throwing version
	template<typename pointee_type>
	inline
	void swap(
		::arg::typed_reference<pointee_type>& lhs, 
		::arg::typed_reference<pointee_type>& rhs) throw()
	{
	    lhs.swap(rhs);
	}

}
#endif
