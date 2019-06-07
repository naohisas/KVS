/*****************************************************************************/
/**
 *  @file   LineRendererGLSL.h
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
#pragma once
#include <kvs/DebugNew>
#include <kvs/Module>
#include <kvs/LineObject>
#include <kvs/LineRenderer>
#include <kvs/Shader>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>


namespace kvs
{

namespace glsl
{

class LineRenderer : public kvs::LineRenderer
{
    kvsModule( kvs::glsl::LineRenderer, Renderer );
    kvsModuleBaseClass( kvs::LineRenderer );

private:
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    kvs::ValueArray<GLint> m_first_array; ///< array of starting indices for the polyline
    kvs::ValueArray<GLsizei> m_count_array; ///< array of the number of indices for the polyline
    bool m_has_connection; ///< check flag for the connection array
    kvs::Shader::ShadingModel* m_shader; ///< shading method
    kvs::ProgramObject m_shader_program; ///< shader program
    kvs::VertexBufferObjectManager m_vbo_manager; ///< vertex buffer object manager

    kvs::Vec2 m_line_width_range;
    float m_outline_width;
    kvs::RGBColor m_outline_color;

public:
    LineRenderer();
    virtual ~LineRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    template <typename ShadingType>
    void setShader( const ShadingType shader );
    void setOutlineWidth( const float width ) { m_outline_width = width; }
    void setOutlineColor( const kvs::RGBColor color ) { m_outline_color = color; }

private:
    void create_shader_program();
    void create_buffer_object( const kvs::LineObject* line );
};

template <typename ShadingType>
inline void LineRenderer::setShader( const ShadingType shader )
{
    if ( m_shader )
    {
        delete m_shader;
        m_shader = NULL;
    }

    m_shader = new ShadingType( shader );
    if ( !m_shader )
    {
        kvsMessageError("Cannot create a specified shader.");
    }
};

} // end of namespace glsl

} // end of namespace kvs
