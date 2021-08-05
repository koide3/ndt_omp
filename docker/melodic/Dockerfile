FROM ros:melodic

RUN apt-get update && apt-get install -y --no-install-recommends \
  && apt-get install -y --no-install-recommends \
  wget nano build-essential libomp-dev clang lld\
  ros-melodic-geodesy ros-melodic-pcl-ros ros-melodic-nmea-msgs \
  ros-melodic-rviz ros-melodic-tf-conversions ros-melodic-libg2o \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /root/catkin_ws/src
WORKDIR /root/catkin_ws/src
RUN /bin/bash -c '. /opt/ros/melodic/setup.bash; catkin_init_workspace'
RUN git clone https://github.com/SMRT-AIST/fast_gicp.git --recursive
RUN git clone https://github.com/koide3/hdl_graph_slam.git

COPY . /root/catkin_ws/src/ndt_omp/

WORKDIR /root/catkin_ws
RUN /bin/bash -c '. /opt/ros/melodic/setup.bash; catkin_make'
RUN sed -i "6i source \"/root/catkin_ws/devel/setup.bash\"" /ros_entrypoint.sh

WORKDIR /

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
