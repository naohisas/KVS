#include "SphericalMapImage.h"
#include "CubeMapImage.h"
#include <kvs/ColorImage>
#include <kvs/RGBColor>
#include <kvs/OpenMP>


namespace
{

inline kvs::RGBColor Interp(
    const kvs::ColorImage& image,
    const float x,
    const float y )
{
    KVS_ASSERT( 0.0f <= x && x <= image.width() - 1.0f );
    KVS_ASSERT( 0.0f <= y && y <= image.height() - 1.0f );

    const size_t x0 = kvs::Math::Floor(x);
    const size_t y0 = kvs::Math::Floor(y);
    const size_t x1 = x0 + ( image.width() - 1 > x0 ? 1 : 0 );
    const size_t y1 = y0 + ( image.height() - 1 > y0 ? 1 : 0 );
    const float xratio = x - x0;
    const float yratio = y - y0;

    const kvs::Vec3 p0 = image.pixel( x0, y0 ).toVec3();
    const kvs::Vec3 p1 = image.pixel( x0, y1 ).toVec3();
    const kvs::Vec3 p2 = image.pixel( x1, y0 ).toVec3();
    const kvs::Vec3 p3 = image.pixel( x1, y1 ).toVec3();

    const kvs::Vec3 d = p0 * ( 1.0f - xratio ) + p2 * xratio;
    const kvs::Vec3 e = p1 * ( 1.0f - xratio ) + p3 * xratio;
    return kvs::RGBColor( d * ( 1.0f - yratio ) + e * yratio );
}

}


namespace kvs
{

SphericalMapImage::SphericalMapImage( const kvs::ColorImage& image ):
    kvs::ColorImage( image )
{
}

SphericalMapImage::SphericalMapImage( const kvs::CubeMapImage& cube_image )
{
    this->stitch( cube_image );
}

void SphericalMapImage::stitch( const kvs::CubeMapImage& cube_image )
{
    const size_t w = cube_image.frontImage().width();
    const size_t h = cube_image.frontImage().height();

    this->create( w * 4, h * 3, kvs::ImageBase::Color );
    KVS_OMP_PARALLEL_FOR( schedule(dynamic) )
    for ( size_t j = 0; j < this->height(); j++ )
    {
        const float v = 1.0f - (float)j / ( this->height() - 1 );
        const float theta = v * kvs::Math::pi;
        for ( size_t i = 0; i < this->width(); i++ )
        {
            const float u = (float)i / ( this->width() - 1 );
            const float phi = u * 2.0f * kvs::Math::pi;

            const float x = std::sin( phi ) * std::sin( theta ) * -1.0f;
            const float y = std::cos( theta );
            const float z = std::cos( phi ) * std::sin( theta ) * -1.0f;

            const float a = kvs::Math::Max(
                kvs::Math::Abs( x ), kvs::Math::Abs( y ), kvs::Math::Abs( z ) );
            const float xa = x / a;
            const float ya = y / a;
            const float za = z / a;

            kvs::RGBColor pixel;
            if ( xa == 1 )
            {
                const float si = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f - 1.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( cube_image.rightImage(), si, sj );
            }
            else if ( xa == -1 )
            {
                const float si = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( cube_image.leftImage(), si, sj );
            }
            else if ( ya == 1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f - 1.0f ) * ( h - 1 ) );
                pixel = ::Interp( cube_image.bottomImage(), si, sj );
            }
            else if ( ya == -1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( cube_image.topImage(), si, sj );
            }
            else if ( za == 1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( cube_image.frontImage(), si, sj );
            }
            else if ( za == -1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f - 1.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( cube_image.backImage(), si, sj );
            }
            this->setPixel( i, j, pixel );
        }
    }
}

} // end of namespace kvs
