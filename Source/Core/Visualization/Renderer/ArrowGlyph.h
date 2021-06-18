/*****************************************************************************/
/**
 *  @file   ArrowGlyph.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "GlyphBase.h"
#include <kvs/Module>
#include <kvs/Type>
#include <kvs/Deprecated>


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
    ArrowType m_arrow_type; ///< arrow type
    const kvs::VolumeObjectBase* m_volume; ///< pointer to the volume object (reference)

public:
    ArrowGlyph();
    ArrowGlyph( const kvs::VolumeObjectBase* volume );
    ArrowGlyph( const kvs::VolumeObjectBase* volume, const kvs::TransferFunction& transfer_function );

    void setArrowType( const ArrowType type ) { m_arrow_type = type; }
    void setArrowTypeToLine() { this->setArrowType( LineArrow ); }
    void setArrowTypeToTube() { this->setArrowType( TubeArrow ); }
    ArrowType arrowType() const { return m_arrow_type; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void attach_volume( const kvs::VolumeObjectBase* volume );
    void draw();
    void draw_lines();
    void draw_tubes();
    void draw_line_element( const kvs::RGBColor& color, const kvs::UInt8 opacity );
    void draw_tube_element( const kvs::RGBColor& color, const kvs::UInt8 opacity );
    void initialize();

public:
    KVS_DEPRECATED( void setType( const ArrowType type ) ) { this->setArrowType( type ); }
    KVS_DEPRECATED( ArrowType type() const ) { return this->arrowType(); }
};

} // end of namespace kvs
