# CMake generated Testfile for 
# Source directory: /home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test
# Build directory: /home/vboxuser/catkin_mqp/build/gazebo_ros_pkgs/gazebo_ros/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_gazebo_ros_rostest_test_ros_network_ros_network_default.test "/home/vboxuser/catkin_mqp/build/catkin_generated/env_cached.sh" "/usr/bin/python3" "/opt/ros/noetic/share/catkin/cmake/test/run_tests.py" "/home/vboxuser/catkin_mqp/build/test_results/gazebo_ros/rostest-test_ros_network_ros_network_default.xml" "--return-code" "/usr/bin/python3 /opt/ros/noetic/share/rostest/cmake/../../../bin/rostest --pkgdir=/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros --package=gazebo_ros --results-filename test_ros_network_ros_network_default.xml --results-base-dir \"/home/vboxuser/catkin_mqp/build/test_results\" /home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/ros_network/ros_network_default.test ")
set_tests_properties(_ctest_gazebo_ros_rostest_test_ros_network_ros_network_default.test PROPERTIES  _BACKTRACE_TRIPLES "/opt/ros/noetic/share/catkin/cmake/test/tests.cmake;160;add_test;/opt/ros/noetic/share/rostest/cmake/rostest-extras.cmake;52;catkin_run_tests_target;/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/CMakeLists.txt;11;add_rostest;/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/CMakeLists.txt;0;")
add_test(check_ros_network/ros_network_default.test "rosrun" "rosunit" "check_test_ran.py" "--rostest" "/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/ros_network/ros_network_default.test")
set_tests_properties(check_ros_network/ros_network_default.test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/CMakeLists.txt;14;add_test;/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/CMakeLists.txt;0;")
add_test(_ctest_gazebo_ros_rostest_test_ros_network_ros_network_disabled.test "/home/vboxuser/catkin_mqp/build/catkin_generated/env_cached.sh" "/usr/bin/python3" "/opt/ros/noetic/share/catkin/cmake/test/run_tests.py" "/home/vboxuser/catkin_mqp/build/test_results/gazebo_ros/rostest-test_ros_network_ros_network_disabled.xml" "--return-code" "/usr/bin/python3 /opt/ros/noetic/share/rostest/cmake/../../../bin/rostest --pkgdir=/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros --package=gazebo_ros --results-filename test_ros_network_ros_network_disabled.xml --results-base-dir \"/home/vboxuser/catkin_mqp/build/test_results\" /home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/ros_network/ros_network_disabled.test ")
set_tests_properties(_ctest_gazebo_ros_rostest_test_ros_network_ros_network_disabled.test PROPERTIES  _BACKTRACE_TRIPLES "/opt/ros/noetic/share/catkin/cmake/test/tests.cmake;160;add_test;/opt/ros/noetic/share/rostest/cmake/rostest-extras.cmake;52;catkin_run_tests_target;/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/CMakeLists.txt;11;add_rostest;/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/CMakeLists.txt;0;")
add_test(check_ros_network/ros_network_disabled.test "rosrun" "rosunit" "check_test_ran.py" "--rostest" "/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/ros_network/ros_network_disabled.test")
set_tests_properties(check_ros_network/ros_network_disabled.test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/CMakeLists.txt;14;add_test;/home/vboxuser/catkin_mqp/src/gazebo_ros_pkgs/gazebo_ros/test/CMakeLists.txt;0;")
