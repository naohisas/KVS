/*****************************************************************************/
/**
 *  @file   Axis2DMatrix.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include "Axis2D.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Axis 2D matrix renderer class.
 */
/*===========================================================================*/
class Axis2DMatrix : public kvs::Axis2D
{
    kvsModule( kvs::Axis2DMatrix, Renderer );
    kvsModuleBaseClass( kvs::Axis2D );

private:
    int m_padding; ///< padding in pixels between each rectangle

public:
    Axis2DMatrix(): m_padding( 20 ) {}

    void setPadding( const int padding ) { m_padding = padding; }
    int padding() const { return m_padding; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void setup_axes( const kvs::TableObject* table );
    void draw_axes( const kvs::Rectangle& rect, const size_t dim, const size_t x_index, const size_t y_index );

public:
    KVS_DEPRECATED( void setMargin( const int margin ) ) { this->setPadding( margin ); }
    KVS_DEPRECATED( int margin() const ) { return this->padding(); }
};

} // end of namespace kvs
