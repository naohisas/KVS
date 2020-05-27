/*****************************************************************************/
/**
 *  @file   ScatterPlotMatrixRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/Deprecated>
#include "ScatterPlotRenderer.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  ScatterPlotMatrixRenderer class.
 */
/*===========================================================================*/
class ScatterPlotMatrixRenderer : public kvs::ScatterPlotRenderer
{
    kvsModule( kvs::ScatterPlotMatrixRenderer, Renderer );
    kvsModuleBaseClass( kvs::ScatterPlotRenderer );

private:
    int m_padding; ///< padding in pixels between each rectangle

public:
    ScatterPlotMatrixRenderer(): m_padding( 20 ) {};

    void setPadding( const int padding ) { m_padding = padding; }
    int padding() const { return m_padding; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

public:
    KVS_DEPRECATED( void setMargin( const int margin ) ) { this->setPadding( margin ); }
    KVS_DEPRECATED( int margin() const ) { return this->padding(); }
};

} // end of namespace kvs
