////////////////////////////////////////////////////////////////////////
//
// File: XML.cpp
//
// This is basically a branch of the TinyXML project
// As found on: https://sourceforge.net/projects/tinyxml/
// Only with some light adaptions not found in that project
//
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#pragma once
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4100 )
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Help out windows:
#if defined( _DEBUG ) && !defined( DEBUG )
#define DEBUG
#endif

#define XML_USE_STL	
#include <string>
#include <iostream>
#include <sstream>
#define XML_STRING	std::string

// Microsoft visual studio, version 2005 and higher.
#define XML_SNPRINTF _snprintf_s
#define XML_SNSCANF  _snscanf_s

class XmlDocument;
class XmlElement;
class XmlComment;
class XmlUnknown;
class XmlAttribute;
class XmlText;
class XmlDeclaration;
class XmlParsingData;

// See W3C-XML standard
CString XMLString(CString p_string);

/*	Internal structure for tracking location of items 
  in the XML file.
*/
struct XmlCursor
{
  XmlCursor()		{ Clear(); }
  void Clear()	{ row = col = -1; }

  int row;	// 0 based.
  int col;	// 0 based.
};


/**
  If you call the Accept() method, it requires being passed a XmlVisitor
  class to handle callbacks. For nodes that contain other nodes (Document, Element)
  you will get called with a VisitEnter/VisitExit pair. Nodes that are always leaves
  are simple called with Visit().

  If you return 'true' from a Visit method, recursive parsing will continue. If you return
  false, <b>no children of this node or its sibilings</b> will be Visited.

  All flavors of Visit methods have a default implementation that returns 'true' (continue 
  visiting). You need to only override methods that are interesting to you.

  Generally Accept() is called on the XmlDocument, although all nodes suppert Visiting.

  You should never change the document from a callback.

  @sa XmlNode::Accept()
*/
class XmlVisitor
{
public:
  virtual ~XmlVisitor() {}

  /// Visit a document.
  virtual bool VisitEnter( const XmlDocument& doc )	{ return true; }
  /// Visit a document.
  virtual bool VisitExit( const XmlDocument& doc )	{ return true; }

  /// Visit an element.
  virtual bool VisitEnter( const XmlElement& element, const XmlAttribute* firstAttribute )	{ return true; }
  /// Visit an element.
  virtual bool VisitExit( const XmlElement& element )											{ return true; }

  /// Visit a declaration
  virtual bool Visit( const XmlDeclaration& declaration )		{ return true; }
  /// Visit a text node
  virtual bool Visit( const XmlText& text )						{ return true; }
  /// Visit a comment node
  virtual bool Visit( const XmlComment& comment )				{ return true; }
  /// Visit an unknow node
  virtual bool Visit( const XmlUnknown& unknown )				{ return true; }
};

// Only used by Attribute::Query functions
enum 
{ 
  XML_SUCCESS,
  XML_NO_ATTRIBUTE,
  XML_WRONG_TYPE
};


// Used by the parsing routines.
enum XmlEncoding
{
  XML_ENCODING_UNKNOWN,
  XML_ENCODING_UTF8,
  XML_ENCODING_LEGACY
};

const XmlEncoding XML_DEFAULT_ENCODING = XML_ENCODING_UNKNOWN;

/** XmlBase is a base class for every class in TinyXml.
  It does little except to establish that TinyXml classes
  can be printed and provide some utility functions.

  In XML, the document and elements can contain
  other elements and other types of nodes.

  @verbatim
  A Document can contain:	Element	(container or leaf)
              Comment (leaf)
              Unknown (leaf)
              Declaration( leaf )

  An Element can contain:	Element (container or leaf)
              Text	(leaf)
              Attributes (not on tree)
              Comment (leaf)
              Unknown (leaf)

  A Decleration contains: Attributes (not on tree)
  @endverbatim
*/
class XmlBase
{
  friend class XmlNode;
  friend class XmlElement;
  friend class XmlDocument;

public:
  XmlBase()	:	userData(0)		{}
  virtual ~XmlBase()			{}

  /**	All TinyXml classes can print themselves to a filestream
    or the string class (XmlString in non-STL mode, std::string
    in STL mode.) Either or both cfile and str can be null.
    
    This is a formatted print, and will insert 
    tabs and newlines.
    
    (For an unformatted stream, use the << operator.)
  */
  virtual void Print( FILE* cfile, int depth ) const = 0;
  virtual XML_STRING Print( int depth ) const = 0;

  /**	The world does not agree on whether white space should be kept or
    not. In order to make everyone happy, these global, static functions
    are provided to set whether or not TinyXml will condense all white space
    into a single space or not. The default is to condense. Note changing this
    value is not thread safe.
  */
  static void SetCondenseWhiteSpace( bool condense )		{ condenseWhiteSpace = condense; }

  /// Return the current white space setting.
  static bool IsWhiteSpaceCondensed()						{ return condenseWhiteSpace; }

  /** Return the position, in the original source file, of this node or attribute.
    The row and column are 1-based. (That is the first row and first column is
    1,1). If the returns values are 0 or less, then the parser does not have
    a row and column value.

    Generally, the row and column value will be set when the XmlDocument::Load(),
    XmlDocument::LoadFile(), or any XmlNode::Parse() is called. It will NOT be set
    when the DOM was created from operator>>.

    The values reflect the initial load. Once the DOM is modified programmatically
    (by adding or changing nodes and attributes) the new values will NOT update to
    reflect changes in the document.

    There is a minor performance cost to computing the row and column. Computation
    can be disabled if XmlDocument::SetTabSize() is called with 0 as the value.

    @sa XmlDocument::SetTabSize()
  */
  int Row() const			{ return location.row + 1; }
  int Column() const		{ return location.col + 1; }	///< See Row()

  void  SetUserData( void* user )			{ userData = user; }	///< Set a pointer to arbitrary user data.
  void* GetUserData()						{ return userData; }	///< Get a pointer to arbitrary user data.
  const void* GetUserData() const 		{ return userData; }	///< Get a pointer to arbitrary user data.

  // Table that returs, for a given lead byte, the total number of bytes
  // in the UTF-8 sequence.
  static const int utf8ByteTable[256];

  virtual const char* Parse(	const char* p, 
                XmlParsingData* data, 
                XmlEncoding encoding /*= XML_ENCODING_UNKNOWN */ ) = 0;

  enum
  {
    XML_NO_ERROR = 0,
    XML_ERROR,
    XML_ERROR_OPENING_FILE,
    XML_ERROR_OUT_OF_MEMORY,
    XML_ERROR_PARSING_ELEMENT,
    XML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
    XML_ERROR_READING_ELEMENT_VALUE,
    XML_ERROR_READING_ATTRIBUTES,
    XML_ERROR_PARSING_EMPTY,
    XML_ERROR_READING_END_TAG,
    XML_ERROR_PARSING_UNKNOWN,
    XML_ERROR_PARSING_COMMENT,
    XML_ERROR_PARSING_DECLARATION,
    XML_ERROR_DOCUMENT_EMPTY,
    XML_ERROR_EMBEDDED_NULL,
    XML_ERROR_PARSING_CDATA,
    XML_ERROR_DOCUMENT_TOP_ONLY,

    XML_ERROR_STRING_COUNT
  };

protected:

  static const char* SkipWhiteSpace( const char*, XmlEncoding encoding );
  inline static bool IsWhiteSpace( char c )		
  { 
    return ( isspace( (unsigned char) c ) || c == '\n' || c == '\r' ); 
  }
  inline static bool IsWhiteSpace( int c )
  {
    if ( c < 256 )
      return IsWhiteSpace( (char) c );
    return false;	// Again, only truly correct for English/Latin...but usually works.
  }

  #ifdef XML_USE_STL
  static bool	StreamWhiteSpace( std::istream * in, XML_STRING * tag );
  static bool StreamTo( std::istream * in, int character, XML_STRING * tag );
  #endif

  /*	Reads an XML name into the string provided. Returns
    a pointer just past the last character of the name,
    or 0 if the function has an error.
  */
  static const char* ReadName( const char* p, XML_STRING* name, XmlEncoding encoding );

  /*	Reads text. Returns a pointer past the given end tag.
    Wickedly complex options, but it keeps the (sensitive) code in one place.
  */
  static const char* ReadText(	const char* in,				// where to start
                  XML_STRING* text,			// the string read
                  bool ignoreWhiteSpace,		// whether to keep the white space
                  const char* endTag,			// what ends this text
                  bool ignoreCase,			// whether to ignore case in the end tag
                  XmlEncoding encoding );	// the current encoding

  // If an entity has been found, transform it into a character.
  static const char* GetEntity( const char* in, char* value, int* length, XmlEncoding encoding );

  // Get a character, while interpreting entities.
  // The length can be from 0 to 4 bytes.
  inline static const char* GetChar( const char* p, char* _value, int* length, XmlEncoding encoding )
  {
    assert( p );
    if ( encoding == XML_ENCODING_UTF8 )
    {
      *length = utf8ByteTable[ *((const unsigned char*)p) ];
      assert( *length >= 0 && *length < 5 );
    }
    else
    {
      *length = 1;
    }

    if ( *length == 1 )
    {
      if ( *p == '&' )
        return GetEntity( p, _value, length, encoding );
      *_value = *p;
      return p+1;
    }
    else if ( *length )
    {
      //strncpy( _value, p, *length );	// lots of compilers don't like this function (unsafe),
                        // and the null terminator isn't needed
      for( int i=0; p[i] && i<*length; ++i ) {
        _value[i] = p[i];
      }
      return p + (*length);
    }
    else
    {
      // Not valid text.
      return 0;
    }
  }

  // Puts a string to a stream, expanding entities as it goes.
  // Note this should not contian the '<', '>', etc, or they will be transformed into entities!
  static void PutString( const XML_STRING& str, XML_STRING* out );

  // Return true if the next characters in the stream are any of the endTag sequences.
  // Ignore case only works for english, and should only be relied on when comparing
  // to English words: StringEqual( p, "version", true ) is fine.
  static bool StringEqual(	const char* p,
                const char* endTag,
                bool ignoreCase,
                XmlEncoding encoding );

  static const char* errorString[ XML_ERROR_STRING_COUNT ];

  XmlCursor location;

    /// Field containing a generic user pointer
  void*			userData;
  
  // None of these methods are reliable for any language except English.
  // Good for approximation, not great for accuracy.
  static int IsAlpha( unsigned char anyByte, XmlEncoding encoding );
  static int IsAlphaNum( unsigned char anyByte, XmlEncoding encoding );
  inline static int ToLower( int v, XmlEncoding encoding )
  {
    if ( encoding == XML_ENCODING_UTF8 )
    {
      if ( v < 128 ) return tolower( v );
      return v;
    }
    else
    {
      return tolower( v );
    }
  }
  static void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length );

private:
  XmlBase( const XmlBase& );				// not implemented.
  void operator=( const XmlBase& base );	// not allowed.

  struct Entity
  {
    const char*     str;
    unsigned int	strLength;
    char		    chr;
  };
  enum
  {
    NUM_ENTITY = 5,
    MAX_ENTITY_LENGTH = 6

  };
  static Entity entity[ NUM_ENTITY ];
  static bool condenseWhiteSpace;
};


/** The parent class for everything in the Document Object Model.
  (Except for attributes).
  Nodes have siblings, a parent, and children. A node can be
  in a document, or stand on its own. The type of a XmlNode
  can be queried, and it can be cast to its more defined type.
*/
class XmlNode : public XmlBase
{
  friend class XmlDocument;
  friend class XmlElement;

public:
  #ifdef XML_USE_STL	

      /** An input stream operator, for every class. Tolerant of newlines and
        formatting, but doesn't expect them.
      */
      friend std::istream& operator >> (std::istream& in, XmlNode& base);

      /** An output stream operator, for every class. Note that this outputs
        without any newlines or formatting, as opposed to Print(), which
        includes tabs and new lines.

        The operator<< and operator>> are not completely symmetric. Writing
        a node to a stream is very well defined. You'll get a nice stream
        of output, without any extra whitespace or newlines.
        
        But reading is not as well defined. (As it always is.) If you create
        a XmlElement (for example) and read that from an input stream,
        the text needs to define an element or junk will result. This is
        true of all input streams, but it's worth keeping in mind.

        A XmlDocument will read nodes until it reads a root element, and
      all the children of that root element.
      */	
      friend std::ostream& operator<< (std::ostream& out, const XmlNode& base);

    /// Appends the XML node or attribute to a std::string.
    friend std::string& operator<< (std::string& out, const XmlNode& base );

  #endif

  /** The types of XML nodes supported by TinyXml. (All the
      unsupported types are picked up by UNKNOWN.)
  */
  enum NodeType
  {
    DOCUMENT,
    ELEMENT,
    COMMENT,
    UNKNOWN,
    TEXT,
    DECLARATION,
    TYPECOUNT
  };

  virtual ~XmlNode();

  /** The meaning of 'value' changes for the specific type of
    XmlNode.
    @verbatim
    Document:	filename of the xml file
    Element:	name of the element
    Comment:	the comment text
    Unknown:	the tag contents
    Text:		the text string
    @endverbatim

    The subclasses will wrap this function.
  */
  const char *Value() const { return value.c_str (); }

    #ifdef XML_USE_STL
  /** Return Value() as a std::string. If you only use STL,
      this is more efficient than calling Value().
    Only available in STL mode.
  */
  const std::string& ValueStr() const { return value; }
  #endif

  /** Changes the value of the node. Defined as:
    @verbatim
    Document:	filename of the xml file
    Element:	name of the element
    Comment:	the comment text
    Unknown:	the tag contents
    Text:		the text string
    @endverbatim
  */
  void SetValue(const char * _value) { value = _value;}

    #ifdef XML_USE_STL
  /// STL std::string form.
  void SetValue( const std::string& _value )	{ value = _value; }
  #endif

  /// Delete all the children of this node. Does not affect 'this'.
  void Clear();

  /// One step up the DOM.
        XmlNode* Parent()							{ return parent; }
  const XmlNode* Parent() const				{ return parent; }

  const XmlNode* FirstChild()	const	{ return firstChild; }		///< The first child of this node. Will be null if there are no children.
        XmlNode* FirstChild()					{ return firstChild; }
  const XmlNode* FirstChild( const char * value ) const;			///< The first child of this node with the matching 'value'. Will be null if none found.
  /// The first child of this node with the matching 'value'. Will be null if none found.
        XmlNode* FirstChild( const char * _value ) {
    // Call through to the const version - safe since nothing is changed. Exiting syntax: cast this to a const (always safe)
    // call the method, cast the return back to non-const.
    return const_cast< XmlNode* > ((const_cast< const XmlNode* >(this))->FirstChild( _value ));
  }
  const XmlNode* LastChild() const	{ return lastChild; }		/// The last child of this node. Will be null if there are no children.
        XmlNode* LastChild()	{ return lastChild; }
  
  const XmlNode* LastChild( const char * value ) const;			/// The last child of this node matching 'value'. Will be null if there are no children.
        XmlNode* LastChild( const char * _value ) 
                 {
                   return const_cast< XmlNode* > ((const_cast< const XmlNode* >(this))->LastChild( _value ));
                 }

    #ifdef XML_USE_STL
  const XmlNode* FirstChild( const std::string& _value ) const	{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
        XmlNode* FirstChild( const std::string& _value )				{	return FirstChild (_value.c_str ());	}	///< STL std::string form.
  const XmlNode* LastChild ( const std::string& _value ) const	{	return LastChild (_value.c_str ());	}	///< STL std::string form.
        XmlNode* LastChild ( const std::string& _value )				{	return LastChild (_value.c_str ());	}	///< STL std::string form.
  #endif

  /** An alternate way to walk the children of a node.
    One way to iterate over nodes is:
    @verbatim
      for( child = parent->FirstChild(); child; child = child->NextSibling() )
    @endverbatim

    IterateChildren does the same thing with the syntax:
    @verbatim
      child = 0;
      while( child = parent->IterateChildren( child ) )
    @endverbatim

    IterateChildren takes the previous child as input and finds
    the next one. If the previous child is null, it returns the
    first. IterateChildren will return null when done.
  */
  const XmlNode* IterateChildren( const XmlNode* previous ) const;
        XmlNode* IterateChildren( const XmlNode* previous ) {
    return const_cast< XmlNode* >( (const_cast< const XmlNode* >(this))->IterateChildren( previous ) );
  }

  /// This flavor of IterateChildren searches for children with a particular 'value'
  const XmlNode* IterateChildren( const char * value, const XmlNode* previous ) const;
        XmlNode* IterateChildren( const char * _value, const XmlNode* previous ) {
    return const_cast< XmlNode* >( (const_cast< const XmlNode* >(this))->IterateChildren( _value, previous ) );
  }

    #ifdef XML_USE_STL
  const XmlNode* IterateChildren( const std::string& _value, const XmlNode* previous ) const	{	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
        XmlNode* IterateChildren( const std::string& _value, const XmlNode* previous )        {	return IterateChildren (_value.c_str (), previous);	}	///< STL std::string form.
  #endif

  /** Add a new node related to this. Adds a child past the LastChild.
    Returns a pointer to the new object or NULL if an error occured.
  */
  XmlNode* InsertEndChild( const XmlNode& addThis );


  /** Add a new node related to this. Adds a child past the LastChild.

    NOTE: the node to be added is passed by pointer, and will be
    henceforth owned (and deleted) by tinyXml. This method is efficient
    and avoids an extra copy, but should be used with care as it
    uses a different memory model than the other insert functions.

    @sa InsertEndChild
  */
  XmlNode* LinkEndChild( XmlNode* addThis );

  /** Add a new node related to this. Adds a child before the specified child.
    Returns a pointer to the new object or NULL if an error occured.
  */
  XmlNode* InsertBeforeChild( XmlNode* beforeThis, const XmlNode& addThis );

  /** Add a new node related to this. Adds a child after the specified child.
    Returns a pointer to the new object or NULL if an error occured.
  */
  XmlNode* InsertAfterChild(  XmlNode* afterThis, const XmlNode& addThis );

  /** Replace a child of this node.
    Returns a pointer to the new object or NULL if an error occured.
  */
  XmlNode* ReplaceChild( XmlNode* replaceThis, const XmlNode& withThis );

  /// Delete a child of this node.
  bool RemoveChild( XmlNode* removeThis );

  /// Navigate to a sibling node.
  const XmlNode* PreviousSibling() const			{ return prev; }
  XmlNode* PreviousSibling()						{ return prev; }

  /// Navigate to a sibling node.
  const XmlNode* PreviousSibling( const char * ) const;
  XmlNode* PreviousSibling( const char *_prev ) {
    return const_cast< XmlNode* >( (const_cast< const XmlNode* >(this))->PreviousSibling( _prev ) );
  }

    #ifdef XML_USE_STL
  const XmlNode* PreviousSibling( const std::string& _value ) const	{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
  XmlNode* PreviousSibling( const std::string& _value ) 			{	return PreviousSibling (_value.c_str ());	}	///< STL std::string form.
  const XmlNode* NextSibling( const std::string& _value) const		{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
  XmlNode* NextSibling( const std::string& _value) 					{	return NextSibling (_value.c_str ());	}	///< STL std::string form.
  #endif

  /// Navigate to a sibling node.
  const XmlNode* NextSibling() const				{ return next; }
  XmlNode* NextSibling()							{ return next; }

  /// Navigate to a sibling node with the given 'value'.
  const XmlNode* NextSibling( const char * ) const;
  XmlNode* NextSibling( const char* _next ) {
    return const_cast< XmlNode* >( (const_cast< const XmlNode* >(this))->NextSibling( _next ) );
  }

  /** Convenience function to get through elements.
    Calls NextSibling and ToElement. Will skip all non-Element
    nodes. Returns 0 if there is not another element.
  */
  const XmlElement* NextSiblingElement() const;
  XmlElement* NextSiblingElement() {
    return const_cast< XmlElement* >( (const_cast< const XmlNode* >(this))->NextSiblingElement() );
  }

  /** Convenience function to get through elements.
    Calls NextSibling and ToElement. Will skip all non-Element
    nodes. Returns 0 if there is not another element.
  */
  const XmlElement* NextSiblingElement( const char * ) const;
  XmlElement* NextSiblingElement( const char *_next ) {
    return const_cast< XmlElement* >( (const_cast< const XmlNode* >(this))->NextSiblingElement( _next ) );
  }

    #ifdef XML_USE_STL
  const XmlElement* NextSiblingElement( const std::string& _value) const	{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
  XmlElement* NextSiblingElement( const std::string& _value)				{	return NextSiblingElement (_value.c_str ());	}	///< STL std::string form.
  #endif

  /// Convenience function to get through elements.
  const XmlElement* FirstChildElement()	const;
  XmlElement* FirstChildElement() {
    return const_cast< XmlElement* >( (const_cast< const XmlNode* >(this))->FirstChildElement() );
  }

  /// Convenience function to get through elements.
  const XmlElement* FirstChildElement( const char * _value ) const;
  XmlElement* FirstChildElement( const char * _value ) {
    return const_cast< XmlElement* >( (const_cast< const XmlNode* >(this))->FirstChildElement( _value ) );
  }

    #ifdef XML_USE_STL
  const XmlElement* FirstChildElement( const std::string& _value ) const	{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
  XmlElement* FirstChildElement( const std::string& _value )				{	return FirstChildElement (_value.c_str ());	}	///< STL std::string form.
  #endif

  /** Query the type (as an enumerated value, above) of this node.
    The possible types are: DOCUMENT, ELEMENT, COMMENT,
                UNKNOWN, TEXT, and DECLARATION.
  */
  int Type() const	{ return type; }

  /** Return a pointer to the Document this node lives in.
    Returns null if not in a document.
  */
  const XmlDocument* GetDocument() const;
  XmlDocument* GetDocument() {
    return const_cast< XmlDocument* >( (const_cast< const XmlNode* >(this))->GetDocument() );
  }

  /// Returns true if this node has no children.
  bool NoChildren() const						{ return !firstChild; }

  virtual const XmlDocument*    ToDocument()    const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const XmlElement*     ToElement()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const XmlComment*     ToComment()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const XmlUnknown*     ToUnknown()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const XmlText*        ToText()        const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual const XmlDeclaration* ToDeclaration() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

  virtual XmlDocument*          ToDocument()    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual XmlElement*           ToElement()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual XmlComment*           ToComment()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual XmlUnknown*           ToUnknown()	    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual XmlText*	            ToText()        { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
  virtual XmlDeclaration*       ToDeclaration() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

  /** Create an exact duplicate of this node and return it. The memory must be deleted
    by the caller. 
  */
  virtual XmlNode* Clone() const = 0;

  /** Accept a hierchical visit the nodes in the TinyXML DOM. Every node in the 
    XML tree will be conditionally visited and the host will be called back
    via the XmlVisitor interface.

    This is essentially a SAX interface for TinyXML. (Note however it doesn't re-parse
    the XML for the callbacks, so the performance of TinyXML is unchanged by using this
    interface versus any other.)

    The interface has been based on ideas from:

    - http://www.saxproject.org/
    - http://c2.com/cgi/wiki?HierarchicalVisitorPattern 

    Which are both good references for "visiting".

    An example of using Accept():
    @verbatim
    XmlPrinter printer;
    tinyxmlDoc.Accept( &printer );
    const char* xmlcstr = printer.CStr();
    @endverbatim
  */
  virtual bool Accept( XmlVisitor* visitor ) const = 0;

protected:
  XmlNode( NodeType _type );

  // Copy to the allocated object. Shared functionality between Clone, Copy constructor,
  // and the assignment operator.
  void CopyTo( XmlNode* target ) const;

  #ifdef XML_USE_STL
      // The real work of the input operator.
  virtual void StreamIn( std::istream* in, XML_STRING* tag ) = 0;
  #endif

  // Figure out what is at *p, and parse it. Returns null if it is not an xml node.
  XmlNode* Identify( const char* start, XmlEncoding encoding );

  XmlNode*		parent;
  NodeType		type;

  XmlNode*		firstChild;
  XmlNode*		lastChild;

  XML_STRING	value;

  XmlNode*		prev;
  XmlNode*		next;

private:
  XmlNode( const XmlNode& );				// not implemented.
  void operator=( const XmlNode& base );	// not allowed.
};


/** An attribute is a name-value pair. Elements have an arbitrary
  number of attributes, each with a unique name.

  @note The attributes are not XmlNodes, since they are not
      part of the tinyXML document object model. There are other
      suggested ways to look at this problem.
*/
class XmlAttribute : public XmlBase
{
  friend class XmlAttributeSet;

public:
  /// Construct an empty attribute.
  XmlAttribute() : XmlBase()
  {
    document = 0;
    prev = next = 0;
  }

  #ifdef XML_USE_STL
  /// std::string constructor.
  XmlAttribute( const std::string& _name, const std::string& _value )
  {
    name = _name;
    value = _value;
    document = 0;
    prev = next = 0;
  }
  #endif

  /// Construct an attribute with a name and value.
  XmlAttribute( const char * _name, const char * _value )
  {
    name = _name;
    value = _value;
    document = 0;
    prev = next = 0;
  }

  const char*		Name()  const		{ return name.c_str(); }		///< Return the name of this attribute.
  const char*		Value() const		{ return value.c_str(); }		///< Return the value of this attribute.
  #ifdef XML_USE_STL
  const std::string& ValueStr() const	{ return value; }				///< Return the value of this attribute.
  #endif
  int				IntValue() const;									///< Return the value of this attribute, converted to an integer.
  double			DoubleValue() const;								///< Return the value of this attribute, converted to a double.

  // Get the tinyxml string representation
  const XML_STRING& NameTStr() const { return name; }

  /** QueryIntValue examines the value string. It is an alternative to the
    IntValue() method with richer error checking.
    If the value is an integer, it is stored in 'value' and 
    the call returns XML_SUCCESS. If it is not
    an integer, it returns XML_WRONG_TYPE.

    A specialized but useful call. Note that for success it returns 0,
    which is the opposite of almost all other TinyXml calls.
  */
  int QueryIntValue( int* _value ) const;
  /// QueryDoubleValue examines the value string. See QueryIntValue().
  int QueryDoubleValue( double* _value ) const;

  void SetName( const char* _name )	{ name = _name; }				///< Set the name of this attribute.
  void SetValue( const char* _value )	{ value = _value; }				///< Set the value.

  void SetIntValue( int _value );										///< Set the value from an integer.
  void SetDoubleValue( double _value );								///< Set the value from a double.

    #ifdef XML_USE_STL
  /// STL std::string form.
  void SetName( const std::string& _name )	{ name = _name; }	
  /// STL std::string form.	
  void SetValue( const std::string& _value )	{ value = _value; }
  #endif

  /// Get the next sibling attribute in the DOM. Returns null at end.
  const XmlAttribute* Next() const;
  XmlAttribute* Next() {
    return const_cast< XmlAttribute* >( (const_cast< const XmlAttribute* >(this))->Next() ); 
  }

  /// Get the previous sibling attribute in the DOM. Returns null at beginning.
  const XmlAttribute* Previous() const;
  XmlAttribute* Previous() {
    return const_cast< XmlAttribute* >( (const_cast< const XmlAttribute* >(this))->Previous() ); 
  }

  bool operator==( const XmlAttribute& rhs ) const { return rhs.name == name; }
  bool operator<( const XmlAttribute& rhs )	 const { return name < rhs.name; }
  bool operator>( const XmlAttribute& rhs )  const { return name > rhs.name; }

  /*	Attribute parsing starts: first letter of the name
             returns: the next char after the value end quote
  */
  virtual const char* Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

  // Prints this Attribute to a FILE stream.
  virtual void Print( FILE* cfile, int depth ) const {
    Print( cfile, depth, 0 );
  }
  void Print( FILE* cfile,int depth,XML_STRING* str) const;
  // Print the element to a XML_STRING
  // virtual XML_STRING Print(int depth,XML_STRING* str = NULL) const;
  // Print the element to a XML_STRING
  virtual XML_STRING Print(int depth) const;

  // [internal use]
  // Set the document pointer so the attribute can report errors.
  void SetDocument( XmlDocument* doc )	{ document = doc; }

private:
  XmlAttribute( const XmlAttribute& );				// not implemented.
  void operator=( const XmlAttribute& base );	// not allowed.

  XmlDocument*	document;	// A pointer back to a document, for error reporting.
  XML_STRING name;
  XML_STRING value;
  XmlAttribute*	prev;
  XmlAttribute*	next;
};


/*	A class used to manage a group of attributes.
  It is only used internally, both by the ELEMENT and the DECLARATION.
  
  The set can be changed transparent to the Element and Declaration
  classes that use it, but NOT transparent to the Attribute
  which has to implement a next() and previous() method. Which makes
  it a bit problematic and prevents the use of STL.

  This version is implemented with circular lists because:
    - I like circular lists
    - it demonstrates some independence from the (typical) doubly linked list.
*/
class XmlAttributeSet
{
public:
  XmlAttributeSet();
  ~XmlAttributeSet();

  void Add( XmlAttribute* attribute );
  void Remove( XmlAttribute* attribute );

  const XmlAttribute* First()	const	  { return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
        XmlAttribute* First()					{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
  const XmlAttribute* Last() const		{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }
        XmlAttribute* Last()					{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }

  const XmlAttribute*	Find( const char* _name ) const;
        XmlAttribute*	Find( const char* _name ) 
        {
          return const_cast< XmlAttribute* >( (const_cast< const XmlAttributeSet* >(this))->Find( _name ) );
        }
  #ifdef XML_USE_STL
  const XmlAttribute*	Find( const std::string& _name ) const;
        XmlAttribute*	Find( const std::string& _name ) 
        {
          return const_cast< XmlAttribute* >( (const_cast< const XmlAttributeSet* >(this))->Find( _name ) );
        }
  #endif

private:
  //*ME:	Because of hidden/disabled copy-construktor in XmlAttribute (sentinel-element),
  //*ME:	this class must be also use a hidden/disabled copy-constructor !!!
  XmlAttributeSet( const XmlAttributeSet& );	// not allowed
  void operator=( const XmlAttributeSet& );	// not allowed (as XmlAttribute)

  XmlAttribute sentinel;
};


/** The element is a container class. It has a value, the element name,
  and can contain other elements, text, comments, and unknowns.
  Elements also contain an arbitrary number of attributes.
*/
class XmlElement : public XmlNode
{
public:
  /// Construct an element.
  XmlElement (const char * in_value);

  #ifdef XML_USE_STL
  /// std::string constructor.
  XmlElement( const std::string& _value );
  #endif

  XmlElement( const XmlElement& );

  void operator=( const XmlElement& base );

  virtual ~XmlElement();

  /** Given an attribute name, Attribute() returns the value
    for the attribute of that name, or null if none exists.
  */
  const char* Attribute( const char* name ) const;

  /** Given an attribute name, Attribute() returns the value
    for the attribute of that name, or null if none exists.
    If the attribute exists and can be converted to an integer,
    the integer value will be put in the return 'i', if 'i'
    is non-null.
  */
  const char* Attribute( const char* name, int* i ) const;

  /** Given an attribute name, Attribute() returns the value
    for the attribute of that name, or null if none exists.
    If the attribute exists and can be converted to an double,
    the double value will be put in the return 'd', if 'd'
    is non-null.
  */
  const char* Attribute( const char* name, double* d ) const;

  /** QueryIntAttribute examines the attribute - it is an alternative to the
    Attribute() method with richer error checking.
    If the attribute is an integer, it is stored in 'value' and 
    the call returns XML_SUCCESS. If it is not
    an integer, it returns XML_WRONG_TYPE. If the attribute
    does not exist, then XML_NO_ATTRIBUTE is returned.
  */	
  int QueryIntAttribute( const char* name, int* _value ) const;
  /// QueryDoubleAttribute examines the attribute - see QueryIntAttribute().
  int QueryDoubleAttribute( const char* name, double* _value ) const;
  /// QueryFloatAttribute examines the attribute - see QueryIntAttribute().
  int QueryFloatAttribute( const char* name, float* _value ) const {
    double d;
    int result = QueryDoubleAttribute( name, &d );
    if ( result == XML_SUCCESS ) {
      *_value = (float)d;
    }
    return result;
  }
    #ifdef XML_USE_STL
  /** Template form of the attribute query which will try to read the
    attribute into the specified type. Very easy, very powerful, but
    be careful to make sure to call this with the correct type.

    @return XML_SUCCESS, XML_WRONG_TYPE, or XML_NO_ATTRIBUTE
  */
  template< typename T > int QueryValueAttribute( const std::string& name, T* outValue ) const
  {
    const XmlAttribute* node = attributeSet.Find( name );
    if ( !node )
      return XML_NO_ATTRIBUTE;

    std::stringstream sstream( node->ValueStr() );
    sstream >> *outValue;
    if ( !sstream.fail() )
      return XML_SUCCESS;
    return XML_WRONG_TYPE;
  }
  #endif

  /** Sets an attribute of name to a given value. The attribute
    will be created if it does not exist, or changed if it does.
  */
  void SetAttribute( const char* name, const char * _value );

    #ifdef XML_USE_STL
  const std::string* Attribute( const std::string& name ) const;
  const std::string* Attribute( const std::string& name, int* i     ) const;
  const std::string* Attribute( const std::string& name, double* d  ) const;
  int QueryIntAttribute   ( const std::string& name, int*    _value ) const;
  int QueryDoubleAttribute( const std::string& name, double* _value ) const;

  /// STL std::string form.
  void SetAttribute( const std::string& name, const std::string& _value );
  ///< STL std::string form.
  void SetAttribute( const std::string& name, int _value );
  #endif

  /** Sets an attribute of name to a given value. The attribute
    will be created if it does not exist, or changed if it does.
  */
  void SetAttribute( const char * name, int value );

  /** Sets an attribute of name to a given value. The attribute
    will be created if it does not exist, or changed if it does.
  */
  void SetDoubleAttribute( const char * name, double value );

  /** Deletes an attribute with the given name.
  */
  void RemoveAttribute( const char * name );
    #ifdef XML_USE_STL
  void RemoveAttribute( const std::string& name )	{	RemoveAttribute (name.c_str ());	}	///< STL std::string form.
  #endif

  const XmlAttribute* FirstAttribute() const	{ return attributeSet.First(); }		///< Access the first attribute in this element.
        XmlAttribute* FirstAttribute() 				{ return attributeSet.First(); }
  const XmlAttribute* LastAttribute()	const 	{ return attributeSet.Last(); }		///< Access the last attribute in this element.
        XmlAttribute* LastAttribute()					{ return attributeSet.Last(); }

  /** Convenience function for easy access to the text inside an element. Although easy
    and concise, GetText() is limited compared to getting the XmlText child
    and accessing it directly.
  
    If the first child of 'this' is a XmlText, the GetText()
    returns the character string of the Text node, else null is returned.

    This is a convenient method for getting the text of simple contained text:
    @verbatim
    <foo>This is text</foo>
    const char* str = fooElement->GetText();
    @endverbatim

    'str' will be a pointer to "This is text". 
    
    Note that this function can be misleading. If the element foo was created from
    this XML:
    @verbatim
    <foo><b>This is text</b></foo> 
    @endverbatim

    then the value of str would be null. The first child node isn't a text node, it is
    another element. From this XML:
    @verbatim
    <foo>This is <b>text</b></foo> 
    @endverbatim
    GetText() will return "This is ".

    WARNING: GetText() accesses a child node - don't become confused with the 
         similarly named XmlHandle::Text() and XmlNode::ToText() which are 
         safe type casts on the referenced node.
  */
  const char* GetText() const;

  /// Creates a new Element and returns it - the returned element is a copy.
  virtual XmlNode* Clone() const;
  // Print the Element to a FILE stream.
  virtual void Print( FILE* cfile, int depth ) const;
  // Print the element to a XML_STRING
  virtual XML_STRING Print(int depth ) const;

  /*	Attribtue parsing starts: next char past '<'
             returns: next char past '>'
  */
  virtual const char* Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

  virtual const XmlElement*     ToElement()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
  virtual XmlElement*           ToElement()	          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

  /** Walk the XML tree visiting this node and all of its children. 
  */
  virtual bool Accept( XmlVisitor* visitor ) const;

  // Find next element (child or sibling) with this name
  XmlElement* FindElement(const char* p_name,bool p_recurse = true);

protected:

  void CopyTo( XmlElement* target ) const;
  void ClearThis();	// like clear, but initializes 'this' object as well

  // Used to be public [internal use]
  #ifdef XML_USE_STL
  virtual void StreamIn( std::istream * in, XML_STRING * tag );
  #endif
  /*	[internal use]
    Reads the "value" of the element -- another element, or text.
    This should terminate with the current end tag.
  */
  const char* ReadValue( const char* in, XmlParsingData* prevData, XmlEncoding encoding );

private:

  XmlAttributeSet attributeSet;
};


/**	An XML comment.
*/
class XmlComment : public XmlNode
{
public:
  /// Constructs an empty comment.
  XmlComment() : XmlNode( XmlNode::COMMENT ) {}
  /// Construct a comment from text.
  XmlComment( const char* _value ) : XmlNode( XmlNode::COMMENT ) {
    SetValue( _value );
  }
  XmlComment( const XmlComment& );
  void operator=( const XmlComment& base );

  virtual ~XmlComment()	{}

  /// Returns a copy of this Comment.
  virtual XmlNode* Clone() const;
  // Write this Comment to a FILE stream.
  virtual void Print( FILE* cfile, int depth ) const;
  // Print the element to a XML_STRING
  virtual XML_STRING Print(int depth ) const;

  /*	Attribtue parsing starts: at the ! of the !--
             returns: next char past '>'
  */
  virtual const char* Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

  virtual const XmlComment*  ToComment() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
  virtual XmlComment*  ToComment() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

  /** Walk the XML tree visiting this node and all of its children. 
  */
  virtual bool Accept( XmlVisitor* visitor ) const;

protected:
  void CopyTo( XmlComment* target ) const;

  // used to be public
  #ifdef XML_USE_STL
  virtual void StreamIn( std::istream * in, XML_STRING * tag );
  #endif
//	virtual void StreamOut( XML_OSTREAM * out ) const;

private:

};


/** XML text. A text node can have 2 ways to output the next. "normal" output 
  and CDATA. It will default to the mode it was parsed from the XML file and
  you generally want to leave it alone, but you can change the output mode with 
  SetCDATA() and query it with CDATA().
*/
class XmlText : public XmlNode
{
  friend class XmlElement;
public:
  /** Constructor for text element. By default, it is treated as 
    normal, encoded text. If you want it be output as a CDATA text
    element, set the parameter _cdata to 'true'
  */
  XmlText (const char * initValue ) : XmlNode (XmlNode::TEXT)
  {
    SetValue( initValue );
    m_doCDATA = false;
  }
  virtual ~XmlText() {}

  #ifdef XML_USE_STL
  /// Constructor.
  XmlText( const std::string& initValue ) : XmlNode (XmlNode::TEXT)
  {
    SetValue( initValue );
    m_doCDATA = false;
  }
  #endif

  XmlText( const XmlText& copy ) : XmlNode( XmlNode::TEXT )	
  { 
    copy.CopyTo( this ); 
    m_doCDATA = false;
  }
  void operator=( const XmlText& base )							 	{ base.CopyTo( this ); }

  // Write this text object to a FILE stream.
  virtual void Print( FILE* cfile, int depth ) const;
  // Print the element to a XML_STRING
  virtual XML_STRING Print(int depth ) const;

  /// Queries whether this represents text using a CDATA section.
  bool CDATA() const				{ return m_doCDATA; }
  /// Turns on or off a CDATA representation of text.
  void SetCDATA( bool _cdata )	{ m_doCDATA = _cdata; }

  virtual const char* Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

  virtual const XmlText* ToText() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
  virtual XmlText*       ToText()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

  /** Walk the XML tree visiting this node and all of its children. 
  */
  virtual bool Accept( XmlVisitor* content ) const;

protected :
  ///  [internal use] Creates a new Element and returns it.
  virtual XmlNode* Clone() const;
  void CopyTo( XmlText* target ) const;

  bool Blank() const;	// returns true if all white space and new lines
  // [internal use]
  #ifdef XML_USE_STL
  virtual void StreamIn( std::istream * in, XML_STRING * tag );
  #endif

private:
  bool m_doCDATA;			// true if this should be input and output as a CDATA style text element
};


/** In correct XML the declaration is the first entry in the file.
  @verbatim
    <?xml version="1.0" standalone="yes"?>
  @endverbatim

  TinyXml will happily read or write files without a declaration,
  however. There are 3 possible attributes to the declaration:
  version, encoding, and standalone.

  Note: In this version of the code, the attributes are
  handled as special cases, not generic attributes, simply
  because there can only be at most 3 and they are always the same.
*/
class XmlDeclaration : public XmlNode
{
public:
  /// Construct an empty declaration.
  XmlDeclaration()   : XmlNode( XmlNode::DECLARATION ) {}

#ifdef XML_USE_STL
  /// Constructor.
  XmlDeclaration(	const std::string& _version,
            const std::string& _encoding,
            const std::string& _standalone );
#endif

  /// Construct.
  XmlDeclaration(	const char* _version,
            const char* _encoding,
            const char* _standalone );

  XmlDeclaration( const XmlDeclaration& copy );
  void operator=( const XmlDeclaration& copy );

  virtual ~XmlDeclaration()	{}

  /// Version. Will return an empty string if none was found.
  const char *Version() const			{ return version.c_str (); }
  /// Encoding. Will return an empty string if none was found.
  const char *Encoding() const		{ return encoding.c_str (); }
  /// Is this a standalone document?
  const char *Standalone() const		{ return standalone.c_str (); }

  /// Creates a copy of this Declaration and returns it.
  virtual XmlNode* Clone() const;
  // Print this declaration to a FILE stream.
  virtual void Print( FILE* cfile, int depth, XML_STRING* str ) const;
  virtual void Print( FILE* cfile, int depth ) const {
    Print( cfile, depth, 0 );
  }
  // Print the element to a XML_STRING
  virtual XML_STRING Print(int depth) const;

  virtual const char* Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

  virtual const XmlDeclaration* ToDeclaration() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
  virtual XmlDeclaration*       ToDeclaration()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

  /** Walk the XML tree visiting this node and all of its children. 
  */
  virtual bool Accept( XmlVisitor* visitor ) const;

protected:
  void CopyTo( XmlDeclaration* target ) const;
  // used to be public
  #ifdef XML_USE_STL
  virtual void StreamIn( std::istream * in, XML_STRING * tag );
  #endif

private:

  XML_STRING version;
  XML_STRING encoding;
  XML_STRING standalone;
};


/** Any tag that tinyXml doesn't recognize is saved as an
  unknown. It is a tag of text, but should not be modified.
  It will be written back to the XML, unchanged, when the file
  is saved.

  DTD tags get thrown into XmlUnknowns.
*/
class XmlUnknown : public XmlNode
{
public:
  XmlUnknown() : XmlNode( XmlNode::UNKNOWN )	{}
  virtual ~XmlUnknown() {}

  XmlUnknown( const XmlUnknown& copy ) : XmlNode( XmlNode::UNKNOWN )		{ copy.CopyTo( this ); }
  void operator=( const XmlUnknown& copy )										{ copy.CopyTo( this ); }

  /// Creates a copy of this Unknown and returns it.
  virtual XmlNode* Clone() const;
  // Print this Unknown to a FILE stream.
  virtual void Print( FILE* cfile, int depth ) const;
  // Print the element to a XML_STRING
  virtual XML_STRING Print(int depth ) const;

  virtual const char* Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

  virtual const XmlUnknown*     ToUnknown()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
  virtual XmlUnknown*           ToUnknown()	    { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

  /** Walk the XML tree visiting this node and all of its children. 
  */
  virtual bool Accept( XmlVisitor* content ) const;

protected:
  void CopyTo( XmlUnknown* target ) const;

  #ifdef XML_USE_STL
  virtual void StreamIn( std::istream * in, XML_STRING * tag );
  #endif

private:

};


/** Always the top level node. A document binds together all the
  XML pieces. It can be saved, loaded, and printed to the screen.
  The 'value' of a document node is the xml file name.
*/
class XmlDocument : public XmlNode
{
public:
  /// Create an empty document, that has no name.
  XmlDocument();
  /// Create a document with a name. The name of the document is also the filename of the xml.
  XmlDocument( const char * documentName );

  #ifdef XML_USE_STL
  /// Constructor.
  XmlDocument( const std::string& documentName );
  #endif

  XmlDocument( const XmlDocument& copy );
  void operator=( const XmlDocument& copy );

  virtual ~XmlDocument() {}

  /** Load a file using the current document value.
    Returns true if successful. Will delete any existing
    document data before loading.
  */
  bool LoadFile( XmlEncoding encoding = XML_DEFAULT_ENCODING );
  /// Save a file using the current document value. Returns true if successful.
  bool SaveFile() const;
  /// Load a file using the given filename. Returns true if successful.
  bool LoadFile( const char * filename, XmlEncoding encoding = XML_DEFAULT_ENCODING );
  /// Save a file using the given filename. Returns true if successful.
  bool SaveFile( const char * filename ) const;
  /** Load a file using the given FILE*. Returns true if successful. Note that this method
    doesn't stream - the entire object pointed at by the FILE*
    will be interpreted as an XML file. TinyXML doesn't stream in XML from the current
    file location. Streaming may be added in the future.
  */
  bool LoadFile( FILE*, XmlEncoding encoding = XML_DEFAULT_ENCODING );
  /// Save a file using the given FILE*. Returns true if successful.
  bool SaveFile( FILE* ) const;

  #ifdef XML_USE_STL
  bool LoadFile( const std::string& filename, XmlEncoding encoding = XML_DEFAULT_ENCODING )			///< STL std::string version.
  {
//		StringToBuffer f( filename );
//		return ( f.buffer && LoadFile( f.buffer, encoding ));
    return LoadFile( filename.c_str(), encoding );
  }
  bool SaveFile( const std::string& filename ) const		///< STL std::string version.
  {
//		StringToBuffer f( filename );
//		return ( f.buffer && SaveFile( f.buffer ));
    return SaveFile( filename.c_str() );
  }
  #endif

  /** Parse the given null terminated block of xml data. Passing in an encoding to this
    method (either XML_ENCODING_LEGACY or XML_ENCODING_UTF8 will force TinyXml
    to use that encoding, regardless of what TinyXml might otherwise try to detect.
  */
  virtual const char* Parse( const char* p, XmlParsingData* data = 0, XmlEncoding encoding = XML_DEFAULT_ENCODING );

  /** Get the root element -- the only top level element -- of the document.
    In well formed XML, there should only be one. TinyXml is tolerant of
    multiple elements at the document level.
  */
  const XmlElement* RootElement() const		{ return FirstChildElement(); }
  XmlElement* RootElement()					{ return FirstChildElement(); }

  /** If an error occurs, Error will be set to true. Also,
    - The ErrorId() will contain the integer identifier of the error (not generally useful)
    - The ErrorDesc() method will return the name of the error. (very useful)
    - The ErrorRow() and ErrorCol() will return the location of the error (if known)
  */	
  bool Error() const						{ return error; }

  /// Contains a textual (english) description of the error if one occurs.
  const char * ErrorDesc() const	{ return errorDesc.c_str (); }

  /** Generally, you probably want the error string ( ErrorDesc() ). But if you
    prefer the ErrorId, this function will fetch it.
  */
  int ErrorId()	const				{ return errorId; }

  /** Returns the location (if known) of the error. The first column is column 1, 
    and the first row is row 1. A value of 0 means the row and column wasn't applicable
    (memory errors, for example, have no row/column) or the parser lost the error. (An
    error in the error reporting, in that case.)

    @sa SetTabSize, Row, Column
  */
  int ErrorRow() const	{ return errorLocation.row+1; }
  int ErrorCol() const	{ return errorLocation.col+1; }	///< The column where the error occured. See ErrorRow()

  /** SetTabSize() allows the error reporting functions (ErrorRow() and ErrorCol())
    to report the correct values for row and column. It does not change the output
    or input in any way.
    
    By calling this method, with a tab size
    greater than 0, the row and column of each node and attribute is stored
    when the file is loaded. Very useful for tracking the DOM back in to
    the source file.

    The tab size is required for calculating the location of nodes. If not
    set, the default of 4 is used. The tabsize is set per document. Setting
    the tabsize to 0 disables row/column tracking.

    Note that row and column tracking is not supported when using operator>>.

    The tab size needs to be enabled before the parse or load. Correct usage:
    @verbatim
    XmlDocument doc;
    doc.SetTabSize( 8 );
    doc.Load( "myfile.xml" );
    @endverbatim

    @sa Row, Column
  */
  void SetTabSize( int _tabsize )		{ tabsize = _tabsize; }

  int TabSize() const	{ return tabsize; }

  /** If you have handled the error, it can be reset with this call. The error
    state is automatically cleared if you Parse a new XML block.
  */
  void ClearError()						{	error = false; 
                        errorId = 0; 
                        errorDesc = ""; 
                        errorLocation.row = errorLocation.col = 0; 
                        //errorLocation.last = 0; 
                      }

  /** Write the document to standard out using formatted printing ("pretty print"). */
  void Print() const						{ Print( stdout, 0 ); }

  /* Write the document to a string using formatted printing ("pretty print"). This
    will allocate a character array (new char[]) and return it as a pointer. The
    calling code pust call delete[] on the return char* to avoid a memory leak.
  */
  //char* PrintToMemory() const; 

  /// Print this Document to a FILE stream.
  virtual void Print( FILE* cfile, int depth = 0 ) const;
  // Print the element to a XML_STRING
  virtual XML_STRING Print(int depth ) const;
  // [internal use]
  void SetError( int err, const char* errorLocation, XmlParsingData* prevData, XmlEncoding encoding );

  virtual const XmlDocument*    ToDocument()    const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
  virtual XmlDocument*          ToDocument()          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

  /** Walk the XML tree visiting this node and all of its children. 
  */
  virtual bool Accept( XmlVisitor* content ) const;

protected :
  // [internal use]
  virtual XmlNode* Clone() const;
  #ifdef XML_USE_STL
  virtual void StreamIn( std::istream * in, XML_STRING * tag );
  #endif

private:
  void CopyTo( XmlDocument* target ) const;

  bool error;
  int  errorId;
  XML_STRING errorDesc;
  int tabsize;
  XmlCursor errorLocation;
  bool useMicrosoftBOM;		// the UTF-8 BOM were found when read. Note this, and try to write.
};


/**
  A XmlHandle is a class that wraps a node pointer with null checks; this is
  an incredibly useful thing. Note that XmlHandle is not part of the TinyXml
  DOM structure. It is a separate utility class.

  Take an example:
  @verbatim
  <Document>
    <Element attributeA = "valueA">
      <Child attributeB = "value1" />
      <Child attributeB = "value2" />
    </Element>
  <Document>
  @endverbatim

  Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very 
  easy to write a *lot* of code that looks like:

  @verbatim
  XmlElement* root = document.FirstChildElement( "Document" );
  if ( root )
  {
    XmlElement* element = root->FirstChildElement( "Element" );
    if ( element )
    {
      XmlElement* child = element->FirstChildElement( "Child" );
      if ( child )
      {
        XmlElement* child2 = child->NextSiblingElement( "Child" );
        if ( child2 )
        {
          // Finally do something useful.
  @endverbatim

  And that doesn't even cover "else" cases. XmlHandle addresses the verbosity
  of such code. A XmlHandle checks for null	pointers so it is perfectly safe 
  and correct to use:

  @verbatim
  XmlHandle docHandle( &document );
  XmlElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).ToElement();
  if ( child2 )
  {
    // do something useful
  @endverbatim

  Which is MUCH more concise and useful.

  It is also safe to copy handles - internally they are nothing more than node pointers.
  @verbatim
  XmlHandle handleCopy = handle;
  @endverbatim

  What they should not be used for is iteration:

  @verbatim
  int i=0; 
  while ( true )
  {
    XmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).ToElement();
    if ( !child )
      break;
    // do something
    ++i;
  }
  @endverbatim

  It seems reasonable, but it is in fact two embedded while loops. The Child method is 
  a linear walk to find the element, so this code would iterate much more than it needs 
  to. Instead, prefer:

  @verbatim
  XmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).ToElement();

  for( child; child; child=child->NextSiblingElement() )
  {
    // do something
  }
  @endverbatim
*/
class XmlHandle
{
public:
  /// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
  XmlHandle( XmlNode* _node )					{ this->node = _node; }
  /// Copy constructor
  XmlHandle( const XmlHandle& ref )			{ this->node = ref.node; }
  XmlHandle operator=( const XmlHandle& ref ) { this->node = ref.node; return *this; }

  /// Return a handle to the first child node.
  XmlHandle FirstChild() const;
  /// Return a handle to the first child node with the given name.
  XmlHandle FirstChild( const char * value ) const;
  /// Return a handle to the first child element.
  XmlHandle FirstChildElement() const;
  /// Return a handle to the first child element with the given name.
  XmlHandle FirstChildElement( const char * value ) const;

  /** Return a handle to the "index" child with the given name. 
    The first child is 0, the second 1, etc.
  */
  XmlHandle Child( const char* value, int index ) const;
  /** Return a handle to the "index" child. 
    The first child is 0, the second 1, etc.
  */
  XmlHandle Child( int index ) const;
  /** Return a handle to the "index" child element with the given name. 
    The first child element is 0, the second 1, etc. Note that only XmlElements
    are indexed: other types are not counted.
  */
  XmlHandle ChildElement( const char* value, int index ) const;
  /** Return a handle to the "index" child element. 
    The first child element is 0, the second 1, etc. Note that only XmlElements
    are indexed: other types are not counted.
  */
  XmlHandle ChildElement( int index ) const;

  #ifdef XML_USE_STL
  XmlHandle FirstChild( const std::string& _value ) const				{ return FirstChild( _value.c_str() ); }
  XmlHandle FirstChildElement( const std::string& _value ) const		{ return FirstChildElement( _value.c_str() ); }

  XmlHandle Child( const std::string& _value, int index ) const			{ return Child( _value.c_str(), index ); }
  XmlHandle ChildElement( const std::string& _value, int index ) const	{ return ChildElement( _value.c_str(), index ); }
  #endif

  /** Return the handle as a XmlNode. This may return null.
  */
  XmlNode* ToNode() const			{ return node; } 
  /** Return the handle as a XmlElement. This may return null.
  */
  XmlElement* ToElement() const		{ return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); }
  /**	Return the handle as a XmlText. This may return null.
  */
  XmlText* ToText() const			{ return ( ( node && node->ToText() ) ? node->ToText() : 0 ); }
  /** Return the handle as a XmlUnknown. This may return null.
  */
  XmlUnknown* ToUnknown() const		{ return ( ( node && node->ToUnknown() ) ? node->ToUnknown() : 0 ); }

  /** @deprecated use ToNode. 
    Return the handle as a XmlNode. This may return null.
  */
  XmlNode* Node() const			{ return ToNode(); } 
  /** @deprecated use ToElement. 
    Return the handle as a XmlElement. This may return null.
  */
  XmlElement* Element() const	{ return ToElement(); }
  /**	@deprecated use ToText()
    Return the handle as a XmlText. This may return null.
  */
  XmlText* Text() const			{ return ToText(); }
  /** @deprecated use ToUnknown()
    Return the handle as a XmlUnknown. This may return null.
  */
  XmlUnknown* Unknown() const	{ return ToUnknown(); }

private:
  XmlNode* node;
};


/** Print to memory functionality. The XmlPrinter is useful when you need to:

  -# Print to memory (especially in non-STL mode)
  -# Control formatting (line endings, etc.)

  When constructed, the XmlPrinter is in its default "pretty printing" mode.
  Before calling Accept() you can call methods to control the printing
  of the XML document. After XmlNode::Accept() is called, the printed document can
  be accessed via the CStr(), Str(), and Size() methods.

  XmlPrinter uses the Visitor API.
  @verbatim
  XmlPrinter printer;
  printer.SetIndent( "\t" );

  doc.Accept( &printer );
  fprintf( stdout, "%s", printer.CStr() );
  @endverbatim
*/
class XmlPrinter : public XmlVisitor
{
public:
  XmlPrinter() : depth( 0 ), simpleTextPrint( false ),
           buffer(), indent( "    " ), lineBreak( "\n" ) {}

  virtual bool VisitEnter( const XmlDocument& doc );
  virtual bool VisitExit( const XmlDocument& doc );

  virtual bool VisitEnter( const XmlElement& element, const XmlAttribute* firstAttribute );
  virtual bool VisitExit( const XmlElement& element );

  virtual bool Visit( const XmlDeclaration& declaration );
  virtual bool Visit( const XmlText& text );
  virtual bool Visit( const XmlComment& comment );
  virtual bool Visit( const XmlUnknown& unknown );

  /** Set the indent characters for printing. By default 4 spaces
    but tab (\t) is also useful, or null/empty string for no indentation.
  */
  void SetIndent( const char* _indent )			{ indent = _indent ? _indent : "" ; }
  /// Query the indention string.
  const char* Indent()							{ return indent.c_str(); }
  /** Set the line breaking string. By default set to newline (\n). 
    Some operating systems prefer other characters, or can be
    set to the null/empty string for no indenation.
  */
  void SetLineBreak( const char* _lineBreak )		{ lineBreak = _lineBreak ? _lineBreak : ""; }
  /// Query the current line breaking string.
  const char* LineBreak()							{ return lineBreak.c_str(); }

  /** Switch over to "stream printing" which is the most dense formatting without 
    linebreaks. Common when the XML is needed for network transmission.
  */
  void SetStreamPrinting()						{ indent = "";
                            lineBreak = "";
                          }	
  /// Return the result.
  const char* CStr()								{ return buffer.c_str(); }
  /// Return the length of the result string.
  size_t Size()									{ return buffer.size(); }

  #ifdef XML_USE_STL
  /// Return the result.
  const std::string& Str()						{ return buffer; }
  #endif

private:
  void DoIndent()	{
    for( int i=0; i<depth; ++i )
      buffer += indent;
  }
  void DoLineBreak() {
    buffer += lineBreak;
  }

  int depth;
  bool simpleTextPrint;
  XML_STRING buffer;
  XML_STRING indent;
  XML_STRING lineBreak;
};


#ifdef _MSC_VER
#pragma warning( pop )
#endif

