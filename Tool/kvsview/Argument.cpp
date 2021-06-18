/*****************************************************************************/
/**
 *  @file   Argument.cpp
 *  @author Naohisa Sakamoto
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
#include <kvs/UIColor>
#include <kvs/Axis3D>
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
    addOption( PointRenderer::CommandName, PointRenderer::Description, 0 );
    addOption( LineRenderer::CommandName, LineRenderer::Description, 0 );
    addOption( PolygonRenderer::CommandName, PolygonRenderer::Description, 0 );
    addOption( Isosurface::CommandName, Isosurface::Description, 0 );
    addOption( SlicePlane::CommandName, SlicePlane::Description, 0 );
    addOption( OrthoSlice::CommandName, OrthoSlice::Description, 0 );
    addOption( TransferFunction::CommandName, TransferFunction::Description, 0 );
    addOption( ExtractEdges::CommandName, ExtractEdges::Description, 0 );
    addOption( ExtractVertices::CommandName, ExtractVertices::Description, 0 );
    addOption( ExternalFaces::CommandName, ExternalFaces::Description, 0 );
    addOption( RayCastingRenderer::CommandName, RayCastingRenderer::Description, 0 );
    addOption( ParticleBasedRenderer::CommandName, ParticleBasedRenderer::Description, 0 );
    addOption( Histogram::CommandName, Histogram::Description, 0 );
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
    addHelpOption( "help" );
    addValue( "input data file" );
    addOption( "antialiasing",
               "Enable anti-aliasing. "
               "(optional)",
               0, false );
    addOption( "axis",
               "Show axis. "
               "(optional)",
               0, false );
    addOption( "axis_color",
               "Set axis color. "
               "(default: 0 0 0)",
               3, false );
    addOption( "axis_label_color",
               "Set axis label color. "
               "(default: 0 0 0)",
               3, false );
    addOption( "axis_line_width",
               "Set axis line width. "
               "(default: 2)",
               1, false );
    addOption( "axis_gridline_width",
               "Set axis gridline width. "
               "(default: 1)",
               1, false );
    addOption( "axis_x_label",
               "Set label for the x-axis. "
               "(default: X)",
               1, false );
    addOption( "axis_y_label",
               "Set label for the y-axis. "
               "(default: Y)",
               1, false );
    addOption( "axis_z_label",
               "Set label for the z-axis. "
               "(default: Z)",
               1, false );
    addOption( "axis_gridlines",
               "Set number of gridlines for each axis. "
               "(default: 5 5 5)",
               3, false );
    addOption( "background_color",
               "Set background color. "
               "(default: 212 221 229)",
               3, false );
    addOption( "background_color2",
               "Set top and bottom side color on the background. "
               "(optional: <top: r g b> <bottom: r g b>)",
               6, false );
    addOption( "bounds",
               "Show bounding box. "
               "(optional)",
               0, false );
    addOption( "bounds_color",
               "Set bounding box color. "
               "(default: 0 0 0)",
               3, false );
    addOption( "screen_position",
               "Set screen position. "
               "(default: 0 0)",
               2, false );
    addOption( "screen_size",
               "Set screen size. "
               "(default: 512 512)",
               2, false );
    addOption( "output",
               "Output filename. "
               "(optional: <filename>)",
               1, false );
    addOption( "verbose",
               "Enable additional output message. "
               "(optional)",
               0, false );
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
void Argument::Common::applyTo( kvs::Screen& screen )
{
    if ( this->hasOption("screen_position") )
    {
        auto p = this->valueAsVec2<int>( "screen_position" );
        screen.setPosition( p[0], p[1] );
    }

    if ( this->hasOption("screen_size") )
    {
        auto s = this->valueAsVec2<int>( "screen_size" );
        screen.setSize( s[0], s[1] );
    }
}

/*===========================================================================*/
/**
 *  @brief  Applys the specified options to the screen information.
 *  @param  screen [in] screen parameters
 *  @param  pipe [in] visualization pipeline
 */
/*===========================================================================*/
void Argument::Common::applyTo( kvs::Screen& screen, kvs::VisualizationPipeline& pipe )
{
    // Axis.
    if ( this->hasOption("axis") )
    {
        auto* axis = new kvs::Axis3D();
        axis->setGridDrawMode( kvs::Axis3D::FarGrid );
        axis->setAxisColor( this->valueAsRGBColor( "axis_color", kvs::UIColor::Label() ) );
        axis->setLabelColor( this->valueAsRGBColor( "axis_label_color", kvs::UIColor::Label() ) );
        axis->setValueColor( this->valueAsRGBColor( "axis_label_color", kvs::UIColor::Label() ) );
        axis->setAxisWidth( this->valueAs<kvs::Real32>( "axis_line_width", 3.0f ) );
        axis->setGridlineWidth( this->valueAs<kvs::Real32>( "axis_gridline_width", 1.0f ) );
        axis->setXLabel( this->valueAs<std::string>( "axis_x_label", "X" ) );
        axis->setYLabel( this->valueAs<std::string>( "axis_y_label", "Y" ) );
        axis->setZLabel( this->valueAs<std::string>( "axis_z_label", "Z" ) );
        axis->setNumberOfGridlines( this->valueAsVec3<kvs::UInt32>( "axis_gridlines", kvs::Vec3u( 5, 5, 5 ) ) );
        axis->setAntiAliasingEnabled( this->hasOption( "antialiasing" ) );

        auto* object = const_cast<kvs::ObjectBase*>( pipe.object() );
        screen.registerObject( object, axis );
    }

    // Bounding box.
    if ( this->hasOption("bounds") )
    {
        auto* bounds = new kvs::Bounds();
        bounds->setLineColor( this->valueAsRGBColor( "bounds_color", kvs::UIColor::Label() ) );
        if ( this->hasOption( "antialiasing" ) ) { bounds->enableAntiAliasing(); }

        auto* object = const_cast<kvs::ObjectBase*>( pipe.object() );
        screen.registerObject( object, bounds );
    }

    // Background color.
    auto* background = screen.scene()->background();
    if ( this->hasOption("background_color") )
    {
        auto c = this->valueAsRGBColor( "background_color" );
        background->setColor( c );
    }
    else if ( this->hasOption("background_color2") )
    {
        auto c1 = this->valueAsRGBColor( "background_color2" );
        auto c2 = this->valueAsRGBColor( "background_color2", 3 );
        background->setColor( c1, c2 );
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
            auto* object = kvs::GeometryObjectBase::DownCast( pipe.object() );
            switch ( object->geometryType() )
            {
            case kvs::GeometryObjectBase::Point:
            {
                auto* renderer = kvs::PointRenderer::DownCast( pipe.renderer() );
                renderer->enableAntiAliasing( multisample );
                break;
            }
            case kvs::GeometryObjectBase::Line:
            {
                auto* renderer = kvs::LineRenderer::DownCast( pipe.renderer() );
                renderer->enableAntiAliasing( multisample );
                break;
            }
            case kvs::GeometryObjectBase::Polygon:
            {
                auto* renderer = kvs::PolygonRenderer::DownCast( pipe.renderer() );
                renderer->enableAntiAliasing( multisample );
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
