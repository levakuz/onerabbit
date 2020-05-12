#include "ros/ros.h"
#include <tf/transform_listener.h>
#include <costmap_2d/costmap_2d_ros.h>
#include "nav_msgs/OccupancyGrid.h"
#include "std_msgs/String.h"
#include "nav_msgs/OccupancyGrid.h"
#include <iostream>
#include <fstream>
#include "AMQPcpp.h"
using namespace std;
int main(int argc, char** argv){
    ros::init(argc,argv,"onerabbit");
    ros::NodeHandle n;
    tf::TransformListener listener;
    ros::Rate rate(10.0);
    tf::StampedTransform transform;
    AMQP amqp("test:test@localhost:5672");
     AMQPExchange * ex = amqp.createExchange("map");
     ex->Declare("map", "topic");

     AMQPQueue * qu2 = amqp.createQueue("map");
     qu2->Declare();
     qu2->Bind( "map", "");
     ex->setHeader("Delivery-mode", 2);
     ex->setHeader("Content-type", "text/text");
     ex->setHeader("Content-encoding", "UTF-8");
while(n.ok()){
    try{
     
    string b = to_string(listener.waitForTransform("map","base_link",ros::Time(0),ros::Duration(10.0)));


    ex->Publish(b,"");
    
    listener.lookupTransform("map","base_link",ros::Time(0), transform);
    transform.getOrigin().data();
    ROS_INFO("B=%d", b);
   }catch(tf::TransformException exec){
    ROS_ERROR("%s",exec.what());
        }
    rate.sleep();
     }
   return 0;
};
     
	    


  
    
				   	




