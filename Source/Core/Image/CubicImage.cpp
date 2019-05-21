#include "CubicImage.h"
#include <kvs/Assert>
#include <kvs/RGBColor>


namespace
{

const std::string DirNames[ kvs::CubicImage::NumberOfDirections ] = {
    std::string( "right" ),
    std::string( "left" ),
    std::string( "top" ),
    std::string( "bottom" ),
    std::string( "back" ),
    std::string( "front" )
};

const kvs::Vec3 DirVecs[ kvs::CubicImage::NumberOfDirections ] = {
    kvs::Vec3( +1,  0,  0 ), // right
    kvs::Vec3( -1,  0,  0 ), // left
    kvs::Vec3(  0, +1,  0 ), // top
    kvs::Vec3(  0, -1,  0 ), // bottom
    kvs::Vec3(  0,  0, +1 ), // back
    kvs::Vec3(  0,  0, -1 )  // front
};

const kvs::Vec3 UpVecs[ kvs::CubicImage::NumberOfDirections ] = {
    kvs::Vec3( 0, 1,  0 ), // right
    kvs::Vec3( 0, 1,  0 ), // left
    kvs::Vec3( 0, 0,  1 ), // top
    kvs::Vec3( 0, 0, -1 ), // bottom
    kvs::Vec3( 0, 1,  0 ), // back
    kvs::Vec3( 0, 1,  0 )  // front
};

inline void Draw(
    const kvs::ColorImage& source,
    const size_t offset_i,
    const size_t offset_j,
    kvs::ColorImage* result )
{
    const size_t w = source.width();
    const size_t h = source.height();
    for ( size_t j = 0; j < h; j++ )
    {
        for ( size_t i = 0; i < w; i++ )
        {
            const kvs::RGBColor pixel = source.pixel( i, j );
            result->setPixel( i + offset_i, j + offset_j, pixel );
        }
    }
}

}


namespace kvs
{

std::string CubicImage::DirectionName( const CubicImage::Direction dir )
{
    return ::DirNames[ dir ];
}

kvs::Vec3 CubicImage::DirectionVector( const CubicImage::Direction dir )
{
    return ::DirVecs[ dir ];
}

kvs::Vec3 CubicImage::UpVector( const CubicImage::Direction dir )
{
    return ::UpVecs[ dir ];
}

void CubicImage::stitch()
{
    const size_t w = this->frontImage().width();
    const size_t h = this->frontImage().height();
    KVS_ASSERT( w == this->backImage().width() && h == this->backImage().height() );
    KVS_ASSERT( w == this->topImage().width() && h == this->topImage().height() );
    KVS_ASSERT( w == this->bottomImage().width() && h == this->bottomImage().height() );
    KVS_ASSERT( w == this->rightImage().width() && h == this->rightImage().height() );
    KVS_ASSERT( w == this->leftImage().width() && h == this->leftImage().height() );

    switch ( m_layout )
    {
    case Cross:
    {
        // Cross layout
        //
        //       [+Y]
        //  [-X] [-Z] [+X] [+Z]
        //       [-Y]
        //
        this->create( w * 4, h * 3, kvs::ImageBase::Color );
        ::Draw( this->topImage(), w, 0, this ); // +Y
        ::Draw( this->leftImage(), 0, h, this ); // -X
        ::Draw( this->frontImage(), w, h, this ); // -Z
        ::Draw( this->rightImage(), w * 2, h, this ); // +X
        ::Draw( this->backImage(), w * 3, h, this ); // +Z
        ::Draw( this->bottomImage(), w, h * 2, this ); // -Y
    }
    case Rectangle:
    {
        // Rectangle layout
        //
        // [+X] [+Y] [+Z]
        // [-X] [-Y] [-Z]
        //
        this->create( w * 3, h * 2, kvs::ImageBase::Color );
        ::Draw( this->rightImage(), 0, 0, this ); // +X
        ::Draw( this->leftImage(), 0, h, this ); // -X
        ::Draw( this->topImage(), w, 0, this ); // +Y
        ::Draw( this->bottomImage(), w, h, this ); // -Y
        ::Draw( this->backImage(), w * 2, 0, this ); // +Z
        ::Draw( this->frontImage(), w * 2, h, this ); // -Z
    }
    case Vertical:
    {
        // Vertical layout
        //
        // [+X]
        // [-X]
        // [+Y]
        // [-Y]
        // [+Z]
        // [-Z]
        //
        this->create( w, h * 6, kvs::ImageBase::Color );
        ::Draw( this->rightImage(), 0, 0, this ); // +X
        ::Draw( this->leftImage(), 0, h, this ); // -X
        ::Draw( this->topImage(), 0, h * 2, this ); // +Y
        ::Draw( this->bottomImage(), 0, h * 3, this ); // -Y
        ::Draw( this->backImage(), 0, h * 4, this ); // +Z
        ::Draw( this->frontImage(), 0, h * 5, this ); // -Z
    }
    case Horizontal:
    {
        // Horizontal layout
        //
        // [+X] [-X] [+Y] [-Y] [+Z] [-Z]
        //
        this->create( w * 6, h, kvs::ImageBase::Color );
        ::Draw( this->rightImage(), 0, 0, this ); // +X
        ::Draw( this->leftImage(), w, 0, this ); // -X
        ::Draw( this->topImage(), w * 2, 0, this ); // +Y
        ::Draw( this->bottomImage(), w * 3, 0, this ); // -Y
        ::Draw( this->backImage(), w * 4, 0, this ); // +Z
        ::Draw( this->frontImage(), w * 5, 0, this ); // -Z
    }
    default:
        break;
    }
}

} // end of namespace kvs
