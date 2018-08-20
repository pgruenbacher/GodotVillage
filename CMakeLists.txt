cmake_minimum_required(VERSION 2.6)
project(ProceduralVillageGeneration)

#-------------------------------------------------------------------------------
# SOURCES
#-------------------------------------------------------------------------------

file(
  GLOB_RECURSE
  SRC_files
  src/*
  )

#-------------------------------------------------------------------------------
# FLAGS
#-------------------------------------------------------------------------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg -O3")
set(QT_QMAKE_EXECUTABLE  "/usr/bin/qmake-qt4")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fomit-frame-pointer -frerun-loop-opt -fgcse")

#-------------------------------------------------------------------------------
# OPENGL
#-------------------------------------------------------------------------------
Find_Package(OpenGL REQUIRED)
if ( NOT OPENGL_FOUND )
  message(FATAL_ERROR "OpenGL not found !")
endif ( NOT OPENGL_FOUND )


INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})


#-------------------------------------------------------------------------------
# GLEW
#-------------------------------------------------------------------------------
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories($(GLEW_INCLUDE_DIRS))
endif()


#-------------------------------------------------------------------------------
# QT 4
#-------------------------------------------------------------------------------
Find_Package(Qt4 REQUIRED)
IF(QT_FOUND)
        SET(QT_USE_QTSQL    FALSE)
        SET(QT_USE_QTXML    TRUE)
        SET(QT_USE_QTOPENGL TRUE)
        INCLUDE(${QT_USE_FILE})
ENDIF(QT_FOUND)
IF(NOT QT_FOUND)
        message(FATAL_ERROR "QT4 not found !")   
ENDIF(NOT QT_FOUND)     

QT4_WRAP_CPP(./src/main.cpp ./src/MainWindow.h)
set(MOC_HEADERS src/UI.h src/GL_Widget.h src/MainWindow.h)
QT4_WRAP_CPP(MOC_OUTFILES ${MOC_HEADERS} )


ADD_DEFINITIONS (${QT_DEFINITIONS})



#-------------------------------------------------------------------------------
# EXECUTABLE
#-------------------------------------------------------------------------------
ADD_EXECUTABLE(proceduralVillageGeneration ${SRC_files} ${QT_INHERITED_SRCS} ${MOC_OUTFILES} )
TARGET_LINK_LIBRARIES( proceduralVillageGeneration ${QT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} )


