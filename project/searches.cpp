#include "searches.h"
#include <algorithm>

using namespace std;

const int BigNumber = 99;

Searches::Searches():_Sign(1), _D(0), _Index(0), _End(1)
{
}

void Searches::Start()
{
   _A();
}

bool Searches::_Q2()
{
   return true;
}

void Searches::_A()
{
   if (_Q2())
   {
      _Parent[_End] = _Index;
      if (_Child[_Index] != 0)
         _Sibiling[_Child[_Index]] = _End;
      else
      {
         _Sibiling[_Index] = 0;
         _Child[_Index] = _End;
      }
      _End++;
      _D++;
      _Sign = -_Sign;
      _StateValues[_End] = _Sign * BigNumber;

   }
   else
   {
      if (_D != 0)
      {
         _D --;
         _Sign = -_Sign;
      }

      _StateValues[_Parent[_Index]] = _Minimax(_StateValues[_Parent[_Index]], _Leaf(_StateValues[_Index], _M[_Index]));
      _StateValues[_Index] = _Leaf(_StateValues[_Index], _M[_Index]);

   }
}

int Searches::_Minimax(int v1,int v2)
{
   return (_Sign > 0) ? max(v1,v2) : min(v1,v2);
}

int Searches::_Leaf(int a,int b)
{
   return (a == BigNumber) ? b : a;
}
