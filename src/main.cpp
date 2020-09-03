#include "ros/ros.h"
#include <tf/transform_listener.h>
#include "AMQPcpp.h"
#include "std_msgs/String.h"
#include "nav_msgs/OccupancyGrid.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


using namespace std;
using json = nlohmann::json;

int main (int argc, char** argv) {
    ros::init(argc, argv, "onerabbit");	
    ros::NodeHandle n;
    tf::TransformListener listener;
    //ros::Rate rate(1.0);
    tf::StampedTransform transform;

    AMQP amqp("admin:admin@95.181.230.223:5672");
    AMQPExchange * ex = amqp.createExchange("ros");
    ex->Declare("ros", "topic");

    AMQPQueue * qu2 = amqp.createQueue("coordinates");
    qu2->Declare();
    qu2->Bind( "ros", "");
    ex->setHeader("Delivery-mode", 1);
    ex->setHeader("Content-type", "text/text");
    ex->setHeader("Content-encoding", "UTF-8");
    
    float old_x = 1;
    float old_y = 1;
    float x;
    float y;
    json info;

    
    while (n.ok())
    { 
        

        try{
	    

            listener.lookupTransform("/map","/base_link",ros::Time(0), transform);
 
            ROS_INFO("Got a transform! x = %f, y = %f",transform.getOrigin().x(),transform.getOrigin().y());
	    
	    string id = "id.1,";
	    x = transform.getOrigin().x();
	    y = transform.getOrigin().y();

            if(abs(old_x-x) >=0.01 || abs(old_y-y) >=0.01){
	      string str_x = to_string(x);
	      string str_y = to_string(y);

	      old_x = x;
	      old_y = y;
	      info["id"]= "1";
	      info["x"] = str_x;
              info ["y"] = str_y;
              string str_info = info.dump();
   	      cout<<(str_info);
	   
              string coordinates = id + "x." + str_x + "," + "y." + str_y;
	      ex->Publish(str_info, ""); 
	    }

  	    
	    //string coordinates = i + "x." + str_x + "," + "y." + str_y;
	    //ex->Publish(coordinates, ""); 
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
