#ifndef CALCULATE_H
#define CALCULATE_H

#include"FuroMoterController.h"
#include "FuroRobot.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <queue>

#include "node.h"

const int HEIGHT = 100;
const int WIDTH = 100;

static int map_path[WIDTH][HEIGHT];
static int closed_nodes_map[WIDTH][HEIGHT]; // map of closed (tried-out) nodes
static int open_nodes_map[WIDTH][HEIGHT]; // map of open (not-yet-tried) nodes
static int dir_map[WIDTH][HEIGHT]; // map of directions
const int dir=4; // number of possible directions to go at any position
//static int dx[dir]={1, 1, 0, -1, -1, -1, 0, 1};
//static int dy[dir]={0, 1, 1, 1, 0, -1, -1, -1};
static int dx[dir] = {1, 0, -1, 0};
static int dy[dir] = {0, 1, 0, -1};
static std::vector<std::vector<int>> next;
static int previousSize = 0;

namespace Calculate {

    std::vector<std::vector<int>> run(std::string file, int w, int h, int startX, int startY, int endX, int endY);
    cv::Mat scaleImage(cv::Mat img, double width, double height);
    std::vector<std::vector<int>> pathFind(int width, int height, int startX, int startY,int endX, int endY);
    void createMap(cv::Mat img);
    void clearAndSetObjects(cv::Mat *image, std::vector<std::vector<int>> objects);
    std::vector<int> followPath(int startingOrientation = 1, std::vector<std::vector<int>> path = next );
}

#endif // CALCULATE_H
