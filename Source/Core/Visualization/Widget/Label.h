/*****************************************************************************/
/**
 *  @file   Label.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <kvs/ScreenBase>
#include "WidgetBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Label class.
 */
/*===========================================================================*/
class Label : public kvs::WidgetBase
{
public:
    using BaseClass = kvs::WidgetBase;
    using TextList = std::vector<std::string>;

private:
    TextList m_text_list; ///< text list

public:
    Label( kvs::ScreenBase* screen = 0 );

    virtual void screenUpdated(){};
    virtual void screenResized(){};

    const TextList& textList() const { return m_text_list; }
    const std::string& text( const size_t index ) { return m_text_list[ index ]; }

    void setText( const std::string& text ) { m_text_list.clear(); this->addText( text ); }
    void addText( const std::string& text ) { m_text_list.push_back( text ); }
    void setText( const char* text, ... );
    void addText( const char* text, ... );

protected:
    int adjustedWidth();
    int adjustedHeight();

public:
    void paintEvent();
    void resizeEvent( int width, int height );
};

} // end of namespace kvs
