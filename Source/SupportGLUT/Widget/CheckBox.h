/*****************************************************************************/
/**
 *  @file   CheckBox.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CheckBox.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__CHECK_BOX_H_INCLUDE
#define KVS__GLUT__CHECK_BOX_H_INCLUDE

#include <kvs/glut/WidgetBase>
#include <string>


namespace kvs
{

class ScreenBase;

namespace glut
{

class CheckBoxGroup;

/*===========================================================================*/
/**
 *  @brief  Check box class.
 */
/*===========================================================================*/
class CheckBox : public kvs::glut::WidgetBase
{
public:
    typedef kvs::glut::WidgetBase BaseClass;
    friend class kvs::glut::CheckBoxGroup;

private:
    std::string m_caption; ///< caption
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color
    bool m_state; ///< check state
    kvs::glut::CheckBoxGroup* m_group; ///< pointer to the check box group

public:
    CheckBox( kvs::ScreenBase* screen = 0 );

    virtual void pressed() {};
    virtual void released() {};
    virtual void screenUpdated() {};
    virtual void screenResized() {};
    virtual void stateChanged() {};

    const std::string& caption() const { return m_caption; }
    bool state() const { return m_state; }
    void setCaption( const std::string caption ) { m_caption = caption; }
    void setState( const bool state ) { m_state = state; }

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_box();
    void draw_mark();
    bool contains( int x, int y );
    void attach_group( kvs::glut::CheckBoxGroup* group ) { m_group = group; }
    void detach_group() { m_group = NULL; }

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__CHECK_BOX_H_INCLUDE
