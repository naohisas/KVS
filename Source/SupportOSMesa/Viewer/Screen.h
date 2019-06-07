#pragma once
#include "ScreenBase.h"
#include <string>
#include <kvs/Scene>
#include <kvs/ObjectBase>
#include <kvs/RendererBase>
#include <kvs/VisualizationPipeline>
#include <kvs/EventListener>


namespace kvs
{

namespace osmesa
{

class Screen : public kvs::osmesa::ScreenBase
{
public:
    typedef kvs::osmesa::ScreenBase BaseClass;
    typedef kvs::Scene::ControlTarget ControlTarget;

private:
    bool m_enable_default_paint_event; ///< flag for default paint event
    kvs::Scene* m_scene; ///< scene

public:
    Screen();
    virtual ~Screen();

    kvs::Scene* scene() { return m_scene; }

    void setBackgroundColor( const kvs::RGBColor& color );
    void setBackgroundColor( const kvs::RGBColor& color1, const kvs::RGBColor& color2 );
    void setBackgroundImage( const kvs::ColorImage& image );
    void setEvent( kvs::EventListener* event, const std::string& name = "" );
    void addEvent( kvs::EventListener* event, const std::string& name = "" );

    const std::pair<int,int> registerObject( kvs::ObjectBase* object, kvs::RendererBase* renderer = 0 );
    const std::pair<int,int> registerObject( kvs::VisualizationPipeline* pipeline );

    virtual void enable();
    virtual void disable();
    virtual void reset();

    virtual void initializeEvent();
    virtual void paintEvent();

protected:
    virtual void defaultPaintEvent();
};

} // end of namespace osmesa

} // end of namespace kvs
