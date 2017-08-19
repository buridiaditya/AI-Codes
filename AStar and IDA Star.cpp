#include <iostream>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <queue>
#include <vector>
#include <ctime>
#include <cmath>
#define INF 1000000

using namespace std;

typedef struct node{
	int state[3][3];
	int visited;	
	int cost;
	int Gcost;
	struct 	node* parentNode;	
}node;

typedef struct Compare {
    bool operator()(node const & p1, node const & p2) {
        return p1.cost > p2.cost;
    }
}Compare;

int H1(int currentState[3][3]){
	return 0;
}

int H2(int currentState[3][3]){
	int cost = 0;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			switch(currentState[i][j]){
				case 1:
					cost += abs(i-0)+abs(j-0);
					break;
				case 2:
					cost += abs(i-0)+abs(j-1);
					break;
				case 3:
					cost += abs(i-0)+abs(j-2);
					break;
				case 4:
					cost += abs(i-1)+abs(j-0);
					break;
				case 5:
					cost += abs(i-1)+abs(j-1);
					break;
				case 6:
					cost += abs(i-1)+abs(j-2);
					break;
				case 7:
					cost += abs(i-2)+abs(j-0);
					break;
				case 8:
					cost += abs(i-2)+abs(j-1);
					break;
				default:	
					// cost += std::abs(i-2)+std::abs(j-2);
					break;
			}
		}
	}
	return cost;
}

int H3(int currentState[3][3]){
	int cost = 0;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(3*i+j+1 != currentState[i][j] && currentState[i][j] != -1)
				cost++;
		}
	}
	return cost;
}

int H4(int currentState[3][3]){
	int cost = 0;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			switch(currentState[i][j]){
				case 1:
					cost += sqrt( std::abs(i-0)*std::abs(i-0)) + (std::abs(j-0)*std::abs(j-0) );
					break;
				case 2:
					cost += sqrt( std::abs(i-0)*std::abs(i-0) + std::abs(j-1)*std::abs(j-1) );
					break;
				case 3:
					cost += sqrt( std::abs(i-0)*std::abs(i-0) + std::abs(j-2)*std::abs(j-2) );
					break;
				case 4:
					cost += sqrt( std::abs(i-1)*std::abs(i-1) + std::abs(j-0)*std::abs(j-0) );
					break;
				case 5:
					cost += sqrt(std::abs(i-1)*std::abs(i-1)+std::abs(j-1)*std::abs(j-1));
					break;
				case 6:
					cost += sqrt(std::abs(i-1)*std::abs(i-1)+std::abs(j-2)*std::abs(j-2));
					break;
				case 7:
					cost += sqrt(std::abs(i-2)*std::abs(i-2)+std::abs(j-0)*std::abs(j-0));
					break;
				case 8:
					cost += sqrt(std::abs(i-2)*std::abs(i-2)+std::abs(j-1)*std::abs(j-1));
					break;
				default:	
					// cost += std::abs(i-2)+std::abs(j-2);
					break;
			}
		}
	}
	return cost;
}

void init(node* S,int readD[3][3],int (*H)(int[3][3])){
	S->visited = 0;
	S->Gcost = 0;
	S->parentNode = NULL;
	for(int i = 0; i < 3;i++){
		for(int j = 0; j < 3; j++)
			S->state[i][j] = readD[i][j];
	}
	S->cost = H(S->state);
	return;
}

void reset(unordered_map<int,node*> &hash,priority_queue<node,vector<node>,Compare> &Aqueue){
	while(!Aqueue.empty()) Aqueue.pop();
	for(unordered_map<int,node*>::iterator it = hash.begin(); it != hash.begin(); it++){
		delete it->second;
	}
	hash.clear();
	return;
}

void printState(int state[3][3]){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			cout << state[i][j] << " ";
		cout << "\n";
	}
	return;
}

int computeKey(int state[3][3]){
	int key = 0;
	int po = 1;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(state[i][j] != -1)
				key += po*state[i][j];
			po *= 10;
		}
	}
	return key;
}

int computePathLength(node state){
	int len = 0;
	while(state.parentNode != NULL){
		state = *state.parentNode;
		len++;
	}
	return len;
}

inline bool checkEnd(int state[3][3]){
	return (state[0][0] == 1) && (state[0][1] == 2) && (state[0][2] == 3) && (state[1][0] == 4)
			&& (state[1][1] == 5) && (state[1][2] == 6) && (state[2][0] == 7) && (state[2][1] == 8) 
			&& (state[2][2] == -1);
}


int AStar(node* startState,unordered_map<int,node*> &hash,priority_queue<node,vector<node>,Compare> &Aqueue,int (*H)(int[3][3])){
	int key,noOfNodes = 0;
	node start;
	Aqueue.push(*startState);
	key = computeKey(startState->state);
	hash[key] = startState;
	while(!Aqueue.empty()){
		start = Aqueue.top();
		Aqueue.pop(); 
		key = computeKey(start.state);
		startState = hash[key];
		if(startState->visited == 2)
			continue;
		startState->visited = 2;
		if(checkEnd(start.state)){
			cout << "No Of Nodes Expanded : " << noOfNodes << endl;
			cout << "Path Length : " << computePathLength(start) << endl;
			return noOfNodes;
		}
		else{
			noOfNodes++;
			int temp[3][3];
			for(int i = 0; i < 3; i++)
				for(int j = 0 ; j < 3; j++)
					temp[i][j] = start.state[i][j];
			int cost;
			int i = 0,j = 0;
			node* tempNode;
			for(i = 0; i < 3; i++){
				for(j = 0; j < 3; j++)
					if(start.state[i][j] == -1)
						break;
				if(start.state[i][j] == -1)
					break;
			}
			for(int l = -1; l <= 1; l++){
				for(int m = -1; m <= 1; m++){
					if(i+l < 3 && i+l >= 0 && m+j >=0 && m+j < 3 && std::abs(l)+std::abs(m) == 1){
						for(int i1 = 0; i1 < 3; i1++)
							for(int j1 = 0 ; j1 < 3; j1++)
								temp[i1][j1] = start.state[i1][j1];

						temp[i][j] = start.state[i+l][j+m];
						temp[i+l][j+m] = -1;
						key = computeKey(temp);
						cost = start.Gcost+1+H(temp);
						if(hash.find(key) != hash.end()){
							tempNode = hash[key];
							if(cost < tempNode->cost){
								tempNode->cost = cost;
								tempNode->Gcost = start.Gcost+1;
								tempNode->parentNode = startState;
								tempNode->visited = 1;
								Aqueue.push(*tempNode);
							}	
						}
						else{
							tempNode = new node;
							init(tempNode,temp,H);
							hash[key] = tempNode;
							tempNode->cost = cost;
							tempNode->Gcost = start.Gcost+1;
							tempNode->parentNode = startState;
							tempNode->visited = 1;
							Aqueue.push(*tempNode);
						}
					}
				}
			}
		}
	}
	return -1;
}

int IDAStar(node* start,unordered_map<int,bool> &hash,int (*H)(int[3][3]),int &THRESH,int &noOfNodes,bool &set){
	int i = 0,j = 0,cost,key,keyTemp;
	node* tempNode;
	noOfNodes++;
	int min = 1000000000,answer;
	key = computeKey(start->state);
	hash[key] = true;
	if(checkEnd(start->state)){
		cout << "No Of Nodes Expanded : " << noOfNodes << endl;
		cout << "Path Length : " << computePathLength(*start) << endl;
		set = true;
		hash.erase(key);
		return start->cost;
	}

	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++)
			if(start->state[i][j] == -1)
				break;
		if(start->state[i][j] == -1)
			break;
	}
	for(int l = -1; l <= 1; l++){
		for(int m = -1; m <= 1; m++){
			if(i+l < 3 && i+l >= 0 && m+j >=0 && m+j < 3 && std::abs(l)+std::abs(m) == 1){
				tempNode = new node;
				init(tempNode,start->state,H);
				tempNode->state[i][j] = start->state[i+l][j+m];
				tempNode->state[i+l][j+m] = -1;
				tempNode->parentNode = start;
				cost = start->Gcost + 1 + H(tempNode->state);
				tempNode->Gcost = start->Gcost + 1;
				tempNode->cost = cost;
				keyTemp = computeKey(tempNode->state);
				if(cost > THRESH){
					if(min > cost)
						min = cost;
				}
				else if(cost <= THRESH && hash.find(keyTemp) == hash.end()){
					if(set == true){
						delete tempNode;
						hash.erase(key);
						return min;
					}
					answer = IDAStar(tempNode,hash,H,THRESH,noOfNodes,set);
					if(answer < min)
						min = answer;
				}
				delete tempNode;
			}
		}
	}
	hash.erase(key);
	return min;
}

int main(int argc,char** argv){
	node* startNode;
	int readD[3][3];
	int count = 0,noOfCases = 0,noOfNodes = 0,THRESH,temp,st,sst;
	unordered_map<int,node*> hash;
	unordered_map<int,bool> hashS;
	priority_queue<node,vector<node>,Compare> Aqueue;
	fstream S(argv[1],ios::in | ios::out);
	bool answer;

	while(!S.eof()){
		S >> temp;	
		readD[count/3][count%3] = temp;
		count++;
		if(count == 9){
			noOfCases++;
			cout << "Case " << noOfCases << " : " << endl;
			printState(readD);
			cout << "------------Using Heuristic H(n) = 0------------" << endl;

			// ---------------------------------------------A* using heuristic 1---------------------------------------------

			cout << "----Using A* :---- " << endl;

			startNode = new node;
			init(startNode,readD,H1);
			st = clock();
			reset(hash,Aqueue);
			if(AStar(startNode,hash,Aqueue,H1) == -1){
				cout << "No solution found" << endl;
			}	
			sst = clock();
			cout << "Runtime : " << (sst-st)/double(CLOCKS_PER_SEC) << "sec" << endl;

			// ---------------------------------------------IDA* using heuristic 1---------------------------------------------
			cout << "----IDA*:----" << endl;
			
			startNode = new node;
			init(startNode,readD,H1);
			hashS.clear();

			st = clock();
			noOfNodes = 0;answer = false;THRESH = 2;
			while(!answer and THRESH < 30){
				THRESH = IDAStar(startNode,hashS,H1,THRESH,noOfNodes,answer);		
			}	
			if(!answer)
				cout << "No solution found" << endl;
			sst = clock();
			cout << "Runtime : " << (sst-st)/double(CLOCKS_PER_SEC) << "sec" << endl;

			
			cout << "-------------Using Heuristic H(n) = ManHattan Distance------------" << endl;

			// ---------------------------------------------A* using heuisitic 2---------------------------------------------

			cout << "----Using A* :----" << endl;
			st = clock();

			startNode = new node;
			init(startNode,readD,H2);
			reset(hash,Aqueue);		

			if(AStar(startNode,hash,Aqueue,H2) == -1){
				cout << "No solution found" << endl;
			}
			sst = clock();
			cout << "Runtime : " << (sst-st)/double(CLOCKS_PER_SEC) << "sec" << endl;

			// ---------------------------------------------IDA* using heuristic 2---------------------------------------------

			cout << "----IDA*:----" << endl;
			st = clock();

			startNode = new node;
			init(startNode,readD,H2);
			hashS.clear();

			noOfNodes = 0;answer = false;THRESH = 2;
			while(!answer && THRESH < 30){
				THRESH = IDAStar(startNode,hashS,H2,THRESH,noOfNodes,answer);		
			}	
			if(!answer)
				cout << "No solution found" << endl;
			sst = clock();
			cout << "Runtime : " << (sst-st)/double(CLOCKS_PER_SEC) << "sec"  << endl;

			cout << "-------------Using Heuristic H(n) = Tiles Displaced ------------" << endl;
			// ---------------------------------------------A* using heuisitic 3---------------------------------------------

			cout << "----Using A* :----" << endl;
			st = clock();

			startNode = new node;
			init(startNode,readD,H3);
			reset(hash,Aqueue);		

			if(AStar(startNode,hash,Aqueue,H3) == -1){
				cout << "No solution found" << endl;
			}
			sst = clock();
			cout << "Runtime : " << (sst-st)/double(CLOCKS_PER_SEC) << "sec" << endl;

			// ---------------------------------------------IDA* using heuristic 3---------------------------------------------

			cout << "----IDA*:----" << endl;
			st = clock();

			startNode = new node;
			init(startNode,readD,H3);
			hashS.clear();

			noOfNodes = 0;answer = false;THRESH = 2;
			while(!answer && THRESH < 30){
				THRESH = IDAStar(startNode,hashS,H3,THRESH,noOfNodes,answer);		
			}	
			if(!answer)
				cout << "No solution found" << endl;
			sst = clock();
			cout << "Runtime : " << (sst-st)/double(CLOCKS_PER_SEC) << "sec"  << endl;

			cout << "-------------Using Heuristic H(n) = Eucledian Distance------------" << endl;
			// ---------------------------------------------A* using heuisitic 4---------------------------------------------

			cout << "----Using A* :----" << endl;
			st = clock();

			startNode = new node;
			init(startNode,readD,H4);
			reset(hash,Aqueue);		

			if(AStar(startNode,hash,Aqueue,H4) == -1){
				cout << "No solution found" << endl;
			}
			sst = clock();
			cout << "Runtime : " << (sst-st)/double(CLOCKS_PER_SEC) << "sec" << endl;

			// ---------------------------------------------IDA* using heuristic 4---------------------------------------------

			cout << "----IDA*:----" << endl;
			st = clock();

			startNode = new node;
			init(startNode,readD,H4);
			hashS.clear();

			noOfNodes = 0;answer = false;THRESH = 2;
			while(!answer && THRESH < 30){
				THRESH = IDAStar(startNode,hashS,H4,THRESH,noOfNodes,answer);		
			}	
			if(!answer)
				cout << "No solution found" << endl;
			sst = clock();
			cout << "Runtime : " << (sst-st)/double(CLOCKS_PER_SEC) << "sec"  << endl;
			// ------------------------------------------------------------------

			cout << "\n";
			count = 0;
		}
	}
	return 0;
}
