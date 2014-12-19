/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::CommandLine class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1205 2012-06-15 07:16:47Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <kvs/CommandLine>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    /* Options:
     *   -x <float>  : floating value 1 (1 value, required)
     *   -y <float>  : floating value 2 (1 value, required)
     *   -o <string> : operator 'sum', 'sub', 'mul', 'div' (1 value, not required)
     *   filename (not reuqired)
     *
     * Examples:
     *   ./CommandLine -x 1.5 -y 0.2 -o sum
     *   ./CommandLine -x 2.9 -y 7.1 -o div result.txt
     *   ./CommandLine -h
     */

    // Setup command line options.
    kvs::CommandLine commandline( argc, argv );
    commandline.addHelpOption();
    commandline.addOption( "x","floating value 1 (required).", 1, true );
    commandline.addOption( "y","floating value 2 (required).", 1, true );
    commandline.addOption( "o","operator 'sum', 'sub', 'mul', 'div' (default: 'sum').", 1, false );
    commandline.addValue( "filename.", false );

    // Parse given command line options.
    if ( !commandline.parse() ) return 1;

    // Get a floating value which is given by the option 'x'.
    float x = commandline.optionValue<float>("x");

    // Get a floating value which is given by the option 'y'.
    float y = commandline.optionValue<float>("y");

    // Get an operator string which is given by the option 'o'.
    std::string o = "sum"; // default value
    if ( commandline.hasOption("o") )
    {
        o = commandline.optionValue<std::string>("o");
    }

    // Calculation.
    float result = 0.0f;
    if ( o == "sum" )
    {
        o = "+";
        result = x + y;
    }
    else if ( o == "sub" )
    {
        o = "-";
        result = x - y;
    }
    else if ( o == "mul" )
    {
        o = "*";
        result = x * y;
    }
    else if ( o == "div" )
    {
        o = "/";
        result = x / y;
    }
    else
    {
        std::cerr << "Unknown operator '" << o << "'." << std::endl;
        return 1;
    }

    // Output the results.
    if ( commandline.hasValues() )
    {
        std::string filename = commandline.value<std::string>();
        std::ofstream ofs( filename.c_str(), std::ios::out );
        ofs << x << " " << o << " " << y << " = " << result << std::endl;
        ofs.close();
    }
    else
    {
        std::cout << x << " " << o << " " << y << " = " << result << std::endl;
    }

    return 0;
}
