#include "ros/ros.h"
#include "std_msgs/String.h"
#include <tf/transform_listener.h>
#include <iostream>
#include <fstream>


int main(int argc, char **argv)
{

  ros::init(argc, argv, "PoseUpdate");


  ros::NodeHandle n;


  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  tf::TransformListener listener;

  ros::Rate rate(1.0);
  std::ofstream myfile;
  myfile.open("/tmp/OUTPUTXY.txt");

  while (n.ok())
  {
    tf::StampedTransform transform;
    try
    {
        //ROS_INFO("Attempting to read pose...");
        listener.lookupTransform("/map","/base_link",ros::Time(0), transform);

        ROS_INFO("Got a transform! x = %f, y = %f",transform.getOrigin().x(),transform.getOrigin().y());
        myfile << transform.getOrigin().x() << "," << transform.getOrigin().y() << "\n";
    }
    catch (tf::TransformException ex)
    {
        ROS_ERROR("Nope! %s", ex.what());
    } 


    rate.sleep();

  }
  myfile.close();
  ROS_ERROR("I DIED");
  return 0;
}
