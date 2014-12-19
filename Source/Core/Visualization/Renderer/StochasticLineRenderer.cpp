/*****************************************************************************/
/**
 *  @file   StochasticLineRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "StochasticLineRenderer.h"
#include <cmath>
#include <kvs/OpenGL>
#include <kvs/LineObject>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Assert>
#include <kvs/Message>
#include <kvs/Xorshift128>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns a random number as integer value.
 *  @return random number
 */
/*===========================================================================*/
int RandomNumber()
{
    const int C = 12347;
    static kvs::Xorshift128 R;
    return C * R.randInteger();
}

/*===========================================================================*/
/**
 *  @brief  Returns vertex-color array.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt8> VertexColors( const kvs::LineObject* line )
{
    if ( line->colorType() == kvs::LineObject::VertexColor ) return line->colors();

    const size_t nvertices = line->numberOfVertices();
    const kvs::RGBColor color = line->color();

    kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        colors[ 3 * i + 0 ] = color.r();
        colors[ 3 * i + 1 ] = color.g();
        colors[ 3 * i + 2 ] = color.b();
    }

    return colors;
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticLineRenderer class.
 */
/*===========================================================================*/
StochasticLineRenderer::StochasticLineRenderer():
    StochasticRendererBase( new Engine() )
{
}

/*===========================================================================*/
/**
 *  @brief  Sets an opacity value.
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void StochasticLineRenderer::setOpacity( const kvs::UInt8 opacity )
{
    static_cast<Engine&>( engine() ).setOpacity( opacity );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Engine class.
 */
/*===========================================================================*/
StochasticLineRenderer::Engine::Engine():
    m_line_opacity( 255 ),
    m_has_connection( false ),
    m_random_index( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Releases the GPU resources.
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::release()
{
    m_shader_program.release();
    m_vbo.release();
    m_ibo.release();
}

/*===========================================================================*/
/**
 *  @brief  Create.
 *  @param  object [in] pointer to the line object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::LineObject* line = kvs::LineObject::DownCast( object );
    kvs::LineObject::LineType type = line->lineType();
    m_has_connection = line->numberOfConnections() > 0 &&
        ( type == kvs::LineObject::Segment || type == kvs::LineObject::Uniline );

    attachObject( object );
    createRandomTexture();
    this->create_shader_program();
    this->create_buffer_object( line );
}

/*===========================================================================*/
/**
 *  @brief  Update.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
}

/*===========================================================================*/
/**
 *  @brief  Set up.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    m_random_index = m_shader_program.attributeLocation("random_index");

    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
    m_shader_program.bind();
    m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
    m_shader_program.setUniform( "random_texture_size_inv", 1.0f / randomTextureSize() );
    m_shader_program.setUniform( "random_texture", 0 );
    m_shader_program.setUniform( "opacity", m_line_opacity / 255.0f );
    m_shader_program.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Draw an ensemble.
 *  @param  object [in] pointer to the line object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::LineObject* line = kvs::LineObject::DownCast( object );

    kvs::VertexBufferObject::Binder bind1( m_vbo );
    kvs::ProgramObject::Binder bind2( m_shader_program );
    kvs::Texture::Binder bind3( randomTexture() );
    {
        const size_t size = randomTextureSize();
        const int count = repetitionCount() * ::RandomNumber();
        const float offset_x = static_cast<float>( ( count ) % size );
        const float offset_y = static_cast<float>( ( count / size ) % size );
        const kvs::Vec2 random_offset( offset_x, offset_y );
        m_shader_program.setUniform( "random_offset", random_offset );

        const size_t nlines = line->numberOfConnections();
        const size_t index_size = line->numberOfVertices() * 2 * sizeof( kvs::UInt16 );
        const size_t coord_size = line->numberOfVertices() * 3 * sizeof( kvs::Real32 );

        KVS_GL_CALL( glLineWidth( line->size() ) );

        // Enable coords.
        KVS_GL_CALL( glEnableClientState( GL_VERTEX_ARRAY ) );
        KVS_GL_CALL( glVertexPointer( 3, GL_FLOAT, 0, (GLbyte*)NULL + index_size ) );

        // Enable colors.
        KVS_GL_CALL( glEnableClientState( GL_COLOR_ARRAY ) );
        KVS_GL_CALL( glColorPointer( 3, GL_UNSIGNED_BYTE, 0, (GLbyte*)NULL + index_size + coord_size ) );

        // Enable random index.
        KVS_GL_CALL( glEnableVertexAttribArray( m_random_index ) );
        KVS_GL_CALL( glVertexAttribPointer( m_random_index, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, (GLubyte*)NULL + 0 ) );

        // Draw lines.
        if ( m_has_connection )
        {
            kvs::IndexBufferObject::Binder bind4( m_ibo );
            if ( line->lineType() == kvs::LineObject::Uniline )
            {
                KVS_GL_CALL( glDrawElements( GL_LINE_STRIP, nlines, GL_UNSIGNED_INT, 0 ) );
            }
            else if ( line->lineType() ==  kvs::LineObject::Segment )
            {
                KVS_GL_CALL( glDrawElements( GL_LINES, 2 * nlines, GL_UNSIGNED_INT, 0 ) );
            }
        }
        else
        {
            if ( line->lineType() == kvs::LineObject::Polyline )
            {
                // if OpenGL version is 1.4 or later
                GLint* first = m_first_array.data();
                GLsizei* count = m_count_array.data();
                GLsizei primecount = m_first_array.size();
                KVS_GL_CALL( glMultiDrawArrays( GL_LINE_STRIP, first, count, primecount ) );
                // else
                //for ( size_t i = 0; i < nlines; i++ )
                //{
                //    const GLint first = m_first_array[i];
                //    const GLsizei count = m_count_array[i];
                //    KVS_GL_CALL( glDrawArrays( GL_LINE_STRIP, first, count ) );
                //}
            }
            else if ( line->lineType() == kvs::LineObject::Strip )
            {
                const size_t nvertices = line->numberOfVertices();
                KVS_GL_CALL( glDrawArrays( GL_LINE_STRIP, 0, nvertices ) );
            }
        }

        // Disable coords.
        KVS_GL_CALL( glDisableClientState( GL_VERTEX_ARRAY ) );

        // Disable colors.
        KVS_GL_CALL( glDisableClientState( GL_COLOR_ARRAY ) );

        // Disable random index.
        KVS_GL_CALL( glDisableVertexAttribArray( m_random_index ) );
    }

//    countRepetitions();
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::create_shader_program()
{
    kvs::ShaderSource vert( "SR_line.vert" );
    kvs::ShaderSource frag( "SR_line.frag" );
    m_shader_program.build( vert, frag );
}

/*===========================================================================*/
/**
 *  @brief  Create buffer objects.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::create_buffer_object( const kvs::LineObject* line )
{
    if ( line->numberOfColors() != 1 && line->colorType() == kvs::LineObject::LineColor )
    {
        kvsMessageError("Not supported line color type.");
        return;
    }

    const size_t nvertices = line->numberOfVertices();
    kvs::ValueArray<kvs::UInt16> indices( nvertices * 2 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        const unsigned int count = i * 12347;
        indices[ 2 * i + 0 ] = static_cast<kvs::UInt16>( ( count ) % randomTextureSize() );
        indices[ 2 * i + 1 ] = static_cast<kvs::UInt16>( ( count / randomTextureSize() ) % randomTextureSize() );
    }
    kvs::ValueArray<kvs::Real32> coords = line->coords();
    kvs::ValueArray<kvs::UInt8> colors = ::VertexColors( line );

    const size_t index_size = indices.byteSize();
    const size_t coord_size = coords.byteSize();
    const size_t color_size = colors.byteSize();
    const size_t byte_size = index_size + coord_size + color_size;

    m_vbo.create( byte_size );
    m_vbo.bind();
    m_vbo.load( index_size, indices.data(), 0 );
    m_vbo.load( coord_size, coords.data(), index_size );
    m_vbo.load( color_size, colors.data(), index_size + coord_size );
    m_vbo.unbind();

    if ( m_has_connection )
    {
        const size_t nlines = line->numberOfConnections();
        const size_t connection_size = sizeof( kvs::UInt32 ) * 2 * nlines;
        m_ibo.create( connection_size );
        m_ibo.bind();
        m_ibo.load( connection_size, line->connections().data(), 0 );
        m_ibo.unbind();
    }
    else
    {
        if ( line->lineType() == kvs::LineObject::Polyline )
        {
            const kvs::UInt32* pconnections = line->connections().data();
            m_first_array.allocate( line->numberOfConnections() );
            m_count_array.allocate( m_first_array.size() );
            for ( size_t i = 0; i < m_first_array.size(); ++i )
            {
                m_first_array[i] = pconnections[ 2 * i ];
                m_count_array[i] = pconnections[ 2 * i + 1 ] - pconnections[ 2 * i ] + 1;
            }
        }
    }
}

} // end of namespace kvs
