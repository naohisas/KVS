#pragma once
#include <kvs/ColorImage>


namespace kvs
{

class CubicImage;

/*===========================================================================*/
/**
 *  @brief  Spherical map image class.
 */
/*===========================================================================*/
class SphericalImage : public kvs::ColorImage
{
public:
    SphericalImage( const kvs::ColorImage& image );
    SphericalImage( const kvs::CubicImage& cubic_image );

    void stitch( const kvs::CubicImage& cubic_image );
};

} // end of namespace kvs
