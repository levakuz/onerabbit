#include <actionlib/client/simple_action_client.h>
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "AMQPcpp.h"
#include <tf/transform_listener.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;


int distance_from_table = 0.5;


class Table{
  public:

    int x11;
    int x22;
    int y11;
    int y22;


    int x_first_point;
    int y_first_point;

    int x_second_point;
    int y_second_point;

    int x_third_point;
    int y_third_point;

    int x_fourth_point;
    int y_fourth_point;

    void calculate_first_point(int x11, int x22, int y11, int y22){
      x_first_point = x22 - x11;
      y_first_point = y11 + distance_from_table;
    }


    void calculate_second_point(int x11, int x22, int y11, int y22){
      x_second_point = x22 + distance_from_table;
      y_second_point = y11 - y22;
    }
    

    void calculate_third_point(int x11, int x22, int y11, int y22){
      x_third_point = x11 - x22;
      y_third_point = y22 - distance_from_table;
    }


    void calculate_fourth_point(int x11, int x22, int y11, int y22){
      x_fourth_point = x11 - distance_from_table;
      y_fourth_point = y11 - y22;
    }  

};



Table table[20];

typedef actionlib::SimpleActionClient<geometry_msgs::PoseStamped> Client;
int  onMessage( AMQPMessage * message) {


  json data = message->getMessage(&j);
  geometry_msgs::PoseStamped goal;
  num = data["table"]
  table[atoi(num)].x11 = data["x11"]
  table[atoi(num)].x22 = data["x22"]
  table[atoi(num)].y11 = data["y11"]
  table[atoi(num)].y22 = data["y22"]
  table[atoi(num)].calculate_first_point();
  table[atoi(num)].calculate_second_point();
  table[atoi(num)].calculate_third_point();
  table[atoi(num)].calculate_fourth_point();

  return 0; 
}

int  onMessage1( AMQPMessage * message) {
  
  Client client("go_to_position", false);
  client.waitForServer();
  
  geometry_msgs::PoseStamped goal;

  string data = message->getMessage(&j);
  
 
  goal.header.stamp=ros::Time::now();
	goal.header.frame_id="map";
  goal.pose.position.x=table[atoi(data)].x_first_point;
	goal.pose.position.y=table[atoi(data)].y_first_point;;
	goal.pose.position.z=0.0;
  client.sendGoal(goal);
  client.waitForResult(ros::Duration(5.0));


  if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    printf("Robot is on table №2");
  else
  { goal.header.stamp=ros::Time::now();
	  goal.header.frame_id="map";
    goal.pose.position.x=table[atoi(data)].x_second_point;
	  goal.pose.position.y=table[atoi(data)].y_second_point;;
	  goal.pose.position.z=0.0;
    client.sendGoal(goal);
    client.waitForResult(ros::Duration(5.0)); 
    if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      printf("Robot is on table №" + data);
    else
    {
      goal.header.stamp=ros::Time::now();
	    goal.header.frame_id="map";
      goal.pose.position.x=table[atoi(data)].x_second_point;
	    goal.pose.position.y=table[atoi(data)].y_second_point;;
	    goal.pose.position.z=0.0;
      client.sendGoal(goal);
      client.waitForResult(ros::Duration(5.0)); 
      if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        printf("Robot is on table №" + data);
      else
      {
        goal.header.stamp=ros::Time::now();
	      goal.header.frame_id="map";
        goal.pose.position.x=table[atoi(data)].x_third_point;
	      goal.pose.position.y=table[atoi(data)].y_third_point;;
	      goal.pose.position.z=0.0;
        client.sendGoal(goal);
        client.waitForResult(ros::Duration(5.0));
        if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            
          printf("Robot is on table №" + data);
            
        else
        {
          goal.header.stamp=ros::Time::now();
	        goal.header.frame_id="map";
          goal.pose.position.x=table[atoi(data)].x_fourth_point;
	        goal.pose.position.y=table[atoi(data)].y_fourth_point;;
	        goal.pose.position.z=0.0;
          client.sendGoal(goal);
          client.waitForResult(ros::Duration(5.0));
          if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                
            printf("Robot is on table №" + data);

          else
          
            printf("Can not reach the object");
              
            
          
        }
            
      }
          
    }
        
  }
  
  printf("Current State: %s\n", client.getState().toString().c_str());
  return 0; 
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "go_to_position_client");

  AMQP amqp("admin:admin@95.181.230.223:5672");

  AMQPQueue * qu2 = amqp.createQueue("tables_coordinates");
  qu2->Declare();
  qu2->Bind( "amq.topic", "");		
  qu2->addEvent(AMQP_MESSAGE, onMessage );
  qu2->Consume(AMQP_NOACK);


  AMQPQueue * qu2 = amqp.createQueue("robot_delivery_order");
  qu2->Declare();
  qu2->Bind( "amq.topic", "");		
  qu2->addEvent(AMQP_MESSAGE, onMessage1);
  qu2->Consume(AMQP_NOACK);


  ros::spin();
}
