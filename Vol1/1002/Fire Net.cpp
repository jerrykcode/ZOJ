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
