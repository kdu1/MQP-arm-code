/*#include <ros/ros.h>
#include "Robot.h" //pray the red lines are just there
#include <memory>
#include <sensor_msgs/JointState.h>

//this will now be the "arm" class like in the example code I found
class ROSWrapperArm 
{
private:
    std::unique_ptr<Robot> robot;
    ros::Subscriber write_subscriber_;
    ros::Subscriber read_subscriber_;

    //do we need this or do we only publish when it calls write
    ros::Timer current_write_timer_;
    double publish_current_write_frequency_;

//okay nvm so it has the hiddevice(easyhid) and write stuff in a seperate file (controller) from the one doing the rossing (arm)
//seems like instead of doing a wrapper it needs to be in the code
//and publish JointState messages
//should I test this with gazebo or is there even time

public:
    ROSWrapperArm()
    {
        //publish coords from servo_jp
        //also is measured_js supposed to be a thing or does it not line up with the example
        _measured_js_publisher = rospy.Publisher(ns + 'measured_js',
                                                      sensor_msgs.msg.JointState,
                                                      queue_size = 10)


        //call write?
        read_subscriber_ = nh->subscribe(
            "read", 10, &ROSWrapperArm::callbackRead, this);
    }

    void stop()
    {
        //motor->stop();
    }

    void callbackRead(const std_msgs::Int32 &msg)
    {
        robot->read()//int id, std::vector<Complex> values idk how send this
        //this also doesn't return the actual readed thing, just the success
        //maybe I could modify it so it returns the readed thing or returns it from somewhere
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ros_driver");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(4);
    spinner.start();

    ROSWrapperArm ros_driver_wrapper(&nh);
    ROS_INFO("ROS driver is now started");
    
    ros::waitForShutdown();
    ros_driver_wrapper.stop();
}*/