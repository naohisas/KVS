/*****************************************************************************/
/**
 *  @file   PointRendererGLSL.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PointRenderer.h 1398 2012-12-05 09:33:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/DebugNew>
#include <kvs/Module>
#include <kvs/PointObject>
#include <kvs/Shader>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Point renderer class with VBO
 */
/*===========================================================================*/
class PointRenderer : public kvs::PointRenderer
{
    kvsModule( kvs::glsl::PointRenderer, Renderer );
    kvsModuleBaseClass( kvs::PointRenderer );

private:
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    bool m_has_normal; ///< check flag for the normal array
    kvs::Shader::ShadingModel* m_shader; ///< shading method
    kvs::ProgramObject m_shader_program; ///< shader program
    kvs::VertexBufferObjectManager m_vbo_manager; ///< vertex buffer object manager

public:
    PointRenderer();
    virtual ~PointRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    template <typename ShadingType>
    void setShader( const ShadingType shader );

private:
    void create_shader_program();
    void create_buffer_object( const kvs::PointObject* point );
};

template <typename ShadingType>
inline void PointRenderer::setShader( const ShadingType shader )
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
