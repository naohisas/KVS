/****************************************************************************/
/**
 *  @file CommandLine.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CommandLine.h 1338 2012-11-04 02:45:57Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__COMMAND_LINE_H_INCLUDE
#define KVS__COMMAND_LINE_H_INCLUDE

#include <string>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <kvs/String>
#include <kvs/Message>


namespace kvs
{

/*==========================================================================*/
/**
 *  Commandline class
 */
/*==========================================================================*/
class CommandLine
{
public:

    enum HelpMessageMode
    {
        UsageOnly, ///< usage only mode
        UsageAndOption ///< usage and option mode
    };

public:

    typedef std::string Argument;
    class Option;
    class Value;

public:

    typedef std::vector<std::string> Arguments;
    typedef std::vector<Option> Options;
    typedef std::vector<Value> Values;

protected:

    int m_argc; ///< argument count
    char** m_argv; ///< argument values
    std::string m_command_name; ///< command name
    size_t m_max_length; ///< max length of the option name
    bool m_no_help; ///< no help option
    std::string m_help_option; ///< help option character (ex: 'h')
    Arguments m_arguments; ///< argument values
    Options m_options; ///< options
    Values m_values; ///< values

public:

    CommandLine();
    CommandLine( int argc, char** argv );
    CommandLine( int argc, char** argv, const std::string& command_name );
    virtual ~CommandLine();

public:

    int argc() const { return m_argc; }
    char** argv() const { return m_argv; }
    const std::string& commandName() const { return m_command_name; }
    void setArguments( int argc, char** argv )
    {
        m_argc = argc;
        m_argv = argv;
        m_command_name = std::string( m_argv[0] );
    }

    bool parse();
    bool read();
    void clear();

    template <class T>
    T value( size_t index = 0 ) const;
    bool hasValues() const;
    size_t numberOfValues() const;

    template <class T>
    T optionValue( const std::string& option_name, size_t index = 0 ) const;
    bool hasOption( const std::string& option_name ) const;
    bool hasOptionValue( const std::string& option_name ) const;
    size_t numberOfOptions() const;

    void addHelpOption( const std::string& help_option = "h" );
    void addOption(
        const std::string& name,
        const std::string& description,
        size_t nvalues = 0,
        bool is_required = false );
    void addValue( const std::string& description, bool is_required = true );
    void showHelpMessage( HelpMessageMode mode = UsageOnly ) const;

#if KVS_ENABLE_DEPRECATED
protected:
    void add_help_option( const std::string& help_option = "h" )
    {
        this->addHelpOption( help_option );
    }

    void add_option(
        const std::string& name,
        const std::string& description,
        size_t             nvalues     = 0,
        bool               is_required = false )
    {
        this->addOption( name, description, nvalues, is_required );
    }

    void add_value( const std::string& description, bool is_required = true )
    {
        this->addValue( description, is_required );
    }
#endif

private:

    bool is_option( const std::string& argument ) const;
    std::string get_option_name( const std::string& argument ) const;
    bool is_help_option( const std::string& value ) const;
    Options::iterator find_option( const std::string& argument );
    bool read_option_values( 
        Arguments::iterator& argument,
        Options::iterator&   option );
    void print_help_message( HelpMessageMode mode ) const;
};

/*==========================================================================*/
/**
 *  Option class.
 */
/*==========================================================================*/
class CommandLine::Option
{
private:

    std::string m_name; ///< option name
    std::string m_description; ///< option description
    size_t m_nvalues; ///< number of required values
    bool m_is_required; ///< true, if the option is required
    bool m_is_given; ///< true, if the option is given
    std::vector<std::string> m_values; ///< option values

public:

    Option();

    explicit Option(
        const std::string& name,
        const std::string& description = "",
        size_t nvalues = 0,
        bool is_required = false );

    void setValue( const std::string& value ) { m_values.push_back( value ); this->given(); }
    void given() { m_is_given = true; }
    const std::string& name() const { return m_name; }
    const std::string& description() const { return m_description; }
    size_t numberOfValues() const { return m_nvalues; }
    bool isRequired() const { return m_is_required; }
    bool isGiven() const { return m_is_given; }
    const std::vector<std::string>& values() const { return m_values; }

    template <typename T>
    T value( size_t index ) const;
    friend bool operator <( const Option& lhs, const Option& rhs );
    friend bool operator ==( const Option& lhs, const Option& rhs );
};

/*==========================================================================*/
/**
 *  Value class.
 */
/*==========================================================================*/
class CommandLine::Value
{
private:

    std::string m_description; ///< value description
    bool m_is_required; ///< true, if the value is required
    bool m_is_given; ///< true, if the value is given
    Argument m_value; ///< value (not allocated)

public:

    Value();

    explicit Value(
        const std::string& description,
        bool is_required = true );

    void setValue( const std::string& value ) { m_value = value; m_is_given = true; }
    const std::string& description() const { return m_description; }
    bool isRequired() const { return m_is_required; }
    bool isGiven() const { return m_is_given; }

    template <typename T>
    T value() const;
    friend bool operator <( const Value& lhs, const Value& rhs );
    friend bool operator ==( const Value& lhs, const Value& rhs );
};

/*==========================================================================*/
/**
 *  Get input value.
 *  @param index [in] index of input value list
 *  @return input value
 */
/*==========================================================================*/
template <class T>
inline T CommandLine::value( size_t index ) const
{
    return m_values[ index ].value<T>();
}

/*==========================================================================*/
/**
 *  Get option value for given option.
 *  @param option_name [in] option name
 *  @param index [in] index of option value list
 *  @return option value
 */
/*==========================================================================*/
template <class T>
inline T CommandLine::optionValue( const std::string& option_name, size_t index ) const
{
    Option key( option_name );
    Options::const_iterator option =
        std::find( m_options.begin(), m_options.end(), key );

    if ( option == m_options.end() || !option->isGiven() )
    {
        kvsMessageError( "Cannot find '-%s' option.", option->name().c_str() );
        return T( 0 );
    }
    return option->value<T>( index );
}

/*==========================================================================*/
/**
 *  Get option value.
 *  @param index [in] index of option value list
 *  @return option value
 */
/*==========================================================================*/
template <typename T>
inline T CommandLine::Option::value( size_t index ) const
{
    if ( index >= m_nvalues )
    {
        kvsMessageError( "Option '-%s' has only %d values.",
                         m_name.c_str(), m_nvalues );
        return T( 0 );
    }
    return kvs::String::To<T>( m_values[ index ] );
}

/*==========================================================================*/
/**
 *  Get value.
 *  @return value
 */
/*==========================================================================*/
template <typename T>
inline T CommandLine::Value::value() const
{
    return kvs::String::To<T>( m_value );
}

} // end of namespace kvs

#endif // KVS__COMMAND_LINE_H_INCLUDE
