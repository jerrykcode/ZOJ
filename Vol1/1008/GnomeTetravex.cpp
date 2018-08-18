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
