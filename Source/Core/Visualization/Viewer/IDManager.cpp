/****************************************************************************/
/**
 *  @file   IDManager.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "IDManager.h"
#include <kvs/Assert>
#include <algorithm>
#include <functional>
#include <iterator>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new IDManager class.
 */
/*==========================================================================*/
IDManager::IDManager()
{
    m_flip_table.clear();
    m_id_list.clear();
}

/*==========================================================================*/
/**
 *  @brief  Destroys the IDManager class.
 */
/*==========================================================================*/
IDManager::~IDManager()
{
    m_flip_table.clear();
    m_id_list.clear();
}

/*===========================================================================*/
/**
 *  @brief  Returns the ID pair specified by the given index.
 *  @param  index [in] index
 */
/*===========================================================================*/
IDManager::IDPair IDManager::id( size_t index ) const
{
    auto p = m_id_list.begin();
    std::advance( p, m_flip_table[ index ] );
    return *p;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of pairs of the ID
 *  @return numebr of pairs of the ID
 */
/*===========================================================================*/
size_t IDManager::size() const
{
    return m_id_list.size();
}

/*===========================================================================*/
/**
 *  @brief  Inserts the object ID and renderer ID.
 *  @param  object_id [in] object ID
 *  @param  renderer_id [in] renderer ID
 */
/*===========================================================================*/
void IDManager::insert( int object_id, int renderer_id )
{
    m_id_list.push_back( IDPair( object_id, renderer_id ) );
    this->update_flip_table();
}

/*===========================================================================*/
/**
 *  @brief  Inserts the object ID.
 *  @param  object_id [in] object ID
 *
 *  The renderer stored in last of the ID list will be used for rendering the
 *  inserted object.
 */
/*===========================================================================*/
void IDManager::insertObjectID( int object_id )
{
    KVS_ASSERT( m_id_list.size() > 0 );
    this->insert( object_id, m_id_list.back().second );
    this->update_flip_table();
}

/*===========================================================================*/
/**
 *  @brief  Inserts the renderer ID.
 *  @param  renderer_id [in] renderer ID
 *
 *  The object stored in last of the ID list will be renderered by using the
 *  inserted renderer.
 */
/*===========================================================================*/
void IDManager::insertRendererID( int renderer_id )
{
    KVS_ASSERT( m_id_list.size() > 0 );
    this->insert( m_id_list.back().first, renderer_id );
    this->update_flip_table();
}

/*===========================================================================*/
/**
 *  @brief  Returns the object ID list paired by the specified renderer ID.
 *  @param  renderer_id [in] renderer ID
 *  @return object ID list
 */
/*===========================================================================*/
std::vector<int> IDManager::objectID( int renderer_id ) const
{
    std::vector<int> object_ids;
    for ( const auto& p : m_id_list )
    {
        if ( renderer_id == p.second ) { object_ids.push_back( p.first ); }
    }
    return object_ids;
}

/*===========================================================================*/
/**
 *  @brief  Returns the object ID stored in the last of the ID list.
 *  @return object ID
 */
/*===========================================================================*/
int IDManager::objectID() const
{
    return m_id_list.back().first;
}

/*===========================================================================*/
/**
 *  @brief  Returns the renderer ID list paired by the specified object ID.
 *  @param  object_id [in] object ID
 *  @return renderer ID list
 */
/*===========================================================================*/
std::vector<int> IDManager::rendererID( int object_id ) const
{
    std::vector<int> renderer_ids;
    for ( const auto& p : m_id_list )
    {
        if ( object_id == p.first ) { renderer_ids.push_back( p.second ); }
    }
    return renderer_ids;
}

/*===========================================================================*/
/**
 *  @brief  Returns the renderer ID stored in the last of the ID list.
 *  @return renderer ID
 */
/*===========================================================================*/
int IDManager::rendererID() const
{
    return m_id_list.back().second;
}

/*===========================================================================*/
/**
 *  @brief  Erases the registered object IDs and renderer IDs.
 */
/*===========================================================================*/
void IDManager::erase()
{
    m_id_list.clear();
    m_flip_table.clear();
}

/*===========================================================================*/
/**
 *  @brief  Erases the registered IDs specified by given IDs.
 *  @param  object_id [in] object ID
 *  @param  renderer_id [in] renderer ID
 */
/*===========================================================================*/
void IDManager::erase( int object_id, int renderer_id )
{
    m_id_list.remove_if( [&]( IDPair& p ) { return object_id == p.first && renderer_id == p.second; } );
    this->update_flip_table();
}

/*===========================================================================*/
/**
 *  @brief  Erases the registered IDs specified by given object ID.
 *  @param  object_id [in] object ID
 */
/*===========================================================================*/
void IDManager::eraseByObjectID( int object_id )
{
    m_id_list.remove_if( [&]( IDPair& p ) { return object_id == p.first; } );
    this->update_flip_table();
}

/*===========================================================================*/
/**
 *  @brief  Erases the registered IDs specified by given renderer ID.
 *  @param  renderer_id [in] renderer ID
 */
/*===========================================================================*/
void IDManager::eraseByRendererID( int renderer_id )
{
    m_id_list.remove_if( [&]( IDPair& p ) { return renderer_id == p.second; } );
    this->update_flip_table();
}

/*===========================================================================*/
/**
 *  @brief  Changes object ID stored in the last of the list by given object ID.
 *  @param  object_id [in] object ID
 */
/*===========================================================================*/
void IDManager::changeObject( int object_id )
{
    m_id_list.back().first = object_id;
}

/*===========================================================================*/
/**
 *  @brief  Changes object ID paired by given renderer ID by given object ID.
 *  @param  renderer_id [in] renderer ID
 *  @param  object_id [in] object ID
 */
/*===========================================================================*/
void IDManager::changeObject( int renderer_id, int object_id )
{
    for ( auto p = m_id_list.rbegin(); p != m_id_list.rend(); p++ )
    {
        if ( renderer_id == p->second )
        {
            p->first = object_id;
            return;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Changes object ID paired by given ID pair by given object ID.
 *  @param  id_pair [in] pair of IDs
 *  @param  object_id [in] object ID
 */
/*===========================================================================*/
void IDManager::changeObject( const IDPair& id_pair, int object_id )
{
    for ( auto p = m_id_list.rbegin(); p != m_id_list.rend(); p++ )
    {
        if ( *p == id_pair )
        {
            p->first = object_id;
            return;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Changes renderer ID stored in the last of the list by given renderer ID.
 *  @param  renderer_id [in] renderer ID
 */
/*===========================================================================*/
void IDManager::changeRenderer( int renderer_id )
{
    m_id_list.back().second = renderer_id;
}

/*===========================================================================*/
/**
 *  @brief  Changes renderer ID paired by given object ID by given renderer ID.
 *  @param  object_id [in] object ID
 *  @param  renderer_id [in] renderer ID
 */
/*===========================================================================*/
void IDManager::changeRenderer( int object_id, int renderer_id )
{
    for ( auto p = m_id_list.rbegin(); p != m_id_list.rend(); p++ )
    {
        if ( object_id == p->first )
        {
            p->second = renderer_id;
            return;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Changes renderer ID paired by given ID pair by given renderer ID.
 *  @param  id_pair [in] pair of IDs
 *  @param  renderer_id [in] renderer ID
 */
/*===========================================================================*/
void IDManager::changeRenderer( const IDPair& id_pair, int renderer_id )
{
    for ( auto p = m_id_list.rbegin(); p != m_id_list.rend(); p++ )
    {
        if ( *p == id_pair )
        {
            p->second = renderer_id;
            return;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Flips the ID list.
 */
/*===========================================================================*/
void IDManager::flip()
{
    std::vector<int> next( m_id_list.size() );
    std::copy( m_flip_table.begin(), m_flip_table.end(), next.begin() );
    std::next_permutation( next.begin(), next.end(), std::less<int>() );
    std::copy( next.begin(), next.end(), m_flip_table.begin() );
}

/*===========================================================================*/
/**
 *  @brief  Updates the flip table.
 */
/*===========================================================================*/
void IDManager::update_flip_table()
{
    m_flip_table.clear();
    const int n = this->size();
    for ( int i = 0; i < n; i++ )
    {
        m_flip_table.push_back( i );
    }
}

} // end of namespace kvs
