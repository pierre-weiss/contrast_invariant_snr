#include "isotonic_regression_tree.h"

// FUNCTIONS ASSOCIATED TO STRUCT NODE
Node::Node(int s, int i, double y, double w) : sign(s), x(0), y(y), w(w), id(i) {}

void Node::addChildren(Node *n)
{
    children.push_back(n);
}

Node::~Node()
{
    for (auto child : children)
    {
        delete child;
    }
}

void Node::print(int k)
{
    for (int i = 0; i < k; ++i)
    {
        std::cout << "\t";
    }
    std::cout << "s : "<< sign << " -- x : " << x  << ", y : " << y << " -- w : " << w << std::endl;
    for (auto c : children)
    {
        c->print(k+1);
    }
}

// FUNCTIONS ASSOCIATED TO CLASS MESSAGE
Message::Message(int l, double* b, double* s, double sm, double ym, double sp, double yp)
: am(sm), bm(ym), ap(sp), bp(yp)
{
    breakpoints.clear();
    slope.clear();
    for (int i = 0; i < l; i++)
    {
        breakpoints.push_back(b[i]);
        slope.push_back(s[i]);
    }
}

void Message::print()
{
    using namespace std;
    cout << "Message" << endl;
    for (int i = 0; i < length(); i++)
    {
        cout << breakpoints[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < length(); i++)
    {
        cout << slope[i] << " ";
    }
    cout << endl;
    cout << " am, bm, " << am << ", " << bm << endl;
    cout << " ap, bp, " << ap << ", " << bp << endl;
    
}

// MAIN CODE 
Message fusion(Message& m1, const Message& m2)
{
    Message res;
    res.am = m1.am + m2.am;
    res.bm = m1.bm + m2.bm;
    res.ap = m1.ap + m2.ap;
    res.bp = m1.bp + m2.bp;
    int i = 0, j = 0;
    for (i=0, j=0; i < m1.length() && j < m2.length();)
    {
        if (m1.breakpoints[i] < m2.breakpoints[j])
        {
            res.breakpoints.push_back(m1.breakpoints[i]);
            res.slope.push_back(m1.slope[i]);
            ++i;
        }
        else
        {
            res.breakpoints.push_back(m2.breakpoints[j]);
            res.slope.push_back(m2.slope[j]);
            ++j;
        }
        // Beware, we do not treat the case where the x-coord of the breakpoints are equal
    }
    for (;i < m1.length(); ++i)
    {
        res.breakpoints.push_back(m1.breakpoints[i]);
        res.slope.push_back(m1.slope[i]);
    }
    for (;j < m2.length(); ++j)
    {
        res.breakpoints.push_back(m2.breakpoints[j]);
        res.slope.push_back(m2.slope[j]);
    }
    
    return res;
}

/* Given a message m (describing a nondecreasing piecewise linear function f) and a sign s, this function
 * stores the result of the inf-convolution g defined for all y by:
 * g(y) = inf_{x, s*(x-y)>=0} f(x)
 * */
double infConvolution(Message &m, int s)
{
    double x = 0;
    double a, b = 0;
    
    if (s >= 0) // must be larger than  parent
    {
        a = m.am; b = m.bm;
        while(m.length() > 0 && a*m.breakpoints.front()+b < 0)
        {
            double tmp_s = m.slope.front();
            
            a += tmp_s;
            m.slope.pop_front();
            
            b += -m.breakpoints.front()*tmp_s;
            m.breakpoints.pop_front();
        }
        x = -b/a;
        m.breakpoints.push_front(x);
        m.slope.push_front(a);
        m.am = 0;
        m.bm = 0;
    }
    else
    {
        a = m.ap; b = m.bp;
        while(m.length() > 0 && a*m.breakpoints.back()+b > 0)
        {
            double tmp_s = m.slope.back();
            
            a -= tmp_s;
            m.slope.pop_back();
            
            b -= -m.breakpoints.back()*tmp_s;
            m.breakpoints.pop_back();
        }
        x = -b/a;
        m.breakpoints.push_back(x);
        m.slope.push_back(-a);
        m.ap = 0;
        m.bp = 0;
    }
    return x;
}

Message searchNode(Node *root)
{
    Message m;
    for (auto child : root->children) // Sum the messages of the children (inf-convolutions)
    {
        m = fusion(m, searchNode(child));
    }
    // Add the offset from the quadratic unary.
    m.am += root->w;
    m.bm -= root->w*root->y;
    m.ap += root->w;
    m.bp -= root->w*root->y;
    // Then return the min convolution of the message
    root->x = infConvolution(m, root->sign);
    return m;
}

void backprop(Node *root, double y)
{
    int s = root->sign;
    double x = root->x;
    if  (s*(x-y) <= 0)
        root->x = y;
    for (auto child : root->children)
    {
        backprop(child, root->x);
    }
    
}

void Recursive_Tree_Search(Node &root)
{
    searchNode(&root);
    for (auto child : root.children)
    {
        backprop(child, root.x);
    }
}

double score(Node *root)
{
    double res = (root->w)*(root->x - root->y)*(root->x - root->y)/2;
    for (auto child : root->children)
    {
        res += score(child);
    }
    return res;
}
