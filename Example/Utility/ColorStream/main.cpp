/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::ColorStream class.
 */
/*****************************************************************************/
#include <iostream>
#include <string>
#include <kvs/ColorStream>


int main()
{
    // Change message color.
    std::cout << kvs::ColorStream::Black( "Message1 Black" ) << std::endl;
    std::cout << kvs::ColorStream::Red( "Message1 Red" ) << std::endl;
    std::cout << kvs::ColorStream::Green( "Message1 Green" ) << std::endl;
    std::cout << kvs::ColorStream::Yellow( "Message1 Yellow" ) << std::endl;
    std::cout << kvs::ColorStream::Blue( "Message1 Blue" ) << std::endl;
    std::cout << kvs::ColorStream::Magenta( "Message1 Magenta" ) << std::endl;
    std::cout << kvs::ColorStream::Cyan( "Message1 Cyan" ) << std::endl;
    std::cout << kvs::ColorStream::LightGray( "Message1 LightGray" ) << std::endl;
    std::cout << kvs::ColorStream::DarkGray( "Message1 DarkGray" ) << std::endl;
    std::cout << kvs::ColorStream::LightRed( "Message1 LightRed" ) << std::endl;
    std::cout << kvs::ColorStream::LightGreen( "Message1 LightGreen" ) << std::endl;
    std::cout << kvs::ColorStream::LightYellow( "Message1 LightYellow" ) << std::endl;
    std::cout << kvs::ColorStream::LightBlue( "Message1 LightBlue" ) << std::endl;
    std::cout << kvs::ColorStream::LightMagenta( "Message1 LightMagenta" ) << std::endl;
    std::cout << kvs::ColorStream::LightCyan( "Message1 LightCyan" ) << std::endl;
    std::cout << kvs::ColorStream::White( "Message1 White" ) << std::endl;
    std::cout << std::endl;

    // Change message format.
    std::cout << kvs::ColorStream::Bold( "Message2" ) << std::endl;
    std::cout << kvs::ColorStream::Dim( "Message3" ) << std::endl;
    std::cout << kvs::ColorStream::Underline( "Message4" ) << std::endl;
    std::cout << kvs::ColorStream::Blink( "Message5" ) << std::endl;
    std::cout << kvs::ColorStream::Reverse( "Message6" ) << std::endl;
    std::cout << std::endl;

    // Conbination.
    std::cout << kvs::ColorStream::Underline( kvs::ColorStream::Bold( "Message7" ) ) << std::endl;

    std::cout << kvs::ColorStream::Bold
              << kvs::ColorStream::Red
              << "Message8"
              << kvs::ColorStream::Reset
              << std::endl;

    // Change foreground/background colors.
    kvs::ColorStream cs;
    cs.setForegroundColor( kvs::ColorStream::ColorYellow );
    cs.setBackgroundColor( kvs::ColorStream::ColorBlue );
    std::cout << cs << "Message9" << kvs::ColorStream::Reset << std::endl;

    return 0;
}
