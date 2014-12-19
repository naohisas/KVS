/*****************************************************************************/
/**
 *  @file   ShaderObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ShaderObject.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__SHADER_OBJECT_H_INCLUDE
#define KVS__SHADER_OBJECT_H_INCLUDE

#include <kvs/ShaderSource>
#include <kvs/GL>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  Shader object class.
 */
/*===========================================================================*/
class ShaderObject
{
protected:

    GLuint m_id; ///< shader ID
    GLenum m_type; ///< shader type (GL_VERTEX_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER)

public:

    ShaderObject( const GLenum type );
    virtual ~ShaderObject();

    GLuint id() const;
    std::string log() const;
    std::string source() const;
    void setSource( const kvs::ShaderSource& source ) const;

    void create();
    void release();
    bool compile() const;
    bool compile( const kvs::ShaderSource& source ) const;

    bool isCreated() const;
    bool isValid() const;
    bool isCompiled() const;

protected:

    ShaderObject();
    void createID();
    void deleteID();

public:
    KVS_DEPRECATED( bool create( const kvs::ShaderSource& source ) ) { this->create(); return this->compile( source ); }
    KVS_DEPRECATED( void clear() ) { this->release(); }
};

} // end of namespace kvs

#endif // KVS__SHADER_OBJECT_H_INCLUDE
