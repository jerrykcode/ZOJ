# ZOJ Problem Set - 1008 Gnome Tetravex

[原题地址](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemId=8)
## Description

Hart is engaged in playing an interesting game, Gnome Tetravex, these days. In the game, at the beginning, the player is given n\*n squares. Each square is divided into four triangles marked four numbers (range from 0 to 9). In a square, the triangles are the left triangle, the top triangle, the right triangle and the bottom triangle. For example, Fig. 1 shows the initial state of 2\*2 squares.

![Fig.1](https://i.loli.net/2018/08/18/5b77e23da0d91.jpg)

*Fig. 1 The initial state with 2\*2 squares*

The player is required to move the squares to the termination state. In the termination state, any two adjoining squares should make the adjacent triangle marked with the same number. Fig. 2 shows one of the termination states of the above example.

![](https://i.loli.net/2018/08/18/5b77e2906cc57.jpg)

*Fig. 2 One termination state of the above example*  

It seems the game is not so hard. But indeed, Hart is not accomplished in the game. He can finish the easiest game successfully. When facing with a more complex game, he can find no way out.

One day, when Hart was playing a very complex game, he cried out, "The computer is making a goose of me. It's impossible to solve it." To such a poor player, the best way to help him is to tell him whether the game could be solved. If he is told the game is unsolvable, he needn't waste so much time on it.


## Input

The input file consists of several game cases. The first line of each game case contains one integer n, 0 <= n <= 5, indicating the size of the game.

The following n\*n lines describe the marking number of these triangles. Each line consists of four integers, which in order represent the top triangle, the right triangle, the bottom triangle and the left triangle of one square.

After the last game case, the integer 0 indicates the termination of the input data set.


## Output

You should make the decision whether the game case could be solved. For each game case, print the game number, a colon, and a white space, then display your judgment. If the game is solvable, print the string "Possible". Otherwise, please print "Impossible" to indicate that there's no way to solve the problem.

Print a blank line between each game case.

**Note: Any unwanted blank lines or white spaces are unacceptable.**


## Sample Input

2  
5 9 1 4  
4 4 5 6  
6 8 5 4  
0 4 4 3  
2  
1 1 1 1  
2 2 2 2  
3 3 3 3  
4 4 4 4  
0  


## Output for the Sample Input

Game 1: Possible  

Game 2: Impossible  

## 题意

一个n \* n网格，网格中每个正方形被它的两条对角线分割成4个三角形，每个三角形中有数字。问是否可能，用这些正方形重组网格，使网格中每两个个相邻的正方形中，相邻的三角形中的数字相等。上图1为初始网格，图2为重组后的网格。

## 思路

DFS，对n\*n的重组网格，从(0, 0)开始尝试填入正方形。

超时处理，参考[“Gnome Tetravex游戏, ZOJ1008” 的一种解法(下)](https://blog.csdn.net/PosPro/article/details/51405683)，对重复出现的正方形只进行一次DFS递归。

## 参考
[“Gnome Tetravex游戏, ZOJ1008” 的一种解法(下)](https://blog.csdn.net/PosPro/article/details/51405683)

## 代码
[GnomeTetravex.cpp](https://github.com/jerrykcode/ZOJ/blob/master/Vol1/1008/GnomeTetravex.cpp)
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

typedef unsigned char BYTE; //Numbers range from 0 to 9

typedef struct Square { //正方形
    Square(BYTE top, BYTE right, BYTE bottom, BYTE left)
        : top(top), right(right), bottom(bottom), left(left) {}
    BYTE top;
    BYTE right;
    BYTE bottom;
    BYTE left;
} *PSquare;

int getSquareValue(PSquare pSquare) {
    return *(int *)pSquare; //returns the value composed by top, right, bottom and left
}

typedef struct Grid { //格子，包括正方形及该正方形出现的次数
    Grid(PSquare pSquare, BYTE count) : pSquare(pSquare), count(count) {}
    PSquare pSquare;
    BYTE count; //Number of times the square appeared，square重复出现的次数
} *PGrid;

class GnomeTetravex {
public:
    GnomeTetravex() {}
    ~GnomeTetravex();
    void init(int n);
    bool canSolve();
private:
    int n;
    vector<PGrid> grids;  //Grids in the game 
    PSquare **solution;    //Solution of the game
    void release();
    bool dfs(int i, int j);
};

GnomeTetravex::~GnomeTetravex() {
    release();
    vector<PGrid>().swap(grids);
}

void GnomeTetravex::release() { //释放内存
    for (int i = 0; i < grids.size(); i++) {
        delete grids[i]->pSquare;
        grids[i]->pSquare = NULL;
        delete grids[i];
        grids[i] = NULL;
    }
    grids.clear();
    free(solution);
}

void GnomeTetravex::init(int n) {
    static int count = 0;
    if (count++) release();
    this->n = n;
    solution = new PSquare*[n];
    BYTE top, right, bottom, left;
    int t, r, b, l;
    int value;
    bool flag;
    for (int i = 0; i < n; i++) {
        solution[i] = new PSquare[n];
        for (int j = 0; j < n; j++) {
            solution[i][j] = NULL; //solution[i][j]存储grids中的PGrid指针指向的Grid结构体中的PSquare指针
            scanf("%d %d %d %d", &t, &r, &b, &l); //输入正方形[i][j]中的三角形的值
            top = (BYTE)(t & 255);
            right = (BYTE)(r & 255);
            bottom = (BYTE)(b & 255);
            left = (BYTE)(l & 255);
            value = top + (right << 8) + (bottom << 16) + (left << 24); //三角形组成的数值
            flag = false;
            for (PGrid pGrid : grids) { //已存储的格子
                if (getSquareValue(pGrid->pSquare) == value) { //正方形重复
                    pGrid->count++; //重复次数++
                    flag = true;
                    break;
                }
            }
            if (!flag) { //已存储的格子中没有正方形与当前输入的正方形相等
                PSquare pSquare = new Square(top, right, bottom, left);
                PGrid pGrid = new Grid(pSquare, 1);
                grids.push_back(pGrid);
            }
        }
    }
}

bool GnomeTetravex::canSolve() {
    return dfs(0, 0);
}

bool GnomeTetravex::dfs(int i, int j) { //给[i][j]尝试填入正方形
    if (i >= n)
        return true;
    for (PGrid pGrid : grids) { //供选择的正方形
        if (pGrid->count == 0) continue; //该种正方形已用完
		//填入位置的左边存在且左边正方形的right不等于此尝试正方形的left，不合要求
        if (j - 1 >= 0 && solution[i][j - 1]->right != pGrid->pSquare->left) continue;
		//填入位置的上方存在且上方正方形的bottom不等于此尝试正方形的top，不合要求
        if (i - 1 >= 0 && solution[i - 1][j]->bottom != pGrid->pSquare->top) continue;
		//右边和下方尚未填入正方形，故不用判断
        solution[i][j] = pGrid->pSquare; //solution[i][j]与pGrid->pSquare指向相同的内存
        pGrid->count--;
        int next_j = j + 1 < n ? j + 1 : 0; //下一个DFS的位置，判断是否要进入下一行
        int next_i = j + 1 < n ? i : i + 1;
        if (dfs(next_i, next_j)) return true;
        solution[i][j] = NULL; //指向NULL
        pGrid->count++;
    }
    return false;
}

int main()
{
    GnomeTetravex gnomeTetravex;
    int n;
    scanf("%d", &n);
    int count = 1;
    while (n != 0) {
        if (count != 1) printf("\n");
        gnomeTetravex.init(n);
        printf("Game %d: ", count++);
        gnomeTetravex.canSolve() ? printf("Possible\n") : printf("Impossible\n");
        scanf("%d", &n);
    }
    return 0;
}

```
