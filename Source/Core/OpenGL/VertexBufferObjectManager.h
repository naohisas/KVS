/*****************************************************************************/
/**
 *  @file   VertexBufferObjectManager.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/VertexBufferObject>
#include <kvs/IndexBufferObject>
#include <kvs/AnyValueArray>
#include <kvs/OpenGL>
#include <vector>


namespace kvs
{

class VertexBufferObjectManager
{
public:
    struct VertexBuffer
    {
        GLenum type = 0; ///< GL_UNSIGNED_BYTE, GL_FLOAT, etc
        GLsizei size = 0; ///< data size [bytes]
        GLint dim = 0; ///< data dimension
        GLsizei stride = 0; ///< data stride
        const GLvoid* pointer = nullptr; ///< pointer to the data
        GLsizei offset = 0; ///< offset bytes
        VertexBuffer() = default;
    };

    struct VertexAttribBuffer : public VertexBuffer
    {
        GLuint index = 0; ///< index of vertex attribute to be modified
        GLboolean normalized = GL_FALSE; ///< data values should be normalized (GL_TRUE) or not (GL_FALSE)
        VertexAttribBuffer() = default;
        friend bool operator == ( const VertexAttribBuffer& left, const VertexAttribBuffer& right )
        {
            return left.index == right.index;
        }
    };

    struct IndexBuffer
    {
        GLenum type = 0; ///< GL_UNSIGNED_BYTE, GL_FLOAT, etc
        GLsizei size = 0; ///< data size [bytes]
        const GLvoid* pointer = nullptr; ///< pointer to the data
        IndexBuffer() = default;
    };

    using VertexAttribBuffers = std::vector<VertexAttribBuffer>;

public:
    class Binder
    {
        const kvs::VertexBufferObjectManager& m_manager;
    public:
        Binder( const kvs::VertexBufferObjectManager& manager ):
            m_manager( manager ) { m_manager.bind(); }
        ~Binder() { m_manager.unbind(); }
        Binder( const Binder& ) = delete;
        Binder& operator = ( const Binder& ) = delete;
    };

private:
    kvs::VertexBufferObject m_vbo{}; ///< vertex buffer object (VBO)
    kvs::IndexBufferObject m_ibo{}; ///< index buffer object (IBO)
    size_t m_vbo_size = 0; ///< data size of VBO
    size_t m_ibo_size = 0; ///< data size of IBO

    VertexBuffer m_vertex_array; ///< vertex array buffer
    VertexBuffer m_color_array; ///< color array buffer
    VertexBuffer m_normal_array; ///< normal array buffer
    VertexBuffer m_tex_coord_array; ///< texture coordinate array buffer
    IndexBuffer m_index_array; ///< index array buffer
    VertexAttribBuffers m_vertex_attrib_arrays; ///< vertex attribute array buffers

public:
    VertexBufferObjectManager() = default;
    virtual ~VertexBufferObjectManager() { this->release(); }

    const kvs::VertexBufferObject& vertexBufferObject() const { return m_vbo; }
    const kvs::IndexBufferObject& indexBufferObject() const { return m_ibo; }
    const VertexBuffer& vertexArray() const { return m_vertex_array; }
    const VertexBuffer& colorArray() const { return m_color_array; }
    const VertexBuffer& normalArray() const { return m_normal_array; }
    const VertexBuffer& texCoordArray() const { return m_tex_coord_array; }
    const IndexBuffer& indexArray() const { return m_index_array; }
    const VertexAttribBuffers& vertexAttribArrays() const { return m_vertex_attrib_arrays; }
    const VertexAttribBuffer& vertexAttribArray( const size_t index ) const { return m_vertex_attrib_arrays[index]; }

    void setVertexArray( const VertexBuffer& buffer ) { m_vertex_array = buffer; }
    void setColorArray( const VertexBuffer& buffer ) { m_color_array = buffer; }
    void setNormalArray( const VertexBuffer& buffer ) { m_normal_array = buffer; }
    void setTexCoordArray( const VertexBuffer& buffer ) { m_tex_coord_array = buffer; }
    void setIndexArray( const IndexBuffer& buffer ) { m_index_array = buffer; }
    void setVertexAttribArray( const VertexAttribBuffer& buffer );

    void setVertexArray( const kvs::AnyValueArray& array, const size_t dim, const size_t stride = 0 );
    void setColorArray( const kvs::AnyValueArray& array, const size_t dim, const size_t stride = 0 );
    void setNormalArray( const kvs::AnyValueArray& array, const size_t stride = 0 );
    void setTexCoordArray( const kvs::AnyValueArray& array, const size_t dim, const size_t stride = 0 );
    void setIndexArray( const kvs::AnyValueArray& array );
    void setVertexAttribArray( const kvs::AnyValueArray& array, const size_t index, const size_t dim, const bool normalized = false, const size_t stride = 0 );

    void create();
    void bind() const;
    void unbind() const;
    void release();

    void drawArrays( GLenum mode, GLint first, GLsizei count );
    void drawArrays( GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount );
    void drawArrays( GLenum mode, const kvs::ValueArray<GLint>& first, const kvs::ValueArray<GLsizei>& count );
    void drawElements( GLenum mode, GLsizei count );
    void drawElements( GLenum mode, const GLsizei* count, GLsizei drawcount );
    void drawElements( GLenum mode, const kvs::ValueArray<GLsizei>& count );

private:
    size_t vertex_buffer_object_size() const;
    void enable_client_state() const;
    void disable_client_state() const;
};

} // end of namespace kvs
