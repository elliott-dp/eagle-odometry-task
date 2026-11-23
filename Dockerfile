FROM ros:humble

ENV ROS_DISTRO=humble \
    ROS_VERSION=2 \
    ROS_PYTHON_VERSION=3 \
    AMENT_PREFIX_PATH=/opt/ros/humble \
    CMAKE_PREFIX_PATH=/opt/ros/humble \
    PATH=/opt/ros/humble/bin:$PATH \
    LD_LIBRARY_PATH=/opt/ros/humble/lib:/opt/ros/humble/lib/aarch64-linux-gnu \
    PYTHONPATH=/opt/ros/humble/local/lib/python3.10/dist-packages:/opt/ros/humble/lib/python3.10/site-packages

ARG DEBIAN_FRONTEND=noninteractive
ARG USERNAME=eagle
ARG USER_UID=1000
ARG USER_GID=1000

RUN apt-get update && apt-get install -y --no-install-recommends \
        ros-humble-desktop \
        build-essential cmake ninja-build gdb git clang-format \
        python3-pip python3-colcon-common-extensions python3-rosdep \
        libpcl-dev libeigen3-dev \
        ros-humble-pcl-conversions \
        ros-humble-pcl-ros \
        ros-humble-tf2-eigen \
        ros-humble-tf2-sensor-msgs \
        ros-humble-tf2-tools \
        ros-humble-rqt-graph \
        ros-humble-rqt-reconfigure \
        xserver-xephyr \
        sudo \
    && rm -rf /var/lib/apt/lists/*

RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m -s /bin/bash ${USERNAME} \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${USERNAME} \
    && echo "source /opt/ros/humble/setup.bash" >> /home/${USERNAME}/.bashrc

USER ${USERNAME}
WORKDIR /home/${USERNAME}/ws
CMD ["bash"]