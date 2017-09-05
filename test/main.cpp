#include <QCoreApplication>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <queue>

#include "node.h"

const int HEIGHT = 302;
const int WIDTH = 401;

int map[WIDTH][HEIGHT];
static int closed_nodes_map[WIDTH][HEIGHT]; // map of closed (tried-out) nodes
static int open_nodes_map[WIDTH][HEIGHT]; // map of open (not-yet-tried) nodes
static int dir_map[WIDTH][HEIGHT]; // map of directions
const int dir=8; // number of possible directions to go at any position
static int dx[dir]={1, 1, 0, -1, -1, -1, 0, 1};
static int dy[dir]={0, 1, 1, 1, 0, -1, -1, -1};

std::vector<std::vector<int>> path;
using namespace cv;

void pathFind(int startX, int startY, int endX, int endY);
bool operator<(const node & a, const node & b);

int main(){

        std::vector<std::vector<int>> objects;
        std::vector<std::vector<int>> cspace;

        double alpha = 0.5;
        double beta;

        cv::Mat img = cv::imread("/home/twbot/Desktop/myImage.JPG");
        cv::Mat out, final, dst;
        cv::resize(img, out, cv::Size(), 0.10,0.10);
        cv::resize(img, final, cv::Size(), 0.10,0.10);

        int startY = HEIGHT-20;
        int startX = WIDTH -20;
        int endY = 20;
        int endX = 20;


        std::cout << "Height: " << out.rows << "Width: " << out.cols << std::endl;
        std::cout << "Image size: " << HEIGHT*WIDTH << std::endl;
        std::cout << "Start: " << startX << ", " << startY;
        std::cout << " End: " << endX << ", " << endY << std::endl;

        cv::namedWindow("Original", 0);
        cv::imshow("Original", img);
        cv::waitKey(5);

        //Color obstacles and determine index values of obstacles
        for(int i = 0; i < out.rows; i++){
            for (int j = 0; j < out.cols; j++){
                cv::Vec3b &color = out.at<cv::Vec3b>(i, j);
                if((color[0] < 80)){
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
        cv::waitKey(2);

        //Set c-space of obstacles
        for(unsigned int i = 0; i < objects.size();i++){
            int x = objects[i][0];
            int y = objects[i][1];
            for(int j = 1; j < 30; j++){
                for(int d = 0; d < dir; d++){
                    int xdx=x+(dx[d]*j); int ydy=y+(dy[d]*j);
                    if(!(xdx<0 || xdx>WIDTH-1 || ydy<0 || ydy>HEIGHT-1)){
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
        cv::waitKey(2);

        //Add obstacles and c-space to map
        for(unsigned int i =0; i <cspace.size();i++){
            int x = cspace[i][0];
            int y = cspace[i][1];
            map[x][y] = 1;
        }

        pathFind(startX, startY, endX, endY);
        //std::cout << "HERE!";
        for(int i = 0; i < HEIGHT; i++){
            for (int j = 0; j < WIDTH; j++){
                for(unsigned int w=0; w < path.size(); w++){
                    int x = path[w][0];
                    int y = path[w][1];
                    if(x==j && y == i){
                        cv::Vec3b &color = final.at<cv::Vec3b>(i, j);
                        color[1] = 255;
                        //std::cout << x << " " << y << "; ";
                    }
                }
            }
        }

        cv::namedWindow("AStar", 0);
        beta = ( 1.0 - alpha );
        addWeighted( out, alpha, final, beta, 0.0, dst);
        cv::imshow("AStar", dst);
        cv::waitKey(0);

    return 0;
}

// Determine priority (in the priority queue)
bool operator<(const node & a, const node & b)
{
  return a.getF() > b.getF();
}

//A-star path finding
void pathFind(int startX, int startY,
                        int endX, int endY)
{
        static std::priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
        static int pqi; // pq index
        static node* n0;
        static node* m0;
        static int i, j, x, y, xdx, ydy;
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
            //if((*n0).estimate(xFinish, yFinish) == 0)
            if(x==endX && y==endY)
            {
                // generate the path from finish to start
                // by following the directions
                while(!(x==startX && y==startY))
                {
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

                count++;
                //xdx = xdx*c; ydy = ydy*c;
                if(!(xdx<0 || xdx>WIDTH-1 || ydy<0 || ydy>HEIGHT-1 || map[xdx][ydy]==1
                    || closed_nodes_map[xdx][ydy]==1))
                {
                    // generate a child node
                    m0=new node( xdx, ydy, n0->getG(),
                                 n0->getF());
                    m0->updateG(i);
                    m0->updateF(endX, endY);

                    // if it is not in the open list then add into that
                    if(open_nodes_map[xdx][ydy]==0)
                    {
                        open_nodes_map[xdx][ydy]=m0->getF();
                        pq[pqi].push(*m0);
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
    }

