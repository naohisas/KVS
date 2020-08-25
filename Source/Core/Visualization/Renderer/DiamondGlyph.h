/*****************************************************************************/
/**
 *  @file   DiamondGlyph.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__DIAMOND_GLYPH_H_INCLUDE
#define KVS__DIAMOND_GLYPH_H_INCLUDE

#include "GlyphBase.h"
#include <kvs/MapperBase>
#include <kvs/Module>


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
 *  @brief  Diamond glyph class.
 */
/*===========================================================================*/
class DiamondGlyph : public kvs::GlyphBase
{
    kvsModule( kvs::DiamondGlyph, Renderer );
    kvsModuleBaseClass( kvs::GlyphBase );

private:

    const kvs::VolumeObjectBase* m_volume; ///< pointer to the volume object (reference)

public:

    DiamondGlyph();
    DiamondGlyph( const kvs::VolumeObjectBase* volume );
    DiamondGlyph( const kvs::VolumeObjectBase* volume, const kvs::TransferFunction& transfer_function );
    virtual ~DiamondGlyph();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:

    void attach_volume( const kvs::VolumeObjectBase* volume );
    void draw();
    void draw_element( const kvs::RGBColor& color, const kvs::UInt8 opacity );
    void initialize();
};

} // end of namespace kvs

#endif // KVS__DIAMOND_GLYPH_H_INCLUDE
