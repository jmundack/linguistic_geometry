#include "grammarofzones.h"
#include "trajectory.h"
#include <fstream>
#include <iostream>
#include <iomanip>

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
void GrammarOfZones::_PrintOneD(vector<size_t> &oneD)
   {
   for (size_t i = 0; i < _NumRows; i++)
   {
      for (size_t j = 0; j < _Pieces.size()/_NumRows; j++)
         cout << setw(7) <<oneD.at(i*_NumRows +j) << "("<< i*_NumRows + j << ")" << " ";
      cout << endl;
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
         in >> row;
         if (in.eof()) 
            break;
         in >> col;
         row--;
         col--;
         oneD.push_back(row*_NumRows + col);
      }
   }
   oneD.erase(oneD.begin());
   cout << "currentPath : " << currentPath << endl;
   cout << "oneD : " ;
   for (size_t i = 0; i < oneD.size(); i++)
      cout << oneD.at(i) << " ";
   cout << endl;
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
      _V.clear();
      _V.resize(_Pieces.size(), 0);
      _Time.clear();
      _Time.resize(_Pieces.size(), 2*_Pieces.size());
      _W.clear();
      _W.resize(_Pieces.size(), 0);
      _NextTime.clear();
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
              (u.x) / _NumRows +1,
              (u.x) % _NumRows +1,
              _DummyGenericPieces);

   Trajectory t(_Pieces.at(u.x),
                board,
                (u.y) / _NumRows +1, 
                (u.y) % _NumRows+1);
   // check for trajectory length to u.length if needed

   // make sure the pices are opposing
   if (_IsDifferent(_Pieces.at(_U.x), _Pieces.at(_U.y)))
   {
      q1 = true;
   }
   if (q1)
   {
//       _Trajectories.push_back(*(t.GetTrajectories().begin()));
       _Trajectories.push_back(*(t.GetTrajectories().rbegin()));
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
   if (_U.x == _Pieces.size()-1 && _U.y == _Pieces.size()-1)
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
   {
      _NextTime.clear();
      _NextTime.resize(_Pieces.size(), 2 * _Pieces.size());
   }
   cout << "<--- " << __FUNCTION__ << endl;

}

void GrammarOfZones::_F()
{
   cout << "---> " << __FUNCTION__ << endl;
   for (size_t i = 0; i < _NumRows; i++)
   {
      for (size_t j = 0; j < _Pieces.size()/_NumRows; j++)
         cout << setw(7) <<_Time.at(i*_NumRows +j) << "("<< i*_NumRows + j << ")" << " ";
      cout << endl;
   }

   for (size_t i = 0; i < _NumRows; i++)
   {
      for (size_t j = 0; j < _Pieces.size()/_NumRows; j++)
         cout << setw(7) <<_V.at(i*_NumRows +j) << "("<< i*_NumRows + j << ")" << " ";
      cout << endl;
   }

   if (((_U.x != _Pieces.size() -1) && (_U.length > 0)) ||
       ((_U.y == _Pieces.size() -1) && (_U.length <= 0)))
   {
      _U.x++;
   }
   else if ((_U.x == _Pieces.size() -1) ||
            ((_U.length == 0) && (_U.y != _Pieces.size() -1)))
   {
      _U.x = 0;
      _U.y ++;
      _U.length = _Time.at(_U.y) * _V.at(_U.y);
   }
   else
   {
      cerr << "_F() failed!!" << endl;
   }
   cout << "_F -- x : " << _U.x << " y : " << _U.y << " length : " << _U.length << endl;
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_Q4()
{
   cout << "---> " << __FUNCTION__ << endl;
   bool q4(false);
   if (_Pieces.at(_U.x) != ' ' && _Pieces.at(_U.x) != 'X') // there is a piece on x
   {
      if ((_U.length > 0) && (_U.x != _UInitial.x) && (_U.x != _UInitial.y))
      {
         cout << "_U.x : " << _U.x << " _U.y: " << _U.y << endl;
         twoD board(_NumRows,
                    _Pieces.size() / _NumRows,
                    _U.x / _NumRows +1,
                    _U.x % _NumRows +1,
                    _DummyGenericPieces);

         cout << "going to create trajectory" << endl;
         Trajectory t(_Pieces.at(_U.x),
                      board,
                      _U.y / _NumRows +1, 
                      _U.y % _NumRows +1);
         cout << "created trajectory _U.x : " << _U.x << " _U.y: " << _U.y  << " length : " << _U.length
            << " of shorted length: " << t.GetShortestPathDistance() << endl;

         if (!_IsDifferent(_Pieces.at(_U.x), _Pieces.at(_UInitial.x)))
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
              _U.x / _NumRows +1,
              _U.x % _NumRows+1,
              _DummyGenericPieces);

   Trajectory t(_Pieces.at(_U.x),
                board,
                _U.y / _NumRows+1, 
                _U.y % _NumRows+1);


   Paths trajectories(t.GetTrajectories());
   for(Paths::const_iterator itr = trajectories.begin(); itr != trajectories.end(); ++itr)
   {
      const string path(*itr);
      if (_IsPathValid(path))
      {
         _Trajectories.push_back(path);
         cout << "Trajectory : ";
         for (size_t i = 0 ; i < _Trajectories.size(); i++)
            cout << _Trajectories.at(i) << "-->";
         cout << endl;

         vector<size_t> oneD(_ConvertToOneDimension(_Trajectories.back()));
         oneD.pop_back();
         for (size_t i = 0; i < oneD.size(); i++)
         {
            _W.at(oneD.at(i)) = 1;
         }
         _Alpha(_Time.at(_U.y) - _U.length +1);
         break;
      }
      else
         cout << "Ignoring path : " << path << endl;
   }
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_Alpha(const int k)
{
   cout << "---> " << __FUNCTION__ << endl;
   vector<size_t> oneD(_ConvertToOneDimension(_Trajectories.back()));
   oneD.pop_back();
   for (size_t i = 0; i < oneD.size(); i++)
   {
      const size_t index = oneD.at(i);
      if (_NextTime.at(index) == 2 * _Pieces.size())
         _NextTime.at(index) = k;
      else if (_NextTime.at(index) < static_cast<size_t>(k))
         _NextTime.at(index) = k;
   }
   cout << "_Alpha updated Nexttime to : " << endl;
   _PrintOneD(_NextTime);
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_Q5()
{
   cout << "---> " << __FUNCTION__ << endl;
   bool q5(false);
   for (size_t i = 0; i < _W.size();i++)
   {
      q5 |= (_W.at(i) != 0);
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
   _W.clear();
   _W.resize(_Pieces.size(), 0);
   _Time = _NextTime;
   cout << "Going to higher negations..." << endl;
   cout << "V : " << endl;
   _PrintOneD(_V);
   cout << "Time : " << endl;
   _PrintOneD(_Time);
   cout << "W : " << endl;
   _PrintOneD(_W);
   cout << "<--- " << __FUNCTION__ << endl;
}

void GrammarOfZones::_A6()
{
   cout << "Done!!" << endl;
}

bool GrammarOfZones::_IsPathValid(const string &path)
{
   vector<size_t> pathOneD(_ConvertToOneDimension(path));
   size_t numIntersections(0);
   for (size_t i = 0; i < _Trajectories.size(); i++)
   {
      vector<size_t> trajectoryOneD(_ConvertToOneDimension(_Trajectories.at(i)));
      for (size_t j = 0; j < pathOneD.size(); j++)
      {
         for (size_t k = 0; k < trajectoryOneD.size(); k++)
         {
            if (pathOneD.at(j) == trajectoryOneD.at(k))
               numIntersections++;
         }
      }
   }
   return (numIntersections == 1);
}
