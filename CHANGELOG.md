### Version 2.9.0 Released (2020.5.10)
**Added new classes and functions**
+ kvs::Stat::Sum
+ kvs::Stat::Mean
+ kvs::Stat::Var
+ kvs::Stat::VarP
+ kvs::Stat::Cov
+ kvs::Stat::CovP
+ kvs::Stat::StdDev
+ kvs::Stat::StdDevP
+ kvs::Stat::Corr
+ kvs::Stat::AutoCorr
+ kvs::Stat::CrossCorr
+ kvs::Stat::Standardize
+ kvs::StudentTDistributiion
+ kvs::FisherFDistribution
+ kvs::ChiSquaredDistribution
+ kvs::GaussDistribution
+ kvs::ExponentialDistribution
+ kvs::BetaFunction
+ kvs::GammaFunction
+ kvs::LinearRegression
+ kvs::LassoRegression
+ kvs::RidgeRegression
+ kvs::StructuredExtractScalar
+ kvs::PolygonToPolygon
+ kvs::Deleter
+ kvs::Json

**Added new methods**
+ kvs::PolygonRenderer::setPolygonOffset
+ kvs::glsl::PolygonRenderer::setPolygonOffset
+ kvs::Vector{n}::Zero
+ kvs::Vector{2,3,4,n}::Ones
+ kvs::Vector{2,3,4}::UnitX
+ kvs::Vector{2,3,4}::UnitY
+ kvs::Vector{3,4}::UnitZ
+ kvs::Vector{4}::UnitW
+ kvs::Vector{n}::Unit
+ kvs::Vector{2,3,4,n}::Constant
+ kvs::Vector{2,3,4,n}::Identity
+ kvs::Vector{2,3,4,n}::Random
+ kvs::Vector{2,3,4,n}::setZero
+ kvs::Vector{2,3,4,n}::setOnes
+ kvs::Vector{2,3,4}::setUnitX
+ kvs::Vector{2,3,4}::setUnitY
+ kvs::Vector{3,4}::setUnitZ
+ kvs::Vector{4}::setUnitW
+ kvs::Vector{n}::setUnit
+ kvs::Vector{2,3,4,n}::setConstant
+ kvs::Vector{2,3,4,n}::setIdentity
+ kvs::Vector{2,3,4,n}::setRandom
+ kvs::Vector{2,3,4,n}::format
+ kvs::Vector{n}::Vector( InIter first, InIter last )
+ kvs::Vector{n}::Vector( Vector&& ) // move constructor
+ kvs::Vector{n}::operator =( Vector&& ) // move assignment operator
+ kvs::Vector{n}::opeator =/
+ kvs::Vector{n}::begin
+ kvs::Vector{n}::end
+ kvs::Vector{n}::resize
+ kvs::Matrix{22,33,44,nm}::Zero
+ kvs::Matrix{22,33,44,nm}::Ones
+ kvs::Matrix{22,33,44,nm}::Identity
+ kvs::Matrix{22,33,44,nm}::Constant
+ kvs::Matrix{22,33,44,nm}::Random
+ kvs::Matrix{22,33,44,nm}::setZero
+ kvs::Matrix{22,33,44,nm}::setOnes
+ kvs::Matrix{22,33,44,nm}::setIdentity
+ kvs::Matrix{22,33,44,nm}::setConstant
+ kvs::Matrix{22,33,44,nm}::setDiagonal
+ kvs::Matrix{22,33,44,nm}::setRandom
+ kvs::Matrix{22,33,44,nm}::isSymmetric
+ kvs::Matrix{22,33,44,nm}::isDiagonal
+ kvs::Matrix{22,33,44,nm}::format
+ kvs::Matrix{nm}::isSquare
+ kvs::Matrix{nm}::Matrix( Matrix&& ) // move constructor
+ kvs::Matrix{nm}::operator =( Matrix&& ) // move assignment operator
+ kvs::Matrix{nm}::size
+ kvs::Matrix{nm}::beginRows
+ kvs::Matrix{nm}::begin
+ kvs::Matrix{nm}::beginInRowOrder
+ kvs::Matrix{nm}::beginInColumnOrder
+ kvs::Matrix{nm}::beginRow
+ kvs::Matrix{nm}::beginColumn
+ kvs::Matrix{nm}::endRows
+ kvs::Matrix{nm}::end
+ kvs::Matrix{nm}::endInRowOrder
+ kvs::Matrix{nm}::endInColumnOrder
+ kvs::Matrix{nm}::endRow
+ kvs::Matrix{nm}::endColumn
+ kvs::Matrix{nm}::rbegin
+ kvs::Matrix{nm}::rbeginInRowOrder
+ kvs::Matrix{nm}::rbeginInColumnOrder
+ kvs::Matrix{nm}::rend
+ kvs::Matrix{nm}::rendInRowOrder
+ kvs::Matrix{nm}::rendInColumnOrder
+ kvs::Matrix{nm}::resize
+ kvs::Value::Random
+ kvs::Value::RandomSeed
+ kvs::Value::SetSeed
+ kvs::Value::SetRandomSeed
+ kvs::ValueArray::Random
+ kvs::ValueArray::Linear
+ kvs::ValueArray::min
+ kvs::ValueArray::max
+ kvs::ValueArray::sum
+ kvs::ValueArray::argmin
+ kvs::ValueArray::argmax
+ kvs::ValueArray::argsort
+ kvs::ValueArray::sort
+ kvs::ValueArray::shuffle
+ kvs::ValueTable::Random
+ kvs::ValueTable::Linear
+ kvs::ValueTable::begin
+ kvs::ValueTable::end
+ kvs::ValueTable::rbegin
+ kvs::ValueTable::rend
+ kvs::ValueTable::beginInColumnOrder
+ kvs::ValueTable::endInColumnOrder
+ kvs::ValueTable::rbeginInColumnOrder
+ kvs::ValueTable::rendInColumnOrder
+ kvs::ValueTable::beginInRowOrder
+ kvs::ValueTable::endInRowOrder
+ kvs::ValueTable::rbeginInRowOrder
+ kvs::ValueTable::rendInRowOrder
+ kvs::ValueTable::beginColumn
+ kvs::ValueTable::endColumn
+ kvs::ValueTable::beginRow
+ kvs::ValueTable::endRow
+ kvs::ValueTable::clone
+ kvs::Camera::devicePixelRatio
+ kvs::OrientationAxis::setAxisTypeToCornered
+ kvs::OrientationAxis::setAxisTypeToCentered
+ kvs::OrientationAxis::setBoxTypeToWired
+ kvs::OrientationAxis::setBoxTypeToSolid
+ kvs::OrientationAxis::setProjectionTypeToPerspective
+ kvs::OrientationAxis::setProjectionTypeToOrthogonal
+ kvs::Label::setText for std::string
+ kvs::Label::addText for std::string
+ kvs::WidgetBase::font
+ kvs::opencv::MovieRenderer::currentFrameIndex
+ kvs::opencv::MovieRenderer::setFrameIndex
+ kvs::KVSMLLineObject::setWritingDataTypeToAscii
+ kvs::KVSMLLineObject::setWritingDataTypeToExternalAscii
+ kvs::KVSMLLineObject::setWritingDataTypeToExternalBinary
+ kvs::KVSMLPointObject::setWritingDataTypeToAscii
+ kvs::KVSMLPointObject::setWritingDataTypeToExternalAscii
+ kvs::KVSMLPointObject::setWritingDataTypeToExternalBinary
+ kvs::KVSMLPolygonObject::setWritingDataTypeToAscii
+ kvs::KVSMLPolygonObject::setWritingDataTypeToExternalAscii
+ kvs::KVSMLPolygonObject::setWritingDataTypeToExternalBinary
+ kvs::KVSMLStructuredVolumeObject::setWritingDataTypeToAscii
+ kvs::KVSMLStructuredVolumeObject::setWritingDataTypeToExternalAscii
+ kvs::KVSMLStructuredVolumeObject::setWritingDataTypeToExternalBinary
+ kvs::KVSMLTableObject::setWritingDataTypeToAscii
+ kvs::KVSMLTableObject::setWritingDataTypeToExternalAscii
+ kvs::KVSMLTableObject::setWritingDataTypeToExternalBinary
+ kvs::KVSMLUnstructuredVolumeObject::setWritingDataTypeToAscii
+ kvs::KVSMLUnstructuredVolumeObject::setWritingDataTypeToExternalAscii
+ kvs::KVSMLUnstructuredVolumeObject::setWritingDataTypeToExternalBinary
+ kvs::ColorMap::CoolWarm
+ kvs::ColorMap::BrewerBrBG
+ kvs::ColorMap::BrewerPiYG
+ kvs::ColorMap::BrewerPRGn
+ kvs::ColorMap::BrewerPuOr
+ kvs::ColorMap::BrewerRdBu
+ kvs::ColorMap::BrewerRdGy
+ kvs::ColorMap::BrewerRdYlBu
+ kvs::ColorMap::BrewerRdYlGn
+ kvs::ColorMap::BrewerSpectral
+ kvs::LabColor::Mix
+ kvs::LabColor::toRGBColor
+ kvs::MshColor::toRGBColor
+ kvs::RGBColor::toMshColor
+ kvs::ValueArray::format
+ kvs::ValueTable::format

**Added new examples**
+ Example/Utility/Stat
+ Example/Utility/ValueArray
+ Example/Utility/ValueTable
+ Example/Numeric/DistributionFunctions
+ Example/Numeric/LinearRegression
+ Example/Matrix/Matrix
+ Example/Matrix/Matrix22
+ Example/Matrix/Matrix33
+ Example/Matrix/Matrix44
+ Example/Matrix/Vector
+ Example/Matrix/Vector2
+ Example/Matrix/Vector3
+ Example/Matrix/Vector4
+ Example/Visualization/CheckBox
+ Example/Visualization/ColorMapBar
+ Example/Visualization/Label
+ Example/Visualization/OrientationAxis
+ Example/Visualization/PushButton
+ Example/Visualization/RadioButton
+ Example/Visualization/Slider
+ Example/Visualization/ScatterPlot
+ Example/Visualization/RGBFormulae
+ Example/Visualization/DivergingColorMap
+ Example/Visualization/ParallelCoordinates

**Added new macros**
+ KVS_COMPILER_SUPPORT_CXX98
+ KVS_COMPILER_SUPPORT_CXX11
+ KVS_COMPILER_SUPPORT_CXX14
+ KVS_COMPILER_SUPPORT_CXX17
+ KVS_COMPILER_SUPPORT_CXX20

**Deprecated methods**
+ kvs::Vector2::All (use kvs::Vector2::Constant)
+ kvs::Vector2::zero (use kvs::Vector2::setZero)
+ kvs::Vector3::All (use kvs::Vector3::Constant)
+ kvs::Vector3::zero (use kvs::Vector3::setZero)
+ kvs::Vector4::All (use kvs::Vector4::Constant)
+ kvs::Vector4::zero (use kvs::Vector4::setZero)
+ kvs::Vector::zero (use kvs::Vector::setZero)
+ kvs::Vector{2,3,4,n}::length2 (use kvs::Vector{2,3,4,n}::squaredLength)
+ kvs::Vector{n}::setSize (use kvs::Vector{n}::resize)
+ kvs::Matrix2::zero (use kvs::Matrix2::setZero)
+ kvs::Matrix2::identity (use kvs::Matrix2::setIdentity)
+ kvs::Matrix3::zero (use kvs::Matrix3::setZero)
+ kvs::Matrix3::identity (use kvs::Matrix3::setIdentity)
+ kvs::Matrix4::zero (use kvs::Matrix4::setZero)
+ kvs::Matrix4::identity (use kvs::Matrix4::setIdentity)
+ kvs::Matrix::zero (use kvs::Matrix::setZero)
+ kvs::Matrix::identity (use kvs::Matrix::setIdentity)
+ kvs::Matrix{mn}::setSize (use kvs::Matrix{mn}::resize)
+ kvs::Quaternion::length2 (use kvs::QUaternion::squaredLength)
+ kvs::ValueTable::clear (use kvs::ValueTable::release )

**Fixed problem**
+ kvs::EigenDecomposer

**Removed classes**
+ kvs::KVSMLObjectImage (use kvs::KVSMLImageObject)
+ kvs::KVSMLObjectLine (use kvs::KVSMLLineObject)
+ kvs::KVSMLObjectPoint (use kvs::KVSMLPointObject)
+ kvs::KVSMLObjectPolygon (use kvs::KVSMLPolygonObject)
+ kvs::KVSMLObjectStructuredVolume (use kvs::KVSMLStructuredVolumeObject)
+ kvs::KVSMLObjectTable (use kvs::KVSMLTableObject)
+ kvs::KVSMLObjectUnstructuredVolume (use kvs::KVSMLUnstructuredVolumeObject)

### Version 2.8.0 Released (2019.6.7)
**Added new classes and functions**
+ kvs::ColorStream
+ kvs::NullStream
+ kvs::Mouse::trackball
+ kvs::OpenGL::EnableClientState
+ kvs::OpenGL::DisableClientState
+ kvs::OpenGL::VertexPointer
+ kvs::OpenGL::ColorPointer
+ kvs::OpenGL::NormalPointer
+ kvs::OpenGL::TexCoordPointer
+ kvs::OpenGL::MultiDrawArrays
+ kvs::OpenGL::DrawArrays
+ kvs::OpenGL::DrawElements
+ kvs::OpenGL::MultiDrawElements
+ kvs::OpenGL::EnableVertexAttribArray
+ kvs::OpenGL::DisableVertexAttribArray
+ kvs::OpenGL::VertexAttribPointer
+ kvs::VertexBufferObjectManager
+ kvs::CubicImage
+ kvs::SphericalImage
+ kvs::SphericalImageRenderer
+ kvs::opencv::MovieObject
+ kvs::opencv::MovieRenderer
+ kvs::opencv::SphericalMovieRenderer

**Add new methods**
+ kvs::StochasticRenderingCompositor::update
+ kvs::StochasticTetrahedraRenderer::transferFunction
+ kvs::StochasticTetrahedraRenderer::samplingStep
+ kvs::StochasticUniformGridRenderer::transferFunction
+ kvs::StochasticUniformGridRenderer::samplingStep
+ kvs::Directory::RootPath
+ kvs::Directory::Root
+ kvs::Directory::HomePath
+ kvs::Directory::Home
+ kvs::Directory::TempPath
+ kvs::Directory::Temp
+ kvs::Directory::CurrentPath
+ kvs::Directory::Absolute
+ kvs::File::Exists

**Added SupportOSMesa**
+ kvs::osmesa::Context
+ kvs::osmesa::Screen
+ kvs::osmesa::ScreenBase
+ kvs::osmesa::Surface

**Added SupportEGL**
+ kvs::egl::Config
+ kvs::egl::Context
+ kvs::egl::Display
+ kvs::egl::Screen
+ kvs::egl::ScreenBase
+ kvs::egl::Surface

**Added new example**
+ Example/SupportOSMesa/Hello
+ Example/SupportEGL/Hello

**Added new envrionmental variables for compiling KVS**
+ KVS_GL_DIR
+ KVS_GL_INCLUDE_PATH
+ KVS_GL_LIBRARY_PATH
+ KVS_GL_LINK_LIBRARY
+ KVS_GLUT_DIR
+ KVS_GLUT_INCLUDE_PATH
+ KVS_GLUT_LIBRARY_PATH
+ KVS_GLUT_LINK_LIBRARY
+ KVS_GLEW_DIR
+ KVS_GLEW_INCLUDE_PATH
+ KVS_GLEW_LIBRARY_PATH
+ KVS_GLEW_LINK_LIBRARY
+ KVS_OSMESA_DIR
+ KVS_OSMESA_INCLUDE_PATH
+ KVS_OSMESA_LIBRARY_PATH
+ KVS_OSMESA_LINK_LIBRARY
+ KVS_EGL_DIR
+ KVS_EGL_INCLUDE_PATH
+ KVS_EGL_LIBRARY_PATH
+ KVS_EGL_LINK_LIBRARY
+ KVS_OPENMP_DIR
+ KVS_OPENMP_INCLUDE_PATH
+ KVS_OPENMP_LIBRARY_PATH
+ KVS_OPENMP_LINK_LIBRARY

**Added new support in kvsmake**
+ Fortran codes can be compiled with kvsmake

**Fixed problem**
+ kvs::EnsembleAverageBuffer
+ kvs::StochasticPointRenderer
+ kvs::StochasticPolygonRenderer
+ kvs::StochasticTetrahedraRenderer
+ kvs::StochasticUniformGridRenderer
+ kvs::HydrogenVolumeData
+ kvs::CellByCellMetropolisSampling
+ kvs::glsl::PolygonRenderer

**Modified message format in kvs::Message**
+ kvsMessageError()
+ kvsMessageWarning()
+ kvsMessageDebug()
+ kvsMessageAssert()

**Modified shader log message**
+ kvs::ProgramObject
+ kvs::ProgramObject

**Modified for Spark64**
+ KVS_BREAKPOINT
+ KVS_PLATFORM_CPU_SPARK64
+ KVS_PLATFORM_CPU_NAME

**Modified for VS2017 compiler**
+ KVS_COMPILER_VERSION

**Modified for Intel C++ compiler**

**Reimplemented with VBO**
+ kvs::StylizedLineRenderer

**Duprecated in kvs::Mouse**
+ attachCamera (use trackball().attacheCamera instead)
+ setRotationCenter( use trackball().setRotationCenter instead)
+ scaling (use trackball().scaling instead)
+ rotation (use trackball().rotation instead)

**Duprecated in kvs::Directory**
+ directoryPath (use path instead)
+ directoryName (use name instead)

**Removed method**
+ kvs::Directory::sort
+ kvs::Directory::find

### Version 2.7.0 Released (2018.8.4)
**Added new classes and functions**
+ kvs::HCLColor
+ kvs::Rectangle
+ kvs::Vec2u
+ kvs::Vec3u
+ kvs::Vec4u
+ kvs::OpenGL::Vertex
+ kvs::OpenGL::Color
+ kvs::OpenGL::Normal
+ kvs::OpenGL::TexCoord
+ kvs::OpenGL::Hint
+ kvs::OpenGL::SetLightModel
+ kvs::OpenGL::SetPointSize
+ kvs::OpenGL::SetPolygonStipple
+ kvs::OpenGL::SetLineStipple
+ kvs::OpenGL::SetAlphFunc
+ kvs::OpenGL::LoadMatrix
+ kvs::OpenGL::Rotate
+ kvs::OpenGL::Scale
+ kvs::OpenGL::Translate
+ kvs::OpenGL::DrawCylinder
+ kvs::OpenGL::DrawSphere
+ kvs::OpenGL::Render2D
+ kvs::OpenMP::SetNumberOfThreads
+ kvs::OpenMP::GetNumberOfThreads
+ kvs::OpenMP::GetMaxThreads
+ kvs::OpenMP::GetThreadNumber
+ kvs::OpenMP::GetThreadLimit
+ kvs::OpenMP::GetNumberOfProcessors
+ kvs::OpenMP::InParallel
+ kvs::OpenMP::SetDynamic
+ kvs::OpenMP::GetNested
+ kvs::OpenMP::SetNested
+ kvs::OpenMP::SetMaxActiveLevels
+ kvs::OpenMP::GetMaxActiveLevels
+ kvs::OpenMP::GetLevel
+ kvs::OpenMP::GetAncestorThreadNumber
+ kvs::OpenMP::GetTeamSize
+ kvs::OpenMP::GetActiveLevel
+ kvs::OpenMP::InFinal
+ kvs::OpenMP::GetWTime
+ kvs::OpenMP::GetWTick
+ kvs::OpenMP::Mutex
+ kvs::NanoVG
+ kvs::FontStash
+ kvs::Font
+ kvs::FontMetrics
+ kvs::Painter
+ kvs::PaintDevice
+ kvs::WidgetBase
+ kvs::CheckBox
+ kvs::CheckBoxGroup
+ kvs::ColorMapMar
+ kvs::ColorMapPalette
+ kvs::ColorPalette
+ kvs::HistogramBar
+ kvs::Label
+ kvs::OpacityMapBar
+ kvs::OpacityMapPalette
+ kvs::OrientationAxis
+ kvs::PushButton
+ kvs::RadioButton
+ kvs::RadioButtonGroup
+ kvs::Slider
+ kvs::Axis2D
+ kvs::Axis2DMatrix
+ kvs::Axis3D
+ kvs::ParallelAxis
+ kvs::BDMLData
+ kvs::IPLab
+ kvs::IPLabList

**Add new methods**
+ kvs::StochasticLineRenderer::setLineOffset

**Added SupportPython**
+ kvs::python::Array
+ kvs::python::Bool
+ kvs::python::Callable
+ kvs::python::Dict
+ kvs::python::Float
+ kvs::python::Int
+ kvs::python::Interpreter
+ kvs::python::List
+ kvs::python::Module
+ kvs::python::Object
+ kvs::python::String
+ kvs::python::Table
+ kvs::python::Tuple

**Removed classes**
+ kvs::glut::Rectangle (use kvs::Rectangle)

**Removed setSubpixelLevel in particle sampling classes**
+ setSubpixelLevel( subpixel_level ) -> (removed, use setRepetitionLevel( subpixel_level * subpixel_level ))
+ Note: An argument in constructor of particle sampling class is modified to repetition_level not subpixel_level.

**Reimplemented with OpenMP**
+ kvs::CellByCellLayeredSampling
+ kvs::CellByCellMetropolisSampling
+ kvs::CellByCellRejectionSampling
+ kvs::CellByCellUniformSampling

**Added TrueType fonts**
+ NotoSans-Regular.ttf
+ NotoSans-Bold.ttf
+ NotoSans-Italic.ttf
+ NotoSans-BoldItalic.ttf
+ NotoSerif-Regular.ttf
+ NotoSerif-Bold.ttf
+ NotoSerif-Italic.ttf
+ NotoSerif-BoldItalic.ttf
+ entypo.ttf

**Added new example**
+ Example/OpenMP/Hello

**Added some new option in KVS**
+ Environment parameter KVS_CPP for C++ compiler
+ Environment parameter KVS_CC for C compiler
+ Environment parameter KVS_FCC for Fortran compiler
+ Compiler option KVS_ENABLE_OSMESA for OSMesa
+ Compiler option KVS_ENABLE_OPENMP for OpenMP

**Added a new option in kvsmake**
+ Options 'q' and 'Q' for compiling Qt program with kvsmake

### Version 2.6.0 Released (2016.4.5)
**Add new classes and functions**
+ kvs::StylizedLineRenderer

**Add new methods**
+ kvs::ObjectBase::read/write
+ kvs::LineObject::read/write
+ kvs::PointObject::read/write
+ kvs::PolygonObject::read/write
+ kvs::StructuredVolumeObject::read/write
+ kvs::UnstructuredVolumeObject::read/write
+ kvs::ImageObject::read/write
+ kvs::TableObject::read/write

**Fixed problem**
+ kvs::NormalizedDeviceCoordinate::toCameraCoordinate()
+ kvs::Matrix::invert()
+ kvs::gis::GSIMesh

**Supported prism cell**
+ kvs::SlicePlane

**Supported polygon color and normal type of polygon object**
+ kvs::StochasticPolygonRenderer

**Duprecated methods in kvs::Camera class**
+ modelViewMatrix() -> (deprecated, use kvs::ObjectBase::modelingMatrix() and kvs::Camera::viewingMatrix() instead)
+ projectionModelViewMatrix() -> (deprecated, use kvs::ObjectBase::modelingMatrix() and kvs::Camera::viewingMatrix(), kvs::Camera::projectionMatrix() instead)
+ getProjectionMatrix() -> (deprecated, use kvs::Camera::projectionMatrix() instead)
+ getModelViewMatrix() -> (deprecated, use kvs::ObjectBase::modelingMatrix() and kvs::Camera::viewingMatrix())
+ getProjectionModelViewMatrix() -> (deprecated)
+ getCombinedMatrix() -> (deprecated, use kvs::ObjectBase::modelingMatrix() and kvs::Camera::viewingMatrix(), kvs::Camera::projectionMatrix() instead)
+ projectObjectToWindow() -> (deprecated, use Coordinate classes instead)
+ projectWindowToObject() -> (deprecated, use Coordinate classes instead)
+ projectWindowToCamera() -> (deprecated, use Coordinate classes instead)
+ projectObjectToCamera() -> (deprecated, use Coordinate classes instead)
+ projectCameraToObject() -> (deprecated, use Coordinate classes instead)
+ projectWorldToCamera() -> (deprecated, use Coordinate classes instead)
+ projectCameraToWorld() -> (deprecated, use Coordinate classes instead)
+ projectWorldToObject() -> (deprecated, use Coordinate classes instead)
+ projectObjectToWorld() -> (deprecated, use Coordinate classes instead)

### Version 2.5.0 Released (2015.11.18)
**Add new classes and functions**
+ kvs::MarchingPrism
+ kvs::GridBase
+ kvs::UniformGrid
+ kvs::RectilinearGrid
+ kvs::XYZColor
+ kvs::LabColor
+ kvs::MshColor
+ kvs::DivergingColorMap
+ kvs::InverseDistanceWeighting
+ kvs::UnstructuredGradient
+ kvs::UnstructuredQCriterion
+ kvs::PreIntegrationTable2D

**Fixed problem**
+ kvs::qt::ScreenBase
+ kvs::TableObject
+ kvs::ImageObject
+ kvs::PolygonObject
+ kvs::UnstructuredVolumeExporter
+ KVS_STATIC_ASSERT

**Modified methods in kvs::Csv class**
+ numberOfRows() -> (add)
+ nrows() -> (deprecated, use numberOfRows())

**Modified methods in kvs::RGBColor class**
+ Mix() (static) -> (add)
+ toVec3i() -> (add)
+ toHSVColor -> (add)
+ toXYZColor -> (add)
+ red() -> (deprecated, use r())
+ green() -> (deprecated, use g())
+ blue() -> (deprecated, use b())

**Modified methods in kvs::HSVColor class**
+ Mix() (static) -> (add)
+ toRGBColor() -> (add)
+ hue() -> (deprecated, use h())
+ saturation() -> (deprecated, use s())
+ value() -> (deprecated, use v())
+ intensity() -> (deprecated, use v())

**Modified methods in kvs::StreamlineBase class**
+ BothDirections (enum) -> (removed)
+ integrationMethod() -> (add)
+ integrationDirection() -> (add)
+ integrationInterval() -> (add)

**Modified methods in kvs::CellBase class**
+ m_vertices -> m_coords
+ m_scalars -> m_values
+ vertices() -> coords() (deprecated)
+ scalars() -> values() (deprecated)
+ m_global_point -> (removed)
+ setGlobalPoint() -> (removed)
+ globalPoint() -> (add)
+ transformLocalToGlobal() -> localToGlobal() (deprecated)
+ transformGlobalToLocal() -> globalToLocal() (deprecated)
+ center() -> (add)
+ contains() -> (add)
+ containsLocalPoint() -> (add)
+ averagedScalar -> (deprecated)
+ gradient -> (deprecated)
+ gradientVector -> (add)
+ gradientTensor -> (add)
+ localCenter() -> (add)

**Modified methods in kvs::XXXCell class**
+ NumberOfNodes -> (removed)
+ referenceVolume() -> (add)

### Version 2.4.0 Released (2014-10-10)
**Add new classes and functions**
 + kvs::PrismaticCell
 + kvs::CellTree
 + kvs::CellLocator
 + kvs::CellAdjacencyGraphLocator
 + kvs::CellTreeLocator
 + kvs::OpenGL::SetPolygonMode
 + kvs::OpenGL::SetColorMaterial

**Add new methods**
+ kvs::RGBColor::toVec3
+ kvs::RGBAColor::toVec4
+ kvs::LineObject::setLineTypeToStrip
+ kvs::LineObject::setLineTypeToUniline
+ kvs::LineObject::setLineTypeToPolyline
+ kvs::LineObject::setLineTypeToSegment
+ kvs::LineObject::setColorTypeToVertex
+ kvs::LineObject::setColorTypeToLine
+ kvs::PolygonObject::setPolygonTypeToTriangle
+ kvs::PolygonObject::setPolygonTypeToQuadrangle
+ kvs::PolygonObject::setColorTypeToVertex
+ kvs::PolygonObject::setColorTypeToPolygon
+ kvs::PolygonObject::setNormalTypeToVertex
+ kvs::PolygonObject::setNormalTypeToPolygon
+ kvs::StructuredVolumeObject::setGridTypeToUniform
+ kvs::StructuredVolumeObject::setGridTypeToRectilinear
+ kvs::StructuredVolumeObject::setGridTypeToCurvilinear
+ kvs::StructuredVolumeObject::setLabel
+ kvs::StructuredVolumeObject::setUnit
+ kvs::StructuredVolumeObject::label
+ kvs::StructuredVolumeObject::unit
+ kvs::UnstructuredVolumeObject::setCellTypeToTetrahedra
+ kvs::UnstructuredVolumeObject::setCellTypeToHexahedra
+ kvs::UnstructuredVolumeObject::setCellTypeToQuadraticTetrahedra
+ kvs::UnstructuredVolumeObject::setCellTypeToQuadraticHexahedra
+ kvs::UnstructuredVolumeObject::setCellTypeToPyramid
+ kvs::UnstructuredVolumeObject::setCellTypeToPoint
+ kvs::UnstructuredVolumeObject::setCellTypeToPrism
+ kvs::UnstructuredVolumeObject::setLabel
+ kvs::UnstructuredVolumeObject::setUnit
+ kvs::UnstructuredVolumeObject::label
+ kvs::UnstructuredVolumeObject::unit
+ kvs::ImageObject::pixelType
+ kvs::ColorMap::colorSpace
+ kvs::ColorMap::setColorSpace
+ kvs::ColorMap::setColorSpaceToRGB
+ kvs::ColorMap::setColorSpaceToHSV
+ kvs::ColorMap::addPoint (for HSV color value)
+ kvs::UnstructuredVolumeObject::numberOfCellNodes
+ kvs::Camera::setProjectionTypeToPerspective
+ kvs::Camera::setProjectionTypeToOrthogonal
+ kvs::Camera::setProjectionTypeToFrustum
+ kvs::glut::Screen::setControlTargetToObject
+ kvs::glut::Screen::setControlTargetToCamera
+ kvs::glut::Screen::setControlTargetToLight
+ kvs::qt::Screen::setControlTargetToObject
+ kvs::qt::Screen::setControlTargetToCamera
+ kvs::qt::Screen::setControlTargetToLight
+ kvs::Scene::numberOfObjects
+ kvs::Scene::hasObjects
+ kvs::Scene::hasObject
+ kvs::Scene::object
+ kvs::Scene::renderer

**Add new typedef**
+ kvs::Vec2ui (same as kvs::Vector2ui)
+ kvs::Vec2d (same as kvs::Vector2d)
+ kvs::Vec3d (same as kvs::Vector3d)
+ kvs::Vec4ui (same as kvs::Vector4ui)
+ kvs::Vec4d (same as kvs::Vector4d)
+ kvs::Mat2d (same as kvs::Matrix22d)
+ kvs::Mat3d (same as kvs::Matrix33d)
+ kvs::Mat4d (same as kvs::Matrix44d)

**Add new cell type**
+ kvs::UnstructuredVolumeObject::CellType::Prism

**New support**
+ kvs::ExternalFaces supports “Rectilinear"
+ kvs::GlyphBase supports “Rectilinear”

**Deprecated methods**
+ kvs::ImageObject::type (use kvs::ImageObject::pixelType)
+ kvs::ObjectBase::externalPosition (use kvs::ObjectBase::externalCenter)

**Fixed problems**
+ kvs::glsl::ParticleBasedRenderer (PBR_zooming.vert, PBR_zooming.frag)
+ kvs::SphereGlyph
+ kvs::QuadraticTetrahedralCell

**Removed method**
+ kvs::VolumeObjectBase::gridType (use kvs::StructuredVolumeObject::gridType)
+ kvs::VolumeObjectBase::cellType (use kvs::UnstructuredVolumeObject::cellType)

**Removed enum type**
+ kvs::VolumeObjectBase::GridType::Irregular
+ kvs::VolumeObjectBase::GridType (use kvs::StructuredVolumeObject::GridType)
+ kvs::VolumeObjectBase::CellType (use kvs::UnstructuredVolumeObject::CellType)

**Removed template**
+ kvs::ExporterBase
+ kvs::CellBase
+ kvs::TetrahedralCell
+ kvs::HexahedralCell
+ kvs::QuadraticTetrahedralCell
+ kvs::QuadraticHexahedralCell
+ kvs::PyramidalCell
+ kvs::PrismaticCell

**Supported new attributes in KVSML**
+ Endianness can be specified by ‘endian’ (“big” or “little”) in <DataArray>.
+ Label of the value can be specified by ‘label’ in <Value>.
+ Unit of the value can be specified by ‘unit’ in <Value>.

**Add new example**
+ Example/SupportQt/MainWindow

### Version 2.3.0 Released (2014-04-30)
**Add new classes**
+ kvs::OpenGL::WithPushedClientAttrib

**Add new methods**
+ kvs::RendererManager::rendererID
+ kvs::Scene::enableObjectOperation
+ kvs::Scene::disableObjectOperation
+ kvs::Scene::isEnabledObjectOperation
+ kvs::Scene::setEnabledObjectOperation
+ kvs::Scene::updateGLModelingMatrix
+ kvs::Scene::updateGLViewingMatrix
+ kvs::Scene::updateGLProjectionMatrix
+ kvs::Scene::updateGLLightParameter
+ kvs::Scene::isEnabledCollisionDetection
+ kvs::Scene::setEnabledCollisionDetection
+ kvs::StochasticRenderingCompositor::timer
+ kvs::Quaternion::conjugated
+ kvs::glut::OrientationAxis::isEnabledAntiAliasing
+ kvs::glut::OrientationAxis::setEnabledAntiAliasing

**Add new functions**
+ kvs::OpenGL::SetLight
+ kvs::OpenGL::PushClientAttrib
+ kvs::OpenGL::PopClientAttrib
+ kvs::Quaternion::Zero
+ kvs::Quaternion::Identity

**Fixed problems**
+ kvs::StructuredVolumeImporter

**Deprecated methods**
+ kvs::ObjectManager::setActiveObjectID (use kvs::ObjectManager::setActiveObject)
+ kvs::IDManager::operator [] (use kvs::IDManager::id)
+ kvs::Quaternion::rotate (use kvs::Quaternion::Rotate)
+ kvs::Quaternion::rotationQuaternion (use kvs::Quaternion::RotationQuaternion)
+ kvs::Quaternion::linearInterpolation (use kvs::Quaternion::LinearInterpolation)
+ kvs::Quaternion::sphericalLinearInterpolation (use kvs::Quaternion::SphericalLinearInterpolation)
+ kvs::Quaternion::sphericalCubicInterpolation (use kvs::Quaternion::SphericalCubicInterpolation)
+ kvs::Quaternion::splineInterpolation (use kvs::Quaternion::SplineInterpolation)
+ kvs::Quaternion::spline (use kvs::Quaternion::Spline)
+ kvs::Stl::ntriangles (use kvs::Stl::numberOfTriangles)

**Removed class**
+ kvs::ObjectManager::enableAllMove
+ kvs::ObjectManager::disableAllMove
+ kvs::ObjectManager::isEnableAllMove
+ kvs::Scene::enableAllMove
+ kvs::Scene::disableAllMove
+ kvs::Scene::isEnabledAllMove

**Removed method**
+ kvs::ObjectBase::enableCollision
+ kvs::ObjectBase::disableCollision
+ kvs::ObjectBase::canCollision
+ kvs::ObjectBase::positionInDevice
+ kvs::ObjectBase::positionInWorld
+ kvs::ObjectBase::positionInExternal (use kvs::ObjectBase::externalPosition)
+ kvs::ObjectBase::transform
+ kvs::ObjectBase::collision
+ kvs::ObjectManager::detectCollision
+ kvs::ObjectManager::positionInDevice
+ kvs::PipelineModule::object

**Removed macro**
+ kvsClassName
+ kvsModuleName (use kvsModule)

### Version 2.2.0 Released (2014-01-14)
**Add new classes**
+ kvs::ValueTable
+ kvs::AnyValueTable
+ kvs::KMeans
+ kvs::FastKMeans
+ kvs::AdaptiveKMeans
+ kvs::KMeansClustering
+ kvs::PreIntegrationTable3D
+ kvs::Program
+ kvs::Noncopyable
+ kvs::cuda::DriverAPI::Context
+ kvs::cuda::DriverAPI::Device
+ kvs::cuda::DriverAPI::ErrorString
+ kvs::cuda::DriverAPI::Initialization
+ kvs::cuda::RuntimeAPI::Device
+ kvs::cuda::RuntimeAPI::Initialization

**Add new methods**
+ kvs::Scene::registerObject
+ kvs::Scene::removeObject
+ kvs::Scene::replaceObject
+ kvs::Scene::replaceRenderer
+ kvs::ObjectManager::numberOfObjects
+ kvs::TransferFunction::table

**Add new functions**
+ kvs::Math::Fract
+ kvs::Math::Mod
+ kvs::Math::Mix
+ kvs::Math::Step

**Fixed problems**
+ kvs::AVSUcd
+ kvs::ShaderSource
+ kvs::StochasticRenderingCompositor
+ kvs::glsl::RayCastingRenderer (RC_ray_caster.frag)
+ kvs::KVSMLObjectUnstructuredVolume
+ kvs::StochasticUniformGridRenderer
+ kvs::glsl::ParticleBasedRenderer

**Deprecated methods**
+ kvs::ObjectManager::nobjects
+ kvs::RendererManager::nrenderers
+ kvs::StochasticRenderingCompositor::isEnabledShading
+ kvs::StochasticRenderingCompositor::setEnabledShading
+ kvs::StochasticRenderingCompositor::enableShading
+ kvs::StochasticRenderingCompositor::disableShading

### Version 2.1.0 Released (2013-06-05)
**Added some new functions and classes.**
+ kvs::ScreenCaptureEvent
+ kvs::TargetChangeEvent

**Added some new functions and classes.**
+ kvs::OpenGL::Flush
+ kvs::OpenGL::Finish
+ kvs::OpenGL::Clear
+ kvs::OpenGL::SetColorMask
+ kvs::OpenGL::SetDepthMask
+ kvs::OpenGL::SetDepthFunc
+ kvs::OpenGL::SetBlendFunc
+ kvs::OpenGL::SetShadeModel
+ kvs::OpenGL::SetMatrixMode
+ kvs::OpenGL::SetCullFace
+ kvs::OpenGL::SetReadBuffer
+ kvs::OpenGL::SetDrawBuffer
+ kvs::OpenGL::SetDrawBuffers
+ kvs::OpenGL::SetRasterPos
+ kvs::OpenGL::SetViewport
+ kvs::OpenGL::SetOrtho
+ kvs::OpenGL::SetPerspective
+ kvs::OpenGL::SetFrustum
+ kvs::OpenGL::ReadPixels
+ kvs::OpenGL::DrawPixels
+ kvs::OpenGL::GetModelViewMatrix
+ kvs::OpenGL::GetModelViewMatrix
+ kvs::OpenGL::GetProjectionMatrix
+ kvs::OpenGL::GetProjectionMatrix
+ kvs::OpenGL::GetViewport
+ kvs::OpenGL::modelViewMatrix
+ kvs::OpenGL::projectionMatrix
+ kvs::OpenGL::viewport
+ kvs::OpenGL::LoadIdentity
+ kvs::OpenGL::LoadMatrix
+ kvs::OpenGL::LoadMatrix
+ kvs::OpenGL::MultMatrix
+ kvs::OpenGL::MultMatrix
+ kvs::OpenGL::Rotate
+ kvs::OpenGL::Scale
+ kvs::OpenGL::Translate
+ kvs::OpenGL::PushMatrix
+ kvs::OpenGL::PopMatrix
+ kvs::OpenGL::PushAttrib
+ kvs::OpenGL::PopAttrib
+ kvs::OpenGL::WithPushedMatrix
+ kvs::OpenGL::WithPushedAttrib
+ kvs::OpenGL::WithEnabled
+ kvs::OpenGL::WithDisabled

**Added some new methods.**
+ kvs::FrameBuffer::readPixels
+ kvs::FrameBuffer::drawPixels

**Added some new static methods.**
+ kvs::Texture::Bind
+ kvs::Texture::Unbind
+ kvs::Texture::SelectActiveUnit
+ kvs::Texture::SetEnv

**Added some new methods and classes.**
+ kvs::ProgramObject::Binder
+ kvs::ProgramObject::detach
+ kvs::ProgramObject::release
+ kvs::ProgramObject::build
+ kvs::ProgramObject::isCreated
+ kvs::ProgramObject::isValid
+ kvs::ProgramObject::isBound
+ kvs::ProgramObject::isLinked
+ kvs::ProgramObject::setUniform
+ kvs::ProgramObject::setGeometryInputType
+ kvs::ProgramObject::setGeometryOutputType
+ kvs::ProgramObject::setGeometryOutputVertices

**Added some new methods.**
+ kvs::ShaderObject::isCreated
+ kvs::ShaderObject::isValid
+ kvs::ShaderObject::isCompiled

**Deprecated some methods and functions.**
+ kvs::OpenGL::ActivateTextureUnit
+ kvs::ProgramObject::clear
+ kvs::ProgramObject::setUnifromValuei
+ kvs::ProgramObject::setUnifromValuef
+ kvs::ShaderObject::clear
+ kvs::ShaderObject::create( shader_source )
+ kvs::FrameBuffer::read
+ kvs::FrameBuffer::draw
+ kvs::RayCastngRenderer::enableCoarseRendering
+ kvs::RayCastngRenderer::disableCoarseRendering

**Improved implementations.**
+ kvs::glsl::ku::ParticleBasedRenderer
+ kvs::StochasticPointRenderer
+ kvs::StochasticLineRenderer
+ kvs::StochasticPolygonRenderer
+ kvs::StochasticUniformGridRenderer
+ kvs::StochasticTetrahedraRenderer
+ kvs::StochasticMultivariateTetrahedraRenderer
+ kvs::StochasticMultipleTetrahedraRenderer
+ kvs::StochasticRenderingCompositor
+ kvs::StochasticMultipleTetrahedraCompositor
+ kvs::StochasticRendererBase
+ kvs::StochasticRenderingEngine
+ kvs::EnsembleAverageBuffer

**Removed classes.**
+ kvs::StochasticPointEngine
+ kvs::StochasticLineEngine
+ kvs::StochasticPolygonEngine
+ kvs::StochasticUniformGridEngine
+ kvs::StochasticTetrahedraEngine
+ kvs::StochasticMultivariateTetrahedraEngine
+ kvs::StochasitcMultipleTetrahedraEngine

### Version 2.0.1 Released (2013-04-17)
**Fixed some bugs**
+ kvs::HAVSVolumeRenderer

###Version 2.0.0 Released (2013-04-15)
+ Improved the inner structure of the screen class.
+ Removed SupportSAGE, SupportOpenCABIN, and SupportGLEW.
+ Fixed some bugs.
+ Detailed information here (sorry, Japanese only).

###Version 1.3.0 Released (2012-06-09)
**Added some new methods.**
+ kvs::AnyValueArray
+ kvs::ValueArray
+ kvs::Vector2
+ kvs::Vector3
+ kvs::Vector4

**Fixed some bugs**
+ kvs::ValueArray

**Some modifications for the version of KVS (2.0)**

### Version 1.2.0 Released (2012-04-08)
**Added some new classes.**
+ kvs::Gis
+ kvs::SharedPointer
+ kvs::WeekPointer
+ kvs::glew::HAVSVolumeRenderer
+ kvs::glew::PreIntegrationTable
+ kvs::glew::StochasticPointRenderer
+ kvs::glew::StochasticLineRenderer
+ kvs::glew::StochasticPolygonRenderer
+ kvs::glew::StochasticTetrahedraRenderer
+ kvs::glew::StochasticPointEngine
+ kvs::glew::StochasticLineEngine
+ kvs::glew::StochasticPolygonEngine
+ kvs::glew::StochasticTetrahedraEngine
+ kvs::glew::StochasticRenderingCompositor
+ kvs::glew::StochasticMultivariateTetrahedraRenderer
+ kvs::glew::StochasticMultipleTetrahedraRenderer
+ kvs::glew::StochasticMultipleTetrahedraEngine
+ kvs::glew::StochasticMultipleTetrahedraCompositor

**Fixed some bugs in the following classes**
+ kvs::Streamlines
+ kvs::GlyphBase
+ kvs::rits::ParticleBaseRenderer

### Version 1.1.1 Released (2011-12-09)
**Fixed some bugs in the following classes**
+ kvs::GrADS
+ kvs::grads::XYZDef
+ kvs::grads::TDef
+ kvs::grads::DataDescriptorFile

**Prevented an error message for Mac OS X Lion**
+ kvs::glew::ParticleVolumeRenderer

**Prevented an error message for Linux**
+ kvs::IDManager

**kvsmake: Supported resource files for qmake**

### Version 1.1.0 Released (2011-08-10)
**Added some new classes.**
+ kvs::TableObject
+ kvs::TableImporter
+ kvs::KVSMLObjectTable
+ kvs::ParallelCoordinatesRenderer
+ kvs::ScatterPlotRenderer
+ kvs::ScatterPlotMatrixRenderer
+ kvs::glut::ParallelAxisRenderer
+ kvs::glut::Axis2DRenderer
+ kvs::glut::Axis2DMatrixRenderer
+ kvs::RGBFormulae
+ kvs::GrADS
+ kvs::FrontFlow
+ kvs::FrontSTR

**Added new attributes, label, min_value and max_value to Value tag of KVSML format.**
+ kvs::KVSMLObjectStructuredVolume
+ kvs::KVSMLObjectUnstructuredVolume
+ kvs::ValueTag
+ kvs::StructuredVolumeImporter
+ kvs::UnstructuredVolumeImporter
+ kvs::VolumeObjectBase

**Fixed some bugs in the following classes**
+ kvs::SphereGlyph
+ kvs::TableImporter
+ kvs::Csv

### Version 1.0.3 Released (2011-06-18)
**Fixed some bugs in the following classes**
+ kvs::EigenDecomposer
+ kvs::CellBase (transformGlobalToLocal)

**Changed the array related class to use the string**
+ kvs::AnyValue
+ kvs::AnyValueArray
+ kvs::ValueArray

### Version 1.0.2 Released (2011-05-31)
**Fixed some bugs in the following classes**
+ kvs::KVSMLObjectImage
+ kvs::KVSMLObjectStructuredVolume
+ kvs::KVSMLObjectUnstructuredVolume
+ kvs::KVSMLTransferFunction

**Enabled multiple registration of the same object to the screen class**
+ kvs::Screen

**kvsmake: Improved compatibility of 'vc' option with VC10**

### Version 1.0.1 Released (2011-01-15)
**Fixed some bugs in the following classes**
+ kvs::ColorMap
+ kvs::OpacityMap
+ kvs::TrilinearInterpolator
+ kvs::AVSUcd
+ kvs::Stl

**Prevented some warning messages**
+ kvs::tiff::Entry
+ kvs::ValueArray
+ kvs::PipelineModule

**Changed the properties of the member variables in the following class**
+ kvs::ParticleVolumeRenderer

**kvsmake: Improved compatibility of 'vc' option with VC10**

### Version 1.0.0 Released (2010-10-17)
**Initial release**
