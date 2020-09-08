#include <ros/ros.h>
#include <nav_msgs/GetMap.h>
#include "nav_msgs/OccupancyGrid.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "AMQPcpp.h"
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;
int rows;
int cols;
double mapResolution;
vector<vector<bool> > grid;
json info;



void recive(const nav_msgs::OccupancyGrid& msg){
  AMQP amqp("admin:admin@95.181.230.223:5672");
  AMQPExchange * ex = amqp.createExchange("map2");
  ex->Declare("map2", "fanout");
  ex->setHeader("Delivery-mode", 2);
  ex->setHeader("Content-type", "text/text");
  ex->setHeader("Content-encoding", "UTF-8"); 
  AMQPQueue * qu2 = amqp.createQueue("robot_map");
  qu2->Declare();
  qu2->Bind( "map2", "");	

     
  rows = msg.info.height;
  cols = msg.info.width;
  info["cols"] = to_string(cols);
  info ["rows"] = to_string(rows);
  mapResolution = msg.info.resolution;
  // Dynamically resize the grid
  grid.resize(rows);
  for (int i = 0; i < rows; i++) {
    grid[i].resize(cols);
  }
  int currCell = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (msg.data[currCell] == 0){ // unoccupied cell
        grid[i][j] = false;}
      else{
        grid[i][j] = true;} // occupied (100) or unknown cell (-1)
        currCell++;
      
    }  
  }
  printf("Grid map:\n");
     
  for (int i = 0; i < rows; i++)
  {
    printf("Row no. %d\n", i);
    //ex->Publish("Row." + to_string(i), "");
    for (int j = 0; j < cols; j++)
    {
      printf("%d ", grid[i][j] ? 1 : 0);
      //ex->Publish(to_string(grid[i][j] ? 1 : 0), "");
    }
      printf("\n");
  }

  for (int i = 0; i < rows; i++){
    //ex->Publish("Row." + to_string(i), "");
    info[to_string(i)] = grid[i];
  }
    string str_info = info.dump();
    ex->Publish(str_info, "");
    //ex->Publish("end", "");   
    ROS_INFO("Received a %d X %d map @ %.3f m/px\n",
      msg.info.width,
      msg.info.height,
      msg.info.resolution);
    grid.clear();
        
  return ;
}


int main(int argc, char** argv)
{      
 ros::init(argc, argv, "load_ogm");
 ros::NodeHandle nh;
 ros::Rate rate(10);
 ros::Subscriber sub = nh.subscribe("/map", 1000, recive);
 rate.sleep();
 ros::spinOnce();
 return 0;
}
