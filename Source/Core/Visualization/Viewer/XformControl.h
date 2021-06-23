/****************************************************************************/
/**
 *  @file   XformControl.h
 *  @author Naohisa Sakamoto
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
    kvs::Xform m_X_crr{}; ///< current xform
    kvs::Xform m_X_ini{}; ///< initial xform

public:
    XformControl() = default;

    void saveXform() { m_X_ini = m_X_crr; }
    void resetXform() { m_X_crr = m_X_ini; }
    void setXform( const kvs::Xform& X ) { m_X_crr = X; }
    const kvs::Xform xform() const { return m_X_crr; }
    void multiplyXform( const kvs::Xform& X ) { this->setXform( X * this->xform() ); }

    void rotate( const kvs::Mat3& R );
    void translate( const kvs::Vec3& translation );
    void scale( const kvs::Vec3& scaling );

protected:
    ~XformControl() {}
};

} // end of namespace kvs
