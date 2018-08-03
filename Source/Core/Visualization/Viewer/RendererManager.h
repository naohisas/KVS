/****************************************************************************/
/**
 *  @file   RendererManager.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RendererManager.h 1703 2014-01-23 10:52:11Z naohisa.sakamoto@gmail.com $
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
*  Renderer manager class.
*/
/*==========================================================================*/
class RendererManager
{
private:
    typedef kvs::SharedPointer<kvs::RendererBase> RendererPointer;
    typedef std::vector<std::pair<int, RendererPointer> > RendererList;
    typedef RendererList::iterator RendererIterator;

private:
    RendererList m_renderer_list; ///< renderer list
    int m_renderer_id; ///< renderer ID

public:
    RendererManager();
    virtual ~RendererManager();

    int insert( kvs::RendererBase* renderer );
    void erase( bool delete_flag );
    void erase( int id, bool delete_flag );
    void erase( std::string name, bool delete_flag );
    void change( int id, kvs::RendererBase* renderer, bool delete_flag );
    void change( std::string name, kvs::RendererBase* renderer, bool delete_flag );
    kvs::RendererBase* renderer();
    kvs::RendererBase* renderer( int id );
    kvs::RendererBase* renderer( std::string name );

    int numberOfRenderers() const;
    bool hasRenderer() const;
    int rendererID( const kvs::RendererBase* renderer ) const;

    int insert( const kvs::SharedPointer<kvs::RendererBase>& renderer );
    void erase();
    void erase( int id );
    void erase( std::string name );
    void change( int id, const kvs::SharedPointer<kvs::RendererBase>& renderer );
    void change( std::string name, const kvs::SharedPointer<kvs::RendererBase>& renderer );

    //const kvs::SharedPointer<kvs::RendererBase>& renderer( int id );
    //const kvs::SharedPointer<kvs::RendererBase>& renderer( std::string name );

private:
    RendererIterator find_renderer( int id );
    RendererIterator find_renderer( std::string name );

private:
    RendererManager( const RendererManager& );
    RendererManager& operator =( const RendererManager& );

public:
    KVS_DEPRECATED( int nrenderers() const ) { return numberOfRenderers(); }
};

} // end of namespace kvs
