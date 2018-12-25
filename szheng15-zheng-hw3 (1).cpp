//HW3  Due: Friday (10/5) at 11:59PM

//Dijkstra's algorithm
//Implemented with min-Heap
//Time Complexity O((N + E ) log N)

#include <iostream>
#include <vector>
#include <list>
#include <fstream>
using namespace std;
class e_node { //stands for edge node
public:
    int nb;//the neighbor of a currently considered node
    int weight; //weight of the edge to the above neighbor
    e_node() {}//constructor
};
class rt_node { //rt stands for routing table
public:
    int cost;  //cost to a node
    int from;  //the from node to this node
    int checked;
    int h_pos; //the postion in heap for this node
    rt_node() { from = -1;  cost = 9999;  checked = 0; }
};

class h_node { //stands for heap_node
public:
    int id;
    int cost;
    h_node() { id = -1; cost = 9999; }
    h_node(int i, int j) { id = i; cost = j; }
    bool operator<(h_node h) { return (cost < h.cost); }
};

void set_up(vector<rt_node> &rt, vector<h_node> & heap, int &start);
void select_node(vector<list<e_node> > & graph, vector<rt_node> & rt, vector<h_node> &heap);
void neighbor_update(vector<list<e_node> > & graph, vector<rt_node> & rt, vector<h_node> & heap, int &s);

int main() {
    //Do not change code for main function
    ifstream in("text1.txt");
    int start;
    in >> start;  //start is the source node for the algorithm
    int n, e; //n: num of nodes ; e: num of edges
    in >> n >> e;
    list<e_node> le;
    vector<list<e_node> > graph(n, le);
    e_node e1;
    int n1, n2, w;
    for (int i = 0; i < e; i++) {
        in >> n1 >> n2 >> w;
        e1.nb = n2;
        e1.weight = w;
        graph[n1].push_back(e1);
    }
    in.close();
    vector<rt_node> rt(n); //rt stands for routing table
    vector<h_node> heap(n);
    
    set_up(rt, heap, start);//initialize values
    select_node(graph, rt, heap);
    
    for (int i = 0; i < n; i++) {
        
        cout << "cost from " << start << " to " << i << " is: " << rt[i].cost << "  from node is " << rt[i].from << endl;
    }
    getchar();
    getchar();
    return 0;
}


void set_up(vector<rt_node> &rt, vector<h_node> & heap, int &start) {
    
    //Initialization values as you see fit
    rt[start].from = start;
    rt[start].cost = 0;
    for(size_t i = 0; i < heap.size(); i++){
        heap[i].id = i;
        rt[i].h_pos = i;
        rt[i].checked = 0;
    }
    heap[start].id = 0;
    heap[0].id = start;
    heap[0].cost = 0;
    rt[0].h_pos = start;
    rt[start].h_pos = 0;
}


void neighbor_update(vector<list<e_node> > & graph, vector<rt_node> & rt, vector<h_node> & heap, int &s) {
    //this function is to be invoked by select_node
    
    
    //If this code gets too long, you can move some part of it out side and form a separate function.
    for(int i = s - 1; i > 0; i--){
        int temp = (i - 1) * 0.5;
        int minID = temp;
        int leftChild, rightChild;
        if(i % 2){
            leftChild = i;
            if(heap[leftChild].cost < heap[temp].cost){
                minID = leftChild;
            }
        }else{
            leftChild = i - 1;
            rightChild = i;
            if(heap[rightChild].cost < heap[temp].cost){
                minID = rightChild;
            }
            if(heap[leftChild].cost < heap[temp].cost && heap[leftChild].cost < heap[rightChild].cost){
                minID = leftChild;
            }
        }
        if(temp != minID){
            int temp_cost = heap[temp].cost;
            int temp_id;
            heap[temp].cost = heap[minID].cost;
            heap[minID].cost = temp_cost;
            temp_id = heap[temp].id;
            heap[temp].id = heap[minID].id;
            heap[minID].id = temp_id;
        }
    }
    rt[heap[0].id].checked = 1;
}



void select_node(vector<list<e_node> > & graph, vector<rt_node> & rt, vector<h_node> &heap) {
    //Continue to select an unvisited node with minimum cost, and doing some general update
    //Invoke neighbor_udpate to possibly update neighbors
    
    //If this code gets too long, you can move some part of it out side and form a separate function.
    for(int i = 1; i < graph.size(); i++){
        h_node temp;
        temp = heap.back();
        heap.pop_back();
        heap.push_back(heap[0]);
        heap[0] = temp;
        rt[heap.back().id].h_pos = heap.size();
        int minID = heap.back().id;
        int minCost = heap.back().cost;
        list<e_node>::iterator it1 = graph[minID].begin();
        while(it1 != graph[minID].end()){
            int updateNb = it1->nb;
            int newWeight = it1->weight;
            if(rt[updateNb].cost > (newWeight + minCost)){
                rt[updateNb].cost = newWeight + minCost;
                rt[updateNb].from = minID;
            }
            it1++;
        }
        heap.pop_back();
        
        int size_of_heap = heap.size();
        for(int i = 0; i < heap.size(); i++){
            heap[i].cost = rt[heap[i].id].cost;
        }
        neighbor_update(graph, rt, heap, size_of_heap);
    }
}
/*
 //sample screenshot
 cost from 0 to 0 is: 0  from node is 0
 cost from 0 to 1 is : 7  from node is 2
 cost from 0 to 2 is : 4  from node is 0
 cost from 0 to 3 is : 10  from node is 1
 cost from 0 to 4 is : 14  from node is 3
 cost from 0 to 5 is : 9  from node is 1
 */

/*
 //sample input file text1.txt
 0
 6
 12
 0 5 16
 0 4 15
 0 2 4
 0 3 12
 0 1 9
 1 3 3
 1 5 2
 2 3 14
 2 1 3
 3 4 4
 4 5 1
 5 3 2
 */
