#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat src, src_gray;
Mat dst, detected_edges;

static const std::string OPENCV_WINDOW = "Image window";

namespace enc = sensor_msgs::image_encodings;

std::string window_name = "Edge Map";

class mypkg
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_rgb;
	image_transport::Subscriber image_sub_depth;
	image_transport::Publisher image_pub_;

	public:
		mypkg()
		: it_(nh_)
	{
		// Subscrive to input video feed and publish output video feed
		image_sub_rgb = it_.subscribe("/camera/rgb/image_raw", 1,
		&mypkg::imageCb, this);
		//ROS_INFO("1\n");
		//image_sub_depth = it_.subscribe("/camera/depth/image_raw", 1,
		//&mypkg::imageCb, this);

		image_pub_ = it_.advertise("/mypkg/output_video", 1);

		namedWindow(OPENCV_WINDOW);
	}

	~mypkg()
	{
		destroyWindow(OPENCV_WINDOW);
	}
	
	void imageCb(const sensor_msgs::ImageConstPtr& msg)
	{
		cv_bridge::CvImagePtr cv_ptr;
		try
		{
			cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		}
		catch (cv_bridge::Exception& e)
		{
			ROS_ERROR("cv_bridge exception: %s", e.what());
			return;
		}
		
		//ROS_INFO("1 ################################# \n");

		Mat gray, edge, draw;
		cvtColor(cv_ptr->image, gray, CV_BGR2GRAY);

		Canny( gray, edge, 50, 150, 3);

		edge.convertTo(draw, CV_8U);
		
		namedWindow(OPENCV_WINDOW, CV_WINDOW_AUTOSIZE);
		imshow(OPENCV_WINDOW, draw);

		//parse_inputs(edge, 1.5, 162, 3, 0.35, 0, 1, 1);

		waitKey(100);
	}
 
};

int main(int argc, char** argv)
{
	//ROS_INFO("1\n");
	ros::init(argc, argv, "mypkg");
	mypkg ic;
	ros::spin();
	return 0;
}
