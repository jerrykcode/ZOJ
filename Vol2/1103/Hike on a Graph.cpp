// 思路参考 	https://blog.csdn.net/zxjcarrot/article/details/22981847

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <queue>
using namespace std;

//typedef unsigned char BYTE;
//typedef BYTE POS;
typedef short POS;
typedef char Color;

struct Status {
  POS x, y, z;
  int step;
  Status(POS x, POS y, POS z, int step) : x(x), y(y), z(z), step(step) {}
};
bool collected[50][50][50];

int bfs(Color **graph, int n, Status start) {
    queue<Status> q;
    q.push(start);
    while (!q.empty()) {
        Status status = q.front();
        q.pop();
        int x = (int)status.x, y = (int)status.y, z = (int)status.z;
        int step = status.step;
        if (x == y && y == z) {
            return step;
        }
        for (int p = 0; p < n; p++) {
            if (p != x) {
                if (graph[x][p] == graph[y][z] && !collected[p][y][z]) {
                    Status s(p, y, z, step + 1);
                    q.push(s);
                    collected[p][y][z] = true;
                }
            }
            if (p != y) {
                if (graph[y][p] == graph[x][z] && !collected[x][p][z]) {
                    Status s(x, p, z, step + 1);
                    q.push(s);
                    collected[x][p][z] = true;
                }
            }
            if (p != z) {
                if (graph[z][p] == graph[x][y] && !collected[x][y][p]) {
                    Status s(x, y, p, step + 1);
                    q.push(s);
                    collected[x][y][p] = true;
                }
            }
        }
    } //while
    return -1;
}

int main() {
	int n;
	POS p1, p2, p3;
	Color **graph;
	while (1) {
    	cin >> n;
    	if (n == 0) break;
    	for (int x = 0; x < 50; x++)
    	    for (int y = 0; y < 50; y++)
    	        for (int z = 0; z < 50; z++)
    	            collected[x][y][z] = false;
    	cin >> p1 >> p2 >> p3;
    	p1--;p2--;p3--;
    	graph = new Color*[n];
    	for (int i = 0; i < n; i++) {
    	    graph[i] = new Color[n];
    	    for (int j = 0; j < n; j++)
    	        cin >> graph[i][j];
    	}
    	Status start(p1, p2, p3, 0);
    	int step = bfs(graph, n, start);
    	if (step == -1)
    	    cout << "impossible" << endl;
    	else 
    	    cout << step << endl;
    	for (int i = 0; i < n; i++)
    	    free(graph[i]);
    	free(graph);
	}
	return 0;
}
