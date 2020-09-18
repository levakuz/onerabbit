#include <actionlib/client/simple_action_client.h>
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "AMQPcpp.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <boost/algorithm/string.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <nav_msgs/OccupancyGrid.h>
#include <boost/shared_ptr.hpp>
#include <tf2/LinearMath/Quaternion.h>

using namespace std;

float x_start_point = -1.76;
float y_start_point = 0.851;
float distance_from_table = 1.0;
float coefficient_of_check = 3;


class Table{
  public:

    float x;
    float y;

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

    int center_x;
    int center_y;

    double mapResolution;

    bool is_first_point_reachable = true;
    bool is_second_point_reachable = true;
    bool is_third_point_reachable = true;
    bool is_fourth_point_reachable = true;

    vector<vector<bool> > grid_global;

    tf2::Quaternion first_point_angle;
    tf2::Quaternion second_point_angle;
    tf2::Quaternion third_point_angle;
    tf2::Quaternion fourth_point_angle;

    geometry_msgs::Quaternion first_point_angle_msg;
    geometry_msgs::Quaternion second_point_angle_msg;
    geometry_msgs::Quaternion third_point_angle_msg;
    geometry_msgs::Quaternion fourth_point_angle_msg;

    void check_global_planer(){
      boost::shared_ptr<nav_msgs::OccupancyGrid const> sharedPtr;
      nav_msgs::OccupancyGrid planner_points;
      sharedPtr = ros::topic::waitForMessage<nav_msgs::OccupancyGrid>("move_base/global_costmap/costmap",ros::Duration(10));
      planner_points = * sharedPtr;
      cout<<planner_points.info.resolution<<endl;
      mapResolution = planner_points.info.resolution;
      rows = planner_points.info.height;
      cols = planner_points.info.width;
      // Dynamically resize the grid_global
      grid_global.resize(rows);
      for (int i = 0; i < rows; i++) {
        grid_global[i].resize(cols);
        
      }
      int currCell = 0;
      int a = 0;
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          if (planner_points.data[currCell] == 0){ // unoccupied cell
            grid_global[i][j] = false;}
          else{
            grid_global[i][j] = true;
            } // occupied (100) or unknown cell (-1)
          currCell++;
          //cout<< rows<<endl;
          //cout<< cols<<endl;
      
        }  
      }

    }


    void calculate_first_point(){
      int calculated_pixel_x;
      int calculated_pixel_y;
      center_x = rows / 2;
      center_y = cols / 2;
      
      y_first_point = y + distance_from_table;
      x_first_point = x;
      // Check global planner
      while(true){
        calculated_pixel_x =center_x + ( x_first_point / mapResolution);
        calculated_pixel_y = center_y + (y_first_point / mapResolution);
        cout<< "calculated pixel_x_fp "<<calculated_pixel_x<<endl;
        cout<< "calculated pixel_y_fp "<<calculated_pixel_y<<endl;
        if (grid_global[calculated_pixel_y][calculated_pixel_x] == true ){
          //cout<<"calculate again"<<endl;
          //cout<<x_first_point<<endl;
          y_first_point += 0.01; 
        }
        else{ break;}
      }
      if (y_first_point > y + distance_from_table*coefficient_of_check)
      {
        is_first_point_reachable = false;
      }
            
    }
   

    void calculate_second_point(){
      
      int calculated_pixel_x;
      int calculated_pixel_y;

      x_second_point = x + distance_from_table;
      y_second_point = y;

      //check global planner
      while(true){
        calculated_pixel_x =center_x + (x_second_point / mapResolution);
        calculated_pixel_y = center_y + ( y_second_point / mapResolution);    
        cout<<calculated_pixel_x<<endl;
        cout<<calculated_pixel_y<<endl;
        if (grid_global[calculated_pixel_y][calculated_pixel_x ] == true ){
          //cout<<"calculate again"<<endl;
          //cout<<x_first_point<<endl;
          x_second_point += 0.01; 
        }
       else { break;}
      }
      if (x_second_point > x + distance_from_table*coefficient_of_check)
      {
        is_second_point_reachable = false;
      }
    }
    

    void calculate_third_point(){

      
      int calculated_pixel_x;
      int calculated_pixel_y;
      

      x_third_point = x;
      y_third_point = y - distance_from_table;

      //check global planner
      while(true){
        calculated_pixel_x =center_x + (x_third_point / mapResolution);
        calculated_pixel_y = center_y + (y_third_point / mapResolution);
        cout<<calculated_pixel_x<<endl;
        cout<<calculated_pixel_y<<endl;
        
        if (grid_global[calculated_pixel_y][calculated_pixel_x] == true ){
          //cout<<"calculate again"<<endl;
          //cout<<x_first_point<<endl;
          y_third_point -= 0.01; 
        }
        else{ break;}
      }

      if (y_third_point < y - distance_from_table*coefficient_of_check)
      {
        is_third_point_reachable = false;
      }
    }


    void calculate_fourth_point(){
      
      
      int calculated_pixel_x;
      int calculated_pixel_y;
      

      x_fourth_point = x - distance_from_table;
      y_fourth_point = y;

      //check global planner
      while(true){
        calculated_pixel_x =center_x + (x_fourth_point / mapResolution);
        calculated_pixel_y = center_y + (y_fourth_point / mapResolution);
        cout<<calculated_pixel_x<<endl;
        cout<<calculated_pixel_y<<endl;
        if (calculated_pixel_x == 0)
        {
          break;
        }
        if (grid_global[calculated_pixel_y][calculated_pixel_x] == true ){
          //cout<<"calculate again"<<endl;
          //cout<<x_first_point<<endl;
          x_fourth_point -= 0.01; 
        }
        else{ break;}
      }
      if (x_fourth_point < x - distance_from_table*coefficient_of_check)
      {
        is_fourth_point_reachable = false;
      }
    }

    void calculate_angles(){
      
      first_point_angle.setRPY(0, 0, 1.5708*2);
      first_point_angle.normalize();
      first_point_angle_msg = tf2::toMsg(first_point_angle);

      second_point_angle.setRPY( 0, 0, 1.5708 );
      second_point_angle.normalize();
      second_point_angle_msg = tf2::toMsg(second_point_angle);

      third_point_angle.setRPY(0, 0, 1.5708*2);
      third_point_angle.normalize();
      third_point_angle_msg = tf2::toMsg(third_point_angle);

      fourth_point_angle.setRPY( 0, 0, 1.5708);
      fourth_point_angle.normalize();
      fourth_point_angle_msg = tf2::toMsg(fourth_point_angle);


    }  

};

Table table[20];
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


int  onMessage( AMQPMessage * message) {
  MoveBaseClient ac("move_base", true);
  uint32_t j = 0;
  ros::Rate r(10); // 10 hz
  string data = message->getMessage(&j);
  //geometry_msgs::PoseStamped poseStamped;

  move_base_msgs::MoveBaseGoal goal;
  
  
  cout<< data.length()<<endl;  
  if(data.length() > 4){
    try
    {
      std::vector<std::string> results;
      std::vector<std::string> tableinfo;
      std::vector<std::string> keyinfo;
      std::vector<std::string> xinfo;
      std::vector<std::string> yinfo;


      boost::split(results, data, [](char c){return c == ',';});
      boost::split(tableinfo, results[0], [](char c){return c == ':';});
      boost::split(keyinfo, results[1], [](char c){return c == ':';});
      boost::split(xinfo, results[2], [](char c){return c == ':';});
      boost::split(yinfo, results[3], [](char c){return c == ':';});

      
      cout<<"table "<< tableinfo[1]<<endl;
      cout<<"x "<< xinfo[1]<<endl;
      cout<<"y "<< yinfo[1]<<endl;


      float x = stof(xinfo[1]);
      float y = stof(yinfo[1]);

      cout<<stof(xinfo[1])<<endl;
      cout<<stof(yinfo[1])<<endl;
      cout<<stoi(tableinfo[1])<<endl;
      
      
      if(keyinfo[1] == "create"){
        table[stoi(tableinfo[1])].x = x;
        table[stoi(tableinfo[1])].y = y;
        table[stoi(tableinfo[1])].check_global_planer();
        table[stoi(tableinfo[1])].calculate_first_point();
        table[stoi(tableinfo[1])].calculate_second_point();
        table[stoi(tableinfo[1])].calculate_third_point();
        table[stoi(tableinfo[1])].calculate_fourth_point();
        table[stoi(tableinfo[1])].calculate_angles();

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
        table[stoi(tableinfo[1])].x = 0;
        table[stoi(tableinfo[1])].y = 0;

        table[stoi(tableinfo[1])].x_first_point = 0;
        table[stoi(tableinfo[1])].y_first_point = 0;

        table[stoi(tableinfo[1])].x_second_point = 0;
        table[stoi(tableinfo[1])].y_second_point = 0;

        table[stoi(tableinfo[1])].x_third_point = 0;
        table[stoi(tableinfo[1])].y_third_point = 0;

        table[stoi(tableinfo[1])].x_fourth_point = 0;
        table[stoi(tableinfo[1])].y_fourth_point = 0;

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
        ac.waitForResult(ros::Duration(60.0));
		      
        }

      else{
        int table_num = stoi(data);
          cout<< "checking point 1"<<endl;
          cout<< table[table_num].is_first_point_reachable<<endl;
          goal.target_pose.header.stamp=ros::Time::now();
	        goal.target_pose.header.frame_id="map";
          goal.target_pose.pose.position.x=table[table_num].x_first_point;
	        goal.target_pose.pose.position.y=table[table_num].y_first_point;
	        goal.target_pose.pose.position.z=0.0;
          goal.target_pose.pose.orientation.w=table[table_num].first_point_angle_msg.w;
          goal.target_pose.pose.orientation.x=table[table_num].first_point_angle_msg.x;
          goal.target_pose.pose.orientation.y=table[table_num].first_point_angle_msg.y;
          goal.target_pose.pose.orientation.z=table[table_num].first_point_angle_msg.z;
          if (table[table_num].is_first_point_reachable == true)
          {  
            ac.sendGoal(goal);
            ac.waitForResult(ros::Duration(60.0));
            cout<<"going to point 1"<<endl;
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
              cout << "Robot is on table №" << data << endl;
          }

        else
        { 
            cout<< "checking point 2"<<endl;
            cout<< table[table_num].is_second_point_reachable<<endl;
            goal.target_pose.header.stamp=ros::Time::now();
	          goal.target_pose.header.frame_id="map";
            goal.target_pose.pose.position.x=table[table_num].x_second_point;
	          goal.target_pose.pose.position.y=table[table_num].y_second_point;;
	          goal.target_pose.pose.position.z=0.0;
            goal.target_pose.pose.orientation.w=table[table_num].second_point_angle_msg.w;
            goal.target_pose.pose.orientation.x=table[table_num].second_point_angle_msg.x;
            goal.target_pose.pose.orientation.y=table[table_num].second_point_angle_msg.y;
            goal.target_pose.pose.orientation.z=table[table_num].second_point_angle_msg.z;
            if (table[table_num].is_second_point_reachable == true)
            {
              ac.sendGoal(goal);
              ac.waitForResult(ros::Duration(60.0)); 
              cout<<"going to point 2"<<endl;
              if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                cout << "Robot is on table №" << data << endl;
            }  
            

          else
          {
              cout<< "checking point 3"<<endl;
              cout<< table[table_num].is_third_point_reachable<<endl;
              goal.target_pose.header.stamp=ros::Time::now();
	            goal.target_pose.header.frame_id="map";
              goal.target_pose.pose.position.x=table[table_num].x_third_point;
	            goal.target_pose.pose.position.y=table[table_num].y_third_point;;
	            goal.target_pose.pose.position.z=0.0;
              goal.target_pose.pose.orientation.w=table[table_num].third_point_angle_msg.w;
              goal.target_pose.pose.orientation.x=table[table_num].third_point_angle_msg.x;
              goal.target_pose.pose.orientation.y=table[table_num].third_point_angle_msg.y;
              goal.target_pose.pose.orientation.z=table[table_num].third_point_angle_msg.z;
              if (table[table_num].is_third_point_reachable == true)
              {   
                ac.sendGoal(goal);
                ac.waitForResult(ros::Duration(60.0)); 
                cout<<"going to point 3"<<endl;
                if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                  cout << "Robot is on table №" << data << endl;
              }

            
              else
              {
                cout<< "checking point 4"<<endl;
                cout<< table[table_num].is_fourth_point_reachable<<endl;
                goal.target_pose.header.stamp=ros::Time::now();
	              goal.target_pose.header.frame_id="map";
                goal.target_pose.pose.position.x=table[table_num].x_fourth_point;
  	            goal.target_pose.pose.position.y=table[table_num].y_fourth_point;;
	              goal.target_pose.pose.position.z=0.0;
                goal.target_pose.pose.orientation.w=table[table_num].fourth_point_angle_msg.w;
                goal.target_pose.pose.orientation.x=table[table_num].fourth_point_angle_msg.x;
                goal.target_pose.pose.orientation.y=table[table_num].fourth_point_angle_msg.y;
                goal.target_pose.pose.orientation.z=table[table_num].fourth_point_angle_msg.z;
                if (table[table_num].is_third_point_reachable == true)
                {  
                ac.sendGoal(goal);
                ac.waitForResult(ros::Duration(60.0));
                cout<<"going to point 4"<<endl;
                if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                  cout << "Robot is on table №" << data << endl;
                }

              
              else

                printf("Can not reach the object\n");
              
              
            }
            
          }
          
        }
        
      }
  
   printf("Current State: %s\n", ac.getState().toString().c_str());
   
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
  ros::init(argc, argv, "actionlib_move");
  ros::NodeHandle nh();
  /*nh.getParam("xstart", x_start_point);
  nh.getParam("ystart", y_start_point);
  nh.getParam("distance", distance_from_table);
  nh.getParam("coeff", coefficient_of_check);*/
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
