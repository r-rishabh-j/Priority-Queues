#include<bits/stdc++.h>
#include<vector>
#include<queue>
#include<list>
#include<iterator>
using namespace std;
typedef long long int lint;


class node;
struct edge_vertex //graph edge object
{
    node* child; //pointer to the node the edge points to
    lint original_weight;  //stores orginal edge weigths
    lint weight;    //stores new edge weights provided by bellman ford
};
class node //graph node.
{   
    public:
    lint index; //index in graph
    lint distance; //distance from the source
    bool status; //for selection in array based implementation
    lint h_value; //h[v] to change the weights of the edges to make them positive.
    vector<edge_vertex> adj_list; //adj list to store the children along with edge weights.
    node():status(0),index(0),distance(999999),h_value(999999)
    {}
    void normalize_weight() //involved to change edge weights acc to bellman ford algorithm to change edge weights.
    {
        lint size=adj_list.size();
        for(lint i=0;i<size;i++)
        {
            adj_list[i].weight=adj_list[i].original_weight + h_value - adj_list[i].child->h_value; //w[u,v]=w[u,v]+h[u]-h[v]
        }
    }
};

class heap_node //class used for binomial heap
{
    public:
    lint index; //index of the graph node the heap node points to
    vector<heap_node*> children; //children in binomial and fib heaps
    heap_node* parent; //parent in fib and binomial heaps
    //bool marking; //used in fibonacci heaps for marked nodes
    heap_node():index(0),parent(NULL)
    {}
};

class fib_heap_node //fibonacci heaps
{
    public:
    lint index; //index of the graph node the heap node points to
    list<fib_heap_node*> children; //children in binomial and fib heaps
    fib_heap_node* parent; //parent in fib and binomial heaps
    bool marking; //used in fibonacci heaps for marked nodes
    list<fib_heap_node*>::iterator child_list_pos;
    fib_heap_node():index(0),marking(false),parent(NULL)
    {}
};

class bin_heap //used in binary heap
{
    public:
    lint index; //corresponding to the graph node, 1 indexing
    lint heap_position; //corresponding to the position in heap to access the childern and parent, 0 indexing
};

class merge_binomial /*used in merge operation in binomial heap*/
{   
    public:
    bool truth;
    list<heap_node*>::iterator ptr;
    merge_binomial():truth(0)
    {}
};

class merge_fib /*used in merge operation in fib heaps*/
{   
    public:
    bool truth;
    list<fib_heap_node*>::iterator ptr;
    merge_fib():truth(0)
    {}
};
