#include <opencv2/opencv.hpp>
#include "image.hpp"

Image::Image(std::string file_path){
    //读取彩色图片
    cv::Mat src_image=cv::imread(file_path,cv::IMREAD_COLOR);

    //读取失败报错
    if(src_image.empty()){
        std::cerr<<"path error!\n input path:"<<file_path<<"\n";
        return;
    }

    //转化为灰度图
    cv::Mat gray_image;
    cv::cvtColor(src_image,gray_image,cv::COLOR_BGR2GRAY);

    //二值化
    cv::Mat binary_image;
    cv::threshold(gray_image,binary_image,128,255,cv::THRESH_BINARY);

    //去除小连通块
    std::vector<std::vector<cv::Point>> contours;   // 存储轮廓的向量
    std::vector<cv::Vec4i> hierarchy;               // 存储层次结构的向量
    cv::findContours(binary_image,contours,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    cv::Mat result_image = cv::Mat::zeros(binary_image.size(), CV_8UC1); // 创建结果图像
    for(size_t i=0;i<contours.size();i++){
        double area=cv::contourArea(contours[i]);   //当前连通块面积
        if(area>=minSize){
            // 如果面积大于等于最小保留面积，则将该连通区域绘制到结果图像上
            cv::drawContours(result_image,contours,i,cv::Scalar(255),-1);
        }
    }
    
    //保存结果
    processed_image=result_image;
    r=processed_image.rows;
    c=processed_image.cols;

#ifdef DEBUG
    //DEBUG时展示图像
    cv::imshow("Original Image", src_image);
    cv::imshow("Binary Image", binary_image);
    cv::imshow("resultImage", result_image);
#endif
}

Image::Image(cv::Mat src_image){
    // //读取彩色图片
    // cv::Mat src_image=cv::imread(file_path,cv::IMREAD_COLOR);

    // //读取失败报错
    // if(src_image.empty()){
    //     std::cerr<<"path error!\n input path:"<<file_path<<"\n";
    //     return;
    // }

    //压缩
    cv::resize(src_image,src_image,cv::Size(src_image.cols*2/3,src_image.rows*2/3));

    //转化为灰度图
    cv::Mat gray_image;
    cv::cvtColor(src_image,gray_image,cv::COLOR_BGR2GRAY);

    //二值化
    cv::Mat binary_image;
    cv::threshold(gray_image,binary_image,128,255,cv::THRESH_BINARY);

    //去除小连通块
    std::vector<std::vector<cv::Point>> contours;   // 存储轮廓的向量
    std::vector<cv::Vec4i> hierarchy;               // 存储层次结构的向量
    cv::findContours(binary_image,contours,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    cv::Mat result_image = cv::Mat::zeros(binary_image.size(), CV_8UC1); // 创建结果图像
    for(size_t i=0;i<contours.size();i++){
        double area=cv::contourArea(contours[i]);   //当前连通块面积
        if(area>=minSize){
            // 如果面积大于等于最小保留面积，则将该连通区域绘制到结果图像上
            cv::drawContours(result_image,contours,i,cv::Scalar(255),-1);
        }
    }
    
    //保存结果
    processed_image=result_image;
    r=processed_image.rows;
    c=processed_image.cols;


    // std::cout<<"read img ok!\n";
#ifdef DEBUG
    //DEBUG时展示图像
    cv::imshow("Original Image", src_image);
    cv::imshow("Binary Image", binary_image);
    cv::imshow("resultImage", result_image);
#endif
}


void Image::getEdge(){

    left_edge.resize(r,1000000000);
    right_edge.resize(r,0);
    mid.resize(r,0);

    //首先先找下边界的白色中点
    left_edge[r-1]=-1;
    int bottom_white_left,bottom_white_right;
    for(int i=0;i<c;i++){
        if(processed_image.at<uchar>(r-1,i)==255){
            if(left_edge[r-1]==-1){
                left_edge[r-1]=i;
            }
            right_edge[r-1]=i;
        }
    }
    mid[r-1]=(left_edge[r-1]+right_edge[r-1])/2;
    //std::cout<<mid[r-1]<<"\n";

    //使用广度优先搜索寻找其余位置的左右边界与中线位置
    std::queue<std::pair<int,int>>q;
    std::vector<std::vector<uchar>>vis(r,std::vector<uchar>(c,0)); 
    q.push({r-1,mid[r-1]});

    int cnt=0;

    while(!q.empty()){

        //cnt++;

        std::pair<int,int> p=q.front();
        q.pop();
        for(int i=0;i<4;i++){
            int x=p.first+dx[i];
            int y=p.second+dy[i];
            if(x>=0 and x<r and y>=0 and y<c){
                //std::cout<<x<<" "<<y<<" "<<1*(vis[x][y])<<'\n';
                if(!vis[x][y] and processed_image.at<uchar>(x,y)==255){
                    q.push({x,y});
                    vis[x][y]=1;
                    left_edge[x]=std::min(left_edge[x],y);
                    right_edge[x]=std::max(right_edge[x],y);
                }
            }
        }
    }

    // std::cout<<"get edge ok!\n";
}

void Image::getRoadType(){
    road_type=RoadType::Straight;
    std::vector<uchar>touch_left(r,0),touch_right(r,0);
    int cnt_touch_left_right=0;
    for(int i=0;i<r;i++){
        if(left_edge[i]==0)     touch_left[i]=1;
        if(right_edge[i]==c-1)  touch_right[i]=1;
        if(touch_left[i] and touch_right[i]){
            cnt_touch_left_right++;
        }
        mid[i]=(left_edge[i]+right_edge[i])/2;
    }
    if(cnt_touch_left_right*checkCross>=c){
        road_type=RoadType::Cross;
        //找到进入十字路口前后位置连线来避免中线异常突变
        std::vector<int>edge_jump;//中线跳变点
        for(int i=1;i<r;i++){
            if(std::abs(mid[i]-mid[i-1])>checkJump){
                edge_jump.push_back(i);
            }
        }
        int cross_begin=std::min((*edge_jump.rbegin())+2,r-1);
        int cross_end=std::max(0,edge_jump[0]-2);
        int pos_begin=mid[std::min(r-1,cross_begin+1)];
        int pos_end=mid[std::max(0,cross_end-1)];
        double k=(pos_begin-pos_end)/(1.0*(cross_begin-cross_end));
        for(int i=cross_end;i<=cross_begin;i++){
            mid[i]=pos_end+k*(i-cross_end);
        }
    }else{
        //在画面远处边界出界认为是弯道
        int cnt_touch_left=0;
        int cnt_touch_right=0;
        for(int i=0;i<=r/2;i++){
            cnt_touch_left+=touch_left[i];
            cnt_touch_right+=touch_right[i];
        }
        if((cnt_touch_left+cnt_touch_right)*checkTurn>r){
            if(cnt_touch_left>cnt_touch_right){
                road_type=RoadType::LeftTurn;
            }else{
                road_type=RoadType::RightTurn;
            }
        }
    }

    // std::cout<<"get road type\n";

#ifdef DEBUG

    //DEBUG时展示图像
    cv::Mat edge_image=processed_image;
    for(int i=0;i<r;i++){
        edge_image.at<uchar>(i,left_edge[i]+5)=0;
        edge_image.at<uchar>(i,right_edge[i]-5)=0;
        edge_image.at<uchar>(i,mid[i])=0;
        //std::cout<<left_edge[i]<<" "<<right_edge[i]<<" "<<mid[i]<<"\n";
    }
    cv::imshow("edgeimage",edge_image);
    cv::waitKey(0);

#endif
}

double Image::getDir(){
    int x_0,x_1,y_0,y_1;
    x_0=r-1-1.0*r*checkDown;
    x_1=r-1;
    y_0=mid[x_0];
    y_1=mid[x_1];
    return (1.0*(y_1-y_0)/(1.0*(x_1-x_0)));
};
