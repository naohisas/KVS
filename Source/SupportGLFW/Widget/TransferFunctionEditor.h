#pragma once
#include <kvs/TransferFunctionEditorBase>
#include <kvs/glfw/Screen>
#include <kvs/ApplicationBase>
#include <kvs/ScreenBase>


namespace kvs
{

namespace glfw
{

class TransferFunctionEditor : public kvs::TransferFunctionEditorBase, public kvs::glfw::Screen
{
public:
    TransferFunctionEditor( kvs::ScreenBase* parent = 0 );
    virtual ~TransferFunctionEditor() {}

    void show() { kvs::glfw::Screen::show(); this->redraw(); }

protected:
    void update() { this->redraw(); }
};

} // end of namespace glfw

} // end of namespace kvs
