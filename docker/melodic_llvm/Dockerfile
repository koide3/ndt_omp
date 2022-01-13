FROM ros:melodic

RUN apt-get update && apt-get install -y --no-install-recommends \
  wget nano build-essential libomp-dev clang lld\
  ros-melodic-geodesy ros-melodic-pcl-ros ros-melodic-nmea-msgs \
  ros-melodic-rviz ros-melodic-tf-conversions ros-melodic-libg2o \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /root/catkin_ws/src
WORKDIR /root/catkin_ws/src
RUN /bin/bash -c '. /opt/ros/melodic/setup.bash; catkin_init_workspace'
RUN git config --global http.postBuffer 2M
RUN git clone https://github.com/SMRT-AIST/fast_gicp.git --recursive --depth=1
RUN git clone https://github.com/koide3/hdl_graph_slam.git --depth=1

COPY . /root/catkin_ws/src/ndt_omp/

RUN update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld 50

WORKDIR /root/catkin_ws
RUN /bin/bash -c '. /opt/ros/melodic/setup.bash; CC=clang CXX=clang++ catkin_make'
RUN sed -i "6i source \"/root/catkin_ws/devel/setup.bash\"" /ros_entrypoint.sh

WORKDIR /

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
