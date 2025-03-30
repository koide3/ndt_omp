#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pclomp/ndt_omp.h>
#include <flann/flann.hpp>

namespace py = pybind11;

#include <pcl/console/print.h>

pcl::PointCloud<pcl::PointXYZ>::Ptr numpy_to_point_cloud(const Eigen::MatrixXf& points) {
    pcl::console::setVerbosityLevel(pcl::console::L_DEBUG);
    if (points.cols() != 3) {
        throw std::invalid_argument("Input array must have shape (N, 3)");
    }
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    try {
        cloud->points.resize(points.rows());
        for (int i = 0; i < points.rows(); ++i) {
            cloud->points[i].x = points(i, 0);
            cloud->points[i].y = points(i, 1);
            cloud->points[i].z = points(i, 2);
            if (i < 5 || i >= points.rows() - 5) {  // Log first and last 5 points
            }
        }
        cloud->width = points.rows();
        cloud->height = 1;
        cloud->is_dense = true;

    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to convert NumPy array to PCL PointCloud: " + std::string(e.what()));
    }
    return cloud;
}

void bind_ndt_omp(py::module& m) {
    PCL_DEBUG("ndt_omp binding initialized with FLANN version: %s\n", FLANN_VERSION_);
    py::class_<pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>>(m, "NDTOMP")
        .def(py::init<>())
        .def("set_num_threads", &pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>::setNumThreads)
        .def("set_neighborhood_search_method", &pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>::setNeighborhoodSearchMethod)
        .def("set_resolution", &pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>::setResolution)
        .def("set_transformation_epsilon", &pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>::setTransformationEpsilon)
        .def("set_maximum_iterations", &pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>::setMaximumIterations)
        .def("set_input_source", [](pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>& ndt, 
                                   const Eigen::MatrixXf& source) {
            auto source_cloud = numpy_to_point_cloud(source);
            ndt.setInputSource(source_cloud);
        }, py::arg("source"))
        .def("set_input_target", [](pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>& ndt, 
                                   const Eigen::MatrixXf& target) {
            auto target_cloud = numpy_to_point_cloud(target);
            ndt.setInputTarget(target_cloud);
        }, py::arg("target"))
        .def("align", [](pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>& ndt, 
                        const Eigen::Matrix4f& guess) {
            try {
                pcl::PointCloud<pcl::PointXYZ> output;
                ndt.align(output, guess);
                return std::make_tuple(ndt.getFinalTransformation(), ndt.getFitnessScore());
            } catch (const std::exception& e) {
                throw std::runtime_error("NDT alignment failed: " + std::string(e.what()));
            }
        }, py::arg("guess") = Eigen::Matrix4f::Identity())
        .def("get_final_transformation", &pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>::getFinalTransformation)
        .def("has_converged", &pclomp::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>::hasConverged);

    py::enum_<pclomp::NeighborSearchMethod>(m, "NeighborSearchMethod")
        .value("KDTREE", pclomp::NeighborSearchMethod::KDTREE)
        .value("DIRECT7", pclomp::NeighborSearchMethod::DIRECT7)
        .value("DIRECT1", pclomp::NeighborSearchMethod::DIRECT1)
        .export_values();
}

PYBIND11_MODULE(ndt_omp, m) {
    m.doc() = "Python bindings for ndt_omp package with NumPy support";
    bind_ndt_omp(m);
}