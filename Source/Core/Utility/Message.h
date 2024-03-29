/****************************************************************************/
/**
 *  @file   Message.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <sstream>
#include <kvs/Macro>
#include <kvs/Platform>
#include <kvs/ColorStream>
#include <kvs/NullStream>
#include <kvs/Breakpoint>


// Default message
#define kvsMessage                                                    \
    kvs::Message( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC )

// Error message
#define kvsMessageError                                               \
    kvs::Message( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ).error

// Warning message
#define kvsMessageWarning                                             \
    kvs::Message( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ).warning

// Debug message
#define kvsMessageDebug                                               \
    kvs::Message( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ).debug

// Assert message
#if defined ( KVS_ENABLE_DEBUG )
#define kvsMessageAssert                                              \
    kvs::Message( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ).assertion
#else
#define kvsMessageAssert(...)
#endif


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Message class.
 */
/*===========================================================================*/
class Message
{
public:
    enum Type
    {
        Default = 0, ///< default message
        Error, ///< error message
        Warning, ///< warning message
        Debug, ///< debug message
        Assert ///< assert message
    };

    struct Context
    {
        const char* file = 0; ///< file name
        int line = 0; ///< line number
        const char* func = 0; ///< function name
    };

    typedef void (*Handler)( std::ostream& stream, const kvs::Message::Type, const kvs::Message::Context&, const std::string& );

private:
    static Handler m_handler; ///< message handler

public:
    static void DefaultHandler( std::ostream& stream, const kvs::Message::Type type, const kvs::Message::Context& context, const std::string& message );
    static void SetHandler( Handler handler );

private:
    Type m_type = Message::Default; ///< message type
    Context m_context{}; ///< message context
    std::ostringstream m_message{}; ///< output message

public:
    Message( const char* file, int line, const char* func )
    {
        m_context.file = file;
        m_context.line = line;
        m_context.func = func;
    }

    Message& error()
    {
        m_type = Message::Error;
        return *this;
    }

    Message& warning()
    {
        m_type = Message::Warning;
        return *this;
    }

    Message& debug()
    {
        m_type = Message::Debug;
        return *this;
    }

    void error( const char* format, ... )
    {
        this->error();

        std::va_list args;
        va_start( args, format );
        m_message << this->format_arguments( format, args );
        va_end( args );

        this->dump();
    }

    void warning( const char* format, ... )
    {
        this->warning();

        std::va_list args;
        va_start( args, format );
        m_message << this->format_arguments( format, args );
        va_end( args );

        this->dump();
    }

    void debug( const char* format, ... )
    {
        this->debug();

#if defined ( KVS_ENABLE_DEBUG )
        std::va_list args;
        va_start( args, format );
        m_message << this->format_arguments( format, args );
        va_end( args );

        this->dump();
#endif
    }

    void assertion( const bool expr, const char* format, ... )
    {
        m_type = Message::Assert;

#if defined ( KVS_ENABLE_DEBUG )
        if ( !expr )
        {
            std::va_list args;
            va_start( args, format );
            m_message << this->format_arguments( format, args );
            va_end( args );

            this->dump();
        }
#endif
    }

    void dump()
    {
#if defined ( KVS_ENABLE_DEBUG )
        m_handler( std::cerr, m_type, m_context, m_message.str() );
        if ( m_type == kvs::Message::Error ) { KVS_BREAKPOINT; }
        if ( m_type == kvs::Message::Assert ) { KVS_BREAKPOINT; }
#else
        if ( m_type == kvs::Message::Debug )
        {
            // Disable output stream.
            std::cerr.setstate( std::ios_base::failbit );
            m_handler( std::cerr, m_type, m_context, m_message.str() );
            std::cerr.clear();
        }
        else
        {
            m_handler( std::cerr, m_type, m_context, m_message.str() );
        }
        if ( m_type == kvs::Message::Assert ) { KVS_BREAKPOINT; }
#endif
    }

    Message& operator () ()
    {
        return *this;
    }

    void operator () ( const char* format, ... )
    {
        std::va_list args;
        va_start( args, format );
        m_message << this->format_arguments( format, args );
        va_end( args );

        this->dump();
    }

    template <typename T>
    Message& operator << ( const T& message )
    {
        m_message << message;
        return *this;
    }

    Message& operator << ( std::ostream& (*os)(std::ostream&) )
    {
        this->dump();
        return *this;
    }

private:
    std::string format_arguments( const char* fmt, std::va_list args )
    {
        std::string buffer;
        buffer.resize( 64 );

        size_t n = std::vsnprintf( &buffer[0], buffer.size(), fmt, args );
        if ( n > buffer.size() )
        {
            buffer.resize( n + 1 );
            n = std::vsnprintf( &buffer[0], buffer.size(), fmt, args );
        }

        buffer.resize( n );
        return buffer;
    }
};

} // end of namespace kvs
