/*****************************************************************************/
/**
 *  @file   VertexBufferObjectManager.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "VertexBufferObjectManager.h"
#include <algorithm>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns name of OpenGL data type.
 *  @param  array [in] value array
 *  @return name of OpenGL data type
 */
/*===========================================================================*/
inline GLenum GLType( const kvs::AnyValueArray& array )
{
    switch ( array.typeID() )
    {
    case kvs::Type::TypeInt8: return GL_BYTE;
    case kvs::Type::TypeInt16: return GL_SHORT;
    case kvs::Type::TypeInt32: return GL_INT;
    case kvs::Type::TypeUInt8: return GL_UNSIGNED_BYTE;
    case kvs::Type::TypeUInt16: return GL_UNSIGNED_SHORT;
    case kvs::Type::TypeUInt32: return GL_UNSIGNED_INT;
    case kvs::Type::TypeReal32: return GL_FLOAT;
    case kvs::Type::TypeReal64: return GL_DOUBLE;
    default: break;
    }
    return kvs::Type::UnknownType;
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Sets vertex attribute array buffer.
 *  @param  buffer [in] vertex attribute array buffer
 */
/*===========================================================================*/
void VertexBufferObjectManager::setVertexAttribArray(
    const VertexAttribBuffer& buffer )
{
    auto result = std::find(
        m_vertex_attrib_arrays.begin(),
        m_vertex_attrib_arrays.end(), buffer );
    if ( result == m_vertex_attrib_arrays.end() )
    {
        m_vertex_attrib_arrays.push_back( buffer );
    }
    else
    {
        *result = buffer;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets vertex value array.
 *  @param  array [in] vertex value array
 *  @param  dim [in] dimension of the array
 *  @param  stride [in] data stride
 */
/*===========================================================================*/
void VertexBufferObjectManager::setVertexArray(
    const kvs::AnyValueArray& array,
    const size_t dim,
    const size_t stride )
{
    m_vertex_array.type = ::GLType( array );
    m_vertex_array.size = array.byteSize();
    m_vertex_array.dim = dim;
    m_vertex_array.stride = stride;
    m_vertex_array.pointer = array.data();
}

/*===========================================================================*/
/**
 *  @brief  Sets color value array.
 *  @param  array [in] color value array
 *  @param  dim [in] dimension of the array
 *  @param  stride [in] data stride
 */
/*===========================================================================*/
void VertexBufferObjectManager::setColorArray(
    const kvs::AnyValueArray& array,
    const size_t dim,
    const size_t stride )
{
    m_color_array.type = ::GLType( array );
    m_color_array.size = array.byteSize();
    m_color_array.dim = dim;
    m_color_array.stride = stride;
    m_color_array.pointer = array.data();
}

/*===========================================================================*/
/**
 *  @brief  Sets normal vector array.
 *  @param  array [in] color value array
 *  @param  stride [in] data stride
 */
/*===========================================================================*/
void VertexBufferObjectManager::setNormalArray(
    const kvs::AnyValueArray& array,
    const size_t stride )
{
    m_normal_array.type = ::GLType( array );
    m_normal_array.size = array.byteSize();
    m_normal_array.dim = 3;
    m_normal_array.stride = stride;
    m_normal_array.pointer = array.data();
}

/*===========================================================================*/
/**
 *  @brief  Sets texture coordinate value array.
 *  @param  array [in] texture coordinate value array
 *  @param  dim [in] dimension of the array
 *  @param  stride [in] data stride
 */
/*===========================================================================*/
void VertexBufferObjectManager::setTexCoordArray(
    const kvs::AnyValueArray& array,
    const size_t dim,
    const size_t stride )
{
    m_tex_coord_array.type = ::GLType( array );
    m_tex_coord_array.size = array.byteSize();
    m_tex_coord_array.dim = dim;
    m_tex_coord_array.stride = stride;
    m_tex_coord_array.pointer = array.data();
}

/*===========================================================================*/
/**
 *  @brief  Sets index array.
 *  @param  array [in] index array
 */
/*===========================================================================*/
void VertexBufferObjectManager::setIndexArray(
    const kvs::AnyValueArray& array )
{
    m_index_array.type = ::GLType( array );
    m_index_array.size = array.byteSize();
    m_index_array.pointer = array.data();
}

/*===========================================================================*/
/**
 *  @brief  Sets vertex attribute value array.
 *  @param  array [in] vertex attribute value array
 *  @param  index [in] index of vertex attribute to be modified
 *  @param  dim [in] dimension of the array
 *  @param  normalized [in] data values should be normalized (GL_TRUE) or not (GL_FALSE)
 *  @param  stride [in] data stride
 */
/*===========================================================================*/
void VertexBufferObjectManager::setVertexAttribArray(
    const kvs::AnyValueArray& array,
    const size_t index,
    const size_t dim,
    const bool normalized,
    const size_t stride )
{
    VertexAttribBuffer attrib_array;
    attrib_array.type = ::GLType( array );
    attrib_array.size = array.byteSize();
    attrib_array.dim = dim;
    attrib_array.stride = stride;
    attrib_array.pointer = array.data();
    attrib_array.index = index;
    attrib_array.normalized = ( normalized ) ? GL_TRUE : GL_FALSE;

    auto result = std::find(
        m_vertex_attrib_arrays.begin(),
        m_vertex_attrib_arrays.end(),
        attrib_array );
    if ( result == m_vertex_attrib_arrays.end() )
    {
        m_vertex_attrib_arrays.push_back( attrib_array );
    }
    else
    {
        *result = attrib_array;
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates a vertex buffer object and an index buffer object.
 */
/*===========================================================================*/
void VertexBufferObjectManager::create()
{
    const size_t vbo_size = this->vertex_buffer_object_size();
    if ( vbo_size > 0 )
    {
        m_vbo.create( vbo_size );
        {
            size_t offset = 0;
            m_vbo.bind();
            if ( m_vertex_array.size > 0 )
            {
                m_vertex_array.offset = offset;
                offset += m_vbo.load(
                    m_vertex_array.size,
                    m_vertex_array.pointer,
                    m_vertex_array.offset  );
            }

            if ( m_color_array.size > 0 )
            {
                m_color_array.offset = offset;
                offset += m_vbo.load(
                    m_color_array.size,
                    m_color_array.pointer,
                    m_color_array.offset );
            }

            if ( m_normal_array.size > 0 )
            {
                m_normal_array.offset = offset;
                offset += m_vbo.load(
                    m_normal_array.size,
                    m_normal_array.pointer,
                    m_normal_array.offset );
            }

            if ( m_tex_coord_array.size > 0 )
            {
                m_tex_coord_array.offset = offset;
                offset +=  m_vbo.load(
                    m_tex_coord_array.size,
                    m_tex_coord_array.pointer,
                    m_tex_coord_array.offset );
            }

            for ( size_t i = 0; i < m_vertex_attrib_arrays.size(); i++ )
            {
                if ( m_vertex_attrib_arrays[i].size > 0 )
                {
                    m_vertex_attrib_arrays[i].offset = offset;
                    offset +=  m_vbo.load(
                        m_vertex_attrib_arrays[i].size,
                        m_vertex_attrib_arrays[i].pointer,
                        m_vertex_attrib_arrays[i].offset );
                }
            }

            m_vbo.unbind();
        }

        const size_t ibo_size = m_index_array.size;
        if ( ibo_size > 0 )
        {
            m_ibo.create( ibo_size );
            m_ibo.bind();
            m_ibo.load( m_index_array.size, m_index_array.pointer, 0 );
            m_ibo.unbind();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Binds vertex buffer object.
 */
/*===========================================================================*/
void VertexBufferObjectManager::bind() const
{
    m_vbo.bind();
    this->enable_client_state();
}

/*===========================================================================*/
/**
 *  @brief  Unbinds vertex buffer object.
 */
/*===========================================================================*/
void VertexBufferObjectManager::unbind() const
{
    this->disable_client_state();
    m_vbo.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Release resources of vertex buffer object and index buffer object.
 */
/*===========================================================================*/
void VertexBufferObjectManager::release()
{
    m_vbo.release();
    m_ibo.release();
    m_vbo_size = 0;
    m_ibo_size = 0;

    m_vertex_array = VertexBuffer();
    m_color_array = VertexBuffer();
    m_normal_array = VertexBuffer();
    m_tex_coord_array = VertexBuffer();
    m_index_array = IndexBuffer();
    m_vertex_attrib_arrays.shrink_to_fit();
}

/*===========================================================================*/
/**
 *  @brief  Draws vertex buffer object.
 *  @param  mode [in] rendering geometric primitives
 *  @param  first [in] starting index in the arrays
 *  @param  count [in] number of indices to be rendered
 */
/*===========================================================================*/
void VertexBufferObjectManager::drawArrays(
    GLenum mode,
    GLint first,
    GLsizei count )
{
    kvs::OpenGL::DrawArrays( mode, first, count );
}

/*===========================================================================*/
/**
 *  @brief  Draws vertex buffer object.
 *  @param  mode [in] rendering geometric primitives
 *  @param  first [in] array of starting index in the arrays
 *  @param  count [in] array of number of indices to be rendered
 *  @param  drawcount [in] size of the first and count
 */
/*===========================================================================*/
void VertexBufferObjectManager::drawArrays(
    GLenum mode,
    const GLint* first,
    const GLsizei* count,
    GLsizei drawcount )
{
    kvs::OpenGL::MultiDrawArrays( mode, first, count, drawcount );
}

/*===========================================================================*/
/**
 *  @brief  Draws vertex buffer object.
 *  @param  mode [in] rendering geometric primitives
 *  @param  first [in] array of starting index in the arrays
 *  @param  count [in] array of number of indices to be rendered
 */
/*===========================================================================*/
void VertexBufferObjectManager::drawArrays(
    GLenum mode,
    const kvs::ValueArray<GLint>& first,
    const kvs::ValueArray<GLsizei>& count )
{
    kvs::OpenGL::MultiDrawArrays( mode, first, count );
}

/*===========================================================================*/
/**
 *  @brief  Draws vertex buffer object with index buffer object.
 *  @param  mode [in] rendering geometric primitives
 *  @param  count [in] number of indices to be rendered
 */
/*===========================================================================*/
void VertexBufferObjectManager::drawElements(
    GLenum mode,
    GLsizei count )
{
    kvs::IndexBufferObject::Binder bind( m_ibo );
    kvs::OpenGL::DrawElements( mode, count, m_index_array.type, 0 );
}

/*===========================================================================*/
/**
 *  @brief  Draws vertex buffer object with index buffer object.
 *  @param  mode [in] rendering geometric primitives
 *  @param  count [in] array of number of indices to be rendered
 *  @param  drawcount [in] size of the count
 */
/*===========================================================================*/
void VertexBufferObjectManager::drawElements(
    GLenum mode,
    const GLsizei* count,
    GLsizei drawcount )
{
    kvs::IndexBufferObject::Binder bind( m_ibo );
    kvs::OpenGL::MultiDrawElements( mode, count, m_index_array.type, 0, drawcount );
}

/*===========================================================================*/
/**
 *  @brief  Draws vertex buffer object with index buffer object.
 *  @param  mode [in] rendering geometric primitives
 *  @param  count [in] array of number of indices to be rendered
 */
/*===========================================================================*/
void VertexBufferObjectManager::drawElements(
    GLenum mode,
    const kvs::ValueArray<GLsizei>& count )
{
    kvs::IndexBufferObject::Binder bind( m_ibo );
    kvs::OpenGL::MultiDrawElements( mode, count, m_index_array.type, 0 );
}

/*===========================================================================*/
/**
 *  @brief  Returns data size of vertex buffer object.
 *  @return data size of vertex buffer object
 */
/*===========================================================================*/
size_t VertexBufferObjectManager::vertex_buffer_object_size() const
{
    size_t vbo_size = 0;
    vbo_size += BufferObject::PaddedBufferSize( m_vertex_array.size );
    vbo_size += BufferObject::PaddedBufferSize( m_color_array.size );
    vbo_size += BufferObject::PaddedBufferSize( m_normal_array.size );
    vbo_size += BufferObject::PaddedBufferSize( m_tex_coord_array.size );
    for ( size_t i = 0; i < m_vertex_attrib_arrays.size(); i++ )
    {
        vbo_size += BufferObject::PaddedBufferSize( m_vertex_attrib_arrays[i].size );
    }
    return vbo_size;
}

/*===========================================================================*/
/**
 *  @brief  Enables client-side capability
 */
/*===========================================================================*/
void VertexBufferObjectManager::enable_client_state() const
{
    if ( m_vertex_array.size > 0 )
    {
        const VertexBuffer& array = m_vertex_array;
        const GLvoid* offset = (GLbyte*)NULL + array.offset;
        kvs::OpenGL::EnableClientState( GL_VERTEX_ARRAY );
        kvs::OpenGL::VertexPointer( array.dim, array.type, array.stride, offset );
    }

    if ( m_color_array.size > 0 )
    {
        const VertexBuffer& array = m_color_array;
        const GLvoid* offset = (GLbyte*)NULL + array.offset;
        kvs::OpenGL::EnableClientState( GL_COLOR_ARRAY );
        kvs::OpenGL::ColorPointer( array.dim, array.type, array.stride, offset );
    }

    if ( m_normal_array.size > 0 )
    {
        const VertexBuffer& array = m_normal_array;
        const GLvoid* offset = (GLbyte*)NULL + array.offset;
        kvs::OpenGL::EnableClientState( GL_NORMAL_ARRAY );
        kvs::OpenGL::NormalPointer( array.type, array.stride, offset );
    }

    if ( m_tex_coord_array.size > 0 )
    {
        const VertexBuffer& array = m_tex_coord_array;
        const GLvoid* offset = (GLbyte*)NULL + array.offset;
        kvs::OpenGL::EnableClientState( GL_TEXTURE_COORD_ARRAY );
        kvs::OpenGL::TexCoordPointer( array.dim, array.type, array.stride, offset );
    }

    for ( size_t i = 0; i < m_vertex_attrib_arrays.size(); i++ )
    {
        const VertexAttribBuffer& array = m_vertex_attrib_arrays[i];
        if ( array.size > 0 )
        {
            const GLvoid* offset = (GLbyte*)NULL + array.offset;
            kvs::OpenGL::EnableVertexAttribArray( array.index );
            kvs::OpenGL::VertexAttribPointer(
                array.index,
                array.dim,
                array.type,
                array.normalized,
                array.stride,
                offset );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Disables client-side capability
 */
/*===========================================================================*/
void VertexBufferObjectManager::disable_client_state() const
{
    if ( m_vertex_array.size > 0 )
    {
        kvs::OpenGL::DisableClientState( GL_VERTEX_ARRAY );
    }

    if ( m_color_array.size > 0 )
    {
        kvs::OpenGL::DisableClientState( GL_COLOR_ARRAY );
    }

    if ( m_normal_array.size > 0 )
    {
        kvs::OpenGL::DisableClientState( GL_NORMAL_ARRAY );
    }

    if ( m_tex_coord_array.size > 0 )
    {
        kvs::OpenGL::DisableClientState( GL_TEXTURE_COORD_ARRAY );
    }

    for ( size_t i = 0; i < m_vertex_attrib_arrays.size(); i++ )
    {
        const VertexAttribBuffer& array = m_vertex_attrib_arrays[i];
        if ( array.size > 0 )
        {
            kvs::OpenGL::DisableVertexAttribArray( array.index );
        }
    }
}

} // end of namespace kvs
