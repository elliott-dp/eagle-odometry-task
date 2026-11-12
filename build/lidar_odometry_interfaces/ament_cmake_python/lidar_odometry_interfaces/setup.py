from setuptools import find_packages
from setuptools import setup

setup(
    name='lidar_odometry_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('lidar_odometry_interfaces', 'lidar_odometry_interfaces.*')),
)
