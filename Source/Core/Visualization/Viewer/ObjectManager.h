/****************************************************************************/
/**
 *  @file   ObjectManager.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <map>
#include <kvs/ObjectBase>
#include <kvs/Tree>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
*  Object manager class.
*/
/*==========================================================================*/
class ObjectManager : public kvs::ObjectBase
{
private:
    using BaseClass = kvs::ObjectBase;
    using ObjectTree = kvs::Tree<kvs::ObjectBase*>;
    using ObjectIterator = ObjectTree::iterator;
    using ObjectPair = std::pair<int,ObjectIterator>;
    using ObjectMap = std::map<int,ObjectIterator>;

private:
    bool m_has_active_object = false; ///< If active object exists true.
    ObjectIterator m_root{}; ///< pointer to the root of the tree
    ObjectIterator m_active_object{}; ///< pointer to the active object
    ObjectMap m_object_map{}; ///< object map
    int m_current_object_id = 0; ///< current object ID
    ObjectTree m_object_tree{}; ///< object tree

public:
    ObjectManager();
    virtual ~ObjectManager();

    int insert( kvs::ObjectBase* object );
    int insert( int id, kvs::ObjectBase* object );
    void erase( bool delete_flag = true );
    void erase( int id, bool delete_flag = true );
    void erase( std::string name, bool delete_flag = true );
    void change( int id, kvs::ObjectBase* object, bool delete_flag = true );
    void change( std::string name, kvs::ObjectBase* object, bool delete_flag = true );
    kvs::ObjectBase* object();
    kvs::ObjectBase* object( int id );
    kvs::ObjectBase* object( std::string name );
    kvs::ObjectBase* activeObject();

    int numberOfObjects() const;
    bool hasObject() const;
    bool hasActiveObject() const;
    int objectID( const kvs::ObjectBase *object ) const;
    int parentObjectID( const ObjectIterator it ) const;
    int parentObjectID( const kvs::ObjectBase *object ) const;
    int parentObjectID( int id ) const;
    int activeObjectID() const;

    kvs::Xform xform() const;
    kvs::Xform xform( int id ) const;
    void resetXform();
    void resetXform( int id );
    void resetActiveObjectXform();

    bool setActiveObject( int id );
    void eraseActiveObject();
    void releaseActiveObject();

    void rotate( const kvs::Mat3& rotation );
    void translate( const kvs::Vec3& translation );
    void scale( const kvs::Vec3& scaling );

    void updateExternalCoords();

private:
    void insert_root();
    void update_normalize_parameters( const kvs::Vec3& min_ext, const kvs::Vec3& max_ext );
    void update_normalize_parameters();
    kvs::ObjectBase* get_control_target();
    kvs::Vec3 get_rotation_center( kvs::ObjectBase* object );
    ObjectIterator get_control_first_pointer();
    ObjectIterator get_control_last_pointer();
    kvs::Xform get_centering_xform( kvs::ObjectBase* object );
    void push_centering_xform();
    void pop_centering_xform();

private:
    ObjectManager( const ObjectManager& );
    ObjectManager& operator =( const ObjectManager& );
};

} // end of namespace kvs
