/*****************************************************************************/
/**
 *  @file   CellTree.h
 *  @author Naohisa Sakamoto, Guo Jiazhen
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*----------------------------------------------------------------------------
 *
 * References:
 * [1] C. Garth and K.I. Joy, "Fast, memory-efficient cell location in unstructured
 *     grids for visualization, IEEE Transactions on Visualization and Computer
 *     Graphics, Vol.16, No.6, pp.1541–1550, 2010.
 * [2] Guo Jiazhen, "An Efficient Hyperstreamline Visualization Method on Large
 *     -Scale Unstructured Datasets," Master Thesis, Kyoto Univeristy, 2012.
 */
/*****************************************************************************/
#pragma once
#include <vector>
#include <kvs/Type>
#include <kvs/UnstructuredVolumeObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Cell tree class.
 */
/*===========================================================================*/
class CellTree
{
public:

    struct Node
    {
        kvs::UInt32 index;
        union { struct { kvs::Real32 lmax, rmin; } node; struct { kvs::UInt32 size, start; } leaf; };

        void makeNode( kvs::UInt32 left, kvs::UInt32 d, kvs::Real32 b[2] ) { index = ( d & 3 ) | ( left << 2 ); node.lmax = b[0]; node.rmin = b[1]; }
        void makeLeaf( kvs::UInt32 st, kvs::UInt32 sz ) { index = 3; leaf.size = sz; leaf.start = st; }
        void setChildren( kvs::UInt32 left ) { index = this->dim() | ( left << 2 ); }
        bool isNode() const { return ( index & 3 ) != 3; }
        bool isLeaf() const { return index == 3; }
        kvs::UInt32 left() const { return ( index >> 2 ); }
        kvs::UInt32 right() const { return ( index >> 2 ) + 1; }
        kvs::UInt32 dim() const { return index & 3; }
    };

    struct PreTraversal
    {
        const CellTree& m_ct;
        kvs::UInt32 m_stack[32];
        kvs::UInt32* m_sp;
        const kvs::Real32* m_pos;

        PreTraversal( const CellTree& ct, const kvs::Real32* pos ):
            m_ct( ct ),
            m_pos( pos )
        {
            m_stack[0] = 0; // initialize stack pop to zero
            m_sp = m_stack + 1; // initialize stack pointer
        }

        const CellTree::Node* next()
        {
            while ( true )
            {
                // if stack is empty
                if ( m_sp == m_stack ) { return 0; }

                // &m_ct.nodes.front() is the address of the root node
                // move the stack pointer backward by 1, and take it's dereference
                const CellTree::Node* n = &m_ct.nodes.front() + *(--m_sp);

                if ( n->isLeaf() ) { return n; }

                const kvs::Real32 p = m_pos[ n->dim() ]; // the value corresponding to split dim of the node
                const kvs::UInt32 left = n->left(); // get the VECTOR STORAGE index of the left child

                bool l = p <= n->node.lmax;
                bool r = p > n->node.rmin;
                if ( l && r )
                {
                    if ( n->node.lmax - p < p - n->node.rmin )
                    {
                        *(m_sp++) = left;
                        *(m_sp++) = left + 1;
                    }
                    else
                    {
                        *(m_sp++) = left + 1;
                        *(m_sp++) = left;
                    }
                }
                else if ( l ) { *(m_sp++) = left; }
                else if ( r ) { *(m_sp++) = left + 1; }
            }
        }
    };

    struct PreTraversalCached
    {
        const CellTree& m_ct;
        kvs::UInt32 m_stack[32];
        kvs::UInt32* m_sp;
        const kvs::Real32* m_pos;

        PreTraversalCached( const CellTree& ct, const kvs::Real32* pos, const kvs::UInt32 previous_index ):
            m_ct( ct ),
            m_pos( pos )
        {
            // dangerous, be sure hint_stack and hint_sp are well initialized
            m_stack[0] = 0; // initialize stack pointer
            if ( previous_index != 0 )
            {
                m_stack[1] = previous_index;
                m_sp = m_stack + 2;
            }
            else
            {
                m_sp = m_stack + 1;
            }
        }

        const CellTree::Node* next()
        {
            while ( true )
            {
                // if stack is empty
                if ( m_sp == m_stack ) { return 0; }

                // &m_ct.nodes.front() is the address of the root node
                // move the stack pointer backward by 1, and take it's dereference
                const CellTree::Node* n = &m_ct.nodes.front() + *(--m_sp);

                if ( n->isLeaf() ) { return n; }

                const kvs::Real32 p = m_pos[ n->dim() ]; // the value corresponding to split dim of the node
                const kvs::UInt32 left = n->left(); // get the VECTOR STORAGE index of the left child

                bool l = p <= n->node.lmax;
                bool r = p > n->node.rmin;
                if ( l && r )
                {
                    if ( n->node.lmax-p < p-n->node.rmin )
                    {
                        *(m_sp++) = left;
                        *(m_sp++) = left + 1;
                    }
                    else
                    {
                        *(m_sp++) = left + 1;
                        *(m_sp++) = left;
                    }
                }
                else if( l ) { *(m_sp++) = left; }
                else if( r ) { *(m_sp++) = left + 1; }
            }
        }

        const unsigned int* stack() { return m_stack; }
        const unsigned int* sp() { return m_sp; }
    };

    struct InTraversalCached
    {
        struct Stack
        {
            kvs::UInt32 m_stack[32];
            kvs::UInt32* m_p_stack;

            Stack() { m_p_stack = m_stack; }
            kvs::UInt32 size() { return ( m_p_stack - m_stack ); }
            void push( const kvs::UInt32 num ) { if ( size() > 31 ) { kvsMessageError("Overflow"); return; } *(m_p_stack++) = num; }
            kvs::UInt32 pop() { if ( m_p_stack == m_stack ) { kvsMessageError("Empty"); return *m_p_stack; } return *(--m_p_stack); }
            kvs::UInt32 top() { if ( m_p_stack == m_stack ) { kvsMessageError("Empty"); } return *m_p_stack; }
            bool has( const kvs::UInt32 num ) const
            {
                kvs::UInt32* p_stack = m_p_stack;
                while ( p_stack-- != m_stack ) { if ( *p_stack == num ) { return true; } }
                return false;
            }
        };

        const CellTree& m_ct;
        kvs::UInt32 m_stack[32];
        kvs::UInt32* m_sp;
        const kvs::Real32* m_pos;
        Stack m_lrstack;

        InTraversalCached( const CellTree& ct, const kvs::Real32* pos, kvs::UInt32 hint_stack[32], kvs::UInt32* hint_sp ):
            m_ct( ct ),
            m_pos( pos )
        {
            memcpy( m_stack, hint_stack, 128 );
            int n = hint_sp - hint_stack; // initialize stack pointer 
            m_sp = m_stack + n;
        }

        const CellTree::Node* next()
        {
            while ( true )
            {
                // if returned to the root the second time
                if ( m_sp == m_stack ) { return 0; }

                // &m_ct.nodes.front() is the address of the root node
                // move the stack pointer backward by 1, and take it's dereference
                const CellTree::Node* n = &m_ct.nodes.front() + *(--m_sp);

                if ( n->isLeaf() ) { return n; }

                const kvs::Real32 p = m_pos[ n->dim() ]; // the value corresponding to split dim of the node
                const kvs::UInt32 left = n->left(); // get the VECTOR STORAGE index of the left child

                bool l = p <= n->node.lmax;
                bool r = p > n->node.rmin;
                if ( l && r )
                {
                    if ( !m_lrstack.has( *m_sp ) ) { m_lrstack.push( *m_sp ); }
                    else { continue; } // if already registered in lr_stack

                    if ( n->node.lmax-p < p-n->node.rmin )
                    {
                        m_sp++;
                        *(m_sp++) = left;
                        *(m_sp++) = left + 1;
                    }
                    else
                    {
                        m_sp++;
                        *(m_sp++) = left + 1;
                        *(m_sp++) = left;
                    }
                }
                else if ( l )
                {
                    if ( *(m_sp + 1) == left ) { continue; } // *(m_sp+1) = -1;
                    m_sp++; *(m_sp++) = left;
                }
                else if ( r )
                {
                    if ( *(m_sp + 1) == left + 1 ) { continue; } // *(m_sp+1) = -1;
                    m_sp++; *(m_sp++) = left + 1;
                }
            }
        }
    };

public:

    std::vector<Node> nodes; ///< nodes of the cell tree
    std::vector<kvs::UInt32> leaves; ///< leaves of the cell tree

public:

    CellTree();
    CellTree( const kvs::UnstructuredVolumeObject* volume, bool enable_mthreading = false );
    ~CellTree();

    kvs::UInt32 height( Node& node );
    void build( const kvs::UnstructuredVolumeObject* volume, bool enable_mthreading = false );
};

} // end of namespace kvs
