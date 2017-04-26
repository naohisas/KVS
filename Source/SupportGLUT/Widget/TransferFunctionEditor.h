#pragma once
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
#include <kvs/PushButton>
#include <kvs/HistogramBar>
#include <kvs/ColorPalette>
#include <kvs/ColorMapPalette>
#include <kvs/OpacityMapPalette>
#include <kvs/glut/Screen>


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

private:
    kvs::ScreenBase* m_screen; ///< pointer to screen
    kvs::ColorPalette* m_color_palette; ///< color palette
    kvs::ColorMapPalette* m_color_map_palette; ///< color map palette
    kvs::OpacityMapPalette* m_opacity_map_palette; ///< opacity map palette
    kvs::HistogramBar* m_histogram; ///< histogram
    kvs::PushButton* m_reset_button; ///< reset button
    kvs::PushButton* m_undo_button; ///< undo button
    kvs::PushButton* m_redo_button; ///< redo button
    kvs::PushButton* m_save_button; ///< save button
    kvs::PushButton* m_apply_button; ///< apply button
    kvs::TransferFunction m_initial_transfer_function; ///< initial transfer function
    kvs::Real32 m_min_value; ///< min value
    kvs::Real32 m_max_value; ///< max value

public:
    TransferFunctionEditor( kvs::ScreenBase* parent = 0 );
    virtual ~TransferFunctionEditor();

    kvs::ScreenBase* screen() { return m_screen; }
    const kvs::ColorPalette* colorPalette() const { return m_color_palette; }
    const kvs::ColorMapPalette* colorMapPalette() const { return m_color_map_palette; }
    const kvs::OpacityMapPalette* opacityMapPalette() const { return m_opacity_map_palette; }
    const kvs::ColorMap colorMap() const { return m_color_map_palette->colorMap(); }
    const kvs::OpacityMap opacityMap() const { return m_opacity_map_palette->opacityMap(); }
    const kvs::TransferFunction transferFunction() const;
    size_t undoStackSize() const { return m_undo_stack.size(); }
    size_t redoStackSize() const { return m_redo_stack.size(); }
    size_t maxStackSize() const { return m_max_stack_size; }

    void setTransferFunction( const kvs::TransferFunction& transfer_function );
    void setVolumeObject( const kvs::VolumeObjectBase* object );
    void setMaxStackSize( const size_t stack_size ) { m_max_stack_size = stack_size; }

    virtual void reset();
    virtual void undo();
    virtual void redo();
    virtual void save();
    virtual void apply() {}

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
