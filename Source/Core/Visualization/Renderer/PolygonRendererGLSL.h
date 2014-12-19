/*****************************************************************************/
/**
 *  @file   PolygonRendererGLSL.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonRenderer.h 1398 2012-12-05 09:33:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLSL__POLYGON_RENDERER_H_INCLUDE
#define KVS__GLSL__POLYGON_RENDERER_H_INCLUDE

#include <kvs/DebugNew>
#include <kvs/Module>
#include <kvs/PolygonObject>
#include <kvs/Shader>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObject>
#include <kvs/IndexBufferObject>


namespace kvs
{

namespace glsl
{

class PolygonRenderer : public kvs::PolygonRenderer
{
    kvsModule( kvs::glsl::PolygonRenderer, Renderer );
    kvsModuleBaseClass( kvs::PolygonRenderer );

protected:

    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    bool m_has_normal; ///< check flag for the normal array
    bool m_has_connection; ///< check flag for the connection array
    kvs::Shader::ShadingModel* m_shader; ///< shading method
    kvs::ProgramObject m_shader_program; ///< shader program
    kvs::VertexBufferObject m_vbo; ///< vertex buffer object
    kvs::IndexBufferObject m_ibo; ///< index buffer object

public:

    PolygonRenderer();
    virtual ~PolygonRenderer();

public:

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    template <typename ShadingType>
    void setShader( const ShadingType shader );

private:

    void create_shader_program();
    void create_buffer_object( const kvs::PolygonObject* point );
};

template <typename ShadingType>
inline void PolygonRenderer::setShader( const ShadingType shader )
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

#endif // KVS__GLSL__POLYGON_RENDERER_H_INCLUDE
