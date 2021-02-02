/*****************************************************************************/
/**
 *  @file   RayCastingRendererGLSL.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "RayCastingRendererGLSL.h"
#include <kvs/DebugNew>
#include <kvs/Shader>
#include <kvs/Vector3>
#include <kvs/OpenGL>
#include <kvs/Coordinate>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Normalizes value array.
 *  @param  volume [in] pointer to the volume object
 *  @param  min_value [in] minimum value of the volume data
 *  @param  max_value [in] maximum value of the volume data
 *  @return normalized value array
 */
/*===========================================================================*/
template <typename T>
kvs::AnyValueArray NormalizeValues(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Real32 min_value,
    const kvs::Real32 max_value )
{
    const kvs::Real32 scale = 1.0f / ( max_value - min_value );
    const size_t nnodes = volume->numberOfNodes();
    const T* src = static_cast<const T*>( volume->values().data() );

    kvs::ValueArray<kvs::Real32> data( nnodes );
    kvs::Real32* dst = data.data();
    for ( size_t i = 0; i < nnodes; i++ )
    {
        *(dst++) = static_cast<kvs::Real32>(( *(src++) - min_value ) * scale);
    }

    return kvs::AnyValueArray( data );
}

/*===========================================================================*/
/**
 *  @brief  Returns unsigned value array converted from signed value array.
 *  @param  volume [in] pointer to the volume object
 *  @return unsigned value array
 */
/*===========================================================================*/
template <typename DstType, typename SrcType>
kvs::AnyValueArray SignedToUnsigned( const kvs::StructuredVolumeObject* volume )
{
    const SrcType min = kvs::Value<SrcType>::Min();
    const size_t nvalues = volume->values().size();
    const SrcType* src = static_cast<const SrcType*>( volume->values().data() );

    kvs::ValueArray<DstType> data( nvalues );
    DstType* dst = data.data();
    for ( size_t i = 0; i < nvalues; i++ )
    {
        *(dst++) = static_cast<DstType>( *(src++) - min );
    }

    return kvs::AnyValueArray( data );
}

} // end of namespace


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Create a volume buffer object on GPU.
 *  @param  volume [in] pointer to the volume object
 *  @param  tfunc [in] transfer function
 */
/*===========================================================================*/
void RayCastingRenderer::BufferObject::create(
    const kvs::StructuredVolumeObject* volume,
    const kvs::TransferFunction& tfunc )
{
    const size_t width = volume->resolution().x();
    const size_t height = volume->resolution().y();
    const size_t depth = volume->resolution().z();

    GLenum data_format = 0;
    GLenum data_type = 0;
    kvs::AnyValueArray data_value;
    const std::type_info& type = volume->values().typeInfo()->type();
    if ( type == typeid( kvs::UInt8 ) )
    {
        data_format = GL_ALPHA8;
        data_type = GL_UNSIGNED_BYTE;
        data_value = volume->values();
    }
    else if ( type == typeid( kvs::UInt16 ) )
    {
        data_format = GL_ALPHA16;
        data_type = GL_UNSIGNED_SHORT;
        data_value = volume->values();
    }
    else if ( type == typeid( kvs::Int8 ) )
    {
        data_format = GL_ALPHA8;
        data_type = GL_UNSIGNED_BYTE;
        data_value = ::SignedToUnsigned<kvs::UInt8,kvs::Int8>( volume );
    }
    else if ( type == typeid( kvs::Int16 ) )
    {
        data_format = GL_ALPHA16;
        data_type = GL_UNSIGNED_SHORT;
        data_value = ::SignedToUnsigned<kvs::UInt16,kvs::Int16>( volume );
    }
    else if ( type == typeid( kvs::UInt32 ) )
    {
        data_format = GL_ALPHA;
        data_type = GL_FLOAT;
        kvs::Real32 min_value = static_cast<kvs::Real32>( volume->minValue() );
        kvs::Real32 max_value = static_cast<kvs::Real32>( volume->maxValue() );
        if ( tfunc.hasRange() )
        {
            min_value = tfunc.colorMap().minValue();
            max_value = tfunc.colorMap().maxValue();
        }
        data_value = ::NormalizeValues<kvs::UInt32>( volume, min_value, max_value );
    }
    else if ( type == typeid( kvs::Int32 ) )
    {
        data_format = GL_ALPHA;
        data_type = GL_FLOAT;
        kvs::Real32 min_value = static_cast<kvs::Real32>( volume->minValue() );
        kvs::Real32 max_value = static_cast<kvs::Real32>( volume->maxValue() );
        if ( tfunc.hasRange() )
        {
            min_value = tfunc.colorMap().minValue();
            max_value = tfunc.colorMap().maxValue();
        }
        data_value = ::NormalizeValues<kvs::Int32>( volume, min_value, max_value );
    }
    else if ( type == typeid( kvs::Real32 ) )
    {
        data_format = GL_ALPHA;
        data_type = GL_FLOAT;
        kvs::Real32 min_value = static_cast<kvs::Real32>( volume->minValue() );
        kvs::Real32 max_value = static_cast<kvs::Real32>( volume->maxValue() );
        if ( tfunc.hasRange() )
        {
            min_value = tfunc.colorMap().minValue();
            max_value = tfunc.colorMap().maxValue();
        }
        data_value = ::NormalizeValues<kvs::Real32>( volume, min_value, max_value );
    }
    else if ( type == typeid( kvs::Real64 ) )
    {
        data_format = GL_ALPHA;
        data_type = GL_FLOAT;
        kvs::Real32 min_value = static_cast<kvs::Real32>( volume->minValue() );
        kvs::Real32 max_value = static_cast<kvs::Real32>( volume->maxValue() );
        if ( tfunc.hasRange() )
        {
            min_value = tfunc.colorMap().minValue();
            max_value = tfunc.colorMap().maxValue();
        }
        data_value = ::NormalizeValues<kvs::Real64>( volume, min_value, max_value );
    }
    else
    {
        kvsMessageError( "Not supported data type '%s'.",
                         volume->values().typeInfo()->typeName() );
    }

    m_manager.setPixelFormat( data_format, GL_ALPHA, data_type );
    m_manager.setWrapS( GL_CLAMP_TO_BORDER );
    m_manager.setWrapT( GL_CLAMP_TO_BORDER );
    m_manager.setWrapR( GL_CLAMP_TO_BORDER );
    m_manager.setMagFilter( GL_LINEAR );
    m_manager.setMinFilter( GL_LINEAR );
    m_manager.create( width, height, depth, data_value.data() );
}

/*===========================================================================*/
/**
 *  @brief  Draw a volume buffer object.
 */
/*===========================================================================*/
void RayCastingRenderer::BufferObject::draw()
{
    kvs::OpenGL::Disable( GL_DEPTH_TEST );
    kvs::OpenGL::Disable( GL_LIGHTING );

    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( 0, 1, 0, 1, -1, 1 );
            kvs::OpenGL::Begin( GL_QUADS );
            kvs::OpenGL::Color( kvs::Vec3( 1.0, 1.0, 1.0 ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1, 1 ), kvs::Vec2( 1, 1 ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0, 1 ), kvs::Vec2( 0, 1 ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0, 0 ), kvs::Vec2( 0, 0 ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1, 0 ), kvs::Vec2( 1, 0 ) );
            kvs::OpenGL::End();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Create a bounding cube buffer object on GPU.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void RayCastingRenderer::BoundingBufferObject::create(
    const kvs::StructuredVolumeObject* volume )
{
    /* Index number of the bounding cube.
     *
     *       4 ------------ 5
     *     / |            / |
     *    /  |           /  |
     *   7--------------6   |
     *   |   |          |   |
     *   |   0 ---------|-- 1
     *   |  /           |  /
     *   | /            | /
     *   3 ------------ 2
     *
     */
    const kvs::Vec3u min( 0, 0, 0 );
    const kvs::Vec3u max( volume->resolution() - kvs::Vec3u( 1, 1, 1 ) );
    const size_t nelements = 72; // = 4 vertices x 3 dimensions x 6 faces

    const float minx = static_cast<float>( min.x() );
    const float miny = static_cast<float>( min.y() );
    const float minz = static_cast<float>( min.z() );
    const float maxx = static_cast<float>( max.x() );
    const float maxy = static_cast<float>( max.y() );
    const float maxz = static_cast<float>( max.z() );

    const float coords[ nelements ] = {
        minx, miny, minz, // 0
        maxx, miny, minz, // 1
        maxx, miny, maxz, // 2
        minx, miny, maxz, // 3

        minx, maxy, maxz, // 7
        maxx, maxy, maxz, // 6
        maxx, maxy, minz, // 5
        minx, maxy, minz, // 4

        minx, maxy, minz, // 4
        maxx, maxy, minz, // 5
        maxx, miny, minz, // 1
        minx, miny, minz, // 0

        maxx, maxy, minz, // 5
        maxx, maxy, maxz, // 6
        maxx, miny, maxz, // 2
        maxx, miny, minz, // 1

        maxx, maxy, maxz, // 6
        minx, maxy, maxz, // 7
        minx, miny, maxz, // 3
        maxx, miny, maxz, // 2

        minx, miny, minz, // 0
        minx, miny, maxz, // 3
        minx, maxy, maxz, // 7
        minx, maxy, minz  // 4
    };

    kvs::VertexBufferObjectManager::VertexBuffer vertex_array;
    vertex_array.type = GL_FLOAT;
    vertex_array.size = sizeof( float ) * nelements;
    vertex_array.dim = 3;
    vertex_array.pointer = coords;
    m_manager.setVertexArray( vertex_array );
    m_manager.create();
}

/*===========================================================================*/
/**
 *  @brief  Draw a bounding cube buffer object.
 */
/*===========================================================================*/
void RayCastingRenderer::BoundingBufferObject::draw()
{
    kvs::VertexBufferObjectManager::Binder binder( m_manager );
    m_manager.drawArrays( GL_QUADS, 0, 72 );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new RayCastingRenderer class.
 */
/*===========================================================================*/
RayCastingRenderer::RayCastingRenderer():
    m_draw_front_face( true ),
    m_draw_back_face( true ),
    m_draw_volume( true ),
    m_enable_jittering( false ),
    m_step( 0.5f ),
    m_opaque( 1.0f )
{
    BaseClass::setShader( kvs::Shader::Lambert() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new RayCastingRenderer class.
 *  @param  tfunc [in] transfer function
 */
/*===========================================================================*/
RayCastingRenderer::RayCastingRenderer( const kvs::TransferFunction& tfunc ):
    m_draw_front_face( true ),
    m_draw_back_face( true ),
    m_draw_volume( true ),
    m_enable_jittering( false ),
    m_step( 0.5f ),
    m_opaque( 1.0f )
{
    BaseClass::setTransferFunction( tfunc );
    BaseClass::setShader( kvs::Shader::Lambert() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new RayCastingRenderer class.
 *  @param  shader [in] shader
 */
/*===========================================================================*/
template <typename ShadingType>
RayCastingRenderer::RayCastingRenderer( const ShadingType shader ):
    m_draw_front_face( true ),
    m_draw_back_face( true ),
    m_draw_volume( true ),
    m_enable_jittering( false ),
    m_step( 0.5f ),
    m_opaque( 1.0f )
{
    BaseClass::setShader( shader );
}

/*===========================================================================*/
/**
 *  @brief  Main rendering routine.
 *  @param  object [i] pointer to the object
 *  @param  camera [i] pointer to the camera
 *  @param  light [i] pointer to the light
 */
/*===========================================================================*/
void RayCastingRenderer::exec(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    const size_t width = camera->windowWidth();
    const size_t height = camera->windowHeight();
    auto* volume = kvs::StructuredVolumeObject::DownCast( object );

    if ( BaseClass::isWindowCreated() )
    {
        BaseClass::setWindowSize( width, height );
        BaseClass::setDevicePixelRatio( camera->devicePixelRatio() );
        BaseClass::setObject( volume );
        const size_t framebuffer_width = BaseClass::framebufferWidth();
        const size_t framebuffer_height = BaseClass::framebufferHeight();
        this->create_shader_program( BaseClass::shader(), BaseClass::isEnabledShading() );
        this->create_framebuffer( framebuffer_width, framebuffer_height );
        this->create_buffer_object( volume );
    }

    if ( BaseClass::isWindowResized( width, height ) )
    {
        BaseClass::setWindowSize( width, height );
        const size_t framebuffer_width = BaseClass::framebufferWidth();
        const size_t framebuffer_height = BaseClass::framebufferHeight();
        this->update_framebuffer( framebuffer_width, framebuffer_height );
    }

    if ( BaseClass::isObjectChanged( volume ) )
    {
        BaseClass::setObject( volume );
        const size_t framebuffer_width = BaseClass::framebufferWidth();
        const size_t framebuffer_height = BaseClass::framebufferHeight();
        this->update_shader_program( BaseClass::shader(), BaseClass::isEnabledShading() );
        this->update_framebuffer( framebuffer_width, framebuffer_height );
        this->update_buffer_object( volume );
    }

    if ( !m_transfer_function_texture.isValid() )
    {
        const size_t width = BaseClass::transferFunction().resolution();
        const auto table = BaseClass::transferFunction().table();
        m_transfer_function_texture.release();
        m_transfer_function_texture.setWrapS( GL_CLAMP_TO_EDGE );
        m_transfer_function_texture.setMagFilter( GL_LINEAR );
        m_transfer_function_texture.setMinFilter( GL_LINEAR );
        m_transfer_function_texture.setPixelFormat( GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT  );
        m_transfer_function_texture.create( width, table.data() );
    }

    this->setup_shader_program( BaseClass::shader(), object, camera, light );

    if ( m_draw_volume )
    {
        m_ray_casting_shader.bind();
        this->draw_buffer_object( volume );
        m_ray_casting_shader.unbind();
    }

    kvs::OpenGL::Finish();
    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Sets drawing buffer.
 *  @param  drawing_buffer [in] drawing buffer (front, back or volume)
 */
/*===========================================================================*/
void RayCastingRenderer::setDrawingBuffer( const RayCastingRenderer::DrawingBuffer drawing_buffer )
{
    m_draw_front_face = false;
    m_draw_back_face = false;
    m_draw_volume = false;

    switch ( drawing_buffer )
    {
    case RayCastingRenderer::FrontFace: m_draw_front_face = true; break;
    case RayCastingRenderer::BackFace: m_draw_back_face = true; break;
    case RayCastingRenderer::Volume:
    {
        m_draw_front_face = true;
        m_draw_back_face = true;
        m_draw_volume = true;
        break;
    }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets tranfer function.
 *  @param  tfunc [in] transfer function
 */
/*===========================================================================*/
void RayCastingRenderer::setTransferFunction( const kvs::TransferFunction& tfunc )
{
     BaseClass::setTransferFunction( tfunc );

     if ( m_transfer_function_texture.isLoaded() )
     {
         m_transfer_function_texture.release();
     }
}

void RayCastingRenderer::create_shader_program(
    const kvs::Shader::ShadingModel& shading_model,
    const bool shading_enabled )
{
    // Build bounding cube shader.
    {
        kvs::ShaderSource vert( "RC_bounding_cube.vert" );
        kvs::ShaderSource frag( "RC_bounding_cube.frag" );
        m_bounding_cube_shader.build( vert, frag );
    }

    // Build ray caster.
    {
        kvs::ShaderSource vert( "RC_ray_caster.vert" );
        kvs::ShaderSource frag( "RC_ray_caster.frag" );
#if defined( _TEXTURE_RECTANGLE_ )
        frag.define("ENABLE_TEXTURE_RECTANGLE");
#endif
        if ( m_enable_jittering ) frag.define( "ENABLE_JITTERING" );
        if ( shading_enabled )
        {
            switch ( shading_model.type() )
            {
            case kvs::Shader::LambertShading: frag.define( "ENABLE_LAMBERT_SHADING" ); break;
            case kvs::Shader::PhongShading: frag.define( "ENABLE_PHONG_SHADING" ); break;
            case kvs::Shader::BlinnPhongShading: frag.define( "ENABLE_BLINN_PHONG_SHADING" ); break;
            default: /* NO SHADING */ break;
            }
        }
        m_ray_casting_shader.build( vert, frag );

        m_ray_casting_shader.bind();
        m_ray_casting_shader.setUniform( "sampling_step", m_step );
        m_ray_casting_shader.setUniform( "opaque", m_opaque );
        m_ray_casting_shader.unbind();
    }
}

void RayCastingRenderer::update_shader_program(
    const kvs::Shader::ShadingModel& shading_model,
    const bool shading_enabled )
{
    m_bounding_cube_shader.release();
    m_ray_casting_shader.release();
    this->create_shader_program( shading_model, shading_enabled );
}

void RayCastingRenderer::setup_shader_program(
    const kvs::Shader::ShadingModel& shading_model,
    const kvs::ObjectBase* object,
    const kvs::Camera* camera,
    const kvs::Light* light )
{
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_CULL_FACE );
    kvs::OpenGL::Disable( GL_LIGHTING );

    // Copy the depth and color buffer to each corresponding textures.
    const size_t framebuffer_width = BaseClass::framebufferWidth();
    const size_t framebuffer_height = BaseClass::framebufferHeight();
    {
        kvs::Texture::Binder unit6( m_depth_texture, 5 );
        m_depth_texture.loadFromFrameBuffer( 0, 0, framebuffer_width, framebuffer_height );

        kvs::Texture::Binder unit7( m_color_texture, 6 );
        m_color_texture.loadFromFrameBuffer( 0, 0, framebuffer_width, framebuffer_height );
    }

    // OpenGL variables.
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM_inverse = PM.inverted();

    // Draw the bounding cube to setup entry/exit textures.
    {
        kvs::ProgramObject::Binder bind( m_bounding_cube_shader );
        m_bounding_cube_shader.setUniform( "ModelViewProjectionMatrix", PM );

        // Change renderig target to the entry/exit FBO.
        kvs::FrameBufferObject::GuardedBinder binder( m_entry_exit_framebuffer );
        if ( m_draw_back_face )
        {
            // Draw the back face of the bounding cube for the entry points.
            kvs::OpenGL::SetDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
            kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            kvs::OpenGL::SetCullFace( GL_FRONT );
            m_bounding_cube_buffer.draw();
        }
        if ( m_draw_front_face )
        {
            // Draw the front face of the bounding cube for the entry points.
            kvs::OpenGL::SetDrawBuffer( GL_COLOR_ATTACHMENT1_EXT );
            kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            kvs::OpenGL::SetCullFace( GL_BACK );
            m_bounding_cube_buffer.draw();
        }
    }

    kvs::OpenGL::Disable( GL_CULL_FACE );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

    if ( BaseClass::isEnabledShading() ) kvs::OpenGL::Enable( GL_LIGHTING );
    else kvs::OpenGL::Disable( GL_LIGHTING );

    kvs::ProgramObject::Binder bind( m_ray_casting_shader );
    m_ray_casting_shader.setUniform( "ModelViewProjectionMatrix", PM );
    m_ray_casting_shader.setUniform( "ModelViewProjectionMatrixInverse", PM_inverse );

    const float f = camera->back();
    const float n = camera->front();
    const float to_zw1 = ( f * n ) / ( f - n );
    const float to_zw2 = 0.5f * ( ( f + n ) / ( f - n ) ) + 0.5f;
    const float to_ze1 = 0.5f + 0.5f * ( ( f + n ) / ( f - n ) );
    const float to_ze2 = ( f - n ) / ( f * n );
    const kvs::Vec3 light_position = kvs::WorldCoordinate( light->position() ).toObjectCoordinate( object ).position();
    const kvs::Vec3 camera_position = kvs::WorldCoordinate( camera->position() ).toObjectCoordinate( object ).position();
    m_ray_casting_shader.setUniform( "to_zw1", to_zw1 );
    m_ray_casting_shader.setUniform( "to_zw2", to_zw2 );
    m_ray_casting_shader.setUniform( "to_ze1", to_ze1 );
    m_ray_casting_shader.setUniform( "to_ze2", to_ze2 );
    m_ray_casting_shader.setUniform( "light_position", light_position );
    m_ray_casting_shader.setUniform( "camera_position", camera_position );
    m_ray_casting_shader.setUniform( "shading.Ka", shading_model.Ka );
    m_ray_casting_shader.setUniform( "shading.Kd", shading_model.Kd );
    m_ray_casting_shader.setUniform( "shading.Ks", shading_model.Ks );
    m_ray_casting_shader.setUniform( "shading.S",  shading_model.S );
}

void RayCastingRenderer::create_framebuffer( const size_t width, const size_t height )
{
    const size_t size = 32;
    const auto rnd = kvs::ValueArray<kvs::UInt8>::Random( size * size );
    m_jittering_texture.setWrapS( GL_REPEAT );
    m_jittering_texture.setWrapT( GL_REPEAT );
    m_jittering_texture.setMagFilter( GL_NEAREST );
    m_jittering_texture.setMinFilter( GL_NEAREST );
    m_jittering_texture.setPixelFormat( GL_LUMINANCE8, GL_LUMINANCE, GL_UNSIGNED_BYTE  );
    m_jittering_texture.create( size, size, rnd.data() );

    m_depth_texture.setWrapS( GL_CLAMP_TO_BORDER );
    m_depth_texture.setWrapT( GL_CLAMP_TO_BORDER );
    m_depth_texture.setMagFilter( GL_LINEAR );
    m_depth_texture.setMinFilter( GL_LINEAR );
    m_depth_texture.setPixelFormat( GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT  );
    m_depth_texture.create( width, height );

    m_color_texture.setWrapS( GL_CLAMP_TO_BORDER );
    m_color_texture.setWrapT( GL_CLAMP_TO_BORDER );
    m_color_texture.setMagFilter( GL_LINEAR );
    m_color_texture.setMinFilter( GL_LINEAR );
    m_color_texture.setPixelFormat( GL_RGBA32F_ARB, GL_RGB, GL_FLOAT  );
    m_color_texture.create( width, height );

    m_entry_texture.setWrapS( GL_CLAMP_TO_BORDER );
    m_entry_texture.setWrapT( GL_CLAMP_TO_BORDER );
    m_entry_texture.setMagFilter( GL_LINEAR );
    m_entry_texture.setMinFilter( GL_LINEAR );
    m_entry_texture.setPixelFormat( GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT );
    m_entry_texture.create( width, height );

    m_exit_texture.setWrapS( GL_CLAMP_TO_BORDER );
    m_exit_texture.setWrapT( GL_CLAMP_TO_BORDER );
    m_exit_texture.setMagFilter( GL_LINEAR );
    m_exit_texture.setMinFilter( GL_LINEAR );
    m_exit_texture.setPixelFormat( GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT  );
    m_exit_texture.create( width, height );

    m_entry_exit_framebuffer.create();
    m_entry_exit_framebuffer.attachColorTexture( m_exit_texture, 0 );
    m_entry_exit_framebuffer.attachColorTexture( m_entry_texture, 1 );

    m_ray_casting_shader.bind();
    m_ray_casting_shader.setUniform( "width", static_cast<GLfloat>( width ) );
    m_ray_casting_shader.setUniform( "height", static_cast<GLfloat>( height ) );
    m_ray_casting_shader.unbind();
}

void RayCastingRenderer::update_framebuffer( const size_t width, const size_t height )
{
    m_jittering_texture.release();
    m_depth_texture.release();
    m_color_texture.release();
    m_entry_texture.release();
    m_exit_texture.release();
    m_entry_exit_framebuffer.release();
    this->create_framebuffer( width, height );
}

void RayCastingRenderer::create_buffer_object( const kvs::StructuredVolumeObject* volume )
{
    m_volume_buffer.create( volume, BaseClass::transferFunction() );
    m_bounding_cube_buffer.create( volume );

    // Set uniform variables.
    const kvs::Vec3 r( volume->resolution() );
    const kvs::Real32 max_ngrids = kvs::Math::Max( r.x(), r.y(), r.z() );
    const kvs::Vec3 ratio( r.x() / max_ngrids, r.y() / max_ngrids, r.z() / max_ngrids );
    const kvs::Vec3 reciprocal( 1.0f / r.x(), 1.0f / r.y(), 1.0f / r.z() );
    kvs::Real32 min_range = 0.0f;
    kvs::Real32 max_range = 0.0f;
    kvs::Real32 min_value = BaseClass::transferFunction().colorMap().minValue();
    kvs::Real32 max_value = BaseClass::transferFunction().colorMap().maxValue();
    const std::type_info& type = volume->values().typeInfo()->type();
    if ( type == typeid( kvs::UInt8 ) )
    {
        min_range = 0.0f;
        max_range = 255.0f;
        if ( !BaseClass::transferFunction().hasRange() )
        {
            min_value = 0.0f;
            max_value = 255.0f;
        }
    }
    else if ( type == typeid( kvs::Int8 ) )
    {
        min_range = static_cast<kvs::Real32>( kvs::Value<kvs::UInt8>::Min() );
        max_range = static_cast<kvs::Real32>( kvs::Value<kvs::UInt8>::Max() );
        if ( !BaseClass::transferFunction().hasRange() )
        {
            min_value = -128.0f;
            max_value = 127.0f;
        }
    }
    else if ( type == typeid( kvs::UInt16 ) )
    {
        min_range = static_cast<kvs::Real32>( kvs::Value<kvs::UInt16>::Min() );
        max_range = static_cast<kvs::Real32>( kvs::Value<kvs::UInt16>::Max() );
        if ( !BaseClass::transferFunction().hasRange() )
        {
            min_value = static_cast<kvs::Real32>( volume->minValue() );
            max_value = static_cast<kvs::Real32>( volume->maxValue() );
        }
    }
    else if ( type == typeid( kvs::Int16 ) )
    {
        min_range = static_cast<kvs::Real32>( kvs::Value<kvs::Int16>::Min() );
        max_range = static_cast<kvs::Real32>( kvs::Value<kvs::Int16>::Max() );
        if ( !BaseClass::transferFunction().hasRange() )
        {
            min_value = static_cast<kvs::Real32>( volume->minValue() );
            max_value = static_cast<kvs::Real32>( volume->maxValue() );
        }
    }
    else if ( type == typeid( kvs::UInt32 ) ||
              type == typeid( kvs::Int32  ) ||
              type == typeid( kvs::Real32 ) ||
              type == typeid( kvs::Real64 ) )
    {
        min_range = 0.0f;
        max_range = 1.0f;
        min_value = 0.0f;
        max_value = 1.0f;
    }
    else
    {
        kvsMessageError( "Not supported data type '%s'.",
                         volume->values().typeInfo()->typeName() );
    }

    m_ray_casting_shader.bind();
    m_ray_casting_shader.setUniform( "volume.resolution", r );
    m_ray_casting_shader.setUniform( "volume.resolution_ratio", ratio );
    m_ray_casting_shader.setUniform( "volume.resolution_reciprocal", reciprocal );
    m_ray_casting_shader.setUniform( "volume.min_range", min_range );
    m_ray_casting_shader.setUniform( "volume.max_range", max_range );
    m_ray_casting_shader.setUniform( "transfer_function.min_value", min_value );
    m_ray_casting_shader.setUniform( "transfer_function.max_value", max_value );
    m_ray_casting_shader.unbind();
}

void RayCastingRenderer::update_buffer_object( const kvs::StructuredVolumeObject* volume )
{
    m_volume_buffer.release();
    m_bounding_cube_buffer.release();
    this->create_buffer_object( volume );
}

void RayCastingRenderer::draw_buffer_object( const kvs::StructuredVolumeObject* volume )
{
    kvs::Texture::Binder unit1( m_volume_buffer.manager(), 0 );
    kvs::Texture::Binder unit2( m_exit_texture, 1 );
    kvs::Texture::Binder unit3( m_entry_texture, 2 );
    kvs::Texture::Binder unit4( m_transfer_function_texture, 3 );
    kvs::Texture::Binder unit5( m_jittering_texture, 4 );
    kvs::Texture::Binder unit6( m_depth_texture, 5 );
    kvs::Texture::Binder unit7( m_color_texture, 6 );

    m_ray_casting_shader.setUniform( "volume_data", 0 );
    m_ray_casting_shader.setUniform( "exit_points", 1 );
    m_ray_casting_shader.setUniform( "entry_points", 2 );
    m_ray_casting_shader.setUniform( "transfer_function_data", 3 );
    m_ray_casting_shader.setUniform( "jittering_texture", 4 );
    m_ray_casting_shader.setUniform( "depth_texture", 5 );
    m_ray_casting_shader.setUniform( "color_texture", 6 );

    m_volume_buffer.draw();
}

} // end of namespace glsl

} // end of namespace kvs
