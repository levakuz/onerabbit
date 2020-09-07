#include <actionlib/client/simple_action_client.h>
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "AMQPcpp.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <boost/algorithm/string.hpp>
using namespace std;

float x_start_point = 7.0;
float y_start_point = -3.0;
float distance_from_table = 0.5;


class Table{
  public:

    float x11;
    float x22;
    float y11;
    float y22;


    float x_first_point;
    float y_first_point;

    float x_second_point;
    float y_second_point;

    float x_third_point;
    float y_third_point;

    float x_fourth_point;
    float y_fourth_point;

    void calculate_first_point(){

      x_first_point = (x11 + x22) / 2;
      y_first_point = y11 + distance_from_table;

    }


    void calculate_second_point(){
      x_second_point = x22 + distance_from_table;
      y_second_point = (y11 + y22) / 2;
    }
    

    void calculate_third_point(){
      x_third_point = (x11 + x22) / 2;
      y_third_point = y22 - distance_from_table;
    }


    void calculate_fourth_point(){
      x_fourth_point = x11 - distance_from_table;
      y_fourth_point = (y11 + y22) / 2;
    }  

};

Table table[20];
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;






int  onMessage( AMQPMessage * message) {
  MoveBaseClient ac("move_base", true);
  uint32_t j = 0;
  ros::NodeHandle n;
  ros::Rate r(10); // 10 hz
  string data = message->getMessage(&j);
  //geometry_msgs::PoseStamped poseStamped;

  move_base_msgs::MoveBaseGoal goal;
  
  cout<< table[1].x11<<endl;  
  cout<< data.length()<<endl;  
  if(data.length() > 4){
    try
    {
      std::vector<std::string> results;
      std::vector<std::string> tableinfo;
      std::vector<std::string> x11info;
      std::vector<std::string> x22info;
      std::vector<std::string> y11info;
      std::vector<std::string> y22info;

      boost::split(results, data, [](char c){return c == ',';});
      boost::split(tableinfo, results[0], [](char c){return c == '.';});
      boost::split(x11info, results[1], [](char c){return c == '.';});
      boost::split(x22info, results[2], [](char c){return c == '.';});
      boost::split(y11info, results[3], [](char c){return c == '.';});
      boost::split(y22info, results[4], [](char c){return c == '.';});
      
      

      table[stoi(tableinfo[1])].x11 = stoi(x11info[1]);
      table[stoi(tableinfo[1])].x22 = stoi(x22info[1]);
      table[stoi(tableinfo[1])].y11 = stoi(y11info[1]);
      table[stoi(tableinfo[1])].y22 = stoi(y22info[1]);
      table[stoi(tableinfo[1])].calculate_first_point();
      table[stoi(tableinfo[1])].calculate_second_point();
      table[stoi(tableinfo[1])].calculate_third_point();
      table[stoi(tableinfo[1])].calculate_fourth_point();
      cout<< table[1].x11<<endl;
      cout<<table[1].x_first_point<<endl;
      cout<<table[1].y_first_point<<endl;  
    
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
  }

  else if(data.length()<5){
    try
    {
      
      if (data == "True"){  //Получен верный заказ
        goal.target_pose.header.stamp=ros::Time::now();
	      goal.target_pose.header.frame_id="map";
        goal.target_pose.pose.position.x=x_start_point;
	      goal.target_pose.pose.position.y=y_start_point;
	      goal.target_pose.pose.position.z=0.0;
        goal.target_pose.pose.orientation.w=1.0;
        cout << "Еду домой" << endl;
        ac.sendGoal(goal);
        ac.waitForResult(ros::Duration(5.0));
		      
        }
      

      
     
    else{
      int table_num = stoi(data);
      goal.target_pose.header.stamp=ros::Time::now();
	    goal.target_pose.header.frame_id="map";
      goal.target_pose.pose.position.x=table[table_num].x_first_point;
	    goal.target_pose.pose.position.y=table[table_num].y_first_point;
	    goal.target_pose.pose.position.z=0.0;
      goal.target_pose.pose.orientation.w=1.0;
      ac.sendGoal(goal);
      ac.waitForResult(ros::Duration(60.0));


      if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        cout << "Robot is on table №" << data << endl;
      else
      { 
        goal.target_pose.header.stamp=ros::Time::now();
	      goal.target_pose.header.frame_id="map";
        goal.target_pose.pose.position.x=table[table_num].x_second_point;
	      goal.target_pose.pose.position.y=table[table_num].y_second_point;;
	      goal.target_pose.pose.position.z=0.0;
        goal.target_pose.pose.orientation.w=1.0;
        ac.sendGoal(goal);
        ac.waitForResult(ros::Duration(5.0)); 
        if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
          cout << "Robot is on table №" << data << endl;
        else
        {
          goal.target_pose.header.stamp=ros::Time::now();
	        goal.target_pose.header.frame_id="map";
          goal.target_pose.pose.position.x=table[table_num].x_second_point;
	        goal.target_pose.pose.position.y=table[table_num].y_second_point;;
	        goal.target_pose.pose.position.z=0.0;
          goal.target_pose.pose.orientation.w=1.0;
          ac.sendGoal(goal);
          ac.waitForResult(ros::Duration(5.0)); 
          if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            cout << "Robot is on table №" << data << endl;
          else
          {
            goal.target_pose.header.stamp=ros::Time::now();
	          goal.target_pose.header.frame_id="map";
            goal.target_pose.pose.position.x=table[table_num].x_third_point;
	          goal.target_pose.pose.position.y=table[table_num].y_third_point;;
	          goal.target_pose.pose.position.z=0.0;
            goal.target_pose.pose.orientation.w=1.0;
            ac.sendGoal(goal);
            ac.waitForResult(ros::Duration(5.0));
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            
              cout << "Robot is on table №" << data << endl;
            
            else
            {
              goal.target_pose.header.stamp=ros::Time::now();
	            goal.target_pose.header.frame_id="map";
              goal.target_pose.pose.position.x=table[table_num].x_fourth_point;
  	          goal.target_pose.pose.position.y=table[table_num].y_fourth_point;;
	            goal.target_pose.pose.position.z=0.0;
              goal.target_pose.pose.orientation.w=1.0;
              ac.sendGoal(goal);
              ac.waitForResult(ros::Duration(5.0));
              if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                
                cout << "Robot is on table №" << data << endl;

              else
          
                printf("Can not reach the object");
              
            
          
            }
            
          }
          
        }
        
      }
  
   printf("Current State: %s\n", ac.getState().toString().c_str());
   
    }
  }

    
  catch(const std::exception& e)
  {
      std::cerr << e.what() << '\n';  
  }
}        
  //ros::spinOnce();
	//r.sleep();
  
  return 0;      
}
  
  
 
  


int main(int argc, char** argv)
{
  ros::init(argc, argv, "go_to_position_client");
  MoveBaseClient ac("move_base", true);
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  AMQP amqp("admin:admin@95.181.230.223:5672");

  AMQPQueue * qu2 = amqp.createQueue("robot_delivery_order");
  qu2->Declare();		
  qu2->addEvent(AMQP_MESSAGE, onMessage );
  qu2->Consume(AMQP_NOACK);

}
