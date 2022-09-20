cmake_minimum_required(VERSION 3.10)
project(ndt_omp)

include(GNUInstallDirs)

find_package(PCL 1.7 REQUIRED COMPONENTS registration)
find_package(OpenMP REQUIRED)

add_library(ndt_omp
	src/pclomp/voxel_grid_covariance_omp.cpp
	src/pclomp/ndt_omp.cpp
	src/pclomp/gicp_omp.cpp
)

target_link_libraries(ndt_omp
	${PCL_REGISTRATION_LIBRARIES}
	OpenMP::OpenMP_CXX
)

target_include_directories(ndt_omp PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/include
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	${PCL_INCLUDE_DIRS}
)

install(TARGETS ndt_omp
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install headers
install(DIRECTORY include/pclomp
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

