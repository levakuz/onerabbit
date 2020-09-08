#include <actionlib/client/simple_action_client.h>
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "AMQPcpp.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <boost/algorithm/string.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <nav_msgs/OccupancyGrid.h>
#include <boost/shared_ptr.hpp>

using namespace std;

float x_start_point = -1.76;
float y_start_point = 0.851;
float distance_from_table = 0.455;


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
    
    int rows;
    int cols;
    double mapResolution;

    vector<vector<bool> > grid;

    void check_global_planer(){
      boost::shared_ptr<nav_msgs::OccupancyGrid const> sharedPtr;
      nav_msgs::OccupancyGrid planner_points;
      sharedPtr = ros::topic::waitForMessage<nav_msgs::OccupancyGrid>("move_base/global_costmap/costmap",ros::Duration(10));
      planner_points = * sharedPtr;
      cout<<planner_points.info.resolution<<endl;
      mapResolution = planner_points.info.resolution;
      rows = planner_points.info.height;
      cols = planner_points.info.width;
      cout<<"im here3"<<endl;
      // Dynamically resize the grid
      grid.resize(rows);
      for (int i = 0; i < rows; i++) {
        grid[i].resize(cols);
        
      }
      int currCell = 0;
      int a = 0;
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          if (planner_points.data[currCell] == 0){ // unoccupied cell
            grid[i][j] = false;}
          else{
            grid[i][j] = true;
            a += 1;} // occupied (100) or unknown cell (-1)
          currCell++;
          cout<<grid[i][j]<<endl;
          cout<<a<<endl;
      
        }  
      }

    }

    void calculate_first_point(){
      bool k = false;
      int calculated_pixel_x;
      int calculated_pixel_y;
      int center_x = rows/2;
      int center_y = cols/2;

      y_first_point = y11 + distance_from_table;
      x_first_point = (x11 + x22) / 2;
      while(k != true){
        calculated_pixel_x =center_x + 1/ (mapResolution / x_first_point);
        calculated_pixel_y = center_y + 1/ (mapResolution / y_first_point);
        cout<<calculated_pixel_x<<endl;
        cout<<calculated_pixel_y<<endl;
        if (grid[calculated_pixel_x][calculated_pixel_y] == true ){
          cout<<"calculate again"<<endl;
          cout<<x_first_point<<endl;
          y_first_point += 0.001; 
        }
        else{ k = true;}
      }
      
      
    }
   


    void calculate_second_point(){
      
      bool k = false;
      int calculated_pixel_x;
      int calculated_pixel_y;
      int center_x = rows/2;
      int center_y = cols/2;

      x_second_point = x22 + distance_from_table;
      y_second_point = (y11 + y22) / 2;

      while(k != true){
        calculated_pixel_x =center_x + 1/ (mapResolution / x_second_point);
        calculated_pixel_y = center_y + 1/ (mapResolution / y_second_point);
        cout<<calculated_pixel_x<<endl;
        cout<<calculated_pixel_y<<endl;
        if (grid[calculated_pixel_x][calculated_pixel_y] == true ){
          cout<<"calculate again"<<endl;
          cout<<x_first_point<<endl;
          x_second_point += 0.001; 
        }
        else{ k = true;}
      }
    }
    

    void calculate_third_point(){

      bool k = false;
      int calculated_pixel_x;
      int calculated_pixel_y;
      int center_x = rows/2;
      int center_y = cols/2;

      x_third_point = (x11 + x22) / 2;
      y_third_point = y22 - distance_from_table;

      while(k != true){
        calculated_pixel_x =center_x + 1/ (mapResolution / x_third_point);
        calculated_pixel_y = center_y + 1/ (mapResolution / y_third_point);
        cout<<calculated_pixel_x<<endl;
        cout<<calculated_pixel_y<<endl;
        if (grid[calculated_pixel_x][calculated_pixel_y] == true ){
          cout<<"calculate again"<<endl;
          cout<<x_first_point<<endl;
          y_third_point = y_third_point - 0.001; 
        }
        else{ k = true;}
      }
    }


    void calculate_fourth_point(){
      
      bool k = false;
      int calculated_pixel_x;
      int calculated_pixel_y;
      int center_x = rows/2;
      int center_y = cols/2;

      x_fourth_point = x11 - distance_from_table;
      y_fourth_point = (y11 + y22) / 2;

      while(k != true){
        calculated_pixel_x =center_x + 1/ (mapResolution / x_fourth_point);
        calculated_pixel_y = center_y + 1/ (mapResolution / y_fourth_point);
        cout<<calculated_pixel_x<<endl;
        cout<<calculated_pixel_y<<endl;
        if (grid[calculated_pixel_x][calculated_pixel_y] == true ){
          cout<<"calculate again"<<endl;
          cout<<x_first_point<<endl;
          x_fourth_point = x_fourth_point - 0.001; 
        }
        else{ k = true;}
      }
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
      std::vector<std::string> keyinfo;
      std::vector<std::string> x11info;
      std::vector<std::string> x22info;
      std::vector<std::string> y11info;
      std::vector<std::string> y22info;

      boost::split(results, data, [](char c){return c == ',';});
      boost::split(tableinfo, results[0], [](char c){return c == ':';});
      boost::split(keyinfo, results[1], [](char c){return c == ':';});
      boost::split(x11info, results[2], [](char c){return c == ':';});
      boost::split(x22info, results[3], [](char c){return c == ':';});
      boost::split(y11info, results[4], [](char c){return c == ':';});
      boost::split(y22info, results[5], [](char c){return c == ':';});
      
      cout<<"table "<< tableinfo[1]<<endl;
      cout<<"x11 "<< x11info[1]<<endl;
      cout<<"x22 "<< x22info[1]<<endl;
      cout<<"y11 "<< y11info[1]<<endl;
      cout<<"y22 "<< y22info[1]<<endl;
      cout<<stof(x11info[1])<<endl;

      float x11 = stof(x11info[1]);
      float x22 = stof(x22info[1]);
      float y11 = stof(y11info[1]);
      float y22 = stof(y22info[1]);

      cout<<stof(x22info[1])<<endl;
      cout<<stof(y11info[1])<<endl;
      cout<<stof(y22info[1])<<endl;
      cout<<stoi(tableinfo[1])<<endl;
      
      
      if(keyinfo[1] == "create"){
        table[stoi(tableinfo[1])].x11 = x11;
        table[stoi(tableinfo[1])].x22 = x22;
        table[stoi(tableinfo[1])].y11 = y11;
        table[stoi(tableinfo[1])].y22 = y22;
        table[stoi(tableinfo[1])].check_global_planer();
        table[stoi(tableinfo[1])].calculate_first_point();
        table[stoi(tableinfo[1])].calculate_second_point();
        table[stoi(tableinfo[1])].calculate_third_point();
        table[stoi(tableinfo[1])].calculate_fourth_point();

        cout<< "x_first_point "<<table[stoi(tableinfo[1])].x_first_point<<endl;
        cout<<"y_first_point "<< table[stoi(tableinfo[1])].y_first_point<<endl;
        
        cout<< "x_second_point "<<table[stoi(tableinfo[1])].x_second_point<<endl;
        cout<<"y_second_point "<< table[stoi(tableinfo[1])].y_second_point<<endl;
        
        cout<< "x_third_point "<<table[stoi(tableinfo[1])].x_third_point<<endl;
        cout<<"y_third_point "<< table[stoi(tableinfo[1])].y_third_point<<endl;
        
        cout<< "x_fourth_point "<<table[stoi(tableinfo[1])].x_fourth_point<<endl;
        cout<<"y_fourth_point "<< table[stoi(tableinfo[1])].y_fourth_point<<endl;
      }

      else if(keyinfo[1] == "remove"){
        table[stoi(tableinfo[1])].x11 = 0;
        table[stoi(tableinfo[1])].x22 = 0;
        table[stoi(tableinfo[1])].y11 = 0;
        table[stoi(tableinfo[1])].y22 = 0;
      }
        

      
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
  ros::NodeHandle nh;
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
