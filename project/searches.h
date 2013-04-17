#pragma once
#include <map>
#include <string>

class Searches
{
public:
    Searches();
    void Start();
private:

    bool _Q2();
    void _A();

    std::map<int,int> _Child;
    std::map<int,int> _Parent;
    std::map<int,int> _Sibiling;
    std::map<std::pair<int,int>, std::string> _Moves;
    std::map<int,int> _StateValues;
    std::map<int,int> _M;

    int _Minimax(int v1,int v2);
    int _Leaf(int a,int b);

    int _Sign;
    int _D;
    int _Index;
    int _End;
};

