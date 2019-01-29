/* idcc_mex.cpp
 *
 * Matlab function to compute the connected components of an image,
 * in 4-connexity.
 *
 * As it is written for Matlab, all the arrays in the code are in column
 * major format, i.e. for an array representing a matrix with m lines and n columns,
 * array[k] = matrix[x*n + y]
 *
 * Developper : Gabriel Bathie (07/2018)
 * */


#include <iostream>
#include <queue>
#include "mex.h"

struct Point
{
	int x, y;
	Point(int a, int b)
	{ 
		x = a;
		y = b;
	}
	
	std::vector<Point> neighbors(int w, int h)
	{
		std::vector<Point> res;
		if (x > 0)
		{
			res.push_back(Point(x-1,y));
		}
		if (y > 0)
		{
			res.push_back(Point(x,y-1));
		}
		if (x+1 < w)
		{
			res.push_back(Point(x+1,y));
		}
		if (y+1 < h)
		{
			res.push_back(Point(x,y+1));
		}
		return res;
	}
};

double *getConnComp(const double *u, int w, int h)
{
	std::queue<Point> stack({Point(0,0)});
	double *res = new double[w*h];
	int* visited = new int[w*h];
	for (int i = 0; i < w*h; ++i)
	{
		res[i] = 0;
		visited[i] = 0;
	}
	visited[0] = 1;
	double id = 1.0;
	int k = 0;
	while (!stack.empty())
	{
		Point cur = stack.front();
		stack.pop();
		res[cur.x*h + cur.y] = id;
		for (auto p : cur.neighbors(w,h))
		{
			if ((!visited[p.x*h + p.y]) && (u[p.x*h + p.y] == u[cur.x*h + cur.y]))
			{					
				visited[p.x*h + p.y] = 1;
				stack.push(p);
			}
		}
		
		// If the stack is empty, we have visited a whole connected component,
		// Go to the next connected component if there is one
		if(stack.empty())
		{
			id += 1.0;
			while(visited[k] && k < w*h)
			{
				++k;
			}
			if (k < w*h)
			{
				int y = k%h;
				stack.push(Point((k-y)/h, y));
				
				visited[k] = 1;
			}
		}
	}
	delete[] visited;
	return res;
	
}

// Entry point for Matlab
//
// Input:
// u : the image
// 
// Output :
// idcc : the id of the connected component of each pixel of u
//
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    // Ouput : idcc
    // Input : u
    double *u, *idcc;
    
    // Check for proper input
    switch(nrhs) {
        case 1 : //mexPrintf("Good call.\n");
            break;
        default: mexErrMsgTxt("Bad number of inputs.\n");
        break;
    }
    if (nlhs > 1) {mexErrMsgTxt("Too many outputs.\n");}
    
    int n,m;
    
    // Get input arguments
    // Note that n0 and n1 are reversed because of row major in C VS column major format in Matlab
    u=mxGetPr(prhs[0]);
    
    // Size of the image...
    m=mxGetM(prhs[0]); //number of rows
    n=mxGetN(prhs[0]); //number of columns
    
    // Create output arguments
    plhs[0] = mxCreateDoubleMatrix(m,n,mxREAL);
    idcc=mxGetPr(plhs[0]);
    
    double *tmp = getConnComp(u,n,m);
    for (int i = 0; i < n*m; ++i)
    {
    	idcc[i] = tmp[i];
    }
    
    delete[] tmp;    
}
