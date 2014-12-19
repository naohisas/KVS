/*****************************************************************************/
/**
 *  @file   DisplayList.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DisplayList.h 1436 2013-03-17 12:44:02Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__DISPLAY_LIST_H_INCLUDE
#define KVS__DISPLAY_LIST_H_INCLUDE

#if KVS_ENABLE_DEPRECATED

#include <kvs/OpenGL>


namespace kvs
{

class DisplayList
{
protected:

    GLuint m_id; ///< display list ID
    GLsizei m_range; ///< number of lists

public:

    DisplayList();
    DisplayList( const GLsizei range );
    virtual ~DisplayList();

    GLuint id() const;
    GLsizei range() const;

    void begin( const GLuint index = 0, const GLenum mode = GL_COMPILE );
    void end();
    bool create( const GLsizei range );
    void clear();
    void render( const GLuint index = 0 );
};

} // end of namespace kvs

#endif

#endif // KVS__DISPLAY_LIST_H_INCLUDE
