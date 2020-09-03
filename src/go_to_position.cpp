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
	ros::Rate loop_rate(1);
	ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);		

	uint32_t j = 0;
	string data = message->getMessage(&j);
	
		  cout << data << endl;

	i++;

	cout << "#" << i << " tag="<< message->getDeliveryTag() << " content-type:"<< message->getHeader("Content-type") ;
	cout << " encoding:"<< message->getHeader("Content-encoding")<< " mode="<<message->getHeader("Delivery-mode")<<endl;

	k=0;
	if (data=="01"){  
		
 			while(n.ok()){
				geometry_msgs::PoseStamped poseStamped;
		 		string data = message->getMessage(&j);
				poseStamped.header.stamp=ros::Time::now();
				poseStamped.header.frame_id="map";

		
				poseStamped.pose.position.x=39.782;
				poseStamped.pose.position.y=-71.837;
				poseStamped.pose.position.z=0.0;

				poseStamped.pose.orientation.w=0.563;
				poseStamped.pose.orientation.z=-0.827;

					pub.publish(poseStamped);
					cout << "Еду" << endl;
					k++;
					if (k==5)
						break;
						
					ros::spinOnce();
					loop_rate.sleep();
					
						}
			

				
				

	}

	if (data=="True"){
		
 			while(n.ok()){				

				geometry_msgs::PoseStamped poseStamped;
		 		string data = message->getMessage(&j);
				poseStamped.header.stamp=ros::Time::now();
				poseStamped.header.frame_id="map";

		
				poseStamped.pose.position.x=35.679;
				poseStamped.pose.position.y=-76.190;
				poseStamped.pose.position.z=0.0;

				poseStamped.pose.orientation.w=0.987;
				poseStamped.pose.orientation.z=-0.159;
		

			        pub.publish(poseStamped);
			        cout << "Еду" << endl;


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


		AMQP amqp("admin:admin@95.181.230.223:5672");

     		AMQPQueue * qu2 = amqp.createQueue("ROSINFO");
     		qu2->Declare();
     		qu2->Bind( "amq.topic", "");

		ros::init(argc,argv, "go_to_position");
		
		
		qu2->addEvent(AMQP_MESSAGE, onMessage );
		qu2->Consume(AMQP_NOACK);
		
		
		
		return 0;


}



