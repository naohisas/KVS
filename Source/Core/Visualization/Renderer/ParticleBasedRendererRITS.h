/*****************************************************************************/
/**
 *  @file   ParticleBasedRendererRITS.h
 *  @author Makoto Uemura, Satoshi Tanaka
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
#ifndef KVS__GLSL__RITS__PARTICLE_BASED_RENDERER_H_INCLUDE
#define KVS__GLSL__RITS__PARTICLE_BASED_RENDERER_H_INCLUDE

#include <kvs/VolumeRendererBase>
#include <kvs/PointObject>
#include <kvs/Module>
#include <kvs/Texture2D>
#include <kvs/RenderBuffer>
#include <kvs/VertexBufferObject>
#include <kvs/FrameBufferObject>
#include <kvs/FragmentShader>
#include <kvs/VertexShader>
#include <kvs/ProgramObject>
//#include <kvs/ParticleVolumeRenderer>
//#include "EnsembleAverageBuffer.h" // SAKAMOTO-TANAKA


namespace kvs
{

namespace glsl
{

namespace rits
{

/*===========================================================================*/
/**
 *  @brief  Ensemble average buffer class.
 */
/*===========================================================================*/
class EnsembleAverageBuffer
{
protected:

    bool m_enable_accum; ///< check flag for the accumulation buffer
    size_t m_width; ///< buffer width
    size_t m_height; ///< buffer height
    size_t m_count; ///< number of accumulations
    kvs::Texture2D m_texture; ///< texture for the repetition
    kvs::FrameBufferObject m_framebuffer; ///< Frame Buffer Object
    float m_accum_scale; ///< accumulation scale

public:

    EnsembleAverageBuffer( const bool use_accum = false );

    void create( const size_t width, const size_t height );
    void clear();
    void bind();
    void unbind();
    void add();
    void draw();
    void enableAccumulationBuffer();
    void disableAccumulationBuffer();
    bool isEnabledAccumulationBuffer();
    size_t count() const;
    float opacity() const;

protected:

    void draw_quad( const float r, const float g, const float b, const float a );
};

#define KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_BYTE // *_BYTE or *_FLOAT

/*===========================================================================*/
/**
 *  @brief  Particle based renderer class developed by Ritsumeikan University.
 *  @author Makoto Uemura, Satoshi Tanaka
 */
/*===========================================================================*/
class ParticleBasedRenderer : public kvs::VolumeRendererBase
{
    kvsModule( kvs::glsl::rits::ParticleBasedRenderer, Renderer );
    kvsModuleBaseClass( kvs::VolumeRendererBase );

protected:

    class Particles;
    class Renderer;

protected:

    typedef GLushort IndexType;
    typedef GLubyte ColorType;
    typedef GLfloat CoordType;
#if defined( KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_FLOAT )
    typedef GLfloat NormalType;
#elif defined( KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_BYTE )
    typedef GLbyte NormalType;
#else
#error "KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_* is not defined."
#endif

    // Reference data (NOTE: not allocated in thie class).
    const kvs::PointObject* m_ref_point; ///< pointer to the point data

    size_t m_subpixel_level; ///< subpixel level
    size_t m_repetition_level; ///< repetition level
    size_t m_coarse_level; ///< repetition level for coarse rendering (LOD)
    float m_zooming_factor; ///< zooming factor
    size_t m_random_texture_size; ///< size of the random number texture
    size_t m_circle_threshold; ///< threshold for the shape of the particle
    size_t m_lower_limit_level; ///< lower limit of repeat level for repeat level zooming ADD_UEMURA
    size_t m_render_width; ///< extended window width for subpixel processing
    size_t m_render_height; ///< extended window height for subpixel processing
    bool m_enable_pre_downloading; ///< flag whether to donwonload the particles first
    bool m_enable_accumulation_buffer; ///< flag whether to use the accumulation buffer
    bool m_enable_random_texture; ///< flag whether to use the random number texture
    EnsembleAverageBuffer m_ensemble_buffer; ///< ensemble average buffer
    kvs::Texture2D m_random_texture; ///< random number texture
    kvs::ProgramObject m_zoom_shader; ///< shader (program object) for zooming
    kvs::ProgramObject m_resize_shader; ///< shader (program object) for resizing
    GLint m_loc_point_identifier; ///<
    kvs::FrameBufferObject m_resize_framebuffer; ///< RGB buffer for subpixel processing
    kvs::RenderBuffer m_resize_depthbuffer; ///< depth buffer for subpixel processing
    kvs::Texture2D m_resize_texture; ///< texture for subpixel processing
    kvs::VertexBufferObject* m_vbo; ///< vertex buffer object (VBO) for the particles
    Particles* m_particles; ///< particles on GPU
    Renderer* m_renderer; ///< renderer for VBO
    bool m_enable_lod; ///< enable LOD rendering
    bool m_enable_zooming; ///< enable zooming
    bool m_enable_repetition_level_zooming; //flag whether to use repetition level zooming ADD_UEMURA
    bool m_enable_hyblid_zooming;//ADD_UEMURA
    bool m_enable_shuffle; ///< enable shuffle ADD_UEMURA
    float  m_modelview_matrix[16]; ///< modelview matrix

public:

    ParticleBasedRenderer();
    ParticleBasedRenderer( kvs::PointObject* point, const size_t subpixel_level = 1, const size_t repeat_level = 1 );
    virtual ~ParticleBasedRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void initialize();
    void attachPointObject( const kvs::PointObject* point );
    void setSubpixelLevel( const size_t subpixel_level );
    void setRepetitionLevel( const size_t repetition_level );
    void setRandomTextureSize( const size_t random_texture_size );
    void setCircleThreshold( const size_t circle_threshold );
    void setMinRepetitionLevelInZooming(const size_t lower_limit_level );//ADD_UEMURA
    void enableLODControl( const size_t coarse_level = 1 );
    void disableLODControl();
    void enableZooming();
    void enableParticleZooming(){enableZooming();}; //same as enableZooming() ADD_UEMURA
    void enableRepetitionLevelZooming();//ADD_UEMURA
    void enableShuffle();//ADD_UEMURA
    void disableZooming();
    void disableParticleZooming(){disableZooming();}; //same as disableZooming() ADD_UEMURA
    void disableRepetitionLevelZooming();//ADD_UEMURA
    void disableShuffle();//ADD_UEMURA
    void enableCoarseRendering( const size_t coarse_level = 1 );
    void disableCoarseRendering();
    void enableAccumulationBuffer();
    void disableAccumulationBuffer();
    void enableRandomTexture();
    void disableRandomTexture();

    size_t subpixelLevel() const;
    size_t repetitionLevel() const;
    size_t randomTextureSize() const;
    size_t circleThreshold() const;
    bool isEnabledAccumulationBuffer() const;
    bool isEnabledRandomTexture() const;
    bool isHyblidZooming(){ return m_enable_zooming && m_enable_repetition_level_zooming; };//ADD_UEMURA

protected:

    void create_image( const kvs::PointObject* point, const kvs::Camera* camera, const kvs::Light* light );
    void initialize_opengl();
    void initialize_resize_texture();
    void create_random_texture();
    void align_particles();
    void create_vertexbuffer();
    void download_vertexbuffer( Renderer& renderer, kvs::VertexBufferObject& vbo );
    void draw_vertexbuffer( const Renderer& renderer, kvs::VertexBufferObject& vbo, const float modelview_matrix[16] );
    void calculate_zooming_factor( const kvs::PointObject* point, const kvs::Camera* camera );
    void setup_zoom_shader( const float modelview_matrix[16] );
    void setup_resize_shader();

}; // end of class

/*===========================================================================*/
/**
 *  @brief  Particle class for PBVR.
 */
/*===========================================================================*/
class ParticleBasedRenderer::Particles
{
private:

    size_t m_nvertices; ///< number of vertices
    ParticleBasedRenderer::IndexType* m_indices; ///< index array
    ParticleBasedRenderer::CoordType* m_coords; ///< coordinate value array
    ParticleBasedRenderer::NormalType* m_normals; ///< normal vector array
    ParticleBasedRenderer::ColorType* m_colors; ///< color value array

public:

    Particles();
    ~Particles();

    void release();
    void create( const size_t nvertices, const bool has_normal, const bool has_index );

    bool hasIndex() const;
    bool hasNormal() const;
    size_t nvertices() const;
    size_t byteSizePerVertex() const;
    size_t byteSize() const;
    const ParticleBasedRenderer::IndexType* indices() const;
    const ParticleBasedRenderer::CoordType* coords() const;
    const ParticleBasedRenderer::NormalType* normals() const;
    const ParticleBasedRenderer::ColorType* colors() const;
    ParticleBasedRenderer::IndexType* indices();
    ParticleBasedRenderer::CoordType* coords();
    ParticleBasedRenderer::NormalType* normals();
    ParticleBasedRenderer::ColorType* colors();
};

/*===========================================================================*/
/**
 *  @brief  VBO rendering information class
 */
/*===========================================================================*/
class ParticleBasedRenderer::Renderer
{
    const ParticleBasedRenderer::Particles* m_particles; //< pointer to the particles
    size_t m_start; ///< start number of input vertices
    size_t m_count; ///< number of vertices
    size_t m_off_index; ///< offset bytes for the index array
    size_t m_off_coord; ///< offset bytes for the coodinate value array
    size_t m_off_normal; ///< offset bytes for the normal vector array
    size_t m_off_color; ///< offset bytes for the color value array
    size_t m_loc_identifier; ///<

public:

    Renderer();

    void set( const ParticleBasedRenderer::Particles* particles, const size_t start, const size_t count, const size_t loc_identifier );
    size_t download( kvs::VertexBufferObject& vbo );
    void draw() const;
    size_t nvertices() const;
    size_t byteSizePerVertex() const;
    size_t byteSize() const;
};

} // end of namespace rits

} // end of namespace glsl

} // end of namespace kvs

#endif // KVS__GLSL__RITS__PARTICLE_BASED_RENDERER_H_INCLUDE
