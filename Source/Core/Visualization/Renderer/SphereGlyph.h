/*****************************************************************************/
/**
 *  @file   SphereGlyph.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SphereGlyph.h 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__SPHERE_GLYPH_H_INCLUDE
#define KVS__SPHERE_GLYPH_H_INCLUDE

#include "GlyphBase.h"
#include <kvs/PointObject>
#include <kvs/Module>
#include <kvs/OpenGL>
#include <kvs/Deprecated>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;
class PointObject;
class VolumeObjectBase;
class TransferFunction;
class RGBColor;

/*===========================================================================*/
/**
 *  @brief  Sphere glyph class.
 */
/*===========================================================================*/
class SphereGlyph : public kvs::GlyphBase
{
    kvsModule( kvs::SphereGlyph, Renderer );
    kvsModuleBaseClass( kvs::GlyphBase );

private:

    size_t m_nslices; ///< number of subdivisions around the z-axis (lines of longitude)
    size_t m_nstacks; ///< number of subdivisions along the z-axis (lines of latitude)
    const kvs::PointObject* m_point; ///< pointer to the point object (reference)
    const kvs::VolumeObjectBase* m_volume; ///< pointer to the volume object (reference)

public:

    SphereGlyph();
    SphereGlyph( const kvs::PointObject* point );
    SphereGlyph( const kvs::VolumeObjectBase* volume );
    SphereGlyph( const kvs::VolumeObjectBase* volume, const kvs::TransferFunction& transfer_function );

    void setNumberOfSlices( const size_t nslices ) { m_nslices = nslices; }
    void setNumberOfStacks( const size_t nstacks ) { m_nstacks = nstacks; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:

    void attach_point( const kvs::PointObject* point );
    void attach_volume( const kvs::VolumeObjectBase* volume );
    void draw();
    void draw_element( const kvs::RGBColor& color, const kvs::UInt8 opacity );
    void initialize();

public:
    KVS_DEPRECATED( void setNSlices( const size_t nslices ) ) { this->setNumberOfSlices( nslices ); }
    KVS_DEPRECATED( void setNStacks( const size_t nstacks ) ) { this->setNumberOfStacks( nstacks ); }
};

} // end of namespace kvs

#endif // KVS__SPHERE_GLYPH_H_INCLUDE
