#include "p1_5.h"
#include <vector>
#include <string>
#include <deque>
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;
const int MAX = 5;

//hw_1_4
struct compare_polyominoes{
	static void rotate(vector<string>& M){
		int l = (int)M.size();
		int w = (int)M[0].size();
		std::vector<std::string> m(w);
		for(int i=0; i<w; i++) m[i].resize(l);
		for(int i=0; i<w; i++)	for(int j=0; j<l; j++)	m[i][j] = M[j][w-1-i];
		M = m;
	}
	static void reflect(vector<string>& M){
		int l = (int)M.size();
		int w = (int)M[0].size();
		std::vector<std::string> m(l);
		for(int i=0; i<l; i++) m[i].resize(w);
		for(int i=0; i<l; i++)	for(int j=0; j<w; j++)	m[i][j] = M[l-1-i][j];
		M = m;
	}
	static vector<vector<string>> D8(const vector<string>& A){
		vector<vector<string>> group;
		vector<string> cur = A;
		for(int i=0; i<4; i++){	group.push_back(cur);	rotate(cur);}
		reflect(cur);
		for(int i=0; i<4; i++){	group.push_back(cur);	rotate(cur);}
		return group;
	}
	vector<string> findMin(const vector<string>& A) const{
		vector<string> a = A;
		vector<vector<string>> group = D8(a);
		return *min_element(group.begin(), group.end());
	}
	bool operator()(const vector<string>& A0, const vector<string>& B0) const{
		vector<string> A = findMin(A0);
		vector<string> B = findMin(B0);
		return A < B;
	}
};
//1_4_hw

int k; // K, the length and the width of the map
int N; // n, the number of ingredient
int visited[MAX][MAX] = {0};
struct Coordinate{
	int x,y;
};
int dx[4] = {0,-1,0,1};
int dy[4] = {1,0,-1,0};

deque<Coordinate> path;
set<vector<string>, compare_polyominoes> table; 

bool isReach(deque<Coordinate> path){
	//bool up = false;
	bool left = false;
	bool down = false;
	bool right = false;
	for(int i=0; i<path.size(); i++){
		//if(path[i].x == 0) up = true;
		if(path[i].x == k-1) down = true;
		if(path[i].y == 0) left = true;
		if(path[i].y == k-1) right = true;
	}
	return left && down && right;
	//return up && left && down && right;
}

void clearMap(int m){ // delete all m to 0 in visited[][]
	for(int i=0; i<k; i++)
		for(int j=0; j<k; j++)
			if (visited[i][j] == m)
				visited[i][j] = 0;
}

vector<string> dqToVec(deque<Coordinate> path){
	vector<string> s(k);
    for(int i=0; i<k; i++) s[i].resize(k,'0');
    for(int i=0; i<path.size(); i++){
		int x = path[i].x;
		int y = path[i].y;
		s[x][y] = '1';
	}
	return s;
}
/*
void traversal(vector<string> s){
	//print
	cout << endl;
	for(int i=0; i<k; i++) {
		for(int j=0; j<k; j++) cout << s[i][j];
		cout << endl;
	}
}
void traversal(deque<Coordinate> path){
	for(int i=0; i<path.size(); i++){
		cout << '(' << path[i].x << ',' << path[i].y << ')' << "->";
	}
	cout << endl;
}
*/
int demandNum(deque<Coordinate> path){
	int lmax = 10;
	int rmax = 0;
	int dmax = 0;
	for(int i=0; i<path.size(); i++){
		if(path[i].y < lmax) lmax = path[i].y;
		if(path[i].y > rmax) rmax = path[i].y;
		if(path[i].x > dmax) dmax = path[i].x;
	}
	return lmax + 2*k - rmax - dmax - 2;
}

vector<string> mp;
void dfs(int x, int y){ // index intdicate what index this cell is at. 1,2,3...
	Coordinate cur = {x,y};
	//cout << cur.x << cur.y << endl;
	if(!visited[cur.x][cur.y]){
		path.push_back(cur);
		visited[cur.x][cur.y] = path.size();
	}
	if(path.size() == N){ // ending condition
		if(isReach(path)){ // add to set
			vector<string> vec = dqToVec(path);
			table.insert(vec);
		}
		path.pop_back();
		if(visited[cur.x][cur.y] != 1) //when reaching an end, track back
		dfs(path[visited[cur.x][cur.y]-2].x, path[visited[cur.x][cur.y]-2].y);
		return;
	}
	//cut
	if( demandNum(path) > N - path.size()){
		path.pop_back();
		if(visited[cur.x][cur.y] != 1) //when reaching an end, track back
		dfs(path[visited[cur.x][cur.y]-2].x, path[visited[cur.x][cur.y]-2].y);
		return;
	}
	// search its neighbors
	for(int i=0; i<4; i++){
		int a = cur.x+dx[i];
		int b = cur.y+dy[i];
		if(a<0 || a>=k || b<0 || b>=k) continue;
		if(visited[a][b]) continue;
		if(mp[a][b] == 'x') continue;//also the fist line with x 
		dfs(a,b);
		return;
	}
	if(visited[cur.x][cur.y] != 1) //when reaching an end, track back
		dfs(path[visited[cur.x][cur.y]-2].x, path[visited[cur.x][cur.y]-2].y);
	else	// clear marks, it exacute when cur is at start
		{
			clearMap(path.size()+1);
			path.pop_back();
			if(path.size())
				dfs(path[path.size()-1].x, path[path.size()-1].y);
		}
}

int count_polyominoes(int n, int K, vector<string> s) {
	mp = s;
	k = K;
	N = n;
	for(int i=0; i<K; i++){
		if(mp[0][i] != 'x') {
			dfs(0,i);
			mp[0][i] = 'x';
		}
	}
	//for(auto a:table) traversal(a);
	return table.size();
}
