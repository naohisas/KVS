/****************************************************************************/
/**
 *  @file   IDManager.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <list>
#include <vector>
#include <utility>
#include <cstdlib>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  ID manager class.
 */
/*==========================================================================*/
class IDManager
{
public:
    using IDPair = std::pair<int,int>;
    using IDList = std::list<IDPair>;
    using IDIterator = std::list<IDPair>::iterator;
    using IDReverseIterator = std::list<IDPair>::reverse_iterator;

private:
    std::vector<int> m_flip_table{}; ///< accessor to ID list
    IDList m_id_list{}; ///< ID list

public:
    IDManager();
    virtual ~IDManager();

    IDPair id( size_t index ) const;
    size_t size() const;
    void insert( int object_id, int renderer_id );
    void insertObjectID( int object_id );
    void insertRendererID( int renderer_id );
    std::vector<int> objectID( int renderer_id ) const;
    int objectID() const;
    std::vector<int> rendererID( int object_id ) const;
    int rendererID() const;
    void erase();
    void erase( int object_id, int renderer_id );
    void eraseByObjectID( int object_id );
    void eraseByRendererID( int renderer_id );
    void changeObject( int object_id );
    void changeObject( int renderer_id, int object_id );
    void changeObject( const IDPair& id_pair, int object_id );
    void changeRenderer( int renderer_id );
    void changeRenderer( int object_id, int renderer_id );
    void changeRenderer( const IDPair& id_pair, int renderer_id );
    void flip();

private:
    void update_flip_table();

    IDManager( const IDManager& );
    IDManager& operator =( const IDManager& );
};

} // end of namespace kvs
