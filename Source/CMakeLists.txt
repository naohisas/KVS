project(KVS)
cmake_minimum_required(VERSION 2.8)
set(KVS_VERSION 2.4.0)
SET(serial "2.4.0")
SET(soserial "0")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(GenerateExportHeader)
#
# kvsCore
#
if(TRUE)
	# Glob resources
	file(GLOB_RECURSE kvsCore_SOURCES "${PROJECT_SOURCE_DIR}/Core/*.cpp")
	file(GLOB_RECURSE kvsCore_HEADERS "${PROJECT_SOURCE_DIR}/Core/*.h")
	file(GLOB_RECURSE kvsCore_VS "${PROJECT_SOURCE_DIR}/Core/*.vert")
	file(GLOB_RECURSE kvsCore_FS "${PROJECT_SOURCE_DIR}/Core/*.frag")
	set(kvsCore_INCLUDE_DIRS "")
	foreach(_headerFile &{kvsCore_HEADERS})
		get_filename_component(_dir ${_headerFile} PATH)
		list(APPEND kvsCore_INCLUDE_DIRS ${_dir})
	endforeach()
	foreach(_headerFile &{kvsCore_VS})
		get_filename_component(_dir ${_headerFile} PATH)
		list(APPEND kvsCore_INCLUDE_DIRS ${_dir})
	endforeach()
	foreach(_headerFile &{kvsCore_VS})
		get_filename_component(_dir ${_headerFile} PATH)
		list(APPEND kvsCore_INCLUDE_DIRS ${_dir})
	endforeach()
	list(REMOVE_DUPLICATES kvsCore_INCLUDE_DIRS)

	# Set include dir
	include_directories(
		${kvsCore_INCLUDE_DIRS}
		${PROJECT_SOURCE_DIR}
	)

	# Create kvsCore
	add_library(kvsCore ${kvsCore_SOURCES} )
	generate_export_header(kvsCore)
	# set_target_properties(kvsCore PROPERTIES VERSION ${serial} SOVERSION ${soserial})
	set_property(TARGET kvsCore PROPERTY VERSION ${serial})
	set_property(TARGET kvsCore PROPERTY SOVERSION ${soversion})
	set_property(TARGET kvsCore PROPERTY INTERFACE_kvsCore_MAJOR_VERSION ${serial})
	set_property(TARGET kvsCore PROPERTY COMPATIBLE_INTERFACE_STRING kvs_MAJOR_VERSION 2)

	# Install kvsCore
	INSTALL(
		TARGETS kvsCore EXPORT kvsCoreTargets # エクスポート名の指定
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib 
		RUNTIME DESTINATION bin
		INCLUDES DESTINATION include
	)
	INSTALL(
		FILES 
			${PROJECT_SOURCE_DIR}/kvsCore
			"${CMAKE_CURRENT_BINARY_DIR}/kvscore_export.h"
		DESTINATION 
			include
		COMPONENT
			Devel
	)
	INSTALL(
		DIRECTORY ${PROJECT_SOURCE_DIR}/Core
		DESTINATION include
	)
	INSTALL(
		DIRECTORY ${PROJECT_SOURCE_DIR}/kvs
		DESTINATION include
	)

	# settings for config-cmake
	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/kvsCore/kvsCoreConfigVersion.cmake"
		VERSION ${KVS_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	export(EXPORT kvsCoreTargets
		FILE "${CMAKE_CURRENT_BINARY_DIR}/kvsCore/kvsCoreTargets.cmake"
		NAMESPACE kvs::
	)
	configure_file(cmake/kvsCoreConfig.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/kvsCore/kvsCoreConfig.cmake"
		COPYONLY
	)

	set(ConfigPackageLocation lib/cmake/kvsCore)
	install(EXPORT kvsCoreTargets
		FILE
			kvsCoreTargets.cmake
		NAMESPACE
			# kvsCore::
			kvs::
		DESTINATION
			${ConfigPackageLocation}
	)
	install(
		FILES
			cmake/kvsCoreConfig.cmake
			"${CMAKE_CURRENT_BINARY_DIR}/kvsCore/kvsCoreConfigVersion.cmake"
		DESTINATION
			${ConfigPackageLocation}
		COMPONENT
			Devel
	)

endif()

#
# kvsSupportGLUT
#
if(KVS_SUPPORT_GLUT)

	# set flag
	add_definitions(-DKVS_ENABLE_OPENGL=1)

	# Glob resources
	file(GLOB_RECURSE kvsSupportGLUT_SOURCES "${PROJECT_SOURCE_DIR}/SupportGLUT/*.cpp")
	file(GLOB_RECURSE kvsSupportGLUT_HEADERS "${PROJECT_SOURCE_DIR}/SupportGLUT/*.h")
	set(kvsSupportGLUT_INCLUDE_DIRS "")
	foreach(_headerFile &{kvsSupportGLUT_HEADERS})
		get_filename_component(_dir ${_headerFile} PATH)
		list(APPEND kvsSupportGLUT_INCLUDE_DIRS ${_dir})
	endforeach()
	list(REMOVE_DUPLICATES kvsSupportGLUT_INCLUDE_DIRS)

	# Set include dir
	include_directories(
		${kvsSupportGLUT_INCLUDE_DIRS}
		${PROJECT_SOURCE_DIR}
	)

	# Link GLUT
	find_package(GLUT REQUIRED)
	if (GLUT_FOUND)
		include_directories(${GLUT_INCLUDE_DIRS})
		link_libraries(${GLUT_LIBRARIES})
	endif()

	# Create kvsSupportGLUT
	add_library(kvsSupportGLUT ${kvsSupportGLUT_SOURCES})
	generate_export_header(kvsSupportGLUT)
	# SET_TARGET_PROPERTIES(kvsSupportGLUT PROPERTIES VERSION ${serial} SOVERSION ${soserial})
	set_property(TARGET kvsSupportGLUT PROPERTY VERSION ${serial})
	set_property(TARGET kvsSupportGLUT PROPERTY SOVERSION ${soversion})
	set_property(TARGET kvsSupportGLUT PROPERTY INTERFACE_kvsSupportGLUT_MAJOR_VERSION ${serial})
	set_property(TARGET kvsSupportGLUT PROPERTY COMPATIBLE_INTERFACE_STRING kvs_MAJOR_VERSION 2)

	# Lin kvsCore
	target_link_libraries(kvsSupportGLUT kvsCore)
	
	# Link OpenGL
	if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		find_library(OPENGL OpenGL)
	else()
  		find_library(OPENGL GL)
	endif()
	if (NOT OPENGL)
		message(FATAL_ERROR "OpenGL Framework/Library not found")
	endif()
	target_link_libraries(kvsSupportGLUT ${OPENGL})
    target_link_libraries(kvsCore ${OPENGL})



	# Install kvsSupportGLUT
	INSTALL(
		TARGETS kvsSupportGLUT EXPORT kvsSupportGLUTTargets
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		INCLUDES DESTINATION include
	)
	INSTALL(
		FILES 
			${PROJECT_SOURCE_DIR}/kvsSupportGLUT
			"${CMAKE_CURRENT_BINARY_DIR}/kvssupportglut_export.h"
		DESTINATION 
			include
		COMPONENT
			Devel
	)
	INSTALL(
		DIRECTORY ${PROJECT_SOURCE_DIR}/SupportGLUT
		DESTINATION include
	)
	
	# settings for config-cmake
	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/kvsSupportGLUT/kvsSupportGLUTConfigVersion.cmake"
		VERSION ${KVS_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	export(EXPORT kvsCoreTargets
		FILE "${CMAKE_CURRENT_BINARY_DIR}/kvsSupportGLUT/kvsSupportGLUTTargets.cmake"
		NAMESPACE kvs::
	)
	configure_file(cmake/kvsCoreConfig.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/kvsSupportGLUT/kvsSupportGLUTConfig.cmake"
		COPYONLY
	)

	set(ConfigPackageLocation lib/cmake/kvsSupportGLUT)
	install(EXPORT kvsSupportGLUTTargets
		FILE
			kvsSupportGLUTTargets.cmake
		NAMESPACE
			kvs::
		DESTINATION
			${ConfigPackageLocation}
	)
	install(
		FILES
			cmake/kvsSupportGLUTConfig.cmake
			"${CMAKE_CURRENT_BINARY_DIR}/kvsSupportGLUT/kvsSupportGLUTConfigVersion.cmake"
		DESTINATION
			${ConfigPackageLocation}
		COMPONENT
			Devel
	)

endif()


#
# kvsSupportOpenCV
#
if(KVS_SUPPORT_OPENCV)
	# OpenCV
	find_package(OpenCV)

	file(GLOB_RECURSE kvsSupportOpenCV_SOURCES "${PROJECT_SOURCE_DIR}/SupportOpenCV/*.cpp")
	file(GLOB_RECURSE kvsSupportOpenCV_HEADERS "${PROJECT_SOURCE_DIR}/SupportOpenCV/*.h")
	set(kvsSupportOpenCV_INCLUDE_DIRS "")
	foreach(_headerFile &{kvsSupportOpenCV_HEADERS})
		get_filename_component(_dir ${_headerFile} PATH)
		list(APPEND kvsSupportOpenCV_INCLUDE_DIRS ${_dir})
	endforeach()
	list(REMOVE_DUPLICATES kvsSupportOpenCV_INCLUDE_DIRS)

	include_directories(
		${kvsSupportOpenCV_INCLUDE_DIRS}
		${PROJECT_SOURCE_DIR}
	)


	# Create kvsSupportOpenCV
	add_library(kvsSupportOpenCV ${kvsSupportOpenCV_SOURCES})
	SET_TARGET_PROPERTIES(kvsSupportOpenCV PROPERTIES VERSION ${serial} SOVERSION ${soserial})


	# Install kvsSupportGLUT
	INSTALL(
		TARGETS kvsSupportOpenCV
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
	)
	INSTALL(
		FILES ${PROJECT_SOURCE_DIR}/kvsSupportOpenCV
		DESTINATION include
	)
	INSTALL(
		DIRECTORY ${PROJECT_SOURCE_DIR}/SupportOpenCV
		DESTINATION include
	)
endif()


#
# kvsSupportQt
#
if(KVS_SUPPORT_QT)
	# Qt5
	find_package(Qt5Widgets QUIET)
	if(Qt5Widgets_FOUND)
		MESSAGE("Qt5 was FOUND")
		SET(KVS_QT_VERSION 5)

		find_package(Qt5Widgets REQUIRED)
		find_package(Qt5Core REQUIRED)
		find_package(Qt5OpenGL REQUIRED)
		find_package(Qt5Gui REQUIRED)

		SET(CMAKE_AUTOMOC ON)
		SET(CMAKE_INCLUDE_CURRENT_DIR ON)

		add_definitions(
			${Qt5Widgets_DEFINITIONS}
			${Qt5Core_DEFINITIONS}
			${Qt5OpenGL_DEFINITIONS}
			${Qt5Gui_INCLUDE_DIRS}
		)
		include_directories(
			${Qt5Widgets_INCLUDE_DIRS}
			${Qt5Core_INCLUDE_DIRS}
			${Qt5OpenGL_INCLUDE_DIRS}
			${Qt5Gui_INCLUDE_DIRS}
		)
	else()
		# fixme: unreadable if else statements
		# Qt4
		find_package(Qt4 QUIET)
		if(Qt4_FOUND)
			MESSAGE("Qt4 was FOUND")
			find_package(Qt4 REQUIRED)
			set(CMAKE_AUTOMOC ON)
			set(CMAKE_INCLUDE_CURRENT_DIR ON)
			set(QT_USE_QTOPENGL TRUE)
			add_definitions(${QT_DEFINITIONS})
			include(${QT_USE_FILE})
		else()
			MESSAGE("Qt was not found!!!")
		endif()
	endif()

	# GLOB SOURCE 
	file(GLOB_RECURSE kvsSupportQt_SOURCES "${PROJECT_SOURCE_DIR}/SupportQt/*.cpp")
	file(GLOB_RECURSE kvsSupportQt_HEADERS "${PROJECT_SOURCE_DIR}/SupportQt/*.h")
	set(kvsSupportQt_INCLUDE_DIRS "")
	foreach(_headerFile &{kvsSupportQt_HEADERS})
		get_filename_component(_dir ${_headerFile} PATH)
		list(APPEND kvsSupportQt_INCLUDE_DIRS ${_dir})
	endforeach()
	list(REMOVE_DUPLICATES kvsSupportQt_INCLUDE_DIRS)

	include_directories(
		${kvsSupoprtQt_INCLUDE_DIRS}
		${PROJECT_SOURCE_DIR}
	)

	# Create kvsSupportGLUT
	add_library(kvsSupportQt ${kvsSupportQt_SOURCES})
	generate_export_header(kvsSupportQt)
	# SET_TARGET_PROPERTIES(kvsSupportQt PROPERTIES VERSION ${serial} SOVERSION ${soserial})
	set_property(TARGET kvsSupportQt PROPERTY VERSION ${serial})
	set_property(TARGET kvsSupportQt PROPERTY SOVERSION ${soversion})
	set_property(TARGET kvsSupportQt PROPERTY INTERFACE_kvsSupportQt_MAJOR_VERSION ${serial})
	set_property(TARGET kvsSupportQt PROPERTY COMPATIBLE_INTERFACE_STRING kvs_MAJOR_VERSION 2)


	# Install kvsSupportGLUT
	INSTALL(
		TARGETS kvsSupportQt EXPORT kvsSupportQtTargets
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		INCLUDES DESTINATION include
	)
	INSTALL(
		FILES 
			${PROJECT_SOURCE_DIR}/kvsSupportQt
			"${CMAKE_CURRENT_BINARY_DIR}/kvssupportqt_export.h"
		DESTINATION 
			include
		COMPONENT
			Devel
	)
	INSTALL(
		DIRECTORY ${PROJECT_SOURCE_DIR}/SupportQt
		DESTINATION include
	)

	# settings for config-cmake
	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/kvsSupportQt/kvsSupportQtConfigVersion.cmake"
		VERSION ${KVS_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	export(EXPORT kvsSupportQtTargets
		FILE "${CMAKE_CURRENT_BINARY_DIR}/kvsSupportQt/kvsSupportQtTargets.cmake"
		NAMESPACE kvs::
	)
	configure_file(cmake/kvsCoreConfig.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/kvsSupportQt/kvsSupportQtConfig.cmake"
		COPYONLY
	)

	set(ConfigPackageLocation lib/cmake/kvsSupportQt)
	install(EXPORT kvsSupportQtTargets
		FILE
			kvsSupportQtTargets.cmake
		NAMESPACE
			kvs::
		DESTINATION
			${ConfigPackageLocation}
	)
	install(
		FILES
			cmake/kvsSupportQtConfig.cmake
			"${CMAKE_CURRENT_BINARY_DIR}/kvsSupportQt/kvsSupportQtConfigVersion.cmake"
		DESTINATION
			${ConfigPackageLocation}
		COMPONENT
			Devel
	)
	
endif()
