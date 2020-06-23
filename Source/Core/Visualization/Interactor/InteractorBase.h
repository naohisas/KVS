/*****************************************************************************/
/**
 *  @file   InteractorBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventListener>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Interactor base class.
 */
/*===========================================================================*/
class InteractorBase : public kvs::EventListener
{
public:
    using BaseClass = kvs::EventListener;

public:
    InteractorBase();
    virtual ~InteractorBase() {}

protected:
    virtual void initializeEvent();
    virtual void paintEvent();
    virtual void resizeEvent( int w, int h );
    virtual void keyPressEvent( kvs::KeyEvent* e );
};

} // end of namespace kvs
