#include <ros/ros.h>
#include <can_msgs/Frame.h>
#include <std_msgs/Float32.h>
#include <cmath>

class subAndPub
{
public:

  //SubAndPub constructor definition
  subAndPub()
  {        
    //set up.
    pubSteering = nh.advertise<std_msgs::Float32>("steering_angle", 10);

    //Subscribe to whatever topic is publishing the CAN frames
    //if you use 'rosrun socketcan_bridge socketcan_to_topic' that is 'received_messages'
    sub = nh.subscribe("received_messages", 10, &subAndPub::callback, this);

  }

  void callback(const can_msgs::Frame& input)
  {
    ros::Rate publish_rate(10);   

    if (input.id == 342)
    {
      steering_angle.data = float(   ((int16_t)((input.data[0] << 8) + input.data[1])) * -0.1 );

      std::cout << "Steering angle:  " << steering_angle.data << std::endl;

        pubSteering.publish(steering_angle);
        publish_rate.sleep();
    }
  }

private:
  ros::NodeHandle nh; 
  ros::Publisher  pubSteering;
  ros::Subscriber sub; 
  std_msgs::Float32 steering_angle;

};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sub_n_pub");
  subAndPub mydood;
  ros::spin();
  return 0;

}   
