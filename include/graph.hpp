#ifndef _GRAPH_HPP
#define _GRAPH_HPP

#include "vector.hpp"
template<class T>
class Vector;

class Edge{
public:
	int a, b, dist;
	Edge(){};
	Edge(int i, int j, int e){
        this->a = i;
        this->b = j;
        this->dist = e;
    }
	~Edge(){}
};

class Graph{
private:
	int** shortest_time;
	int max_vertex;
public:
	Graph(Vector<Edge*> *edges, int max_vertex){
		this->max_vertex = max_vertex;
		this->FloydWarshall(edges);
		// std::cout<<max_vertex<<"\n";
		// std::cout<<"Graph START\n";
		// std::cout<<*this;
		// std::cout<<"Graph DONE\n";
	}

	friend std::ostream& operator<< (std::ostream&, const Graph&);


	int shortest(int a, int b)const{
		return shortest_time[a][b];
	}
	int max_id()const{
		return this->max_vertex;
	}
	
	void FloydWarshall(Vector<Edge*> *edges){
		shortest_time = new int*[max_vertex+3];
		// std::cout<<"floydwarshall\n";
		for(int i=0; i<max_vertex+3; i++){
			shortest_time[i] = new int[max_vertex+3];
		}
		int a, b, dis;
		for(int i=0; i<edges->size(); i++){
			a = (*edges)[i]->a;
            b = (*edges)[i]->b;
            dis = (*edges)[i]->dist;
			shortest_time[b][a] = shortest_time[a][b] = dis;
		}
		// std::cout<<"startalgo\n";
		for(int k=1; k<=max_vertex; k++){
			for(int i=1; i<=max_vertex; i++){
				for(int j=1; j<=max_vertex; j++){
					if((shortest_time[i][k] != 0 && shortest_time[k][j] != 0) 
					&& (shortest_time[i][j] > (shortest_time[i][k] + shortest_time[k][j]))){
						shortest_time[i][j] = (shortest_time[i][k] + shortest_time[k][j]);
						shortest_time[j][i] = shortest_time[i][j];
					}
				}
			}
		}
	}
	
	~Graph(){
		for(int i=0; i<max_vertex+3; i++){
			delete shortest_time[i];
		}
		delete shortest_time;
	}
};

std::ostream& operator<< (std::ostream& out, const Graph& g){
    out << std::setfill(' ');
	for(int i=1; i<=g.max_id(); i++){
		for(int j=1; j<=g.max_id(); j++){
			out << std::setw(5) << g.shortest(i,j);
		}
		out << "\n";
	}
	return out;
}

#endif
