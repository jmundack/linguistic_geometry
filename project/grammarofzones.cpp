#include "grammarofzones.h"
#include "trajectory.h"
#include <fstream>
#include <iostream>

using namespace std;

map<std::string, std::vector<std::pair<int,int> > > _DummyGenericPieces;

namespace 
{
   bool _IsDifferent(const char a, const char b)
   {
     return ((isupper(a) && !isupper(b)) ||
       (isupper(b) && !isupper(a)));
   }
}

vector<size_t> GrammarOfZones::_ConvertToOneDimension(const string &currentPath)
{
   vector<size_t> oneD;
   string _Filename("convert.dat");
   {
      ofstream out(_Filename.c_str());
      for (size_t j = 0; j < currentPath.size(); j++)
      {
         if (isdigit(currentPath.at(j)))
            out << currentPath.at(j);
         else if (currentPath.at(j) == ',')
            out << ' ';
         else if (currentPath.at(j) == ')')
            out << endl;
      }
      out.close();
   }
   {
      ifstream in(_Filename.c_str());
      while (in.good() && !in.eof())
      {
         int row(0), col(0);
         in >> col;
         if (in.eof()) 
            break;
         in >> row;
         oneD.push_back(row*_NumRows + col);
      }
   }
   oneD.erase(oneD.begin());
   return oneD;
}

GrammarOfZones::GrammarOfZones(const U &u, std::vector<char> &pieces,const int numRows):_UInitial(u),_U(u),_Pieces(pieces),_NumRows(numRows)
{
}

vector<string> GrammarOfZones::GetZone()
{
   _Trajectories.clear();
   if (_Q1(_U))
   {
      cout << "Q1 if true" << endl;
      _U = U(0,0,0);
      _V.resize(_Pieces.size(), 0);
      _Time.resize(_Pieces.size(), 2*_Pieces.size());
      _W.resize(_Pieces.size(), 0);
      _NextTime.resize(_Pieces.size(), 2*_Pieces.size());
      _A2();
   }
   return _Trajectories;
}

bool GrammarOfZones::_Q1(const U &u)
{
   cout << "---> " << __FUNCTION__ << endl;
   bool q1(false);
   if (_Pieces.at(u.x) == ' ')
      return q1;
   if (_Pieces.at(u.x) == 'X')
      return q1;
   twoD board(_NumRows,
              _Pieces.size() / _NumRows,
              u.x / _NumRows,
              u.x % _NumRows,
              _DummyGenericPieces);

   Trajectory t(_Pieces.at(u.x),
                board,
                u.y / _NumRows, 
                u.y % _NumRows);
   // check for trajectory length to u.length if needed

   // make sure the pices are opposing
   if (_IsDifferent(_Pieces.at(_U.x), _Pieces.at(_U.y)))
   {
      q1 = true;
   }
   if (q1)
   {
       _Trajectories.push_back(*(t.GetTrajectories().begin()));
   }
   cout << "<--- " << __FUNCTION__ << endl;
   return q1;
}

  
void GrammarOfZones::_A2()
{
   cout << "---> " << __FUNCTION__ << endl;
   vector<size_t> oneD(_ConvertToOneDimension(_Trajectories.back()));
   for (size_t i = 0; i < oneD.size(); i++)
   {
      _V.at(oneD.at(i)) = 1;
      _Time.at(oneD.at(i)) = i+2;
   }
   _Q3();
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_Q3()
{
   cout << "---> " << __FUNCTION__ << endl;
   if (_U.x == _Pieces.size() && _U.y == _Pieces.size())
   {
      _Q5();
   }
   else
   {
      _A3();
      _Q4();
   }
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_A3()
{
   cout << "---> " << __FUNCTION__ << endl;
   _F();
   if (_U.x == 0 && _U.y == 0 && _U.length ==0)
      _NextTime.resize(_Pieces.size(), 2 * _Pieces.size());
   cout << "<--- " << __FUNCTION__ << endl;

}

void GrammarOfZones::_F()
{
   cout << "---> " << __FUNCTION__ << endl;
   if (((_U.x != _Pieces.size()) && (_U.length > 0)) ||
       ((_U.y == _Pieces.size()) && (_U.length <= 0)))
   {
      _U.x++;
   }
   else if ((_U.x == _Pieces.size()) ||
            ((_U.length == 0) && (_U.y != _Pieces.size())))
   {
      _U.x = 1;
      _U.y ++;
      _U.length = _Time.at(_U.y) * _V.at(_U.y);
   }
   else
      cerr << "_F() failed!!" << endl;
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_Q4()
{
   cout << "---> " << __FUNCTION__ << endl;
   bool q4(false);
   if (_Pieces.at(_U.x) != ' ' && _Pieces.at(_U.x) != 'X') // there is a piece on x
   {
      if ((_U.length > 0) && (_U.x != _UInitial.x) && (_U.y != _UInitial.y))
      {
         twoD board(_NumRows,
                    _Pieces.size() / _NumRows,
                    _U.x / _NumRows,
                    _U.x % _NumRows,
                    _DummyGenericPieces);

         string piece(1,_Pieces.at(_U.x));
         Trajectory t(piece,
                      board,
                      _U.y / _NumRows, 
                      _U.y % _NumRows);

         if (_IsDifferent(_Pieces.at(_U.x), _Pieces.at(_UInitial.x)))
            q4 = (t.GetShortestPathDistance() == 1);
         else
            q4 = (t.GetShortestPathDistance() <= _U.length);
      }
   }
   if (q4)
      _A4();
   _Q3();
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_A4()
{
   cout << "---> " << __FUNCTION__ << endl;
   twoD board(_NumRows,
              _Pieces.size() / _NumRows,
              _U.x / _NumRows,
              _U.x % _NumRows,
              _DummyGenericPieces);

   Trajectory t(_Pieces.at(_U.x),
                board,
                _U.y / _NumRows, 
                _U.y % _NumRows);

  _Trajectories.push_back(*(t.GetTrajectories().begin()));

  // populate w and next time
   vector<size_t> oneD(_ConvertToOneDimension(_Trajectories.back()));
   oneD.pop_back();
   for (size_t i = 0; i < oneD.size(); i++)
   {
      _W.at(oneD.at(i)) = 1;
   }
   _Alpha(_Time.at(_U.y) - _U.length +1);
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_Alpha(const int k)
{
   cout << "---> " << __FUNCTION__ << endl;
   vector<size_t> oneD(_ConvertToOneDimension(_Trajectories.back()));
   oneD.pop_back();
   for (size_t i = 0; i < oneD.size(); i++)
   {
      if (_NextTime.at(i) == 2 * _Pieces.size())
         _NextTime.at(i) = k;
      else if (_NextTime.at(i) < static_cast<size_t>(k))
         _NextTime.at(i) = k;
   }
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_Q5()
{
   cout << "---> " << __FUNCTION__ << endl;
   bool q5(false);
   for (size_t i = 0; i < _W.size();i++)
   {
      q5 |= (_W.at(i) == 0);
   }
   if (q5)
   {
      _A5();
      _Q3();
   }
   else
      _A6();
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_A5()
{
   cout << "---> " << __FUNCTION__ << endl;
   _U = U(0,0,0);
   _V = _W;
   _W.resize(_Pieces.size(), 0);
   _Time = _NextTime;
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_A6()
{
   cout << "Done!!" << endl;
}
