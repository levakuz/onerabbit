#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "AMQPcpp.h"
#include <tf/transform_listener.h>
#include <chrono>
#include <ctime>
using namespace std;
int i=0;
int k =0;
int  onMessage( AMQPMessage * message) {
       ros::NodeHandle n;
	ros::Rate loop_rate(1);
	ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);		

	uint32_t j = 0;
	string data = message->getMessage(&j);
	
		  cout << data << endl;

	i++;

	cout << "#" << i << " tag="<< message->getDeliveryTag() << " content-type:"<< message->getHeader("Content-type") ;
	cout << " encoding:"<< message->getHeader("Content-encoding")<< " mode="<<message->getHeader("Delivery-mode")<<endl;

	k=0;
	if (data=="1"){  
		
 			while(n.ok()){
				geometry_msgs::PoseStamped poseStamped;
		 		string data = message->getMessage(&j);
				poseStamped.header.stamp=ros::Time::now();
				poseStamped.header.frame_id="map";

		
				poseStamped.pose.position.x=0.5;
				poseStamped.pose.position.y=-7.0;
				poseStamped.pose.position.z=0.0;

				poseStamped.pose.orientation.w=1.0;
	//ROS_INFO("Move to position:\n"
			//	 "1) pos.linear: x=%f y=%f z=%f\n"
			//	 "1) pos.angular: x=%f y=%f z=%f\n",
	//			 poseStamped.pose.position.x, poseStamped.pose.position.y, poseStamped.pose.position.z,);
					pub.publish(poseStamped);
					cout << "Еду" << endl;
					k++;
					if (k==5)
						break;
						
					ros::spinOnce();
					loop_rate.sleep();
					
						}
			

				
				

	}

	if (data=="True"){/*time_t sec = time(NULL);
tm* t = localtime(&sec);
cout<<"Текущее время и дата:"<<asctime(t)<<endl;
cout<<ros::Time::now()<<endl;*/
		
 			while(n.ok()){				

				geometry_msgs::PoseStamped poseStamped;
		 		string data = message->getMessage(&j);
				poseStamped.header.stamp=ros::Time::now();
				poseStamped.header.frame_id="map";

		
				poseStamped.pose.position.x=7.0;
				poseStamped.pose.position.y=-1.0;
				poseStamped.pose.position.z=0.0;

				poseStamped.pose.orientation.w=1.0;
		
			//ROS_INFO("Move to position:\n"
			//	 "1) pos.linear: x=%f y=%f z=%f\n"
			//	 "1) pos.angular: x=%f y=%f z=%f\n",
	//			 poseStamped.pose.position.x, poseStamped.pose.position.y, poseStamped.pose.position.z,);
					pub.publish(poseStamped);
					cout << "Еду" << endl;
				time_t seconds = time(NULL);
tm* timeinfo = localtime(&seconds);
cout<<"Текущее время и дата:"<<asctime(timeinfo)<<endl;

					k++;
					if (k==5)
						break;
						
					ros::spinOnce();
					loop_rate.sleep();
					
						}

				

	}

	return 0;
};



int main(int argc, char **argv)
{		
		//ros::NodeHandle n;

		AMQP amqp("test:test@localhost:5672");

     		AMQPQueue * qu2 = amqp.createQueue("ROSINFO");
     		qu2->Declare();
     		qu2->Bind( "amq.topic", "");

		ros::init(argc,argv, "go_to_position");
		
		
		qu2->addEvent(AMQP_MESSAGE, onMessage );
		qu2->Consume(AMQP_NOACK);
		
		
		
		return 0;
		/*ros::init(argc,argv, "go_to_position");
		ros::NodeHandle n;
		ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);		
		while(n.ok()){
		ros::Rate loop_rate(1);
		geometry_msgs::PoseStamped poseStamped;
		
		poseStamped.header.stamp=ros::Time::now();
		poseStamped.header.frame_id="map";

		
		poseStamped.pose.position.x=2.0;
		poseStamped.pose.position.y=-7.0;
		poseStamped.pose.position.z=0.0;

		
		poseStamped.pose.orientation.w=1.0;

		
		pub.publish(poseStamped);
		}*/
		return 0;
}
//rostopic pub /move_base_simple/goal geometry_msgs/PoseSped  '{header: {stamp: now, frame_id: "map"}, pose: {position: {x: -2.0, y: 0.0, z: 0.0}, orientation: {w: 1.0}}}'

