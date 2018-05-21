#include <ros/ros.h>
#include <can_msgs/Frame.h>

#include <steering_monitor/Steering.h>
#include <cmath>

class subAndPub
{
public:

  //SubAndPub constructor definition
  subAndPub()
  {         
    //set up.
    pubSteering = nh.advertise<steering_monitor::Steering>("steering_angle", 10);
    steering_sensor.header.frame_id = "/controls";

    //Subscribe to whatever topic is publishing the CAN frames
    //if you use 'rosrun socketcan_bridge socketcan_to_topic' that is 'received_messages'
    sub = nh.subscribe("received_messages", 10, &subAndPub::callback, this);

  }

  void callback(const can_msgs::Frame& input)
  {
    ros::Rate publish_rate(100);   

    if (input.id == 342)
    {
      steering_sensor.header.stamp = ros::Time::now();
      steering_sensor.angle      = float(((int16_t)((input.data[0] << 8) + input.data[1])) * -0.1 );
      steering_sensor.angle_rate = float(((int16_t)((input.data[2] << 8) + input.data[3])));

      std::cout << "Steering angle:  "      << steering_sensor.angle      << std::endl;
      std::cout << "Steering angle_rate:  " << steering_sensor.angle_rate << std::endl;
      pubSteering.publish(steering_sensor);
      publish_rate.sleep();
    }
  }

private:
  ros::NodeHandle nh; 
  ros::Publisher  pubSteering;
  ros::Subscriber sub; 
  steering_monitor::Steering steering_sensor;

};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sub_n_pub");
  subAndPub mydood;
  ros::spin();
  return 0;

}   
