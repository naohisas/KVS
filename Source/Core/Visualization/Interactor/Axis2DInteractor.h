/*****************************************************************************/
/**
 *  @file   Axis2DInteractor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/InteractorBase>
#include <kvs/Axis2D>
#include <kvs/Painter>
#include <kvs/TableObject>
#include <kvs/UIColor>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Axis 2D interactor class.
 */
/*===========================================================================*/
class Axis2DInteractor : public kvs::InteractorBase
{
public:
    using BaseClass = kvs::InteractorBase;

private:
    kvs::TableObject* m_table = nullptr; ///< table object
    const kvs::Axis2D* m_axis = nullptr; ///< axis 2D (reference)
    kvs::Vec2i m_p0{ 0, 0 }; ///< mouse pressing position in pixel coordinates
    kvs::Vec2i m_p1{ 0, 0 }; ///< mouse moving position in pixel coordinates
    kvs::Vec2i m_p{ 0, 0 };  ///< current mouse position in window coordinates
    bool m_range_setting = false; ///< range selecting mode if true
    bool m_range_moving = false; ///< range moving mode if true
    kvs::Painter m_painter{}; ///< painter
    kvs::Real32 m_range_width = 20.0f; ///< range bar width
    kvs::Real32 m_range_edge_width = 2.0f; ///< range bar edge width
    kvs::RGBAColor m_range_color{ kvs::UIColor::Fill() }; ///< range bar color
    kvs::RGBAColor m_range_edge_color{ kvs::UIColor::TertiaryLabel() }; ///< range bar edge color

public:
    Axis2DInteractor( const kvs::Axis2D* axis );
    virtual ~Axis2DInteractor() = default;

    kvs::Real32 rangeWidth() const { return m_range_width; }
    kvs::Real32 rangeEdgeWidth() const { return m_range_edge_width; }
    const kvs::RGBAColor& rangeEdgeColor() const { return m_range_edge_color; }
    const kvs::RGBAColor& rangeColor() const { return m_range_color; }
    void setRangeWidth( const kvs::Real32 width ) { m_range_width = width; }
    void setRangeEdgeWidth( const kvs::Real32 width ) { m_range_edge_width = width; }
    void setRangeColor( const kvs::RGBAColor& color ) { m_range_color = color; }
    void setRangeEdgeColor( const kvs::RGBAColor& color ) { m_range_edge_color = color; }
    void resetTable() { m_table = nullptr; }

protected:
    kvs::TableObject* table();
    kvs::Vec2i toWindowCoordinates( const kvs::Vec2i& p );
    virtual void mousePressEvent( kvs::MouseEvent* e );
    virtual void mouseMoveEvent( kvs::MouseEvent* e );
    virtual void mouseReleaseEvent( kvs::MouseEvent* e );
    virtual void mouseDoubleClickEvent( kvs::MouseEvent* e );
    virtual void paintEvent();
};

} // end of namespace kvs
