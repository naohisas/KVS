/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for label widget
 */
/*****************************************************************************/
//#define KVS_APP_USE_GLUT
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/Label>
#include <kvs/Font>
#include <kvs/FontMetrics>
#include <kvs/UIColor>


/*===========================================================================*/
/**
 *  @brief  User-defined paint event class.
 */
/*===========================================================================*/
class PaintEvent : public kvs::PaintEventListener
{
    void update()
    {
        glBegin( GL_TRIANGLES );
        glColor3ub( 255,   0,   0 ); glVertex3d(  0.0,  3.0, 0.0 );
        glColor3ub(   0, 255,   0 ); glVertex3d(  3.0, -3.0, 0.0 );
        glColor3ub(   0,   0, 255 ); glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    }
};

/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );

    PaintEvent paint_event;

    kvs::Screen screen( &app );
    screen.addEvent( &paint_event );
    screen.create();

    std::string text1( "Label1" );
    kvs::Label label1( &screen );
    label1.setX( 10 );
    label1.setY( 10 );
    label1.setText( text1 );

    kvs::Font font1 = label1.font();
    kvs::FontMetrics font_metrics1( font1, screen.paintDevice() );
    std::cout << text1 << std::endl;
    std::cout << "    Font family: " << font1.familyName() << std::endl;
    std::cout << "    Font style: " << font1.styleName() << std::endl;
    std::cout << "    Font size: " << font1.size() << std::endl;
    std::cout << "    Text height: " << font_metrics1.height() << std::endl;
    std::cout << "    Text width: " << font_metrics1.width( text1 ) << std::endl;

    std::string text2( "Label2" );
    kvs::Label label2( &screen );
    label2.setX( label1.x() );
    label2.setY( label1.y() + font_metrics1.height() );
    label2.setText( text2 );

    kvs::Font font2 = label2.font();
    font2.setSize( font1.size() * 2 );
    font2.setStyleToBold();
    label2.setFont( font2 );

    kvs::FontMetrics font_metrics2( font2, screen.paintDevice() );
    std::cout << text2 << std::endl;
    std::cout << "    Font family: " << font2.familyName() << std::endl;
    std::cout << "    Font style: " << font2.styleName() << std::endl;
    std::cout << "    Font size: " << font2.size() << std::endl;
    std::cout << "    Text height: " << font_metrics2.height() << std::endl;
    std::cout << "    Text width: " << font_metrics2.width( text1 ) << std::endl;

    std::string text3( "Label3" );
    kvs::Label label3( &screen );
    label3.setX( label2.x() );
    label3.setY( label2.y() + font_metrics2.height() );
    label3.setText( text3 );

    kvs::Font font3 = label3.font();
    font3.setSize( 50 );
    font3.setColor( kvs::UIColor::Blue() );
    font3.setFamilyToSerif();
    font3.setStyleToItalic();
    label3.setFont( font3 );

    kvs::FontMetrics font_metrics3( font3, screen.paintDevice() );
    std::cout << text3 << std::endl;
    std::cout << "    Font family: " << font3.familyName() << std::endl;
    std::cout << "    Font style: " << font3.styleName() << std::endl;
    std::cout << "    Font size: " << font3.size() << std::endl;
    std::cout << "    Text height: " << font_metrics3.height() << std::endl;
    std::cout << "    Text width: " << font_metrics3.width( text1 ) << std::endl;

    std::string text4( "Label4" );
    std::string text41( "Label4-1" );
    std::string text42( "Label4-2" );
    std::string text43( "Label4-3" );
    kvs::Label label4( &screen );
    label4.setX( label3.x() );
    label4.setY( label3.y() + font_metrics3.height() );
    label4.setBackgroundColor( kvs::UIColor::Fill() );
    label4.setText( text4 );
    label4.addText( text41 );
    label4.addText( text42 );
    label4.addText( text43 );

    kvs::Font font4 = label4.font();
    font4.setSize( 50 );
//    font4.setColor( kvs::UIColor::Red() );
    font4.setStyleToBold();
//    font4.setVerticalAlignToBottom();
    label4.setFont( font4 );

    kvs::FontMetrics font_metrics4( font4, screen.paintDevice() );
    std::cout << text4 << std::endl;
    std::cout << text41 << std::endl;
    std::cout << text42 << std::endl;
    std::cout << text43 << std::endl;
    std::cout << "    Font family: " << font4.familyName() << std::endl;
    std::cout << "    Font style: " << font4.styleName() << std::endl;
    std::cout << "    Font size: " << font4.size() << std::endl;
    std::cout << "    Text height: " << font_metrics4.height() << std::endl;
    std::cout << "    Text width: " << font_metrics4.width( text1 ) << std::endl;

    label1.show();
    label2.show();
    label3.show();
    label4.show();

    return app.run();
}
