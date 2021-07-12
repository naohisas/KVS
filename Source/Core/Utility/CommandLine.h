/****************************************************************************/
/**
 *  @file   CommandLine.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    class Option;
    class Value;
    typedef std::string Argument;
    typedef std::vector<std::string> Arguments;
    typedef std::vector<Option> Options;
    typedef std::vector<Value> Values;

private:
    int m_argc = 0; ///< argument count
    char** m_argv = 0; ///< argument values
    std::string m_command_name = ""; ///< command name
    size_t m_max_length = 0; ///< max length of the option name
    bool m_no_help = 0; ///< no help option
    std::string m_help_option = "h"; ///< help option character (ex: 'h')
    Arguments m_arguments{}; ///< argument values
    Options m_options{}; ///< options
    Values m_values{}; ///< values

public:
    CommandLine() = default;
    CommandLine( int argc, char** argv ): m_argc( argc ), m_argv( argv ), m_command_name( argv[0] ) {}
    CommandLine( int argc, char** argv, const std::string& name ): m_argc( argc ), m_argv( argv ), m_command_name( name ) {}
    virtual ~CommandLine() = default;

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
    std::string m_name = ""; ///< option name
    std::string m_description = ""; ///< option description
    size_t m_nvalues = 0; ///< number of required values
    bool m_is_required = false; ///< true, if the option is required
    bool m_is_given = false; ///< true, if the option is given
    std::vector<std::string> m_values{}; ///< option values

public:
    Option() = default;
    explicit Option(
        const std::string& name,
        const std::string& description = "",
        size_t nvalues = 0,
        bool is_required = false ):
        m_name( name ),
        m_description( description ),
        m_nvalues( nvalues ),
        m_is_required( is_required ) {}

    void setValue( const std::string& value ) { m_values.push_back( value ); this->given(); }
    void given() { m_is_given = true; }
    const std::string& name() const { return m_name; }
    const std::string& description() const { return m_description; }
    size_t numberOfValues() const { return m_nvalues; }
    bool isRequired() const { return m_is_required; }
    bool isGiven() const { return m_is_given; }
    const std::vector<std::string>& values() const { return m_values; }

    template <typename T> T value( size_t index ) const;
    friend bool operator <( const Option& lhs, const Option& rhs ) { return lhs.m_name < rhs.m_name; }
    friend bool operator ==( const Option& lhs, const Option& rhs ) { return lhs.m_name == rhs.m_name; }
};

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
 *  Value class.
 */
/*==========================================================================*/
class CommandLine::Value
{
private:
    std::string m_description = ""; ///< value description
    bool m_is_required = true; ///< true, if the value is required
    bool m_is_given = false; ///< true, if the value is given
    Argument m_value{}; ///< value (not allocated)

public:
    Value() = default;
    explicit Value(
        const std::string& description,
        bool is_required = true ):
        m_description( description ),
        m_is_required( is_required ) {}

    void setValue( const std::string& value ) { m_value = value; m_is_given = true; }
    const std::string& description() const { return m_description; }
    bool isRequired() const { return m_is_required; }
    bool isGiven() const { return m_is_given; }

    template <typename T> T value() const { return kvs::String::To<T>( m_value ); }
    friend bool operator <( const Value& lhs, const Value& rhs ) { return lhs.m_description < rhs.m_description; }
    friend bool operator ==( const Value& lhs, const Value& rhs ) { return lhs.m_description == rhs.m_description; }
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

} // end of namespace kvs
