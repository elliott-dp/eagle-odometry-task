import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('lidar_odometry'), 'config', 'odometry.yaml')

    return LaunchDescription([
        Node(
            package='lidar_odometry',
            executable='odometry_node',
            name='lidar_odometry',
            output='screen',
            parameters=[config],
        ),
    ])
