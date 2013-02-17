#pragma once
#include "twoD.h"

   typedef std::pair<int,int> Cordinates;
   typedef std::vector<Cordinates> Path;
class Trajectory
{
public:
    Trajectory(const std::string &piece,
               const twoD &initialBoard,
               const int endingX,
               const int endingY);

    std::vector<Path> GetShortestTrajectories();

private:
    const std::string _Piece;
    twoD _InitialBoard;
    const int _EndingX;
    const int _EndingY;
    twoD::Array2D _Sum;
};
