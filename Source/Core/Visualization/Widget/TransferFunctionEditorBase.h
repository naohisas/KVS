/*****************************************************************************/
/**
 *  @file   TransferFunctionEditorBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <deque>
#include <functional>
#include <kvs/TransferFunction>
#include <kvs/MouseReleaseEventListener>
#include <kvs/PushButton>
#include <kvs/HistogramBar>
#include <kvs/ColorPalette>
#include <kvs/ColorMapPalette>
#include <kvs/OpacityMapPalette>


namespace kvs
{

class TransferFunctionEditorBase
{
public:
    using ApplyFunc = std::function<void(kvs::TransferFunction)>;

private:
    class StackEvent;
    std::deque<kvs::TransferFunction> m_undo_stack; ///< undo stack
    std::deque<kvs::TransferFunction> m_redo_stack; ///< redo stack
    StackEvent* m_stack_event; ///< stack event
    size_t m_max_stack_size; ///< max. stack size

private:
    kvs::ScreenBase* m_parent = nullptr; ///< pointer to screen
    kvs::ColorPalette* m_color_palette = nullptr; ///< color palette
    kvs::ColorMapPalette* m_color_map_palette = nullptr; ///< color map palette
    kvs::OpacityMapPalette* m_opacity_map_palette = nullptr; ///< opacity map palette
    kvs::HistogramBar* m_histogram = nullptr; ///< histogram
    kvs::PushButton* m_reset_button = nullptr; ///< reset button
    kvs::PushButton* m_undo_button = nullptr; ///< undo button
    kvs::PushButton* m_redo_button = nullptr; ///< redo button
    kvs::PushButton* m_save_button = nullptr; ///< save button
    kvs::PushButton* m_apply_button = nullptr; ///< apply button
    kvs::TransferFunction m_initial_transfer_function{}; ///< initial transfer function
    kvs::Real32 m_min_value = 0.0f; ///< min value
    kvs::Real32 m_max_value = 0.0f; ///< max value
    ApplyFunc m_apply_func = nullptr;
    int m_width = 350;
    int m_height = 512;
    int m_margin = 10;

public:
    TransferFunctionEditorBase( kvs::ScreenBase* parent = 0 ): m_parent( parent ) {}
    virtual ~TransferFunctionEditorBase();

    kvs::ScreenBase* screen() { return m_parent; }
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

    void apply( ApplyFunc func ) { m_apply_func = func; }

    virtual void reset();
    virtual void undo();
    virtual void redo();
    virtual void save();
    virtual void apply() { if ( m_apply_func ) m_apply_func( transferFunction() ); }

protected:
    void setup( kvs::ScreenBase* screen );
    void layout();
    virtual void update() = 0; // screen->redraw()
};

/*===========================================================================*/
/**
 *  @brief  Stack event for undo/redo.
 */
/*===========================================================================*/
class TransferFunctionEditorBase::StackEvent : public kvs::MouseReleaseEventListener
{
    kvs::TransferFunctionEditorBase* m_editor = nullptr;

public:
    StackEvent( kvs::TransferFunctionEditorBase* editor ): m_editor( editor ) {}
    void update( kvs::MouseEvent* event );
};

} // end of namespace kvs
