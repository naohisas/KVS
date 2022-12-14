/*****************************************************************************/
/**
 *  @file   CategoryAxis.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ValueAxis>
#include <kvs/StringList>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Category axis class.
 */
/*===========================================================================*/
class CategoryAxis : public kvs::ValueAxis
{
private:
    kvs::StringList m_category_labels{}; ///< category label list
    size_t m_stride = 1; ///< label stride

public:
    CategoryAxis( Align align = Bottom ):
        kvs::ValueAxis( align ) {}

    CategoryAxis( const CategoryAxis& axis ):
        kvs::ValueAxis( axis ),
        m_category_labels( axis.m_category_labels ),
        m_stride( axis.m_stride ) {}

    virtual ~CategoryAxis() = default;

    const kvs::StringList& categoryLabels() const { return m_category_labels; }
    const std::string& categoryLabel( const size_t index ) const { return m_category_labels[ index ]; }
    size_t numberOfCategories() const { return m_category_labels.size(); }
    size_t stride() const { return m_stride; }

    void setCategoryLabels( const kvs::StringList& labels ) { m_category_labels = labels; }
    void setStride( const size_t stride ) { m_stride = stride; }

    virtual void draw( kvs::Painter& painter );

protected:
    int drawCategoryLabelsOnTop( kvs::Painter& painter );
    int drawCategoryLabelsOnBottom( kvs::Painter& painter );
    int drawCategoryLabelsOnLeft( kvs::Painter& painter );
    int drawCategoryLabelsOnRight( kvs::Painter& painter );
};

} // end of namespace kvs
