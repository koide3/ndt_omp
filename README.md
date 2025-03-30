# ndt_omp

This package provides an OpenMP-boosted Normal Distributions Transform (and GICP) algorithm derived from PCL. The NDT algorithm is modified to be SSE-friendly and multi-threaded, achieving up to 10 times faster performance than the original PCL version.

For using this package in non-ROS1 projects (ROS2 or without ROS), see forked repositories: [dfki-ric/pclomp](https://github.com/dfki-ric/pclomp), [tier4/ndt_omp](https://github.com/tier4/ndt_omp).

[![Build](https://github.com/koide3/ndt_omp/actions/workflows/build.yml/badge.svg)](https://github.com/koide3/ndt_omp/actions/workflows/build.yml)

## Benchmark (on Core i7-6700K)
```
$ roscd ndt_omp/data
$ rosrun ndt_omp align 251370668.pcd 251371071.pcd
--- pcl::NDT ---
single : 282.222[msec]
10times: 2921.92[msec]
fitness: 0.213937

--- pclomp::NDT (KDTREE, 1 threads) ---
single : 207.697[msec]
10times: 2059.19[msec]
fitness: 0.213937

--- pclomp::NDT (DIRECT7, 1 threads) ---
single : 139.433[msec]
10times: 1356.79[msec]
fitness: 0.214205

--- pclomp::NDT (DIRECT1, 1 threads) ---
single : 34.6418[msec]
10times: 317.03[msec]
fitness: 0.208511

--- pclomp::NDT (KDTREE, 8 threads) ---
single : 54.9903[msec]
10times: 500.51[msec]
fitness: 0.213937

--- pclomp::NDT (DIRECT7, 8 threads) ---
single : 63.1442[msec]
10times: 343.336[msec]
fitness: 0.214205

--- pclomp::NDT (DIRECT1, 8 threads) ---
single : 17.2353[msec]
10times: 100.025[msec]
fitness: 0.208511
```

Several methods for neighbor voxel search are implemented. If you select `pclomp::KDTREE`, results will be identical to the original `pcl::NDT`. We recommend using `pclomp::DIRECT7` for a balance of speed and stability. For extremely fast registration, choose `pclomp::DIRECT1`, though it may be less stable.

<img src="data/screenshot.png" height="400px" /><br>
Red: target, Green: source, Blue: aligned

## Python Bindings

This repository now includes Python bindings for `ndt_omp`, enabling its use in Python projects.

### Installation
To install the Python bindings, ensure you have the following dependencies:
- PCL (any version 1.8+)
- Eigen3
- pybind11 (`pip3 install pybind11`)
- Python 3.8+ (other versions may work but are untested)

Clone the repository and install:
```bash
git clone https://github.com/ali-rehman-ML/py_ndt_omp.git
cd ndt_omp
sudo python3 setup.py install
```

The `setup.py` script dynamically detects your PCL installation, making it compatible with various PCL versions (e.g., 1.8, 1.9, 1.10+).

### Usage
Here’s an example of using the Python bindings with Open3D:
```python
import ndt_omp
import numpy as np
import open3d as o3d #always import open3d after ndt_omp to avoid segmentation fault


# Load point clouds
pcd1 = o3d.io.read_point_cloud("map1.pcd")
pcd2 = o3d.io.read_point_cloud("map2.pcd")
source = np.asarray(pcd1.points).astype(np.float32)
target = np.asarray(pcd2.points).astype(np.float32)

# Initialize NDT-OMP
ndt = ndt_omp.NDTOMP()
ndt.set_num_threads(4)
ndt.set_transformation_epsilon(0.001)
ndt.set_maximum_iterations(100)
ndt.set_neighborhood_search_method(ndt_omp.NeighborSearchMethod.DIRECT7)  
ndt.set_resolution(1.0)

# Set input clouds and align
ndt.set_input_source(source)
ndt.set_input_target(target)
init_guess = np.eye(4).astype(np.float32)
transform, fitness = ndt.align(init_guess)

print("Transformation Matrix:\n", transform)
print("Fitness Score:", fitness)
```

#### Notes
- **Open3D Compatibility**: Importing `open3d` before `ndt_omp` result in segmentaion fault if your open3d and pcl has different flann versions. You will also not be able to use open3d voxel downsampling and SOR filtering if flann version is mis-matched.


### Contributing
The Python bindings were added to enhance accessibility and compatibility. Contributions to improve the bindings, such as adding more PCL features or optimizing performance, are welcome!

## Related Packages
- [ndt_omp](https://github.com/koide3/ndt_omp)
- [fast_gicp](https://github.com/SMRT-AIST/fast_gicp)
