cmake_minimum_required(VERSION 2.8)
 
PROJECT(CreateLabelFiles)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(CreateLabelFiles CreateLabelFiles.cxx)
 
if(VTK_LIBRARIES)
  target_link_libraries(CreateLabelFiles ${VTK_LIBRARIES})
else()
  target_link_libraries(CreateLabelFiles vtkHybrid vtkWidgets)
endif()
