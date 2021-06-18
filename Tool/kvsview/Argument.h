/*****************************************************************************/
/**
 *  @file   Argument.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/CommandLine>
#include <kvs/VisualizationPipeline>
#include <kvs/Screen>
#include <kvs/RGBColor>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <string>


namespace kvsview
{

/*===========================================================================*/
/**
 *  @brief  Argument class for the kvsview command.
 */
/*===========================================================================*/
class Argument : public kvs::CommandLine
{
public:
    class Common;

public:
    Argument( int argc, char** argv );
};

/*===========================================================================*/
/**
 *  @brief  Common argument class for the kvsview command.
 */
/*===========================================================================*/
class Argument::Common : public kvs::CommandLine
{
public:
    Common( int argc, char** argv );
    Common( int argc, char** argv, const std::string& command );
    bool verboseMode();
    void applyTo( kvs::Screen& screen );
    void applyTo( kvs::Screen& screen, kvs::VisualizationPipeline& pipe );

    template <typename T>
    T valueAs( const std::string option, const T default_value ) const
    {
        if ( !this->hasOption( option ) ) { return default_value; }
        else { return this->optionValue<T>( option ); }
    }

    kvs::RGBColor valueAsRGBColor(
        const std::string option,
        const int offset = 0 ) const
    {
        auto r = static_cast<kvs::UInt8>( this->optionValue<int>( option, offset + 0 ) );
        auto g = static_cast<kvs::UInt8>( this->optionValue<int>( option, offset + 1 ) );
        auto b = static_cast<kvs::UInt8>( this->optionValue<int>( option, offset + 2 ) );
        return kvs::RGBColor( r, g, b );
    }

    kvs::RGBColor valueAsRGBColor(
        const std::string option,
        const kvs::RGBColor& default_value,
        const int offset = 0 ) const
    {
        if ( !this->hasOption( option ) ) { return default_value; }
        else { return this->valueAsRGBColor( option, offset ); }
    }

    template <typename T>
    kvs::Vector2<T> valueAsVec2(
        const std::string option,
        const int offset = 0 ) const
    {
        auto x = this->optionValue<T>( option, offset + 0 );
        auto y = this->optionValue<T>( option, offset + 1 );
        return kvs::Vector2<T>( x, y );
    }

    template <typename T>
    kvs::Vector2<T> valueAsVec2(
        const std::string option,
        const kvs::Vector2<T>& default_value,
        const int offset = 0 ) const
    {
        if ( !this->hasOption( option ) ) { return default_value; }
        else { return this->valueAsVec2<T>( option, offset ); }
    }

    template <typename T>
    kvs::Vector3<T> valueAsVec3(
        const std::string option,
        const int offset = 0 ) const
    {
        auto x = this->optionValue<T>( option, offset + 0 );
        auto y = this->optionValue<T>( option, offset + 1 );
        auto z = this->optionValue<T>( option, offset + 2 );
        return kvs::Vector3<T>( x, y, z );
    }

    template <typename T>
    kvs::Vector3<T> valueAsVec3(
        const std::string option,
        const kvs::Vector3<T>& default_value,
        const int offset = 0 ) const
    {
        if ( !this->hasOption( option ) ) { return default_value; }
        else { return this->valueAsVec3<T>( option, offset ); }
    }

    template <typename T>
    kvs::Vector4<T> valueAsVec4(
        const std::string option,
        const int offset = 0 ) const
    {
        auto x = this->optionValue<T>( option, offset + 0 );
        auto y = this->optionValue<T>( option, offset + 1 );
        auto z = this->optionValue<T>( option, offset + 2 );
        auto w = this->optionValue<T>( option, offset + 3 );
        return kvs::Vector4<T>( x, y, z, w );
    }

    template <typename T>
    kvs::Vector4<T> valueAsVec4(
        const std::string option,
        const kvs::Vector4<T>& default_value,
        const int offset = 0 ) const
    {
        if ( !this->hasOption( option ) ) { return default_value; }
        else { return this->valueAsVec4<T>( option, offset ); }
    }

private:
    void set_options();
};

} // end of namespace kvsview
