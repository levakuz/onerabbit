#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "AMQPcpp.h"
#include <tf/transform_listener.h>
#include <chrono>
using namespace std;

int i=0;
int k =0;

int  onMessage( AMQPMessage * message) {
    
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);		
	ros::Rate r(10); // 10 hz


	uint32_t j = 0;
	string data = message->getMessage(&j);
	
	cout << data << endl;

	i++;

	cout << "#" << i << " tag="<< message->getDeliveryTag() << " content-type:"<< message->getHeader("Content-type") ;
	cout << " encoding:"<< message->getHeader("Content-encoding")<< " mode="<<message->getHeader("Delivery-mode")<<endl;

	k=0;
	while(k!=5){
	  if (data=="01"){  
			geometry_msgs::PoseStamped poseStamped;
		 	string data = message->getMessage(&j);
			poseStamped.header.stamp=ros::Time::now();
			poseStamped.header.frame_id="map";

		
			poseStamped.pose.position.x=1;
			poseStamped.pose.position.y=-3;
			poseStamped.pose.position.z=0.0;

			poseStamped.pose.orientation.w=0.563;
			poseStamped.pose.orientation.z=-0.827;

			pub.publish(poseStamped);
			cout << "Еду" << endl;
	  } 

	  else if (data=="True"){

			geometry_msgs::PoseStamped poseStamped;
		 	string data = message->getMessage(&j);
			poseStamped.header.stamp=ros::Time::now();
			poseStamped.header.frame_id="map";

		
			poseStamped.pose.position.x=1;
			poseStamped.pose.position.y=-2;
			poseStamped.pose.position.z=0.0;

			poseStamped.pose.orientation.w=0.987;
			poseStamped.pose.orientation.z=-0.159;
		

			pub.publish(poseStamped);
			cout << "Еду" << endl;					
	  }
	  
    ros::spinOnce();
	  r.sleep();
	  ++k;
		
	}
	
    
	return 0;
};



int main(int argc, char **argv)
{		
  ros::init(argc,argv, "go_to_position");

  AMQP amqp("admin:admin@95.181.230.223:5672");

  AMQPQueue * qu2 = amqp.createQueue("ROSINFO");
  qu2->Declare();
  qu2->Bind( "amq.topic", "");

		
		
		
	qu2->addEvent(AMQP_MESSAGE, onMessage );
	qu2->Consume(AMQP_NOACK);
		
		
		
	return 0;


}



