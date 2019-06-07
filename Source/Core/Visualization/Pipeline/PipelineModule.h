/****************************************************************************/
/**
 *  @file   PipelineModule.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PipelineModule.h 1720 2014-03-12 14:22:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__PIPELINE_MODULE_H_INCLUDE
#define KVS__PIPELINE_MODULE_H_INCLUDE

#include <cstring>
#include <kvs/FilterBase>
#include <kvs/MapperBase>
#include <kvs/ObjectBase>
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/Assert>
#include <kvs/ReferenceCounter>


namespace kvs
{

class VisualizationPipeline;

/*==========================================================================*/
/**
 *  @brief  Pipeline class.
 */
/*==========================================================================*/
class PipelineModule
{
    friend class kvs::VisualizationPipeline;

public:

    enum Category
    {
        Empty = 0, ///< empty module
        Filter, ///< filter module
        Mapper, ///< mapper module
        Renderer ///< renderer module
    };

    union Module
    {
        kvs::FilterBase* filter; ///< pointer to the KVS filter class
        kvs::MapperBase* mapper; ///< pointer to the KVS mapper class
        kvs::RendererBase* renderer; ///< pointer to the KVS renderer class
    };

protected:

    bool m_auto_delete; ///< flag whether the module is deleted or not (usually 'true')
    kvs::ReferenceCounter* m_counter;  ///< Reference counter.
    Category m_category; ///< module category
    Module m_module; ///< pointer to the module (SHARED)

public:

    PipelineModule();
    template <typename T>
    explicit PipelineModule( T* module ):
        m_auto_delete( true ),
        m_counter( 0 ),
        m_category( Empty )
    {
        memset( &m_module, 0, sizeof( Module ) );
        this->create_counter( 1 );
        this->read_module( module, typename kvs::ModuleTraits<T>::ModuleCategory() );
    }
    PipelineModule( const PipelineModule& module );
    ~PipelineModule();

    template <typename T>
    T* get() const
    {
        return( this->get_module<T>( typename kvs::ModuleTraits<T>::ModuleCategory() ) );
    }

    kvs::ObjectBase* exec( const kvs::ObjectBase* object = NULL );

    PipelineModule& operator = ( const PipelineModule& module );

    Category category() const { return m_category; }
    const Module module() const { return m_module; }
    const kvs::FilterBase* filter() const { return m_module.filter; }
    const kvs::MapperBase* mapper() const { return m_module.mapper; }
    const kvs::RendererBase* renderer() const { return m_module.renderer; }
    const char* name() const;
    bool unique() const;

private:

    template <typename T>
    void read_module( T* module, KVS_MODULE_FILTER )
    {
        m_category = PipelineModule::Filter;
        m_module.filter = module;
    }

    template <typename T>
    void read_module( T* module, KVS_MODULE_MAPPER )
    {
        m_category = PipelineModule::Mapper;
        m_module.mapper = module;
    }

    template <typename T>
    void read_module( T* module, KVS_MODULE_RENDERER )
    {
        m_category = PipelineModule::Renderer;
        m_module.renderer = module;
    }

private:

    template <typename T>
    T* get_module( KVS_MODULE_FILTER ) const
    {
        return( reinterpret_cast<T*>( m_module.filter ) );
    }

    template <typename T>
    T* get_module( KVS_MODULE_MAPPER ) const
    {
        return( reinterpret_cast<T*>( m_module.mapper ) );
    }

    template <typename T>
    T* get_module( KVS_MODULE_RENDERER ) const
    {
        return( reinterpret_cast<T*>( m_module.renderer ) );
    }

    void disable_auto_delete();
    void delete_module();
    void shallow_copy( const PipelineModule& module );
    void deep_copy( const PipelineModule& module );
    void create_counter( const size_t counter = 1 );
    void ref();
    void unref();
};

} // end of namespace kvs

#endif // KVS__PIPELINE_MODULE_H_INCLUDE
