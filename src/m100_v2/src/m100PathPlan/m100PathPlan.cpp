#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <demo_flight_control.h>

using namespace std;

string uav_name("");

#define Height     20 //地图的高度
#define Width      20 //地图的宽度
#define MaxLength Height*Width //用于open表的数组

#define Reachable   0 //可达节点
#define Bar         1 //障碍物节点
#define Pass        2 //过程节点
#define Source      3 //起点
#define Destination 4 //终点

#define Sequential 0 //顺序遍历
#define NoSolution 2 //无解决方案
#define Infinity   0xfffffff

#define East       (1 << 0)
#define South_East (1 << 1)
#define South      (1 << 2)
#define South_West (1 << 3)
#define West       (1 << 4)
#define North_West (1 << 5)
#define North      (1 << 6)
#define North_East (1 << 7)

struct Point
{
	signed char x,y;//点坐标
};

struct MapNode
{
	int x,y;//地图坐标
	unsigned char reachable;//节点可达性
	unsigned char sur;//用8bit从低位到高位依次表示东南西北八个方向节点的可达性
	unsigned char value;//节点值
};

struct SpaceNode
{
	MapNode *cur;//当前节点
	bool isPushed;//是否已进过open表
	bool isPoped;//是否已从open出来过
	SpaceNode *from;//父节点
	int F,G;//估价值
	int H;//当前节点到目标节点的估值
};

struct Open
{
	int real_length;
	int max_length;
	SpaceNode *Array[MaxLength];
};

const Point dir[8] =
{
    {0, 1},   // East
    {1, 1},   // South_East
    {1, 0},   // South
    {1, -1},  // South_West
    {0, -1},  // West
    {-1, -1}, // North_West
    {-1, 0},  // North
    {-1, 1}   // North_East
};

int path_plan_priority = 99;
float grid_width = 0.5;//栅格大小
static int srcX,srcY,dstX,dstY;//起点和终点
myTaskMessage::TaskMessage task_waypoint;//控制点序列
static MapNode graph[Height][Width];
static SpaceNode space[Height][Width];


//int map2D[Height][Width];

const int map2D[Height][Width] = {
{0},
{0},
{0},
{0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,},
{0},
{0},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
{0},
{0},
{0},
{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
{0},
{0},
{0},
{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0}};


const char Symbol[5][3] = {"-","|","+","#","*"};

static SpaceNode *start;
static int shortestep;

ros::Publisher waypoint_cmd_pub;
ros::Publisher feedback_waypoint_pub;
void pathplan_cmd_callback(const m100_v2::PubSubUavMsg uavmsg);//订阅并分析控制指令
void AstarPathPlan(myTaskMessage::MyPoint src_waypoint,myTaskMessage::MyPoint dst_waypoint);

void initMap();//初始化原始地图
void initGraph(int sx,int sy,int dx,int dy);//初始化搜索图
void printMap();//打印地图
bool withinMap(int x,int y);//判断节点是否在地图内
int printShortest(myTaskMessage::MyPoint src_waypoint,myTaskMessage::MyPoint dst_waypoint);//打印最短路径
SpaceNode* getShortest();//获取open表中F值最小的节点
int astar();//搜索最短路径
void initOpen(Open *q);//初始化open表
void initSpace(int sx,int sy,int dx,int dy);//初始化搜索空间
void push(Open *q,int x,int y,int g);//向open表添加节点
SpaceNode* pop(Open *q);//从open表取出F最小的节点
void clearMap();//清除地图
int bfs();//A*优化？（防止open表溢出）


int main(int argc, char** argv)
{
    ros::init(argc, argv, "m100PathPlan");
    ROS_INFO("m100PathPlan_node started!");

    ros::NodeHandle nh;

    ros::param::get("~uav_name",uav_name);
    //订阅控制指令
    ros::Subscriber cmd_sub = nh.subscribe(string(uav_name + "/pathplan_cmd").c_str(),10,pathplan_cmd_callback);

    waypoint_cmd_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uav_name + "/waypoint_cmd").c_str(),10);
    feedback_waypoint_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uav_name + "/feedback_waypoint").c_str(),10);

    ros::spin();

    return (0);
}

void pathplan_cmd_callback(const m100_v2::PubSubUavMsg cmd_str)//订阅控制指令
{
    // if(cmd_str.playload.empty())
    //{
    //     ROS_INFO("playload is empty");
    // }
    //else
    // {
    //     ROS_INFO("length = %d",cmd_str.playload.size());
    // }
    

    myTaskMessage::TaskMessage ctl_waypoint;
    ctl_waypoint.ParseFromString(cmd_str.playload);

    task_waypoint.Clear();

    ROS_INFO("ctl_waypoint_count = %d",ctl_waypoint.point_size());
    for(int i=0;i<ctl_waypoint.point_size()-1;i++)//规划各个航迹段
    {
        //printf("(%f,%f)->",ctl_waypoint.point(i).x(),ctl_waypoint.point(i).y());
        AstarPathPlan(ctl_waypoint.point(i),ctl_waypoint.point(i+1));
        //ROS_INFO("pathplan finished once");
    }

    for(int j=0;j<task_waypoint.point_size();j++)
    {
        printf("(%f,%f)->",task_waypoint.point(j).y(),task_waypoint.point(j).x());
    }
    //ROS_INFO("pathplan finished all");

    m100_v2::PubSubUavMsg task_waypoint_msg;
    
    task_waypoint.set_kind(0);
    if(!task_waypoint.SerializeToString(&task_waypoint_msg.playload))
    {
        perror("serialize error:");
        return;
    }
    else
    {
        task_waypoint_msg.cmd_priority = path_plan_priority;
        waypoint_cmd_pub.publish(task_waypoint_msg);//执行
        feedback_waypoint_pub.publish(task_waypoint_msg);//发送
    }
}

void AstarPathPlan(myTaskMessage::MyPoint src_waypoint,myTaskMessage::MyPoint dst_waypoint)
{
    //initMap();
    initGraph(0, 0, 0, 0);
    //printMap();

    srcX = (int)(src_waypoint.y() / grid_width);//实际坐标到栅格点的映射
    srcY = (int)(src_waypoint.x() / grid_width);

    dstX = (int)(dst_waypoint.y() / grid_width);
    dstY = (int)(dst_waypoint.x() / grid_width);
  
    if (withinMap(srcX, srcY))
    {
        if(withinMap(dstX, dstY))
        {
            if (shortestep = printShortest(src_waypoint,dst_waypoint))
            {
                printf("从（%d，%d）到（%d，%d）的最短步数是: %d\n",
                    srcX, srcY, dstX, dstY, shortestep);
                printf("从（%d，%d）到（%d，%d）的代价是: %d\n",
                    srcX, srcY, dstX, dstY, space[dstX][dstY].F);

                printMap();
                clearMap();
                //bfs();
                //printDepth();
                //puts((shortestep == space[dstX][dstY].G) ? "正确" : "错误");
                //clearMap();
            }
            else
            {
                printf("从（%d，%d）不可到达（%d，%d）\n",srcX, srcY, dstX, dstY);
            }
        }
        else
        {
            printf("(%d,%d)",dstX,dstY);
            puts("终点输入错误！");
        }
    }
    else
    {
        printf("(%d,%d)",srcX,srcY);
        puts("源点输入错误！");
    }
}
/*
void initMap()
{
    for(int i=0;i<Height;i++)
    {
	for(int j=0;j<Width;j++)
	{
	    map2D[i][j] = 0;
	}
    }

    for(int i=0;i<Height*Width*0.20;i++)
    {
	map2D[rand()%Height][rand()%Width] = 1;
    }
}
*/
void initGraph(int sx, int sy, int dx, int dy)
{    //地图发生变化时重新构造地
    int i, j;
    srcX = sx;    //起点X坐标
    srcY = sy;    //起点Y坐标
    dstX = dx;    //终点X坐标
    dstY = dy;    //终点Y坐标
    for (i = 0; i < Height; i++)
    {
        for (j = 0; j < Width; j++)
        {
            graph[i][j].x = i; //地图坐标X
            graph[i][j].y = j; //地图坐标Y
            graph[i][j].value = map2D[i][j];
            graph[i][j].reachable = (graph[i][j].value == Reachable);    // 节点可到达性
            graph[i][j].sur = 0; //邻接节点个数
            if (!graph[i][j].reachable)
            {
                continue;
            }
            if (j > 0)
            {
                if (graph[i][j - 1].reachable)    // left节点可以到达
                {
                    graph[i][j].sur |= West;
                    graph[i][j - 1].sur |= East;
                }
                if (i > 0)
                {
                    if (graph[i - 1][j - 1].reachable
                        && graph[i - 1][j].reachable
                        && graph[i][j - 1].reachable)    // up-left节点可以到达
                    {
                        graph[i][j].sur |= North_West;
                        graph[i - 1][j - 1].sur |= South_East;
                    }
                }
            }
            if (i > 0)
            {
                if (graph[i - 1][j].reachable)    // up节点可以到达
                {
                    graph[i][j].sur |= North;
                    graph[i - 1][j].sur |= South;
                }
                if (j < Width - 1)
                {
                    if (graph[i - 1][j + 1].reachable
                        && graph[i - 1][j].reachable
                        && map2D[i][j + 1] == Reachable) // up-right节点可以到达
                    {
                        graph[i][j].sur |= North_East;
                        graph[i - 1][j + 1].sur |= South_West;
                    }
                }
            }
        }
    }
}

void printMap()
{
    int i, j;
    for (i = 0; i < Height; i++)
    {
        for (j = 0; j < Width; j++)
        {
            printf("%s", Symbol[graph[i][j].value]);
        }
        puts("");
    }
    puts("");
}

bool withinMap(int x, int y)
{
    return (x >= 0 && y >= 0
        && x < Height && y < Width);
}

int printShortest(myTaskMessage::MyPoint src_waypoint,myTaskMessage::MyPoint dst_waypoint)
{
    SpaceNode *p;
    int step = 0;
    myTaskMessage::MyPoint* mp;
  
    p = getShortest();
    start = p;
    if (!p)
    {
        return 0;
    }
    else
    {
        while (p->from)
        {
            graph[p->cur->x][p->cur->y].value = Pass;

            mp = task_waypoint.add_point();

            mp->set_x(((float)p->cur->y)*grid_width + grid_width/2);//栅格点到实际坐标的变换
            mp->set_y(((float)p->cur->x)*grid_width + grid_width/2);

            mp->set_z(dst_waypoint.z());//二维航迹规划时需指定飞行高度/偏航角/航点类型
            mp->set_yaw(dst_waypoint.yaw());
            mp->set_type(dst_waypoint.type());

            printf("(%d,%d)->", p->cur->x, p->cur->y);
            p = p->from;
            step++;
        }
        printf("(%d,%d)\n", p->cur->x, p->cur->y);
        graph[srcX][srcY].value = Source;
        graph[dstX][dstY].value = Destination;
        return step;
    }
}

SpaceNode* getShortest()
{    // 获取最短路径
    int result = astar();
    SpaceNode *p, *t, *q = NULL;
    switch(result)
    {
    case Sequential:    //搜索到路径，在搜索空间中从目标点反向走回源点
        p = &(space[dstX][dstY]);
        while (p)    //转置路径
        {
            t = p->from;
            p->from = q;
            q = p;
            p = t;
        }
        space[srcX][srcY].from = q->from;
        return &(space[srcX][srcY]);
    case NoSolution:
        return NULL;
    }
    return NULL;
}

int astar()
{    // A*算法遍历
    int times = 0;
    int i;
    int curX, curY;//当前点坐标
    int surX, surY;//邻接点坐标
    //float surG;
    int surG;

    Open q; //Open表
    SpaceNode *p;
  
    initOpen(&q);
    initSpace(srcX, srcY, dstX, dstY);
    space[srcX][srcY].isPushed = true;
    push(&q, srcX, srcY, 0);
  
    while (q.real_length)
    {   times++;
        p = pop(&q);
        curX = p->cur->x;
        curY = p->cur->y;
        if (!p->H)//H值为0，搜索到目标点，结束
        {
	    printf("check times: %d\n", times);
            return Sequential;
        }
        for (i = 0; i < 8; i++)//检查当前点的8个连通方向的邻接点
        {
            if (! (p->cur->sur & (1 << i)))//该方向邻接点不可达
            {
                continue;
            }

            surX = curX + dir[i].x;
            surY = curY + dir[i].y;
	    if(i % 2 == 0)
	    {
		surG = p->G + 10;//横向纵向移动一格代价为10；
	    }
	    else
	    {
		surG = p->G + 14;//对角线移动一格代价为14；
	    }

            if (!space[surX][surY].isPushed)//如果邻接点未访问过（不在open表中），则将该邻接点放入open表
            {
                space[surX][surY].isPushed = true;
                space[surX][surY].from = p;
                //surG = p->G + sqrt((curX - surX) * (curX - surX) + (curY - surY) * (curY - surY));//G值计算？

                push(&q, surX, surY, surG);
            }
	    else//如果邻接点已经在open表中，需要检查由当前节点到底该节点是否具有更小的G值，
		//如果G值更小，则把当前节点设为该邻接点的父节点，并重新计算F值和G值
	    {
		if(!space[surX][surY].isPoped)
		{
		    if(surG < space[surX][surY].G)
		    {
			space[surX][surY].G = surG;
			space[surX][surY].F = surG + space[surX][surY].H;
			space[surX][surY].from = p;
		    }
		}

	    }
        }
    }
  
    return NoSolution; //无结果
}
// 优先队列基本操作
void initOpen(Open *q)    //优先队列初始化
{
    q->real_length = 0;        // 队内元素数初始为0
    q->max_length = 0;
}

// 搜索空间初始化操作
void initSpace(int sx, int sy, int dx, int dy)
{    
    int i, j;
    for (i = 0; i < Height; i++)
    {
        for (j = 0; j < Width; j++)
        {
            space[i][j].cur = &graph[i][j];        // 当前节点
            space[i][j].isPushed = false;//!graph[i][j].reachable;        // 是否被访问
	    space[i][j].isPoped = false;//!graph[i][j].reachable;
            space[i][j].from = NULL;                // 所来节点
	    space[i][j].F = 0;
	    space[i][j].G = 0;
            space[i][j].H = 10 * (abs(dx - i) + abs(dy - j));    // 评价函数值
        }
    }
    space[sx][sy].F = space[sy][sy].G + space[sx][sy].H;            //起始点评价初始值
    space[dx][dy].G = Infinity;
}

void push(Open *q, int x, int y, int g)
{    //向优先队列（Open表）中添加元素
    SpaceNode *t;
    int i, mintag;
    space[x][y].G = g;    //源点到当前点的代价
    space[x][y].F = space[x][y].G + space[x][y].H;
    q->Array[q->max_length] = &(space[x][y]);
    //mintag = q->real_length;
    q->real_length++;
    q->max_length++;
    //printf("push:max=%d,real=%d,F=%d\n",q->max_length,q->real_length,q->Array[q->max_length-1]->F);

/*
    for (i = 0; i < q->real_length - 1; i++)
    {
        if (q->Array[i]->F < q->Array[mintag]->F)//每次向open表加入新节点时都遍历open表，
						//寻找最小F值的节点（可改进）
        {
            mintag = i;
        }
    }
    t = q->Array[q->real_length - 1];
    q->Array[q->real_length - 1] = q->Array[mintag];
    q->Array[mintag] = t;    //将评价函数值F最小节点置于队头
*/}

SpaceNode* pop(Open *q)
{
    int minFIndex=0;

    for(int i=1;i<q->max_length;i++)//有问题
    {
	if(q->Array[i]->F <= q->Array[minFIndex]->F && q->Array[i]->isPoped==false)
		minFIndex = i;
    }

    SpaceNode *t = q->Array[minFIndex];
    //for(int i=minFIndex;i<q->real_length-1;i++)
    //{
	//q->Array[i] = q->Array[i+1];
    //}

    q->real_length--;
    t->isPoped = true;
    //printf("pop :max=%d,real=%d,F=%d,(%d,%d)\n",q->max_length,q->real_length,t->F,t->cur->x,t->cur->y);
    return t;
    //return q->Array[--q->real_length];//队头节点即是F最小的节点
}

void clearMap()
{    // Clear Map Marks of Steps
    SpaceNode *p = start;
    while (p)
    {
        graph[p->cur->x][p->cur->y].value = Reachable;
        p = p->from;
    }
    graph[srcX][srcY].value = map2D[srcX][srcY];
    graph[dstX][dstY].value = map2D[dstX][dstY];
}
