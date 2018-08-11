# ZOJ Problem Set - 1002 Fire Net

[原题地址](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1002)

* Time Limit: 2 Seconds      
* Memory Limit: 65536 KB

## Description

Suppose that we have a square city with straight streets. A map of a city is a square board with n rows and n columns, each representing a street or a piece of wall.

A blockhouse is a small castle that has four openings through which to shoot. The four openings are facing North, East, South, and West, respectively. There will be one machine gun shooting through each opening.

Here we assume that a bullet is so powerful that it can run across any distance and destroy a blockhouse on its way. On the other hand, a wall is so strongly built that can stop the bullets.

The goal is to place as many blockhouses in a city as possible so that no two can destroy each other. A configuration of blockhouses is legal provided that no two blockhouses are on the same horizontal row or vertical column in a map unless there is at least one wall separating them. In this problem we will consider small square cities (at most 4x4) that contain walls through which bullets cannot run through.

The following image shows five pictures of the same board. The first picture is the empty board, the second and third pictures show legal configurations, and the fourth and fifth pictures show illegal configurations. For this board, the maximum number of blockhouses in a legal configuration is 5; the second picture shows one way to do it, but there are several other ways.

![showImage.jpg](https://i.loli.net/2018/08/11/5b6e7144102db.jpg)

Your task is to write a program that, given a description of a map, calculates the maximum number of blockhouses that can be placed in the city in a legal configuration.

The input file contains one or more map descriptions, followed by a line containing the number 0 that signals the end of the file. Each map description begins with a line containing a positive integer n that is the size of the city; n will be at most 4. The next n lines each describe one row of the map, with a '.' indicating an open space and an uppercase 'X' indicating a wall. There are no spaces in the input file.

For each test case, output one line containing the maximum number of blockhouses that can be placed in the city in a legal configuration.

## Sample input:

4  
.X..  
....  
XX..  
....  
2  
XX  
.X  
3  
.X.  
X.X  
.X.  
3  
...  
.XX  
.XX  
4  
....  
....  
....  
....  
0  
## Sample output:

5  
1  
5  
2  
4  

## 题意

一个n * n(n <= 4)的网格(一个City)中，有"Wall"(图中黑色方块)和"Open Space"(图中空白格)。在"Open Space"中填入最多的"House"(图中黑色圆形)，使没有两个"House"能" destroy each other"。

若两个"House"在同一行或同一列，且他们中间没有"Wall"使他们分隔，则他们" destroy each other"。

图1为初始网格，图2和图3中的"House"不会" destroy each other"，而图4和图5则存在可以" destroy each other"的"House"。

输入初始网格，'X'表示"Wall"，'.'表示"Open Space"，求可能的最大填入的"House"数量。

## 思路

用二维数组存储网格状态:
```cpp
typedef char** CityMap;
```

首先需要两个函数：

* 判断网格中[i][j]位置是否可以填入一个"House" :  
```cpp
bool canPlaceHouse(CityMap cityMap, int i, int j, int n); 
```   
* 在网格中找到[i][j]之后的第一个可以填入"House"的位置，若不存在返回(-1, -1):  
```cpp
pair<int, int> nextPlace(CityMap cityMap, int i, int j, int n) ; 
```   

深度搜索，若[0][0]可以填入"House"则从[0][0]开始，否则从[0][0]之后的第一个可以填入"House"的位置开始，设为[i][j]。递归求[i][j]位置填入与不填入"House"能得到的最大数量

递归函数如下：
```cpp
int dfs(CityMap cityMap, int i, int j, int n, int n_houses); 
```
其中[i][j]为当前需要判断是否填入"House"的位置，n_houses为已经填入的"House"数量，递归时要计算出下一个可以填入"House"的位置：

```cpp
int dfs(CityMap cityMap, int i, int j, int n, int n_houses) {
    if (i == -1)
        return n_houses;
    pair<int, int> block = nextPlace(cityMap, i, j, n); //若[i][j]不填入"House"，下一个可以填入"House"的位置
    int not_chose = dfs(cityMap, block.first, block.second, n, n_houses); //[i][j]不填"House"
    cityMap[i][j] = HOUSE;
    block = nextPlace(cityMap, i, j, n); //若[i][j]填入"House"，下一个可以填入"House"的位置
    int chose = dfs(cityMap, block.first, block.second, n, n_houses + 1); //[i][j]填"House"
    cityMap[i][j] = OPEN;
    return max(not_chose, chose);
}
```

## 代码

[Fire Net.cpp](https://github.com/jerrykcode/ZOJ/blob/master/Vol1/1002/Fire%20Net.cpp)

```cpp
#include <iostream>
using namespace std;

#define OPEN '.'
#define WALL 'X'
#define HOUSE 'O'

typedef char** CityMap;

bool canPlaceHouse(CityMap cityMap, int i, int j, int n) {
    if (cityMap[i][j] != OPEN) return false;
    int i0, j0;
    for (i0 = i - 1; i0 >= 0 && cityMap[i0][j] != WALL; i0--) {
        if (cityMap[i0][j] == HOUSE) return false;
    }
    for (i0 = i + 1; i0 < n && cityMap[j][i0] != WALL; i0++) {
        if (cityMap[i0][j] == HOUSE) return false;
    }
    for (j0 = j - 1; j0 >= 0 && cityMap[i][j0] != WALL; j0--) {
        if (cityMap[i][j0] == HOUSE) return false;
    }
    for (j0 = j + 1; j0 < n && cityMap[i][j0] != WALL; j0++) {
        if (cityMap[i][j0] == HOUSE) return false;
    }
    return true;
}

pair<int, int> nextPlace(CityMap cityMap, int i, int j, int n) {
    for (int i0 = i; i0 < n; i0++)
        for (int j0 = (i0 == i ? j + 1 : 0); j0 < n; j0++)  //First block : (j + 1, i)
            if (canPlaceHouse(cityMap, i0, j0, n)) {
                return pair<int, int>(i0, j0);
            }
    return pair<int, int>(-1, -1);
}

int dfs(CityMap cityMap, int i, int j, int n, int n_houses) {
    if (i == -1)
        return n_houses;
    pair<int, int> block = nextPlace(cityMap, i, j, n);
    int not_chose = dfs(cityMap, block.first, block.second, n, n_houses);
    cityMap[i][j] = HOUSE;
    block = nextPlace(cityMap, i, j, n);
    int chose = dfs(cityMap, block.first, block.second, n, n_houses + 1);
    cityMap[i][j] = OPEN;
    return max(not_chose, chose);
}

int main() 
{
    int n;
    cin >> n;
    while (n) {
        CityMap cityMap = new char *[n];
        for (int i = 0; i < n; i++) {
            cityMap[i] = new char[n];
            for (int j = 0; j < n; j++) 
                cin >> cityMap[i][j];            
        }
        int i, j;
        if (canPlaceHouse(cityMap, 0, 0, n)) {
            i = 0;
            j = 0;
        } else {
            pair<int, int>block = nextPlace(cityMap, 0, 0, n);
            i = block.first;
            j = block.second;
        }
        cout << dfs(cityMap, i, j, n, 0) << endl;
        free(cityMap);
        cin >> n;
    }
    return 0;
}
```
