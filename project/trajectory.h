#pragma once
#include "twoD.h"
#include <list>

   typedef std::pair<int,int> Cordinates;
   typedef std::list<std::string> Paths;
class Trajectory
{
public:
    Trajectory(const std::string &piece,
               const twoD &initialBoard,
               const int endingX,
               const int endingY);

    Trajectory(const char piece,
               const twoD &initialBoard,
               const int endingX,
               const int endingY);

    Paths GetTrajectories();
    Paths GetTrajectories(const int length);
    size_t GetShortestPathDistance() const { return _ShortestDistance; }
    size_t GetLongestPossiblePathDistance() const { return _LongestDistance; }

private:
    void _Init();
    // private funcions
    void _ComputeSum();
    std::vector<Cordinates> _Next(const Cordinates currentPos, const int length);
    void A(const Cordinates start, 
           const Cordinates end, 
           const int length, 
           Paths &paths,
           const std::string currentPath);

    std::vector<Cordinates> _Med(const Cordinates start,
                                 const Cordinates end,
                                 const int length);

    int _Lmed(const Cordinates start,
              const Cordinates end);

    // Data
    const std::string _Piece;
    twoD _InitialBoard;
    const int _EndingX;
    const int _EndingY;
    twoD::Array2D _BegData;
    twoD::Array2D _Sum;
    int _ShortestDistance;
    int _LongestDistance;

};
