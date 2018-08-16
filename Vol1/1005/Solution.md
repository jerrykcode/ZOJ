# ZOJ Problem Set - 1005 Jugs

[原题地址](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemId=5)

* Time Limit: 2 Seconds      
* Memory Limit: 65536 KB      
* Special Judge

## Description

In the movie "Die Hard 3", Bruce Willis and Samuel L. Jackson were confronted with the following puzzle. They were given a 3-gallon jug and a 5-gallon jug and were asked to fill the 5-gallon jug with exactly 4 gallons. This problem generalizes that puzzle.

You have two jugs, A and B, and an infinite supply of water. There are three types of actions that you can use: (1) you can fill a jug, (2) you can empty a jug, and (3) you can pour from one jug to the other. Pouring from one jug to the other stops when the first jug is empty or the second jug is full, whichever comes first. For example, if A has 5 gallons and B has 6 gallons and a capacity of 8, then pouring from A to B leaves B full and 3 gallons in A.

A problem is given by a triple (Ca,Cb,N), where Ca and Cb are the capacities of the jugs A and B, respectively, and N is the goal. A solution is a sequence of steps that leaves exactly N gallons in jug B. The possible steps are

>fill A  
fill B  
empty A  
empty B  
pour A B  
pour B A  
success  

where "pour A B" means "pour the contents of jug A into jug B", and "success" means that the goal has been accomplished.

You may assume that the input you are given does have a solution.

## Input

Input to your program consists of a series of input lines each defining one puzzle. Input for each puzzle is a single line of three positive integers: Ca, Cb, and N. Ca and Cb are the capacities of jugs A and B, and N is the goal. You can assume 0 < Ca <= Cb and N <= Cb <=1000 and that A and B are relatively prime to one another.

## Output

Output from your program will consist of a series of instructions from the list of the potential output lines which will result in either of the jugs containing exactly N gallons of water. The last line of output for each puzzle should be the line "success". Output lines start in column 1 and there should be no empty lines nor any trailing spaces.

## Sample Input
3 5 4  
5 7 3  

## Sample Output
fill B  
pour B A  
empty A  
pour B A  
fill B  
pour B A  
success  
fill A  
pour A B  
fill A  
pour A B  
empty B  
pour A B  
success  

## 题意

给出两个容量分别为CA、CB(0 < CA <= CB)的容器，通过以下操作，使B容器中的水达到N(N <= CB <= 1000)
>fill A    装满A  
fill B     装满B  
empty A    清空A  
empty B    清空B  
pour A B   A倒入B  
pour B A   B倒入A  

## 思路

BFS求从(0, 0)到(0, N)的最短路径。

定义各种Action
```cpp
typedef enum {
    NO_ACTION,
    START,
    FILL_A,
    FILL_B,
    EMPTY_A,
    EMPTY_B,
    POUR_A_TO_B,
    POUR_B_TO_A
} Actions;
```

由于CA、CB、N <= 1000，故用short即可存储
```typedef short VOLUME;```

用```typedef pair<VOLUME, VOLUME> coordinate;```表示一个状态，即coordinate(a, b)表示A中有a单位水，B中有b单位水。

```cpp
typedef struct node {
    coordinate c;
    int action;
} Path;
```
Path结构体表示路径，包括上一个状态以及将上一个状态变为当前状态的操作。path结构体数组存储路径，path[a][b]为到(a, b)状态的路径，path[a][b].c为(a, b)的上一个状态，path[a][b].action为上一个状态到当前状态(a, b)的操作。
初始化：
```cpp
Path p = {coordinate(-1, -1), NO_ACTION};
path[i][j] = p;
```
对于初始状态(0, 0):
```cpp
path[0][0].c = coordinate(-1, -1);
path[0][0].action = START; 
```

BFS中的队列存储的数据类型是coordinate，初始(0, 0)入队，然后对每个出队列的状态c进行各种操作(fill A, fill B , etc)，得到的结果若之前从未出现过则入队(相当于BFS遍历图时，每个出队列的顶点，其相邻且从未出现过的顶点入队)，(a, b)从未出现过即path[a][b].action == NO_ACTION，设得到的结果为(a, b)，则path[a][b].c = c(状态(a, b)是从状态c到达的); path[a][b].action = 当前进行的操作(状态(a, b)通过当前操作到达)。若得到的结果为(x, N)，x为任意数，N为需要的目标，则跳出循环，根据path数组往回找路径。

## 代码

[Jugs.cpp](https://github.com/jerrykcode/ZOJ/blob/master/Vol1/1005/Jugs.cpp)

```cpp

#include <iostream>
using namespace std;
#include <queue>
#include <stack>
#include <string>

typedef enum { //Actions
    NO_ACTION, 
    START,
    FILL_A,
    FILL_B,
    EMPTY_A,
    EMPTY_B,
    POUR_A_TO_B,
    POUR_B_TO_A
} Actions;

typedef short VOLUME; //max volume of a and b is 1000, short is enough
typedef pair<VOLUME, VOLUME> coordinate; //coordinate(va, vb) means there are va volume of water in A and vb in B 
                                         //(va, vb)表示A中有va单位水，B中有vb单位水
typedef struct node { //Path. 表示从一个coordinate状态到另一个状态的路径
    coordinate c;     //The previous coordinate 前一个状态coordinate
    int action;       //The action it took from the previous coordinate to this 从前一个状态到这里用的action
} Path;

class Jug {
public:
    Jug() {}
    ~Jug() {
        if (path) free(path);
    }
    void solvePuzzle(VOLUME ca, VOLUME cb, VOLUME goal);
private:
    VOLUME capacityA, capacityB, goal;
    //路径数组。path[a][b]表示到(a, b)的路径。即：通过path[a][b].action，上一个状态(path[a][b].c)变为了现在的状态(a, b)
    //path array. The previous coordinate (path[a][b].c) changed to the current coordinate(a, b) by the action(path[a][b].action)
    Path **path;
    void init(VOLUME ca ,VOLUME cb, VOLUME goal);
    //Action functions
    coordinate fillA(coordinate c) { return coordinate(capacityA, c.second); }
    coordinate fillB(coordinate c) { return coordinate(c.first, capacityB); }
    coordinate emptyA(coordinate c) { return coordinate(0, c.second); }
    coordinate emptyB(coordinate c) { return coordinate(c.first, 0); }
    coordinate pour(coordinate c, int action);
    void printPath(Path des);
    void bfs();
    string getActionString(int action);
};

void Jug::solvePuzzle(VOLUME ca, VOLUME cb, VOLUME goal) {
    if (goal == 0){
        cout << "success" << endl;
        return;
    }
    init(ca, cb, goal);
    bfs();
}

void Jug::init(VOLUME ca, VOLUME cb, VOLUME goal) {
    static int count = 0;
    capacityA = ca;
    capacityB = cb;
    this->goal = goal;
    if (count) free(path); //Free path except the first time
    count++;
    path = new Path*[capacityA + 1];
    for (int i = 0; i < capacityA + 1; i++) {
        path[i] = new Path[capacityB + 1];
        for (int j = 0; j < capacityB + 1; j++) {
            Path p = {coordinate(-1, -1), NO_ACTION};
            path[i][j] = p;
        }
    }
}

void Jug::bfs() {
    queue<coordinate> q;
    q.push(coordinate(0, 0));
    path[0][0].action = START;
    int flag = 0;
    coordinate c, tmp;
    while (!q.empty()) {
        c = q.front();
        q.pop();
        VOLUME a = c.first, b = c.second;
        int lastAction = path[a][b].action;
        if (lastAction != EMPTY_A) { //If the previous action is "empty A", we should not "fill A" immediately
                                     //若上一个action是将A清空，就不应该立即装满A（剪枝），否则上一个清空无意义
            //Fill A
            tmp = fillA(c);
            if (path[tmp.first][tmp.second].action == NO_ACTION) { //There dose not exists a path to tmp yet
                                                                   //尚不存在到tmp的路径
                path[tmp.first][tmp.second].c = c;
                path[tmp.first][tmp.second].action = FILL_A;
                if (tmp.second == goal) {
                    flag = 1;
                    break;
                }
                q.push(tmp);
            }
        }
        if (lastAction != EMPTY_B) {
            //Fill B
            tmp = fillB(c);
            if (path[tmp.first][tmp.second].action == NO_ACTION) {
                path[tmp.first][tmp.second].c = c;
                path[tmp.first][tmp.second].action = FILL_B;
                if (tmp.second == goal) {
                    flag = 1;
                    break;
                }
                q.push(tmp);
            }
        }
        if (lastAction != FILL_A) {
            //Empty A
            tmp = emptyA(c);
            if (path[tmp.first][tmp.second].action == NO_ACTION) {
                path[tmp.first][tmp.second].c = c;
                path[tmp.first][tmp.second].action = EMPTY_A;
                if (tmp.second == goal) {
                    flag = 1;
                    break;
                }
                q.push(tmp);
            }
        }
        if (lastAction != FILL_B) {
            //Empty B
            tmp = emptyB(c);
            if (path[tmp.first][tmp.second].action == NO_ACTION) {
                path[tmp.first][tmp.second].c = c;
                path[tmp.first][tmp.second].action = EMPTY_B;
                if (tmp.second == goal) {
                    flag = 1;
                    break;
                }
                q.push(tmp);
            }
        }
        //pour a b
        tmp = pour(c, POUR_A_TO_B);
        if (path[tmp.first][tmp.second].action == NO_ACTION) {
            path[tmp.first][tmp.second].c = c;
            path[tmp.first][tmp.second].action = POUR_A_TO_B;
            if (tmp.second == goal) {
                flag = 1;
                break;
            }
            q.push(tmp);
        }
        //pour b a
        tmp = pour(c, POUR_B_TO_A);
        if (path[tmp.first][tmp.second].action == NO_ACTION) {
            path[tmp.first][tmp.second].c = c;
            path[tmp.first][tmp.second].action = POUR_B_TO_A;
            if (tmp.second == goal) {
                flag = 1;
                break;
            }
            q.push(tmp);
        }
    } //while
    if (flag) {
        printPath(path[tmp.first][tmp.second]);
    }
}

coordinate Jug::pour(coordinate c, int action) {
    VOLUME a = c.first, b = c.second;
    switch(action) {
    case POUR_A_TO_B : {
        VOLUME bLeft = capacityB - b;
        if (bLeft >= a) {
            b += a;
            a = 0;
        } else {
            a -= bLeft;
            b = capacityB;
        }
        break;
    }
    case POUR_B_TO_A :{
        VOLUME aLeft = capacityA - a;
        if (aLeft >= b) {
            a += b;
            b = 0;
        } else {
            a = capacityA;
            b -= aLeft;
        }
        break;
    }
    default : {
        a = -1;
        b = -1;
        break;
    }
    }
    return coordinate(a, b);
}

void Jug::printPath(Path des) {
    stack<int> s;
    while (des.action != START) {
        s.push(des.action);
        des = path[des.c.first][des.c.second];
    }
    while (!s.empty()) {
        cout << getActionString(s.top()) << endl;
        s.pop();
    }
    cout << "success" << endl;
}

string Jug::getActionString(int action) {
    switch (action)
    {
    case NO_ACTION : case START: {
        return "";
        break;
    }
    case FILL_A : {
        return "fill A";
        break;
    }
    case FILL_B : {
        return "fill B";
        break;
    }
    case EMPTY_A : {
        return "empty A";
        break;
    }
    case EMPTY_B : {
        return "empty B";
        break;
    }
    case POUR_A_TO_B : {
        return "pour A B";
        break;
    }
    case POUR_B_TO_A : {
        return "pour B A";
        break;
    }
    default:
        break;
    }
    return "";
}

//main

int main()
{
    Jug jug;
    int ca, cb, n;
    while (cin >> ca >> cb >> n)  {
        jug.solvePuzzle(ca, cb, n);
    }
    return 0;
}

```
