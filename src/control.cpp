#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int16.h"
#include "math.h"


float comp; int way;
float resta;

std_msgs::Float64 wei;
std_msgs::Int16 f;
int i=1;

int count_1=0;
void nombredecallback(const std_msgs::Float64 &msg){
  wei=msg;
  wei.data=abs(wei.data);
  if(i==1){
    comp=wei.data-4000;
    i=0;
  }
}

void reinicio(const std_msgs::Int16 &re){
  way=2;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "flags");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("/peso", 1000, nombredecallback);
  ros::Subscriber sub_reboot = nh.subscribe("/reboot", 1000, reinicio);
  ros::Publisher pub = nh.advertise<std_msgs::Int16 >("/signal",1000);
  ros::Rate rate(10);
  ros::Rate wait(5);
  f.data=0;
  wei.data=1000000;
  way=0;
  while (ros::ok()){
    ros::spinOnce();
    switch(f.data){
      case 0:
        if(wei.data<=comp){
          if(way==0){
            f.data=1;
            way=1;
          }
        }
      break;

      case 1:
        if(wei.data>comp){
          if(way==1){
            f.data=2;
          }  
        }
      break;

      case 2:
        if(way==2){
          f.data=3;
        }
      break;

      case 3:
        f.data=0;
        way=0;
      break;
    }
    pub.publish(f);
    rate.sleep();
    ROS_INFO("valor de wei es %f, way es %d y sign es %d", abs(wei.data), way, f.data);
}
  return 0;
}
