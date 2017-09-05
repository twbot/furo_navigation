#include "node.h"
#include <math.h>
node::node(int x, int y, int d, int p){
    this->xPos = x;
    this->yPos = y;
    this->gVal = d;
    this->fVal = p;
}

int node::getX() const{
    return this->xPos;
}

int node::getY() const{
    return this->yPos;
}

int node::getF() const{
    return this->fVal;
}

int node::getG() const{
    return this->gVal;
}
void node::updateF(const int &x, const int &y){

    this->fVal = this->gVal+getHeuristic(x, y)*10;
}

void node::updateG(const int &i){
    this->gVal += (i%2==0?10:14);
}

const int & node::getHeuristic(const int &x, const int &y){
    static int xDest, yDest, distance;
    xDest = x-this->xPos;
    yDest = y-this->yPos;

    distance = static_cast<int>(sqrt(xDest*xDest
                                            +yDest*yDest));
    return distance;
}
