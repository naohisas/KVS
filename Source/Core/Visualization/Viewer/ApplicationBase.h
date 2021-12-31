/*****************************************************************************/
/**
 *  @file   ApplicationBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Compiler>
#include <kvs/UIColor>
#include <kvs/Rectangle>
#include <list>
#if defined( KVS_COMPILER_VC )
#pragma warning(disable:4800)
#endif


namespace kvs
{

class ScreenBase;

/*===========================================================================*/
/**
 *  @brief  ApplicationBase class.
 */
/*===========================================================================*/
class ApplicationBase
{
private:
    int m_argc = 0; ///< argument count
    char** m_argv = nullptr; ///< argument values
    std::list<kvs::ScreenBase*> m_screens{}; ///< list of the pointer to the screen
    kvs::UIColor& m_color = kvs::UIColor::Instance(); ///< UI element color
    kvs::Rectangle m_desktop{}; ///< desktop rectangle

public:
    ApplicationBase( int argc, char** argv );
    virtual ~ApplicationBase() = default;

    void setColorMode( const kvs::UIColor::Mode mode ) { m_color.setMode( mode ); }
    void setColorModeToDark() { m_color.setModeToDark(); }
    void setColorModeToLight() { m_color.setModeToLight(); }

    int argc() { return m_argc; }
    char** argv() { return m_argv; }
    const kvs::Rectangle& desktop() const { return m_desktop; }

    void attach( kvs::ScreenBase* screen );
    void detach( kvs::ScreenBase* screen );
    virtual int run() = 0;
    virtual void quit() = 0;

protected:
    std::list<kvs::ScreenBase*>& screens() { return m_screens; }
    void setDesktop( const kvs::Rectangle& rect ) { m_desktop = rect; }
};

} // end of namespace kvs
