#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <queue>
#include <string>
#include <math.h>
#include <vector> 
#include <stack>

using namespace std;

class AdjMatrix{
private:
	int n;
	vector< vector<int> > adj;
	vector<bool> visited;
	stack <int> S;

public:
	AdjMatrix(int n = 0){
		this->n = n;

		for(int i = 0; i < n; ++i){
			visited.push_back(false);	//initialize all of the nodes are NOT  visited

			vector<int> row;

			for(int j =0; j < n; ++j){
				row.push_back(0);	//O means there is no adjacent vertex.
			}
			//cout << "row size = " <<row.size() <<endl;
			adj.push_back(row);
		}
		//cout << "Total nodes from adj size = " <<adj.size() <<endl;
	}

	AdjMatrix(const AdjMatrix& originAdj){
		n = originAdj.getN();
		adj = originAdj.getAdj();
		visited = originAdj.getVisited();
	}

	int getN() const {
		return n;
	}

	vector < vector<int> > getAdj() const {
		return  adj;
	}

	vector<bool> getVisited() const {
		return visited;
	}
	void addEdge(int origin, int destin){
		if(origin > n || destin > n || origin < 0 || destin < 0){	//invalid situations
			cout << "Invalid Edge" <<endl;
		}
		else{
			adj.at(origin - 1).at(destin - 1) = 1;	//add an edge
			adj.at(destin - 1).at(origin - 1) = 1;	//add an Edge
			//UNDIRECTED GRAPH
		}
	}

	void display(){
		for(int i = 0; i < n; ++i){
			int a = i + 1;
			cout << a;
			for(int j = 0; j <n; ++j){
				int b = j + 1;
				if(adj.at(i).at(j) == 1)
				cout  << " -> " << b;
			}
			cout <<endl;
		}
		return;
	}

	bool checkAvaiableEdge(int u){
		for(int i = 0; i < n; ++i){
			if(adj.at(u).at(i) == 1)
				return true;
		}
		return false;
	}

	bool allVerticesVisited(){
		for(int i  = 0; i  < n; ++i){
			if(!visited.at(i)){
				//cout << "1 vertex is visited." <<endl;
				return false;
			}
		}
		return true;
	}

	bool allEdgesVisited(){
		for(int i = 0; i < n; ++i){
			for(int j = 0; j < n; j++){
				if(adj.at(i).at(j) == 1)
					return false;
			}
		}
		return true;
	}

	bool Search(int u){
		S.push(u);
		//cout << "u just pushed in = " <<u <<endl;
		visited.at(u) = true;
		if(!checkAvaiableEdge(u)){
			if(allVerticesVisited() && allEdgesVisited()){	//if all vertices are visited and there is available edge to go, it is true if all edges are visited
				//cout << "current u = " << u <<endl;
				//cout << "all are visited" <<endl;
				return true;
			}
			else{	//it is false if they are not visited
				//cout << "u just popped out = " <<u <<endl;
				S.pop();
				return false;
			}
		}

		for(int i = 0; i < n; i++){
			if(adj.at(u).at(i) == 1){
				adj.at(u).at(i) = 0;
				adj.at(i).at(u) = 0;

				bool f = Search(i);
				if(f){
					//cout << "f is True" <<endl;
					return true;
				}
				else{
					adj.at(u).at(i) = 1;
					adj.at(i).at(u) = 1;
				}
			}
		}
		//cout << "u just popped out = " <<u <<endl;
		S.pop();
		return false;

	}


	void clearStack(){
		int size = S.size();
		for(int i = 0; i < size; i++){
			S.pop();
		}
	}

	void printPath(){
		//cout << "S size = " << S.size() <<endl;
		cout << "One possible path is ";
		int size = S.size();
		for(int i = 0; i < size; i++){
			cout << (S.top() +1) << " ";
			S.pop();
		}
		cout <<endl;
	}

};



int main(){
	/*
	AdjMatrix AM(5);

	cout << "initialize an AM" << endl;

	AM.addEdge(1,2);
	AM.addEdge(2,3);
	AM.addEdge(3,1);
	AM.addEdge(2,4);
	AM.addEdge(4,5);
	AM.addEdge(5,2);
	AM.addEdge(1,4);
	AM.addEdge(3,5);

	cout << "all edges added sucessfully." <<endl;
	AM.display();
	
	*/
 	
	
 	string fileName;	//file input
	cout << "Please input our file name: ";
	cin >> fileName;

	int vertices;
	int edges;

	int origin;
	int destin;

	ifstream inFS(fileName.c_str());
	
	if(!inFS.is_open())
  		cerr << "ERROR: File Failed to Open!" <<endl;	//if failed to read the fileName

	inFS >> vertices;
	inFS >> edges;

	AdjMatrix AM(vertices);


	for(int i = 0; i < edges; i++){
		inFS >> origin;
		inFS >> destin;

		AM.addEdge(origin,destin);
	}

	inFS.close();


	cout << "vertices = " <<vertices <<endl;
	cout << "edges = " <<edges <<endl;
	
	

	AM.display();
	


	//////////////////////////
	//////////////////////////
	//////////////////////////
	vector<int> oddNodes;
		for(int i = 0; i < AM.getAdj().size();  i++){	
			int count = 0;

			for(int j =0; j < AM.getAdj().size(); j++){	//count how many edges a vertex has
				if(AM.getAdj().at(i).at(j) == 1)
				count++;
			}
			if(count % 2 == 1)	//if there is an odd node, push it in 
				oddNodes.push_back(i);
		}

		if((oddNodes.size() != 2) && (oddNodes.size() != 0)){	//if number of odd nodes are NOT 0 and 2, no solutions
			cout << "There is no solution." <<endl;
			//AM.printPath();
		}
		else if (oddNodes.size() == 2){
			cout << "2 odd nodes means Eulerian path(start from one odd vertex and end with the other odd vertex." <<endl;
			for(int i = 0; i < oddNodes.size(); i++){
				AdjMatrix fakeAM = AM;
				//cout << "i = " << i <<endl;
				if(fakeAM.Search(oddNodes.at(i))){
					fakeAM.printPath();
				}
				else{
					fakeAM.clearStack();
				}
			}
		}
		else if(oddNodes.size() == 0){
			cout << "0 odd nodes means Eulerian cycle(start from one vertex and end the same vertex)." <<endl;
			for(int j = 0; j < AM.getAdj().size(); j++){
				AdjMatrix fakeAM = AM;
				//cout << "j = " << j <<endl;
				if(fakeAM.Search(j)){
					fakeAM.printPath();
				}
				else{
					fakeAM.clearStack();
				}
			}
		}


		//////////////////////////
		//////////////////////////
		//////////////////////////

	
	
	return 0;
}
