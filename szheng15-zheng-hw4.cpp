//HW4

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
void update_heap(vector<rt_node> & rt, vector<h_node> & heap);

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
    
    int totalCost = 0;
    for (int i = 0; i < n; i++) {
        if(start != i){
            cout << rt[i].from << " " << i << " " << rt[i].cost << endl;
            totalCost += rt[i].cost;
        }
    }
    cout << "The Overall Cost is " << totalCost << endl;
    getchar();
    getchar();
    return 0;
}


void set_up(vector<rt_node> &rt, vector<h_node> & heap, int &start) {
    rt[start].from = start;
    rt[start].cost = 0;
    for(int i = 0; i < heap.size(); i++){
        heap[i].id = i;
        rt[i].h_pos = i;
    }
    heap[start].id = 0;
    heap[0].id = start;
    heap[0].cost = 0;
    rt[0].h_pos = start;
    rt[start].h_pos = 0;
}

void update_heap(vector<rt_node> & rt, vector<h_node> & heap){
    int i = 0;
    if((2*i)+2 <= heap.size()){
        if((2*i)+1 < heap.size() && heap[(2*i)+1].cost < heap[i].cost){
            h_node temp;
            temp.id = heap[(2*i)+1].id;
            temp.cost = heap[(2*i)+1].cost;
            heap[(2*i)+1].cost = heap[i].cost;
            heap[(2*i)+1].id = heap[i].id;
            heap[i].id = temp.id;
            heap[i].cost = temp.cost;
            rt[heap[(2*i)+1].id].h_pos = (2*i)+1;
            rt[heap[i].id].h_pos = i;
            i = (2*i)+1;
        }
        if((2*i)+2 < heap.size() && heap[(2*i)+2].cost < heap[i].cost){
            h_node temp;
            temp.id = heap[(2*i)+2].id;
            temp.cost = heap[(2*i)+2].cost;
            heap[(2*i)+2].id = heap[i].id;
            heap[(2*i)+2].cost = heap[i].cost;
            heap[i].id = temp.id;
            heap[i].cost = temp.cost;
            rt[heap[(2*i)+2].id].h_pos = (2*i)+2;
            rt[heap[i].id].h_pos = i;
            i = (2*i)+2;
            
        }
    }
}

void neighbor_update(vector<list<e_node> > & graph, vector<rt_node> & rt, vector<h_node> & heap, int &s) {
    for(int i = s; i > 0; i--){
        int root = (i-1)/2;
        if(heap[root].cost > heap[i].cost){
            h_node temp;
            rt[heap[i].id].h_pos = root;
            rt[heap[root].id].h_pos = i;
            temp.id = heap[i].id;
            temp.cost = heap[i].cost;
            heap[i].id = heap[root].id;
            heap[root].id = temp.id;
            heap[i].cost = heap[root].cost;
            heap[root].cost = temp.cost;
        }
    }
}



void select_node(vector<list<e_node> > & graph, vector<rt_node> & rt, vector<h_node> &heap) {
    for(int i = 0; i < rt.size(); i++){
        h_node small;
        small = heap[0];
        heap[0].id = heap[(heap.size() - 1)].id;
        heap[0].cost = heap[(heap.size() - 1)].cost;
        rt[heap[0].id].h_pos = 0;
        heap.pop_back();
        //list<e_node>::iterator it1 = graph[small.id].begin();
        for(list<e_node>::iterator it1 = graph[small.id].begin(); it1 != graph[small.id].end(); it1++){
            if(rt[it1->nb].checked != 1){
                int n = it1->nb;
                int w = it1->weight;
                if(rt[n].cost > w){
                    heap[rt[n].h_pos].cost = w;
                    rt[n].from = small.id;
                    rt[n].cost = w;
                    neighbor_update(graph, rt, heap, rt[n].h_pos);
                }
            }
        }
        rt[small.id].checked = 1;
        rt[small.id].h_pos = heap.size() - 1;
        update_heap(rt, heap);
    }
}

