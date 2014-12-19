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
#ifndef KVS__POINT_RENDERING_FUNCTION_H_INCLUDE
#define KVS__POINT_RENDERING_FUNCTION_H_INCLUDE

#include <kvs/PointObject>
#include <kvs/RGBColor>
#include <kvs/OpenGL>


namespace
{

/*
 * S: size,   Ss: size array
 * C: color,  Cs: color array
 * N: normal, Ns: normal array
 */

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_S_C_Ns( const kvs::PointObject* point )
{
    glEnable( GL_NORMALIZE );

    const float         size  = point->size();
    const kvs::RGBColor color = point->color();

    glPointSize( size );
    glColor3ub( color.r(), color.g(), color.b() );

    const float* vertex = point->coords().data();
    const float* normal = point->normals().data();

    glBegin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for( size_t i = 0; i < nvertices; i++ )
        {
            const size_t i3 = i * 3;
            glNormal3f( normal[i3], normal[i3+1], normal[i3+2] );
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
    }
    glEnd();
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_S_C( const kvs::PointObject* point )
{
    const float         size  = point->size();
    const kvs::RGBColor color = point->color();

    glPointSize( size );
    glColor3ub( color.r(), color.g(), color.b() );

    const float* vertex = point->coords().data();
    glBegin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for( size_t i = 0; i < nvertices; i++ )
        {
            size_t i3 = i * 3;
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
    }
    glEnd();
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_S_Cs_Ns( const kvs::PointObject* point )
{
    glEnable( GL_NORMALIZE );

    const float size = point->size();
    glPointSize( size );

    const float*         vertex = point->coords().data();
    const float*         normal = point->normals().data();
    const unsigned char* color  = point->colors().data();
    glBegin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for( size_t i = 0; i < nvertices; i++ )
        {
            size_t i3 = i * 3;
            glNormal3f( normal[i3], normal[i3+1], normal[i3+2] );
            glColor3ub( color[i3],  color[i3+1],  color[i3+2]  );
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
    }
    glEnd();
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_S_Cs( const kvs::PointObject* point )
{
    const float size = point->size();
    glPointSize( size );

    const float*         vertex = point->coords().data();
    const unsigned char* color = point->colors().data();
    glBegin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for( size_t i = 0; i < nvertices; i++ )
        {
            const size_t i3 = i * 3;
            glColor3ub( color[i3],  color[i3+1],  color[i3+2]  );
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
    }
    glEnd();
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_S( const kvs::PointObject* point )
{
    const float size = point->size();
    glPointSize( size );

    const float* vertex = point->coords().data();
    glBegin( GL_POINTS );
    {
        const size_t nvertices = point->numberOfVertices();
        for( size_t i = 0; i < nvertices; i++ )
        {
            const size_t i3 = i * 3;
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
    }
    glEnd();
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_Ss_C_Ns( const kvs::PointObject* point )
{
    glEnable( GL_NORMALIZE );

    const kvs::RGBColor color = point->color();
    glColor3ub( color.r(), color.g(), color.b() );

    const float* size   = point->sizes().data();
    const float* vertex = point->coords().data();
    const float* normal = point->normals().data();

    const size_t nvertices = point->numberOfVertices();
    for( size_t i = 0; i < nvertices; i++ )
    {
        const size_t i3 = i * 3;
        glPointSize( size[i] );
        glBegin( GL_POINTS );
        {
            glNormal3f( normal[i3], normal[i3+1], normal[i3+2] );
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
        glEnd();
    }
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_Ss_C( const kvs::PointObject* point )
{
    const kvs::RGBColor color = point->color();
    glColor3ub( color.r(), color.g(), color.b() );

    const float* size   = point->sizes().data();
    const float* vertex = point->coords().data();

    const size_t nvertices = point->numberOfVertices();
    for( size_t i = 0; i < nvertices; i++ )
    {
        const size_t i3 = i * 3;
        glPointSize( size[i] );
        glBegin( GL_POINTS );
        {
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
        glEnd();
    }
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_Ss_Cs_Ns( const kvs::PointObject* point )
{
    glEnable( GL_NORMALIZE );

    const float*         size   = point->sizes().data();
    const float*         vertex = point->coords().data();
    const float*         normal = point->normals().data();
    const unsigned char* color  = point->colors().data();

    const size_t nvertices = point->numberOfVertices();
    for( size_t i = 0; i < nvertices; i++ )
    {
        const size_t i3 = i * 3;
        glPointSize( size[i] );
        glBegin( GL_POINTS );
        {
            glNormal3f( normal[i3], normal[i3+1], normal[i3+2] );
            glColor3ub( color[i3],  color[i3+1],  color[i3+2]  );
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
        glEnd();
    }
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_Ss_Cs( const kvs::PointObject* point )
{
    const float*         size   = point->sizes().data();
    const float*         vertex = point->coords().data();
    const unsigned char* color  = point->colors().data();

    const size_t nvertices = point->numberOfVertices();
    for( size_t i = 0; i < nvertices; i++ )
    {
        const size_t i3 = i * 3;
        glPointSize( size[i] );
        glBegin( GL_POINTS );
        {
            glColor3ub( color[i3],  color[i3+1],  color[i3+2]  );
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
        glEnd();
    }
};

/*==========================================================================*/
/**
 *  
 *  @param point [in] pointer to the point object
 */
/*==========================================================================*/
void Rendering_Ss( const kvs::PointObject* point )
{
    const float* size   = point->sizes().data();
    const float* vertex = point->coords().data();

    const size_t nvertices = point->numberOfVertices();
    for( size_t i = 0; i < nvertices; i++ )
    {
        size_t i3 = i * 3;
        glPointSize( size[i] );
        glBegin( GL_POINTS );
        {
            glVertex3f( vertex[i3], vertex[i3+1], vertex[i3+2] );
        }
        glEnd();
    }
};

typedef void (*PointRenderingFunctionType)( const kvs::PointObject* point );

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
    const size_t nsizes    = point->numberOfSizes();
    const size_t ncolors   = point->numberOfColors();
    const size_t nnormals  = point->numberOfNormals();

    if( nsizes == 1 )
    {
        if( ncolors == 1 )
            return( ( nnormals > 0 ) ? Type_S_C_Ns : Type_S_C );
        else if( ncolors > 1 )
            return( ( nnormals > 0 ) ? Type_S_Cs_Ns : Type_S_Cs );
        else
            return( Type_S );
    }
    else
    {
        if( ncolors == 1 )
            return( ( nnormals > 0 ) ? Type_Ss_C_Ns : Type_Ss_C );
        else if( ncolors > 1 )
            return( ( nnormals > 0 ) ? Type_Ss_Cs_Ns : Type_Ss_Cs );
        else
            return( Type_Ss );
    }
};

void PointRenderingFunction( const kvs::PointObject* point )
{
    if( point->numberOfVertices() > 0 )
    {
        PointRenderingType type = GetPointRenderingType( point );
        Rendering[type]( point );
    }
};

} // end of namespace

#endif // KVS__POINT_RENDERING_FUNCTION_H_INCLUDE
