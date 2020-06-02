/****************************************************************************/
/**
 *  @file XformControl.h
 */
/****************************************************************************/
#pragma once
#include <kvs/Xform>


namespace kvs
{

/*==========================================================================*/
/**
 *  Xform control class.
 */
/*==========================================================================*/
class XformControl
{
private:
    kvs::Xform m_current_xform; ///< current xform
    kvs::Xform m_initial_xform; ///< initial xform

public:
    XformControl(){}

    void saveXform();
    void resetXform();
    void setXform( const kvs::Xform& xform );
    void multiplyXform( const kvs::Xform& xform );
    const kvs::Xform xform() const;

    void rotate( const kvs::Mat3& rotation );
    void translate( const kvs::Vec3& translation );
    void scale( const kvs::Vec3& scaling );

protected:
    ~XformControl(){}
};

} // end of namespace kvs
