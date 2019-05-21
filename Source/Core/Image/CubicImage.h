#pragma once
#include <kvs/ColorImage>
#include <kvs/Vector3>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Cube map image class.
 */
/*===========================================================================*/
class CubicImage : public kvs::ColorImage
{
public:
    enum Direction
    {
        Right = 0,  // +x
        Left = 1,   // -x
        Top = 2,    // +y
        Bottom = 3, // -y
        Back = 4,   // +z
        Front = 5,  // -z
        NumberOfDirections = 6
    };

    enum Layout
    {
        Cross = 0,
        Rectangle = 1,
        Vertical = 2,
        Horizontal = 3
    };

public:
    static std::string DirectionName( const Direction dir );
    static kvs::Vec3 DirectionVector( const Direction dir );
    static kvs::Vec3 UpVector( const Direction dir );

private:
    Layout m_layout;
    kvs::ColorImage m_images[NumberOfDirections];

public:
    CubicImage(): m_layout( Cross ) {}

    Layout layout() const { return m_layout; }
    const kvs::ColorImage& image( const Direction& dir ) const { return m_images[dir]; }
    const kvs::ColorImage& rightImage() const { return this->image( Right ); }
    const kvs::ColorImage& leftImage() const { return this->image( Left ); }
    const kvs::ColorImage& topImage() const { return this->image( Top ); }
    const kvs::ColorImage& bottomImage() const { return this->image( Bottom ); }
    const kvs::ColorImage& backImage() const { return this->image( Back ); }
    const kvs::ColorImage& frontImage() const { return this->image( Front ); }

    void setLayout( const Layout layout ) { m_layout = layout; }
    void setLayoutToCross() { this->setLayout( Cross ); }
    void setLayoutToRectangle() { this->setLayout( Rectangle ); }
    void setLayoutToVertical() { this->setLayout( Vertical ); }
    void setLayoutToHorizontal() { this->setLayout( Horizontal ); }

    void setImage( const Direction dir, const kvs::ColorImage& image ) { m_images[dir] = image; }
    void setRightImage( const kvs::ColorImage& image ) { this->setImage( Right, image ); }
    void setLeftImage( const kvs::ColorImage& image ) { this->setImage( Left, image ); }
    void setTopImage( const kvs::ColorImage& image ) { this->setImage( Top, image ); }
    void setBottomImage( const kvs::ColorImage& image ) { this->setImage( Bottom, image ); }
    void setBackImage( const kvs::ColorImage& image ) { this->setImage( Back, image ); }
    void setFrontImage( const kvs::ColorImage& image ) { this->setImage( Front, image ); }

    void stitch();
};

} // end of namespace kvs
