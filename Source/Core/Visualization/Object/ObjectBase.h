/*****************************************************************************/
/**
 *  @file   ObjectBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <iostream>
#include <kvs/XformControl>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/Matrix44>
#include <kvs/Module>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Object base class.
 */
/*==========================================================================*/
class ObjectBase : public kvs::XformControl
{
    kvsModuleBase( kvs::ObjectBase );

public:
    enum ObjectType
    {
        Geometry = 0, ///< Geometric object.
        Volume, ///< Volumetric object.
        Image, ///< Image object
        Table, ///< table object
        ObjectManager, ///< Object manager
        UnknownObject ///< unknown object (for user defined object data)
    };

private:
    ObjectType m_object_type = UnknownObject; ///< object type
    std::string m_name = "unknown"; ///< object name
    kvs::Vec3 m_min_object_coord{ -3.0f, -3.0f, -3.0f }; ///< min coord in the object coordinate system
    kvs::Vec3 m_max_object_coord{  3.0f,  3.0f,  3.0f }; ///< max coord in the object coordinate system
    kvs::Vec3 m_min_external_coord{ -3.0f, -3.0f, -3.0f }; ///< min coord in the external coordinate system
    kvs::Vec3 m_max_external_coord{  3.0f,  3.0f,  3.0f }; ///< max coord in the external coordinate system
    bool m_has_min_max_object_coords = false; ///< has min-max coorinate values ?
    bool m_has_min_max_external_coords = false; ///< has min-max coorinate values ?
    kvs::Vec3 m_object_center{ 0.0f, 0.0f, 0.0f }; ///< center of the object in object coordinate system
    kvs::Vec3 m_external_center{ 0.0f, 0.0f, 0.0f }; ///< center of the object in external object coordinate system
    kvs::Vec3 m_normalize{ 1.0f, 1.0f, 1.0f }; ///< normalize parameter
    bool m_visible = true; ///< visiblity for the object

public:
    ObjectBase( const ObjectType type = UnknownObject ): m_object_type( type ) {};
    virtual ~ObjectBase() = default;

    ObjectBase& operator = ( const ObjectBase& object );

    void setName( const std::string& name ) { m_name = name; }
    void setMinMaxObjectCoords( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord );
    void setMinMaxExternalCoords( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord );
    void setVisible( const bool visible = true ) { m_visible = visible; }
    void show() { this->setVisible( true ); }
    void hide() { this->setVisible( false ); }
    virtual void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    virtual bool read( const std::string& filename );
    virtual bool write( const std::string& filename, const bool ascii = true, const bool external = false ) const;

    ObjectType objectType() const { return m_object_type; }
    const std::string& name() const { return m_name; }
    const kvs::Vec3& minObjectCoord() const { return m_min_object_coord; }
    const kvs::Vec3& maxObjectCoord() const { return m_max_object_coord; }
    const kvs::Vec3& minExternalCoord() const { return m_min_external_coord; }
    const kvs::Vec3& maxExternalCoord() const { return m_max_external_coord; }
    bool hasMinMaxObjectCoords() const { return m_has_min_max_object_coords; }
    bool hasMinMaxExternalCoords() const { return m_has_min_max_external_coords; }
    const kvs::Vec3& objectCenter() const { return m_object_center; }
    const kvs::Vec3& externalCenter() const { return m_external_center; }
    const kvs::Vec3& normalize() const { return m_normalize; }
    const kvs::Mat4 modelingMatrix() const { return this->xform().toMatrix(); }
    bool isVisible() const { return m_visible; }
//    bool isShown() const { return m_visible; }

    void updateNormalizeParameters();
    virtual void updateMinMaxCoords() {};

protected:
    void setObjectType( ObjectType object_type ) { m_object_type = object_type; }
    void setObjectCenter( const kvs::Vec3& object_center ) { m_object_center = object_center; }
    void setNormalize( const kvs::Vec3& normalize ) { m_normalize = normalize; }

public:
    KVS_DEPRECATED( ObjectBase( const kvs::Vec3& translation, const kvs::Vec3& scaling, const kvs::Mat3& rotation ) )
    {
        m_name = std::string("unknown");
        m_min_object_coord = kvs::Vec3( -3.0, -3.0, -3.0 );
        m_max_object_coord = kvs::Vec3(  3.0,  3.0,  3.0 );
        m_min_external_coord = kvs::Vec3( -3.0, -3.0, -3.0 );
        m_max_external_coord = kvs::Vec3(  3.0,  3.0,  3.0 );
        m_has_min_max_object_coords = false;
        m_has_min_max_external_coords = false;
        m_visible = true;

        this->setXform( kvs::Xform( translation, scaling, rotation ) );
        this->saveXform();
    }

    KVS_DEPRECATED( void rotate( const kvs::Mat3& rotation, const kvs::Vec3& center ) )
    {
        const kvs::Xform x = kvs::Xform::Translation( center )
                           * kvs::Xform::Rotation( rotation )
                           * kvs::Xform::Translation( -center );
        this->multiplyXform( x );
    }

    KVS_DEPRECATED( void scale( const kvs::Vec3& scaling, const kvs::Vec3& center ) )
    {
        const kvs::Xform x = kvs::Xform::Translation( center )
                           * kvs::Xform::Scaling( scaling )
                           * kvs::Xform::Translation( -center );
        this->multiplyXform( x );
    }

    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const ObjectBase& object ) );
    KVS_DEPRECATED( const kvs::Vec3& externalPosition() const ) { return this->externalCenter(); }
    KVS_DEPRECATED( bool isShown() const ) { return this->isVisible(); }
};

} // end of namespace kvs
