/*****************************************************************************/
/**
 *  @file   CategoryAxis.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "CategoryAxis.h"
#include <kvs/FontMetrics>
#include <kvs/Painter>
#include <kvs/Vector2>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Draw the axis with category labels.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void CategoryAxis::draw( kvs::Painter& painter )
{
    switch ( ValueAxis::align() )
    {
    case Top:
    {
        const int margin = this->drawCategoryLabelsOnTop( painter );
        this->drawLabelOnTop( painter, margin );
        ValueAxis::drawTickMarksOnTop( painter );
        ValueAxis::drawAxisOnTop( painter );
        break;
    }
    case Bottom:
    {
        const int margin = this->drawCategoryLabelsOnBottom( painter );
        this->drawLabelOnBottom( painter, margin );
        ValueAxis::drawTickMarksOnTop( painter );
        ValueAxis::drawAxisOnTop( painter );
        break;
    }
    case Left:
    {
        const int margin = this->drawCategoryLabelsOnLeft( painter );
        this->drawLabelOnLeft( painter, margin );
        ValueAxis::drawTickMarksOnTop( painter );
        ValueAxis::drawAxisOnTop( painter );
        break;
    }
    case Right:
    {
        const int margin = this->drawCategoryLabelsOnRight( painter );
        this->drawLabelOnRight( painter, margin );
        ValueAxis::drawTickMarksOnTop( painter );
        ValueAxis::drawAxisOnTop( painter );
        break;
    }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the category labels on the top axis.
 *  @param  painter [in] painter
 *  @return margin between the top axis and the axis label
 */
/*===========================================================================*/
int CategoryAxis::drawCategoryLabelsOnTop( kvs::Painter& painter )
{
    const float x0 = rect().x0();
    const float x1 = rect().x1();
    const float y0 = rect().y0();
    const auto& metrics = painter.fontMetrics();

    const auto ncats = this->numberOfCategories();
    const auto cat_width = ( x1 - x0 ) / ncats;
    const auto cat_offset = cat_width * 0.5f;
    for ( size_t i = 0; i < ncats; ++i )
    {
        if ( i % m_stride ) { continue; }
        const auto label = m_category_labels[i];
        const int x = x0 + cat_width * i + cat_offset - metrics.width( label ) / 2;
        const int y = y0 - metrics.height() - ValueAxis::labelOffset();
        painter.drawText( kvs::Vec2( x, y ), label );
    }

    return metrics.height();
}

/*===========================================================================*/
/**
 *  @brief  Draw the category labels on the bottom axis.
 *  @param  painter [in] painter
 *  @return margin between the bottom axis and the axis label
 */
/*===========================================================================*/
int CategoryAxis::drawCategoryLabelsOnBottom( kvs::Painter& painter )
{
    const float x0 = rect().x0();
    const float x1 = rect().x1();
    const float y1 = rect().y1();
    const auto& metrics = painter.fontMetrics();

    const auto ncats = this->numberOfCategories();
    const auto cat_width = ( x1 - x0 ) / ncats;
    const auto cat_offset = cat_width * 0.5f;
    for ( size_t i = 0; i < ncats; ++i )
    {
        if ( i % m_stride ) { continue; }
        const auto label = m_category_labels[i];
        const int x = x0 + cat_width * i + cat_offset - metrics.width( label ) / 2;
        const int y = y1 + metrics.height() + ValueAxis::labelOffset();
        painter.drawText( kvs::Vec2( x, y ), label );
    }

    return metrics.height();
}

/*===========================================================================*/
/**
 *  @brief  Draw the category labels on the left axis.
 *  @param  painter [in] painter
 *  @return margin between the left axis and the axis label
 */
/*===========================================================================*/
int CategoryAxis::drawCategoryLabelsOnLeft( kvs::Painter& painter )
{
    const float x0 = rect().x0();
    const float y0 = rect().y0();
    const float y1 = rect().y1();
    const auto& metrics = painter.fontMetrics();

    const auto ncats = this->numberOfCategories();
    const auto cat_height = ( y1 - y0 ) / ncats;
    const auto cat_offset = cat_height * 0.5f;

    int margin = 0;
    const int offset = 3; // additional offset between the axis and tick label
    for ( size_t i = 0; i < ncats; ++i )
    {
        if ( i % m_stride ) { continue; }
        const auto label = m_category_labels[i];
        const int x = x0 - metrics.width( label ) - ValueAxis::labelOffset() - offset;
        const int y = y1 - cat_height * i - cat_offset + metrics.height() / 2;
        painter.drawText( kvs::Vec2( x, y ), label );
        margin = kvs::Math::Max( margin, metrics.width( label ) );
    }

    return margin + offset;
}

/*===========================================================================*/
/**
 *  @brief  Draw the category labels on the right axis.
 *  @param  painter [in] painter
 *  @return margin between the right axis and the axis label
 */
/*===========================================================================*/
int CategoryAxis::drawCategoryLabelsOnRight( kvs::Painter& painter )
{
    const float x1 = rect().x1();
    const float y0 = rect().y0();
    const float y1 = rect().y1();
    const auto& metrics = painter.fontMetrics();

    const auto ncats = this->numberOfCategories();
    const auto cat_height = ( y1 - y0 ) / ncats;
    const auto cat_offset = cat_height * 0.5f;

    int margin = 0;
    const int offset = 3; // additional offset between the axis and tick label
    for ( size_t i = 0; i < ncats; ++i )
    {
        if ( i % m_stride ) { continue; }
        const auto label = m_category_labels[i];
        const int x = x1 + ValueAxis::labelOffset() + offset;
        const int y = y1 - cat_height * i - cat_offset + metrics.height() / 2;
        painter.drawText( kvs::Vec2( x, y ), label );
        margin = kvs::Math::Max( margin, metrics.width( label ) );
    }

    return margin + offset;
}

} // end of namespace kvs
