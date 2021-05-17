#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "AMQPcpp.h"

#include <boost/algorithm/string.hpp>


using namespace std;

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


int  onMessage( AMQPMessage * message) {
  
  uint32_t j = 0;
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);		
  ros::Rate r(10); // 10 hz
  string data = message->getMessage(&j);
  geometry_msgs::PoseStamped poseStamped;
  int k = 0;
  
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
      onMessage(message);
      
    }
  }
  else if(data.length()<5){
    try
    {
      while (k!=5)
      {
        if (data == "True"){  //Получен верный заказ
	  poseStamped.header.stamp=ros::Time::now();
	  poseStamped.header.frame_id="map";
          poseStamped.pose.position.x=7.0;
	  poseStamped.pose.position.y=-3.0;
	  poseStamped.pose.position.z=0.0;

	  poseStamped.pose.orientation.w=1.0;
          pub.publish(poseStamped);
	  cout << "Еду домой" << endl;
        }   
     
        else{

          int table_num = stoi(data);	 		
	  poseStamped.header.stamp=ros::Time::now();
	  poseStamped.header.frame_id="map";

	  cout<<table[table_num].x_first_point<< endl;
          cout<<table[table_num].y_first_point<< endl;
  	  poseStamped.pose.position.x=table[table_num].x_fourth_point;
	  poseStamped.pose.position.y=table[table_num].y_fourth_point;
	  poseStamped.pose.position.z=0.0;

	  poseStamped.pose.orientation.w=1.0;
	  //poseStamped.pose.orientation.z=-0.827;
          //poseStamped.pose.orientation.x=-1.0;

	  pub.publish(poseStamped);
	  cout << "Еду" << endl;

        }
      ros::spinOnce();
      r.sleep();
      ++k;      
      }  
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
  }

  return 0;
}
int main(int argc, char** argv)
{

  ros::init(argc, argv, "go_to_position_client");
  AMQP amqp("admin:admin@95.181.230.223:5672");
  AMQPQueue * qu2 = amqp.createQueue("robot_delivery_order");
  qu2->Declare();		
  qu2->addEvent(AMQP_MESSAGE, onMessage);


  /*AMQPQueue * qu1 = amqp2.createQueue("tables_coordinates");
  qu1->Declare();
  qu1->Bind( "amq.topic", "tables_coordinates");		
  qu1->addEvent(AMQP_MESSAGE, onMessage1 );*/
  
  
  while(ros::ok()){
    qu2->Consume(AMQP_NOACK);
  } 
}
