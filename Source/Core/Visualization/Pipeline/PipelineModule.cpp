/****************************************************************************/
/**
 *  @file   PipelineModule.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PipelineModule.cpp 1720 2014-03-12 14:22:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "PipelineModule.h"
#include <cstring>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PipelineModule class.
 */
/*===========================================================================*/
PipelineModule::PipelineModule():
    m_auto_delete( true ),
    m_counter( 0 ),
    m_category( Empty )
{
    memset( &m_module, 0, sizeof( Module ) );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PipelineModule class.
 *  @param  module [in] pipeline module
 */
/*===========================================================================*/
PipelineModule::PipelineModule( const PipelineModule& module ):
    m_auto_delete( true ),
    m_counter( 0 ),
    m_category( Empty )
{
    memset( &m_module, 0, sizeof( Module ) );
    this->shallow_copy( module );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the PipelineModule class.
 */
/*===========================================================================*/
PipelineModule::~PipelineModule()
{
    this->unref();
}

/*===========================================================================*/
/**
 *  @brief  Executes the pipeline module.
 *  @param  object [in] pointer to the object base
 *  @return pointer to the executed object
 */
/*===========================================================================*/
kvs::ObjectBase* PipelineModule::exec( const kvs::ObjectBase* object )
{
    if ( !object ) return NULL;

    switch ( m_category )
    {
    case PipelineModule::Filter: return m_module.filter->exec( object );
    case PipelineModule::Mapper: return m_module.mapper->exec( object );
    default: break;
    }

    return NULL;
}

/*===========================================================================*/
/**
 *  @brief  Copys the module.
 *  @param  module [in] module
 */
/*===========================================================================*/
PipelineModule& PipelineModule::operator = ( const PipelineModule& module )
{
    if ( this != &module )
    {
        if ( m_category == module.m_category )
        {
            this->shallow_copy( module );
        }
        else
        {
            this->deep_copy( module );
        }
    }

    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Returns the name of the module.
 *  @return name of the module
 */
/*===========================================================================*/
const char* PipelineModule::name() const
{
    switch ( m_category )
    {
    case PipelineModule::Filter: return m_module.filter->moduleName();
    case PipelineModule::Mapper: return m_module.mapper->moduleName();
    case PipelineModule::Renderer: return m_module.renderer->moduleName();
    default: break;
    }

    return "Unknown module";
}

/*===========================================================================*/
/**
 *  @brief  Check the reference status of the pipeline module.
 *  @return true if the pipeline module isn't shared
 */
/*===========================================================================*/
bool PipelineModule::unique() const
{
    return m_counter ? m_counter->value() == 1 : true;
}

/*===========================================================================*/
/**
 *  @brief  Disable function of the auto-delete.
 */
/*===========================================================================*/
void PipelineModule::disable_auto_delete()
{
    m_auto_delete = false;
}

/*===========================================================================*/
/**
 *  @brief  Deletes the pipeline module.
 */
/*===========================================================================*/
void PipelineModule::delete_module()
{
    // WARNING: If m_auto_delete is 'false', the module is not deleted in this
    // class. In this case, you have to delete the module by calling the delete_module
    // method after calling the enable_auto_delete method. However, you can only
    // call these methods in the VisualizationPipeline class since these methods
    // is defined as private method and the VisualizationPipeline class is specified
    // as friend class in this class definition.
    if ( !m_auto_delete ) return;

    switch ( m_category )
    {
    case PipelineModule::Filter: if ( m_module.filter ) delete m_module.filter; m_module.filter = NULL; break;
    case PipelineModule::Mapper: if ( m_module.mapper ) delete m_module.mapper; m_module.mapper = NULL; break;
    case PipelineModule::Renderer: if ( m_module.renderer ) delete m_module.renderer;m_module.renderer = NULL; break;
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Shallow copys.
 *  @param  module [in] pipeline module
 */
/*===========================================================================*/
void PipelineModule::shallow_copy( const PipelineModule& module )
{
    this->unref();
    m_counter = module.m_counter;
    m_category = module.m_category;
    m_module = module.m_module;
    this->ref();
}

/*===========================================================================*/
/**
 *  @brief  Deep copys.
 *  @param  module [in] pipeline module
 */
/*===========================================================================*/
void PipelineModule::deep_copy( const PipelineModule& module )
{
    this->unref();
    this->create_counter();
    m_category = module.m_category;
    m_module = module.m_module;
}

/*===========================================================================*/
/**
 *  @brief  Creates a reference counter.
 *  @param  counter [in] counter number
 */
/*===========================================================================*/
void PipelineModule::create_counter( const size_t counter )
{
    m_counter = new kvs::ReferenceCounter( counter );
    KVS_ASSERT( m_counter != NULL );
}

/*===========================================================================*/
/**
 *  @brief  Increments the reference counter.
 */
/*===========================================================================*/
void PipelineModule::ref()
{
    if ( m_counter ) { m_counter->increment(); }
}

/*===========================================================================*/
/**
 *  @brief  Decrements the reference counter.
 */
/*===========================================================================*/
void PipelineModule::unref()
{
    if ( m_counter )
    {
        m_counter->decrement();

        if ( m_counter->value() == 0 )
        {
            this->delete_module();
            if ( m_counter ) { delete m_counter; }
        }
    }

    m_counter = 0;
    m_category = PipelineModule::Empty;
}

} // end of namespace kvs
