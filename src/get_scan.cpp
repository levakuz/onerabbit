#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include "AMQPcpp.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


using namespace std;
using json = nlohmann::json;

void recive(const sensor_msgs::LaserScan& msg){


  AMQP amqp("lev:lev@192.168.1.35:5672");
  AMQPExchange * ex = amqp.createExchange("ros");
  ex->Declare("ros", "topic");
  ex->setHeader("Delivery-mode", 1);
  ex->setHeader("Content-type", "text/text");
  ex->setHeader("Content-encoding", "UTF-8"); 
  AMQPQueue * qu2 = amqp.createQueue("laser_data");
  qu2->Declare();
  qu2->Bind( "ros", "");
  int flag = 1;
  if (flag == 1){	

  float angle_min = msg.angle_min;
  float angle_max = msg.angle_max;
  float time_increment = msg.time_increment;
  float scan_time = msg.scan_time;
  float range_min = msg.range_min;
  float range_max = msg.range_max;
  
  cout<< "angle_min: ";
  cout<< msg.angle_min<<endl;
  cout<< "angle_max: ";
  cout<< msg.angle_max<<endl;
  cout<< "time_increment: ";
  cout<< msg.time_increment<<endl;
  cout<< "scan_time: ";
  cout<< msg.scan_time<<endl;
  cout<< "range_min: ";
  cout<< msg.range_min<<endl;
  cout<< "range_max: ";
  cout<< msg.range_max<<endl;

  

  json info;
  info["angle_min"] = to_string(angle_min);
  info["angle_max"] = to_string(angle_max);
  info["time_increment"] = to_string(time_increment);
  info["scan_time"] = to_string(scan_time);
  info["range_min"] = to_string(range_min);
  info["range_max"] = to_string(range_max);
  
  
  string str_info = info.dump();
  ex->Publish(str_info, "");
  flag = 2;
  }

  json data;
  for(int i=0;i<1080;i++){

    data[i] = msg.ranges[i];   
  };
    string str_data = data.dump();
    ex->Publish(str_data,"");
}

int main(int argc, char** argv)
{      
 ros::init(argc, argv, "load_ogm");
 ros::NodeHandle nh;
 ros::Subscriber sub = nh.subscribe("/scan", 1000, recive);
 ros::spinOnce();
 return 0;
}
