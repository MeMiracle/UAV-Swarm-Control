//
// Created by stone on 18-4-19.
//
#ifndef CARMOVE_INF_TYPEDEF_H
#define CARMOVE_INF_TYPEDEF_H


using namespace std;

enum direction {
    left, left_up, up, right_up, right, right_down,
     down, left_down, here,rise,descend
};

enum behavoir{
    keepformation = 1,
    movetogoal = 2,
    avoidobstacles = 3,
    exception = 4
};

struct Point3D{
    /*
     * 记录位置点
     */
    float x;
    float y;
    float z;

    Point3D()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Point3D(float _x, float _y,float _z = 0):x(_x),y(_y),z(_z)
    {

    }

    bool operator==(const Point3D& p)
    {
        if(this->x == p.x && this->y == p.y)
            return this->z == p.z;
        else
            return false;
    }

    bool operator<(const Point3D& p)
    {
        if(this->x < p.x)
            return this->y<p.y;
        else return (this->x + this->y) < (p.x + p.y);
    }

    void operator=(const Point3D& p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }

    double operator-(const Point3D &other) const
    /*
     * 实现为两点之间的直线距离
     */
    {
        auto x_ = abs(this->x - other.x);
        auto y_ = abs(this->y - other.y);
        auto z_ = abs(this->z - other.z);
        return sqrt(pow(x_,2) + pow(y_,2) + pow(z_,2));
    }
};

class Target {
public:
	Point3D point;
	int markerID;

    Target():markerID(0)

    {
        point.x = 0;
        point.y = 0;
        point.z = 0;
    }

	Target(float x, float y, float z, int id) {
		point.x = x;
		point.y = y;
		point.z = z;
		markerID = id;
	}

    Target(Point3D p,int id)
    {
        point.x = p.x;
        point.y = p.y;
        point.z = p.z;
        markerID = id;
    }

	~Target() {

	};
};

struct FormationPoint {
    int carid;
    Point3D point;

    FormationPoint():carid(0)
    {
        point = Point3D(0,0,0);
    }

    FormationPoint(int id, float x, float y, float z)
    {
        carid = id;
        point = Point3D(x, y, z);
    }

    FormationPoint(int id, Point3D _point)
    {
        carid = id;
        point = _point;
    }

    FormationPoint(const FormationPoint& other)
    {
        carid = other.carid;
        point = other.point;
    }
};


#define C_PI (double)3.141592653589793
const float deg2rad = C_PI / 180.0;
const float rad2deg = 180.0 / C_PI;

class uav_zyl{
private:
    int uavID;//用于后期选择队长使用,目前用0表示队长，1表示跟随的小车
    Point3D formationPoint;
    Point3D Position;//自身坐标系

public:
   float speed = 0.6;//方向合速度
   float x_speed = speed * sin(45 * deg2rad);//x轴分速度
   float y_speed = speed * cos(45 * deg2rad);//y轴分速度
   float z_speed = 0.2;

    uav_zyl(float x, float y,float z,int id):Position(x,y,z),uavID(id)
    {   

    }

    uav_zyl(const uav_zyl& other);

    uav_zyl operator =(const uav_zyl &other);

    bool approimate(Point3D p1,Point3D p2,double len = 0.30)
    {
        if((p1.x - p2.x) < len && (p1.y - p2.y) < len)
        {
            return ((p1.x - p2.x) > -len) && ((p1.y - p2.y) > -len);
        }
        else
            return false;
    }

    int getUavID() {return uavID;}

    Point3D getPosition() {return Position;}

    void setFormationPoint(Point3D point) {formationPoint = point;}

    void moveTowardDir(direction adjDir, int moveCtrlMode,float coefficient = 1.0);

    void setPosition(float x,float y,float z) {Position.x = x;Position.y=y;Position.z=z;}

    Point3D getFormationPoint() {return formationPoint;}

    void setUavID(int id) {uavID = id;}
    
    void changeVelocity(float vx,float vy,float vz) {x_speed=vx; y_speed=vy; z_speed=vz;}
};

#endif //CARMOVE_INF_TYPEDEF_H
