/*****************************************************************************/
/**
 *  @file   main.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once


namespace kvsconv
{

/*===========================================================================*/
/**
 *  Main class.
 */
/*===========================================================================*/
class Main
{
private:
    int m_argc; ///< argument count
    char** m_argv; ///< argument values

public:
    Main( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}
    bool exec();
};

} // end of namespace kvsconv
