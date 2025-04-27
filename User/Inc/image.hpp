#ifndef IMAGE
#define IMAGE

#include <opencv2/opencv.hpp>
#include <string>
#include <queue>
#include <algorithm>


//#define DEBUG

//路况枚举类型
enum RoadType {
    Straight,           //直行
    LeftTurn,           //左转
    RightTurn,          //右转
    Cross,              //十字路口
    LeftRoundabout,     //左侧环岛
    RightRoundabout     //右侧环岛
};

//最小连通块面积
const int minSize=10000;

//偏移数组
const int dx[]={0,-1,0,1};
const int dy[]={1,0,-1,0};

//各种阈值
const int checkCross=10;    //两边同时出界占1/checkCross认为是十字路口
const int checkJump=5;      //纵坐标差1，横坐标相差大于checkJump认为发现边线跳变
const int checkTurn=5;      //上半部分丢线占1/checkTurn认为是弯道
const double checkDown=0.3; //最下方占比为checkDown的部分参与斜率计算

//图像类
class Image{
    private:
    cv::Mat processed_image;
    int r,c;
    std::vector<int>left_edge,right_edge,mid;
    RoadType road_type;
    
    public:
    Image(std::string file_path);
    Image(cv::Mat src_image);
    void getEdge();
    void getRoadType();
    double getDir();
};

#endif