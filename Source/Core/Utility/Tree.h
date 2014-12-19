/****************************************************************************/
/**
 *  @file   Tree.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Tree.h 1650 2013-10-31 07:08:58Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TREE_H_INCLUDE
#define KVS__TREE_H_INCLUDE

#include <cassert>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <set>
#include <queue>
#include <kvs/Assert>
#include <kvs/Message>
#include <kvs/Deprecated>


namespace kvs
{

namespace internal
{

/*==========================================================================*/
/**
 *  @struct Tree node structure.
 */
/*==========================================================================*/
template <typename T>
struct TreeNode
{
    TreeNode* parent; ///< pointer to the parent node
    TreeNode* first_child; ///< pointer to the first child node
    TreeNode* last_child; ///< pointer to the last child node
    TreeNode* prev_sibling; ///< pointer to the previous sibling node
    TreeNode* next_sibling; ///< pointer to the next sibling node
    T data; ///< node data

    TreeNode() {}
    TreeNode( const T& x ) : data( x ) {}
};

// Forward declarations of iterators.
template <typename T> class TreeIteratorBase;
template <typename T> class TreePreOrderIterator;
template <typename T> class TreePostOrderIterator;
template <typename T> class TreeSiblingIterator;
template <typename T> class TreeBreadthFirstIterator;
template <typename T> class TreeLeafIterator;

/*==========================================================================*/
/**
 *  @brief  Base class of the tree iterator.
 */
/*==========================================================================*/
template <typename T>
class TreeIteratorBase
{
    // Friend classes.
    friend class TreePreOrderIterator<T>;
    friend class TreePostOrderIterator<T>;
    friend class TreeSiblingIterator<T>;
    friend class TreeBreadthFirstIterator<T>;
    friend class TreeLeafIterator<T>;

public:

    // Type definitions.
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

protected:

    TreeNode<T>* m_node; ///< pointer to the node
    bool m_skip_current_children; ///< check flag

public:

    TreeIteratorBase():
        m_node( 0 ),
        m_skip_current_children( false ) {}

    TreeIteratorBase( TreeNode<T>* node ):
        m_node( node ),
        m_skip_current_children( false ) {}

    virtual ~TreeIteratorBase() {}

public:

    virtual TreeIteratorBase&  operator ++ () = 0;
    virtual TreeIteratorBase&  operator -- () = 0;

    TreeIteratorBase& operator += ( unsigned int num )
    {
        while ( num > 0 ) { ++( *this ); --num; }
        return *this;
    }

    TreeIteratorBase& operator -= ( unsigned int num )
    {
        while ( num > 0 ) { --( *this ); --num; }
        return *this;
    }

    bool operator == ( const TreeIteratorBase& other ) const
    {
        return m_node == other.m_node;
    }

    bool operator != ( const TreeIteratorBase& other ) const
    {
        return !( *this == other );
    }

public:

    reference operator * () const
    {
        return m_node->data;
    }

    pointer operator -> () const
    {
        return &( m_node->data );
    }

public:

    TreeSiblingIterator<T> begin() const
    {
        TreeSiblingIterator<T> ret( m_node->first_child );
        ret.m_parent = m_node;

        return ret;
    }

    TreeSiblingIterator<T> end() const
    {
        TreeSiblingIterator<T> ret( 0 );
        ret.m_parent = m_node;

        return ret;
    }

    TreeNode<T>* node()
    {
        return m_node;
    }

    TreeNode<T>* node() const
    {
        return m_node;
    }

    void skipChildren()
    {
        m_skip_current_children = true;
    }

    unsigned int numberOfChildren() const
    {
        TreeNode<T>* p = m_node->first_child;
        if ( p == 0 ) return 0;

        unsigned int counter = 1;
        while ( p != m_node->last_child )
        {
            ++counter;
            p = p->next_sibling;
        }

        return counter;
    }
};

/*==========================================================================*/
/**
 *  @brief  Pre-order iterator.
 */
/*==========================================================================*/
template <typename T>
class TreePreOrderIterator : public TreeIteratorBase<T>
{
    using TreeIteratorBase<T>::m_node;
    using TreeIteratorBase<T>::m_skip_current_children;

public:

    TreePreOrderIterator():
        TreeIteratorBase<T>( 0 ) {}

    TreePreOrderIterator( TreeNode<T>* node ):
        TreeIteratorBase<T>( node ) {}

    TreePreOrderIterator( const TreeIteratorBase<T>& other ):
        TreeIteratorBase<T>( other.node() ) {}

    TreePreOrderIterator( const TreeSiblingIterator<T>& other ):
        TreeIteratorBase<T>( other.node() )
    {
        if ( m_node == 0 )
        {
            if ( other.rangeLast() != 0 )
            {
                m_node = other.rangeLast();
            }
            else
            {
                m_node = other.m_parent;
            }

            TreeIteratorBase<T>::skipChildren();
            ++( *this );
        }
    }

public:

    virtual TreeIteratorBase<T>& operator ++ ()
    {
        KVS_ASSERT( m_node != 0 );

        if ( !m_skip_current_children &&
            m_node->first_child != 0 )
        {
            m_node = m_node->first_child;
        }
        else
        {
            m_skip_current_children = false;

            while ( m_node->next_sibling == 0 )
            {
                m_node = m_node->parent;
                if ( m_node == 0 ) return *this;
            }
            m_node = m_node->next_sibling;
        }

        return *this;
    }

    virtual TreeIteratorBase<T>& operator -- ()
    {
        KVS_ASSERT( m_node != 0 );

        if ( m_node->prev_sibling )
        {
            m_node = m_node->prev_sibling;
            while ( m_node->last_child )
            {
                m_node = m_node->last_child;
            }
        }
        else
        {
            m_node = m_node->parent;
            if ( m_node == 0 ) return *this;
        }

        return *this;
    }
};

/*==========================================================================*/
/**
 *  @brief  Post-order iterator.
 */
/*==========================================================================*/
template <typename T>
class TreePostOrderIterator : public TreeIteratorBase<T>
{
    using TreeIteratorBase<T>::m_node;
    using TreeIteratorBase<T>::m_skip_current_children;

public:

    TreePostOrderIterator():
        TreeIteratorBase<T>( 0 ) {}

    TreePostOrderIterator( TreeNode<T>* t ):
        TreeIteratorBase<T>( t ) {}

    TreePostOrderIterator( const TreeIteratorBase<T>& other ):
        TreeIteratorBase<T>( other.node() ) {}

    TreePostOrderIterator( const TreeSiblingIterator<T>& other ):
        TreeIteratorBase<T>( other.node() )
    {
        if ( m_node == 0 )
        {
            if ( other.range_last() != 0 ) m_node = other.range_last();
            else m_node = other.parent;
            TreeIteratorBase<T>::skip_children();
            ++( *this );
        }
    }

public:

    virtual TreeIteratorBase<T>& operator ++ ()
    {
        KVS_ASSERT( m_node != 0 );

        if ( m_node->next_sibling == 0 )
        {
            m_node = m_node->parent;
        }
        else
        {
            m_node = m_node->next_sibling;
            if ( m_skip_current_children )
            {
                m_skip_current_children = false;
            }
            else
            {
                while ( m_node->first_child ) m_node = m_node->first_child;
            }
        }

        return *this;
    }

    virtual TreeIteratorBase<T>&  operator--()
    {
        KVS_ASSERT( m_node != 0 );

        if ( m_skip_current_children || m_node->last_child == 0 )
        {
            m_skip_current_children = false;

            while ( m_node->prev_sibling == 0 ) m_node = m_node->parent;
            m_node = m_node->prev_sibling;
        }
        else
        {
            m_node = m_node->last_child;
        }

        return *this;
    }

    void descend_all()
    {
        KVS_ASSERT( m_node != 0 );

        while ( m_node->first_child ) m_node = m_node->first_child;
    }
};

/*==========================================================================*/
/**
 *  @brief  Sibling iterator.
 */
/*==========================================================================*/
template <typename T>
class TreeSiblingIterator : public TreeIteratorBase<T>
{
    using TreeIteratorBase<T>::m_node;

public:

    TreeNode<T>* m_parent;

public:

    TreeSiblingIterator():
        TreeIteratorBase<T>() {}

    TreeSiblingIterator( const TreeSiblingIterator<T>& other ):
        TreeIteratorBase<T>( other ),
        m_parent( other.m_parent ) {}

    TreeSiblingIterator( const TreeIteratorBase<T>& other ):
        TreeIteratorBase<T>( other.node() )
    {
        this->set_parent();
    }

    TreeSiblingIterator( TreeNode<T>* t ):
        TreeIteratorBase<T>( t )
    {
        this->set_parent();
    }

    virtual TreeIteratorBase<T>& operator ++ ()
    {
        if ( m_node ) m_node = m_node->next_sibling;

        return *this;
    }

    virtual TreeIteratorBase<T>& operator -- ()
    {
        if ( m_node )
        {
            m_node = m_node->prev_sibling;
        }
        else
        {
            KVS_ASSERT( m_parent != 0 );

            m_node = m_parent->last_child;
        }

        return *this;
    }

    TreeNode<T>* rangeFirst() const
    {
        TreeNode<T>* tmp = m_parent->first_child;
        return tmp;
    }

    TreeNode<T>* rangeLast() const
    {
        return m_parent->last_child;
    }

private:

    void set_parent()
    {
        m_parent = 0;

        if ( m_node == 0 ) return;
        if ( m_node->parent != 0 ) m_parent = m_node->parent;
    }
};

/*==========================================================================*/
/**
 *  Breadth first iterator.
 */
/*==========================================================================*/
template <typename T>
class TreeBreadthFirstIterator : public TreeIteratorBase<T>
{
    using TreeIteratorBase<T>::m_node;

private:

    std::queue<TreeNode<T>*> m_node_queue;

public:

    TreeBreadthFirstIterator():
        TreeIteratorBase<T>() {}

    TreeBreadthFirstIterator( TreeNode<T>* t ):
        TreeIteratorBase<T>( t )
    {
        m_node_queue.push( t );
    }

    TreeBreadthFirstIterator( const TreeIteratorBase<T>& other ):
        TreeIteratorBase<T>( other.node() )
    {
        m_node_queue.push( other.node() );
    }

    virtual TreeIteratorBase<T>& operator ++ ()
    {
        TreeSiblingIterator<T> sibling = this->begin();
        while ( sibling != this->end() )
        {
            m_node_queue.push( sibling.node() );
            ++sibling;
        }

        m_node_queue.pop();
        if ( m_node_queue.size() > 0 )
        {
            m_node = m_node_queue.front();
        }
        else
        {
            m_node = 0;
        }

        return *this;
    }

private:

    virtual TreeIteratorBase<T>&  operator -- ()
    {
        KVS_ASSERT( false );
        return *this;
    }
};

/*==========================================================================*/
/**
 *  Leaf iterator.
 */
/*==========================================================================*/
template <typename T>
class TreeLeafIterator : public TreeIteratorBase<T>
{
    using TreeIteratorBase<T>::m_node;

public:

    TreeLeafIterator():
        TreeIteratorBase<T>( 0 ) {}

    TreeLeafIterator( TreeNode<T>* t ):
        TreeIteratorBase<T>( t ) {}

    TreeLeafIterator( const TreeIteratorBase<T>& other ):
        TreeIteratorBase<T>( other.node() ) {}

    TreeLeafIterator( const TreeSiblingIterator<T>& other ):
        TreeIteratorBase<T>( other.node() )
    {
        if ( m_node == 0 )
        {
            if ( other.range_last() != 0 )
            {
                m_node = other.range_last();
            }
            else
            {
                m_node = other.m_parent;
            }

            ++( *this );
        }
    }

    virtual TreeIteratorBase<T>& operator ++ ()
    {
        while ( m_node->next_sibling == 0 )
        {
            if ( m_node->parent == 0 ) return *this;
            m_node = m_node->parent;
        }

        m_node = m_node->next_sibling;

        while ( m_node->last_child )
        {
            m_node = m_node->first_child;
        }

        return *this;
    }

    virtual TreeIteratorBase<T>& operator -- ()
    {
        while ( m_node->prev_sibling == 0 )
        {
            if ( m_node->parent == 0 ) return *this;
            m_node = m_node->parent;
        }

        m_node = m_node->prev_sibling;

        while ( m_node->last_child )
        {
            m_node = m_node->last_child;
        }

        return *this;
    }
};

} // end of namespace internal


/*==========================================================================*/
/**
 *  Tree class.
 */
/*==========================================================================*/
template <typename T>
class Tree
{
public:

    // Type definitions.
    typedef T value_type;
    typedef Tree<T> tree_type;
    typedef internal::TreeNode<T> node_type;
    typedef internal::TreePreOrderIterator<T> iterator;
    typedef internal::TreeIteratorBase<T> iterator_base;
    typedef internal::TreePreOrderIterator<T> pre_order_iterator;
    typedef internal::TreePostOrderIterator<T> post_order_iterator;
    typedef internal::TreeSiblingIterator<T> sibling_iterator;
    typedef internal::TreeBreadthFirstIterator<T> breadth_first_iterator;
    typedef internal::TreeLeafIterator<T> leaf_iterator;

protected:

    node_type* m_head; ///< head node

public:

    Tree()
    {
        m_head = new node_type;
        m_head->parent       = 0;
        m_head->first_child  = 0;
        m_head->last_child   = 0;
        m_head->prev_sibling = m_head;
        m_head->next_sibling = m_head;
    }

    Tree( const T& t )
    {
        m_head = new node_type;
        m_head->parent       = 0;
        m_head->first_child  = 0;
        m_head->last_child   = 0;
        m_head->prev_sibling = m_head;
        m_head->next_sibling = m_head;
        this->insert( begin(), t );
    }

    virtual ~Tree()
    {
        this->clear();
        delete m_head;
    }

public:

    pre_order_iterator begin() const
    {
        return pre_order_iterator( m_head->next_sibling );
    }

    pre_order_iterator end() const
    {
        return pre_order_iterator( m_head );
    }

    post_order_iterator beginPost() const
    {
        node_type* tmp = m_head->next_sibling;

        if ( tmp != m_head )
        {
            while ( tmp->first_child ) tmp = tmp->first_child;
        }

        return post_order_iterator( tmp );
    }

    post_order_iterator endPost() const
    {
        return post_order_iterator( m_head );
    }

    sibling_iterator begin( const iterator_base& pos ) const
    {
        if ( pos.node()->first_child == 0 )
        {
            return end( pos );
        }

        return pos.node()->first_child;
    }

    sibling_iterator end( const iterator_base& pos ) const
    {
        sibling_iterator ret( 0 );
        ret.m_parent = pos.node();

        return ret;
    }

    breadth_first_iterator beginBreadth() const
    {
        return breadth_first_iterator( m_head->next_sibling );
    }

    breadth_first_iterator endBreadth() const
    {
        breadth_first_iterator e;
        return e;
    }

    leaf_iterator beginLeaf() const
    {
        node_type* tmp = m_head->next_sibling;

        if ( tmp != m_head )
        {
            while ( tmp->first_child ) tmp = tmp->first_child;
        }

        return leaf_iterator( tmp );
    }

    leaf_iterator endLeaf() const
    {
        return leaf_iterator( m_head );
    }

public:

    void clear()
    {
        if ( m_head )
        {
            while ( m_head->next_sibling != m_head )
            {
                this->erase( pre_order_iterator( m_head->next_sibling ) );
            }
        }
    }

    template <typename Iter>
    Iter erase( Iter it )
    {
        node_type* cur = it.node();

        KVS_ASSERT( cur != m_head );

        Iter ret = it;
        ret.skipChildren();
        ++ret;

        this->eraseChildren( it );
        if ( cur->prev_sibling == 0 )
        {
            cur->parent->first_child = cur->next_sibling;
        }
        else
        {
            cur->prev_sibling->next_sibling = cur->next_sibling;
        }

        if ( cur->next_sibling == 0 )
        {
            cur->parent->last_child = cur->prev_sibling;
        }
        else
        {
            cur->next_sibling->prev_sibling = cur->prev_sibling;
        }

        delete cur;

        return ret;
    }

    void eraseChildren( const iterator_base& it )
    {
        node_type* cur  = it.node()->first_child;
        node_type* prev = 0;

        while ( cur != 0 )
        {
            prev = cur;
            cur  = cur->next_sibling;

            this->eraseChildren( pre_order_iterator( prev ) );
            delete prev;
        }

        it.node()->first_child = 0;
        it.node()->last_child  = 0;
    }

public:

    // insert node as last child of node pointed to by position (first one inserts empty node)
    template <typename Iter>
    Iter appendChild( Iter position )
    {
        KVS_ASSERT( position.node() != m_head );

        node_type* tmp = new node_type;
        tmp->first_child = 0;
        tmp->last_child  = 0;

        tmp->parent() = position.node;
        if ( position.node()->last_child != 0 )
        {
            position.node()->last_child->next_sibling = tmp;
        }
        else
        {
            position.node()->first_child = tmp;
        }
        tmp->prev_sibling = position.node->last_child;
        position.node->last_child = tmp;
        tmp->next_sibling = 0;

        return tmp;
    }

    template <typename Iter>
    Iter appendChild( Iter position, const T& x )
    {
        KVS_ASSERT( position.node() != m_head );

        node_type* tmp = new node_type( x );
        tmp->first_child = 0;
        tmp->last_child  = 0;

        tmp->parent = position.node();
        if ( position.node()->last_child != 0 )
        {
            position.node()->last_child->next_sibling = tmp;
        }
        else
        {
            position.node()->first_child = tmp;
        }

        tmp->prev_sibling = position.node()->last_child;
        position.node()->last_child = tmp;
        tmp->next_sibling = 0;

        return tmp;
    }

public:

    // insert node as previous sibling of node pointed to by position
    template <typename Iter>
    Iter insert( Iter position, const T& x )
    {
        node_type* tmp = new node_type( x );
        tmp->first_child = 0;
        tmp->last_child  = 0;

        tmp->parent       = position.node()->parent;
        tmp->next_sibling = position.node();
        tmp->prev_sibling = position.node()->prev_sibling;
        position.node()->prev_sibling = tmp;

        if ( tmp->prev_sibling == 0 )
        {
            tmp->parent->first_child = tmp;
        }
        else
        {
            tmp->prev_sibling->next_sibling = tmp;
        }

        return tmp;
    }

    // insert node as previous sibling of node pointed to by position
    sibling_iterator insert( sibling_iterator position, const T& x )
    {
        node_type* tmp = new node_type( x );
        tmp->first_child = 0;
        tmp->last_child  = 0;

        tmp->next_sibling = position.node();
        if ( position.node() == 0 )
        { // iterator points to end of a subtree
            tmp->parent       = position.parent;
            tmp->prev_sibling = position.range_last();
        }
        else
        {
            tmp->parent       = position.node()->parent;
            tmp->prev_sibling = position.node()->prev_sibling;
            position.node()->prev_sibling = tmp;
        }

        if ( tmp->prev_sibling == 0 )
        {
            tmp->parent->first_child = tmp;
        }
        else
        {
            tmp->prev_sibling->next_sibling = tmp;
        }

        return tmp;
    }

public:

    // count the total number of nodes
    int size() const
    {
        pre_order_iterator it  = begin();
        pre_order_iterator eit = end();

        int i = 0;
        while ( it != eit ){ ++i; ++it; }

        return i;
    }

    // compute the depth to the root
    int depth( const iterator_base& it ) const
    {
        node_type* pos = it.node();

        KVS_ASSERT( pos != 0 );

        int ret = 0;
        while ( pos->parent != 0 ){ pos = pos->parent; ++ret; }

        return ret;
    }

    // count the number of children of node at position
    unsigned int numberOfChildren( const iterator_base& it ) const
    {
        node_type* pos = it.node()->first_child;
        if ( pos == 0 ) return( 0 );

        unsigned int ret = 1;
        while ( ( pos = pos->next_sibling ) ) ++ret;

        return ret;
    }

    // count the number of 'next' siblings of node at iterator
    unsigned int numberOfSiblings( const iterator_base& it ) const
    {
        node_type* pos = it.node();

        unsigned int ret = 1;
        while ( pos->next_sibling && pos->next_sibling != m_head )
        {
            ++ret;
            pos = pos->next_sibling;
        }

        return ret;
    }
};

} // end of namespace kvs

#endif // KVS__TREE_H_INCLUDE
