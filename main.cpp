/*r-rishabh-j*/
#include<bits/stdc++.h>
#include<time.h> 
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<queue>
#include<list>
#include<iterator>
#include"graph.h"
using namespace std;

typedef long long int lint;

typedef list<heap_node*>::iterator list_ptr;
typedef list<fib_heap_node*>::iterator list_ptr2;

//function prototypes
list<heap_node*> binomial_heap_make_heap(vector<heap_node>& nodes, vector<heap_node*>& h_pointer, vector<node>& g_nodes, lint no_of_nodes);
void binomial_heap_decrease_key(list<heap_node*>& roots, vector<heap_node>& nodes, vector<heap_node*>& h_pointer, vector<node>& g_nodes, lint index, lint new_val);
list_ptr binomial_heap_update_min(list<heap_node*>& roots,vector<node>& g_nodes);
void binomial_heap_consolidate(list<heap_node*>& roots,vector<node>& g_nodes); //function to merge the trees of same rank in binomial heap.
lint binomial_heap_extract_min(list<heap_node*>& roots,list_ptr min_ptr,vector<node>& g_nodes);
void binomial_heap_dijkstra_algo(vector<node> &vertex,lint source,lint no_of_nodes, bool D);

//fibonacci heap functions
list_ptr2 fibonacci_heap_update_min(list<fib_heap_node*>& roots,vector<node>& g_nodes);
void fibonacci_heap_consolidate(list<fib_heap_node*>& roots,vector<node>& g_nodes) ;//function to merge ;the trees of same rank in fibonacci heap.
lint fibonacci_heap_extract_min(list<fib_heap_node*>& roots,list_ptr min_ptr,vector<node>& g_nodes); //fibonacci extract min
void fibonacci_heap_decrease_key(list<fib_heap_node*>& roots,vector<fib_heap_node>& nodes,vector<node>& g_nodes,lint index,lint new_val);
void fibonacci_heap_dijkstra_algo(vector<node> &vertex,lint source,lint no_of_nodes, bool D);

//binary heap functions
void binary_heap_percolate_up(lint index, vector<bin_heap> &nodes, vector<bin_heap*> &h_pointer, vector<node> &vertex, lint no_of_nodes); //iterative function to heapify up
void binary_heap_decrease_key(lint index,lint new_distance, vector<bin_heap> &nodes, vector<bin_heap*> &h_pointer, vector<node> &vertex, lint no_of_nodes); //decrease key to heapify due to changes in dist values
void binary_heap_percolate_down(lint index, vector<bin_heap> &nodes, vector<bin_heap*> &h_pointer, vector<node> &vertex); //recursive function to heapify down
lint binary_heap_extract_min(vector<bin_heap> &nodes, vector<bin_heap*> &h_pointer, vector<node> &vertex, lint no_of_nodes);
void binary_heap_dijkstra_algo(vector<node> &vertex,lint source,lint no_of_nodes, bool D);

//array based implementation
lint array_min_dist_node(vector<node> &vertex, lint no_of_nodes); //for array based implementation
void array_based_dijkstra_algo(vector<node> &vertex,lint source, lint no_of_nodes, bool D);

lint bellman_ford(vector<node> &vertex, lint no_of_nodes, bool D);

void binomial_heap_decrease_key(list<heap_node*>& roots, vector<heap_node>& nodes, vector<heap_node*>& h_pointer, vector<node>& g_nodes, lint index, lint new_val)
{
    //decrease key and then percolate up, and then update min
    g_nodes[index-1].distance=new_val; /*assign new distance value*/
    if(h_pointer[index-1]->parent!=NULL)
    {   
        heap_node* parent=h_pointer[index-1]->parent;
        heap_node* current_node=h_pointer[index-1];
        while(parent!=NULL && (g_nodes[index-1].distance<g_nodes[parent->index-1].distance || (index<parent->index && g_nodes[index-1].distance==g_nodes[parent->index-1].distance)))
        {
            h_pointer[parent->index-1]=h_pointer[index-1];
            h_pointer[parent->index-1]->index=parent->index;
            parent->index=index;
            h_pointer[index-1]=parent;
            parent=parent->parent;
        }
    }
}

list_ptr binomial_heap_update_min(list<heap_node*>& roots,vector<node>& g_nodes)
{
    //traverse thru list to update min
    list_ptr min_ptr=roots.begin();
    if(min_ptr==roots.end())
    {
        return min_ptr;
    }
    list_ptr ptr=roots.begin();
    ptr++;
    lint min=g_nodes[(*min_ptr)->index-1].distance;
    for(;ptr!=roots.end();ptr++)
    {
        if(g_nodes[(*ptr)->index-1].distance<min || (g_nodes[(*ptr)->index-1].distance==min && (*ptr)->index<(*min_ptr)->index))         
        {
            min=g_nodes[(*ptr)->index-1].distance;
            min_ptr=ptr;
        }
    }
    return min_ptr;
}

void binomial_heap_consolidate(list<heap_node*>& roots,vector<node>& g_nodes) /*function to merge the trees of same rank in binomial heap.*/
{   
    /*binomial heap merge operation
    merge trees of same rank together to maintain no. of nodes=2^K in each heap
    returns update min_pointer*/
    /*To do-
    1. Update rank
    2. update parent
    3. delete the list node which is now a child
    */
    vector<merge_binomial> rank_ptr(64); /*vector for ranks. Maximum rank in the very worst case can be 64*/
    list_ptr iter=roots.begin();    //iterator to the list
    while(iter!=roots.end())
    {   
        lint current_rank=(*iter)->children.size(); //rank of the current root

        if(rank_ptr[current_rank].truth==0)     /*if truth==0 means ptr==NULL*/
        {
            rank_ptr[current_rank].ptr=iter++; /*if such a rank does not exist, then store the pointer*/
            rank_ptr[current_rank].truth=1;
        }
        else if((*(rank_ptr[current_rank].ptr))!=(*iter)) /*if such a rank exists and the pointer contained is not the current pointer*/
        {   
            /*if the prev root of the same rank contains node with distance higher than the current root, or if
            both have the same distance value and the previous root has a higher index then make the previous root a child of the current root.*/
            if(g_nodes[(*(rank_ptr[current_rank].ptr))->index-1].distance>g_nodes[(*iter)->index-1].distance || (g_nodes[(*(rank_ptr[current_rank].ptr))->index-1].distance==g_nodes[(*iter)->index-1].distance && (*iter)->index<(*(rank_ptr[current_rank].ptr))->index))   //change index to weight later
            {
                (*iter)->children.push_back(*rank_ptr[current_rank].ptr);  //push the node with greater index to the one with higher index
                (*(rank_ptr[current_rank].ptr))->parent=(*iter);              //make the root the parent of the node inserted in the child vector
                
                roots.erase(rank_ptr[current_rank].ptr);  //delete list node having ptr to the node which is now a child
                rank_ptr[current_rank].truth=0; /*make rank pointer NULL*/
            }
            else    
            {
                (*(rank_ptr[current_rank].ptr))->children.push_back(*iter);  //push the node with greater index to the one with higher index
                (*iter)->parent=(*(rank_ptr[current_rank].ptr));              //make the root the parent of the node inserted in the child vector
                
                rank_ptr[current_rank].truth=0;     /*make rank pointer NULL*/
                roots.erase(iter);                  //delete list node having ptr to the node which is now a child
                iter=rank_ptr[current_rank].ptr;    //shift iterator to the root whose rank has been increased.
            }
        }
        else 
        {
            iter++; /*increment the iterator ahead if no consolidation has to be made.*/
        }
    }
}

lint binomial_heap_extract_min(list<heap_node*>& roots,list_ptr min_ptr,vector<node>& g_nodes)
{
    /*/delete min, push children to the root list, consolidate and then update min
    //returns index of the min dist node*/
    if(min_ptr==roots.end())
    {
        return -1;
    }
    vector<heap_node*> children=(*min_ptr)->children;
    lint min_val_g_node_index=(*min_ptr)->index;
    roots.erase(min_ptr);
    for(lint i=0;i<children.size();i++)
    {
        roots.push_front(children[i]);
        children[i]->parent=NULL;
    }
    binomial_heap_consolidate(roots, g_nodes); /*merge operation*/

    if(g_nodes[min_val_g_node_index-1].distance==999999)
    return -1;
    else
    return min_val_g_node_index;
}

void binomial_heap_dijkstra_algo(vector<node> &vertex,lint source, lint no_of_nodes, bool D)
{   
    vector<heap_node> Heap_nodes(no_of_nodes);
    vector<heap_node*> h_pointer(no_of_nodes);

    list<heap_node*> roots;    
    for(lint i=0;i<no_of_nodes;i++)  //make binomial heap
    {   
        vertex[i].distance=999999;
        Heap_nodes[i].index=i+1;
        Heap_nodes[i].parent=NULL;
        h_pointer[i]=&Heap_nodes[i];
        roots.push_front(&Heap_nodes[i]);
    }
    vertex[source-1].distance=0;  //set source vertex distance to 0 
    binomial_heap_consolidate(roots,vertex); //consolidate the heap

    for(lint i=0;i<no_of_nodes;i++)
    {
        list_ptr min_ptr=binomial_heap_update_min(roots,vertex);
        lint index=binomial_heap_extract_min(roots,min_ptr,vertex);  /*heap operation to get minimum distance node*/
        if(index==-1)/*break if no node available or unreachable node is there*/
        {
            break;
        }
        else
        {   /*relax operation*/
            lint _size=vertex[index-1].adj_list.size();
            for(lint j=0;j<_size;j++)
            {   
                lint wt=vertex[index-1].adj_list[j].weight;
                if(wt<0)
                {   
                    cout<<"-1\n";
                    return;
                }
                if(vertex[index-1].distance!=999999 && (wt + vertex[index-1].distance < vertex[index-1].adj_list[j].child->distance))
                {
                    vertex[index-1].adj_list[j].child->distance=wt + vertex[index-1].distance;
                    binomial_heap_decrease_key(roots,Heap_nodes,h_pointer,vertex,vertex[index-1].adj_list[j].child->index,vertex[index-1].adj_list[j].child->distance);
                }
            }
        }
    }

    for(lint i=0;i<no_of_nodes;i++)
    {   
        if(vertex[i].distance==999999)
        cout<<"999999 ";
        else
        cout<<vertex[i].distance+vertex[i].h_value-vertex[source-1].h_value<<" ";
    }
    cout<<"\n";
}

//fibonacci heap functions
list_ptr2 fibonacci_heap_update_min(list<fib_heap_node*>& roots,vector<node>& g_nodes)
{
    //traverse thru list to update min
    list_ptr2 min_ptr=roots.begin();
    if(min_ptr==roots.end())
    {
        return min_ptr;
    }
    list_ptr2 ptr=roots.begin();
    ptr++;
    lint min=g_nodes[(*min_ptr)->index-1].distance;
    for(;ptr!=roots.end();ptr++)
    {
        if(g_nodes[(*ptr)->index-1].distance<min || (g_nodes[(*ptr)->index-1].distance==min && (*ptr)->index<(*min_ptr)->index))        
        {
            min=g_nodes[(*ptr)->index-1].distance;
            min_ptr=ptr;
        }
    }
    return min_ptr;
}

void fibonacci_heap_consolidate(list<fib_heap_node*>& roots,vector<node>& g_nodes) //function to merge the trees of same rank in fibonacci heap.
{   
    //fibonacci heap merge operation
    //merge trees of same rank together
    //returns update min_pointer
    /*To do-
    1. Update rank
    2. update parent
    3. delete the list node which is now a child
    */
    vector<merge_fib> rank_ptr(64);  /*golden ratio rule*/
    list_ptr2 iter=roots.begin();    //iterator to the list
    while(iter!=roots.end())
    {   
        (*iter)->parent=NULL;
        lint current_rank=(*iter)->children.size(); //rank of the current root

        if(rank_ptr[current_rank].truth==0)
        {   
            rank_ptr[current_rank].ptr=iter++; /*/if such a rank does not exist, then store pointer*/
            rank_ptr[current_rank].truth=1;
        }
        else if((*(rank_ptr[current_rank].ptr))!=(*iter)) /*/if such a rank exists and the pointer contained by vector is not the current pointer*/
        {   
            //if the prev root of the same rank contains node with distance higher than the current root, or if
            //both have the same distance value and the previous root has a higher index then make the previous root a child of the current root.
            if(g_nodes[(*(rank_ptr[current_rank].ptr))->index-1].distance>g_nodes[(*iter)->index-1].distance || (g_nodes[(*(rank_ptr[current_rank].ptr))->index-1].distance==g_nodes[(*iter)->index-1].distance && (*iter)->index<(*(rank_ptr[current_rank].ptr))->index))   //change index to weight later
            {
                (*iter)->children.push_back(*(rank_ptr[current_rank].ptr));  //push the node with greater index to the one with higher index
                (*(rank_ptr[current_rank].ptr))->parent=(*iter);              //make the root the parent of the node inserted in the child list
                list_ptr2 child_list_iter=(*iter)->children.end();
                child_list_iter--;
                (*(rank_ptr[current_rank].ptr))->child_list_pos=child_list_iter;

                roots.erase(rank_ptr[current_rank].ptr);  //delete list node having ptr to the node which is now a child
                rank_ptr[current_rank].truth=0;
            }
            else    
            {
                (*(rank_ptr[current_rank].ptr))->children.push_back(*iter);  //push the node with greater index to the one with higher index
                (*iter)->parent=(*(rank_ptr[current_rank].ptr));              //make the root the parent of the node inserted in the child list
                list_ptr2 child_list_iter=(*(rank_ptr[current_rank].ptr))->children.end();
                child_list_iter--;
                (*iter)->child_list_pos=child_list_iter;

                roots.erase(iter);                         //delete list node having ptr to the node which is now a child/
                rank_ptr[current_rank].truth=0;
                iter=rank_ptr[current_rank].ptr;  //shift iterator to the root whose rank has been increased.
            }
        }
        else 
        {
            iter++; //increment the iterator ahead if no consolidation has to be made.
        }    
    }
}

lint fibonacci_heap_extract_min(list<fib_heap_node*>& roots,list_ptr2 min_ptr,vector<node>& g_nodes) //fibonacci extract min
{
    /*delete min, push children to the root list, consolidate.
    returns index of the min dist node*/
    if(min_ptr==roots.end())
    {
        return -1;
    }
    lint min_val_g_node_index=(*min_ptr)->index;
    
    roots.splice(roots.begin(),(*min_ptr)->children);
    roots.erase(min_ptr);
    fibonacci_heap_consolidate(roots, g_nodes); /*consolidating in extract min*/
    if(g_nodes[min_val_g_node_index-1].distance==999999)
    return -1;
    else
    return min_val_g_node_index;
}

void fibonacci_heap_decrease_key(list<fib_heap_node*>& roots,vector<fib_heap_node>& nodes,vector<node>& g_nodes,lint index,lint new_val)
{
    g_nodes[index-1].distance=new_val; /*assign new value*/
    if(nodes[index-1].parent!=NULL)
    {
        fib_heap_node* current_node=&nodes[index-1];
        fib_heap_node* parent=nodes[index-1].parent;
        if(g_nodes[current_node->index-1].distance<g_nodes[parent->index-1].distance || (current_node->index<parent->index && g_nodes[current_node->index-1].distance==g_nodes[parent->index-1].distance))
        {   
            while(1)
            {   
                current_node->parent->children.erase(current_node->child_list_pos); /*erase current node from parent's list*/

                current_node->parent=NULL;
                current_node->marking=false; /*unmark node and push into root list*/
                roots.push_front(current_node);
                /*if new parent is null or parent is unmarked then stop, otherwise continue*/
                if(parent->parent==NULL)
                {   
                    break;
                }
                else if(parent->marking==false)
                {
                    parent->marking=true;
                    break;
                }
                current_node=parent;
                parent=parent->parent;
            }    
        }
    }
}

void fibonacci_heap_dijkstra_algo(vector<node> &vertex,lint source,lint no_of_nodes, bool D)
{   
    vector<fib_heap_node> Heap_nodes(no_of_nodes); /*heap nodes*/
    list<fib_heap_node*> roots; /*pointer to heap nodes for O(1) find*/
    list_ptr2 init_;

    for(lint i=0;i<no_of_nodes;i++)  /*initialising the heap*/
    {
        vertex[i].distance=999999;
        Heap_nodes[i].index=i+1;
        Heap_nodes[i].parent=NULL;
        Heap_nodes[i].marking=0;
        roots.push_front(&Heap_nodes[i]);
        if(i==source-1)
        {
            init_=roots.end();
            init_--;
        }
    }
    vertex[source-1].distance=0;  /*/set source vertex distance to 0*/ 

    for(lint i=0;i<no_of_nodes;i++)
    {
        list_ptr2 min_ptr;//=fibonacci_heap_update_min(roots,vertex);
        if(i==0)
        min_ptr=init_;
        min_ptr=fibonacci_heap_update_min(roots,vertex);
        lint index=fibonacci_heap_extract_min(roots,min_ptr,vertex);  /*heap operation !*/
        if(index==-1)
        {
            break;
        }
        else
        {   
            /*relax operations*/
            lint _size=vertex[index-1].adj_list.size();
            for(lint j=0;j<_size;j++)
            {   
                lint wt=vertex[index-1].adj_list[j].weight;
                if(wt<0)
                {
                    return;
                }
                if(vertex[index-1].distance!=999999 && (wt + vertex[index-1].distance < vertex[index-1].adj_list[j].child->distance))
                {
                    vertex[index-1].adj_list[j].child->distance=wt + vertex[index-1].distance;
                    fibonacci_heap_decrease_key(roots,Heap_nodes,vertex,vertex[index-1].adj_list[j].child->index,vertex[index-1].adj_list[j].child->distance);
                }
            }
        }
    }
    for(lint i=0;i<no_of_nodes;i++)
    {   
        if(vertex[i].distance==999999)
        cout<<"999999 ";
        else
        cout<<vertex[i].distance+vertex[i].h_value-vertex[source-1].h_value<<" ";
    }
    cout<<"\n";
}

lint bellman_ford(vector<node> &vertex, lint no_of_nodes, bool D)
{
    node source; //extra node inderted in the graph to find h[v] for all the nodes
    source.index=0;
    source.h_value=0;
    for(lint i=0;i<no_of_nodes;i++)
    {   //creating new edges between the new node and the other nodes
        edge_vertex temp_node;
        temp_node.child=&vertex[i];
        temp_node.original_weight=0;
        source.adj_list.push_back(temp_node);
    }

    for(lint iter=0;iter<no_of_nodes;iter++) //bellman ford algorithm starting from the source
    {
        bool is_change_inner=false;
        for(lint k=-1;k<no_of_nodes;k++)
        {
            node* temp_root; 

            if(k==-1)
            temp_root=&source;
            else
            {
                temp_root=&vertex[k];
            }
            /* relax operations for all the connected vertex*/
            for(lint i=0;i<temp_root->adj_list.size();i++)
            {   
                if(temp_root->h_value!=999999 && (temp_root->h_value + temp_root->adj_list[i].original_weight<temp_root->adj_list[i].child->h_value))
                {   
                    is_change_inner=true;
                    temp_root->adj_list[i].child->h_value=temp_root->h_value + temp_root->adj_list[i].original_weight; //relax step
                }
            }
        }
        if(is_change_inner==false)
        {   
             break;
        }
        else if(is_change_inner==true && iter==no_of_nodes-1)
        {
            return -1;
        }
    }
    return 1;
}

//binary heap functions
void binary_heap_percolate_up(lint index, vector<bin_heap> &nodes, vector<bin_heap*> &h_pointer, vector<node> &vertex, lint no_of_nodes) //iterative function to heapify up
{   
    //O(logV) operation to percolate the vertex upwards.
    lint current_heap_pos=h_pointer[index-1]->heap_position; //0 indexing
    if(current_heap_pos!=0)
    {
        lint parent_pos=(current_heap_pos-1)/2;
        lint parent_graph_index=nodes[parent_pos].index;
        while(vertex[index-1].distance<vertex[parent_graph_index-1].distance || (vertex[index-1].distance==vertex[parent_graph_index-1].distance && index<parent_graph_index))
        {
            h_pointer[parent_graph_index-1]=&nodes[current_heap_pos];  
            h_pointer[parent_graph_index-1]->index=parent_graph_index;
            h_pointer[parent_graph_index-1]->heap_position=current_heap_pos;

            h_pointer[index-1]=&nodes[parent_pos];
            h_pointer[index-1]->heap_position=parent_pos;
            h_pointer[index-1]->index=index;

            if(h_pointer[index-1]->heap_position==0)
            break;
            else
            {
                current_heap_pos=h_pointer[index-1]->heap_position;
                parent_pos=(current_heap_pos-1)/2;
                parent_graph_index=nodes[parent_pos].index;
            }
            
        }
    }
}

void binary_heap_decrease_key(lint index,lint new_distance, vector<bin_heap> &nodes, vector<bin_heap*> &h_pointer, vector<node> &vertex, lint no_of_nodes) /*decrease key to heapify due to changes in dist values*/
{
    vertex[index-1].distance=new_distance;
    binary_heap_percolate_up(index,nodes,h_pointer,vertex,no_of_nodes);
}

void binary_heap_percolate_down(lint index, vector<bin_heap> &nodes, vector<bin_heap*> &h_pointer, vector<node> &vertex) /*recursive function to heapify down*/
{   
    /*O(logV) operation*/
    lint heap_size=nodes.size();
    lint heap_pos=h_pointer[index-1]->heap_position;
    lint lch=-1;
    lint rch=-1;
    if(2*heap_pos+1<heap_size)
    lch=2*heap_pos+1;
    if(2*heap_pos+2<heap_size)
    rch=2*heap_pos+2;

    if(lch==-1 && rch==-1)
    return;
    else if(lch==-1 && rch!=-1)
    {//percolate down right
        if(vertex[nodes[rch].index-1].distance<vertex[index-1].distance || (nodes[rch].index<index && (vertex[nodes[rch].index-1].distance==vertex[index-1].distance)))
        {
            nodes[heap_pos].index=nodes[rch].index;
            h_pointer[nodes[rch].index-1]=&nodes[heap_pos];
            nodes[rch].index=index;
            h_pointer[index-1]=&nodes[rch];
            binary_heap_percolate_down(index,nodes,h_pointer,vertex);
        }
        else
        return;
    }
    else if(lch!=-1 && rch==-1)
    {//percolate down left
        if(vertex[nodes[lch].index-1].distance<vertex[index-1].distance || (nodes[lch].index<index && (vertex[nodes[lch].index-1].distance==vertex[index-1].distance)))
        {
            nodes[heap_pos].index=nodes[lch].index;
            h_pointer[nodes[lch].index-1]=&nodes[heap_pos];
            nodes[lch].index=index;
            h_pointer[index-1]=&nodes[lch];
            binary_heap_percolate_down(index,nodes,h_pointer,vertex);
        }
        else
        return;
    }
    else
    { //check which one if smaller and then percolate
        //1. check the smaller one. compare with 
        //2. equal. then check whether to do percolate. if also equal, then check indeces. if node has bigger index than the one with smaller index, then percolate down.
        lint rch_index=nodes[rch].index;
        lint lch_index=nodes[lch].index;
        lint rch_dist=vertex[nodes[rch].index-1].distance;
        lint lch_dist=vertex[nodes[lch].index-1].distance;

        if(vertex[index-1].distance<min(rch_dist,lch_dist))
        return;
        else
        {
            if(rch_dist==lch_dist)
            {
                if(vertex[index-1].distance>rch_dist)
                {
                    //percolate
                    if(rch_index<lch_index)
                    {
                        nodes[heap_pos].index=nodes[rch].index;
                        h_pointer[nodes[rch].index-1]=&nodes[heap_pos];
                        nodes[rch].index=index;
                        h_pointer[index-1]=&nodes[rch];
                        binary_heap_percolate_down(index,nodes,h_pointer,vertex);
                    }
                    else
                    {
                        nodes[heap_pos].index=nodes[lch].index;
                        h_pointer[nodes[lch].index-1]=&nodes[heap_pos];
                        nodes[lch].index=index;
                        h_pointer[index-1]=&nodes[lch];
                        binary_heap_percolate_down(index,nodes,h_pointer,vertex);    
                    }
                }
                else 
                {   
                    if(lch_index<index && lch_index<rch_index)
                    {
                        //go left
                        nodes[heap_pos].index=nodes[lch].index;
                        h_pointer[nodes[lch].index-1]=&nodes[heap_pos];
                        nodes[lch].index=index;
                        h_pointer[index-1]=&nodes[lch];
                        binary_heap_percolate_down(index,nodes,h_pointer,vertex);
                    }
                    else if (lch_index>rch_index && rch_index<index)
                    {
                        //go right
                        nodes[heap_pos].index=nodes[rch].index;
                        h_pointer[nodes[rch].index-1]=&nodes[heap_pos];
                        nodes[rch].index=index;
                        h_pointer[index-1]=&nodes[rch];
                        binary_heap_percolate_down(index,nodes,h_pointer,vertex);
                    }
                    else
                    return;                
                }
            }
            else 
            {
                if(rch_dist<lch_dist)
                {   
                    if(vertex[index-1].distance==rch_dist && index<rch_index)
                    return;
                    else
                    {
                        nodes[heap_pos].index=nodes[rch].index;
                        h_pointer[nodes[rch].index-1]=&nodes[heap_pos];
                        nodes[rch].index=index;
                        h_pointer[index-1]=&nodes[rch];
                        binary_heap_percolate_down(index,nodes,h_pointer,vertex);
                    }
                }
                else
                {   
                    if(vertex[index-1].distance==lch_dist && index<lch_index)
                    return;
                    else
                    {
                        nodes[heap_pos].index=nodes[lch].index;
                        h_pointer[nodes[lch].index-1]=&nodes[heap_pos];
                        nodes[lch].index=index;
                        h_pointer[index-1]=&nodes[lch];
                        binary_heap_percolate_down(index,nodes,h_pointer,vertex);
                    }   
                }   
            }
        }   
    }
}

lint binary_heap_extract_min(vector<bin_heap> &nodes, vector<bin_heap*> &h_pointer, vector<node> &vertex, lint no_of_nodes)
{   
    /*/O(logV operation)*/
    if(nodes.empty())
    return -1;

    lint min_dist_node=nodes[0].index;
    if(nodes.size()==1)
    {
        nodes.pop_back();
        if(vertex[min_dist_node-1].distance==999999)
        return -1;
        else
        return min_dist_node;
    }

    vector<bin_heap>:: iterator ptr=nodes.end();
    ptr--; //this points to the last element in the heap
    
    nodes[0].index=(*ptr).index;
    h_pointer[(*ptr).index-1]=&nodes[0];
    nodes.pop_back();
    
    binary_heap_percolate_down(nodes[0].index,nodes,h_pointer,vertex); //percolate down, O(logV operation)

    if(vertex[min_dist_node-1].distance==999999)
    return -1;
    else
    return min_dist_node;
}

void binary_heap_dijkstra_algo(vector<node> &vertex,lint source,lint no_of_nodes, bool D)
{   
    vector<bin_heap> Heap_nodes(no_of_nodes); /*the nodes in the heap corresponding to the graph nodes*/
    vector<bin_heap*> h_pointer(no_of_nodes); /*vector of pointer to the heap nodes for find() in O(1) time*/
    
    /*initialising the heap*/
    for(lint i=0;i<no_of_nodes;i++)
    {
        Heap_nodes[i].index=i+1;
        Heap_nodes[i].heap_position=i;
        h_pointer[i]=&Heap_nodes[i];
        vertex[i].distance=999999;
    }
    vertex[source-1].distance=0;

    binary_heap_percolate_up(source,Heap_nodes,h_pointer,vertex,no_of_nodes); /* Make heap in O(logV) time since the source has minimum distance.*/
    
    for(lint i=0;i<no_of_nodes;i++)
    {
        lint index=binary_heap_extract_min(Heap_nodes,h_pointer,vertex,no_of_nodes);  //heap operation !
        if(index==-1)
        {
            break;
        }
        else
        {   /*relax operations*/
            lint _size=vertex[index-1].adj_list.size();
            for(lint j=0;j<_size;j++)
            {   
                lint wt=vertex[index-1].adj_list[j].weight;
                if(wt<0)
                {   
                    cout<<"-1\n";
                    return;
                }
                if(vertex[index-1].distance!=999999 && (wt + vertex[index-1].distance < vertex[index-1].adj_list[j].child->distance))
                {
                    vertex[index-1].adj_list[j].child->distance=wt + vertex[index-1].distance;
                    binary_heap_decrease_key(vertex[index-1].adj_list[j].child->index,vertex[index-1].adj_list[j].child->distance,Heap_nodes,h_pointer,vertex,no_of_nodes);
                }
            }
        }
    }
    for(lint i=0;i<no_of_nodes;i++)
    {   
        if(vertex[i].distance==999999)
        cout<<"999999 ";
        else
        cout<<vertex[i].distance+vertex[i].h_value-vertex[source-1].h_value<<" ";
    }
    cout<<"\n";
}

/*/array based implementation*/
lint array_min_dist_node(vector<node> &vertex, lint no_of_nodes) /*/for array based implementation*/
{   
    /*this function returns an unvisited min distance node for dijsktra
    if no node is available or node other than at infinity is available, then this returns -1.
    */
    lint min=999999;
    lint min_index=-1;
    for(lint i=0;i<no_of_nodes;i++) //O(V) fixed
    {
        if(vertex[i].status==0 && vertex[i].distance<min)
        {
            min=vertex[i].distance;
            min_index=i+1;
        }
    }
    return min_index;
}

void array_based_dijkstra_algo(vector<node> &vertex,lint source, lint no_of_nodes, bool D)
{   
    for(lint i=0;i<no_of_nodes;i++)
    {
        vertex[i].distance=999999; /*set distance of all nodes as infinity and mark unvisited*/
        vertex[i].status=0;  
    }
    vertex[source-1].distance=0;  //set source vertex distance to 0 

    for(lint i=0;i<no_of_nodes;i++)
    {
        lint index=array_min_dist_node(vertex,no_of_nodes);  /*/heap operation, O(V) complexity fixed*/
        
        if(index==-1) 
        {
            break; /*no reachable node left*/
        }
        else
        {   
            /*relax operations of all the vertices in the adjacency list*/
            lint _size=vertex[index-1].adj_list.size();
            for(lint j=0;j<_size;j++)
            {   
                lint wt=vertex[index-1].adj_list[j].weight;
                if(wt<0)
                {
                    cout<<"-1\n";
                    return;
                }
                if(vertex[index-1].distance!=999999 && (wt + vertex[index-1].distance < vertex[index-1].adj_list[j].child->distance))
                {
                    vertex[index-1].adj_list[j].child->distance=wt + vertex[index-1].distance;
                }
            }
            vertex[index-1].status=1;
        }
    }
    for(lint i=0;i<no_of_nodes;i++)
    {   
        if(vertex[i].distance==999999)
        cout<<"999999 ";
        else
        cout<<vertex[i].distance+vertex[i].h_value-vertex[source-1].h_value<<" ";
    }
    cout<<"\n";
}


int main(int argc, char** argv)
{   
    int heap_type=4;
    if(argc>1)
    heap_type=atoi(argv[1]);
    if(heap_type>4 || heap_type<1) /*if invalid type given then fibonacci will be exectued*/
    heap_type=4;
    
    int test_cases=0;
    cin>>test_cases;
    clock_t time[test_cases];
    for(int ROUND=0;ROUND<test_cases;ROUND++)
    {
        lint no_of_nodes=0;
        cin>>no_of_nodes;
            bool is_directed=1;
            cin>>is_directed;

            if(no_of_nodes<1)
            {
             cout<<-1<<endl;
             continue;
            }

            vector<node> graph(no_of_nodes);
            bool invalid=0;
            lint edge_weights;
            for(lint i=0;i<no_of_nodes;i++)
            {   
                graph[i].index=i+1;
                for(lint j=0;j<no_of_nodes;j++)
                {
                    cin>>edge_weights;
                    if(edge_weights<0 && is_directed==0) /*if the graph is undirected and the graph has a negative edge weight, then neg cycle exists*/
                    {
                        invalid=true;
                    }
                    else
                    {
                        if(edge_weights!=999999 && i!=j)/*here, make edge iff edge weight is not 999999 or if i!=j*/
                        {   
                            edge_vertex temp;
                            temp.child=&graph[j];
                            temp.original_weight=edge_weights;
                            graph[i].adj_list.push_back(temp);
                        }
                    }
                }
            }
            time[ROUND]=clock();
            if(no_of_nodes==1)
            {
                cout<<"0\n";
                time[ROUND]=clock()-time[ROUND];
                continue;
            }
            if(invalid==true)
            {   
                cout<<"-1\n";
                time[ROUND]=clock()-time[ROUND]; 
                continue;
            }
            int valid=bellman_ford(graph,no_of_nodes,is_directed);
            if(valid==-1) /*if valid==-1, the negative cycle exists. Then dont go for V times disjktra.*/
            {
                cout<<"-1\n";
                time[ROUND]=clock()-time[ROUND]; 
                continue;
            }
            for(lint i=0;i<no_of_nodes;i++)
            {
                graph[i].normalize_weight(); /*makinge edge weigths positive for each edge*/
            }

             /*V times dijkstra*/   
                if(heap_type==1) /*/array*/
                {
                    for(lint i=0;i<no_of_nodes;i++)
                    array_based_dijkstra_algo(graph,i+1,no_of_nodes,is_directed);
                }
                else if(heap_type==2)  /*/binary heap*/
                {
                    for(lint i=0;i<no_of_nodes;i++)
                    binary_heap_dijkstra_algo(graph,i+1,no_of_nodes,is_directed);
                }
                else if(heap_type==3) /*binomial heap based*/
                {
                    for(lint i=0;i<no_of_nodes;i++)
                    binomial_heap_dijkstra_algo(graph,i+1,no_of_nodes,is_directed);
                }
                else if(heap_type==4) /*fibonacci heap based*/
                {
                    for(lint i=0;i<no_of_nodes;i++)
                    fibonacci_heap_dijkstra_algo(graph,i+1,no_of_nodes,is_directed);
                }

            time[ROUND]=clock()-time[ROUND];       
    }   

        for(int i=0;i<test_cases;i++)
        {
            double time_taken = ((double)time[i])/CLOCKS_PER_SEC;
            cout<< fixed << time_taken << setprecision(6)<<" ";
        } 
        cout<<endl;
}