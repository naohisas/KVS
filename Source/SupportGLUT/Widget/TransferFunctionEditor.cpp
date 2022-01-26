/*****************************************************************************/
/**
 *  @file   TransferFunctionEditor.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "TransferFunctionEditor.h"
#include <kvs/RGBColor>
#include <kvs/EventBase>


namespace kvs
{

namespace glut
{

TransferFunctionEditor::TransferFunctionEditor( kvs::ScreenBase* parent ):
    kvs::TransferFunctionEditorBase( parent )
{
    const kvs::RGBColor base_color( 50, 50, 50 );
    this->setBackgroundColor( base_color );
    this->setup( this );

    auto* l = this->eventHandler()->listener( "DefaultInteractor" );
    l->disableEvent( kvs::EventBase::EventType::WheelEvent );
    l->disableEvent( kvs::EventBase::EventType::MouseMoveEvent );
    l->disableEvent( kvs::EventBase::EventType::TimerEvent );
}

void TransferFunctionEditor::show()
{
    kvs::glut::Screen::show();
    kvs::TransferFunctionEditorBase::layout();
}

} // end of namespace glut

} // end of namespace kvs
