#pragma once
#include <kvs/TransferFunctionEditorBase>
#include <kvs/glfw/Screen>


namespace kvs
{

namespace glfw
{

class TransferFunctionEditor : public kvs::TransferFunctionEditorBase, public kvs::glfw::Screen
{
public:
    TransferFunctionEditor( kvs::ScreenBase* parent = 0 );
    virtual ~TransferFunctionEditor() {}

protected:
    void update() { this->redraw(); }
    void initializeEvent() { kvs::glfw::Screen::reset(); }
};

} // end of namespace glfw

} // end of namespace kvs
