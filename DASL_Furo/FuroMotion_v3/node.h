#ifndef NODE_H
#define NODE_H

class node{
    //total distance traveled
    int fVal;
    //heuristic
    int gVal;

public:
    node(int x, int y, int d, int p);
    int getX() const;
    int getY() const;
    int getF() const;
    int getG() const;
    const int &getHeuristic(const int &x, const int &y);
    void updateG(const int &i);
    void updateF(const int &x, const int &y);


private:
    //Current pixel position
    int xPos;
    int yPos;
};

#endif // NODE_H
