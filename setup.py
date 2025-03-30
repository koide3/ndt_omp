from setuptools import setup, Extension
import pybind11
import os
import subprocess

# Function to get PCL include directory dynamically
def get_pcl_include_dir():
    try:
        # Use pkg-config to find PCL include directory
        result = subprocess.check_output(['pkg-config', '--cflags-only-I', 'pcl_common'], text=True).strip()
        # Extract include dirs (e.g., "-I/usr/include/pcl-1.10")
        include_dirs = [d[2:] for d in result.split() if d.startswith('-I')]
        if include_dirs:
            return include_dirs[0]  # Return the first PCL include dir
    except subprocess.CalledProcessError:
        pass  # pkg-config failed, fall back to search

    # Fallback: Search common PCL installation paths
    common_paths = [
        '/usr/include/pcl-1.12',
        '/usr/include/pcl-1.11',
        '/usr/include/pcl-1.10',
        '/usr/include/pcl-1.9',
        '/usr/include/pcl-1.8',
        '/usr/local/include/pcl-1.12',
        '/usr/local/include/pcl-1.11',
        '/usr/local/include/pcl-1.10',
        '/usr/local/include/pcl-1.9',
        '/usr/local/include/pcl-1.8',
    ]
    for path in common_paths:
        if os.path.exists(os.path.join(path, 'pcl/point_cloud.h')):
            return path

    raise RuntimeError("Could not find PCL include directory. Please install PCL or specify its path.")

# Get PCL include directory
pcl_include_dir = get_pcl_include_dir()

# Define the extension module
ext_modules = [
    Extension(
        'ndt_omp',
        [
            'bindings/ndt_omp_binding.cpp',
            'src/pclomp/ndt_omp.cpp',              
            'src/pclomp/voxel_grid_covariance_omp.cpp' 
        ],
        include_dirs=[
            pybind11.get_include(), 
            'include',              
            pcl_include_dir,         
            '/usr/include/eigen3',   
        ],
        library_dirs=[
            '/usr/lib/x86_64-linux-gnu',  
            '/usr/local/lib',             
        ],
        libraries=['pcl_common', 'pcl_registration', 'pcl_kdtree', 'pcl_filters', 'gomp'],
        extra_compile_args=['-std=c++17','-O3', '-fopenmp'],
        extra_link_args=['-fopenmp'],
        language='c++'
    ),
]

setup(
    name='ndt_omp',
    version='0.1',
    description='Python bindings for ndt_omp',
    ext_modules=ext_modules,
)