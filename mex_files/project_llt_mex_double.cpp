#include "flst_double.cpp"
#include <vector>
#include <ctime>
#include "isotonic_regression_tree.h"
#include "mex.h"


double average(std::vector<LsPoint> &contour, double *u1, int n1)
{
    double res = 0;
    int count = contour.size();
    for (LsPoint &p : contour)
    {
        res += u1[p.x * n1 + p.y];
    }
    return res/count;
}


// Copies the FLST tree to a form interpretable by the isotonic regression
static int localNodeId = 0;
void CreateNodeFromShapeTree(Node *vroot, LsShape* sroot, std::vector<std::pair<Node*, LsShape*>> *VectorLinks)
{
    VectorLinks->push_back(std::pair<Node*, LsShape*>(vroot,sroot));
    sroot->shapeId = localNodeId;
    
    for (LsShape* child = sroot->find_child();
    child!=0;child=child->find_sibling())
    {
        ++localNodeId;
        int sign = (child->type == LsShape::INF) ? -1 : 1;
        Node *vchild = new Node(sign, localNodeId, 0, 0);
        vroot->addChildren(vchild);
        CreateNodeFromShapeTree(vchild, child, VectorLinks);
    }
}


void Debug(LsShape *shape, int n0, int n1)
{
    LsTreeIterator it(LsTreeIterator::Pre, shape);
    auto end = LsTreeIterator::end(LsTreeIterator::Pre, shape);
    for (; it != end; ++it)
    {
        mexPrintf("Tree - gray = %1.2e",(*it)->gray);
        mexPrintf(" - area = %i",(*it)->area);
        mexPrintf(" - id = %i\n",(*it)->shapeId);
        /*for(int i = 0; i < (*it)->area; ++i)
         * {
         * //if (p.x < n0 && p.y < n1)
         * mexPrintf("\t point : %i, %i \n", (*it)->pixels[i].x, (*it)->pixels[i].y);
         * }/*
         * for (auto p :  (*it)->contour)
         * {
         * mexPrintf("\t point : %i, %i \n", p.x, p.y);
         * }  */
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    // Ouput : u, [time_tree;time_DP;time_total]
    // Input : u0, u1
    int n0,n1;
    double *u; double *u0; double *u1;
    
	using namespace std;
	
    double *times;
	double t_ini, tree_time, DP_time;
	clock_t t_begin, t_end; 
    tree_time=0;DP_time=0;
    
    t_begin=clock();
    t_ini=t_begin;

    // Check for proper input
    switch(nrhs) {
        case 2 : //mexPrintf("Projection being computed.\n");
        break;
        default: mexErrMsgTxt("Bad number of inputs.\n");
        break;
    }
    if (nlhs > 2) {mexErrMsgTxt("Too many outputs.\n");}
    
    // Get input arguments
    u0=mxGetPr(prhs[0]);
    u1=mxGetPr(prhs[1]);
    n0=mxGetM(prhs[0]); //number of rows
    n1=mxGetN(prhs[0]); //number of columns
    
    // Note that n0 and n1 are reversed because of row major in C VS column major format in Matlab
    plhs[0] = mxCreateDoubleMatrix(n0,n1,mxREAL);
    plhs[1] = mxCreateDoubleMatrix(3,1,mxREAL);
    u=mxGetPr(plhs[0]);
    times=mxGetPr(plhs[1]);
    
    double* uu0=new double[n0*n1];
    double* uu1=new double[n0*n1];
    
    // 1) Compute the FLLT of u0.
    for (int i=0;i<n0;++i){
        for (int j=0;j<n1;++j){
            uu0[j+i*n1] = u0[i+j*n0];
            uu1[j+i*n1] = u1[i+j*n0];
        }
    }
    
    t_begin=clock();
    LsTree tree(uu0, n1, n0);
    t_end=clock();
    tree_time =  double(t_end - t_begin) / CLOCKS_PER_SEC;
	//mexPrintf("Tree:%1.2e -- #shapes=%i \n",tree_time,tree.iNbShapes);
    
    // 2) Copies the tree and evaluates the mean of u1 on the LL of u0.
    LsShape* ShapeRoot = &tree.shapes[0];
    
    //find the root
    while(ShapeRoot->find_parent()) {ShapeRoot = ShapeRoot->find_parent();}
    // Recursive copy
    Node ValueRoot(0, 0,0,ShapeRoot->contour.size());
    // A list of pointers mapping the nodes from the nodetree to the nodes of the shapetree
    std::vector<std::pair<Node*, LsShape*>> VectorLinks;
    
    localNodeId = 0; // This line is very important, if you remove it, the function will crash after a few calls
    CreateNodeFromShapeTree(&ValueRoot, ShapeRoot, &VectorLinks);
    
    // 3) Evaluates averages and counts
    double* avg=new double[tree.iNbShapes]();
    int* count=new int[tree.iNbShapes]();
    for(int i = 0; i<n0*n1; ++i)
    {
        LsShape *smallest = tree.smallest_shape(i);
        avg[smallest->shapeId] += uu1[i];
        count[smallest->shapeId]++;
    }
    for(int i = 0; i<tree.iNbShapes; ++i)
    {
        avg[i]/=count[i];
        VectorLinks[i].first->y = avg[i];
        VectorLinks[i].first->w = count[i];
    }
    
    // 4) Call the isotonic regression -> x
   	t_begin=clock();
    Recursive_Tree_Search(ValueRoot);
    t_end=clock();
    DP_time +=  double(t_end - t_begin) / CLOCKS_PER_SEC;
   	//mexPrintf("DP 1:%1.2e \n", DP_time);
    
    // Here, we assign x to tree.
    for (std::pair<Node*, LsShape*> link : VectorLinks)
    {
        link.second->gray = link.first->x;
    }
    
    // 5) Reconstruct an image u from x 
    double *uu=tree.build_image();
    for (int i=0;i<n0;++i){
        for (int j=0;j<n1;++j){
            u[i+j*n0]=uu[j+i*n1];
        }
    }

    delete uu0;
    delete uu1;
    delete avg;
    delete count;
    delete uu;
    
    t_end=clock();
    times[0]=tree_time;
    times[1]=DP_time;
    times[2]=double(t_end - t_ini) / CLOCKS_PER_SEC;
}
