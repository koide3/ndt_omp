FROM ros:foxy

RUN apt-get update && apt-get install -y --no-install-recommends \
  wget nano build-essential libomp-dev clang lld git\
  ros-foxy-geodesy ros-foxy-pcl-ros ros-foxy-nmea-msgs \
  ros-foxy-rviz2 ros-foxy-libg2o \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /root/colcon_ws/src
WORKDIR /root/colcon_ws/src
# Comment it out until the repository supports ROS2 builds.
# RUN git clone https://github.com/SMRT-AIST/fast_gicp.git --recursive --depth=1
# RUN git clone https://github.com/koide3/hdl_graph_slam.git --depth=1

COPY . /root/colcon_ws/src/ndt_omp/

RUN update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld 50

WORKDIR /root/colcon_ws
RUN /bin/bash -c '. /opt/ros/foxy/setup.bash; CC=clang CXX=clang++ colcon build'
RUN sed -i "6i source \"/root/colcon_ws/devel/setup.bash\"" /ros_entrypoint.sh

WORKDIR /

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
