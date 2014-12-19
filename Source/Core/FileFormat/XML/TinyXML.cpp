/****************************************************************************/
/**
 *  @file TinyXML.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TinyXML.cpp 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
/*
  Copyright (c) 2000 Lee Thomason (www.grinninglizard.com)

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any
  damages arising from the use of this software.

  Permission is granted to anyone to use this software for any
  purpose, including commercial applications, and to alter it and
  redistribute it freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must
  not claim that you wrote the original software. If you use this
  software in a product, an acknowledgment in the product documentation
  would be appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and
  must not be misrepresented as being the original software.

  3. This notice may not be removed or altered from any source
  distribution.
*/
#include "TinyXML.h"
#include <cctype>
#include <kvs/DebugNew>


namespace { template <typename T> void Ignore( T ){} }

const char* TiXmlBase::errorString[ TIXML_ERROR_STRING_COUNT ] =
{
    "No error.",
    "Failed to open file.",
    "Memory allocation failed.",
    "Error parsing Element.",
    "Failed to read Element name",
    "Error reading Element value.",
    "Error reading Attributes.",
    "Error: empty tag.",
    "Error reading end tag.",
    "Error parsing Unknown.",
    "Error parsing Comment.",
    "Error parsing Declaration.",
};


const char* TiXmlBase::SkipWhiteSpace( const char* p )
{
    while ( p && *p && 
            ( isspace( *p ) || *p == '\n' || *p == '\r' ) )
        p++;
    return p;
}

const char* TiXmlBase::ReadName( const char* p, std::string* name )
{
    *name = "";
    const char* start = p;

    // Names start with letters or underscores.
    // After that, they can be letters, underscores, numbers,
    // hyphens, or colons. (Colons are valid ony for namespaces,
    // but tinyxml can't tell namespaces from names.)
    if ( p && ( isalpha( *p ) || *p == '_' ) )
    {
        p++;
        while( p && *p && 
               (   isalnum( *p ) 
                   || *p == '_'
                   || *p == '-'
                   || *p == ':' ) )
        {
            p++;
        }
        name->append( start, p - start );
        return p;
    }
    return 0;
}

const char* TiXmlBase::ReadText(
    const char*  p,
    std::string* text,
    bool         trimWhiteSpace,
    const char*  endTag,
    bool         caseInsensitive )
{
    ::Ignore( caseInsensitive );

    *text = "";
    if ( !trimWhiteSpace                // certain tags always keep whitespace
         /*|| !condenseWhiteSpace*/ )   // if true, whitespace is always kept
    {
        // Keep all the white space.
        while (    p && *p
                   && strncmp( p, endTag, strlen(endTag) ) != 0
            )
        {
            char c = *(p++);
            (* text) += c;
        }
    }
    else
    {
        bool whitespace = false;

        // Remove leading white space:
        p = SkipWhiteSpace( p );
        while (    p && *p
                   && strncmp( p, endTag, strlen(endTag) ) != 0 )
        {
            if ( *p == '\r' || *p == '\n' )
            {
                whitespace = true;
                ++p;
            }
            else if ( isspace( *p ) )
            {
                whitespace = true;
                ++p;
            }
            else
            {
                // If we've found whitespace, add it before the
                // new character. Any whitespace just becomes a space.
                if ( whitespace )
                {
                    (* text) += ' ';
                    whitespace = false;
                }
                char c = *(p++);
                (* text) += c;
            }
        }
    }
    return p + strlen( endTag );
}


const char* TiXmlDocument::Parse( const char* start )
{
    // Parse away, at the document level. Since a document
    // contains nothing but other tags, most of what happens
    // here is skipping white space.

    const char* p = start;

    p = SkipWhiteSpace( p );
    if ( !p || !*p )
    {
        error = true;
        errorDesc = "Document empty.";
    }

    while ( p && *p )
    {
        if ( *p != '<' )
        {
            error = true;
            errorDesc = "The '<' symbol that starts a tag was not found.";
            break;
        }
        else
        {
            TiXmlNode* node = IdentifyAndParse( &p );
            if ( node )
            {
                LinkEndChild( node );
            }
        }
        p = SkipWhiteSpace( p );
    }
    return 0;   // Return null is fine for a document: once it is read, the parsing is over.
}


TiXmlNode* TiXmlNode::IdentifyAndParse( const char** where )
{
    const char* p = *where;
    TiXmlNode* returnNode = 0;
    assert( *p == '<' );
    TiXmlDocument* doc = GetDocument();

    p = SkipWhiteSpace( p+1 );

    // What is this thing?
    // - Elements start with a letter or underscore, but xml is reserved.
    // - Comments: <!--
    // - Everthing else is unknown to tinyxml.
    //
    if (           tolower( *(p+0) ) == '?'
                   && tolower( *(p+1) ) == 'x' 
                   && tolower( *(p+2) ) == 'm'
                   && tolower( *(p+3) ) == 'l' )
    {
#ifdef DEBUG_PARSER
        printf( "XML parsing Declaration\n" );
#endif
        returnNode = new TiXmlDeclaration();
    }
    else if ( isalpha( *p ) || *p == '_' )
    {
#ifdef DEBUG_PARSER
        printf( "XML parsing Element\n" );
#endif
        returnNode = new TiXmlElement( "" );
    }
    else if (    *(p+0) == '!'
                 && *(p+1) == '-'
                 && *(p+2) == '-' )
    {
#ifdef DEBUG_PARSER
        printf( "XML parsing Comment\n" );
#endif
        returnNode = new TiXmlComment();
    }
    else if ( strncmp(p, "![CDATA[", 8) == 0 )
    {
        TiXmlNode* cdataNode = new TiXmlCData();
        if ( !cdataNode )
        {
            if ( doc ) doc->SetError( TIXML_ERROR_OUT_OF_MEMORY );
            return 0;
        }

        returnNode = cdataNode;
    }
    else
    {
#ifdef DEBUG_PARSER
        printf( "XML parsing Comment\n" );
#endif
        returnNode = new TiXmlUnknown();
    }

    if ( returnNode )
    {
        // Set the parent, so it can report errors
        returnNode->parent = this;
        p = returnNode->Parse( p );
    }
    else
    {
        if ( doc )
            doc->SetError( TIXML_ERROR_OUT_OF_MEMORY );
        p = 0;
    }
    *where = p;
    return returnNode;
}


const char* TiXmlElement::Parse( const char* p )
{
    TiXmlDocument* document = GetDocument();
    p = SkipWhiteSpace( p );
    if ( !p || !*p )
    {
        if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT );
        return 0;
    }

    // Read the name.
    p = ReadName( p, &value );
    if ( !p )
    {
        if ( document ) document->SetError( TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME );
        return 0;
    }

    std::string endTag = "</";
    endTag += value;
    endTag += ">";

    // Check for and read attributes. Also look for an empty
    // tag or an end tag.
    while ( p && *p )
    {
        p = SkipWhiteSpace( p );
        if ( !p || !*p )
        {
            if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES );
            return 0;
        }
        if ( *p == '/' )
        {
            // Empty tag.
            if ( *(p+1) != '>' )
            {
                if ( document ) document->SetError( TIXML_ERROR_PARSING_EMPTY );
                return 0;
            }
            return p+2;
        }
        else if ( *p == '>' )
        {
            // Done with attributes (if there were any.)
            // Read the value -- which can include other
            // elements -- read the end tag, and return.
            p = ReadValue( p+1 ); // Note this is an Element method, and will set the error if one happens.
            if ( !p )
                return 0;

            // We should find the end tag now
            std::string buf( p, endTag.size() );
            if ( endTag == buf )
            {
                return p+endTag.size();
            }
            else
            {
                if ( document ) document->SetError( TIXML_ERROR_READING_END_TAG );
                return 0;
            }
        }
        else
        {
            // Try to read an element:
            TiXmlAttribute attrib;
            attrib.SetDocument( document );
            p = attrib.Parse( p );

            if ( p )
            {
                SetAttribute( attrib.Name(), attrib.Value() );
            }
        }
    }
    return 0;
}


const char* TiXmlElement::ReadValue( const char* p )
{
    TiXmlDocument* document = GetDocument();

    // Read in text and elements in any order.
    p = SkipWhiteSpace( p );
    while ( p && *p )
    {
        const char* start = p;
        while ( *p && *p != '<' )
            p++;

        if ( !*p )
        {
            if ( document ) document->SetError( TIXML_ERROR_READING_ELEMENT_VALUE );
            return 0;
        }
        if ( p != start )
        {
            // Take what we have, make a text element.
            TiXmlText* text = new TiXmlText();

            if ( !text )
            {
                if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY );
                return 0;
            }
            text->Parse( start );
            if ( !text->Blank() )
                LinkEndChild( text );
            else
                delete text;
        } 
        else 
        {
            // We hit a '<'
            // Have we hit a new element or an end tag?
            if ( *(p+1) == '/' )
            {
                return p;       // end tag
            }
            else
            {
//                              TiXmlElement* element = new TiXmlElement( "" );
// 
//                              if ( element )
//                              {
//                                      p = element->Parse( p+1 );
//                                      if ( p )
//                                              LinkEndChild( element );
//                              }
//                              else
//                              {
//                                      if ( document ) document->SetError( ERROR_OUT_OF_MEMORY );
//                                      return 0;
//                              }
                TiXmlNode* node = IdentifyAndParse( &p );
                if ( node )
                {
                    LinkEndChild( node );
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    return 0;
}


const char* TiXmlUnknown::Parse( const char* p )
{
    const char* end = strchr( p, '>' );
    if ( !end )
    {
        TiXmlDocument* document = GetDocument();
        if ( document )
            document->SetError( TIXML_ERROR_PARSING_UNKNOWN );
        return 0;
    }
    else
    {
        value = std::string( p, end-p );
//              value.resize( end - p );
        return end + 1;                 // return just past the '>'
    }
}


const char* TiXmlComment::Parse( const char* p )
{
    assert( *p == '!' && *(p+1) == '-' && *(p+2) == '-' );

    // Find the end, copy the parts between to the value of
    // this object, and return.
    const char* start = p+3;
    const char* end = strstr( p, "-->" );
    if ( !end )
    {
        TiXmlDocument* document = GetDocument();
        if ( document )
            document->SetError( TIXML_ERROR_PARSING_COMMENT );
        return 0;
    }
    else
    {
        // Assemble the comment, removing the white space.
        bool whiteSpace = false;

        const char* q;
        for( q=start; q<end; q++ )
        {
            if ( isspace( *q ) )
            {
                if ( !whiteSpace )
                {
                    value += ' ';
                    whiteSpace = true;
                }
            }
            else
            {
                value += *q;
                whiteSpace = false;
            }
        }
//              value = std::string( start, end-start );
        return end + 3;                 // return just past the '>'
    }
}


const char* TiXmlAttribute::Parse( const char* p )
{
    // Read the name, the '=' and the value.
    p = ReadName( p, &name );
    if ( !p )
    {
        if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES );
        return 0;
    }
    p = SkipWhiteSpace( p );
    if ( !p || *p != '=' )
    {
        if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES );
        return 0;
    }

    p = SkipWhiteSpace( p+1 );
    if ( !p || !*p )
    {
        if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES );
        return 0;
    }

    const char* end = 0;
    const char* start = p+1;
    const char* past = 0;

    if ( *p == '\'' )
    {
        end = strchr( start, '\'' );
        past = end+1;
    }
    else if ( *p == '"' )
    {
        end = strchr( start, '"' );
        past = end+1;
    }
    else
    {
        // All attribute values should be in single or double quotes.
        // But this is such a common error that the parser will try
        // its best, even without them.
        start--;
        for ( end = start; *end; end++ )
        {
            if ( isspace( *end ) || *end == '/' || *end == '>' )
                break;
        }
        past = end;
    }
    value = std::string( start, end-start );
    return past;
}


const char* TiXmlText::Parse( const char* p )
{
    value = "";
    bool ignoreWhite = true;

    const char* end = "<";
    p = ReadText( p, &value, ignoreWhite, end, false );

    if ( p )
        return p-1;     // don't truncate the '<'
    return 0;

#if 0
    // Remove leading white space:
    p = SkipWhiteSpace( p );
    while ( *p && *p != '<' )
    {
        if ( *p == '\r' || *p == '\n' )
        {
            whitespace = true;
        }
        else if ( isspace( *p ) )
        {
            whitespace = true;
        }
        else
        {
            // If we've found whitespace, add it before the
            // new character. Any whitespace just becomes a space.
            if ( whitespace )
            {
                value += ' ';
                whitespace = false;
            }
            value += *p;
        }
        p++;
    }
    // Keep white space before the '<' 
    if ( whitespace )
        value += ' ';

    return p;
#endif
}


const char* TiXmlCData::Parse( const char* p )
{
    value = "";

    bool ignoreWhite = false;
    p += 8;
    const char* end = "]]>";
    p = ReadText( p, &value, ignoreWhite, end, false );
    if ( p )
        return p;
    return 0;
}


const char* TiXmlDeclaration::Parse( const char* p )
{
    // Find the beginning, find the end, and look for
    // the stuff in-between.
    const char* start = p+4;
    const char* end  = strstr( start, "?>" );

    // Be nice to the user:
    if ( !end )
    {
        end = strstr( start, ">" );
        end++;
    }
    else
    {
        end += 2;
    }

    if ( !end )
    {
        TiXmlDocument* document = GetDocument();
        if ( document )
            document->SetError( TIXML_ERROR_PARSING_DECLARATION );
        return 0;
    }
    else
    {
        const char* p;

        p = strstr( start, "version" );
        if ( p && p < end )
        {
            TiXmlAttribute attrib;
            attrib.Parse( p );
            version = attrib.Value();
        }

        p = strstr( start, "encoding" );
        if ( p && p < end )
        {
            TiXmlAttribute attrib;
            attrib.Parse( p );
            encoding = attrib.Value();
        }

        p = strstr( start, "standalone" );
        if ( p && p < end )
        {
            TiXmlAttribute attrib;
            attrib.Parse( p );
            standalone = attrib.Value();
        }
    }
    return end;
}

bool TiXmlText::Blank()
{
    for ( unsigned i=0; i<value.size(); i++ )
        if ( !isspace( value[i] ) )
            return false;
    return true;
}


TiXmlNode::TiXmlNode( NodeType _type )
{
    parent = 0;
    type = _type;
    firstChild = 0;
    lastChild = 0;
    prev = 0;
    next = 0;
}


TiXmlNode::~TiXmlNode()
{
    TiXmlNode* node = firstChild;
    TiXmlNode* temp = 0;

    while ( node )
    {
        temp = node;
        node = node->next;
        delete temp;
    }
}

void TiXmlNode::Clear()
{
    TiXmlNode* node = firstChild;
    TiXmlNode* temp = 0;

    while ( node )
    {
        temp = node;
        node = node->next;
        delete temp;
    }

    firstChild = 0;
    lastChild = 0;
}


TiXmlNode* TiXmlNode::LinkEndChild( TiXmlNode* node )
{
    node->parent = this;

    node->prev = lastChild;
    node->next = 0;

    if ( lastChild )
        lastChild->next = node;
    else
        firstChild = node; // it was an empty list.

    lastChild = node;
    return node;
}

TiXmlNode* TiXmlNode::InsertEndChild( const TiXmlNode& addThis )
{
    TiXmlNode* node = addThis.Clone();
    if ( !node )
        return 0;

    return LinkEndChild( node );
}

TiXmlNode* TiXmlNode::InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis )
{
    if ( beforeThis->parent != this )
        return 0;

    TiXmlNode* node = addThis.Clone();
    if ( !node )
        return 0;
    node->parent = this;

    node->next = beforeThis;
    node->prev = beforeThis->prev;
    beforeThis->prev->next = node;
    beforeThis->prev = node;
    return node;
}


TiXmlNode* TiXmlNode::InsertAfterChild( TiXmlNode* afterThis, const TiXmlNode& addThis )
{
    if ( afterThis->parent != this )
        return 0;

    TiXmlNode* node = addThis.Clone();
    if ( !node )
        return 0;
    node->parent = this;

    node->prev = afterThis;
    node->next = afterThis->next;
    afterThis->next->prev = node;
    afterThis->next = node;
    return node;
}


TiXmlNode* TiXmlNode::ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis )
{
    if ( replaceThis->parent != this )
        return 0;

    TiXmlNode* node = withThis.Clone();
    if ( !node )
        return 0;

    node->next = replaceThis->next;
    node->prev = replaceThis->prev;

    if ( replaceThis->next )
        replaceThis->next->prev = node;
    else
        lastChild = node;

    if ( replaceThis->prev )
        replaceThis->prev->next = node;
    else
        firstChild = node;

    delete replaceThis;
    return node;
}


bool TiXmlNode::RemoveChild( TiXmlNode* removeThis )
{
    if ( removeThis->parent != this )
    {
        assert( 0 );
        return false;
    }

    if ( removeThis->next )
        removeThis->next->prev = removeThis->prev;
    else
        lastChild = removeThis->prev;

    if ( removeThis->prev )
        removeThis->prev->next = removeThis->next;
    else
        firstChild = removeThis->next;

    delete removeThis;
    return true;
}


TiXmlNode* TiXmlNode::FirstChild( const std::string& value ) const
{
    TiXmlNode* node;
    for ( node = firstChild; node; node = node->next )
    {
        if ( node->Value() == value )
            return node;
    }
    return 0;
}


TiXmlNode* TiXmlNode::LastChild( const std::string& value ) const
{
    TiXmlNode* node;
    for ( node = lastChild; node; node = node->prev )
    {
        if ( node->Value() == value )
            return node;
    }
    return 0;
}


TiXmlNode* TiXmlNode::IterateChildren( TiXmlNode* previous )
{
    if ( !previous )
    {
        return FirstChild();
    }
    else
    {
        assert( previous->parent == this );
        return previous->NextSibling();
    }
}


TiXmlNode* TiXmlNode::IterateChildren( const std::string& val, TiXmlNode* previous )
{
    if ( !previous )
    {
        return FirstChild( val );
    }
    else
    {
        assert( previous->parent == this );
        return previous->NextSibling( val );
    }
}


TiXmlNode* TiXmlNode::NextSibling( const std::string& value ) const
{
    TiXmlNode* node;
    for ( node = next; node; node = node->next )
    {
        if ( node->Value() == value )
            return node;
    }
    return 0;
}


TiXmlNode* TiXmlNode::PreviousSibling( const std::string& value ) const
{
    TiXmlNode* node;
    for ( node = prev; node; node = node->prev )
    {
        if ( node->Value() == value )
            return node;
    }
    return 0;
}


void TiXmlElement::RemoveAttribute( const std::string& name )
{
    TiXmlAttribute* node = attributeSet.Find( name );
    if ( node )
    {
        attributeSet.Remove( node );
        delete node;
    }
}


TiXmlElement* TiXmlNode::FirstChildElement() const
{
    TiXmlNode* node;

    for (       node = FirstChild();
                node;
                node = node->NextSibling() )
    {
        if ( node->ToElement() )
            return node->ToElement();
    }
    return 0;
}


TiXmlElement* TiXmlNode::FirstChildElement( const std::string& value ) const
{
    TiXmlNode* node;

    for (       node = FirstChild( value );
                node;
                node = node->NextSibling( value ) )
    {
        if ( node->ToElement() )
            return node->ToElement();
    }
    return 0;
}


TiXmlElement* TiXmlNode::NextSiblingElement() const
{
    TiXmlNode* node;

    for (       node = NextSibling();
                node;
                node = node->NextSibling() )
    {
        if ( node->ToElement() )
            return node->ToElement();
    }
    return 0;
}


TiXmlElement* TiXmlNode::NextSiblingElement( const std::string& value ) const
{
    TiXmlNode* node;

    for (       node = NextSibling( value );
                node;
                node = node->NextSibling( value ) )
    {
        if ( node->ToElement() )
            return node->ToElement();
    }
    return 0;
}



TiXmlDocument* TiXmlNode::GetDocument() const
{
    const TiXmlNode* node;

    for( node = this; node; node = node->parent )
    {
        if ( node->ToDocument() )
            return node->ToDocument();
    }
    return 0;
}


// TiXmlElement::TiXmlElement() 
//      : TiXmlNode( TiXmlNode::ELEMENT )
// {
// }

TiXmlElement::TiXmlElement( const std::string& _value ) 
    : TiXmlNode( TiXmlNode::ELEMENT )
{
    firstChild = lastChild = 0;
    value = _value;
}

TiXmlElement::~TiXmlElement()
{
    while( attributeSet.First() )
    {
        TiXmlAttribute* node = attributeSet.First();
        attributeSet.Remove( node );
        delete node;
    }
}

const std::string* TiXmlElement::Attribute( const std::string& name ) const
{
    TiXmlAttribute* node = attributeSet.Find( name );

    if ( node )
        return &(node->Value() );

    return 0;
}


const std::string* TiXmlElement::Attribute( const std::string& name, int* i ) const
{
    const std::string* s = Attribute( name );
    if ( s )
        *i = atoi( s->c_str() );
    else
        *i = 0;
    return s;
}


void TiXmlElement::SetAttribute( const std::string& name, int val )
{
    char buf[64];
    sprintf( buf, "%d", val );

    std::string v = buf;

    SetAttribute( name, v );
}


void TiXmlElement::SetAttribute( const std::string& name, const std::string& value )
{
    TiXmlAttribute* node = attributeSet.Find( name );
    if ( node )
    {
        node->SetValue( value );
        return;
    }

    TiXmlAttribute* attrib = new TiXmlAttribute( name, value );
    if ( attrib )
    {
        attributeSet.Add( attrib );
    }
    else
    {
        TiXmlDocument* document = GetDocument();
        if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY );
    }
}


void TiXmlElement::Print( FILE* fp, int depth )
{
    int i;
    for ( i=0; i<depth; i++ )
        fprintf( fp, "    " );

    fprintf( fp, "<%s", value.c_str() );

    TiXmlAttribute* attrib;
    for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
    {
        fprintf( fp, " " );
        attrib->Print( fp, 0 );
    }
    // If this node has children, give it a closing tag. Else
    // make it an empty tag.
    TiXmlNode* node;
    if ( firstChild )
    {
        fprintf( fp, ">" );

        for ( node = firstChild; node; node=node->NextSibling() )
        {
            if ( !node->ToText() )
                fprintf( fp, "\n" );
            node->Print( fp, depth+1 );
        }
        fprintf( fp, "\n" );
        for ( i=0; i<depth; i++ )
            fprintf( fp, "    " );
        fprintf( fp, "</%s>", value.c_str() );
    }
    else
    {
        fprintf( fp, " />" );
    }
}

TiXmlNode* TiXmlElement::Clone() const
{
    TiXmlElement* clone = new TiXmlElement( Value() );

    if ( !clone )
        return 0;

    CopyToClone( clone );

    // Clone the attributes, then clone the children.
    TiXmlAttribute* attribute = 0;
    for(        attribute = attributeSet.First(); 
                attribute; 
                attribute = attribute->Next() )
    {
        clone->SetAttribute( attribute->Name(), attribute->Value() );
    }

    TiXmlNode* node = 0;
    for ( node = firstChild; node; node = node->NextSibling() )
    {
        clone->LinkEndChild( node->Clone() );
    }
    return clone;
}


TiXmlDocument::TiXmlDocument() : TiXmlNode( TiXmlNode::DOCUMENT )
{
    error = false;
//      factory = new TiXmlFactory();
}


TiXmlDocument::TiXmlDocument( const std::string& documentName ) : TiXmlNode( TiXmlNode::DOCUMENT )
{
//      factory = new TiXmlFactory();
    value = documentName;
    error = false;
}

// void TiXmlDocument::SetFactory( TiXmlFactory* f )
// {
//      delete factory;
//      factory = f;
// }


bool TiXmlDocument::LoadFile()
{
    return LoadFile( value );
}


bool TiXmlDocument::SaveFile()
{
    return SaveFile( value );
}


bool TiXmlDocument::LoadFile( FILE* fp )
{
    // Delete the existing data:
    Clear();

    unsigned size, first;
    first = ftell( fp );
    fseek( fp, 0, SEEK_END );
    size = ftell( fp ) - first + 1;
    fseek( fp, first, SEEK_SET );

    char* buf = new char[size];
    char* p = buf;
    while( fgets( p, size, fp ) )
    {
        p = strchr( p, 0 );
    }
    fclose( fp );

    Parse( buf );
    delete [] buf;

    if ( !Error() )
        return true;

    return false;
}


bool TiXmlDocument::LoadFile( const std::string& filename )
{
    // Delete the existing data:
    Clear();

    // Load the new data:
    FILE* fp = fopen( filename.c_str(), "r" );
    if ( fp )
    {
        return LoadFile(fp);
    }
    else
    {
        SetError( TIXML_ERROR_OPENING_FILE );
    }
    return false;
}


bool TiXmlDocument::SaveFile( const std::string& filename )
{
    FILE* fp = fopen( filename.c_str(), "w" );
    if ( fp )
    {
        Print( fp, 0 );
        fclose( fp );
        return true;
    }
    return false;
}


TiXmlNode* TiXmlDocument::Clone() const
{
    TiXmlDocument* clone = new TiXmlDocument();
    if ( !clone )
        return 0;

    CopyToClone( clone );
    clone->error = error;
    clone->errorDesc = errorDesc;

    TiXmlNode* node = 0;
    for ( node = firstChild; node; node = node->NextSibling() )
    {
        clone->LinkEndChild( node->Clone() );
    }
    return clone;
}


void TiXmlDocument::Print( FILE* fp, int )
{
    TiXmlNode* node;
    for ( node=FirstChild(); node; node=node->NextSibling() )
    {
        node->Print( fp, 0 );
        fprintf( fp, "\n" );
    }
}


TiXmlAttribute* TiXmlAttribute::Next()
{
    // We are using knowledge of the sentinel. The sentinel
    // have a value or name.
    if ( next->value.empty() && next->name.empty() )
        return 0;
    return next;
}


TiXmlAttribute* TiXmlAttribute::Previous()
{
    // We are using knowledge of the sentinel. The sentinel
    // have a value or name.
    if ( prev->value.empty() && prev->name.empty() )
        return 0;
    return prev;
}


void TiXmlAttribute::Print( FILE* fp, int )
{
    if ( value.find( '\"' ) != std::string::npos )
        fprintf( fp, "%s='%s'", name.c_str(), value.c_str() );
    else
        fprintf( fp, "%s=\"%s\"", name.c_str(), value.c_str() );
}


void TiXmlComment::Print( FILE* fp, int depth )
{
    for ( int i=0; i<depth; i++ )
        fprintf( fp, "    " );
    fprintf( fp, "<!--%s-->", value.c_str() );
}


TiXmlNode* TiXmlComment::Clone() const
{
    TiXmlComment* clone = new TiXmlComment();

    if ( !clone )
        return 0;

    CopyToClone( clone );
    return clone;
}


void TiXmlText::Print( FILE* fp, int )
{
    fprintf( fp, "%s", value.c_str() );
}


TiXmlNode* TiXmlText::Clone() const
{
    TiXmlText* clone = 0;
    clone = new TiXmlText();

    if ( !clone )
        return 0;

    CopyToClone( clone );
    return clone;
}


TiXmlDeclaration::TiXmlDeclaration( const std::string& _version, 
                                    const std::string& _encoding,
                                    const std::string& _standalone )
    : TiXmlNode( TiXmlNode::DECLARATION ) 
{
    version = _version;
    encoding = _encoding;
    standalone = _standalone;
}


void TiXmlDeclaration::Print( FILE* fp, int )
{
    std::string out = "<?xml ";

    if ( !version.empty() )
    {
        out += "version=\"";
        out += version;
        out += "\" ";
    }
    if ( !encoding.empty() )
    {
        out += "encoding=\"";
        out += encoding;
        out += "\" ";
    }
    if ( !standalone.empty() )
    {
        out += "standalone=\"";
        out += standalone;
        out += "\" ";
    }
    out += "?>";

    fprintf( fp, "%s", out.c_str() );
}


TiXmlNode* TiXmlDeclaration::Clone() const
{
    TiXmlDeclaration* clone = new TiXmlDeclaration();

    if ( !clone )
        return 0;

    CopyToClone( clone );
    clone->version = version;
    clone->encoding = encoding;
    clone->standalone = standalone;
    return clone;
}


void TiXmlUnknown::Print( FILE* fp, int depth )
{
    for ( int i=0; i<depth; i++ )
        fprintf( fp, "    " );
    fprintf( fp, "<%s>", value.c_str() );
}


TiXmlNode* TiXmlUnknown::Clone() const
{
    TiXmlUnknown* clone = new TiXmlUnknown();

    if ( !clone )
        return 0;

    CopyToClone( clone );
    return clone;
}


TiXmlAttributeSet::TiXmlAttributeSet()
{
    sentinel.next = &sentinel;
    sentinel.prev = &sentinel;
}


TiXmlAttributeSet::~TiXmlAttributeSet()
{
    assert( sentinel.next == &sentinel );
    assert( sentinel.prev == &sentinel );
}


void TiXmlAttributeSet::Add( TiXmlAttribute* addMe )
{
    assert( !Find( addMe->Name() ) );   // Shouldn't be multiply adding to the set.

    addMe->next = &sentinel;
    addMe->prev = sentinel.prev;

    sentinel.prev->next = addMe;
    sentinel.prev      = addMe;
}

void TiXmlAttributeSet::Remove( TiXmlAttribute* removeMe )
{
    TiXmlAttribute* node;

    for( node = sentinel.next; node != &sentinel; node = node->next )
    {
        if ( node == removeMe )
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->next = 0;
            node->prev = 0;
            return;
        }
    }
    assert( 0 );                // we tried to remove a non-linked attribute.
}


TiXmlAttribute* TiXmlAttributeSet::Find( const std::string& name ) const
{
    TiXmlAttribute* node;

    for( node = sentinel.next; node != &sentinel; node = node->next )
    {
        if ( node->Name() == name )
            return node;
    }
    return 0;
}
