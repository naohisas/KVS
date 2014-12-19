/*****************************************************************************/
/**
 *  @file   Exception.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS_EXCEPTION_H_INCLUDE
#define KVS_EXCEPTION_H_INCLUDE

#include <exception>
#include <string>
#include <sstream>
#include <kvs/Message>


#if KVS_ENABLE_DEBUG
#if 0
#define KVS_THROW( EXCEPTION, message ) \
    ( ( kvsMessageError( message ) ), \
    ( kvs::detail::Throw<EXCEPTION>( message, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ) ) )
#endif
// Temporal workaround
#define KVS_THROW( EXCEPTION, message ) \
    ( ( kvsMessageError( std::string( message ).c_str() ) ), \
     ( kvs::detail::Throw<EXCEPTION>( message, KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ) ) )
#else
#define KVS_THROW( EXCEPTION, message ) throw EXCEPTION( message )
#endif


namespace kvs
{

namespace detail
{
    inline std::string MakeSourceInfo( const char* file, int line, const char* func )
    {
        std::stringstream ss;
        ss << "FILE: " << file << " (" << line << ")" << "\n";
        ss << "FUNC: " << func << "\n";
        return ss.str();
    }

    template <typename ExceptionType>
    inline void Throw( const std::string& message, const char* file, int line, const char* func )
    {
        throw ExceptionType( message + "\n" + MakeSourceInfo( file, line, func ) );
    }
}

class Exception : public std::exception
{
protected:
    explicit Exception( const std::string& message )
        : m_message( message )
    {}

    ~Exception() throw() {}

public:
    const char* what() const throw()
    {
        return m_message.c_str();
    }

    void addMessage( const std::string& str )
    {
        m_message += str;
    }

private:
    std::string m_message;
}; // Exception


class NullPointingException : public kvs::Exception
{
public:
    explicit NullPointingException( const std::string& message )
        : kvs::Exception( message ) {}
};

class ArgumentException : public kvs::Exception
{
public:
    explicit ArgumentException( const std::string& message )
        : kvs::Exception( message ) {}
};

class NotSupportedException : public kvs::Exception
{
public:
    explicit NotSupportedException( const std::string& message )
        : kvs::Exception( message ) {}
};

class FileReadFaultException : public kvs::Exception
{
public:
    explicit FileReadFaultException( const std::string& message )
        : kvs::Exception( message ) {}
};

class FileWriteFaultException : public kvs::Exception
{
public:
    explicit FileWriteFaultException( const std::string& message )
        : kvs::Exception( message ) {}
};

class InvalidDataException : public kvs::Exception
{
public:
    explicit InvalidDataException( const std::string& message )
        : kvs::Exception( message ) {}
};

class OpenGLException : public kvs::Exception
{
public:
    explicit OpenGLException( const std::string& message )
        : kvs::Exception( message ) {}
};

} // kvs

#endif
