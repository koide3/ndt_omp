FROM ros:galactic

RUN apt-get update && apt-get install -y --no-install-recommends \
  wget nano build-essential libomp-dev clang lld git\
  ros-galactic-geodesy ros-galactic-pcl-ros ros-galactic-nmea-msgs \
  ros-galactic-rviz2 ros-galactic-libg2o \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /root/colcon_ws/src
WORKDIR /root/colcon_ws/src
# Comment it out until the repository supports ROS2 builds.
# RUN git clone https://github.com/SMRT-AIST/fast_gicp.git --recursive --depth=1
# RUN git clone https://github.com/koide3/hdl_graph_slam.git --depth=1

COPY . /root/colcon_ws/src/ndt_omp/

WORKDIR /root/colcon_ws
RUN /bin/bash -c '. /opt/ros/galactic/setup.bash; colcon build'
RUN sed -i "6i source \"/root/colcon_ws/devel/setup.bash\"" /ros_entrypoint.sh

WORKDIR /

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
