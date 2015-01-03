CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(serial "0.1.0")
SET(soserial "0")

# include
find_package(GLUT)
find_package(OpenGL)
find_package(GLEW)
find_package(kvsCore)
find_package(kvsSupportGLUT)

include_directories(
	${PROJECT_SOURCE_DIR}/Source
)


# kvsmake
file(GLOB_RECURSE kvsmake_SOURCES "${PROJECT_SOURCE_DIR}/Tool/kvsmake/*.cpp")
file(GLOB_RECURSE kvsmake_TEMPLATE "${PROJECT_SOURCE_DIR}/Tool/kvsmake/*.template")
file(GLOB_RECURSE kvs_conf "${PROJECT_SOURCE_DIR}/*.conf")
file(GLOB_RECURSE kvs_makefile_def "${PROJECT_SOURCE_DIR}/*.def")
add_executable(kvsmake ${kvsmake_SOURCES})
target_link_libraries(kvsmake
	${GLUT_LIBRARY}
	${OPENGL_LIBRARY}
	${GLEW_LIBRARY}
	kvsCore
	kvsSupportGLUT
)
install(
	TARGETS kvsmake
	DESTINATION bin
)
install(
	FILES ${kvsmake_TEMPLATE}
	DESTINATION bin
)
install(
	FILES ${kvs_conf}
	DESTINATION ${CMAKE_INSTALL_PREFIX}
)
install(
	FILES ${kvs_makefile_def}
	DESTINATION ${CMAKE_INSTALL_PREFIX}
)


# kvsview
file(GLOB_RECURSE kvsview_SOURCES "${PROJECT_SOURCE_DIR}/Tool/kvsview/*.cpp")
add_executable(kvsview ${kvsview_SOURCES})
target_link_libraries(kvsview
	${GLUT_LIBRARY} 
	${OPENGL_LIBRARY}
	${GLEW_LIBRARY} 
	kvsCore 
	kvsSupportGLUT
)
install(
	TARGETS kvsview
	DESTINATION bin
)


# kvscheck
file(GLOB_RECURSE kvscheck_SOURCES "${PROJECT_SOURCE_DIR}/Tool/kvscheck/*.cpp")
add_executable(kvscheck ${kvscheck_SOURCES})
target_link_libraries(kvscheck
	${GLUT_LIBRARY} 
	${OPENGL_LIBRARY}
	${GLEW_LIBRARY}
	kvsCore 
	kvsSupportGLUT
)
install(
	TARGETS kvscheck
	DESTINATION bin
)


# kvsconv
file(GLOB_RECURSE kvsconv_SOURCES "${PROJECT_SOURCE_DIR}/Tool/kvsconv/*.cpp")
add_executable(kvsconv ${kvsconv_SOURCES})
target_link_libraries(kvsconv
	${GLUT_LIBRARY} 
	${OPENGL_LIBRARY}
	${GLEW_LIBRARY} 
	kvsCore 
	kvsSupportGLUT
)
install(
	TARGETS kvsconv
	DESTINATION bin
)