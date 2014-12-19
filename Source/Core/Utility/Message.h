/****************************************************************************/
/**
 *  @file Message.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Message.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MESSAGE_H_INCLUDE
#define KVS__MESSAGE_H_INCLUDE

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <kvs/Macro>
#include <kvs/Platform>

// Error message
#define kvsMessageError \
    ( kvs::Message( kvs::Message::Error, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ) )

// Debug message
#if defined ( KVS_ENABLE_DEBUG )
#define kvsMessageDebug( message ) \
    ( kvs::Message( kvs::Message::Debug, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ) ( message ) )
#else
#define kvsMessageDebug( message )
#endif

// Warning message
#if defined ( KVS_ENABLE_DEBUG )
#define kvsMessageWarning( expression, message ) \
    ( kvs::Message( kvs::Message::Warning, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, ( expression ) ) ( message ) )
#else
#define kvsMessageWarning( expression, message )
#endif

// Assert message
#if defined ( KVS_ENABLE_DEBUG )
#define kvsMessageAssert( expression, message ) \
    ( kvs::Message( kvs::Message::Assert, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, ( expression ) ) ( message ) )
#else
#define kvsMessageAssert( expression, message )
#endif


#define KVS_MESSAGE_RED    "31"
#define KVS_MESSAGE_GREEN  "32"
#define KVS_MESSAGE_BROWN  "33"
#define KVS_MESSAGE_BLUE   "34"
#define KVS_MESSAGE_PURPLE "35"
#define KVS_MESSAGE_SKY    "36"
#define KVS_MESSAGE_WHITE  "37"
#define KVS_MESSAGE_NORMAL "0"


#if defined ( KVS_PLATFORM_WINDOWS )
#define KVS_MESSAGE_SET_COLOR( color ) ""
#define KVS_MESSAGE_RESET_COLOR        ""
#else
#define KVS_MESSAGE_CHANGE_COLOR_BEGIN
#define KVS_MESSAGE_SET_COLOR( color ) "\x1b[" << color << "m"
#define KVS_MESSAGE_RESET_COLOR        "\x1b[0m"
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  Message class.
 */
/*==========================================================================*/
class Message
{
public:

    enum MessageType
    {
        Error = 0,
        Warning,
        Debug,
        Assert,
        NumberOfMessageTypes
    };

protected:

    MessageType m_type; ///< message type
    const char* m_file; ///< file name
    int         m_line; ///< line number
    const char* m_func; ///< function name
    bool        m_expr; ///< expression

public:

    Message( MessageType type, const char* file, int line, const char* func, bool expr = false );

    void operator ()( const char* msg, ... );
};

} // end of namespace kvs

#endif // KVS__MESSAGE_H_INCLUDE
