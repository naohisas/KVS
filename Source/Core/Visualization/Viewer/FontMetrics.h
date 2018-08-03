#pragma once
#include <kvs/PaintDevice>
#include <kvs/Font>
#include <string>


namespace kvs
{

class FontMetrics
{
private:
    const kvs::Font& m_font;
    kvs::PaintDevice* m_device;

public:
    FontMetrics( const kvs::Font& font, kvs::PaintDevice* device );

    int height() const;
    int width( const std::string& text ) const;
};

} // end of namespace kvs
