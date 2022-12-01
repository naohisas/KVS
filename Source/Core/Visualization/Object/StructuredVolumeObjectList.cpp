/*****************************************************************************/
/**
 *  @file   StructuredVolumeObjectList.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "StructuredVolumeObjectList.h"
#include <kvs/StructuredVolumeImporter>


namespace kvs
{

using ThisClass = StructuredVolumeObjectList;

/*===========================================================================*/
/**
 *  @brief  Returns structured volume object imported from the specified file.
 *  @param  filename [in] filenmae
 *  @return imported structured volume object
 */
/*===========================================================================*/
ThisClass::Object ThisClass::DefaultImporter( const ThisClass::Filename& filename )
{
    return kvs::StructuredVolumeImporter( filename );
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of objects stored in the list.
 *  @return number of objects
 */
/*===========================================================================*/
size_t ThisClass::size() const
{
    const auto nobjs = m_objects.size();
    if ( nobjs > 0 ) { return nobjs; }
    return m_filenames.size();
}

/*===========================================================================*/
/**
 *  @brief  Updates min/max values of all objects.
 */
/*===========================================================================*/
void ThisClass::updateMinMaxValues()
{
    if ( this->size() > 0 )
    {
        auto object = this->load(0);
        object.updateMinMaxValues();

        auto min_value = object.minValue();
        auto max_value = object.maxValue();
        for ( size_t i = 1; i < this->size(); ++i )
        {
            object = this->load(i);
            object.updateMinMaxValues();
            min_value = kvs::Math::Min( min_value, object.minValue() );
            max_value = kvs::Math::Max( max_value, object.maxValue() );
        }

        m_min_value = min_value;
        m_max_value = max_value;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets min/max values.
 *  @param  min_value [in] min value
 *  @param  max_value [in] max value
 */
/*===========================================================================*/
void ThisClass::setMinMaxValues( const float min_value, const float max_value )
{
    m_min_value = min_value;
    m_max_value = max_value;
}

/*===========================================================================*/
/**
 *  @brief  Loads the object specified the index.
 *  @param  index [in] index of the objects
 *  @return Loaded object
 */
/*===========================================================================*/
ThisClass::Object ThisClass::load( const size_t index ) const
{
    if ( index < m_objects.size() ) { return m_objects[ index ]; }
    if ( index < m_filenames.size() ) { return m_importer( m_filenames[ index ] ); }
    return Object();
}

/*===========================================================================*/
/**
 *  @brief  Loads all of the objects.
 *  @return true, if the loading process is done successfully
 */
/*===========================================================================*/
bool ThisClass::load()
{
    if ( !m_objects.empty() )
    {
        m_objects.clear();
        m_objects.shrink_to_fit();
    }

    if ( m_filenames.empty() ) { return false; }

    bool ret = true;
    for ( auto& filename : m_filenames )
    {
        auto object = m_importer( filename );
        if ( object.values().empty() ) { ret = false; }
        m_objects.push_back( object );
    }

    return ret;
}

} // end of namespace kvs
