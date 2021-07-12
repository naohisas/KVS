/*****************************************************************************/
/**
 *  @file   ColorStream.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/String>
#include <kvs/Deprecated>


#define KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( color_name )          \
    template <typename T>                                               \
    static std::string (color_name)( const T t )                        \
    {                                                                   \
        return std::string( CodeString( Color##color_name ) ).append( t ).append( CodeString( ColorDefault ) ); \
    }                                                                   \
    template <typename T>                                               \
    static std::string Background##color_name( const T t )              \
    {                                                                   \
        return std::string( CodeString( Color##color_name + 10 ) ).append( t ).append( CodeString( ColorDefault + 10 ) ); \
    }                                                                   \
    static std::ostream& (color_name)( std::ostream& os ) { return os << CodeString( Color##color_name ); } \
    static std::ostream& Background##color_name( std::ostream& os ) { return os << CodeString( Color##color_name + 10 ); } \

#define KVS_COLOR_STREAM__STATIC_FORMAT_FUNCTIONS( format_name )        \
    template <typename T>                                               \
    static std::string (format_name)( const T t )                       \
    {                                                                   \
        return std::string( CodeString( Format##format_name##On ) ).append( t ).append( CodeString( FormatResetAll ) ); \
    }                                                                   \
    static std::ostream& (format_name)( std::ostream& os ) { return os << CodeString( Format##format_name##On ); } \
    static std::ostream& Reset##format_name( std::ostream& os ) { return os << CodeString( Format##format_name##Off ); } \

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Color stream class.
 */
/*===========================================================================*/
class ColorStream
{
public:
    enum Format
    {
        FormatResetAll = 0,
        FormatBoldOn = 1,
        FormatBoldOff = 21,
        FormatDimOn = 2,
        FormatDimOff = 22,
        FormatUnderlineOn = 4,
        FormatUnderlineOff = 24,
        FormatBlinkOn = 5,
        FormatBlinkOff = 25,
        FormatReverseOn = 7,
        FormatReverseOff = 27,
        FormatHideOn = 8,
        FormatHideOff = 28
    };

    enum Color
    {
        ColorDefault = 39,
        ColorBlack = 30,
        ColorRed = 31,
        ColorGreen = 32,
        ColorYellow = 33,
        ColorBlue = 34,
        ColorMagenta = 35,
        ColorCyan = 36,
        ColorLightGray = 37,
        ColorDarkGray = 90,
        ColorLightRed = 91,
        ColorLightGreen = 92,
        ColorLightYellow = 93,
        ColorLightBlue = 94,
        ColorLightMagenta = 95,
        ColorLightCyan = 96,
        ColorWhite = 97
    };

public:
    static std::string CodeString( const int code )
    {
        return std::string( "\x1b[" + kvs::String::From( code ) + "m" );
    }

    static std::ostream& ResetForegroundColor( std::ostream& os ) { return os << CodeString( ColorDefault ); }
    static std::ostream& ResetBackgroundColor( std::ostream& os ) { return os << CodeString( ColorDefault + 10 ); }
    static std::ostream& Reset( std::ostream& os ) { return os << CodeString( FormatResetAll ); }

    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( Black );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( Red );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( Green );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( Yellow );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( Blue );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( Magenta );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( Cyan );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( LightGray );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( DarkGray );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( LightRed );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( LightGreen );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( LightYellow );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( LightBlue );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( LightMagenta );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( LightCyan );
    KVS_COLOR_STREAM__STATIC_COLOR_FUNCTIONS( White );

    KVS_COLOR_STREAM__STATIC_FORMAT_FUNCTIONS( Bold );
    KVS_COLOR_STREAM__STATIC_FORMAT_FUNCTIONS( Dim );
    KVS_COLOR_STREAM__STATIC_FORMAT_FUNCTIONS( Underline );
    KVS_COLOR_STREAM__STATIC_FORMAT_FUNCTIONS( Blink );
    KVS_COLOR_STREAM__STATIC_FORMAT_FUNCTIONS( Reverse );
    KVS_COLOR_STREAM__STATIC_FORMAT_FUNCTIONS( Hide );

private:
    Color m_foreground_color{ ColorDefault }; ///< foreground color
    Color m_background_color{ Color( ColorDefault + 10 ) }; ///< background color
    Format m_format_bold{ FormatBoldOff }; ///< bold format
    Format m_format_dim{ FormatDimOff }; ///< dim format
    Format m_format_underline{ FormatUnderlineOff }; ///< underline format
    Format m_format_blink{ FormatBlinkOff }; ///< blink format
    Format m_format_reverse{ FormatReverseOff }; ///< reverse format
    Format m_format_hide{ FormatHideOff }; ///< hide format

public:
    ColorStream() = default;

    void setForegroundColor( const Color color ) { m_foreground_color = color; }
    void setBackgroundColor( const Color color ) { m_background_color = Color( color + 10 ); }

    void setBoldEnabled( const bool enabled ) { m_format_bold = ( enabled ) ? FormatBoldOn : FormatBoldOff; }
    void setDimEnabled( const bool enabled ) { m_format_dim = ( enabled ) ? FormatDimOn : FormatDimOff; }
    void setUnderlineEnabled( const bool enabled ) { m_format_underline = ( enabled ) ? FormatUnderlineOn : FormatUnderlineOff; }
    void setBlinkEnabled( const bool enabled ) { m_format_blink = ( enabled ) ? FormatBlinkOn : FormatBlinkOff; }
    void setReverseEnabled( const bool enabled ) { m_format_reverse = ( enabled ) ? FormatReverseOn : FormatReverseOff; }
    void setHideEnabled( const bool enabled ) { m_format_hide = ( enabled ) ? FormatHideOn : FormatHideOff; }

    bool isBoldEnabled() const { return m_format_bold == FormatBoldOn; }
    bool isDimEnabled() const { return m_format_dim == FormatDimOn; }
    bool isUnderlineEnabled() const { return m_format_underline == FormatUnderlineOn; }
    bool isBlinkEnabled() const { return m_format_blink == FormatBlinkOn; }
    bool isReverseEnabled() const { return m_format_reverse == FormatReverseOn; }
    bool isHideEnabled() const { return m_format_hide == FormatHideOn; }

    ColorStream& reset()
    {
        m_foreground_color = ColorDefault;
        m_background_color = Color( ColorDefault + 10 );
        m_format_bold = FormatBoldOff;
        m_format_dim = FormatDimOff;
        m_format_underline = FormatUnderlineOff;
        m_format_blink = FormatBlinkOff;
        m_format_reverse = FormatReverseOff;
        m_format_hide = FormatHideOff;
        return *this;
    }

    friend std::ostream& operator << ( std::ostream& os, const ColorStream& cs )
    {
        return os << CodeString( cs.m_foreground_color )
                  << CodeString( cs.m_background_color )
                  << CodeString( cs.m_format_bold )
                  << CodeString( cs.m_format_dim )
                  << CodeString( cs.m_format_underline )
                  << CodeString( cs.m_format_blink )
                  << CodeString( cs.m_format_reverse )
                  << CodeString( cs.m_format_hide );
    }

public:
    KVS_DEPRECATED( void setEnabledBold( const bool on ) ) { this->setBoldEnabled( on ); }
    KVS_DEPRECATED( void setEnabledDim( const bool on ) ) { this->setDimEnabled( on ); }
    KVS_DEPRECATED( void setEnabledUnderline( const bool on ) ) { this->setUnderlineEnabled( on ); }
    KVS_DEPRECATED( void setEnabledBlink( const bool on ) ) { this->setBlinkEnabled( on ); }
    KVS_DEPRECATED( void setEnabledReverse( const bool on ) ) { this->setReverseEnabled( on ); }
    KVS_DEPRECATED( void setEnabledHide( const bool on ) ) { this->setHideEnabled( on ); }
    KVS_DEPRECATED( void enableBold() ) { this->setBoldEnabled( true ); }
    KVS_DEPRECATED( void enableDim() ) { this->setDimEnabled( true ); }
    KVS_DEPRECATED( void enableUnderline() ) { this->setUnderlineEnabled( true ); }
    KVS_DEPRECATED( void enableBlink() ) { this->setBlinkEnabled( true ); }
    KVS_DEPRECATED( void enableReverse() ) { this->setReverseEnabled( true ); }
    KVS_DEPRECATED( void enableHide() ) { this->setHideEnabled( true ); }
    KVS_DEPRECATED( void disableBold() ) { this->setBoldEnabled( false ); }
    KVS_DEPRECATED( void disableDim() ) { this->setDimEnabled( false ); }
    KVS_DEPRECATED( void disableUnderline() ) { this->setUnderlineEnabled( false ); }
    KVS_DEPRECATED( void disableBlink() ) { this->setBlinkEnabled( false ); }
    KVS_DEPRECATED( void disableReverse() ) { this->setReverseEnabled( false ); }
    KVS_DEPRECATED( void disableHide() ) { this->setHideEnabled( false ); }
};

} // end of namespace kvs
