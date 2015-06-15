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
#ifndef KVS__LINE_RENDERING_FUNCTION_H_INCLUDE
#define KVS__LINE_RENDERING_FUNCTION_H_INCLUDE

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
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Strip_VCs_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );
    glBegin( GL_LINE_STRIP );
    {
        const size_t nvertices = line->numberOfVertices();

        for( size_t i = 0; i < nvertices; i++ )
        {
            const kvs::RGBColor& color    = line->color(i);
            const kvs::Vector3f& position = line->coord(i);

            glColor3ub( color.r(),    color.g(),    color.b()  );
            glVertex3f( position.x(), position.y(), position.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Strip_VCs_Ss( const kvs::LineObject* line )
{
    const size_t nlines = line->numberOfVertices() - 1;
    for( size_t i = 0; i < nlines; i++ )
    {
        glLineWidth( line->size(i) );

        const kvs::RGBColor& color1    = line->color(i);
        const kvs::Vector3f& position1 = line->coord(i);
        const kvs::RGBColor& color2    = line->color(i+1);
        const kvs::Vector3f& position2 = line->coord(i+1);

        glBegin( GL_LINES );
        {
            glColor3ub( color1.r(),    color1.g(),    color1.b()  );
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glColor3ub( color2.r(),    color2.g(),    color2.b()  );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Strip_LC_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    const size_t         nvertices = line->numberOfVertices();
    const kvs::RGBColor& color     = line->color(0);

    glBegin( GL_LINE_STRIP );
    {
        glColor3ub( color.r(), color.g(), color.b() );
        for( size_t i = 0; i < nvertices; i++ )
        {
            const kvs::Vector3f& position = line->coord(i);
            glVertex3f( position.x(), position.y(), position.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Strip_LCs_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    const size_t num = line->numberOfVertices() - 1;

    glBegin( GL_LINES );
    {
        for( size_t i = 0; i < num; i++ )
        {
            const kvs::RGBColor& color     = line->color(i);
            const kvs::Vector3f& position1 = line->coord(i);
            const kvs::Vector3f& position2 = line->coord(i+1);

            glColor3ub( color.r(),    color.g(),    color.b()  );
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Strip_LC_Ss( const kvs::LineObject* line )
{
    const size_t         num   = line->numberOfVertices() - 1;
    const kvs::RGBColor& color = line->color(0);
    glColor3ub( color.r(), color.g(), color.b() );

    for( size_t i = 0; i < num; i++ )
    {
        glLineWidth( line->size( i ) );

        const kvs::Vector3f& position1 = line->coord(i);
        const kvs::Vector3f& position2 = line->coord(i+1);

        glBegin( GL_LINES );
        {
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Strip_LCs_Ss( const kvs::LineObject* line )
{
    const size_t num = line->numberOfVertices() - 1;

    for( size_t i = 0; i < num; i++ )
    {
        const kvs::RGBColor& color     = line->color(i);
        const kvs::Vector3f& position1 = line->coord(i);
        const kvs::Vector3f& position2 = line->coord(i+1);

        glLineWidth( line->size( i ) );
        glColor3ub( color.r(),    color.g(),    color.b()  );

        glBegin( GL_LINES );
        {
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Uniline_VCs_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    glBegin( GL_LINE_STRIP );
    {
        const size_t nconnections = line->numberOfConnections();
        for( size_t i = 0; i < nconnections; i++ )
        {
            size_t id = *( line->connections().data() + i );

            const kvs::RGBColor& color    = line->color(id);
            const kvs::Vector3f& position = line->coord(id);

            glColor3ub( color.r(),    color.g(),    color.b()  );
            glVertex3f( position.x(), position.y(), position.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Uniline_VCs_Ss( const kvs::LineObject* line )
{
    const size_t num = line->numberOfConnections() - 1;
    for( size_t i = 0; i < num; i++ )
    {
        size_t id1 = *( line->connections().data() + i   );
        size_t id2 = *( line->connections().data() + i+1 );

        const kvs::RGBColor& color1    = line->color(id1);
        const kvs::RGBColor& color2    = line->color(id2);
        const kvs::Vector3f& position1 = line->coord(id1);
        const kvs::Vector3f& position2 = line->coord(id2);

        glLineWidth( line->size( i ) );
        glBegin( GL_LINES );
        {
            glColor3ub( color1.r(),    color1.g(),    color1.b()  );
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glColor3ub( color2.r(),    color2.g(),    color2.b()  );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Uniline_LC_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    glBegin( GL_LINE_STRIP );
    {
        const kvs::RGBColor& color = line->color(0);
        glColor3ub( color.r(), color.g(), color.b() );

        const size_t nconnections = line->numberOfConnections();

        for( size_t i = 0; i < nconnections; i++ )
        {
            const kvs::Vector3f& position = line->coord(i);
            glVertex3d( position.x(), position.y(), position.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Uniline_LCs_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    glBegin( GL_LINES );
    {
        const size_t num = line->numberOfConnections() - 1;
        for( size_t i = 0; i < num; i++ )
        {
            const kvs::RGBColor& color     = line->color(i);
            const kvs::Vector3f& position1 = line->coord(i);
            const kvs::Vector3f& position2 = line->coord(i+1);

            glColor3ub( color.r(),    color.g(),    color.b()  );
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Uniline_LC_Ss( const kvs::LineObject* line )
{
    const kvs::RGBColor& color = line->color(0);
    glColor3ub( color.r(), color.g(), color.b() );

    const size_t num = line->numberOfConnections() - 1;

    for( size_t i = 0; i < num; i++ )
    {
        const kvs::Vector3f& position1 = line->coord(i  );
        const kvs::Vector3f& position2 = line->coord(i+1);

        glLineWidth( line->size( i ) );
        glBegin( GL_LINES );
        {
            glVertex3d( position1.x(), position1.y(), position1.z() );
            glVertex3d( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Uniline_LCs_Ss( const kvs::LineObject* line )
{
    const size_t num = line->numberOfConnections() - 1;
    for( size_t i = 0; i < num; i++ )
    {
        const kvs::RGBColor& color     = line->color(i);
        const kvs::Vector3f& position1 = line->coord(i);
        const kvs::Vector3f& position2 = line->coord(i+1);

        glLineWidth( line->size( i ) );
        glColor3ub( color.r(), color.g(), color.b()  );
        glBegin( GL_LINES );
        {
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Polyline_VCs_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1   = *( line->connections().data() + index );
        const size_t id2   = *( line->connections().data() + index + 1 );

        glBegin( GL_LINE_STRIP );
        {
            for( size_t j = id1; j <= id2; j++ )
            {
                const kvs::RGBColor& color    = line->color(j);
                const kvs::Vector3f& position = line->coord(j);

                glColor3ub( color.r(),    color.g(),    color.b()  );
                glVertex3f( position.x(), position.y(), position.z() );
            }
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Polyline_VCs_Ss( const kvs::LineObject* line )
{
    int ctr = 0;
    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2*i;
        const size_t id1   = *( line->connections().data() + index   );
        const size_t id2   = *( line->connections().data() + index+1 );

        for( size_t j = id1; j < id2; j++ )
        {
            const kvs::RGBColor& color1    = line->color(j);
            const kvs::RGBColor& color2    = line->color(j+1);
            const kvs::Vector3f& position1 = line->coord(j);
            const kvs::Vector3f& position2 = line->coord(j+1);

            glLineWidth( line->size( ctr ) );
            glBegin( GL_LINES );
            {
                glColor3ub( color1.r(),    color1.g(),    color1.b()  );
                glVertex3f( position1.x(), position1.y(), position1.z() );
                glColor3ub( color2.r(),    color2.g(),    color2.b()  );
                glVertex3f( position2.x(), position2.y(), position2.z() );
            }
            glEnd();

            ctr++;
        }
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Polyline_LC_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1   = *( line->connections().data() + index );
        const size_t id2   = *( line->connections().data() + index + 1 );

        glBegin( GL_LINE_STRIP );
        {
            const kvs::RGBColor& color = line->color(0);
            glColor3ub( color.r(), color.g(), color.b() );

            for( size_t j = id1; j < id2; j++ )
            {
                const kvs::Vector3f& position1 = line->coord(j);
                const kvs::Vector3f& position2 = line->coord(j+1);

                glVertex3f( position1.x(), position1.y(), position1.z() );
                glVertex3f( position2.x(), position2.y(), position2.z() );
            }
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Polyline_LCs_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    int ctr = 0;
    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2 * i;
        const size_t id1   = *( line->connections().data() + index );
        const size_t id2   = *( line->connections().data() + index + 1 );

        glBegin( GL_LINE_STRIP );
        {
            for( size_t j = id1; j < id2; j++ )
            {
                const kvs::Vector3f& position1 = line->coord(j);
                const kvs::Vector3f& position2 = line->coord(j+1);
                const kvs::RGBColor& color     = line->color(ctr);

                glColor3ub( color.r(), color.g(), color.b() );
                glVertex3f( position1.x(), position1.y(), position1.z() );
                glVertex3f( position2.x(), position2.y(), position2.z() );
                ctr++;
            }
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Polyline_LC_Ss( const kvs::LineObject* line )
{
    int ctr = 0;
    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2*i;
        const size_t id1   = *( line->connections().data() + index   );
        const size_t id2   = *( line->connections().data() + index+1 );

        const kvs::RGBColor& color = line->color(0);
        glColor3ub( color.r(), color.g(), color.b() );

        for( size_t j = id1; j < id2; j++ )
        {
            const kvs::Vector3f& position1 = line->coord(j);
            const kvs::Vector3f& position2 = line->coord(j+1);

            glLineWidth( line->size( ctr ) );
            glBegin( GL_LINES );
            {
                glVertex3f( position1.x(), position1.y(), position1.z() );
                glVertex3f( position2.x(), position2.y(), position2.z() );
            }
            glEnd();

            ctr++;
        }
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Polyline_LCs_Ss( const kvs::LineObject* line )
{
    int ctr = 0;
    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2*i;
        const size_t id1   = *( line->connections().data() + index   );
        const size_t id2   = *( line->connections().data() + index+1 );

        for( size_t j = id1; j < id2; j++ )
        {
            const kvs::RGBColor& color     = line->color(ctr);
            const kvs::Vector3f& position1 = line->coord(j);
            const kvs::Vector3f& position2 = line->coord(j+1);

            glLineWidth( line->size( ctr ) );
            glBegin( GL_LINES );
            {
                glColor3ub( color.r(),     color.g(),     color.b()     );
                glVertex3f( position1.x(), position1.y(), position1.z() );
                glVertex3f( position2.x(), position2.y(), position2.z() );
            }
            glEnd();

            ctr++;
        }
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Segment_VCs_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    glBegin( GL_LINES );
    {
        const size_t num = line->numberOfConnections() * 2;
        for( size_t i = 0; i < num; i++ )
        {
            const size_t id = *( line->connections().data() + i );

            const kvs::RGBColor& color    = line->color(id);
            const kvs::Vector3f& position = line->coord(id);

            glColor3ub( color.r(),    color.g(),    color.b()  );
            glVertex3f( position.x(), position.y(), position.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Segment_VCs_Ss( const kvs::LineObject* line )
{
    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2*i;
        const size_t id1   = *( line->connections().data() + index   );
        const size_t id2   = *( line->connections().data() + index+1 );

        const kvs::RGBColor& color1    = line->color(id1);
        const kvs::RGBColor& color2    = line->color(id2);
        const kvs::Vector3f& position1 = line->coord(id1);
        const kvs::Vector3f& position2 = line->coord(id2);

        glLineWidth( line->size( i ) );
        glBegin( GL_LINES );
        {
            glColor3ub( color1.r(),    color1.g(),    color1.b()  );
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glColor3ub( color2.r(),    color2.g(),    color2.b()  );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Segment_LC_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    const kvs::RGBColor& color = line->color(0);
    glColor3ub( color.r(), color.g(), color.b() );

    glBegin( GL_LINES );
    {
        const size_t num = line->numberOfConnections() * 2;
        for( size_t i = 0; i < num; i++ )
        {
            const size_t id = *( line->connections().data() + i );
            const kvs::Vector3f& position = line->coord(id);

            glVertex3f( position.x(), position.y(), position.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Segment_LCs_S( const kvs::LineObject* line )
{
    glLineWidth( line->size( 0 ) );

    glBegin( GL_LINES );
    {
        const size_t nconnections = line->numberOfConnections();
        for( size_t i = 0; i < nconnections; i++ )
        {
            const kvs::RGBColor& color = line->color(i);

            const size_t index = 2 * i;
            const size_t id1 = *( line->connections().data() + index   );
            const size_t id2 = *( line->connections().data() + index+1 );

            const kvs::Vector3f& position1 = line->coord(id1);
            const kvs::Vector3f& position2 = line->coord(id2);

            glColor3ub( color.r(),    color.g(),    color.b()  );
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
    }
    glEnd();
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Segment_LC_Ss( const kvs::LineObject* line )
{
    const kvs::RGBColor& color = line->color(0);
    glColor3ub( color.r(), color.g(), color.b() );

    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const size_t index = 2*i;
        const size_t id1   = *( line->connections().data() + index );
        const size_t id2   = *( line->connections().data() + index+1 );

        const kvs::Vector3f& position1 = line->coord(id1);
        const kvs::Vector3f& position2 = line->coord(id2);

        glLineWidth( line->size( i ) );
        glBegin( GL_LINES );
        {
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

/*==========================================================================*/
/**
 *  
 *  @param line [in] pointer to the line object
 */
/*==========================================================================*/
void Rendering_Segment_LCs_Ss( const kvs::LineObject* line )
{
    const size_t nconnections = line->numberOfConnections();
    for( size_t i = 0; i < nconnections; i++ )
    {
        const kvs::RGBColor& color = line->color(i);

        const size_t index = 2 * i;
        const size_t id1 = *( line->connections().data() + index   );
        const size_t id2 = *( line->connections().data() + index+1 );
        const kvs::Vector3f& position1 = line->coord(id1);
        const kvs::Vector3f& position2 = line->coord(id2);

        glColor3ub( color.r(), color.g(), color.b() );
        glLineWidth( line->size( i ) );

        glBegin( GL_LINES );
        {
            glVertex3f( position1.x(), position1.y(), position1.z() );
            glVertex3f( position2.x(), position2.y(), position2.z() );
        }
        glEnd();
    }
}

typedef void (*LineRenderingFunctionType)( const kvs::LineObject* line );

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
    const size_t nsizes    = line->numberOfSizes();
    const size_t ncolors   = line->numberOfColors();

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

void LineRenderingFunction( const kvs::LineObject* line )
{
    if( line->numberOfVertices() > 0 )
    {
        LineRenderingType type = GetLineRenderingType( line );
        Rendering[type]( line );
    }
};

#else

void LineRenderingFunction( const kvs::LineObject* )
{
}

#endif // KVS_ENABLE_OPENGL

} // end of namespace

#endif // KVS__LINE_RENDERING_FUNCTION_H_INCLUDE
