cmake_minimum_required(VERSION 3.10)
project(PCLOMP VERSION "1.0.0")

include(GNUInstallDirs)

find_package(PCL 1.7 REQUIRED COMPONENTS registration)
find_package(OpenMP REQUIRED)

set(HEADERS
	src/pclomp/gicp_omp.h
	src/pclomp/ndt_omp.h
	src/pclomp/voxel_grid_covariance_omp.h
)

add_library(registration SHARED
	src/voxel_grid_covariance_omp.cpp
	src/ndt_omp.cpp
	src/gicp_omp.cpp
)

set_target_properties(registration PROPERTIES
	PUBLIC_HEADER "${HEADERS}"
)

target_link_libraries(registration
	PUBLIC ${PCL_REGISTRATION_LIBRARIES}
	PRIVATE OpenMP::OpenMP_CXX
)

target_include_directories(registration PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	${PCL_INCLUDE_DIRS}
)

# Install binaries
install(TARGETS registration EXPORT pclomp-targets
	ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pclomp
)

# Export the library interface
install(EXPORT pclomp-targets
	NAMESPACE pclomp::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pclomp
)

# Create and install the version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file("pclomp-config-version.cmake"
	VERSION ${PCLOMP_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(
	FILES
		pclomp-config.cmake
		${PROJECT_BINARY_DIR}/pclomp-config-version.cmake
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/pclomp
)
