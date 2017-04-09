#include "PaintDevice.h"


namespace kvs
{

PaintDevice::PaintDevice():
    m_render_engine( NULL ),
    m_text_engine( NULL )
{
}

PaintDevice::~PaintDevice()
{
    if ( m_render_engine ) delete m_render_engine;
    if ( m_text_engine ) delete m_text_engine;
}

void PaintDevice::create()
{
    if ( !m_render_engine )
    {
        const int flag = kvs::NanoVG::Antialias;
        m_render_engine = new kvs::NanoVG( flag );
    }

    if ( !m_text_engine )
    {
        const int width = 512;
        const int height = 512;
        const int flag = kvs::FontStash::ZeroTopLeft;
        m_text_engine = new kvs::FontStash( width, height, flag );
        m_text_engine->addFont( "SansRegular", "NotoSans-Regular.ttf" );
        m_text_engine->addFont( "SansItalic", "NotoSans-Italic.ttf" );
        m_text_engine->addFont( "SansBold", "NotoSans-Bold.ttf" );
        m_text_engine->addFont( "SansBoldItalic", "NotoSans-BoldItalic.ttf" );
        m_text_engine->addFont( "SerifRegular", "NotoSerif-Regular.ttf" );
        m_text_engine->addFont( "SerifItalic", "NotoSerif-Italic.ttf" );
        m_text_engine->addFont( "SerifBold", "NotoSerif-Bold.ttf" );
        m_text_engine->addFont( "SerifBoldItalic", "NotoSerif-BoldItalic.ttf" );
        m_text_engine->addFont( "Icon", "entypo.ttf" );
    }
}

} // end of namespace kvs
