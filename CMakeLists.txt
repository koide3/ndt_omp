cmake_minimum_required(VERSION 3.10)
project(ndt_omp)

include(GNUInstallDirs)

#add_definitions(-std=c++14)
#set(CMAKE_CXX_FLAGS "-std=c++14")

#if (BUILD_WITH_MARCH_NATIVE)
#  add_compile_options(-march=native)
#else()
#  add_definitions(-msse -msse2 -msse3 -msse4 -msse4.1 -msse4.2)
#  set(CMAKE_CXX_FLAGS "-msse -msse2 -msse3 -msse4 -msse4.1 -msse4.2")
#endif()

find_package(PCL 1.7 REQUIRED COMPONENTS registration visualization)
find_package(OpenMP REQUIRED)

message(STATUS "OpenMP_CXX_INCLUDE_DIRS:" ${OpenMP_CXX_INCLUDE_DIRS})
message(STATUS "OpenMP_CXX_LIBRARIES:" ${OpenMP_CXX_LIBRARIES})
message(STATUS "OpenMP_CXX_FLAGS:" ${OpenMP_CXX_FLAGS})


#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")


add_library(ndt_omp
	src/pclomp/voxel_grid_covariance_omp.cpp
	src/pclomp/ndt_omp.cpp
	src/pclomp/gicp_omp.cpp
)

target_link_libraries(ndt_omp
	${PCL_REGISTRATION_LIBRARIES}
	${OpenMP_CXX_LIBRARIES}
)

target_include_directories(ndt_omp PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/include
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	${PCL_INCLUDE_DIRS}
	${OpenMP_CXX_INCLUDE_DIRS}
)

#############
## INSTALL ##
#############

install(TARGETS ndt_omp
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install headers
install(DIRECTORY include/pclomp
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

