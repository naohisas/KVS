/****************************************************************************/
/**
 *  @file   VisualizationPipeline.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <string>
#include <list>
#include <kvs/Indent>
#include <kvs/ObjectBase>
#include <kvs/GeometryObjectBase>
#include <kvs/VolumeObjectBase>
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/PipelineModule>


namespace kvs
{

/*==========================================================================*/
/**
 *  Visualization pipeline class.
 */
/*==========================================================================*/
class VisualizationPipeline
{
public:
    using ModuleList = std::list<kvs::PipelineModule>;

private:
    size_t m_id = 0; ///< pipeline ID
    std::string m_filename{""}; ///< filename
    bool m_cache = true; ///< cache mode (DISABLE NOW)
    ModuleList m_module_list{}; ///< pipeline module list

    const kvs::ObjectBase* m_object = nullptr; ///< pointer to the object inserted to the manager
    const kvs::RendererBase* m_renderer = nullptr; ///< pointer to the renderer inserted to the manager

private:
    VisualizationPipeline();

public:
    explicit VisualizationPipeline( const std::string& filename );
    explicit VisualizationPipeline( kvs::ObjectBase* object );
    virtual ~VisualizationPipeline();

    VisualizationPipeline& connect( kvs::PipelineModule& module );
    bool import();
    bool exec();

    bool cache() const { return m_cache; }
    void enableCache() { m_cache = true; }
    void disableCache() { m_cache = false; }
    bool hasObject() const { return m_object != nullptr; }
    bool hasRenderer() const;
    const kvs::ObjectBase* object() const { return m_object; }
    const kvs::RendererBase* renderer() const { return m_renderer; }
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;

    friend std::string& operator << ( std::string& str, const VisualizationPipeline& pipeline );
    friend std::ostream& operator << ( std::ostream& os, const VisualizationPipeline& pipeline );

private:
    bool create_renderer_module( const kvs::ObjectBase* object );
    bool create_renderer_module( const kvs::GeometryObjectBase* geometry );
    bool create_renderer_module( const kvs::VolumeObjectBase* volume );
    ModuleList::iterator find_module( const kvs::PipelineModule::Category category );
    size_t count_module( const kvs::PipelineModule::Category category ) const;
};

} // end of namespace kvs
