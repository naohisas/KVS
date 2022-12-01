/****************************************************************************/
/**
 *  @file   StructuredVolumeObjectList.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/ObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/Module>
#include <vector>
#include <string>
#include <functional>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  StructuredVolumeObjectList class.
 */
/*===========================================================================*/
class StructuredVolumeObjectList : public kvs::ObjectBase
{
    kvsModule( kvs::StructuredVolumeObjectList, Object );
    kvsModuleBaseClass( kvs::ObjectBase );

public:
    using Filename = std::string;
    using FilenameList = std::vector<Filename>;
    using Object = kvs::StructuredVolumeObject;
    using ObjectList = std::vector<Object>;
    using Importer = std::function<Object(const Filename&)>;
    static Object DefaultImporter( const Filename& filename );

private:
    FilenameList m_filenames{}; ///< filename list
    ObjectList m_objects{}; ///< object list
    Importer m_importer{ DefaultImporter }; ///< importer
    kvs::Real64 m_min_value = 0.0; ///< min value
    kvs::Real64 m_max_value = 0.0; ///< max value

public:
    StructuredVolumeObjectList() = default;
    StructuredVolumeObjectList( const ObjectList& objects ): m_objects( objects ) {}
    StructuredVolumeObjectList(
        const FilenameList& filenames,
        Importer importer = DefaultImporter ):
        m_filenames( filenames ),
        m_importer( importer ) {}

    const ObjectList& objects() const { return m_objects; }
    const FilenameList& filenames() const { return m_filenames; }

    void setObjects( const ObjectList& objects ) { m_objects = objects; }
    void setFilenames( const FilenameList& filenames ) { m_filenames = filenames; }
    void setImporter( Importer importer ) { m_importer = importer; }

    kvs::Real64 minValue() const { return m_min_value; }
    kvs::Real64 maxValue() const { return m_max_value; }
    void updateMinMaxValues();
    void setMinMaxValues( const float min_value, const float max_value );

    size_t size() const;
    Object load( const size_t index ) const;
    bool load();
};

} // end of namespace kvs
