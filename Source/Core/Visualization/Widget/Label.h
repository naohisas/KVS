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
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;

private:
    TextList m_text_list; ///< text list
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;

public:
    Label( kvs::ScreenBase* screen = 0 );

    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }

    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); }

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
