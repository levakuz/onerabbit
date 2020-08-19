#include "ros/ros.h"
#include <tf/transform_listener.h>
#include "AMQPcpp.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


using namespace std;
using json = nlohmann::json;

int main (int argc, char** argv) {
    ros::init(argc, argv, "onerabbit");	
    ros::NodeHandle n;

    AMQP amqp("lev:lev@192.168.1.35:5672");
    AMQPExchange * ex = amqp.createExchange("ros");
    ex->Declare("ros", "topic");

    AMQPQueue * qu2 = amqp.createQueue("battery_charge_info");
    qu2->Declare();
    qu2->Bind( "ros", "");
    ex->setHeader("Delivery-mode", 1);
    ex->setHeader("Content-type", "text/text");
    ex->setHeader("Content-encoding", "UTF-8");
    
    float charge = 100;
    json info;

    
    while (n.ok())
    { 

	    

 	if(charge!=0){
	    
	      string id = "id.1,";
	      --charge;

	      info["id"]= "1";
	      info["charge"] = charge;
              string str_info = info.dump();
   	      cout<<(str_info);
	   
	      ex->Publish(str_info, ""); 
	 }
	else{
	  
          charge = 100;
	  info["id"]= "1";
	  info["charge"] = charge;
          string str_info = info.dump();
   	  cout<<(str_info);
	   
	  ex->Publish(str_info, ""); 

	}     

  	    
        


              ros::Duration(1).sleep();
            }
    
    
				   	

	
	return 0;

}
