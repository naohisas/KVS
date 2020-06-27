#include "TransferFunctionEditor.h"
#include <kvs/RGBColor>
#include <kvs/EventBase>


namespace kvs
{

namespace glfw
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

} // end of namespace glfw

} // end of namespace kvs
