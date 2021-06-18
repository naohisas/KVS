/****************************************************************************/
/**
 *  @file   RendererManager.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <utility>
#include <vector>
#include <kvs/SharedPointer>
#include <kvs/Deprecated>


namespace kvs
{

class RendererBase;

/*==========================================================================*/
/**
*  @breif  Renderer manager class.
*/
/*==========================================================================*/
class RendererManager
{
private:
    using RendererPointer = kvs::SharedPointer<kvs::RendererBase>;
    using RendererList = std::vector<std::pair<int, RendererPointer> >;
    using RendererIterator = RendererList::iterator;

private:
    RendererList m_renderer_list{}; ///< renderer list
    int m_renderer_id = 0; ///< renderer ID

public:
    RendererManager() = default;
    virtual ~RendererManager() = default;
    RendererManager( const RendererManager& ) = delete;
    RendererManager& operator =( const RendererManager& ) = delete;

    int insert( kvs::RendererBase* renderer );
    void erase( bool delete_flag );
    void erase( int id, bool delete_flag );
    void erase( std::string name, bool delete_flag );
    void change( int id, kvs::RendererBase* renderer, bool delete_flag );
    void change( std::string name, kvs::RendererBase* renderer, bool delete_flag );
    kvs::RendererBase* renderer();
    kvs::RendererBase* renderer( int id );
    kvs::RendererBase* renderer( std::string name );

    int numberOfRenderers() const { return m_renderer_list.size(); }
    bool hasRenderer() const { return this->numberOfRenderers() != 0; }
    int rendererID( const kvs::RendererBase* renderer ) const;

    int insert( const kvs::SharedPointer<kvs::RendererBase>& renderer );
    void erase() { m_renderer_list.clear(); }
    void erase( int id );
    void erase( std::string name );
    void change( int id, const kvs::SharedPointer<kvs::RendererBase>& renderer );
    void change( std::string name, const kvs::SharedPointer<kvs::RendererBase>& renderer );

private:
    RendererIterator find_renderer( int id );
    RendererIterator find_renderer( std::string name );
};

} // end of namespace kvs
