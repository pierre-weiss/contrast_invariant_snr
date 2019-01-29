#include "isotonic_regression_tree.h"

// Entry point for Matlab
//
// Input:
// T: the tree of size Nx1 is encoded through a single array of parents
// s: array of signs of size Nx1 (s(i)=1 means that the node is larger than its parent)
// w: array of weights of size Nx1
// y: array of data of size Nx1
//
// Output:
// x: minimizer of ||sqrt(w).*(x-y)||_2^2 s.t. s_i(x_i-x_j)>=0, (i,j) in E
//
// Compilation: mex isotonic_regression_tree.cpp -o isotononic_regression_tree_mex.cpp
//
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    // Ouput : x
    // Input : T, s, w, y
    
    // Check for proper input
    switch(nrhs) {
        case 4 : /*mexPrintf("Good call.\n");*/
            break;
        default: mexErrMsgTxt("Bad number of inputs.\n");
        break;
    }
    if (nlhs > 1) {mexErrMsgTxt("Too many outputs.\n");}
    
    int n;
    double *T,*s,*w,*y,*x;
    
    // Get input arguments
    // Note that n0 and n1 are reversed because of row major in C VS column major format in Matlab
    T=mxGetPr(prhs[0]);
    s=mxGetPr(prhs[1]);
    w=mxGetPr(prhs[2]);
    y=mxGetPr(prhs[3]);
    
    // Size of the image...
    n=mxGetM(prhs[0]); //number of rows
    
    // Create output arguments
    plhs[0] = mxCreateDoubleMatrix(n,1,mxREAL);
    x=mxGetPr(plhs[0]);
    
    Node root(s[0], 0, y[0], w[0]);
    Node **nodes = new Node*[n];
    nodes[0] = &root;
    for (int i = 1; i < n; ++i)
    {
        Node* tmp = new Node(s[i], i, y[i], w[i]);
        nodes[i] = tmp;
        nodes[int(T[i])-1]->addChildren(tmp);
    }
    Recursive_Tree_Search(root);
    
    for (int i = 0; i < n; ++i)
    {
        x[i] = nodes[i]->x;
    }
    
    delete[] nodes;
}