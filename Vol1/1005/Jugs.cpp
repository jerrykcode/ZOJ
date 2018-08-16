
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
