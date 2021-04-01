/****************************************************************************/
/**
 *  @file   RendererManager.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "RendererManager.h"
#include <kvs/RendererBase>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Renderer deleter class. (for compatibility)
 */
/*===========================================================================*/
class RendererDeleter
{
    bool m_auto_del = true;
public:
    RendererDeleter( bool auto_del = true ): m_auto_del( auto_del ) {}
    void operator ()( kvs::RendererBase* r ) { if ( m_auto_del ) { delete r; } }
    void disableAutoDelete() { m_auto_del = false; }
};

/*===========================================================================*/
/**
 *  @brief  Disables auto-delete for the specified renderer.
 *  @param  sp [in] shared pointer to the renderer
 */
/*===========================================================================*/
void DisableAutoDelete( const kvs::SharedPointer<kvs::RendererBase>& sp )
{
    // Disable deletion if the deleter type of sp is RendererDeleter.
    RendererDeleter* d = kvs::get_deleter<RendererDeleter>( sp );
    if ( d ) { d->disableAutoDelete(); }
}

} // end of namespace


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Insert the renderer.
 *  @param  renderer [in] pointer to the renderer
 *  @return renderer ID
 */
/*==========================================================================*/
int RendererManager::insert( kvs::RendererBase* renderer )
{
    bool auto_delete = true;
    RendererIterator registered_renderer = m_renderer_list.begin();
    RendererIterator last = m_renderer_list.end();
    while ( registered_renderer != last )
    {
        if ( registered_renderer->second.get() == renderer )
        {
            auto_delete = false;
            break;
        }
        ++registered_renderer;
    }

    return this->insert( RendererPointer( renderer, ::RendererDeleter( auto_delete ) ) );
}

/*==========================================================================*/
/**
 *  @brief  Erase the renderer.
 *  @param  delete_flag [in] deleting the allocated memory flag
 */
/*==========================================================================*/
void RendererManager::erase( bool delete_flag )
{
    if ( !delete_flag )
    {
        RendererIterator registered_renderer = m_renderer_list.begin();
        RendererIterator last = m_renderer_list.end();
        while ( registered_renderer != last )
        {
            ::DisableAutoDelete( registered_renderer->second );
            ++registered_renderer;
        }
    }

    m_renderer_list.clear();
}

/*==========================================================================*/
/**
 *  @brief  Erase the renderer which is specified by the given ID.
 *  @param  id [in] renderer ID
 *  @param  delete_flag [in] deleting the allocated memory flag
 */
/*==========================================================================*/
void RendererManager::erase( int id, bool delete_flag )
{
    RendererIterator renderer = this->find_renderer( id );
    if ( renderer == m_renderer_list.end() ) return;

    if ( !delete_flag )
    {
        ::DisableAutoDelete( renderer->second );
    }

    m_renderer_list.erase( renderer );
}

/*==========================================================================*/
/**
 *  @breif  Erase the renderer which is specified by the given name.
 *  @param  name [in] renderer name
 *  @param  delete_flag [in] deleting the allocated memory flag
 */
/*==========================================================================*/
void RendererManager::erase( std::string name, bool delete_flag )
{
    RendererIterator renderer = this->find_renderer( name );
    if ( renderer == m_renderer_list.end() ) return;

    if ( !delete_flag )
    {
        ::DisableAutoDelete( renderer->second );
    }

    m_renderer_list.erase( renderer );
}

/*==========================================================================*/
/**
 *  @brief  Change the renderer by the specificated renderer ID.
 *  @param  id [in] renderer ID stored in the renderer manager
 *  @param  renderer [in] pointer to the inserting renderer
 *  @param  delete_flg [in] deleting the allocated memory flag
 */
/*==========================================================================*/
void RendererManager::change( int id, kvs::RendererBase* renderer, bool delete_flag )
{
    // Search the object which is specified by given renderer ID in the
    // renderer pointer list. If it isn't found, this method executes nothing.
    RendererIterator registered_renderer = this->find_renderer( id );
    if ( registered_renderer == m_renderer_list.end() ) return;

    if ( !delete_flag )
    {
        ::DisableAutoDelete( registered_renderer->second );
    }

    // Change the renderer.
    registered_renderer->second = RendererPointer( renderer, ::RendererDeleter() );
}

/*==========================================================================*/
/**
 *  @breif  Change the renderer by the specificated renderer name.
 *  @param  name [in] renderer name stored in the renderer manager
 *  @param  renderer [in] pointer to the inserting renderer
 *  @param  delete_flag [in] deleting the allocated memory flag
 */
/*==========================================================================*/
void RendererManager::change( std::string name, kvs::RendererBase* renderer, bool delete_flag )
{
    RendererIterator registered_renderer = this->find_renderer( name );
    if ( registered_renderer == m_renderer_list.end() ) return;

    if ( !delete_flag )
    {
        ::DisableAutoDelete( registered_renderer->second );
    }

    // Change the renderer.
    registered_renderer->second = RendererPointer( renderer, ::RendererDeleter() );
}

/*==========================================================================*/
/**
 *  @brief  Returns the renderer.
 *  @return pointer to the renderer
 */
/*==========================================================================*/
kvs::RendererBase* RendererManager::renderer()
{
    return m_renderer_list.begin()->second.get();
}

/*==========================================================================*/
/**
 *  @breig  Returns the renderer which is specified by the given ID.
 *  @param  id [in] renderer ID
 *  @return pointer to the renderer
 */
/*==========================================================================*/
kvs::RendererBase* RendererManager::renderer( int id )
{
    RendererIterator registered_renderer = this->find_renderer( id );
    if ( registered_renderer != m_renderer_list.end() )
    {
        return registered_renderer->second.get();
    }

    return NULL;
}

/*==========================================================================*/
/**
 *  @brief  Returns the renderer which is specified by the given name.
 *  @param  name [in] renderer name
 *  @return pointer to the renderer
 */
/*==========================================================================*/
kvs::RendererBase* RendererManager::renderer( std::string name )
{
    RendererIterator registered_renderer = this->find_renderer( name );
    if ( registered_renderer != m_renderer_list.end() )
    {
        return registered_renderer->second.get();
    }

    return NULL;
}

/*===========================================================================*/
/**
 *  @brief  Returns ID of the specified renderer.
 *  @param  renderer [in] pointer to the renderer
 *  @return ID of the renderer
 */
/*===========================================================================*/
int RendererManager::rendererID( const kvs::RendererBase* renderer ) const
{
    RendererList::const_iterator registered_renderer = m_renderer_list.begin();
    RendererList::const_iterator last = m_renderer_list.end();
    while ( registered_renderer != last )
    {
        if ( registered_renderer->second.get() == renderer )
        {
            return registered_renderer->first;
        }
        ++registered_renderer;
    }

    return -1;
}

/*==========================================================================*/
/**
 *  @brief  Insert the renderer.
 *  @param  renderer [in] pointer to the renderer
 *  @return renderer ID
 */
/*==========================================================================*/
int RendererManager::insert( const kvs::SharedPointer<kvs::RendererBase>& renderer )
{
    ++m_renderer_id;
    m_renderer_list.push_back( std::make_pair( m_renderer_id, renderer ) );
    return m_renderer_id;
}

/*==========================================================================*/
/**
 *  @brief  Erase the renderer which is specified by the given ID.
 *  @param  id [in] renderer ID
 */
/*==========================================================================*/
void RendererManager::erase( int id )
{
    RendererIterator renderer = this->find_renderer( id );
    if ( renderer == m_renderer_list.end() ) return;
    m_renderer_list.erase( renderer );
}

/*==========================================================================*/
/**
 *  @brief  Erase the renderer which is specified by the given name.
 *  @param  name [in] renderer name
 */
/*==========================================================================*/
void RendererManager::erase( std::string name )
{
    RendererIterator renderer = this->find_renderer( name );
    if ( renderer == m_renderer_list.end() ) return;
    m_renderer_list.erase( renderer );
}

/*==========================================================================*/
/**
 *  @brief  Change the renderer by the specificated renderer ID.
 *  @param  id [in] renderer ID stored in the renderer manager
 *  @param  renderer [in] pointer to the inserting renderer
 */
/*==========================================================================*/
void RendererManager::change(
    int id,
    const kvs::SharedPointer<kvs::RendererBase>& renderer )
{
    // Search the object which is specified by given renderer ID in the
    // renderer pointer list. If it isn't found, this method executes nothing.
    RendererIterator registered_renderer = this->find_renderer( id );
    if ( registered_renderer == m_renderer_list.end() ) return;

    // Change the renderer.
    registered_renderer->second = renderer;
}

/*==========================================================================*/
/**
 *  @brief  Change the renderer by the specificated renderer name.
 *  @param  name [in] renderer name stored in the renderer manager
 *  @param  renderer [in] pointer to the inserting renderer
 */
/*==========================================================================*/
void RendererManager::change(
    std::string name,
    const kvs::SharedPointer<kvs::RendererBase>& renderer )
{
    RendererIterator registered_renderer = this->find_renderer( name );
    if ( registered_renderer == m_renderer_list.end() ) return;

    // Change the renderer.
    registered_renderer->second = renderer;
}

/*===========================================================================*/
/**
 *  @brief  Returns the interator to the renderer specified by the ID.
 *  @param  id [in] renderer ID
 *  @return interotr of the renderer
 */
/*===========================================================================*/
RendererManager::RendererIterator RendererManager::find_renderer( int id )
{
    RendererIterator registered_renderer = m_renderer_list.begin();
    RendererIterator last = m_renderer_list.end();
    while ( registered_renderer != last )
    {
        if ( registered_renderer->first == id )
        {
            return registered_renderer;
        }
        ++registered_renderer;
    }

    return last;
}

/*===========================================================================*/
/**
 *  @brief  Returns the interator to the renderer specified by the name.
 *  @param  name [in] renderer name
 *  @return interator of the renderer
 */
/*===========================================================================*/
RendererManager::RendererIterator RendererManager::find_renderer( std::string name )
{
    RendererIterator registered_renderer = m_renderer_list.begin();
    RendererIterator last = m_renderer_list.end();
    while ( registered_renderer != last )
    {
        if ( registered_renderer->second->name() == name )
        {
            return registered_renderer;
        }
        ++registered_renderer;
    }

    return last;
}

} // end of namespace kvs
