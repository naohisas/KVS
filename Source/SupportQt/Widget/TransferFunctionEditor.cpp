#include "TransferFunctionEditor.h"
#include <kvs/InitializeEventListener>


namespace kvs
{

namespace qt
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

    this->addEvent( new kvs::InitializeEventListener( [&]()
    {
        kvs::TransferFunctionEditorBase::layout();
    } ) );
}

void TransferFunctionEditor::show()
{
    kvs::qt::Screen::show();
}

} // end of namespace qt

} // end of namespace kvs
