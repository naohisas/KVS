/****************************************************************************/
/**
 *  @file MessageBlock.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MessageBlock.h 1536 2013-04-15 02:21:56Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MESSAGE_BLOCK_H_INCLUDE
#define KVS__MESSAGE_BLOCK_H_INCLUDE

#include <kvs/ValueArray>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Message block class.
 */
/*==========================================================================*/
class MessageBlock
{
protected:

    /*     MessageBlock
     *
     *    --------------    ---
     *   | message_size |    |
     *    --------------     |
     *   |              |
     *   |              | m_block
     *   |   message    |
     *   |              |    |
     *   |              |    |
     *    --------------    ---
     */

    kvs::ValueArray<unsigned char> m_block; ///< message block

public:

    MessageBlock();
    MessageBlock( const void* data, const size_t data_size );
    MessageBlock( const std::string& data );
    template<typename T>
    MessageBlock( const std::vector<T>& data );
    virtual ~MessageBlock();

    size_t size() const;
    void* data();
    const void* data() const;
    size_t blockSize() const;
    void* blockData();
    const void* blockData() const;
    std::string toString() const;

    void copy( const void* data, const size_t data_size );
    void copy( const std::string& data );
    template <typename T>
    void copy( const std::vector<T>& data );
    void* allocate( const size_t data_size );
    void release();

public:

    KVS_DEPRECATED( void* pointer() ) { return this->data(); }
    KVS_DEPRECATED( const void* pointer() const ) { return this->data(); }
    KVS_DEPRECATED( void* blockPointer() ) { return this->blockData(); }
    KVS_DEPRECATED( const void* blockPointer() const ) { return this->blockData(); }
    KVS_DEPRECATED( void deallocate() ) { this->release(); }
};

} // end of namespace kvs

#endif // KVS__MESSAGE_BLOCK_H_INCLUDE
