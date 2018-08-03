/****************************************************************************/
/**
 *  @file   Light.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Light.h 1799 2014-08-04 05:36:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once

#include <kvs/XformControl>
#include <kvs/Vector3>
#include <kvs/Camera>
#include <kvs/Deprecated>


namespace kvs
{

class RGBAColor;

/*==========================================================================*/
/**
 *  Light class
 */
/*==========================================================================*/
class Light : public kvs::XformControl
{
private:
    kvs::Vec3 m_transform_center;
    unsigned int  m_id; ///< light ID
    kvs::Vec3 m_diffuse; ///< diffuse color
    kvs::Vec3 m_ambient; ///< ambient color
    kvs::Vec3 m_specular; ///< specular color

public:

    static void SetModelLocalViewer( bool flag );
    static void SetModelTwoSide( bool flag );
    static void SetModelAmbient( float ambient[4] );

public:

    Light();
    virtual ~Light();

    void setID( const unsigned int id );
    void setPosition( const float x, const float y, const float z );
    void setPosition( const kvs::Vec3& position );
    void setColor( const float r, const float g, const float b );
    void setColor( const kvs::RGBAColor& color );
    void setDiffuse( const float r, const float g, const float b );
    void setDiffuse( const kvs::RGBAColor& color );
    void setAmbient( const float r, const float g, const float b );
    void setAmbient( const kvs::RGBAColor& color );
    void setSpecular( const float r, const float g, const float b );
    void setSpecular( const kvs::RGBAColor& color );

    unsigned int id() const { return m_id; }
    const kvs::Vec3& diffuse() const { return m_diffuse; }
    const kvs::Vec3& ambient() const { return m_ambient; }
    const kvs::Vec3& specular() const { return m_specular; }
    const kvs::Vec3 position() const;

    virtual void update( const kvs::Camera* camera );
    void initialize();
    void on() const;
    void off() const;
    bool isEnabled() const;

    void resetXform();
    void rotate( const kvs::Mat3& rotation );
    void translate( const kvs::Vec3& translation );
    void scale( const kvs::Vec3& scaling );

public:

    KVS_DEPRECATED( static void setModelLocalViewer( bool flag ) ) { SetModelLocalViewer( flag ); }
    KVS_DEPRECATED( static void setModelTwoSide( bool flag ) ) { SetModelTwoSide( flag ); }
    KVS_DEPRECATED( static void setModelAmbient( float ambient[4] ) ) { SetModelAmbient( ambient ); }
};

} // end of namespace kvs
