#include "calculate.h"
#include "node.h"
#include "mainwindow.h"
#include"FuroMoterController.h"
#include "FuroRobot.h"
#include "arduino.h"

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <queue>
#include <ctime>
#include <thread>


std::vector<std::vector<int>> Calculate::run(std::string file, int width, int height, int sX, int sY, int gX, int gY)
{
    std::vector<std::vector<int>> objects;
    std::vector<std::vector<int>> cspace;

    double alpha = 0.5;
    double beta;

    int dirR[8]={1, 1, 0, -1, -1, -1, 0, 1};
    int dirC[8]={0, 1, 1, 1, 0, -1, -1, -1};

    cv::Mat img = cv::imread(file);
    cv::Mat out, final, dst;
    out = scaleImage(img, width, height);
    final = scaleImage(img, width, height);

    int startY = sY;
    int startX = sX;
    int endY = gY;
    int endX = gX;

    //Color obstacles and determine index values of obstacles
    for(int i = 0; i < out.rows; i++){
        for (int j = 0; j < out.cols; j++){
            cv::Vec3b &color = out.at<cv::Vec3b>(i, j);
            if((color[0] < 120)){
                color[0] = 0;
                color[1] = 0;
                color[2] = 255;
                std::vector<int> pos;
                pos.push_back(j);
                pos.push_back(i);
                objects.push_back(pos);
            }
        }
    }

    cv::namedWindow("Obstacle Detection", 0);
    cv::imshow("Obstacle Detection", out);
    cv::waitKey(5);
    cv::destroyWindow("Obstacle Detection");

    //Set c-space of obstacles
    for(unsigned int i = 0; i < objects.size();i++){
        int x = objects[i][0];
        int y = objects[i][1];
        for(int j = 1; j < 2; j++){
            for(int d = 0; d < 8; d++){
                int xdx=x+(dirR[d]*j); int ydy=y+(dirC[d]*j);
                if(!(xdx<0 || xdx>width-1 || ydy<0 || ydy>height-1)){
                    cv::Vec3b &color = final.at<cv::Vec3b>(ydy, xdx);
                    color[0] = 255;
                    color[1] = 0;
                    color[2] = 0;
                    std::vector<int> pos;
                    pos.push_back(xdx);
                    pos.push_back(ydy);
                    cspace.push_back(pos);
                }
            }
        }
    }

    cv::namedWindow("With c-space", 0);
    cv::imshow("With c-space", final);
    cv::waitKey(5);
    cv::destroyWindow("With c-space");

    //Add obstacles and c-space to map
    for(unsigned int i =0; i <cspace.size();i++){
        int x = cspace[i][0];
        int y = cspace[i][1];
        map_path[y][x] = 1;
    }

    std::vector<std::vector<int>> path = pathFind(width, height, startX, startY, endX, endY);
    for(int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            for(unsigned int w=0; w < path.size(); w++){
                int x = path[w][0];
                int y = path[w][1];
                if(x==i && y == j){
                    cv::Vec3b &color = final.at<cv::Vec3b>(i, j);
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 0;
                }
            }
        }
    }

    cv::namedWindow("AStar", 0);
    beta = ( 1.0 - alpha );
    addWeighted( out, alpha, final, beta, 0.0, dst);
    cv::imshow("AStar", dst);
    cv::waitKey(0);
    cv::destroyAllWindows();

    for(unsigned int i = 0; i < path.size(); i++ ){
        int x = path[i][0];
        int y = path[i][1];
    }
//    previousSize = path.size() - previousSize;
    return path;
//    std::fill(path.begin(), path.end(), std::vector<int>(2, 0));

}

cv::Mat Calculate::scaleImage(cv::Mat img, double width, double height)
{
    cv::Mat scaledImage;
    double x = height/img.rows;
    double y = width/img.cols;
    cv::resize(img, scaledImage, cv::Size(), y, x);
    return scaledImage;
}

void Calculate::createMap(cv::Mat image)
{

}

void Calculate::clearAndSetObjects(cv::Mat *image, std::vector<std::vector<int>> objects)
{
    for(int i = 0; i < image->rows; i++){
        for (int j = 0; j < image->cols; j++){
            cv::Vec3b &color = image->at<cv::Vec3b>(i, j);
            color[0] = 255;
            color[1] = 255;
            color[2] = 255;
        }
    }
}


bool operator<(const node & a, const node & b)
{
  return a.getF() > b.getF();
}

//A-star path finding
std::vector<std::vector<int>> Calculate::pathFind(int width, int height, int startX, int startY,int endX, int endY)
{
    std::vector<std::vector<int>> path;
    static std::priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static node* n0;
    static node* m0;
    static int i, j, x, y, xdx, ydy;
    static int prevX, prevY;
    static int count = 0;
    pqi=0;

    // reset the node maps
    for(y=0;y<HEIGHT;y++)
    {
        for(x=0;x<WIDTH;x++)
        {
            closed_nodes_map[x][y]=0;
            open_nodes_map[x][y]=0;
        }
    }
    // create the start node and push into list of open nodes
    n0=new node(startX, startY, 0, 0);
    n0->updateF(endX, endY);
    pq[pqi].push(*n0);
    open_nodes_map[x][y]=n0->getF(); // mark it on the open nodes map

    delete n0;
    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0=new node( pq[pqi].top().getX(), pq[pqi].top().getY(),
                     pq[pqi].top().getG(), pq[pqi].top().getF());

        x=n0->getX(); y=n0->getY();

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x][y]=0;
        // mark it on the closed nodes map
        closed_nodes_map[x][y]=1;

        // quit searching when the goal state is reached
        if(x==endX && y==endY)
        {
            // generate the path from finish to start
            // by following the directions
            while(!(x==startX && y==startY))
            {
                std::cout << "Creating Path" << std::endl;
                std::vector<int> val;
                val.push_back(x);
                val.push_back(y);
                path.push_back(val);
                j=dir_map[x][y];
                x+=dx[j];
                y+=dy[j];
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();
        }

        // generate moves (child nodes) in all possible directions
        //for(int c = 1; c <= 30; c++){
        for(i=0;i<dir;i++)
        {
            xdx=x+(dx[i]); ydy=y+(dy[i]);
            prevX = xdx;
            prevY = ydy;
            int prior_orientation = 2;
            count++;

            //xdx = xdx*c; ydy = ydy*c;
            if(!(xdx<0 || xdx>width-1 || ydy<0 || ydy>height-1 || map_path[xdx][ydy]==1
                || closed_nodes_map[xdx][ydy]==1))
            {
                // generate a child node
                m0=new node( xdx, ydy, n0->getG(),
                             n0->getF());
                m0->updateG(i);

                if(( xdx == (x+0) && ((ydy == (y+1)) || (ydy == (y-1)))) && (prior_orientation == 2)){
                    m0->updateP(true);
                }
                else if(( ydy == (y+0) && ((xdx == (x+1)) || (xdx == (x-1)))) && (prior_orientation == 1)){
                    m0->updateP(true);
                }
                else{
                    m0->updateP(false);
                }

                m0->updateF(endX, endY);

                // if it is not in the open list then add into that
                if(open_nodes_map[xdx][ydy]== 0)
                {
                    open_nodes_map[xdx][ydy]=m0->getF();
                    pq[pqi].push(*m0);
                    std::cout << "X pos: " << m0->getX();
                    std::cout << " Y pos: " << m0->getY() << std::endl;
                    delete m0;
                    // mark its parent node direction
                    dir_map[xdx][ydy]=(i+dir/2)%dir;
                }
                else if(open_nodes_map[xdx][ydy]>m0->getF())
                {
                    // update the priority info
                    open_nodes_map[xdx][ydy]=m0->getF();
                    // update the parent direction info
                    dir_map[xdx][ydy]=(i+dir/2)%dir;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(pq[pqi].top().getX()==xdx &&
                           pq[pqi].top().getY()==ydy))
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
                else delete m0; // garbage collection
            }

        }
    }
    return path;
}

std::vector<int> Calculate::followPath(int startingOrientation, std::vector<std::vector<int>> path){

  std::vector<int> go;
  std::reverse(path.begin(), path.end());
  //grab orientation of robot relative to picture
  //if robot is facing opposite pixel (0,0) [top left]  to pixel (MAX, MAX) [bottom right], then mul both dir by 1
  //if robot is facing toward pixel (0,0) [top left] from pixel (MAX, MAX) [bottom right], then mul both dir by -1
  //will change in future to ensure
  int dir = startingOrientation;
  int orientation = 2;
  int which = 0;
  unsigned int i =0;
  //int dx[dir] = {1, 0, -1, 0};
  //int dy[dir] = {0, 1, 0, -1};
  unsigned int size = path.size()-1;
  std::cout << "Path size: " << size << std::endl;
  while(i < size){
          int x = path[i+1][0]*dir;
          int y = path[i+1][1]*dir;
          int xPrev = path[i][0]*dir;
          int yPrev = path[i][1]*dir;
          x = x-xPrev;
          y = y-yPrev;\
          if(orientation == 1){
              if(x==0 && y ==1){
                  //Move Forward
                  orientation = 1;
                  which = 0;
              }
              else if(x==1 && y ==0){
                  //Move Right
                  orientation = 2;
                  which = 1;
              }
              else if(x == -1 && y==0){
                  //Move Left
                  orientation = 0;
                  which = 3;
              }
              else if(x==0 && y ==-1){
                  //Move Backward
                  orientation = 3;
                  which = 2;
              }
          }
          else if(orientation == 2){
              if(x==0 && y ==1){
                  //Move left
                  which = 3;
                  orientation = 1;
              }
              else if(x==1 && y ==0){
                  //Move Forward
                  which = 0;
                  orientation = 2;
              }
              else if(x == -1 && y==0){
                  //Move Backward
                  orientation = 0;
                  which = 2;
              }
              else if(x==0 && y ==-1){
                  //Move Right
                  orientation = 3;
                  which = 1;
              }
          }
          else if(orientation == 3){
              if(x==0 && y ==1){
                  //Move Backward
                  orientation = 1;
                  which = 2;
              }
              else if(x==1 && y ==0){
                  //Move Left
                  orientation = 2;
                  which = 3;
              }
              else if(x == -1 && y==0){
                  //Move Right
                  orientation = 0;
                  which = 1;
              }
              else if(x==0 && y ==-1){
                  //Move Forward
                  orientation = 3;
                  which = 0;
              }
          }
          go.push_back(which);
        i++;

  }
  return go;
}
