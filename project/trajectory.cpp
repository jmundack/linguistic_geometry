#include "trajectory.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits.h>

using namespace std;

namespace
{
   string _ConvertToPiece(const char c)
   {
      string piece;
      switch(c)
      {
         case 'R':
         case 'r': piece="Rook"; break;
         case 'K':
         case 'k': piece="King"; break;
         case 'B':
         case 'b': piece = "Bishop"; break;
         case 'Q':
         case 'q': piece = "Queen"; break;
         case 'N':
         case 'n': piece ="Knight"; break;
         case 'P':
         case 'p': piece = "Pawn"; break;
         default: cerr << "Generic piece!!" << endl; break;
      }
      return piece;
   }
}

Trajectory::Trajectory(const string &piece,
                       const twoD &initialBoard,
                       const int endingX,
                       const int endingY):_Piece(piece),_InitialBoard(initialBoard),_EndingX(endingX),_EndingY(endingY),_ShortestDistance(INT_MAX),_LongestDistance(-1)
{
   cout << "************ computing for begBoard for " << _Piece << " ****************" << endl;
   twoD begBoard(_InitialBoard);
   begBoard.compute_distances(_Piece); 
   _BegData = begBoard.GetData();

   cout << "************ DONE computing for begBoard ****************" << endl;
   _ComputeSum();

   _Init();
}

Trajectory::Trajectory(const char piece,
                       const twoD &initialBoard,
                       const int endingX,
                       const int endingY):_Piece(_ConvertToPiece(piece)),_InitialBoard(initialBoard),_EndingX(endingX),_EndingY(endingY),_ShortestDistance(INT_MAX),_LongestDistance(-1)
{
   cout << "************ computing for begBoard ****************" << endl;
   twoD begBoard(_InitialBoard);
   begBoard.compute_distances(_Piece); 
   _BegData = begBoard.GetData();

   cout << "************ DONE computing for begBoard ****************" << endl;
   _ComputeSum();

   _Init();
}


void Trajectory::_Init()
{
}

Paths Trajectory::GetTrajectories()
{
   return GetTrajectories(_ShortestDistance);
}

Paths Trajectory::GetTrajectories(const int length)
{
   Paths paths;
   cout << "Calling A" << endl;
   if (length == _ShortestDistance)
      A(_InitialBoard.GetStratingCordinates(),
        make_pair(_EndingX,_EndingY),
        length, 
        paths,
        "");
   else
   {
      const Cordinates startingCordinates(_InitialBoard.GetStratingCordinates());
      const Cordinates endingCordinates(make_pair(_EndingX,_EndingY));
      vector<Cordinates> meds(_Med(startingCordinates,
                                   endingCordinates,
                                   length));
      for (size_t k = 0; k < meds.size(); k++)
      {
         cout << "***** Creating Trajectory one*****" << endl;
         Trajectory one(_Piece,
                        _InitialBoard,
                        meds.at(k).first,
                        meds.at(k).second);
         cout << "***** DONE Creating Trajectory one*****" << endl;
         cout << "***** Creating Trajectory one trajectories *****" << endl;
         Paths firstPaths(one.GetTrajectories());
         cout << "***** DONE Creating Trajectory one trajectories *****" << endl;

         twoD newBoard(_InitialBoard);
         newBoard.SetCordinates(meds.at(k).first, meds.at(k).second);
         cout << "***** Creating Trajectory two*****" << endl;
         Trajectory two(_Piece,
                        newBoard,
                        _EndingX,
                        _EndingY);
         cout << "***** DONE Creating Trajectory two*****" << endl;
         cout << "***** Creating Trajectory two trajectories *****" << endl;
         Paths secondPaths(two.GetTrajectories());
         cout << "***** DONE Creating Trajectory two trajectories *****" << endl;
         for (Paths::const_iterator i = firstPaths.begin(); i != firstPaths.end(); ++i)
            for (Paths::const_iterator j = secondPaths.begin(); j != secondPaths.end(); ++j)
            {
               string secondPath(*j);
               string fixedSecondPath(secondPath.substr(secondPath.find_first_of(')') +1));
               const string fullPath(*i + fixedSecondPath);
               paths.push_back(fullPath);
            }
      }
   }
   cout << "DONE Calling A" << endl;
   cout << "Paths : " << endl;
   int counter(0);
   for (Paths::const_iterator i =paths.begin() ; i != paths.end(); ++i)
      cout << "\t" << counter++ << "--> " << *i << endl;
   cout.flush();
   return paths;
}

void Trajectory::_ComputeSum()
{
   cout << "************ computing for  finalBoard ****************" << endl;
   twoD finalBoard(_InitialBoard);
   finalBoard.compute_distances(_Piece, _EndingX, _EndingY);
   cout << "************ DONE computing for  finalBoard ****************" << endl;

   const twoD::Array2D &finalData = finalBoard.GetData();

   _Sum.clear();
   _Sum.resize(_BegData.size());
   for (size_t i = 0; i < _BegData.size(); i++)
      _Sum.at(i).resize(_BegData.at(0).size());

   for (size_t i = 0; i < _BegData.size(); i++)
      for (size_t j = 0; j < _BegData.at(i).size(); j++)
      {
         _Sum.at(i).at(j) = _BegData.at(i).at(j) + finalData.at(i).at(j);
         _ShortestDistance = min(_ShortestDistance, _Sum.at(i).at(j));
         _LongestDistance = max(_LongestDistance, _Sum.at(i).at(j));
      }

   cout << "Beg board: " << endl;
   for (size_t i = 0; i < _BegData.size(); i++)
   {
      for (size_t j = 0; j < _BegData.at(i).size(); j++)
         cout << _BegData.at(i).at(j) << "\t";
      cout << endl;
   }

   cout << "Final board: " << endl;
   for (size_t i = 0; i < finalData.size(); i++)
   {
      for (size_t j = 0; j < finalData.at(i).size(); j++)
         cout << finalData.at(i).at(j) << "\t";
      cout << endl;
   }


   cout << "Sum : " << endl;
   for (size_t i = 0; i < _BegData.size(); i++)
   {
      for (size_t j = 0; j < _BegData.at(i).size(); j++)
         cout << _Sum.at(i).at(j) << "\t";
      cout << endl;
   }
   cout << "_ShortestDistance : " << _ShortestDistance << endl;
   cout << "_LongestDistance : " << _LongestDistance << endl;
}

vector<Cordinates> Trajectory::_Next(const Cordinates currentPos, const int length)
{
   vector<Cordinates> nextCordinates;
   twoD currentBoard(_InitialBoard);
   currentBoard.compute_distances(_Piece, currentPos.first, currentPos.second);
   twoD::Array2D currentData(currentBoard.GetData());

   for (size_t i = 0; i < _Sum.size() ;i++)
      for (size_t j = 0; j < _Sum.at(i).size() ;j++)
      {
         if (_Sum.at(i).at(j) == _ShortestDistance)
         {
            if (currentData.at(i).at(j) == 1)
            {
               if (_BegData.at(i).at(j) == _ShortestDistance - length +1)
               {
                  nextCordinates.push_back(make_pair(i+1,j+1));
               }
            }
         }
      }
   return nextCordinates;
}

void Trajectory::A(const Cordinates start, 
                   const Cordinates end, 
                   const int length, 
                   Paths &paths,
                   const string currentPath)
{
   stringstream ss;
   ss << currentPath << "(" << start.first << "," << start.second << ")";
   if (length != 0)
   {
      vector<Cordinates> nextCordinates(_Next(start,length));
      for (size_t i = 0; i < nextCordinates.size(); i++)
         A(nextCordinates.at(i), end, length -1, paths, ss.str());
   }
   else
   {
      paths.push_back(ss.str());
   }
}

vector<Cordinates> Trajectory::_Med(const Cordinates start,
                                    const Cordinates end,
                                    const int length)
{

  vector<Cordinates> nextCordinates;
   twoD currentBoard(_InitialBoard);
   currentBoard.compute_distances(_Piece, start.first, start.second);
   twoD::Array2D currentData(currentBoard.GetData());

   for (size_t i = 0; i < _Sum.size() ;i++)
      for (size_t j = 0; j < _Sum.at(i).size() ;j++)
      {
         if (_Sum.at(i).at(j) == _ShortestDistance+1)
         {
            nextCordinates.push_back(make_pair(i+1,j+1));
         }
      }
   return nextCordinates;

}

int Trajectory::_Lmed(const Cordinates start,
                      const Cordinates end)
{
   twoD currentBoard(_InitialBoard);
   currentBoard.compute_distances(_Piece, start.first, start.second);
   twoD::Array2D currentData(currentBoard.GetData());
   return currentData.at(end.first -1).at(end.second -1);
}
