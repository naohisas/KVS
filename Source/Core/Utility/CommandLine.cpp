/****************************************************************************/
/**
 *  @file CommandLine.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CommandLine.cpp 1616 2013-07-12 03:22:24Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <kvs/CommandLine>
#include <kvs/Message>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
CommandLine::Option::Option():
    m_name(),
    m_description(),
    m_nvalues( 0 ),
    m_is_required( false ),
    m_is_given( false ),
    m_values()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param name [in] option name
 *  @param description [in] description for option
 *  @param nvalues [in] number of option values
 *  @param is_required [in] whether option is required ?
 */
/*==========================================================================*/
CommandLine::Option::Option(
    const std::string& name,
    const std::string& description,
    size_t nvalues,
    bool is_required ):
    m_name( name ),
    m_description( description ),
    m_nvalues( nvalues ),
    m_is_required( is_required ),
    m_is_given( false ),
    m_values()
{
}

/*==========================================================================*/
/**
 *  Comparison operator '<'.
 *  @param lhs [in] option (left hand side)
 *  @param rhs [in] option (right hand side)
 */
/*==========================================================================*/
bool operator <( const CommandLine::Option& lhs, const CommandLine::Option& rhs )
{
    return lhs.m_name < rhs.m_name;
}

/*==========================================================================*/
/**
 *  Comparison operator '=='.
 *  @param lhs [in] option (left hand side)
 *  @param rhs [in] option (right hand side)
 */
/*==========================================================================*/
bool operator ==( const CommandLine::Option& lhs, const CommandLine::Option& rhs )
{
    return lhs.m_name == rhs.m_name;
}

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
CommandLine::Value::Value():
    m_description(),
    m_is_required( true ),
    m_is_given( false ),
    m_value()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param description [in] description of value
 *  @param is_required [in] whether option is required ?
 */
/*==========================================================================*/
CommandLine::Value::Value( const std::string& description, bool is_required ):
    m_description( description ),
    m_is_required( is_required ),
    m_is_given( false ),
    m_value()
{
}

/*==========================================================================*/
/**
 *  Comparison operator '<'.
 *  @param lhs [in] value (left hand side)
 *  @param rhs [in] value (right hand side)
 */
/*==========================================================================*/
bool operator <( const CommandLine::Value& lhs, const CommandLine::Value& rhs )
{
    return lhs.m_description < rhs.m_description;
}

/*==========================================================================*/
/**
 *  Comparison operator '=='.
 *  @param lhs [in] value (left hand side)
 *  @param rhs [in] value (right hand side)
 */
/*==========================================================================*/
bool operator ==( const CommandLine::Value& lhs, const CommandLine::Value& rhs )
{
    return lhs.m_description == rhs.m_description;
}

/*===========================================================================*/
/**
 *  @brief  Constructor.
 */
/*===========================================================================*/
CommandLine::CommandLine():
    m_argc( 0 ),
    m_argv( 0 ),
    m_command_name( "" ),
    m_max_length( 0 ),
    m_no_help( true ),
    m_help_option( "h" ),
    m_arguments(),
    m_options(),
    m_values()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param argc [in] argument count
 *  @param argv [in] argument value
 */
/*==========================================================================*/
CommandLine::CommandLine( int argc, char** argv ):
    m_argc( argc ),
    m_argv( argv ),
    m_command_name( argv[0] ),
    m_max_length( 0 ),
    m_no_help( true ),
    m_help_option( "h" ),
    m_arguments(),
    m_options(),
    m_values()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param argc [in] argument count
 *  @param argv [in] argument value
 *  @param command_name [in] command name
 */
/*==========================================================================*/
CommandLine::CommandLine( int argc, char** argv, const std::string& command_name ):
    m_argc( argc ),
    m_argv( argv ),
    m_command_name( command_name ),
    m_max_length( 0 ),
    m_no_help( true ),
    m_help_option( "h" ),
    m_arguments(),
    m_options(),
    m_values()
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
CommandLine::~CommandLine()
{
}

/*==========================================================================*/
/**
 *  Parse command line arguments.
 *  @return true, if parse process is done successfully
 */
/*==========================================================================*/
bool CommandLine::parse()
{
    if ( m_argc == 0 )
    {
        kvsMessageError("Nothing arguments.");
        return false;
    }

    bool allow_no_value = ( m_values.size() == 0 );
    if ( !allow_no_value )
    {
        // Check the 'is_required' property of the input values, when the some
        // input values are added in m_values.
        Values::const_iterator v = m_values.begin();
        while ( v != m_values.end() )
        {
            if ( !v->isRequired() ) { allow_no_value = true; }
            ++v;
        }
    }

    // Output help message.
    if ( m_argc == 1 && !allow_no_value )
    {
        this->print_help_message( UsageOnly );
        this->clear();
        return false;
    }

    // Oputput help message in detail when '-h' option is given.
    for ( int i = 1; i < m_argc; ++i )
    {
        std::string arg( m_argv[i] );

        if ( !m_no_help && is_help_option( arg ) )
        {
            this->print_help_message( UsageAndOption );
            this->clear();
            return false;
        }
        m_arguments.push_back( arg );
    }

    // Main loops in this method.
    Arguments::iterator argument = m_arguments.begin();
    Options::iterator option = m_options.begin();
    Values::iterator value = m_values.begin();
    while ( argument != m_arguments.end() )
    {
        if ( this->is_option( *argument ) )
        {
            // Search the given option from the option set.
            option = this->find_option( *argument );
            if ( option == m_options.end() )
            {
                kvsMessageError( "Unknown option '%s'", argument->c_str() );
                this->clear();
                return false;
            }

            // Read the option values.
            if ( !this->read_option_values( argument, option ) )
            {
                kvsMessageError( "%d values is required for the option '-%s'",
                                 option->numberOfValues(),
                                 option->name().c_str() );
                this->clear();
                return false;
            }
        }
        else
        {
            if ( value != m_values.end() )
            {
                value->setValue( *argument );

                ++value;
            }

            ++argument;
        }
    }

    // Check the required argument.
    for ( size_t i = 0; i < m_options.size(); ++i )
    {
        if ( m_options[i].isRequired() && !m_options[i].isGiven() )
        {
            kvsMessageError( "Option '-%s' is required.", m_options[i].name().c_str() );
            this->clear();
            return false;
        }
    }

    for ( size_t i = 0; i < m_values.size(); ++i )
    {
        if ( m_values[i].isRequired() && !m_values[i].isGiven() )
        {
            kvsMessageError( "Input value is required." );
            this->clear();
            return false;
        }
    }

    return true;
}

/*==========================================================================*/
/**
 *  Read command line arguments.
 *  @return true, if the reading process is done successfully
 */
/*==========================================================================*/
bool CommandLine::read()
{
    if ( m_argc == 0 )
    {
        kvsMessageError("Nothing arguments.");
        return false;
    }

    bool allow_no_value = ( m_values.size() == 0 );
    if ( !allow_no_value )
    {
        // Check the 'is_required' property of the input values, when the some
        // input values are added in m_values.
        Values::const_iterator v = m_values.begin();
        while ( v != m_values.end() )
        {
            if ( !v->isRequired() ) { allow_no_value = true; }
            ++v;
        }
    }

    // Output help message.
    if ( m_argc == 1 && !allow_no_value )
    {
        this->print_help_message( UsageOnly );
        this->clear();
        return false;
    }

    // Oputput help message in detail when '-h' option is given.
    for ( int i = 1; i < m_argc; ++i )
    {
        const char* const p_value = m_argv[i];

        if ( !m_no_help && is_help_option( p_value ) )
        {
            this->print_help_message( UsageAndOption );
            this->clear();
            return false;
        }
        else
        {
            std::string arg( p_value );
            m_arguments.push_back( arg );
        }
    }

    // Main loops in this method.
    Arguments::iterator argument = m_arguments.begin();
    Options::iterator   option   = m_options.begin();
    Values::iterator    value    = m_values.begin();
    while ( argument != m_arguments.end() )
    {
        if ( this->is_option( *argument ) )
        {
            // Serch the given option from the option set.
            option = this->find_option( *argument );
            if ( option == m_options.end() ) { return true; }

            // Read the option values.
            if ( !this->read_option_values( argument, option ) ) { return true; }
        }
        else
        {
            if ( value != m_values.end() )
            {
                if ( value->isRequired() )
                {
                    if ( argument->empty() ) { return true; }

                    value->setValue( *argument );
                }

                ++value;
            }

            ++argument;
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Clear argument lists.
 */
/*===========================================================================*/
void CommandLine::clear()
{
    m_arguments.clear();
    m_options.clear();
    m_values.clear();
}

/*==========================================================================*/
/**
 *  Test whether command line has input value.
 *  @return true, if command line has input value
 */
/*==========================================================================*/
bool CommandLine::hasValues() const
{
    Values::const_iterator value      = m_values.begin();
    Values::const_iterator last_value = m_values.end();

    if ( value == last_value ) { return false; }

    while ( value != last_value )
    {
        if ( !value->isGiven() ) { return false; }
        ++value;
    }

    return true;
}

/*==========================================================================*/
/**
 *  Get number of input values.
 *  @return number of input values
 */
/*==========================================================================*/
size_t CommandLine::numberOfValues() const
{
    return m_values.size();
}

/*==========================================================================*/
/**
 *  Test whether command line has given option.
 *  @param option_name [in] option name
 *  @return true, if command line has given option
 */
/*==========================================================================*/
bool CommandLine::hasOption( const std::string& option_name ) const
{
    Option key( option_name );
    Options::const_iterator p = std::find( m_options.begin(), m_options.end(), key );

    return p != m_options.end() && p->isGiven();
}

/*==========================================================================*/
/**
 *  Test whether command line has option value for given option.
 *  @param option_name [in] option name
 *  @return true, if command line has option value for given option
 */
/*==========================================================================*/
bool CommandLine::hasOptionValue( const std::string& option_name ) const
{
    Option key( option_name );
    Options::const_iterator p = std::find( m_options.begin(), m_options.end(), key );

    return p != m_options.end() && p->numberOfValues() > 0;
}

/*==========================================================================*/
/**
 *  Get number of options.
 *  @return number of options
 */
/*==========================================================================*/
size_t CommandLine::numberOfOptions() const
{
    return m_options.size();
}

/*==========================================================================*/
/**
 *  Add help option.
 *  @param help_option [in] character for help option
 */
/*==========================================================================*/
void CommandLine::addHelpOption( const std::string& help_option )
{
    m_help_option = help_option;
    m_no_help = false;

    this->addOption( help_option, "Output help message.", 0, false );
}

/*==========================================================================*/
/**
 *  Add option.
 *  @param name [in] option name
 *  @param description [in] option description
 *  @param nvalues [in] number of option values
 *  @param is_required [in] whether option is required ?
 */
/*==========================================================================*/
void CommandLine::addOption(
    const std::string& name,
    const std::string& description,
    size_t nvalues,
    bool is_required )
{
    m_max_length = name.length() + 1 > m_max_length ? name.length() + 1 : m_max_length;

    Option option( name, description, nvalues, is_required );
    m_options.push_back( option );
}

/*==========================================================================*/
/**
 *  Add input value.
 *  @param description [in] description of input value
 *  @param is_required [in] whether input value is required ?
 */
/*==========================================================================*/
void CommandLine::addValue( const std::string& description, bool is_required )
{
    CommandLine::Value value( description, is_required );
    m_values.push_back( value );
}

/*==========================================================================*/
/**
 *  Show help message.
 *  @param mode [in] help message mode
 */
/*==========================================================================*/
void CommandLine::showHelpMessage( HelpMessageMode mode ) const
{
    this->print_help_message( mode );
}

/*==========================================================================*/
/**
 *  Test whether given argument is option.
 *  @param argument [in] argument
 *  @return true, if given argument is option
 */
/*==========================================================================*/
bool CommandLine::is_option( const std::string& argument ) const
{
    return argument[0] == '-';
}

/*==========================================================================*/
/**
 *  Get option name.
 *  @param argument [in] argument
 *  @return option name
 */
/*==========================================================================*/
std::string CommandLine::get_option_name( const std::string& argument ) const
{
    return argument.substr( 1 );
}

/*==========================================================================*/
/**
 *  Test whether given value is help option.
 *  @param value [in] value
 *  @return true, if given value is help option
 */
/*==========================================================================*/
bool CommandLine::is_help_option( const std::string& value ) const
{
    return value == "-" + m_help_option;
}

/*==========================================================================*/
/**
 *  Find option.
 *  @param argument [in] argument
 *  @return iterator of option for given argument
 */
/*==========================================================================*/
CommandLine::Options::iterator CommandLine::find_option( 
    const std::string& argument )
{
    std::string option_name = this->get_option_name( argument );
    CommandLine::Option key( option_name );

    return std::find( m_options.begin(), m_options.end(), key );
}

/*==========================================================================*/
/**
 *  Read option values.
 *  @param argument [in] iterator of argument
 *  @param option [in] iterator of option
 *  @return true, if read process is done successfully
 */
/*==========================================================================*/
bool CommandLine::read_option_values(
    Arguments::iterator& argument,
    Options::iterator& option )
{
    ++argument; // skip option name.

    // No option value.
    if ( option->numberOfValues() == 0 )
    {
        option->given();
        return true;
    }

    for ( size_t i = 0; i < option->numberOfValues(); ++i )
    {
        if ( argument == m_arguments.end() ) { return false; }
        if ( this->is_option( *argument ) ) { return false; }

        option->setValue( *argument );
        ++argument;
    }

    return true;
}

/*==========================================================================*/
/**
 *  Print help message.
 *  @param mode [in] help message mode
 */
/*==========================================================================*/
void CommandLine::print_help_message( HelpMessageMode mode ) const
{
    if ( mode >= UsageOnly )
    {
        std::cerr << "Usage: " << m_command_name << " ";
        std::cerr << "[options] ";
        for ( size_t i = 0; i < m_values.size(); ++i )
        {
            std::cerr << "<" << m_values[i].description() << "> ";
        }
        std::cerr << std::endl;
    }

    if ( mode >= UsageAndOption )
    {
        std::cerr << std::endl << "Options:" << std::endl;
        Options::const_iterator option = m_options.begin();
        while ( option != m_options.end() )
        {
            std::string option_name = "-" + option->name();
            std::cerr << std::left << std::setw( m_max_length + 1 ) << option_name;
            std::cerr << ": " << option->description();
            if ( option->isRequired() ) { std::cerr << " [required]"; }
            std::cerr << std::endl;
            ++option;
        }
    }
}

} // end of namespace kvs
