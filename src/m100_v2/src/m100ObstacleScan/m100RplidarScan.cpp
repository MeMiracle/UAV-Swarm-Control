#include <ros/ros.h>
#include <string.h> 
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/NavSatFix.h>
#include <geometry_msgs/Vector3Stamped.h>

#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/search/search.h>
#include <pcl/search/kdtree.h>
#include <pcl/segmentation/extract_clusters.h>
#include <m100_v2/Position.h>

#include <m100_v2/ObstacleMsg.h>
#include <std_msgs/Float32MultiArray.h>
#include <fstream>

using namespace std;

typedef struct cloudtype
{
	float x;
	float y;
	float z;
	float distance;
	int num;
	int cluster;	
	float scan_range;
	float scan_degree;
}cloudType;

typedef struct kerneltype
{
	int length;	//3*3
	int width;
}kernelType;

typedef struct obstacletype
{
	int x;	//3*3
	int y;
    int z;
}obstacleType;

typedef struct clustertype
{
	int ID;	//3*3
	int num;
    int startID;
    int endID;
}clusterType;

class point
{
public:
	float x;
	float y;
	float z;
	int visited = 0;
	int pointtype = 1;//1噪声，2边界点，3核心点
	int cluster = 0;
    int index;
    float distance;
	vector<int> corepts;//存储邻域内点的索引
	point() {}
	point(float a, float b, float c, int d, float e)
	{
		x = a;
		y = b;
		z = c;
        index = d;
        distance = e;
	}
};

#ifndef RAD2DEG
#define RAD2DEG(x) ((x)*180./M_PI)
#endif

#ifndef DEG2RAD
#define DEG2RAD(x) ((x)*M_PI/180.)
#endif

int _isnan(double x) { return x != x; }
int _isinf(double x) { return !_isnan(x) && _isnan(x - x); }

m100_v2::ObstacleMsg obstacle;
string uav_name;
float distanceThreshold=0.4;
bool showPoitCloud;
int clusterMemberThreshold=5;
bool logging=0;

//#define showPoitCloud
#define VIEWER_SHOW 0
#if VIEWER_SHOW
    pcl::visualization::PCLVisualizer viewer;
#endif

string getDirectory()
{
    char abs_path[1024];
    int cnt = readlink("/proc/self/exe", abs_path, 1024);//获取可执行程序的绝对路径
    if(cnt < 0|| cnt >= 1024)
    {
        return NULL;
    }

    //最后一个'/' 后面是可执行程序名，去掉devel/lib/m100_v2/exe，只保留前面部分路径
    int count=0;
    for(int i = cnt; i >= 0; --i)
    {
        if(abs_path[i]=='/')
        {
            abs_path[i + 1]='\0';
            count++;

            if(count == 4)
            {
                break;
            }
        }
    }

    string path(abs_path);

    path = path + "src/";

    return path;
}

vector<cloudType> remove_invalid_data(sensor_msgs::LaserScan& scan)	// 输入点云，返回包含有效点云的数组
{
	int count_total = scan.ranges.size();
	vector<cloudType> cloudTemp;
	cloudType temp;

	for (int i=0;i<count_total;i++)
	{
		if (_isinf(scan.ranges[i])==0)
		{   
		
			float degree = RAD2DEG(scan.angle_min + scan.angle_increment * i);  
			temp.x = scan.ranges[i]*sin(DEG2RAD(degree));
			temp.y = scan.ranges[i]*cos(DEG2RAD(degree));
			temp.z = 0;	// warning
			temp.num = i;
			temp.scan_range = scan.ranges[i];
			temp.scan_degree = degree;
            temp.cluster = 0;
            temp.distance = -1;
			
			cloudTemp.push_back(temp);
		}
	}
	return cloudTemp;
}

sensor_msgs::LaserScan data_expansion(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int count = scan->ranges.size();
    sensor_msgs::LaserScan scan_msg = *scan;
    
    for (int i=count-1;i>=0;i--)
    {
        if(_isinf(scan_msg.ranges[i]))
        {
            if(i==0)
            {
                if(_isinf(scan_msg.ranges[count-1]) != 1)
                {
                    //ROS_INFO("the forward data is %f", scan_msg.ranges[count-1]);
                    scan_msg.ranges[0] = scan_msg.ranges[count-1];
                }
            }
            else if(_isinf(scan_msg.ranges[i-1]) != 1)
            {
                //ROS_INFO("the forward data is %f", scan_msg.ranges[count-1]);
                scan_msg.ranges[i] = scan_msg.ranges[i-1];
            }
        }
    }

    return scan_msg;
}

vector<cloudType> compute_distance(vector<cloudType>& cloudTemp)	// 输入数组，计算数组每一个数与后一个的欧式距离，注意最后一个和第一个相接
{
	int count = cloudTemp.size();
	
	for (int i=0;i<count-1;i++)
	{
		cloudTemp[i].distance = pow((cloudTemp[i+1].x-cloudTemp[i].x), 2) +
			pow((cloudTemp[i+1].y-cloudTemp[i].y), 2) +
			pow((cloudTemp[i+1].z-cloudTemp[i].z), 2);
		cloudTemp[i].distance = sqrt(cloudTemp[i].distance);
        //ROS_INFO("%f %f %f %f %f",cloudTemp[i].x,cloudTemp[i+1].x,cloudTemp[i].y,cloudTemp[i+1].y,cloudTemp[i].distance);
	}

	cloudTemp[count-1].distance = pow((cloudTemp[count-1].x-cloudTemp[0].x), 2) +
		pow((cloudTemp[count-1].y-cloudTemp[0].y), 2) +
		pow((cloudTemp[count-1].z-cloudTemp[0].z), 2);
	cloudTemp[count-1].distance = sqrt(cloudTemp[count-1].distance);
	//ROS_INFO("zero %f %f %f %f %f",cloudTemp[count-1].x,cloudTemp[0].x,cloudTemp[count-1].y,cloudTemp[0].y,cloudTemp[0].distance);

	return cloudTemp;
}

void soft(vector<cloudType> cloudTemp) // 输入数组，按照距离进行排序
{
	int count = cloudTemp.size();
	
	
}

// dbscan cluster
float distanceBetweenTwoPoints(point a, point b) {
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}
float distanceToOrigin(pcl::PointXYZ a) {
	return sqrt((a.x)*(a.x) + (a.y)*(a.y) + (a.z)*(a.z));
}
vector<point> DBSCAN(vector<cloudType>& cloudTemp, ros::Publisher *pub) // 改进 DBSCAN 聚类，返回
{
    int count = cloudTemp.size();
    float selfObstacle = 0.5;
    ros::param::get("~selfObstacle",selfObstacle);
	ros::param::get("~distanceThreshold",distanceThreshold);
	ros::param::get("~clusterMemberThreshold",clusterMemberThreshold);
    vector<point> corecloud;//构建核心点集
    vector<point> allcloud;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);//初始化点云
    pcl::PointXYZ tempCloud;
    for (int i=0;i<count;i++)
	{
		if (cloudTemp[i].x!=0 && cloudTemp[i].y!=0)
		{
			tempCloud.x = cloudTemp[i].x;
			tempCloud.y = cloudTemp[i].y;
			tempCloud.z = cloudTemp[i].z;
            cloud->points.push_back(tempCloud);
		}
	}
    //ROS_INFO("DNSCAN data size:%d",count);

    float resolution = 0.4f;//最低一级octree的最小体素的尺寸
	pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> octree(resolution);//初始化octree
	octree.setInputCloud(cloud);
	octree.addPointsFromInputCloud();

	size_t len = cloud->points.size();
	
	for (size_t i = 0; i < len; i++)
	{		
        float tempDistance = distanceToOrigin(cloud->points[i]);
		point pt = point(cloud->points[i].x, cloud->points[i].y, cloud->points[i].z, cloudTemp[i].scan_degree, tempDistance);
        allcloud.push_back(pt);
	}
	len = allcloud.size();

	//将核心点放在corecloud中,改变allcloud中的pointtype的值
	for (size_t i = 0; i < len; i++)
	{
		vector<int> radiussearch;//存放点的索引
		vector<float> radiusdistance;//存放点的距离平方
		octree.radiusSearch(cloud->points[i], distanceThreshold, radiussearch, radiusdistance);//八叉树的邻域搜索
		if (radiussearch.size() > clusterMemberThreshold)
			allcloud[i].pointtype = 3;
		corecloud.push_back(allcloud[i]);
	}
	pcl::PointCloud<pcl::PointXYZ>::Ptr corecloud1(new pcl::PointCloud<pcl::PointXYZ>);
	corecloud1->points.resize(corecloud.size());
	for (int i = 0; i < corecloud.size(); i++) {
		corecloud1->points[i].x = corecloud[i].x;
		corecloud1->points[i].y = corecloud[i].y;
		corecloud1->points[i].z = corecloud[i].z;
	}
	pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
	kdtree.setInputCloud(corecloud1);

	for (int i = 0; i<corecloud.size(); i++) {
		vector<int> pointIdxNKNSearch;//存放点的索引
		vector<float> pointRadiusSquaredDistance;//存放点的距离平方
		octree.radiusSearch(corecloud1->points[i], distanceThreshold, pointIdxNKNSearch, pointRadiusSquaredDistance);//八叉树的邻域搜索		
		for (int j = 0; j < pointIdxNKNSearch.size(); j++) {
			corecloud[i].corepts.push_back(pointIdxNKNSearch[j]);
		}
	}
	//将所有核心点根据是否密度可达归类，改变核心点cluster的值
	int outcluster = 0;
	for (int i = 0; i<corecloud.size(); i++) {
		stack<point*> ps;
		if (corecloud[i].visited == 1) continue;
		outcluster++;
		corecloud[i].cluster = outcluster;
		ps.push(&corecloud[i]);
		point *v;
		//将密度可达的核心点归为一类
		while (!ps.empty()) {
			v = ps.top();
			v->visited = 1;
			ps.pop();
			for (int j = 0; j<v->corepts.size(); j++) {
				if (corecloud[v->corepts[j]].visited == 1) continue;
				corecloud[v->corepts[j]].cluster = corecloud[i].cluster;
				corecloud[v->corepts[j]].visited = 1;
				ps.push(&corecloud[v->corepts[j]]);
			}
		}
	}

    //找出所有的边界点，噪声点，对边界点分类，更改其cluster
	for (int i = 0; i<len; i++) {
		if (allcloud[i].pointtype == 3) continue;
		for (int j = 0; j<corecloud.size(); j++) {
			if (distanceBetweenTwoPoints(allcloud[i], corecloud[j])<distanceThreshold) {
				allcloud[i].pointtype = 2;
				allcloud[i].cluster = corecloud[j].cluster;
				break;
			}
		}
	}
	for (int i = 0; i < len; i++)
	{
		if (allcloud[i].pointtype == 1)
			allcloud[i].cluster = 0;
	}

    // Mark the obstacle with the points at both ends of the cluster
    int clusterIndexNum[outcluster]={0};
    // Eliminate clusters with fewer points in the cluster
    for (int i=0;i<corecloud.size();i++)   
    {
        clusterIndexNum[corecloud[i].cluster] ++;
    }

    obstacle.point1.resize(0);
    obstacle.point2.resize(0);
    obstacle.center.resize(0);
    if (clusterIndexNum[1]>clusterMemberThreshold)
    {
           // inline
        float min_radius=10000,first_degree,second_degree;
        geometry_msgs::Point32 tempPoint1,tempPoint2;
        for (int i=180;i<corecloud.size();i++) // cluster 0 may cross 360
        {
            if (corecloud[i].cluster == 1)
            {
                second_degree = corecloud[i].index;
                break;
            }
        }
        for (int i=180;i>=0;i--) // cluster 0 may cross 360
        {
            if (corecloud[i].cluster == 1)
            {
                first_degree = corecloud[i].index;
                break;
            }
        }
        for (int i=0;i<corecloud.size();i++)
        {
            if (corecloud[i].cluster == 1)
            {
                if (min_radius > corecloud[i].distance)
                    min_radius = corecloud[i].distance;
            }
        }
        // create two point
        if (min_radius > selfObstacle)
        {
            tempPoint1.x = min_radius*sin(DEG2RAD(first_degree));
            tempPoint1.y = min_radius*cos(DEG2RAD(first_degree));
            tempPoint1.z = 0;
            tempPoint2.x = min_radius*sin(DEG2RAD(second_degree));
            tempPoint2.y = min_radius*cos(DEG2RAD(second_degree));
            tempPoint2.z = 0;
            obstacle.point1.push_back(tempPoint1);
            obstacle.point2.push_back(tempPoint2);
            obstacle.minRadius.push_back(min_radius);
            geometry_msgs::Point32 temp_obstacle;
            temp_obstacle.x = min_radius;
            temp_obstacle.y = first_degree;
            temp_obstacle.z = second_degree;
            obstacle.center.push_back(temp_obstacle);
            //cout << "cluster 1"<<"  minRadius:"<<min_radius<<"  first:"<<first_degree<<"  second:"<<second_degree<<endl;
        }
    }

    for (int i=2;i<outcluster+1;i++) // except cluster Num.1
    {
        if (clusterIndexNum[i] > clusterMemberThreshold)
        {
            int clusterStart=0;
            geometry_msgs::Point32 tempPoint1,tempPoint2;
            float min_radius,first_degree,second_degree;
            for (int j=0;j<corecloud.size();j++)
            {
                if (corecloud[j].cluster == i)
                {
                    if (clusterStart==0)
                    {
                        min_radius = corecloud[j].distance;
                        second_degree = corecloud[j].index;
                        clusterStart=1;
                        continue;
                    }
                    if (min_radius>corecloud[j].distance)
                        min_radius = corecloud[j].distance;

                    first_degree = corecloud[j].index;
                }
            }

            // create two point
            if (min_radius > selfObstacle)
            {
                tempPoint1.x = min_radius*sin(DEG2RAD(first_degree));
                tempPoint1.y = min_radius*cos(DEG2RAD(first_degree));
                tempPoint1.z = 0;
                tempPoint2.x = min_radius*sin(DEG2RAD(second_degree));
                tempPoint2.y = min_radius*cos(DEG2RAD(second_degree));
                tempPoint2.z = 0;
                obstacle.point1.push_back(tempPoint1);
                obstacle.point2.push_back(tempPoint2);
                obstacle.minRadius.push_back(min_radius);
                geometry_msgs::Point32 temp_obstacle;
                temp_obstacle.x = min_radius;
                temp_obstacle.y = first_degree;
                temp_obstacle.z = second_degree;
                obstacle.center.push_back(temp_obstacle);
                //cout << "cluster "<<i<<"  minRadius:"<<min_radius<<"  first:"<<first_degree<<"  second:"<<second_degree<<endl;
            }
        }
    }

    // publish obstacle data
    pub->publish(obstacle);

    return corecloud;
}

void K_MEANS() // 改进 k-means 聚类，返回
{

}

vector<cloudType> line_K_Means(vector<cloudType>& cloudTemp)
{
    int clusterID = 0;
    int count = cloudTemp.size();
    int startID;
    float distanceThreshold = 0.3;

    for (int i=1;i<count;i++)   // 寻找第一个距离超过阈值起始点
    {
        if (cloudTemp[i].distance>distanceThreshold)
        {
            startID = i;
            break;
        }
    }
    for (int i=startID;i<count;i++)
    {
	    if (cloudTemp[i].distance<distanceThreshold)
        {
            if(cloudTemp[i-1].cluster == 0)
            {
                clusterID++;
                cloudTemp[i].cluster = clusterID;
            }
            else
                cloudTemp[i].cluster = cloudTemp[i-1].cluster;
        }
    }
    if (cloudTemp[0].distance < distanceThreshold)
    {
        if(cloudTemp[count-1].cluster == 0)
        {
            clusterID++;
            cloudTemp[0].cluster = clusterID;
        }
        else
            cloudTemp[0].cluster = cloudTemp[count-1].cluster;
    }
    for (int i=1;i<startID;i++)
    {
        if (cloudTemp[i].distance<distanceThreshold)
        {
            if(cloudTemp[i-1].cluster == 0)
            {
                clusterID++;
                cloudTemp[i].cluster = clusterID;
            }
            else
                cloudTemp[i].cluster = cloudTemp[i-1].cluster;
        }
    }

    int clusterIndexThreshold = 5;
    int maxClusterNum=0;
    
    for (int i=0;i<count;i++)
    {
        if (maxClusterNum < cloudTemp[i].cluster)
        {
            maxClusterNum = cloudTemp[i].cluster;
        }
    }

    int clusterIndexNum[maxClusterNum]={0};
    for (int i=0;i<count;i++)
    {
        if (cloudTemp[i].cluster !=0 )
            clusterIndexNum[cloudTemp[i].cluster-1]++;
    }
    
    // for (int i=0;i<maxClusterNum;i++)
    //     ROS_INFO("clusternum is %d cluster %d size is %d ",maxClusterNum, i, clusterIndexNum[i]);

    for (int i=0;i<count;i++)   // 消除聚类内点数较少的聚类
    {
        if (clusterIndexNum[cloudTemp[i].cluster-1]<clusterIndexThreshold)
        {
            cloudTemp[i].cluster = 0;
        }
    }

    return cloudTemp;
}

vector<obstacleType> rasterize_obstacle(vector<cloudType>& cloudTemp) //栅格化障碍物信息
{
    int count = cloudTemp.size();
    float gridSize = 1.5;
    int x,y,z;
    vector<obstacleType> obstacle;
    obstacletype temp;
    for (int i=0;i<count;i++)
    {
        if (cloudTemp[i].cluster!=0)
        {
            temp.x = cloudTemp[i].x/gridSize;
            temp.y = cloudTemp[i].y/gridSize;
            temp.z = cloudTemp[i].z/gridSize;  
        }
    }
    return obstacle;
}

void static_obstacle_detection()
{

}

void dynamic_obstacle_detection()
{

}

void arc_publish_obstacle(vector<cloudType>& cloudTemp, ros::Publisher *pub)
{
    int count = cloudTemp.size();
    int tempCloudNum=0,maxClusterNum=0;
    float uavRadius = 0.1;
    //vector<clusterType> clusters;
    //clusterType tempCluster;
    int startID;
    obstacle.center.resize(0);

    for (int i=0;i<count;i++)
    {
        if (cloudTemp[i].cluster>maxClusterNum)
            maxClusterNum = cloudTemp[i].cluster;
    }

    for (int i=1;i<maxClusterNum;i++)
    {
        int j=0;
        float min_radius=1000;
        float first_degree=1000,second_degree=1000;
        for (j=0;j<count;j++)
        {
            if (cloudTemp[j].cluster==i && min_radius==1000)
            {
                second_degree = cloudTemp[j].scan_degree;
                min_radius = cloudTemp[j].scan_range;
            }
            else if(cloudTemp[j].cluster==i)
            {
                if (cloudTemp[j].scan_range<min_radius)
                    min_radius = cloudTemp[j].scan_range;
                first_degree = cloudTemp[j].scan_degree;
            }
        }

        // show
        if (min_radius!=1000 && first_degree!=1000 && second_degree!=1000 && min_radius>uavRadius)
        {
            // ROS_INFO("cluster %d,first_degree:%f, second_degree:%f,min_radius:%f",i,first_degree,second_degree,min_radius);
            geometry_msgs::Point32 temp_obstacle;
            temp_obstacle.x = min_radius;
            temp_obstacle.y = first_degree;
            temp_obstacle.z = second_degree;
            obstacle.center.push_back(temp_obstacle);
        }
    }
    
    float min_radius=1000;  // 单独判读最后一个聚类，该聚类有可能是交接聚类
    float first_degree=1000,second_degree=1000;
    if(cloudTemp[count-1].cluster==maxClusterNum && cloudTemp[0].cluster == maxClusterNum)
    {    
        int i=0;
        for (i=0;i<count;i++)
        {   
            if(cloudTemp[i].cluster == maxClusterNum)
            {
                if (min_radius>cloudTemp[i].scan_range)
                    min_radius = cloudTemp[i].scan_range;
            }
            else
            {
                first_degree = cloudTemp[i-1].scan_degree;
                break;
            }
        }

        for (int i=count-1;i>0;i++)
        {
            if (cloudTemp[i].cluster==maxClusterNum)
            {
                if (min_radius>cloudTemp[i].scan_range)
                    min_radius = cloudTemp[i].scan_range;
            }
            else
            {
                second_degree = cloudTemp[i+1].scan_degree;
                break;
            }
        }
    }
    else
    {
        for (int i=count-1;i>=0;i++)
        {
            if (cloudTemp[i].cluster==maxClusterNum && min_radius==1000)
            {
                min_radius = cloudTemp[i].scan_range;
                first_degree = cloudTemp[i].scan_degree;
            }
            else if (cloudTemp[i].cluster==maxClusterNum)
            {
                if (min_radius > cloudTemp[i].scan_range)
                    min_radius = cloudTemp[i].scan_range;
            }
            else
            {
                second_degree = cloudTemp[i+1].scan_degree;
                break;
            }
        }
    }

    if (min_radius!=1000 && second_degree!=1000 && first_degree!=1000 && min_radius>uavRadius)
    {
        // ROS_INFO("cluster %d,first_degree:%f, second_degree:%f,min_radius:%f",maxClusterNum,first_degree,second_degree,min_radius);
        geometry_msgs::Point32 temp_obstacle;
        temp_obstacle.x = min_radius;
        temp_obstacle.y = first_degree;
        temp_obstacle.z = second_degree;
        obstacle.center.push_back(temp_obstacle);
    }
    pub->publish(obstacle);
}

void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan, ros::Publisher *pub)
{
    sensor_msgs::LaserScan scan_msg;
	
	ros::Time begin_time = ros::Time::now ();

    vector<cloudType> cloud;
    scan_msg = data_expansion(scan);
    cloud = remove_invalid_data(scan_msg);

	// logging
    ros::param::get("~logging",logging);
    if (logging)
    {
        char lidar_fileName[] = "m100_v2/log/lidar_log.txt";
        string lidar_workspace_path = getDirectory();
        string lidar_file_name(lidar_workspace_path + lidar_fileName);
        ofstream lidar_str;
        lidar_str.open(lidar_file_name.c_str(), ios::out|ios::app);
        if(lidar_str)
        {
            for (int i=0;i<scan->ranges.size();i++)
            {
				float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);  
                lidar_str << fixed << setw(5)
                    << i
                    << "\t" << scan->ranges[i]
                    << "\t" << degree 
                    << endl;
            }
        }
        lidar_str.close();
    }

    if(scan_msg.ranges.size() == 0)
    {
        obstacle.center.resize(0);
        pub->publish(obstacle);
        return ;
    }
    
    //compute_distance(cloud);
    //line_K_Means(cloud);
    //arc_publish_obstacle(cloud, pub);
	vector<point> corecloud;
    corecloud = DBSCAN(cloud, pub);

    //ROS_INFO("Total %d obstalce clusters ",obstacle.center.size());
	double clustering_time = (ros::Time::now () - begin_time).toSec ();
    //ROS_INFO("time = %f",clustering_time);
    
#if VIEWER_SHOW
    {
        // Show original point cloud 
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_msg (new pcl::PointCloud<pcl::PointXYZ>);
        int countNum = scan->ranges.size();
        cloud_msg->height = 1;
        cloud_msg->width  = countNum;
        cloud_msg->points.resize(cloud_msg->width * cloud_msg->height);
        for(int i = 0; i < countNum; i++) {
            float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);  
            if(scan->ranges[i] && (_isinf(scan->ranges[i])==0)){
                cloud_msg->points[i].x = scan->ranges[i]*sin(DEG2RAD(degree));
                cloud_msg->points[i].y = scan->ranges[i]*cos(DEG2RAD(degree));
                cloud_msg->points[i].z = 0;
            }
            else
            {
                cloud_msg->points[i].x = 0;
                cloud_msg->points[i].y = 0;
                cloud_msg->points[i].z = 0;
            }
        }
        viewer.removePointCloud("cloud_msg"); 
        viewer.addPointCloud(cloud_msg, "cloud_msg");

        // Show point cloud after clustering
        int count = corecloud.size();
        int clusterNum=0;
        for (int i=0;i<count;i++)
        {
            if (clusterNum < corecloud[i].cluster)
                clusterNum = corecloud[i].cluster;
        }
        int clusterIndexNum[clusterNum]={0};
        for (int i=0;i<count;i++)   // 消除聚类内点数较少的聚类
        {
            clusterIndexNum[corecloud[i].cluster] ++;
        }

        pcl::PointXYZ p;
		for (size_t i = 0; i < clusterNum+1; i++)  
		{  
			pcl::PointCloud<pcl::PointXYZ>::Ptr coreCloud(new pcl::PointCloud<pcl::PointXYZ>);
			for (int j=0;j<count;j++)
			{
				if (corecloud[j].cluster == i && clusterIndexNum[i]>clusterMemberThreshold)
				{
					p.x = corecloud[j].x;
					p.y = corecloud[j].y;
					p.z = corecloud[j].z;
				    coreCloud->points.push_back(p);	
				}
			}
			pcl::visualization::PointCloudColorHandlerRandom<pcl::PointXYZ> randomColor(coreCloud);
            pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> singleColor(coreCloud, 0, 255, 0);
			viewer.removePointCloud(std::to_string(i)); 
            viewer.addPointCloud(coreCloud, randomColor, std::to_string(i));
		}  
        /*
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_obstacle (new pcl::PointCloud<pcl::PointXYZ>);
        int obstacleNum = obstacle.point1.size();
        cloud_obstacle->height = 1;
        cloud_obstacle->width  = 2*obstacleNum;
        cout << "total obstacleNum is " << obstacleNum<<endl;
        cloud_obstacle->points.resize(cloud_obstacle->width * cloud_obstacle->height);
        for (int i=0;i<obstacleNum;i++)
        {
            cloud_obstacle->points[2*i].x = obstacle.point1[i].x;
            cloud_obstacle->points[2*i].y = obstacle.point1[i].y;
            cloud_obstacle->points[2*i].z = obstacle.point1[i].z;

            cloud_obstacle->points[2*i+1].x = obstacle.point2[i].x;
            cloud_obstacle->points[2*i+1].y = obstacle.point2[i].y;
            cloud_obstacle->points[2*i+1].z = obstacle.point2[i].z;
        }

        viewer.removePointCloud("obstacle_cloud"); 
        viewer.addPointCloud(cloud_obstacle, "obstacle_cloud");
        viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,10,"obstacle_cloud");
        */
        
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_obstacle (new pcl::PointCloud<pcl::PointXYZ>);
        cloud_obstacle->height = 1;
        cloud_obstacle->width  = 2*obstacle.center.size();
        cloud_obstacle->points.resize(cloud_obstacle->width * cloud_obstacle->height);
        for (int i=0;i<obstacle.center.size();i++)
        {
            cloud_obstacle->points[2*i].x = obstacle.center[i].x*sin(DEG2RAD( obstacle.center[i].z));
            cloud_obstacle->points[2*i].y = obstacle.center[i].x*cos(DEG2RAD( obstacle.center[i].z));
            cloud_obstacle->points[2*i].z = 0;

            cloud_obstacle->points[2*i+1].x = obstacle.center[i].x*sin(DEG2RAD( obstacle.center[i].y));
            cloud_obstacle->points[2*i+1].y = obstacle.center[i].x*cos(DEG2RAD( obstacle.center[i].y));
            cloud_obstacle->points[2*i+1].z = 0;
        }

        viewer.removePointCloud("obstacle_cloud"); 
        viewer.addPointCloud(cloud_obstacle, "obstacle_cloud");
        viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,10,"obstacle_cloud");
        
        
        viewer.spinOnce ();
    }
    #endif

}

geometry_msgs::Vector3 current_atti;
void attitudeCallback(const geometry_msgs::Vector3::ConstPtr& msg)
{
	current_atti = *msg;
}

sensor_msgs::NavSatFix current_gps;
void gpsCallback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
	current_gps = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "m100RplidarScan");
    ROS_INFO("m100RplidarScan_node started.");

    ros::NodeHandle n;

    ros::param::get("~uav_name",uav_name);

    ros::Publisher obstacle_pub = n.advertise<m100_v2::ObstacleMsg>(string(uav_name + "/obstacle").c_str(), 1000);
    ros::Subscriber scanSub = n.subscribe<sensor_msgs::LaserScan>(string(uav_name + "/scan").c_str(), 1000, boost::bind(&scanCallback, _1, &obstacle_pub));
	ros::Subscriber attitudeSub = n.subscribe<geometry_msgs::Vector3>(string(uav_name + "/attitude").c_str(),10,&attitudeCallback);
	ros::Subscriber gpsSub = n.subscribe<sensor_msgs::NavSatFix>(string(uav_name + "/gps").c_str(),10, &gpsCallback);
	
    
    ros::spin();

    ros::shutdown();
    return 0;
}
