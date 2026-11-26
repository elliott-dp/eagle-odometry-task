"""Runs the lidar odometry, and optionally plays a bag into it and opens RViz.

    ros2 launch lidar_odometry odometry.launch.py bag:=/path/to/bag_folder
    ros2 launch lidar_odometry odometry.launch.py bag:=/path/to/bag_folder rate:=0.5 rviz:=false
    ros2 launch lidar_odometry odometry.launch.py config:=/path/to/my_odometry.yaml bag:=...
    ros2 launch lidar_odometry odometry.launch.py use_sim_time:=false   # live sensor, no bag

The bag needs a sensor_msgs/PointCloud2 topic (input_topic in the config, /lidar/points by
default) and a static TF from base_frame (base_link) to the frame of the clouds.
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node


def generate_launch_description():
    share = get_package_share_directory('lidar_odometry')
    bag = LaunchConfiguration('bag')
    use_sim_time = LaunchConfiguration('use_sim_time')

    arguments = [
        DeclareLaunchArgument(
            'bag', default_value='',
            description='Bag folder to play into the node. Empty: only start the node.'),
        DeclareLaunchArgument(
            'rate', default_value='1.0',
            description='Playback speed. A Debug build needs about 0.05 to keep up.'),
        DeclareLaunchArgument(
            'rviz', default_value='true', description='Open RViz.'),
        DeclareLaunchArgument(
            'config', default_value=os.path.join(share, 'config', 'odometry.yaml'),
            description='Parameter file of the node.'),
        DeclareLaunchArgument(
            'use_sim_time', default_value='true',
            description='Follow the clock of the bag. false for a live sensor.'),
    ]

    odometry = Node(
        package='lidar_odometry',
        executable='odometry_node',
        name='lidar_odometry',
        output='screen',
        parameters=[LaunchConfiguration('config'), {'use_sim_time': use_sim_time}],
    )

    # The 2 s delay lets the player and the node find each other before the first scan:
    # a scan sent before that is lost, and the odometry starts from a gap.
    player = ExecuteProcess(
        cmd=['ros2', 'bag', 'play', bag, '--clock', '--delay', '2',
             '--rate', LaunchConfiguration('rate'),
             '--qos-profile-overrides-path', os.path.join(share, 'config', 'bag_qos.yaml')],
        output='screen',
        condition=IfCondition(PythonExpression(["'", bag, "' != ''"])),
    )

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', os.path.join(share, 'rviz', 'odometry.rviz')],
        parameters=[{'use_sim_time': use_sim_time}],
        output='log',
        condition=IfCondition(LaunchConfiguration('rviz')),
    )

    return LaunchDescription(arguments + [odometry, player, rviz])
