/****************************************************************************/
/**
 *  @file   LineRenderingFunction.h
 *  @author Yukio Yasuhara, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineRenderingFunction.h 1418 2013-02-21 07:02:46Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <kvs/LineObject>
#include <kvs/RGBColor>
#include <kvs/Vector3>
#include <kvs/OpenGL>


namespace
{

#if defined( KVS_ENABLE_OPENGL )

/*
 * VC: vertex color, VCs: vertex color array
 * LC: line color,   LCs: line color array
 * S: size,   Ss: size array
 */

/*==========================================================================*/
/**
 *  Rendering strip line with VCs and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Strip_VCs_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    kvs::OpenGL::Begin( GL_LINE_STRIP );
    {
        const size_t nvertices = line->numberOfVertices();
        for ( size_t i = 0; i < nvertices; i++ )
        {
            kvs::OpenGL::Color( line->color(i) );
            kvs::OpenGL::Vertex( line->coord(i) );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering strip ine with VCs and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Strip_VCs_Ss( const kvs::LineObject* line, const float dpr )
{
    const size_t nlines = line->numberOfVertices() - 1;
    for ( size_t i = 0; i < nlines; i++ )
    {
        kvs::OpenGL::SetLineWidth( line->size(i) * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( line->color(i) );
        kvs::OpenGL::Vertex( line->coord(i) );
        kvs::OpenGL::Color( line->color(i+1) );
        kvs::OpenGL::Vertex( line->coord(i+1) );
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering strip line with LC and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Strip_LC_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    kvs::OpenGL::Begin( GL_LINE_STRIP );
    kvs::OpenGL::Color( line->color(0) );
    const size_t nvertices = line->numberOfVertices();
    for ( size_t i = 0; i < nvertices; i++ )
    {
        kvs::OpenGL::Vertex( line->coord(i) );
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering strip line with LCs and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Strip_LCs_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size( 0 ) * dpr );
    kvs::OpenGL::Begin( GL_LINES );
    const size_t num = line->numberOfVertices() - 1;
    for ( size_t i = 0; i < num; i++ )
    {
        kvs::OpenGL::Color( line->color(i) );
        kvs::OpenGL::Vertex( line->coord(i) );
        kvs::OpenGL::Vertex( line->coord(i+1) );
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering strip line with LC and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Strip_LC_Ss( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::Color( line->color(0) );
    const size_t num = line->numberOfVertices() - 1;
    for ( size_t i = 0; i < num; i++ )
    {
        kvs::OpenGL::SetLineWidth( line->size(i) * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Vertex( line->coord(i) );
        kvs::OpenGL::Vertex( line->coord(i+1) );
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering strip line with LCs and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Strip_LCs_Ss( const kvs::LineObject* line, const float dpr )
{
    const size_t num = line->numberOfVertices() - 1;
    for ( size_t i = 0; i < num; i++ )
    {
        kvs::OpenGL::SetLineWidth( line->size(i) * dpr );
        kvs::OpenGL::Color( line->color(i) );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Vertex( line->coord(i) );
        kvs::OpenGL::Vertex( line->coord(i+1) );
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering uniline with VCs and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Uniline_VCs_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    kvs::OpenGL::Begin( GL_LINE_STRIP );
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        const size_t id = *( line->connections().data() + i );
        kvs::OpenGL::Color( line->color(id) );
        kvs::OpenGL::Vertex( line->coord(id) );
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering uniline with VCs and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Uniline_VCs_Ss( const kvs::LineObject* line, const float dpr )
{
    const size_t num = line->numberOfConnections() - 1;
    for ( size_t i = 0; i < num; i++ )
    {
        const size_t id1 = *( line->connections().data() + i   );
        const size_t id2 = *( line->connections().data() + i+1 );
        kvs::OpenGL::SetLineWidth( line->size(i) * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( line->color(id1) );
        kvs::OpenGL::Vertex( line->coord(id1) );
        kvs::OpenGL::Color( line->color(id2) );
        kvs::OpenGL::Vertex( line->coord(id2) );
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering uniline with LC and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Uniline_LC_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    kvs::OpenGL::Begin( GL_LINE_STRIP );
    kvs::OpenGL::Color( line->color(0) );
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        kvs::OpenGL::Vertex( line->coord(i) );
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering uniline with LCs and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Uniline_LCs_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    kvs::OpenGL::Begin( GL_LINES );
    const size_t num = line->numberOfConnections() - 1;
    for ( size_t i = 0; i < num; i++ )
    {
        kvs::OpenGL::Color( line->color(i) );
        kvs::OpenGL::Vertex( line->coord(i) );
        kvs::OpenGL::Vertex( line->coord(i+1) );
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering uniline with LC and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Uniline_LC_Ss( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::Color( line->color(0) );
    const size_t num = line->numberOfConnections() - 1;
    for ( size_t i = 0; i < num; i++ )
    {
        kvs::OpenGL::SetLineWidth( line->size(i) * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Vertex( line->coord(i) );
        kvs::OpenGL::Vertex( line->coord(i+1) );
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering uniline with LCs and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Uniline_LCs_Ss( const kvs::LineObject* line, const float dpr )
{
    const size_t num = line->numberOfConnections() - 1;
    for ( size_t i = 0; i < num; i++ )
    {
        kvs::OpenGL::SetLineWidth( line->size(i) * dpr );
        kvs::OpenGL::Color( line->color(i) );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Vertex( line->coord(i) );
        kvs::OpenGL::Vertex( line->coord(i+1) );
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering polyline with VCs and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Polyline_VCs_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index );
        const size_t id2 = *( line->connections().data() + index + 1 );
        for ( size_t j = id1; j <= id2; j++ )
        {
            kvs::OpenGL::Color( line->color(j) );
            kvs::OpenGL::Vertex( line->coord(j) );
        }
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering polyline with VCs and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Polyline_VCs_Ss( const kvs::LineObject* line, const float dpr )
{
    int ctr = 0;
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index   );
        const size_t id2 = *( line->connections().data() + index + 1 );
        for ( size_t j = id1; j < id2; j++ )
        {
            kvs::OpenGL::SetLineWidth( line->size(ctr) * dpr );
            kvs::OpenGL::Begin( GL_LINES );
            kvs::OpenGL::Color( line->color(j) );
            kvs::OpenGL::Vertex( line->coord(j) );
            kvs::OpenGL::Color( line->color(j+1) );
            kvs::OpenGL::Vertex( line->coord(j+1) );
            kvs::OpenGL::End();
            ctr++;
        }
    }
}

/*==========================================================================*/
/**
 *  Rendering polyline with LC and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Polyline_LC_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        kvs::OpenGL::Color( line->color(0) );
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index );
        const size_t id2 = *( line->connections().data() + index + 1 );
        for ( size_t j = id1; j < id2; j++ )
        {
            kvs::OpenGL::Vertex( line->coord(j) );
            kvs::OpenGL::Vertex( line->coord(j+1) );
        }
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering polyline with LCs and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Polyline_LCs_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    int ctr = 0;
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index );
        const size_t id2 = *( line->connections().data() + index + 1 );
        for ( size_t j = id1; j < id2; j++ )
        {
            kvs::OpenGL::Color( line->color(ctr) );
            kvs::OpenGL::Vertex( line->coord(j) );
            kvs::OpenGL::Vertex( line->coord(j+1) );
            ctr++;
        }
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering polyline with LC and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Polyline_LC_Ss( const kvs::LineObject* line, const float dpr )
{
    int ctr = 0;
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        kvs::OpenGL::Color( line->color(0) );
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index   );
        const size_t id2 = *( line->connections().data() + index + 1 );
        for ( size_t j = id1; j < id2; j++ )
        {
            kvs::OpenGL::SetLineWidth( line->size(ctr) * dpr );
            kvs::OpenGL::Begin( GL_LINES );
            kvs::OpenGL::Vertex( line->coord(j) );
            kvs::OpenGL::Vertex( line->coord(j+1) );
            kvs::OpenGL::End();
            ctr++;
        }
    }
}

/*==========================================================================*/
/**
 *  Rendering polyline with LCs and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Polyline_LCs_Ss( const kvs::LineObject* line, const float dpr )
{
    int ctr = 0;
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index   );
        const size_t id2 = *( line->connections().data() + index+1 );
        for ( size_t j = id1; j < id2; j++ )
        {
            kvs::OpenGL::SetLineWidth( line->size(ctr) * dpr );
            kvs::OpenGL::Begin( GL_LINES );
            kvs::OpenGL::Color( line->color(ctr) );
            kvs::OpenGL::Vertex( line->coord(j) );
            kvs::OpenGL::Vertex( line->coord(j+1) );
            kvs::OpenGL::End();
            ctr++;
        }
    }
}

/*==========================================================================*/
/**
 *  Rendering segment line VCs and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Segment_VCs_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    kvs::OpenGL::Begin( GL_LINES );
    const size_t num = line->numberOfConnections() * 2;
    for ( size_t i = 0; i < num; i++ )
    {
        const size_t id = *( line->connections().data() + i );
        kvs::OpenGL::Color( line->color(id) );
        kvs::OpenGL::Vertex( line->coord(id) );
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering segment line VCs and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Segment_VCs_Ss( const kvs::LineObject* line, const float dpr )
{
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index   );
        const size_t id2 = *( line->connections().data() + index + 1 );
        kvs::OpenGL::SetLineWidth( line->size(i) * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( line->color(id1) );
        kvs::OpenGL::Vertex( line->coord(id1) );
        kvs::OpenGL::Color( line->color(id2) );
        kvs::OpenGL::Vertex( line->coord(id2) );
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering segment line LC and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Segment_LC_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    kvs::OpenGL::Color( line->color(0) );
    kvs::OpenGL::Begin( GL_LINES );
    const size_t num = line->numberOfConnections() * 2;
    for ( size_t i = 0; i < num; i++ )
    {
        const size_t id = *( line->connections().data() + i );
        kvs::OpenGL::Vertex( line->coord(id) );
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering segment line LCs and S.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Segment_LCs_S( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::SetLineWidth( line->size(0) * dpr );
    kvs::OpenGL::Begin( GL_LINES );
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index   );
        const size_t id2 = *( line->connections().data() + index + 1 );
        kvs::OpenGL::Color( line->color(i) );
        kvs::OpenGL::Vertex( line->coord(id1) );
        kvs::OpenGL::Vertex( line->coord(id2) );
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  Rendering segment line with LC and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Segment_LC_Ss( const kvs::LineObject* line, const float dpr )
{
    kvs::OpenGL::Color( line->color(0) );
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index );
        const size_t id2 = *( line->connections().data() + index + 1 );
        kvs::OpenGL::SetLineWidth( line->size( i ) * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Vertex( line->coord( id1 ) );
        kvs::OpenGL::Vertex( line->coord( id2 ) );
        kvs::OpenGL::End();
    }
}

/*==========================================================================*/
/**
 *  Rendering segment line with LCs and Ss.
 *  @param line [in] pointer to the line object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Segment_LCs_Ss( const kvs::LineObject* line, const float dpr )
{
    const size_t nconnections = line->numberOfConnections();
    for ( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index );
        const size_t id2 = *( line->connections().data() + index + 1 );
        kvs::OpenGL::Color( line->color( i ) );
        kvs::OpenGL::SetLineWidth( line->size( i ) * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Vertex( line->coord( id1 ) );
        kvs::OpenGL::Vertex( line->coord( id2 ) );
        kvs::OpenGL::End();
    }
}

typedef void (*LineRenderingFunctionType)( const kvs::LineObject* line, const float dpr );

enum LineRenderingType
{
    Type_Strip_VCs_S       = 0,
    Type_Strip_VCs_Ss      = 1,
    Type_Strip_LC_S        = 2,
    Type_Strip_LCs_S       = 3,
    Type_Strip_LC_Ss       = 4,
    Type_Strip_LCs_Ss      = 5,

    Type_Uniline_VCs_S     = 6,
    Type_Uniline_VCs_Ss    = 7,
    Type_Uniline_LC_S      = 8,
    Type_Uniline_LCs_S     = 9,
    Type_Uniline_LC_Ss     = 10,
    Type_Uniline_LCs_Ss    = 11,

    Type_Polyline_VCs_S    = 12,
    Type_Polyline_VCs_Ss   = 13,
    Type_Polyline_LC_S     = 14,
    Type_Polyline_LCs_S    = 15,
    Type_Polyline_LC_Ss    = 16,
    Type_Polyline_LCs_Ss   = 17,

    Type_Segment_VCs_S     = 18,
    Type_Segment_VCs_Ss    = 19,
    Type_Segment_LC_S      = 20,
    Type_Segment_LCs_S     = 21,
    Type_Segment_LC_Ss     = 22,
    Type_Segment_LCs_Ss    = 23,

    NumberOfRenderingTypes = 24
};

LineRenderingFunctionType Rendering[NumberOfRenderingTypes] =
{
    &Rendering_Strip_VCs_S,
    &Rendering_Strip_VCs_Ss,
    &Rendering_Strip_LC_S,
    &Rendering_Strip_LCs_S,
    &Rendering_Strip_LC_Ss,
    &Rendering_Strip_LCs_Ss,

    &Rendering_Uniline_VCs_S,
    &Rendering_Uniline_VCs_Ss,
    &Rendering_Uniline_LC_S,
    &Rendering_Uniline_LCs_S,
    &Rendering_Uniline_LC_Ss,
    &Rendering_Uniline_LCs_Ss,

    &Rendering_Polyline_VCs_S,
    &Rendering_Polyline_VCs_Ss,
    &Rendering_Polyline_LC_S,
    &Rendering_Polyline_LCs_S,
    &Rendering_Polyline_LC_Ss,
    &Rendering_Polyline_LCs_Ss,

    &Rendering_Segment_VCs_S,
    &Rendering_Segment_VCs_Ss,
    &Rendering_Segment_LC_S,
    &Rendering_Segment_LCs_S,
    &Rendering_Segment_LC_Ss,
    &Rendering_Segment_LCs_Ss,
};

#define RETURN_RENDERING_TYPE(type) \
    switch( line->colorType() ) \
    { \
    case kvs::LineObject::VertexColor: \
    { \
        return( ( nsizes == 1 ) ? \
                Type_ ## type ## _VCs_S : \
                Type_ ## type ## _VCs_Ss ); \
    } \
    default: \
    { \
        if( nsizes == 1 ) \
            return( ( ncolors == 1 ) ? \
                    Type_ ## type ## _LC_S : \
                    Type_ ## type ## _LCs_S ); \
        else \
            return( ( ncolors == 1 ) ? \
                    Type_ ## type ## _LC_Ss : \
                    Type_ ## type ## _LCs_Ss ); \
    } \
    }

LineRenderingType GetLineRenderingType( const kvs::LineObject* line )
{
    const size_t nsizes = line->numberOfSizes();
    const size_t ncolors = line->numberOfColors();
    switch( line->lineType() )
    {
    case kvs::LineObject::Strip:    RETURN_RENDERING_TYPE( Strip );
    case kvs::LineObject::Uniline:  RETURN_RENDERING_TYPE( Uniline );
    case kvs::LineObject::Polyline: RETURN_RENDERING_TYPE( Polyline );
    case kvs::LineObject::Segment:  RETURN_RENDERING_TYPE( Segment );
    default: break;
    }

    return( Type_Strip_VCs_S );
};

void LineRenderingFunction( const kvs::LineObject* line, const float dpr = 1.0f )
{
    if ( line->numberOfVertices() > 0 )
    {
        LineRenderingType type = GetLineRenderingType( line );
        Rendering[type]( line, dpr );
    }
};

#else

void LineRenderingFunction( const kvs::LineObject* )
{
}

#endif // KVS_ENABLE_OPENGL

} // end of namespace
