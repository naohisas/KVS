## Changes
**Version 2.4.0 Released** (2014-10-10)
+ Add new classes and functions
 + kvs::PrismaticCell
 + kvs::CellTree
 + kvs::CellLocator
 + kvs::CellAdjacencyGraphLocator
 + kvs::CellTreeLocator
 + kvs::OpenGL::SetPolygonMode
 + kvs::OpenGL::SetColorMaterial
+ Add new methods
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
+ Add new typedef
 + kvs::Vec2ui (same as kvs::Vector2ui)
 + kvs::Vec2d (same as kvs::Vector2d)
 + kvs::Vec3d (same as kvs::Vector3d)
 + kvs::Vec4ui (same as kvs::Vector4ui)
 + kvs::Vec4d (same as kvs::Vector4d)
 + kvs::Mat2d (same as kvs::Matrix22d)
 + kvs::Mat3d (same as kvs::Matrix33d)
 + kvs::Mat4d (same as kvs::Matrix44d)
+ Add new cell type
 + kvs::UnstructuredVolumeObject::CellType::Prism
+ New support
 + kvs::ExternalFaces supports “Rectilinear"
 + kvs::GlyphBase supports “Rectilinear”
+ Deprecated methods
 + kvs::ImageObject::type (use kvs::ImageObject::pixelType)
 + kvs::ObjectBase::externalPosition (use kvs::ObjectBase::externalCenter)
+ Fixed problems
 + kvs::glsl::ParticleBasedRenderer (PBR_zooming.vert, PBR_zooming.frag)
 + kvs::SphereGlyph
 + kvs::QuadraticTetrahedralCell
+ Removed method
 + kvs::VolumeObjectBase::gridType (use kvs::StructuredVolumeObject::gridType)
 + kvs::VolumeObjectBase::cellType (use kvs::UnstructuredVolumeObject::cellType)
+ Removed enum type
 + kvs::VolumeObjectBase::GridType::Irregular
 + kvs::VolumeObjectBase::GridType (use kvs::StructuredVolumeObject::GridType)
 + kvs::VolumeObjectBase::CellType (use kvs::UnstructuredVolumeObject::CellType)
+ Removed template
 + kvs::ExporterBase
 + kvs::CellBase
 + kvs::TetrahedralCell
 + kvs::HexahedralCell
 + kvs::QuadraticTetrahedralCell
 + kvs::QuadraticHexahedralCell
 + kvs::PyramidalCell
 + kvs::PrismaticCell
+ Supported new attributes in KVSML
 + Endianness can be specified by ‘endian’ (“big” or “little”) in <DataArray>.
 + Label of the value can be specified by ‘label’ in <Value>.
 + Unit of the value can be specified by ‘unit’ in <Value>.
+ Add new example
 + Example/SupportQt/MainWindow
