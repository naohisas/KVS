/****************************************************************************/
/**
 *  @file   VisualizationPipeline.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VisualizationPipeline.h 1720 2014-03-12 14:22:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__VISUALIZATION_PIPELINE_H_INCLUDE
#define KVS__VISUALIZATION_PIPELINE_H_INCLUDE

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

    typedef std::list<kvs::PipelineModule> ModuleList;

private:

    size_t m_id; ///< pipeline ID
    std::string m_filename; ///< filename
    bool m_cache; ///< cache mode (DISABLE NOW)
    ModuleList m_module_list; ///< pipeline module list

    const kvs::ObjectBase* m_object; ///< pointer to the object inserted to the manager
    const kvs::RendererBase* m_renderer; ///< pointer to the renderer inserted to the manager

private:

    VisualizationPipeline();

public:

    explicit VisualizationPipeline( const std::string& filename );
    explicit VisualizationPipeline( kvs::ObjectBase* object );
    virtual ~VisualizationPipeline();

    VisualizationPipeline& connect( kvs::PipelineModule& module );
    bool import();
    bool exec();

    bool cache() const;
    void enableCache();
    void disableCache();
    bool hasObject() const;
    bool hasRenderer() const;
    const kvs::ObjectBase* object() const;
    const kvs::RendererBase* renderer() const;
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

#endif // KVS__VISUALIZATION_PIPELINE_H_INCLUDE
