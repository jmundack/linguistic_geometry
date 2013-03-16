#pragma once
#include "trajectory.h"
#include <vector>

struct U
{
   U(size_t X,size_t Y, size_t L):x(X),y(Y),length(L){}
   size_t x,;
   size_t y;
   size_t length;
};

class GrammarOfZones
{
public:
    GrammarOfZones(const U &u, std::vector<char> &pieces, const int numRows, std::vector<std::pair<int,int> > obstacles);
    std::vector<std::string> GetZone();
private:
    void _A2();
    void _A3();
    void _A4();
    void _A5();
    void _A6();
    std::vector<size_t> _ConvertToOneDimension(const std::string &path);

    bool _Q1(const U &u);
    void _Q3();
    void _Q4();
    void _Q5();
    void _F();
    void _Alpha(const int k);
    void _PrintOneD(std::vector<size_t> &oneD);
    bool _IsPathValid(const std::string &path);
    const U _UInitial;
    U _U;
    std::vector<size_t> _V;
    std::vector<size_t> _W;
    std::vector<size_t> _Time;
    std::vector<size_t> _NextTime;
    std::vector<char> _Pieces;
    const size_t _NumRows;
    std::vector<std::pair<int, int> > _Obstacles;
    std::vector<std::string> _Trajectories;
};
