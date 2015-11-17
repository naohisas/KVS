/*****************************************************************************/
/**
 *  @file   ArrowGlyph.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ArrowGlyph.h 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__ARROW_GLYPH_H_INCLUDE
#define KVS__ARROW_GLYPH_H_INCLUDE

#include "GlyphBase.h"
#include <kvs/Module>
#include <kvs/Type>
#include <kvs/OpenGL>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;
class VolumeObjectBase;
class TransferFunction;
class RGBColor;

/*===========================================================================*/
/**
 *  @brief  ArrowGlyph class.
 */
/*===========================================================================*/
class ArrowGlyph : public kvs::GlyphBase
{
    kvsModule( kvs::ArrowGlyph, Renderer );
    kvsModuleBaseClass( kvs::GlyphBase );

public:

    enum ArrowType
    {
        LineArrow = 0,
        TubeArrow
    };

private:

    ArrowType m_type; ///< arrow type
    const kvs::VolumeObjectBase* m_volume; ///< pointer to the volume object (reference)

public:

    ArrowGlyph();
    ArrowGlyph( const kvs::VolumeObjectBase* volume );
    ArrowGlyph( const kvs::VolumeObjectBase* volume, const kvs::TransferFunction& transfer_function );

    void setType( const ArrowType type ) { m_type = type; }
    ArrowType type() const { return m_type; }
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:

    void attach_volume( const kvs::VolumeObjectBase* volume );
    void draw();
    void draw_lines();
    void draw_tubes();
    void draw_line_element( const kvs::RGBColor& color, const kvs::UInt8 opacity );
    void draw_tube_element( const kvs::RGBColor& color, const kvs::UInt8 opacity );
    void initialize();
};

} // end of namespace kvs

#endif // KVS__ARROW_GLYPH_H_INCLUDE
