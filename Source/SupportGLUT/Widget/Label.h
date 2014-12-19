/*****************************************************************************/
/**
 *  @file   Label.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Label.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__LABEL_H_INCLUDE
#define KVS__GLUT__LABEL_H_INCLUDE

#include <string>
#include <vector>
#include <kvs/glut/Screen>
#include "WidgetBase.h"


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Label class.
 */
/*===========================================================================*/
class Label : public kvs::glut::WidgetBase
{
public:

    typedef kvs::glut::WidgetBase BaseClass;

protected:

    std::vector<std::string> m_text; ///< text list

public:

    Label( kvs::ScreenBase* screen = 0 );

    virtual void screenUpdated( void ){};
    virtual void screenResized( void ){};

    void setText( const char* text, ... );
    void addText( const char* text, ... );

private:

    int get_fitted_width( void );
    int get_fitted_height( void );

public:

    void paintEvent( void );
    void resizeEvent( int width, int height );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__LABEL_H_INCLUDE
