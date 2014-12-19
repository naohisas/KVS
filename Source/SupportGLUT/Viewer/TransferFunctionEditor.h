/*****************************************************************************/
/**
 *  @file   TransferFunctionEditor.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TransferFunctionEditor.h 1724 2014-03-14 05:41:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__TRANSFER_FUNCTION_EDITOR_H_INCLUDE
#define KVS__GLUT__TRANSFER_FUNCTION_EDITOR_H_INCLUDE

#include <string>
#include <deque>
#include <kvs/OpenGL>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/Vector2>
#include <kvs/TransferFunction>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/MouseReleaseEventListener>
#include <kvs/glut/Screen>
#include <kvs/glut/PushButton>
#include <kvs/glut/Text>
#include <kvs/glut/ColorPalette>
#include <kvs/glut/ColorMapPalette>
#include <kvs/glut/OpacityMapPalette>
#include <kvs/glut/Histogram>


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Transfer function editor class.
 */
/*===========================================================================*/
class TransferFunctionEditor : public kvs::glut::Screen
{
public:

    typedef kvs::glut::Screen SuperClass;

private:

    class StackEvent;

    std::deque<kvs::TransferFunction> m_undo_stack; ///< undo stack
    std::deque<kvs::TransferFunction> m_redo_stack; ///< redo stack
    StackEvent* m_stack_event; ///< stack event
    size_t m_max_stack_size; ///< max. stack size

protected:

    kvs::ScreenBase* m_screen; ///< pointer to screen
    kvs::glut::ColorPalette* m_color_palette; ///< color palette
    kvs::glut::ColorMapPalette* m_color_map_palette; ///< color map palette
    kvs::glut::OpacityMapPalette* m_opacity_map_palette; ///< opacity map palette
    kvs::glut::Histogram* m_histogram; ///< histogram
    kvs::glut::PushButton* m_reset_button; ///< reset button
    kvs::glut::PushButton* m_undo_button; ///< undo button
    kvs::glut::PushButton* m_redo_button; ///< redo button
    kvs::glut::PushButton* m_save_button; ///< save button
    kvs::glut::PushButton* m_apply_button; ///< apply button
    kvs::TransferFunction m_initial_transfer_function; ///< initial transfer function
    kvs::Real32 m_min_value; ///< min value
    kvs::Real32 m_max_value; ///< max value

public:

    TransferFunctionEditor( kvs::ScreenBase* parent = 0 );
    virtual ~TransferFunctionEditor();

    kvs::ScreenBase* screen();
    const kvs::glut::ColorPalette* colorPalette() const;
    const kvs::glut::ColorMapPalette* colorMapPalette() const;
    const kvs::glut::OpacityMapPalette* opacityMapPalette() const;
    const kvs::ColorMap colorMap() const;
    const kvs::OpacityMap opacityMap() const;
    const kvs::TransferFunction transferFunction() const;
    size_t undoStackSize() const;
    size_t redoStackSize() const;
    size_t maxStackSize() const;

    void setTransferFunction( const kvs::TransferFunction& transfer_function );
    void setVolumeObject( const kvs::VolumeObjectBase* object );
    void setMaxStackSize( const size_t stack_size );

    virtual void reset();
    virtual void undo();
    virtual void redo();
    virtual void save();
    virtual void apply();

protected:

    virtual void initializeEvent() { this->reset(); }
};

/*===========================================================================*/
/**
 *  @brief  Stack event for undo/redo.
 */
/*===========================================================================*/
class TransferFunctionEditor::StackEvent : public kvs::MouseReleaseEventListener
{
    kvs::glut::TransferFunctionEditor* m_editor;

public:

    StackEvent( kvs::glut::TransferFunctionEditor* editor );

    void update( kvs::MouseEvent* event );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__TRANSFER_FUNCTION_EDITOR_H_INCLUDE
