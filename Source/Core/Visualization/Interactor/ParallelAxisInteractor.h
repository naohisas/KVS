/*****************************************************************************/
/**
 *  @file   ParallelAxisInteractor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/InteractorBase>
#include <kvs/ParallelAxis>
#include <kvs/Vector2>
#include <kvs/Painter>
#include <kvs/TableObject>
#include <kvs/UIColor>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Parallel axis interactor class.
 */
/*===========================================================================*/
class ParallelAxisInteractor : public kvs::InteractorBase
{
public:
    using BaseClass = kvs::InteractorBase;

private:
    kvs::TableObject* m_table = nullptr; ///< table object
    const kvs::ParallelAxis* m_axis = nullptr; ///< parallel axis (reference)
    kvs::Vec2i m_p0{ 0, 0 }; ///< mouse pressing position
    kvs::Vec2i m_p1{ 0, 0 }; ///< mouse moving position
    kvs::Vec2i m_p{ 0, 0 };  ///< current mouse position
    size_t m_selected_axis = 0; ///< selected axis ID
    bool m_range_setting = false; ///< range selecting mode if true
    bool m_range_moving = false; ///< range moving mode if true
    kvs::Painter m_painter{}; ///< painter
    kvs::Real32 m_range_width = 20.0f; ///< range bar width
    kvs::Real32 m_range_edge_width = 2.0f; ///< range bar edge width
    kvs::RGBAColor m_range_color{ kvs::UIColor::Fill() }; ///< range bar color
    kvs::RGBAColor m_range_edge_color{ kvs::UIColor::TertiaryLabel() }; ///< range bar edge color

public:
    ParallelAxisInteractor( const kvs::ParallelAxis* axis );
    virtual ~ParallelAxisInteractor() = default;

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
    virtual void mousePressEvent( kvs::MouseEvent* e );
    virtual void mouseMoveEvent( kvs::MouseEvent* e );
    virtual void mouseReleaseEvent( kvs::MouseEvent* e );
    virtual void mouseDoubleClickEvent( kvs::MouseEvent* e );
    virtual void paintEvent();
};

} // end of namespace kvs
