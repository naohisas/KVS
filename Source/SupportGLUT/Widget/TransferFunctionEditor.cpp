/*****************************************************************************/
/**
 *  @file   TransferFunctionEditor.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TransferFunctionEditor.cpp 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "TransferFunctionEditor.h"
#include <kvs/DebugNew>
#include <kvs/MouseEvent>
#include <kvs/Math>
#include <kvs/HSVColor>
#include <kvs/RGBColor>
#include <kvs/Date>
#include <kvs/Time>
#include <kvs/glut/GLUT>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Background>
#include <SupportGLUT/Viewer/KVSKey.h>
#include <SupportGLUT/Viewer/KVSMouseButton.h>


namespace
{

class ResetButton : public kvs::glut::PushButton
{
    kvs::glut::TransferFunctionEditor* m_editor;

public:

    ResetButton( kvs::glut::TransferFunctionEditor* editor ):
        kvs::glut::PushButton( editor ),
        m_editor( editor )
    {
    }

    void released()
    {
        m_editor->reset();
    }
};

class ApplyButton : public kvs::glut::PushButton
{
    kvs::glut::TransferFunctionEditor* m_editor;

public:

    ApplyButton( kvs::glut::TransferFunctionEditor* editor ):
        kvs::glut::PushButton( editor ),
        m_editor( editor )
    {
    }

    void released()
    {
        m_editor->apply();
    }
};

class SaveButton : public kvs::glut::PushButton
{
    kvs::glut::TransferFunctionEditor* m_editor;

public:

    SaveButton( kvs::glut::TransferFunctionEditor* editor ):
        kvs::glut::PushButton( editor ),
        m_editor( editor )
    {
    }

    void released()
    {
        m_editor->save();
    }
};

class UndoButton : public kvs::glut::PushButton
{
    kvs::glut::TransferFunctionEditor* m_editor;

public:

    UndoButton( kvs::glut::TransferFunctionEditor* editor ):
        kvs::glut::PushButton( editor ),
        m_editor( editor )
    {
    }

    void released()
    {
        m_editor->undo();
    }
};

class RedoButton : public kvs::glut::PushButton
{
    kvs::glut::TransferFunctionEditor* m_editor;

public:

    RedoButton( kvs::glut::TransferFunctionEditor* editor ):
        kvs::glut::PushButton( editor ),
        m_editor( editor )
    {
    }

    void released()
    {
        m_editor->redo();
    }
};

} // end of namespace


namespace kvs
{

namespace glut
{

TransferFunctionEditor::TransferFunctionEditor( kvs::ScreenBase* parent ):
    m_screen( parent ),
    m_color_palette( NULL ),
    m_color_map_palette( NULL ),
    m_opacity_map_palette( NULL ),
    m_histogram( NULL ),
    m_reset_button( NULL ),
    m_undo_button( NULL ),
    m_redo_button( NULL ),
    m_save_button( NULL ),
    m_apply_button( NULL ),
    m_min_value( 0.0f ),
    m_max_value( 0.0f )
{
    const std::string title = "Transfer Function Editor";
    const int x = ( parent != 0 ) ? parent->x() + parent->width() + 5 : 0;
    const int y = ( parent != 0 ) ? parent->y() : 0;
    const int width = 350;
    const int height = 512;
    const int margin = 10;
    const kvs::RGBColor color( 200, 200, 200 );

    SuperClass::scene()->background()->setColor( color );
    SuperClass::setTitle( title );
    SuperClass::setPosition( x, y );
    SuperClass::setSize( width, height );

    const size_t resolution = 256;
    m_initial_transfer_function.create( resolution );

    m_undo_stack.push_front( m_initial_transfer_function );

    m_stack_event = new StackEvent( this );
    SuperClass::addEvent( m_stack_event );

    m_max_stack_size = 10;

    m_color_palette = new kvs::glut::ColorPalette( this );
    m_color_palette->setCaption( "Color palette" );
    m_color_palette->show();

    m_color_map_palette = new kvs::glut::ColorMapPalette( this );
    m_color_map_palette->setCaption( "Color" );
    m_color_map_palette->setColorMap( m_initial_transfer_function.colorMap() );
    m_color_map_palette->setX( m_color_palette->x0() );
    m_color_map_palette->setY( m_color_palette->y1() - m_color_palette->margin() );
    m_color_map_palette->attachColorPalette( m_color_palette );
    m_color_map_palette->show();

    m_opacity_map_palette = new kvs::glut::OpacityMapPalette( this );
    m_opacity_map_palette->setCaption( "Opacity" );
    m_opacity_map_palette->setOpacityMap( m_initial_transfer_function.opacityMap() );
    m_opacity_map_palette->setX( m_color_map_palette->x0() );
    m_opacity_map_palette->setY( m_color_map_palette->y1() - m_color_map_palette->margin() );
    m_opacity_map_palette->setHeight( 100 );
    m_opacity_map_palette->show();

    m_histogram = new kvs::glut::Histogram( this );
    m_histogram->setCaption( "Histogram" );
    m_histogram->setX( m_opacity_map_palette->x0() );
    m_histogram->setY( m_opacity_map_palette->y1() - m_opacity_map_palette->margin() );
    m_histogram->setHeight( 100 );
    m_histogram->setGraphColor( kvs::RGBAColor( 100, 100, 100, 1.0f ) );
    m_histogram->show();

    const size_t button_margin = 5;
    const size_t button_width = ( width - 2 * margin - button_margin ) / 2;

    m_reset_button = new ::ResetButton( this );
    m_reset_button->setCaption( "Reset" );
    m_reset_button->setX( m_histogram->x0() + m_histogram->margin() );
    m_reset_button->setY( m_histogram->y1() );
    m_reset_button->setWidth( button_width );
    m_reset_button->show();

    m_undo_button = new ::UndoButton( this );
    m_undo_button->setCaption( "Undo" );
    m_undo_button->setX( m_reset_button->x1() + button_margin );
    m_undo_button->setY( m_reset_button->y() );
    m_undo_button->setWidth( ( button_width - button_margin ) / 2 );
    m_undo_button->show();

    m_redo_button = new ::RedoButton( this );
    m_redo_button->setCaption( "Redo" );
    m_redo_button->setX( m_undo_button->x1() + button_margin );
    m_redo_button->setY( m_undo_button->y() );
    m_redo_button->setWidth( ( button_width - button_margin ) / 2 );
    m_redo_button->show();

    m_save_button = new ::SaveButton( this );
    m_save_button->setCaption( "Save" );
    m_save_button->setX( m_reset_button->x0() );
    m_save_button->setY( m_reset_button->y1() + button_margin );
    m_save_button->setWidth( button_width );
    m_save_button->show();

    m_apply_button = new ::ApplyButton( this );
    m_apply_button->setCaption( "Apply" );
    m_apply_button->setX( m_save_button->x1() + button_margin );
    m_apply_button->setY( m_save_button->y0() );
    m_apply_button->setWidth( ( width -margin ) / 2 - m_opacity_map_palette->margin() );
    m_apply_button->show();
}

TransferFunctionEditor::~TransferFunctionEditor()
{
    if ( m_stack_event ) delete m_stack_event;
    if ( m_color_palette ) delete m_color_palette;
    if ( m_color_map_palette ) delete m_color_map_palette;
    if ( m_opacity_map_palette ) delete m_opacity_map_palette;
    if ( m_histogram ) delete m_histogram;
    if ( m_reset_button ) delete m_reset_button;
    if ( m_undo_button ) delete m_undo_button;
    if ( m_redo_button ) delete m_redo_button;
    if ( m_apply_button ) delete m_apply_button;
    if ( m_save_button ) delete m_save_button;
}

kvs::ScreenBase* TransferFunctionEditor::screen()
{
    return( m_screen );
}

const kvs::glut::ColorPalette* TransferFunctionEditor::colorPalette() const
{
    return( m_color_palette );
}

const kvs::glut::ColorMapPalette* TransferFunctionEditor::colorMapPalette() const
{
    return( m_color_map_palette );
}

const kvs::glut::OpacityMapPalette* TransferFunctionEditor::opacityMapPalette() const
{
    return( m_opacity_map_palette );
}

const kvs::ColorMap TransferFunctionEditor::colorMap() const
{
    return( m_color_map_palette->colorMap() );
}

const kvs::OpacityMap TransferFunctionEditor::opacityMap() const
{
    return( m_opacity_map_palette->opacityMap() );
}

const kvs::TransferFunction TransferFunctionEditor::transferFunction() const
{
//    const kvs::Real32 min_value = m_initial_transfer_function.colorMap().minValue();
//    const kvs::Real32 max_value = m_initial_transfer_function.colorMap().maxValue();
    kvs::TransferFunction transfer_function( this->colorMap(), this->opacityMap() );
//    transfer_function.setRange( min_value, max_value );
    transfer_function.setRange( m_min_value, m_max_value );

    return( transfer_function );
}

size_t TransferFunctionEditor::undoStackSize() const
{
    return( m_undo_stack.size() );
}

size_t TransferFunctionEditor::redoStackSize() const
{
    return( m_redo_stack.size() );
}

size_t TransferFunctionEditor::maxStackSize() const
{
    return( m_max_stack_size );
}

void TransferFunctionEditor::setTransferFunction( const kvs::TransferFunction& transfer_function )
{
    const kvs::ColorMap& cmap = transfer_function.colorMap();
    const kvs::OpacityMap& omap = transfer_function.opacityMap();

    // Deep copy for the initial transfer function.
    kvs::ColorMap::Table color_map_table( cmap.table().data(), cmap.table().size() );
    kvs::OpacityMap::Table opacity_map_table( omap.table().data(), omap.table().size() );
//    kvs::ColorMap color_map( color_map_table, cmap.minValue(), cmap.maxValue() );
//    kvs::OpacityMap opacity_map( opacity_map_table, omap.minValue(), omap.maxValue() );
    kvs::ColorMap color_map( color_map_table );
    kvs::OpacityMap opacity_map( opacity_map_table );
    m_initial_transfer_function.setColorMap( color_map );
    m_initial_transfer_function.setOpacityMap( opacity_map );

    if ( transfer_function.hasRange() )
    {
        m_min_value = transfer_function.colorMap().minValue();
        m_max_value = transfer_function.colorMap().maxValue();
        m_initial_transfer_function.setRange( m_min_value, m_max_value );
    }

    m_color_map_palette->setColorMap( color_map );
    m_opacity_map_palette->setOpacityMap( opacity_map );

    m_undo_stack.clear();
    m_undo_stack.push_front( m_initial_transfer_function );
}

void TransferFunctionEditor::setVolumeObject( const kvs::VolumeObjectBase* object )
{
    if ( !m_initial_transfer_function.hasRange() )
    {
        m_min_value = object->minValue();
        m_max_value = object->maxValue();
    }

    m_histogram->create( object );
}

void TransferFunctionEditor::setMaxStackSize( const size_t stack_size )
{
    m_max_stack_size = stack_size;
}

void TransferFunctionEditor::reset()
{
    m_color_map_palette->setColorMap( m_initial_transfer_function.colorMap() );
    m_opacity_map_palette->setOpacityMap( m_initial_transfer_function.opacityMap() );

    m_color_map_palette->update();
    m_opacity_map_palette->update();
    this->redraw();
}

void TransferFunctionEditor::apply()
{
}

void TransferFunctionEditor::save()
{
    const std::string date = kvs::Date().toString("");
    const std::string time = kvs::Time().toString("");
    const std::string filename = "tfunc_" + date + "_" + time + ".kvsml";

    kvs::TransferFunction transfer_function = this->transferFunction();
    transfer_function.write( filename );
}

void TransferFunctionEditor::undo()
{
    if ( m_undo_stack.size() > 1 )
    {
        if ( m_redo_stack.size() > m_max_stack_size ) m_redo_stack.pop_back();
        m_redo_stack.push_front( m_undo_stack.front() );
        m_undo_stack.pop_front();

        kvs::TransferFunction& transfer_function = m_undo_stack.front();
        m_color_map_palette->setColorMap( transfer_function.colorMap() );
        m_opacity_map_palette->setOpacityMap( transfer_function.opacityMap() );

        m_color_map_palette->update();
        m_opacity_map_palette->update();
        this->redraw();
    }
}

void TransferFunctionEditor::redo()
{
    if ( m_redo_stack.size() > 1 )
    {
        kvs::TransferFunction& transfer_function = m_redo_stack.front();
        m_color_map_palette->setColorMap( transfer_function.colorMap() );
        m_opacity_map_palette->setOpacityMap( transfer_function.opacityMap() );

        if ( m_undo_stack.size() > m_max_stack_size ) m_undo_stack.pop_back();
        m_undo_stack.push_front( m_redo_stack.front() );
        m_redo_stack.pop_front();

        m_color_map_palette->update();
        m_opacity_map_palette->update();
        this->redraw();
    }
}

TransferFunctionEditor::StackEvent::StackEvent(
    kvs::glut::TransferFunctionEditor* editor ):
    m_editor( editor )
{
}

void TransferFunctionEditor::StackEvent::update( kvs::MouseEvent* event )
{
    if ( m_editor->opacityMapPalette()->palette().isActive() ||
         m_editor->colorMapPalette()->palette().isActive() )
    {
        if ( m_editor->m_undo_stack.size() > m_editor->m_max_stack_size ) m_editor->m_undo_stack.pop_back();
        m_editor->m_undo_stack.push_front( m_editor->transferFunction() );
        m_editor->redraw();
    }
}

} // end of namespace glut

} // end of namespace kvs
