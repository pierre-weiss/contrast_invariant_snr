//#pragma once
#include "mex.h"
#include <iostream>
#include <vector>
#include <deque>

#define INFTY 1e16

struct Node
{
    std::vector<Node*> children;
    int sign; // variation condition relatively to the parent
    double x, y, w;
    int id;
    
    Node(int s, int i, double y, double w);
    void addChildren(Node *n);
    ~Node();
    void print(int k = 0);
};

class Message
{
public:
    double am, bm; 	// slope and offset at -∞
    double ap, bp; 	// slope and offset at +∞
    std::deque<double> breakpoints, slope;		// x-coordinate and slope delta of breakpoints
    
    int length() const {return breakpoints.size();}
    Message(): am(0), ap(0), bm(0), bp(0){}
    Message(int l, double* b, double* s, double sm, double ym, double sp, double yp);    
    void print();
};


void Recursive_Tree_Search(Node &root);