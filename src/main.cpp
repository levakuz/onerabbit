#include "ros/ros.h"
#include <tf/transform_listener.h>
#include "AMQPcpp.h"
#include "std_msgs/String.h"
#include "nav_msgs/OccupancyGrid.h"
#include <iostream>
#include <fstream>
using namespace std;


int main (int argc, char** argv) {
    ros::init(argc, argv, "onerabbit");	
    ros::NodeHandle n;
    tf::TransformListener listener;
    //ros::Rate rate(1.0);
    tf::StampedTransform transform;

    AMQP amqp("test:test@localhost:5672");
    AMQPExchange * ex = amqp.createExchange("ros");
    ex->Declare("ros", "topic");

    AMQPQueue * qu2 = amqp.createQueue("ros");
    qu2->Declare();
    qu2->Bind( "ros", "");
    ex->setHeader("Delivery-mode", 2);
    ex->setHeader("Content-type", "text/text");
    ex->setHeader("Content-encoding", "UTF-8");

    
    while (n.ok())
    { 
        tf::StampedTransform transform;

        try{

            listener.lookupTransform("/map","/base_link",ros::Time(0), transform);
 
            ROS_INFO("Got a transform! x = %f, y = %f",transform.getOrigin().x(),transform.getOrigin().y());
	    string str_x = to_string(transform.getOrigin().x());
	    string str_y = to_string(transform.getOrigin().y());			
	    ex->Publish(str_x, ""); 
	    ex->Publish(str_y, ""); 
            //nav_msgs::OccupancyGrid = ros::topic::waitForMessage<nav_msgs::OccupancyGrid>("/map");
            boost::shared_ptr<nav_msgs::OccupancyGrid const> sharedPtr;
            sharedPtr  = ros::topic::waitForMessage<nav_msgs::OccupancyGrid>("/map", ros::Duration(10));

            }
	    
        catch (tf::TransformException ex)
            {
            ROS_ERROR("Nope! %s", ex.what());
            } 


        //rate.sleep();
            }
    
    
				   	

	
	return 0;

}
