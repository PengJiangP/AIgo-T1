import os
import launch
import launch_ros

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node


def generate_launch_description():

    odom_frame = DeclareLaunchArgument('odom_frame',  default_value='odom')
    base_frame = DeclareLaunchArgument('base_frame',  default_value='base_link')
    topic_cam0 = DeclareLaunchArgument('topic_cam0',  default_value='/cam1')
    topic_cam1 = DeclareLaunchArgument('topic_cam1',  default_value='/cam0')
    topic_imu = DeclareLaunchArgument('topic_imu',  default_value='/imu0')
    ref_B = DeclareLaunchArgument('ref_B',  default_value='031.1669249')
    ref_L = DeclareLaunchArgument('ref_L',  default_value='121.2888731')
    ref_H = DeclareLaunchArgument('ref_H',  default_value='0.0')
    config_pathname = DeclareLaunchArgument('config_pathname', default_value='./install/algot1_base/lib/algot1_base/algo-algot1.json')  
    
    # 启动 algo_algot1_node
    algo_algot1_node = launch_ros.actions.Node(
        package='algot1_base',
        executable='algo_algot1_node',
        output='screen',
        emulate_tty=True,
        parameters=[{
                'config_pathname': launch.substitutions.LaunchConfiguration('config_pathname'),
                'odom_frame': launch.substitutions.LaunchConfiguration('odom_frame'),
                'base_frame': launch.substitutions.LaunchConfiguration('base_frame'),
                'topic_cam0': launch.substitutions.LaunchConfiguration('topic_cam0'),
                'topic_cam1': launch.substitutions.LaunchConfiguration('topic_cam1'),
                'topic_imu': launch.substitutions.LaunchConfiguration('topic_imu'),
                'ref_B': launch.substitutions.LaunchConfiguration('ref_B'),
                'ref_L': launch.substitutions.LaunchConfiguration('ref_L'),
                'ref_H': launch.substitutions.LaunchConfiguration('ref_H'),
        }])
    
    # # 启动rviz2
    pkg_description = get_package_share_directory('algot1_base')
    rviz_config_path = os.path.join(pkg_description,'rviz', 'algo-algot1-rviz2.rviz')
    rviz_node =  launch_ros.actions.Node(
        package="rviz2",
        executable="rviz2",
        name="rviz_node",
        output="screen",
        arguments=['-d', rviz_config_path]
    )

    return LaunchDescription([
        config_pathname,
        odom_frame,
        base_frame,
        topic_cam0,
        topic_cam1,
        topic_imu,
        ref_B,
        ref_L,
        ref_H,
        
        algo_algot1_node,
        rviz_node
    ])
