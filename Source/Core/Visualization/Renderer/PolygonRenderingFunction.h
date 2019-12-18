/****************************************************************************/
/**
 *  @file   PolygonRenderingFunction.h
 *  @author Yukio Yasuhara, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonRenderingFunction.h 1418 2013-02-21 07:02:46Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <kvs/PolygonObject>
#include <kvs/RGBColor>
#include <kvs/Type>
#include <kvs/OpenGL>

/* Function name: Rendering_xxx
 *
 * polygon type
 *   Tri: triangle
 *   Quad: quadrangle
 *
 * color type
 *   VCs: vertex color array
 *   PCs: polygon color array
 *   SC: vertex or polygon color (single color)
 *
 * opacity
 *   Os: opacity array
 *   O: opacity (single value)
 *
 * connection
 *   Cs: connection array
 *
 * normal type
 *   VN: vertex normal
 *   PN: polygon normal
 */

namespace
{

#if defined( KVS_ENABLE_OPENGL )

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with VCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_VCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity();
        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with VCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_VCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity();
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t id0 = *( connections + i3 ) * 3;
            const size_t id1 = *( connections + i3 + 1 ) * 3;
            const size_t id2 = *( connections + i3 + 2 ) * 3;

            kvs::OpenGL::Color( polygon->colors().data() + id0, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id0 );

            kvs::OpenGL::Color( polygon->colors().data() + id1, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id1 );

            kvs::OpenGL::Color( polygon->colors().data() + id2, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id2 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with VCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_VCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, polygon->opacity(i) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with VCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_VCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t id0 = *( connections + i3 ) * 3;
            const size_t id1 = *( connections + i3 + 1 ) * 3;
            const size_t id2 = *( connections + i3 + 2 ) * 3;
            const kvs::UInt8 opacity0 = polygon->opacity( id0 / 3 );
            const kvs::UInt8 opacity1 = polygon->opacity( id1 / 3 );
            const kvs::UInt8 opacity2 = polygon->opacity( id2 / 3 );

            kvs::OpenGL::Color( polygon->colors().data() + id0, opacity0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id0 );

            kvs::OpenGL::Color( polygon->colors().data() + id1, opacity1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id1 );

            kvs::OpenGL::Color( polygon->colors().data() + id2, opacity2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id2 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with SC and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_SC_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with SC, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_SC_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t id0 = *( connections + i3 ) * 3;
            const size_t id1 = *( connections + i3 + 1 ) * 3;
            const size_t id2 = *( connections + i3 + 2 ) * 3;
            kvs::OpenGL::Vertex3( polygon->coords().data() + id0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id2 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with SC and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_SC_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const size_t nopacities = polygon->numberOfOpacities();
        for ( size_t i = 0, i9 = 0; i < nopacities; i++, i9 += 9 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with SC, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_SC_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

            const size_t id0 = *( connections + i3 ) * 3;
            const size_t id1 = *( connections + i3 + 1 ) * 3;
            const size_t id2 = *( connections + i3 + 2 ) * 3;
            kvs::OpenGL::Vertex3( polygon->coords().data() + id0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id2 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with PCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_PCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < ncolors; i++, i3 += 3, i9 += 9 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with PCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_PCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            const size_t id0 = *( connections + i3 ) * 3;
            const size_t id1 = *( connections + i3 + 1 ) * 3;
            const size_t id2 = *( connections + i3 + 2 ) * 3;
            kvs::OpenGL::Vertex3( polygon->coords().data() + id0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id2 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with PCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_PCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < ncolors; i++, i3 += 3, i9 += 9 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for triangles with PCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Tri_PCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            const size_t id0 = *( connections + i3 ) * 3;
            const size_t id1 = *( connections + i3 + 1 ) * 3;
            const size_t id2 = *( connections + i3 + 2 ) * 3;
            kvs::OpenGL::Vertex3( polygon->coords().data() + id0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + id2 );
        }
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Rendering function for quadragles with VCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void Rendering_Quad_VCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Color( polygon->colors().data(), opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadragles with VCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i4 = 0; i < nconnections; i++, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Color( polygon->colors().data() + con3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadragles with VCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, polygon->opacity(i) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadragles with VCs, Os, and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i4 = 0; i < nconnections; i++, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            const kvs::UInt8 opacity0 = polygon->opacity( con0 / 3 );
            const kvs::UInt8 opacity1 = polygon->opacity( con1 / 3 );
            const kvs::UInt8 opacity2 = polygon->opacity( con2 / 3 );
            const kvs::UInt8 opacity3 = polygon->opacity( con3 / 3 );

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Color( polygon->colors().data() + con3, opacity3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @breif  Rendering function for quadragles with SC and O
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_SC_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadragles with SC, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_SC_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i4 = 0; i < nconnections; i++, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadragles with SC and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_SC_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const size_t nopacities = polygon->numberOfOpacities();
        for ( size_t i = 0, i12 = 0; i < nopacities; i++, i12 += 12 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with SC, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_SC_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i4 = 0; i < nconnections; i++, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PCs and O
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const size_t ncolors = polygon->numberOfColors();
        const kvs::UInt8 opacity = polygon->opacity(0);
        for ( size_t i = 0, i3 = 0, i12 = 0; i < ncolors; i++, i3 += 3, i12 += 12 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i12 = 0; i < ncolors; i++, i3 += 3, i12 += 12 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            const kvs::UInt8 opacity = polygon->opacity(i);

            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, VCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_VCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, VCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_VCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, VCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_VCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, VCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_VCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;
            const kvs::UInt8 opacity0 = polygon->opacity( con0 / 3 );
            const kvs::UInt8 opacity1 = polygon->opacity( con1 / 3 );
            const kvs::UInt8 opacity2 = polygon->opacity( con2 / 3 );

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity0 );
            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity1 );
            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity2 );
            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, SC and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_SC_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, SC, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_SC_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;

            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, SC and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_SC_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const size_t nopacities = polygon->numberOfOpacities();
        for ( size_t i = 0, i9 = 0; i < nopacities; i++, i9 += 9 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, SC, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_SC_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;

            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, PCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_PCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < ncolors; i++, i3 += 3, i9 += 9 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, PCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_PCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;

            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, PCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_PCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < ncolors; i++, i3 += 3, i9 += 9 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i9 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with VN, PCs, Os, and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_VN_PCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;
            const kvs::UInt8 opacity = polygon->opacity(i);

            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, VCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_VCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, VCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_VCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i4 = 0; i < nconnections; i++, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Color( polygon->colors().data() + con3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + con3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, VCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_VCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, polygon->opacity(i) );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, VCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_VCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i4 = 0; i < nconnections; i++, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            const kvs::UInt8 opacity0 = polygon->opacity( con0 / 3 );
            const kvs::UInt8 opacity1 = polygon->opacity( con1 / 3 );
            const kvs::UInt8 opacity2 = polygon->opacity( con2 / 3 );
            const kvs::UInt8 opacity3 = polygon->opacity( con3 / 3 );

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity0 );
            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity1 );
            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity2 );
            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Color( polygon->colors().data() + con3, opacity3 );
            kvs::OpenGL::Normal3( polygon->normals().data() + con3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, SC, and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_SC_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const size_t nvertices = polygon->numberOfVertices();
        for ( size_t i = 0, i3 = 0; i < nvertices; i++, i3 += 3 )
        {
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, SC, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_SC_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i4 = 0; i < nconnections; i++, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, SC and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_SC_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const size_t nopacities = polygon->numberOfOpacities();
        for ( size_t i = 0, i12 = 0; i < nopacities; i++, i12 += 12 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, SC, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_SC_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for( size_t i = 0, i4 = 0; i < nconnections; i++, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, PCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_PCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i12 = 0; i < ncolors; i++, i3 += 3, i12 += 12 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, PCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_PCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, PCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_PCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i12 = 0; i < ncolors; i++, i3 += 3, i12 += 12 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i12 + 9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with VN, PCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_VN_PCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );

            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            kvs::OpenGL::Normal3( polygon->normals().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Normal3( polygon->normals().data() + con3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, VCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_VCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t nnormals = polygon->numberOfNormals();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < nnormals; i++, i3 += 3, i9 += 9 )
        {
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Color( polygon->colors().data() + i9, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );

            kvs::OpenGL::Color( polygon->colors().data() + i9 + 3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );

            kvs::OpenGL::Color( polygon->colors().data() + i9 + 6, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, VCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_VCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;

            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, VCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_VCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const size_t nnormals = polygon->numberOfNormals();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < nnormals; i++, i3 += 3, i9 += 9 )
        {
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Color( polygon->colors().data() + i9, polygon->opacity( i3 ) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );

            kvs::OpenGL::Color( polygon->colors().data() + i9 + 3, polygon->opacity( i3 + 1 ) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );

            kvs::OpenGL::Color( polygon->colors().data() + i9 + 6, polygon->opacity( i3 + 2 ) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, VCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_VCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;
            const kvs::UInt8 opacity0 = polygon->opacity( con0 / 3 );
            const kvs::UInt8 opacity1 = polygon->opacity( con1 / 3 );
            const kvs::UInt8 opacity2 = polygon->opacity( con2 / 3 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, SC and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_SC_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const size_t nnormals = polygon->numberOfNormals();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < nnormals; i++, i3 += 3, i9 += 9 )
        {
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, SC, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_SC_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;

            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, SC and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_SC_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const size_t nopacities = polygon->numberOfOpacities();
        for( size_t i = 0, i3 = 0, i9 = 0; i < nopacities; i++, i3 += 0, i9 += 9 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, SC, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_SC_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, PCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_PCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < ncolors; i++, i3 += 3, i9 += 9 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, PCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_PCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, PCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_PCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i9 = 0; i < ncolors; i++, i3 += 3, i9 += 9 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i9 + 6 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for triangles with PN, PCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Tri_PN_PCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_TRIANGLES );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0; i < nconnections; i++, i3 += 3 )
        {
            const size_t con0 = *( connections + i3 ) * 3;
            const size_t con1 = *( connections + i3 + 1 ) * 3;
            const size_t con2 = *( connections + i3 + 2 ) * 3;
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, VCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_VCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t nnormals = polygon->numberOfNormals();
        for( size_t i = 0, i3 = 0, i12 = 0; i < nnormals; i++, i3 += 3, i12 += 12 )
        {
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Color( polygon->colors().data() + i12, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );

            kvs::OpenGL::Color( polygon->colors().data() + i12 + 3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );

            kvs::OpenGL::Color( polygon->colors().data() + i12 + 6, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );

            kvs::OpenGL::Color( polygon->colors().data() + i12 + 9, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PNm VCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_VCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Color( polygon->colors().data() + con3, opacity );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, VCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_VCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const size_t nnormals = polygon->numberOfNormals();
        for ( size_t i = 0, i3 = 0, i12 = 0; i < nnormals; i++, i3 += 3, i12 += 12 )
        {
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Color( polygon->colors().data() + i12, polygon->opacity( i3 ) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );

            kvs::OpenGL::Color( polygon->colors().data() + i12 + 3, polygon->opacity( i3 + 1 ) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );

            kvs::OpenGL::Color( polygon->colors().data() + i12 + 6, polygon->opacity( i3 + 2 ) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );

            kvs::OpenGL::Color( polygon->colors().data() + i12 + 9, polygon->opacity( i3 + 3 ) );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, VCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_VCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            const kvs::UInt8 opacity0 = polygon->opacity( con0 / 3 );
            const kvs::UInt8 opacity1 = polygon->opacity( con1 / 3 );
            const kvs::UInt8 opacity2 = polygon->opacity( con2 / 3 );
            const kvs::UInt8 opacity3 = polygon->opacity( con3 / 3 );

            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Color( polygon->colors().data() + con0, opacity0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );

            kvs::OpenGL::Color( polygon->colors().data() + con1, opacity1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );

            kvs::OpenGL::Color( polygon->colors().data() + con2, opacity2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );

            kvs::OpenGL::Color( polygon->colors().data() + con3, opacity3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, SC and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_SC_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const size_t nnormals = polygon->numberOfNormals();
        for ( size_t i = 0, i3 = 0, i12 = 0; i < nnormals; i++, i3 += 3, i12 += 12 )
        {
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, SC, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_SC_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const kvs::UInt8 opacity = polygon->opacity(0);
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;

            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadragles with PN, SC and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_SC_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);
        const size_t nopacities = polygon->numberOfOpacities();
        for ( size_t i = 0, i3 = 0, i12 = 0; i < nopacities; i++, i3 += 3, i12 += 12 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, SC, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_SC_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::RGBColor color = polygon->color(0);

        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            const kvs::UInt8 opacity = polygon->opacity(i);

            kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @berif  Rendering function for quadrangles with PN, PCs and O.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_PCs_O( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt8 opacity = polygon->opacity(0);
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i12 = 0; i < ncolors; i++, i3 += 3, i12 += 12 )
        {
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, PCs, O and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_PCs_O_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            const kvs::UInt8 opacity = polygon->opacity(0);

            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, PCs and Os.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_PCs_Os( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const size_t ncolors = polygon->numberOfColors();
        for ( size_t i = 0, i3 = 0, i12 = 0; i < ncolors; i++, i3 += 3, i12 += 12 )
        {
            const kvs::UInt8 opacity = polygon->opacity(i);
            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 3 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 6 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + i12 + 9 );
        }
    }
    kvs::OpenGL::End();
}

/*==========================================================================*/
/**
 *  @brief  Rendering function for quadrangles with PN, PCs, Os and Cs.
 *  @param  polygon [in] pointer to the polygon object
 */
/*==========================================================================*/
void Rendering_Quad_PN_PCs_Os_Cs( const kvs::PolygonObject* polygon )
{
    kvs::OpenGL::Begin( GL_QUADS );
    {
        const kvs::UInt32* connections = polygon->connections().data();
        const size_t nconnections = polygon->numberOfConnections();
        for ( size_t i = 0, i3 = 0, i4 = 0; i < nconnections; i++, i3 += 3, i4 += 4 )
        {
            const size_t con0 = *( connections + i4 ) * 3;
            const size_t con1 = *( connections + i4 + 1 ) * 3;
            const size_t con2 = *( connections + i4 + 2 ) * 3;
            const size_t con3 = *( connections + i4 + 3 ) * 3;
            const kvs::UInt8 opacity = polygon->opacity(i);

            kvs::OpenGL::Color( polygon->colors().data() + i3, opacity );
            kvs::OpenGL::Normal3( polygon->normals().data() + i3 );

            kvs::OpenGL::Vertex3( polygon->coords().data() + con0 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con1 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con2 );
            kvs::OpenGL::Vertex3( polygon->coords().data() + con3 );
        }
    }
    kvs::OpenGL::End();
}

typedef void (*PolygonRenderingFunctionType)( const kvs::PolygonObject* polygon );

enum PolygonRenderingType
{
    Type_Tri_VCs_O = 0,
    Type_Tri_VCs_O_Cs,
    Type_Tri_VCs_Os,
    Type_Tri_VCs_Os_Cs,
    Type_Tri_SC_O,
    Type_Tri_SC_O_Cs,
    Type_Tri_SC_Os,
    Type_Tri_SC_Os_Cs,
    Type_Tri_PCs_O,
    Type_Tri_PCs_O_Cs,
    Type_Tri_PCs_Os,
    Type_Tri_PCs_Os_Cs,

    Type_Quad_VCs_O,
    Type_Quad_VCs_O_Cs,
    Type_Quad_VCs_Os,
    Type_Quad_VCs_Os_Cs,
    Type_Quad_SC_O,
    Type_Quad_SC_O_Cs,
    Type_Quad_SC_Os,
    Type_Quad_SC_Os_Cs,
    Type_Quad_PCs_O,
    Type_Quad_PCs_O_Cs,
    Type_Quad_PCs_Os,
    Type_Quad_PCs_Os_Cs,

    Type_Tri_VN_VCs_O,
    Type_Tri_VN_VCs_O_Cs,
    Type_Tri_VN_VCs_Os,
    Type_Tri_VN_VCs_Os_Cs,
    Type_Tri_VN_SC_O,
    Type_Tri_VN_SC_O_Cs,
    Type_Tri_VN_SC_Os,
    Type_Tri_VN_SC_Os_Cs,
    Type_Tri_VN_PCs_O,
    Type_Tri_VN_PCs_O_Cs,
    Type_Tri_VN_PCs_Os,
    Type_Tri_VN_PCs_Os_Cs,

    Type_Quad_VN_VCs_O,
    Type_Quad_VN_VCs_O_Cs,
    Type_Quad_VN_VCs_Os,
    Type_Quad_VN_VCs_Os_Cs,
    Type_Quad_VN_SC_O,
    Type_Quad_VN_SC_O_Cs,
    Type_Quad_VN_SC_Os,
    Type_Quad_VN_SC_Os_Cs,
    Type_Quad_VN_PCs_O,
    Type_Quad_VN_PCs_O_Cs,
    Type_Quad_VN_PCs_Os,
    Type_Quad_VN_PCs_Os_Cs,

    Type_Tri_PN_VCs_O,
    Type_Tri_PN_VCs_O_Cs,
    Type_Tri_PN_VCs_Os,
    Type_Tri_PN_VCs_Os_Cs,
    Type_Tri_PN_SC_O,
    Type_Tri_PN_SC_O_Cs,
    Type_Tri_PN_SC_Os,
    Type_Tri_PN_SC_Os_Cs,
    Type_Tri_PN_PCs_O,
    Type_Tri_PN_PCs_O_Cs,
    Type_Tri_PN_PCs_Os,
    Type_Tri_PN_PCs_Os_Cs,

    Type_Quad_PN_VCs_O,
    Type_Quad_PN_VCs_O_Cs,
    Type_Quad_PN_VCs_Os,
    Type_Quad_PN_VCs_Os_Cs,
    Type_Quad_PN_SC_O,
    Type_Quad_PN_SC_O_Cs,
    Type_Quad_PN_SC_Os,
    Type_Quad_PN_SC_Os_Cs,
    Type_Quad_PN_PCs_O,
    Type_Quad_PN_PCs_O_Cs,
    Type_Quad_PN_PCs_Os,
    Type_Quad_PN_PCs_Os_Cs,

    NumberOfRenderingTypes
};

PolygonRenderingFunctionType Rendering[NumberOfRenderingTypes] =
{
    &Rendering_Tri_VCs_O,
    &Rendering_Tri_VCs_O_Cs,
    &Rendering_Tri_VCs_Os,
    &Rendering_Tri_VCs_Os_Cs,
    &Rendering_Tri_SC_O,
    &Rendering_Tri_SC_O_Cs,
    &Rendering_Tri_SC_Os,
    &Rendering_Tri_SC_Os_Cs,
    &Rendering_Tri_PCs_O,
    &Rendering_Tri_PCs_O_Cs,
    &Rendering_Tri_PCs_Os,
    &Rendering_Tri_PCs_Os_Cs,

    &Rendering_Quad_VCs_O,
    &Rendering_Quad_VCs_O_Cs,
    &Rendering_Quad_VCs_Os,
    &Rendering_Quad_VCs_Os_Cs,
    &Rendering_Quad_SC_O,
    &Rendering_Quad_SC_O_Cs,
    &Rendering_Quad_SC_Os,
    &Rendering_Quad_SC_Os_Cs,
    &Rendering_Quad_PCs_O,
    &Rendering_Quad_PCs_O_Cs,
    &Rendering_Quad_PCs_Os,
    &Rendering_Quad_PCs_Os_Cs,

    &Rendering_Tri_VN_VCs_O,
    &Rendering_Tri_VN_VCs_O_Cs,
    &Rendering_Tri_VN_VCs_Os,
    &Rendering_Tri_VN_VCs_Os_Cs,
    &Rendering_Tri_VN_SC_O,
    &Rendering_Tri_VN_SC_O_Cs,
    &Rendering_Tri_VN_SC_Os,
    &Rendering_Tri_VN_SC_Os_Cs,
    &Rendering_Tri_VN_PCs_O,
    &Rendering_Tri_VN_PCs_O_Cs,
    &Rendering_Tri_VN_PCs_Os,
    &Rendering_Tri_VN_PCs_Os_Cs,

    &Rendering_Quad_VN_VCs_O,
    &Rendering_Quad_VN_VCs_O_Cs,
    &Rendering_Quad_VN_VCs_Os,
    &Rendering_Quad_VN_VCs_Os_Cs,
    &Rendering_Quad_VN_SC_O,
    &Rendering_Quad_VN_SC_O_Cs,
    &Rendering_Quad_VN_SC_Os,
    &Rendering_Quad_VN_SC_Os_Cs,
    &Rendering_Quad_VN_PCs_O,
    &Rendering_Quad_VN_PCs_O_Cs,
    &Rendering_Quad_VN_PCs_Os,
    &Rendering_Quad_VN_PCs_Os_Cs,

    &Rendering_Tri_PN_VCs_O,
    &Rendering_Tri_PN_VCs_O_Cs,
    &Rendering_Tri_PN_VCs_Os,
    &Rendering_Tri_PN_VCs_Os_Cs,
    &Rendering_Tri_PN_SC_O,
    &Rendering_Tri_PN_SC_O_Cs,
    &Rendering_Tri_PN_SC_Os,
    &Rendering_Tri_PN_SC_Os_Cs,
    &Rendering_Tri_PN_PCs_O,
    &Rendering_Tri_PN_PCs_O_Cs,
    &Rendering_Tri_PN_PCs_Os,
    &Rendering_Tri_PN_PCs_Os_Cs,

    &Rendering_Quad_PN_VCs_O,
    &Rendering_Quad_PN_VCs_O_Cs,
    &Rendering_Quad_PN_VCs_Os,
    &Rendering_Quad_PN_VCs_Os_Cs,
    &Rendering_Quad_PN_SC_O,
    &Rendering_Quad_PN_SC_O_Cs,
    &Rendering_Quad_PN_SC_Os,
    &Rendering_Quad_PN_SC_Os_Cs,
    &Rendering_Quad_PN_PCs_O,
    &Rendering_Quad_PN_PCs_O_Cs,
    &Rendering_Quad_PN_PCs_Os,
    &Rendering_Quad_PN_PCs_Os_Cs,
};

PolygonRenderingType GetPolygonRenderingType( const kvs::PolygonObject* polygon )
{
    const size_t nopacities = polygon->numberOfOpacities();
    const size_t ncolors = polygon->numberOfColors();
    const size_t nconnects = polygon->numberOfConnections();
    const kvs::PolygonObject::PolygonType polygon_type = polygon->polygonType();
    const kvs::PolygonObject::NormalType normal_type = polygon->normalType();
    const kvs::PolygonObject::ColorType color_type = polygon->colorType();

    if ( polygon->normals().size() == 0 )
    {
        if ( polygon_type == kvs::PolygonObject::Triangle )
        {
            if ( color_type == kvs::PolygonObject::VertexColor )
            {
                if ( ncolors == 1 )
                {
                    if ( nopacities == 1 )
                        return ( nconnects == 0 ) ? Type_Tri_SC_O : Type_Tri_SC_O_Cs;
                    else
                        return ( nconnects == 0 ) ? Type_Tri_SC_Os : Type_Tri_SC_Os_Cs;
                }
                else
                {
                    if ( nopacities == 1 )
                        return ( nconnects == 0 ) ? Type_Tri_VCs_O : Type_Tri_VCs_O_Cs;
                    else
                        return ( nconnects == 0 ) ? Type_Tri_VCs_Os : Type_Tri_VCs_Os_Cs;
                }
            }
            else if ( color_type == kvs::PolygonObject::PolygonColor )
            {
                if ( ncolors == 1 )
                {
                    if ( nopacities == 1 )
                        return ( nconnects == 0 ) ? Type_Tri_SC_O : Type_Tri_SC_O_Cs;
                    else
                        return ( nconnects == 0 ) ? Type_Tri_SC_Os : Type_Tri_SC_Os_Cs;
                }
                else
                {
                    if ( nopacities == 1 )
                        return ( nconnects == 0 ) ? Type_Tri_PCs_O : Type_Tri_PCs_O_Cs;
                    else
                        return ( nconnects == 0 ) ? Type_Tri_PCs_Os : Type_Tri_PCs_Os_Cs;
                }
            }
        }
        else if ( polygon_type == kvs::PolygonObject::Quadrangle )
        {
            if ( color_type == kvs::PolygonObject::VertexColor )
            {
                if ( ncolors == 1 )
                {
                    if ( nopacities == 1 )
                        return ( nconnects == 0 ) ? Type_Quad_SC_O : Type_Quad_SC_O_Cs;
                    else
                        return ( nconnects == 0 ) ? Type_Quad_SC_Os : Type_Quad_SC_Os_Cs;
                }
                else
                {
                    if ( nopacities == 1 )
                        return ( nconnects == 0 ) ? Type_Quad_VCs_O : Type_Quad_VCs_O_Cs;
                    else
                        return ( nconnects == 0 ) ? Type_Quad_VCs_Os : Type_Quad_VCs_Os_Cs;
                }
            }
            else if ( color_type == kvs::PolygonObject::PolygonColor )
            {
                if ( ncolors == 1 )
                {
                    if ( nopacities == 1 )
                        return ( nconnects == 0 ) ? Type_Quad_SC_O : Type_Quad_SC_O_Cs;
                    else
                        return ( nconnects == 0 ) ? Type_Quad_SC_Os : Type_Quad_SC_Os_Cs;
                }
                else
                {
                    if ( nopacities == 1 )
                        return ( nconnects == 0 ) ? Type_Quad_PCs_O : Type_Quad_PCs_O_Cs;
                    else
                        return ( nconnects == 0 ) ? Type_Quad_PCs_Os : Type_Quad_PCs_Os_Cs;
                }
            }
        }
    }

    else
    {
        if ( normal_type == kvs::PolygonObject::VertexNormal )
        {
            if ( polygon_type == kvs::PolygonObject::Triangle )
            {
                if ( color_type == kvs::PolygonObject::VertexColor )
                {
                    if ( ncolors == 1 )
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Tri_VN_SC_O : Type_Tri_VN_SC_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Tri_VN_SC_Os : Type_Tri_VN_SC_Os_Cs;
                    }
                    else
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Tri_VN_VCs_O : Type_Tri_VN_VCs_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Tri_VN_VCs_Os : Type_Tri_VN_VCs_Os_Cs;
                    }
                }
                else if ( color_type == kvs::PolygonObject::PolygonColor )
                {
                    if ( ncolors == 1 )
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Tri_VN_SC_O : Type_Tri_VN_SC_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Tri_VN_SC_Os : Type_Tri_VN_SC_Os_Cs;
                    }
                    else
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Tri_VN_PCs_O : Type_Tri_VN_PCs_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Tri_VN_PCs_Os : Type_Tri_VN_PCs_Os_Cs;
                    }
                }
            }
            else if ( polygon_type == kvs::PolygonObject::Quadrangle )
            {
                if ( color_type == kvs::PolygonObject::VertexColor )
                {
                    if ( ncolors == 1 )
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Quad_VN_SC_O : Type_Quad_VN_SC_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Quad_VN_SC_Os : Type_Quad_VN_SC_Os_Cs;
                    }
                    else
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Quad_VN_VCs_O : Type_Quad_VN_VCs_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Quad_VN_VCs_Os : Type_Quad_VN_VCs_Os_Cs;
                    }
                }
                else if ( color_type == kvs::PolygonObject::PolygonColor )
                {
                    if ( ncolors == 1 )
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Quad_VN_SC_O : Type_Quad_VN_SC_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Quad_VN_SC_Os : Type_Quad_VN_SC_Os_Cs;
                    }
                    else
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Quad_VN_PCs_O : Type_Quad_VN_PCs_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Quad_VN_PCs_Os : Type_Quad_VN_PCs_Os_Cs;
                    }
                }
            }
        }

        else if ( normal_type == kvs::PolygonObject::PolygonNormal )
        {
            if ( polygon_type == kvs::PolygonObject::Triangle )
            {
                if ( color_type == kvs::PolygonObject::VertexColor )
                {
                    if ( ncolors == 1 )
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Tri_PN_SC_O : Type_Tri_PN_SC_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Tri_PN_SC_Os : Type_Tri_PN_SC_Os_Cs;
                    }
                    else
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Tri_PN_VCs_O : Type_Tri_PN_VCs_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Tri_PN_VCs_Os : Type_Tri_PN_VCs_Os_Cs;
                    }
                }
                else if ( color_type == kvs::PolygonObject::PolygonColor )
                {
                    if ( ncolors == 1 )
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Tri_PN_SC_O : Type_Tri_PN_SC_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Tri_PN_SC_Os : Type_Tri_PN_SC_Os_Cs;
                    }
                    else
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Tri_PN_PCs_O : Type_Tri_PN_PCs_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Tri_PN_PCs_Os : Type_Tri_PN_PCs_Os_Cs;
                    }
                }
            }

            else if ( polygon_type == kvs::PolygonObject::Quadrangle )
            {
                if ( color_type == kvs::PolygonObject::VertexColor )
                {
                    if ( ncolors == 1 )
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Quad_PN_SC_O : Type_Quad_PN_SC_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Quad_PN_SC_Os : Type_Quad_PN_SC_Os_Cs;
                    }
                    else
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Quad_PN_VCs_O : Type_Quad_PN_VCs_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Quad_PN_VCs_Os : Type_Quad_PN_VCs_Os_Cs;
                    }
                }
                else if ( color_type == kvs::PolygonObject::PolygonColor )
                {
                    if ( ncolors == 1 )
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Quad_PN_SC_O : Type_Quad_PN_SC_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Quad_PN_SC_Os : Type_Quad_PN_SC_Os_Cs;
                    }
                    else
                    {
                        if ( nopacities == 1 )
                            return ( nconnects == 0 ) ? Type_Quad_PN_PCs_O : Type_Quad_PN_PCs_O_Cs;
                        else
                            return ( nconnects == 0 ) ? Type_Quad_PN_PCs_Os : Type_Quad_PN_PCs_Os_Cs;
                    }
                }
            }
        }
    }

    return Type_Tri_VCs_O;
};

void PolygonRenderingFunction( const kvs::PolygonObject* polygon )
{
    if ( polygon->numberOfVertices() > 0 )
    {
        PolygonRenderingType type = GetPolygonRenderingType( polygon );
        Rendering[type]( polygon );
    }
};

#else

void PolygonRenderingFunction( const kvs::PolygonObject* )
{
};

#endif

} // end of namespace
