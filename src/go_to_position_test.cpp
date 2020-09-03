#include <actionlib/client/simple_action_client.h>
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "AMQPcpp.h"
#include <tf/transform_listener.h>

typedef actionlib::SimpleActionClient<geometry_msgs::PoseStamped> Client;
int  onMessage( AMQPMessage * message) {
    Client client("go_to_position", false); // true -> don't need ros::spin()
    client.waitForServer();
    string data = message->getMessage(&j);
    geometry_msgs::PoseStamped goal;
      

      
    if (data == "01"){
        goal.header.stamp=ros::Time::now();
	    goal.header.frame_id="map";

        goal.pose.position.x=0.5;
	    goal.pose.position.y=-7.0;
	    goal.pose.position.z=0.0;
        client.sendGoal(goal);
        client.waitForResult(ros::Duration(5.0));
        if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        printf("Robot is on table №1");
        printf("Current State: %s\n", client.getState().toString().c_str());
    }

    if (data == "02"){
        goal.header.stamp=ros::Time::now();
	    goal.header.frame_id="map";

        goal.pose.position.x=0.5;
	    goal.pose.position.y=-7.0;
	    goal.pose.position.z=0.0;
        client.sendGoal(goal);
        client.waitForResult(ros::Duration(5.0));
        if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        printf("Robot is on table №2");
        printf("Current State: %s\n", client.getState().toString().c_str());
    }

    if (data == "03"){
        goal.header.stamp=ros::Time::now();
	    goal.header.frame_id="map";

        goal.pose.position.x=0.5;
	    goal.pose.position.y=-7.0;
	    goal.pose.position.z=0.0;
        client.sendGoal(goal);
        client.waitForResult(ros::Duration(5.0));
        if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        printf("Robot is on table №3");
        printf("Current State: %s\n", client.getState().toString().c_str());
    }

    if (data == "True"){  //Получен верный заказ
        goal.header.stamp=ros::Time::now();
	    goal.header.frame_id="map";

        goal.pose.position.x=0.5;
	    goal.pose.position.y=-7.0;
	    goal.pose.position.z=0.0;
        client.sendGoal(goal);
        client.waitForResult(ros::Duration(5.0));
        if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        printf("Robot is on table №1");
        printf("Current State: %s\n", client.getState().toString().c_str());
    }      

}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "go_to_position_client");


  AMQP amqp("admin:admin@95.181.230.223:5672");

  AMQPQueue * qu2 = amqp.createQueue("ROSINFO");
  qu2->Declare();
  qu2->Bind( "amq.topic", "");		
  qu2->addEvent(AMQP_MESSAGE, onMessage );
  qu2->Consume(AMQP_NOACK);



  
  ros::spin();
}
