#include "trajectory.h"
#include <iostream>

using namespace std;

Trajectory::Trajectory(const string &piece,
                       const twoD &initialBoard,
                       const int endingX,
                       const int endingY):_Piece(piece),_InitialBoard(initialBoard),_EndingX(endingX),_EndingY(endingY)
{
}

vector<Path> Trajectory::GetShortestTrajectories()
{
   vector<Path> shortestPaths;

   cout << "************ computing for begBoard ****************" << endl;
   twoD begBoard(_InitialBoard);
   begBoard.compute_distances(_Piece); 
   cout << "************ DONE computing for begBoard ****************" << endl;

   cout << "************ computing for  finalBoard ****************" << endl;
   twoD finalBoard(_InitialBoard);
   finalBoard.compute_distances(_Piece, 5,8);
   cout << "************ DONE computing for  finalBoard ****************" << endl;

   const twoD::Array2D &begData = begBoard.GetData();
   const twoD::Array2D &finalData = finalBoard.GetData();

   _Sum.clear();
   _Sum.resize(begData.size());
   for (size_t i = 0; i < begData.size(); i++)
      _Sum.at(i).resize(begData.at(0).size());

   for (size_t i = 0; i < begData.size(); i++)
      for (size_t j = 0; j < begData.at(i).size(); j++)
         _Sum.at(i).at(j) = begData.at(i).at(j) + finalData.at(i).at(j);

   cout << "Beg board: " << endl;
   for (size_t i = 0; i < begData.size(); i++)
   {
      for (size_t j = 0; j < begData.at(i).size(); j++)
         cout << begData.at(i).at(j) << "\t";
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
   for (size_t i = 0; i < begData.size(); i++)
   {
      for (size_t j = 0; j < begData.at(i).size(); j++)
         cout << _Sum.at(i).at(j) << "\t";
      cout << endl;
   }

   return shortestPaths;
}
