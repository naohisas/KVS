/****************************************************************************/
/**
 *  @file   CellByCellRejectionSampling.cpp
 *  @author Takum Kawamura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CellByCellRejectionSampling.cpp 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "CellByCellRejectionSampling.h"
#include <vector>
#include <kvs/DebugNew>
#include <kvs/Camera>
#include <kvs/TrilinearInterpolator>
#include <kvs/Value>
#include <kvs/CellBase>
#include <kvs/TetrahedralCell>
#include <kvs/QuadraticTetrahedralCell>
#include <kvs/HexahedralCell>
#include <kvs/QuadraticHexahedralCell>
#include <kvs/PyramidalCell>
#include <kvs/PrismaticCell>


namespace Generator = kvs::CellByCellParticleGenerator;


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellRejectionSampling class.
 */
/*===========================================================================*/
CellByCellRejectionSampling::CellByCellRejectionSampling():
    kvs::MapperBase(),
    kvs::PointObject(),
    m_camera( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellRejectionSampling class.
 *  @param  volume [in] pointer to the volume object
 *  @param  subpixel_level [in] sub-pixel level
 *  @param  sampling_step [in] sapling step
 *  @param  transfer_function [in] transfer function
 *  @param  object_depth [in] depth value of the input volume at the CoG
 */
/*===========================================================================*/
CellByCellRejectionSampling::CellByCellRejectionSampling(
    const kvs::VolumeObjectBase* volume,
    const size_t                 subpixel_level,
    const float                  sampling_step,
    const kvs::TransferFunction& transfer_function,
    const float                  object_depth ):
    kvs::MapperBase( transfer_function ),
    kvs::PointObject(),
    m_camera( 0 )
{
    this->setSubpixelLevel( subpixel_level );
    this->setSamplingStep( sampling_step );
    this->setObjectDepth( object_depth );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellRejectionSampling class.
 *  @param  camera [in] pointer to the camera
 *  @param  volume [in] pointer to the volume object
 *  @param  subpixel_level [in] sub-pixel level
 *  @param  sampling_step [in] sapling step
 *  @param  transfer_function [in] transfer function
 *  @param  object_depth [in] depth value of the input volume at the CoG
 */
/*===========================================================================*/
CellByCellRejectionSampling::CellByCellRejectionSampling(
    const kvs::Camera*           camera,
    const kvs::VolumeObjectBase* volume,
    const size_t                 subpixel_level,
    const float                  sampling_step,
    const kvs::TransferFunction& transfer_function,
    const float                  object_depth ):
    kvs::MapperBase( transfer_function ),
    kvs::PointObject()
{
    this->attachCamera( camera ),
    this->setSubpixelLevel( subpixel_level );
    this->setSamplingStep( sampling_step );
    this->setObjectDepth( object_depth );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the CellByCellMetropolisSampling class.
 */
/*===========================================================================*/
CellByCellRejectionSampling::~CellByCellRejectionSampling()
{
    m_density_map.release();
}

/*===========================================================================*/
/**
 *  @brief  Retruns the sub-pixel level.
 *  @return sub-pixel level
 */
/*===========================================================================*/
size_t CellByCellRejectionSampling::subpixelLevel() const
{
    return m_subpixel_level;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampling step.
 *  @return sampling step
 */
/*===========================================================================*/
float CellByCellRejectionSampling::samplingStep() const
{
    return m_sampling_step;
}

/*===========================================================================*/
/**
 *  @brief  Returns the depth of the object at the center of the gravity.
 *  @return depth
 */
/*===========================================================================*/
float CellByCellRejectionSampling::objectDepth() const
{
    return m_object_depth;
}

/*===========================================================================*/
/**
 *  @brief  Attaches a camera.
 *  @param  camera [in] pointer to the camera
 */
/*===========================================================================*/
void CellByCellRejectionSampling::attachCamera( const kvs::Camera* camera )
{
    m_camera = camera;
}

/*===========================================================================*/
/**
 *  @brief  Sets a sub-pixel level.
 *  @param  subpixel_level [in] sub-pixel level
 */
/*===========================================================================*/
void CellByCellRejectionSampling::setSubpixelLevel( const size_t subpixel_level )
{
    m_subpixel_level = subpixel_level;
}

/*===========================================================================*/
/**
 *  @brief  Sets a sampling step.
 *  @param  sampling_step [in] sampling step
 */
/*===========================================================================*/
void CellByCellRejectionSampling::setSamplingStep( const float sampling_step )
{
    m_sampling_step = sampling_step;
}

/*===========================================================================*/
/**
 *  @brief  Sets a depth of the object at the center of the gravity.
 *  @param  object_depth [in] depth
 */
/*===========================================================================*/
void CellByCellRejectionSampling::setObjectDepth( const float object_depth )
{
    m_object_depth = object_depth;
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the volume object
 *  @return pointer to the point object
 */
/*===========================================================================*/
CellByCellRejectionSampling::SuperClass* CellByCellRejectionSampling::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    const kvs::VolumeObjectBase::VolumeType volume_type = volume->volumeType();
    if ( volume_type == kvs::VolumeObjectBase::Structured )
    {
        if ( m_camera )
        {
            this->mapping( m_camera, reinterpret_cast<const kvs::StructuredVolumeObject*>( object ) );
        }
        else
        {
            // Generate particles by using default camera parameters.
            kvs::Camera* camera = new kvs::Camera();
            this->mapping( camera, reinterpret_cast<const kvs::StructuredVolumeObject*>( object ) );
            delete camera;
        }
    }
    else // volume_type == kvs::VolumeObjectBase::Unstructured
    {
        if ( m_camera )
        {
            this->mapping( m_camera, reinterpret_cast<const kvs::UnstructuredVolumeObject*>( object ) );
        }
        else
        {
            // Generate particles by using default camera parameters.
            kvs::Camera* camera = new kvs::Camera();
            this->mapping( camera, reinterpret_cast<const kvs::UnstructuredVolumeObject*>( object ) );
            delete camera;
        }
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Mapping for the structured volume object.
 *  @param  camera [in] pointer to the camera
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void CellByCellRejectionSampling::mapping( const kvs::Camera* camera, const kvs::StructuredVolumeObject* volume )
{
    // Attach the pointer to the volume object and set the min/max coordinates.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // Calculate the density map.
    m_density_map = Generator::CalculateDensityMap(
        camera,
        BaseClass::volume(),
        static_cast<float>( m_subpixel_level ),
        m_sampling_step,
        BaseClass::transferFunction().opacityMap() );

    // Generate the particles.
    const std::type_info& type = volume->values().typeInfo()->type();
    if (      type == typeid( kvs::UInt8  ) ) this->generate_particles<kvs::UInt8>( volume );
    else if ( type == typeid( kvs::UInt16 ) ) this->generate_particles<kvs::UInt16>( volume );
    else if ( type == typeid( kvs::Int16  ) ) this->generate_particles<kvs::Int16>( volume );
    else if ( type == typeid( kvs::Real32 ) ) this->generate_particles<kvs::Real32>( volume );
    else if ( type == typeid( kvs::Real64 ) ) this->generate_particles<kvs::Real64>( volume );
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Mapping for the unstructured volume object.
 *  @param  camera [in] pointer to the camera
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void CellByCellRejectionSampling::mapping( const kvs::Camera* camera, const kvs::UnstructuredVolumeObject* volume )
{
    // Attach the pointer to the volume object and set the min/max coordinates.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // Calculate the density map.
    m_density_map = Generator::CalculateDensityMap(
        camera,
        BaseClass::volume(),
        static_cast<float>( m_subpixel_level ),
        m_sampling_step,
        BaseClass::transferFunction().opacityMap() );

    // Generate the particles.
    this->generate_particles( volume );
}

/*===========================================================================*/
/**
 *  @brief  Generates particles for the structured volume object.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
template <typename T>
void CellByCellRejectionSampling::generate_particles( const kvs::StructuredVolumeObject* volume )
{
    // Vertex data arrays. (output)
    std::vector<kvs::Real32> vertex_coords;
    std::vector<kvs::UInt8>  vertex_colors;
    std::vector<kvs::Real32> vertex_normals;

    // Set a trilinear interpolator.
    kvs::TrilinearInterpolator interpolator( volume );

    const T* const pvalues = reinterpret_cast<const T*>( volume->values().data() );
    const kvs::ColorMap color_map( BaseClass::transferFunction().colorMap() );

    // Generate particles for each cell.
    const kvs::Vector3ui ncells( volume->resolution() - kvs::Vector3ui::All(1) );
    const size_t ncellnodes = 8;
    for ( kvs::UInt32 z = 0; z < ncells.z(); ++z )
    {
        for ( kvs::UInt32 y = 0; y < ncells.y(); ++y )
        {
            for ( kvs::UInt32 x = 0; x < ncells.x(); ++x )
            {
                // Interpolate at the center of gravity of this cell.
                const kvs::Vector3f cog( x + 0.5f, y + 0.5f, z + 0.5f );
                interpolator.attachPoint( cog );

                // Calculate a number of particles in this cell.
                const float volume_of_cell = 1.0f;
                const float averaged_scalar = interpolator.template scalar<T>();
                const float density = this->calculate_density( averaged_scalar );
                const size_t nparticles = this->calculate_number_of_particles( density, volume_of_cell );

                const kvs::UInt32* const index =interpolator.indices();
                const T S[8] = {
                    pvalues[index[0]], pvalues[index[1]], pvalues[index[2]], pvalues[index[3]],
                    pvalues[index[4]], pvalues[index[5]], pvalues[index[6]], pvalues[index[7]] };
                T S_min = S[0];
                T S_max = S[0];
                for ( size_t i = 1; i < ncellnodes; i++ )
                {
                    S_min = kvs::Math::Min( S_min, S[i] );
                    S_max = kvs::Math::Max( S_max, S[i] );
                }
                const float s_min = static_cast<float>( S_min );
                const float s_max = static_cast<float>( S_max );
                const float p_max = this->calculate_maximum_density( s_min, s_max ) / nparticles;

                // Generate a set of particles in this cell.
                const kvs::Vector3f v( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) );
                size_t count = 0;
                while ( count < nparticles )
                {
                    const kvs::Vector3f coord( Generator::RandomSamplingInCube( v ) );
                    interpolator.attachPoint( coord );

                    const float scalar = interpolator.template scalar<T>();
                    const float density = this->calculate_density( scalar );

                    const float p = density / nparticles;
                    const float R = Generator::GetRandomNumber();
                    if ( p > p_max * R )
                    {
                        // Calculate a color.
                        const kvs::RGBColor color( color_map.at( scalar ) );

                        // Calculate a normal.
                        const Vector3f normal( interpolator.template gradient<T>() );

                        // set coord, color, and normal to point object( this ).
                        vertex_coords.push_back( coord.x() );
                        vertex_coords.push_back( coord.y() );
                        vertex_coords.push_back( coord.z() );

                        vertex_colors.push_back( color.r() );
                        vertex_colors.push_back( color.g() );
                        vertex_colors.push_back( color.b() );

                        vertex_normals.push_back( normal.x() );
                        vertex_normals.push_back( normal.y() );
                        vertex_normals.push_back( normal.z() );

                        count++;
                    }
                } // end of 'paricle' while-loop
            } // end of 'x' loop
        } // end of 'y' loop
    } // end of 'z' loop

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( vertex_coords ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( vertex_colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( vertex_normals ) );
    SuperClass::setSize( 1.0f );
}

/*===========================================================================*/
/**
 *  @brief  Generates particles for the unstructured volume object.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void CellByCellRejectionSampling::generate_particles( const kvs::UnstructuredVolumeObject* volume )
{
    // Vertex data arrays. (output)
    std::vector<kvs::Real32> vertex_coords;
    std::vector<kvs::UInt8>  vertex_colors;
    std::vector<kvs::Real32> vertex_normals;

    // Set a tetrahedral cell interpolator.
    kvs::CellBase* cell = NULL;
    switch ( volume->cellType() )
    {
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    {
        cell = new kvs::TetrahedralCell( volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
    {
        cell = new kvs::QuadraticTetrahedralCell( volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::Hexahedra:
    {
        cell = new kvs::HexahedralCell( volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
    {
        cell = new kvs::QuadraticHexahedralCell( volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::Pyramid:
    {
        cell = new kvs::PyramidalCell( volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::Prism:
    {
        cell = new kvs::PrismaticCell( volume );
        break;
    }
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unsupported cell type.");
        return;
    }
    }

    const kvs::ColorMap color_map( BaseClass::transferFunction().colorMap() );

    // Generate particles for each cell.
    const size_t ncells = volume->numberOfCells();
    const size_t ncellnodes = volume->numberOfCellNodes();
    for ( size_t index = 0; index < ncells; ++index )
    {
        // Bind the cell which is indicated by 'index'.
        cell->bindCell( index );

        // Calculate a number of particles in this cell.
//        const float averaged_scalar = cell->averagedScalar();
        const float averaged_scalar = Generator::AveragedScalar( cell );
        const float density = this->calculate_density( averaged_scalar );
        const size_t nparticles = this->calculate_number_of_particles( density, cell->volume() );

        const float* S = cell->values();
        float S_min = S[0];
        float S_max = S[0];
        for ( size_t i = 1; i < ncellnodes; i++ )
        {
            S_min = kvs::Math::Min( S_min, S[i] );
            S_max = kvs::Math::Max( S_max, S[i] );
        }
        const float s_min = static_cast<float>( S_min );
        const float s_max = static_cast<float>( S_max );
        const float p_max = this->calculate_maximum_density( s_min, s_max ) / nparticles;

        // Generate a set of particles in this cell represented by v0,...,v3 and s0,...,s3.
        size_t count = 0;
        while ( count < nparticles )
        {
            const kvs::Vector3f coord = cell->randomSampling();
            const float scalar = cell->scalar();
            const float density = this->calculate_density( scalar );

            const float p = density / nparticles;
            const float R = Generator::GetRandomNumber();
            if ( p > p_max * R )
            {
                // Calculate a color.
                const kvs::RGBColor color( color_map.at( scalar ) );

                // Calculate a normal.
                const Vector3f normal( cell->gradientVector() );

                // set coord, color, and normal to point object( this ).
                vertex_coords.push_back( coord.x() );
                vertex_coords.push_back( coord.y() );
                vertex_coords.push_back( coord.z() );

                vertex_colors.push_back( color.r() );
                vertex_colors.push_back( color.g() );
                vertex_colors.push_back( color.b() );

                vertex_normals.push_back( normal.x() );
                vertex_normals.push_back( normal.y() );
                vertex_normals.push_back( normal.z() );

                count++;
            }
        } // end of 'paricle' while-loop
    } // end of 'cell' for-loop

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( vertex_coords ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( vertex_colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( vertex_normals ) );
    SuperClass::setSize( 1.0f );

    delete cell;
}

/*===========================================================================*/
/**
 *  @brief  Calculate density value.
 *  @param  scalar [in] scalar value
 *  @return density value
 */
/*===========================================================================*/
float CellByCellRejectionSampling::calculate_density( const float scalar )
{
    const float min_value = BaseClass::transferFunction().colorMap().minValue();
    const float max_value = BaseClass::transferFunction().colorMap().maxValue();
    const float max_range = static_cast<float>( BaseClass::transferFunction().resolution() - 1 );
    const float normalize_factor = max_range / ( max_value - min_value );
    const float normalized_scalar = ( scalar - min_value ) * normalize_factor;
    const size_t index0 = static_cast<size_t>( normalized_scalar );
    const size_t index1 = index0 + 1;
    const float scalar_offset = normalized_scalar - index0;

    const float* const density_map = m_density_map.data();

    if ( index0 == ( BaseClass::transferFunction().resolution() - 1 ) )
    {
        return density_map[ index0 ];
    }
    else
    {
        const float rho0 = density_map[ index0 ];
        const float rho1 = density_map[ index1 ];
        const float interpolated_density = ( rho1 - rho0 ) * scalar_offset + rho0;

        return interpolated_density;
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculate number of particles.
 *  @param  density [in] density value
 *  @param  volume_of_cell [in] volume of cell
 *  @return number of particles
 */
/*===========================================================================*/
size_t CellByCellRejectionSampling::calculate_number_of_particles(
    const float density,
    const float volume_of_cell )
{
    const float N = density * volume_of_cell;
    const float R = Generator::GetRandomNumber();

    size_t n = static_cast<size_t>( N );
    if ( N - n > R ) { ++n; }

    return n;
}

/*===========================================================================*/
/**
 *  @brief  Calculate maximum dentiy value.
 *  @param  scalar0 [in] scalar value
 *  @param  scalar1 [in] scalar value
 *  @return density value
 */
/*===========================================================================*/
float CellByCellRejectionSampling::calculate_maximum_density( const float scalar0, const float scalar1 )
{
    if ( scalar0 > scalar1 )
    {
        kvsMessageError("undefined use of calculate_maximum_density.");
        return 0.0f;
    }

    const float min_value = BaseClass::transferFunction().colorMap().minValue();
    const float max_value = BaseClass::transferFunction().colorMap().maxValue();
    const float max_range = static_cast<float>( BaseClass::transferFunction().resolution() - 1 );
    const float normalize_factor = max_range / ( max_value - min_value );
    const size_t index0 = static_cast<size_t>( ( scalar0 - min_value ) * normalize_factor ) + 1;
    const size_t index1 = static_cast<size_t>( ( scalar1 - min_value ) * normalize_factor );

    const float* const density_map = m_density_map.data();

    float maximum_density = density_map[ index0 ];

    for ( size_t i = index0 + 1; i <= index1; i++ )
    {
        maximum_density = density_map[ i ] > maximum_density ? density_map[ i ] : maximum_density;
    }

    const float density0 = this->calculate_density( scalar0 );
    maximum_density = density0 > maximum_density ? density0 : maximum_density;

    const float density1 = this->calculate_density( scalar1 );
    maximum_density = density1 > maximum_density ? density1 : maximum_density;

    return maximum_density;
}

} // end of namespace kvs
