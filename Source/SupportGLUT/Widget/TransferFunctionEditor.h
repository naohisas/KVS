/*****************************************************************************/
/**
 *  @file   TransferFunctionEditor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/TransferFunctionEditorBase>
#include <kvs/glut/Screen>


namespace kvs
{

namespace glut
{

class TransferFunctionEditor : public kvs::TransferFunctionEditorBase, public kvs::glut::Screen
{
public:
    TransferFunctionEditor( kvs::ScreenBase* parent = 0 );
    virtual ~TransferFunctionEditor() {}

    void show();

protected:
    void update() { this->redraw(); }
};

} // end of namespace glut

} // end of namespace kvs
