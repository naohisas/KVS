/*****************************************************************************/
/**
 *  @file   Argument.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Argument.cpp 1517 2013-04-09 11:55:49Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Argument.h"
#include <kvs/DebugNew>
#include <kvs/Bounds>
#include <kvs/LineObject>
#include <kvs/PointRenderer>
#include <kvs/LineRenderer>
#include <kvs/PolygonRenderer>
#include <kvs/Background>
#include <kvs/glut/Axis3D>
#include "CommandName.h"
#include "PointRenderer.h"
#include "LineRenderer.h"
#include "PolygonRenderer.h"
#include "Isosurface.h"
#include "SlicePlane.h"
#include "OrthoSlice.h"
#include "TransferFunction.h"
#include "ExtractEdges.h"
#include "ExtractVertices.h"
#include "ExternalFaces.h"
#include "RayCastingRenderer.h"
#include "ParticleBasedRenderer.h"
#include "Histogram.h"


namespace kvsview
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for kvsview.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvs::CommandLine( argc, argv, kvsview::CommandName )
{
    addHelpOption();
    addOption( "help", "Output help message for each process. (ex. -help Isosurface)", 1 );
    addOption( kvsview::PointRenderer::CommandName, kvsview::PointRenderer::Description, 0 );
    addOption( kvsview::LineRenderer::CommandName, kvsview::LineRenderer::Description, 0 );
    addOption( kvsview::PolygonRenderer::CommandName, kvsview::PolygonRenderer::Description, 0 );
    addOption( kvsview::Isosurface::CommandName, kvsview::Isosurface::Description, 0 );
    addOption( kvsview::SlicePlane::CommandName, kvsview::SlicePlane::Description, 0 );
    addOption( kvsview::OrthoSlice::CommandName, kvsview::OrthoSlice::Description, 0 );
    addOption( kvsview::TransferFunction::CommandName, kvsview::TransferFunction::Description, 0 );
    addOption( kvsview::ExtractEdges::CommandName, kvsview::ExtractEdges::Description, 0 );
    addOption( kvsview::ExtractVertices::CommandName, kvsview::ExtractVertices::Description, 0 );
    addOption( kvsview::ExternalFaces::CommandName, kvsview::ExternalFaces::Description, 0 );
    addOption( kvsview::RayCastingRenderer::CommandName, kvsview::RayCastingRenderer::Description, 0 );
    addOption( kvsview::ParticleBasedRenderer::CommandName, kvsview::ParticleBasedRenderer::Description, 0 );
    addOption( kvsview::Histogram::CommandName, kvsview::Histogram::Description, 0 );
    addValue( "input data file", false );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument::Common class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Common::Common( int argc, char** argv ):
    kvs::CommandLine( argc, argv, kvsview::CommandName )
{
    this->set_options();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument::Common class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 *  @param  command [in] command name
 */
/*===========================================================================*/
Argument::Common::Common( int argc, char** argv, const std::string& command ):
    kvs::CommandLine( argc, argv, kvsview::CommandName + " -" + command )
{
    this->set_options();
}

/*===========================================================================*/
/**
 *  @brief  Sets common options.
 */
/*===========================================================================*/
void Argument::Common::set_options()
{
    addHelpOption("help");
    addOption("antialiasing", "Enable anti-aliasing. (optional)", 0, false );
    addOption("axis", "Show axis. (optional)", 0, false );
    addOption("axis_color", "Set axis color. (default: 0 0 0)", 3, false );
    addOption("axis_label_color", "Set axis label color. (default: 0 0 0)", 3, false );
    addOption("axis_line_width", "Set axis line width. (default: 2)", 1, false );
    addOption("axis_gridline_width", "Set axis gridline width. (default: 1)", 1, false );
    addOption("axis_x_label", "Set label for the x-axis. (default: X)", 1, false );
    addOption("axis_y_label", "Set label for the y-axis. (default: Y)", 1, false );
    addOption("axis_z_label", "Set label for the z-axis. (default: Z)", 1, false );
    addOption("axis_gridlines", "Set number of gridlines for each axis. (default: 5 5 5)", 3, false );
    addOption("background_color", "Set background color. (default: 212 221 229)", 3, false );
    addOption("background_color2", "Set top and bottom side color on the background. (optional: <top: r g b> <bottom: r g b>)", 6, false );
    addOption("bounds", "Show bounding box. (optional)", 0, false );
    addOption("bounds_color", "Set bounding box color. (default: 0 0 0)", 3, false );
    addOption("screen_position", "Set screen position. (default: 0 0)", 2, false );
    addOption("screen_size", "Set screen size. (default: 512 512)", 2, false );
    addOption("output", "Output filename. (optional: <filename>)", 1, false );
    addOption("verbose", "Enable additional output message. (optional)", 0, false);
    addValue("input data file");
}

/*===========================================================================*/
/**
 *  @brief  Checks whether the verbose mode option is specified or not.
 *  @return true, if the verbose mode option is specified
 */
/*===========================================================================*/
bool Argument::Common::verboseMode()
{
    return this->hasOption("verbose");
}

/*===========================================================================*/
/**
 *  @brief  Applys the specified options to the screen information.
 *  @param  screen [in] screen
 */
/*===========================================================================*/
void Argument::Common::applyTo( kvs::glut::Screen& screen )
{
    int x = 0;
    int y = 0;
    if ( this->hasOption("screen_position") )
    {
        x = this->optionValue<int>("screen_position",0);
        y = this->optionValue<int>("screen_position",1);
    }

    int width = 512;
    int height = 512;
    if ( this->hasOption("screen_size") )
    {
        width  = this->optionValue<int>("screen_size",0);
        height = this->optionValue<int>("screen_size",1);
    }

    screen.setPosition( x, y );
    screen.setSize( width, height );
}

/*===========================================================================*/
/**
 *  @brief  Applys the specified options to the screen information.
 *  @param  screen [in] screen parameters
 *  @param  pipe [in] visualization pipeline
 */
/*===========================================================================*/
void Argument::Common::applyTo( kvs::glut::Screen& screen, kvs::VisualizationPipeline& pipe )
{
    // Axis.
    if ( this->hasOption("axis") )
    {
        kvs::glut::Axis3D* axis = new kvs::glut::Axis3D();

        axis->setAxisColor( kvs::RGBColor( 0, 0, 0 ) );
        if ( this->hasOption("axis_color") )
        {
            const kvs::UInt8 r( static_cast<kvs::UInt8>(this->optionValue<int>("axis_color",0)) );
            const kvs::UInt8 g( static_cast<kvs::UInt8>(this->optionValue<int>("axis_color",1)) );
            const kvs::UInt8 b( static_cast<kvs::UInt8>(this->optionValue<int>("axis_color",2)) );
            axis->setAxisColor( kvs::RGBColor( r, g, b ) );
        }

        axis->setLabelColor( kvs::RGBColor( 0, 0, 0 ) );
        if ( this->hasOption("axis_label_color") )
        {
            const kvs::UInt8 r( static_cast<kvs::UInt8>(this->optionValue<int>("axis_label_color",0)) );
            const kvs::UInt8 g( static_cast<kvs::UInt8>(this->optionValue<int>("axis_label_color",1)) );
            const kvs::UInt8 b( static_cast<kvs::UInt8>(this->optionValue<int>("axis_label_color",2)) );
            axis->setLabelColor( kvs::RGBColor( r, g, b ) );
        }

        axis->setAxisWidth( 3.0f );
        if ( this->hasOption("axis_line_width") )
        {
            const kvs::Real32 width( this->optionValue<kvs::Real32>("axis_line_width") );
            axis->setAxisWidth( width );
        }

        axis->setGridlineWidth( 1.0f );
        if ( this->hasOption("axis_gridline_width") )
        {
            const kvs::Real32 width( this->optionValue<kvs::Real32>("axis_gridline_width") );
            axis->setGridlineWidth( width );
        }

        axis->setXLabel("X");
        if ( this->hasOption("axis_x_label") )
        {
            const std::string label( this->optionValue<std::string>("axis_x_label") );
            axis->setXLabel( label );
        }

        axis->setYLabel("Y");
        if ( this->hasOption("axis_y_label") )
        {
            const std::string label( this->optionValue<std::string>("axis_y_label") );
            axis->setYLabel( label );
        }

        axis->setZLabel("Z");
        if ( this->hasOption("axis_z_label") )
        {
            const std::string label( this->optionValue<std::string>("axis_z_tag") );
            axis->setZLabel( label );
        }

        axis->setNumberOfGridlines( kvs::Vector3ui( 5, 5, 5 ) );
        if ( this->hasOption("axis_gridlines") )
        {
            const unsigned int nx( this->optionValue<unsigned int>("axis_gridlines",0) );
            const unsigned int ny( this->optionValue<unsigned int>("axis_gridlines",1) );
            const unsigned int nz( this->optionValue<unsigned int>("axis_gridlines",2) );
            axis->setNumberOfGridlines( kvs::Vector3ui( nx, ny, nz ) );
        }

        if ( this->hasOption("antialiasing") )
        {
            axis->enableAntiAliasing();
        }

        screen.registerObject( const_cast<kvs::ObjectBase*>( pipe.object() ), axis );
    }

    // Bounding box.
    if ( this->hasOption("bounds") )
    {
        kvs::Bounds* bounds = new kvs::Bounds();
        kvs::LineObject* line = bounds->outputLineObject( pipe.object() );
        delete bounds;

        line->setColor( kvs::RGBColor( 0, 0, 0 ) );
        if ( this->hasOption("bounds_color") )
        {
            const kvs::UInt8 r( static_cast<kvs::UInt8>(this->optionValue<int>("bounds_color",0)) );
            const kvs::UInt8 g( static_cast<kvs::UInt8>(this->optionValue<int>("bounds_color",1)) );
            const kvs::UInt8 b( static_cast<kvs::UInt8>(this->optionValue<int>("bounds_color",2)) );
            line->setColor( kvs::RGBColor( r, g, b ) );
        }

        kvs::VisualizationPipeline pipeline( line );
        kvs::PipelineModule renderer( new kvs::LineRenderer() );
        renderer.get<kvs::LineRenderer>()->disableShading();

        if ( this->hasOption("antialiasing") )
        {
            renderer.get<kvs::LineRenderer>()->enableAntiAliasing();
        }

        pipeline.connect( renderer );
        pipeline.exec();

        screen.registerObject( &pipeline );
    }

    // Background color.
    if ( this->hasOption("background_color") )
    {
        const kvs::UInt8 r( static_cast<kvs::UInt8>(this->optionValue<int>("background_color",0)) );
        const kvs::UInt8 g( static_cast<kvs::UInt8>(this->optionValue<int>("background_color",1)) );
        const kvs::UInt8 b( static_cast<kvs::UInt8>(this->optionValue<int>("background_color",2)) );
        screen.scene()->background()->setColor( kvs::RGBColor( r, g, b ) );
    }
    else if ( this->hasOption("background_color2") )
    {
        const kvs::UInt8 r1( static_cast<kvs::UInt8>(this->optionValue<int>("background_color2",0)) );
        const kvs::UInt8 g1( static_cast<kvs::UInt8>(this->optionValue<int>("background_color2",1)) );
        const kvs::UInt8 b1( static_cast<kvs::UInt8>(this->optionValue<int>("background_color2",2)) );
        const kvs::RGBColor top_color( r1, g1, b1 );

        const kvs::UInt8 r2( static_cast<kvs::UInt8>(this->optionValue<int>("background_color2",3)) );
        const kvs::UInt8 g2( static_cast<kvs::UInt8>(this->optionValue<int>("background_color2",4)) );
        const kvs::UInt8 b2( static_cast<kvs::UInt8>(this->optionValue<int>("background_color2",5)) );
        const kvs::RGBColor bottom_color( r2, g2, b2 );

        screen.scene()->background()->setColor( bottom_color, top_color );
    }

    // Anti-aliasing.
    if ( this->hasOption("antialiasing") )
    {
        // Enable multisampling.
        bool multisample = false;
        if ( multisample )
        {
            kvs::DisplayFormat format( screen.displayFormat() );
            format.setMultisampleBuffer( true );
            screen.setDisplayFormat( format );
        }

        // Apply to the geometry object if the antialiasing option is given.
        if ( pipe.object()->objectType() == kvs::ObjectBase::Geometry )
        {
            switch ( kvs::GeometryObjectBase::DownCast( pipe.object() )->geometryType() )
            {
            case kvs::GeometryObjectBase::Point:
            {
                kvs::RendererBase* renderer = const_cast<kvs::RendererBase*>( pipe.renderer() );
                static_cast<kvs::PointRenderer*>( renderer )->enableAntiAliasing( multisample );
                break;
            }
            case kvs::GeometryObjectBase::Line:
            {
                kvs::RendererBase* renderer = const_cast<kvs::RendererBase*>( pipe.renderer() );
                static_cast<kvs::LineRenderer*>( renderer )->enableAntiAliasing( multisample );
                break;
            }
            case kvs::GeometryObjectBase::Polygon:
            {
                kvs::RendererBase* renderer = const_cast<kvs::RendererBase*>( pipe.renderer() );
                static_cast<kvs::PolygonRenderer*>( renderer )->enableAntiAliasing( multisample );
                break;
            }
            default: break;
            }
        }
    }

/*
    if ( this->hasOption("output") )
    {
        const std::string filename = this->optionValue<std::string>("output");
    }
*/
}

} // end of namespace kvsview
