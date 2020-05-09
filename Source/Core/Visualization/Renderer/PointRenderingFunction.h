/****************************************************************************/
/**
 *  @file   PointRenderingFunction.h
 *  @author Yukio Yasuhara, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PointRenderingFunction.h 1418 2013-02-21 07:02:46Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <kvs/PointObject>
#include <kvs/RGBColor>
#include <kvs/OpenGL>


namespace
{

#if defined( KVS_ENABLE_OPENGL )

/*
 * S: size,   Ss: size array
 * C: color,  Cs: color array
 * N: normal, Ns: normal array
 */

/*==========================================================================*/
/**
 *  Rendering points with S, C and Ns.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_S_C_Ns( const kvs::PointObject* point, const float dpr )
{
    kvs::OpenGL::Enable( GL_NORMALIZE );
    kvs::OpenGL::SetPointSize( point->size() * dpr );
    kvs::OpenGL::Color( point->color() );
    kvs::OpenGL::Begin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for ( size_t i = 0; i < nvertices; i++ )
        {
            kvs::OpenGL::Normal3( point->normals().data() + 3 * i );
            kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        }
    }
    kvs::OpenGL::End();
};

/*==========================================================================*/
/**
 *  Rendering points with S and C.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_S_C( const kvs::PointObject* point, const float dpr )
{
    kvs::OpenGL::SetPointSize( point->size() * dpr );
    kvs::OpenGL::Color( point->color() );
    kvs::OpenGL::Begin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for ( size_t i = 0; i < nvertices; i++ )
        {
            kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        }
    }
    kvs::OpenGL::End();
};

/*==========================================================================*/
/**
 *  Rendering points with S, Cs and Ns.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_S_Cs_Ns( const kvs::PointObject* point, const float dpr )
{
    kvs::OpenGL::Enable( GL_NORMALIZE );
    kvs::OpenGL::SetPointSize( point->size() * dpr );
    kvs::OpenGL::Begin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for( size_t i = 0; i < nvertices; i++ )
        {
            kvs::OpenGL::Normal3( point->normals().data() + 3 * i );
            kvs::OpenGL::Color3( point->colors().data() + 3 * i );
            kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        }
    }
    kvs::OpenGL::End();
};

/*==========================================================================*/
/**
 *  Rendering points with S and Cs.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_S_Cs( const kvs::PointObject* point, const float dpr )
{
    kvs::OpenGL::SetPointSize( point->size() * dpr );
    kvs::OpenGL::Begin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for ( size_t i = 0; i < nvertices; i++ )
        {
            kvs::OpenGL::Color3( point->colors().data() + 3 * i );
            kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        }
    }
    kvs::OpenGL::End();
};

/*==========================================================================*/
/**
 *  Rendering points with S.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_S( const kvs::PointObject* point, const float dpr )
{
    kvs::OpenGL::SetPointSize( point->size() * dpr );
    kvs::OpenGL::Begin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for ( size_t i = 0; i < nvertices; i++ )
        {
            kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        }
    }
    kvs::OpenGL::End();
};

/*==========================================================================*/
/**
 *  Rendering points with Ss, C and Ns.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Ss_C_Ns( const kvs::PointObject* point, const float dpr )
{
    kvs::OpenGL::Enable( GL_NORMALIZE );
    kvs::OpenGL::Color( point->color() );
    const size_t nvertices = point->numberOfVertices();
    for ( size_t i = 0; i < nvertices; i++ )
    {
        kvs::OpenGL::SetPointSize( point->size(i) * dpr );
        kvs::OpenGL::Begin( GL_POINTS );
        kvs::OpenGL::Normal3( point->normals().data() + 3 * i );
        kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        kvs::OpenGL::End();
    }
};

/*==========================================================================*/
/**
 *  Rendering points with Ss and C.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Ss_C( const kvs::PointObject* point, const float dpr )
{
    kvs::OpenGL::Color( point->color() );
    const size_t nvertices = point->numberOfVertices();
    for ( size_t i = 0; i < nvertices; i++ )
    {
        kvs::OpenGL::SetPointSize( point->size(i) * dpr );
        kvs::OpenGL::Begin( GL_POINTS );
        kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        kvs::OpenGL::End();
    }
};

/*==========================================================================*/
/**
 *  Rendering points with Ssm Cs and Ns.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Ss_Cs_Ns( const kvs::PointObject* point, const float dpr )
{
    kvs::OpenGL::Enable( GL_NORMALIZE );
    const size_t nvertices = point->numberOfVertices();
    for ( size_t i = 0; i < nvertices; i++ )
    {
        kvs::OpenGL::SetPointSize( point->size(i) * dpr );
        kvs::OpenGL::Begin( GL_POINTS );
        kvs::OpenGL::Normal3( point->normals().data() + 3 * i );
        kvs::OpenGL::Color3( point->colors().data() + 3 * i );
        kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        kvs::OpenGL::End();
    }
};

/*==========================================================================*/
/**
 *  Rendering points with Ss and Cs.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Ss_Cs( const kvs::PointObject* point, const float dpr )
{
    const size_t nvertices = point->numberOfVertices();
    for ( size_t i = 0; i < nvertices; i++ )
    {
        kvs::OpenGL::SetPointSize( point->size(i) * dpr );
        kvs::OpenGL::Begin( GL_POINTS );
        kvs::OpenGL::Color3( point->colors().data() + 3 * i );
        kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        kvs::OpenGL::End();
    }
};

/*==========================================================================*/
/**
 *  Rendering points with Ss.
 *  @param point [in] pointer to the point object
 *  @param dpr [in] device pixel ratio
 */
/*==========================================================================*/
void Rendering_Ss( const kvs::PointObject* point, const float dpr )
{
    const size_t nvertices = point->numberOfVertices();
    for ( size_t i = 0; i < nvertices; i++ )
    {
        kvs::OpenGL::SetPointSize( point->size(i) * dpr );
        kvs::OpenGL::Begin( GL_POINTS );
        kvs::OpenGL::Vertex3( point->coords().data() + 3 * i );
        kvs::OpenGL::End();
    }
};

typedef void (*PointRenderingFunctionType)( const kvs::PointObject* point, const float dpr );

enum PointRenderingType
{
    Type_S_C_Ns = 0,
    Type_S_C,
    Type_S_Cs_Ns,
    Type_S_Cs,
    Type_S,
    Type_Ss_C_Ns,
    Type_Ss_C,
    Type_Ss_Cs_Ns,
    Type_Ss_Cs,
    Type_Ss,
    NumberOfRenderingTypes
};

PointRenderingFunctionType Rendering[NumberOfRenderingTypes] =
{
    &Rendering_S_C_Ns,
    &Rendering_S_C,
    &Rendering_S_Cs_Ns,
    &Rendering_S_Cs,
    &Rendering_S,
    &Rendering_Ss_C_Ns,
    &Rendering_Ss_C,
    &Rendering_Ss_Cs_Ns,
    &Rendering_Ss_Cs,
    &Rendering_Ss,
};

PointRenderingType GetPointRenderingType( const kvs::PointObject* point )
{
    const size_t nsizes = point->numberOfSizes();
    const size_t ncolors = point->numberOfColors();
    const size_t nnormals = point->numberOfNormals();
    if ( nsizes == 1 )
    {
        if ( ncolors == 1 )
            return ( nnormals > 0 ) ? Type_S_C_Ns : Type_S_C;
        else if ( ncolors > 1 )
            return ( nnormals > 0 ) ? Type_S_Cs_Ns : Type_S_Cs;
        else
            return Type_S;
    }
    else
    {
        if ( ncolors == 1 )
            return ( nnormals > 0 ) ? Type_Ss_C_Ns : Type_Ss_C;
        else if ( ncolors > 1 )
            return ( nnormals > 0 ) ? Type_Ss_Cs_Ns : Type_Ss_Cs;
        else
            return Type_Ss;
    }
};

void PointRenderingFunction( const kvs::PointObject* point, const float dpr = 1.0f )
{
    if( point->numberOfVertices() > 0 )
    {
        PointRenderingType type = GetPointRenderingType( point );
        Rendering[type]( point, dpr );
    }
};

#else

void PointRenderingFunction( const kvs::PointObject* )
{
};

#endif // KVS_ENABLE_OPENGL

} // end of namespace
