/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Tree.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/Tree>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    /* Construct a sample tree as follows:
     *
     *   Root
     *    |
     *    +-- Node A
     *    |    |
     *    |    +-- Node B
     *    |    |
     *    |    +-- Node C
     *    |
     *    +-- Node D
     *         |
     *         +-- Node E
     *         |
     *         +-- Node F
     *         |
     *         +-- Node G
     *
     */
    kvs::Tree<std::string> tree;
    kvs::Tree<std::string>::iterator head    = tree.begin();
    kvs::Tree<std::string>::iterator root    = tree.insert(       head,   "Root" );
    kvs::Tree<std::string>::iterator node_a  = tree.appendChild( root,   "Node A" );
                                  /* node_b */ tree.appendChild( node_a, "Node B" );
                                  /* node_c */ tree.appendChild( node_a, "Node C" );
    kvs::Tree<std::string>::iterator node_d  = tree.appendChild( root,   "Node D" );
                                  /* node_e */ tree.appendChild( node_d, "Node E" );
                                  /* node_f */ tree.appendChild( node_d, "Node F" );
                                  /* node_g */ tree.appendChild( node_d, "Node G" );

    std::cout << "Tree 1:" << std::endl;
    std::cout << std::endl;

    // Standard output by using the default iterator (pre_order_iterator).
    std::cout << "Standard output by using the iterator (pre_order_iterator):" << std::endl;
    kvs::Tree<std::string>::iterator first = tree.begin();
    kvs::Tree<std::string>::iterator last  = tree.end();
    for( ; first != last; ++first )
    {
        std::string indent( tree.depth(first) * 4, ' ' );
        std::cout << indent <<  (*first) << std::endl;
    }
    std::cout << std::endl;

    // Standard output by using the post_order_iterator.
    std::cout << "Standard output by using the post_order_iterator:" << std::endl;
    kvs::Tree<std::string>::post_order_iterator first_post = tree.beginPost();
    kvs::Tree<std::string>::post_order_iterator last_post  = tree.endPost();
    for( ; first_post != last_post; ++first_post )
    {
        std::string indent( tree.depth(first_post) * 4, ' ' );
        std::cout << indent <<  (*first_post) << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
