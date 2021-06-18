/*****************************************************************************/
/**
 *  @file   TrackballInteractor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "InteractorBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Trackball interactor class.
 */
/*===========================================================================*/
class TrackballInteractor : public kvs::InteractorBase
{
public:
    using BaseClass = kvs::InteractorBase;

public:
    TrackballInteractor();
    virtual ~TrackballInteractor() {}

protected:
    virtual void mousePressEvent( kvs::MouseEvent* e );
    virtual void mouseMoveEvent( kvs::MouseEvent* e );
    virtual void mouseReleaseEvent( kvs::MouseEvent* e );
    virtual void wheelEvent( kvs::WheelEvent* e );
    virtual void keyPressEvent( kvs::KeyEvent* e );
    virtual void timerEvent( kvs::TimeEvent* e );
};

} // end of namespace kvs
