/****************************************************************************/
/**
 *  @file Message.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Message.cpp 1297 2012-08-26 08:54:00Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "Message.h"
#include <kvs/ColorStream>


namespace kvs
{

Message::Handler Message::m_handler = Message::DefaultHandler;

/*===========================================================================*/
/**
 *  @brief  Default message handler (output message format)
 *  @param  stream [in] output stream
 *  @param  type [in] message type
 *  @param  context [in] message context
 *  @param  message [in] message
 */
/*===========================================================================*/
void Message::DefaultHandler(
    std::ostream& stream,
    const kvs::Message::Type type,
    const kvs::Message::Context& context,
    const std::string& message )
{
//    std::ostream* stream = &std::cerr;

    stream << kvs::ColorStream::Bold;
    switch ( type )
    {
    case kvs::Message::Error:
    {
        stream << kvs::ColorStream::Red( "KVS ERROR" ) << ": ";
        break;
    }
    case kvs::Message::Warning:
    {
        stream << kvs::ColorStream::Yellow( "KVS WARNING" ) << ": ";
        break;
    }
    case kvs::Message::Debug:
    {
        stream << kvs::ColorStream::Blue( "KVS DEBUG" ) << ": ";
        break;
    }
    case kvs::Message::Assert:
    {
        stream << kvs::ColorStream::Red( "KVS ASSERT" ) << ": ";
        break;
    }
    default: break;
    }

    stream << kvs::ColorStream::Underline << message << kvs::ColorStream::Reset << std::endl;
    stream << "    " << "Func: " << context.func << std::endl;
    stream << "    " << "File: " << context.file << ":" << context.line << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Set a new message handler.
 *  @param  handler [in] handler function
 */
/*===========================================================================*/
void Message::SetHandler( Message::Handler handler )
{
    if ( !handler ) { m_handler = DefaultHandler; }
    else { m_handler = handler; }
}

} // end of namespace kvs
