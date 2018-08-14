# ZOJ Problem Set - 1004 Anagrams by Stack

[原题地址](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemId=4)

* Time Limit: 2 Seconds      
* Memory Limit: 65536 KB

## Description

How can anagrams result from sequences of stack operations? There are two sequences of stack operators which can convert TROT to TORT:

>[  
i i i i o o o o  
i o i i o o i o  
]

where i stands for Push and o stands for Pop. Your program should, given pairs of words produce sequences of stack operations which convert the first word to the second.

## Input
The input will consist of several lines of input. The first line of each pair of input lines is to be considered as a source word (which does not include the end-of-line character). The second line (again, not including the end-of-line character) of each pair is a target word. The end of input is marked by end of file.

## Output
For each input pair, your program should produce a sorted list of valid sequences of i and o which produce the target word from the source word. Each list should be delimited by

>[  
]

and the sequences should be printed in "dictionary order". Within each sequence, each i and o is followed by a single space and each sequence is terminated by a new line.
## Process
A stack is a data storage and retrieval structure permitting two operations:

Push - to insert an item and
Pop - to retrieve the most recently pushed item
We will use the symbol i (in) for push and o (out) for pop operations for an initially empty stack of characters. Given an input word, some sequences of push and pop operations are valid in that every character of the word is both pushed and popped, and furthermore, no attempt is ever made to pop the empty stack. For example, if the word FOO is input, then the sequence:

>i i o i o o	is valid, but  
i i o	is not (it's too short), neither is  
i i o o o i	(there's an illegal pop of an empty stack)  

Valid sequences yield rearrangements of the letters in an input word. For example, the input word FOO and the sequence i i o i o o produce the anagram OOF. So also would the sequence i i i o o o. You are to write a program to input pairs of words and output all the valid sequences of i and o which will produce the second member of each pair from the first.

## Sample Input
madam  
adamm  
bahama  
bahama  
long  
short  
eric  
rice  

## Sample Output
[  
i i i i o o o i o o   
i i i i o o o o i o   
i i o i o i o i o o   
i i o i o i o o i o   
]  
[  
i o i i i o o i i o o o   
i o i i i o o o i o i o   
i o i o i o i i i o o o   
i o i o i o i o i o i o   
]  
[  
]  
[  
i i o i o i o o   
]  

## 题意

给出两个字符串src和target，使用一个栈，用进栈、出栈的方式将第一个字符串转换成第二个字符串。输出所有的进栈出栈方式。进栈'i'，出栈'o'

## 思路

DFS。i、j表示当前src和target要处理的字符下标，n为字符串的长度，若 i == n && j == n，则均处理完毕，得到一种解。否则若当前可以进栈(i < n)，则进栈并递归。若可以出栈(栈不空且栈顶元素等于target[j])，则出栈并递归。注意递归完返回时需要将栈还原为递归之前的状态。

## 代码
[Anagrams by Stack.cpp](https://github.com/jerrykcode/ZOJ/blob/master/Vol1/1004/Anagrams%20by%20Stack.cpp)
```cpp

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <cstring>

using namespace std;

//封装在类中
class StackAnagram {
public:
	//Constructor
    StackAnagram(string src, string target) : src(src), target(target) {}
	//main中调用的函数
    void anagram();
private:
    stack<char> s;
    char *path; //'i'、'o'序列
    vector<char*> paths; //所有的进栈、出栈方式
    string  src, target;

    void dfs(int i, int j);
};

void StackAnagram::anagram() {
    int n = src.size();
    path = new char[n];
    dfs(0, 0);
	//由于dfs中先递归进栈情况后递归出栈，故 'i' 排在 'o' 前，与字典序相符，不需排序
    printf("[\n");
    for (char * p : paths) {
        for (int i = 0; i < 2*n; i++) {
            cout << p[i] << " ";
        }
        cout << endl;
        free(p); //释放内存
    }
    printf("]\n");
    free(path);
}

void StackAnagram::dfs(int i, int j) {
    int n = src.size();
    if (i == n && j == n) { //处理完毕
        char *currentPath = new char[2 * n]; //复制到另一数组中，在anagram函数中输出后释放free
        memcpy(currentPath, path, 2 * n);
        paths.push_back(currentPath);
        return;
    }
    if (i < n) { //可以进栈
        s.push(src[i]);
        path[i + j] = 'i';
        dfs(i + 1, j); //i增长
        s.pop(); //还原
    }
    if (!s.empty() && s.top() == target[j]) { //可以出栈(i >= j因为i等于进栈次数，j等于出栈次数，进栈次数大于等于出栈次数，这里j一定小于n)
        char tmp = s.top();
        s.pop(); //出栈
        path[i + j] = 'o';
        dfs(i, j + 1); //j增长
        s.push(tmp); //还原
    }
}

int main()
{
    string src, target;
    while (cin >> src) {
        cin >> target;
        StackAnagram stackAnagram(src, target);
        stackAnagram.anagram();
    }
    return 0;
}

```
