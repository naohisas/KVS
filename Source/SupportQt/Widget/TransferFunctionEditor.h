#pragma once
#include "../Qt.h"
#include "../Viewer/Screen.h"
#include <kvs/TransferFunctionEditorBase>
#include <kvs/ApplicationBase>
#include <kvs/ScreenBase>


namespace kvs
{

namespace qt
{

class TransferFunctionEditor : public kvs::TransferFunctionEditorBase, public kvs::qt::Screen
{
public:
    TransferFunctionEditor( kvs::ScreenBase* parent = 0 );
    virtual ~TransferFunctionEditor() = default;

    void show();

protected:
    void update() { this->redraw(); }
};

} // end fo namespace qt

} // end of namespace kvs
