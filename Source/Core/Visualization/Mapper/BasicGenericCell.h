/*****************************************************************************/
/**
 *  @file   BasicGenericCell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BasicGenericCell.h 1761 2014-05-07 04:18:15Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/Exception>


namespace kvs
{

namespace cell
{

template <typename T>
class BasicGenericCell
{
public:
    typedef T ValueType;

private:
    struct CellFunctionCallerBase
    {
        virtual ~CellFunctionCallerBase() {}
        virtual void bindCell( kvs::UInt32 index ) = 0;
        virtual const kvs::Vector3<T> centerOfLocal() = 0;
        virtual void adjustPoint( kvs::Vector3<T>& point ) = 0;
        virtual bool includes( const kvs::Vector3<T>& global ) = 0;
        virtual bool includesLocal( const kvs::Vector3<T>& local ) = 0;
        virtual int numberOfNodes() = 0;
        virtual int valueDimension() = 0;
        virtual T volume() = 0;
        virtual const kvs::Vector3<T> globalToLocal( const kvs::Vector3<T>& global ) = 0;
        virtual const kvs::Vector3<T> globalPoint() = 0;
        virtual const kvs::Vector3<T> gradient() = 0;
        virtual const kvs::Matrix33<T> jacobianMatrix() = 0;
        virtual T value( int index ) = 0;
        virtual T scalarValue() = 0;
        virtual void vectorValue( T* output ) = 0;
        virtual T nodeValue( int node_index, int value_index ) = 0;
        virtual T nodeScalarValue( int node_index ) = 0;
        virtual void nodeVectorValue( int node_index, T* output ) = 0;
        virtual void setLocalPoint( const kvs::Vector3<T>& local ) = 0;
        virtual void updateInterpolationFactors( const kvs::Vector3<T>& local ) = 0;
        virtual void updateDifferentialFactors( const kvs::Vector3<T>& local ) = 0;
    };

    template <typename CellType>
    struct CellFunctionCaller : public CellFunctionCallerBase
    {
        CellType* m_cell;

        CellFunctionCaller( const kvs::VolumeObjectBase* volume )
        {
            m_cell = detail::CellGenerator<CellType>::Generate( volume );
        }

        ~CellFunctionCaller()
        {
            detail::CellGenerator<CellType>::Dispose( m_cell );
        }

        void bindCell( kvs::UInt32 index )
        {
            m_cell->bindCell( index );
        }

        const kvs::Vector3<T> centerOfLocal()
        {
            return m_cell->centerOfLocal();
        }

        void adjustPoint( kvs::Vector3<T>& point )
        {
            m_cell->adjustPoint( point );
        }

        bool includes( const kvs::Vector3<T>& global )
        {
            return m_cell->includes( global );
        }

        bool includesLocal( const kvs::Vector3<T>& local )
        {
            return m_cell->includesLocal( local );
        }

        int numberOfNodes()
        {
            return m_cell->numberOfNodes();
        }

        int valueDimension()
        {
            return m_cell->valueDimension();
        }

        T volume()
        {
            return m_cell->volume();
        }

        const kvs::Vector3<T> globalToLocal( const kvs::Vector3<T>& global )
        {
            return m_cell->globalToLocal( global );
        }

        const kvs::Vector3<T> globalPoint()
        {
            return m_cell->globalPoint();
        }

        const kvs::Vector3<T> gradient()
        {
            return m_cell->gradient();
        }

        const kvs::Matrix33<T> jacobianMatrix()
        {
            return m_cell->jacobianMatrix();
        }

        T value( int index )
        {
            return m_cell->value( index );
        }

        T scalarValue()
        {
            return m_cell->scalarValue();
        }

        void vectorValue( T* output )
        {
            m_cell->vectorValue( output );
        }

        T nodeValue( int node_index, int value_index )
        {
            return m_cell->nodeValue( node_index, value_index );
        }

        T nodeScalarValue( int node_index )
        {
            return m_cell->nodeScalarValue( node_index );
        }

        void nodeVectorValue( int node_index, T* output )
        {
            m_cell->nodeVectorValue( node_index, output );
        }

        void setLocalPoint( const kvs::Vector3<T>& local )
        {
            m_cell->setLocalPoint( local );
        }

        void updateInterpolationFactors( const kvs::Vector3<T>& local )
        {
            m_cell->updateInterpolationFactors( local );
        }

        void updateDifferentialFactors( const kvs::Vector3<T>& local )
        {
            m_cell->updateDifferentialFactors( local );
        }
    };

private:
    CellFunctionCallerBase* m_cell;

public:
    BasicGenericCell( const kvs::VolumeObjectBase* object )
    {
        switch ( object->volumeType() )
        {
        case kvs::VolumeObjectBase::Structured:
        {
            typedef kvs::StructuredVolumeObject Volume;
            const Volume* volume = Volume::DownCast( object );
            switch ( volume->gridType() )
            {
            case kvs::StructuredVolumeObject::Uniform:
                m_cell = new CellFunctionCaller<BasicCubicCell<T> >( volume );
                break;
            default: KVS_THROW( kvs::NotSupportedException, "Not supported grid type" );
            }
            break;
        }
        case kvs::VolumeObjectBase::Unstructured:
        {
            typedef kvs::UnstructuredVolumeObject Volume;
            const Volume* volume = Volume::DownCast( object );
            switch ( volume->cellType() )
            {
            case kvs::UnstructuredVolumeObject::Tetrahedra:
                m_cell = new CellFunctionCaller<BasicTetrahedralCell<T> >( volume );
                break;
            case kvs::UnstructuredVolumeObject::Hexahedra:
                m_cell = new CellFunctionCaller<BasicHexahedralCell<T> >( volume );
                break;
            case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
                m_cell = new CellFunctionCaller<BasicQuadraticTetrahedralCell<T> >( volume );
                break;
            case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
                m_cell = new CellFunctionCaller<BasicQuadraticHexahedralCell<T> >( volume );
                break;
            case kvs::UnstructuredVolumeObject::Pyramid:
                m_cell = new CellFunctionCaller<BasicPyramidalCell<T> >( volume );
                break;
            default: KVS_THROW( kvs::NotSupportedException, "Not supported cell type" );
            }
            break;
        }
        default: KVS_THROW( kvs::NotSupportedException, "Not supported volume type" );
        }
    }

    ~BasicGenericCell()
    {
        delete m_cell;
    }

public:
    void bindCell( kvs::UInt32 index )
    {
        m_cell->bindCell( index );
    }

    const kvs::Vector3<T> centerOfLocal()
    {
        return m_cell->centerOfLocal();
    }

    void adjustPoint( kvs::Vector3<T>& point )
    {
        m_cell->adjustPoint( point );
    }

    bool includes( const kvs::Vector3<T>& global )
    {
        return m_cell->includes( global );
    }

    bool includesLocal( const kvs::Vector3<T>& local )
    {
        return m_cell->includesLocal( local );
    }

    int numberOfNodes()
    {
        return m_cell->numberOfNodes();
    }

    int valueDimension()
    {
        return m_cell->valueDimension();
    }

    T volume()
    {
        return m_cell->volume();
    }

    const kvs::Vector3<T> globalToLocal( const kvs::Vector3<T>& global )
    {
        return m_cell->globalToLocal( global );
    }

    const kvs::Vector3<T> globalPoint()
    {
        return m_cell->globalPoint();
    }

    const kvs::Vector3<T> gradient()
    {
        return m_cell->gradient();
    }

    const kvs::Matrix33<T> jacobianMatrix()
    {
        return m_cell->jacobianMatrix();
    }

    T value( int index )
    {
        return m_cell->value( index );
    }

    T scalarValue()
    {
        return m_cell->scalarValue();
    }

    void vectorValue( T* output )
    {
        m_cell->vectorValue( output );
    }

    T nodeValue( int node_index, int value_index )
    {
        return m_cell->nodeValue( node_index, value_index );
    }

    T nodeScalarValue( int node_index )
    {
        return m_cell->nodeScalarValue( node_index );
    }

    void nodeVectorValue( int node_index, T* output )
    {
        m_cell->nodeVectorValue( node_index, output );
    }

    void setLocalPoint( const kvs::Vector3<T>& local )
    {
        m_cell->setLocalPoint( local );
    }

    void updateInterpolationFactors( const kvs::Vector3<T>& local )
    {
        m_cell->updateInterpolationFactors( local );
    }

    void updateDifferentialFactors( const kvs::Vector3<T>& local )
    {
        m_cell->updateDifferentialFactors( local );
    }
};

namespace detail
{

template <typename T>
struct CellGenerator<BasicGenericCell<T> >
{
    typedef BasicGenericCell<T> CellType;

    static CellType* Generate( const kvs::VolumeObjectBase* volume )
    {
        return new CellType( volume );
    }

    static void Dispose( CellType* cell )
    {
        delete cell;
    }
};

} // end of namespace detail

} // end of namespace cell

} // end of namespace kvs
