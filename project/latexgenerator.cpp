#include "latexgenerator.h"
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>

/*
\documentclass[12pt]{article}
\usepackage{chessboard}
\usepackage{chessfss}
\begin{document}
			\chessboard[setpieces={Bf2,pe5},
			            pgfstyle={straightmove},
						markmoves={f2-g3,g3-e5},
						arrow=to, linewidth=0.1em] \\
\end{document}
*/


using namespace std;
namespace 
{
   const string _Alphabets ="abcdefghijklmnopqrstuvwxyz";
}

LatexGenerator::LatexGenerator(const Paths &paths,
                               const Cordinate maxValues,
                               const vector<char> &pieces,
                               const vector<Cordinate> &obstacles,
                               const string &outputFilename)
{
   ofstream out(outputFilename.c_str());
   out << "\\documentclass[12pt]{article}" << endl;
   out << "\\usepackage{chessboard}" << endl;
   out << "\\usepackage{chessfss}" << endl;
   out << "\\begin{document}" << endl;

	out << "\\chessboard[setpieces={";
   int numPieces(0);
   for (size_t i = 0; i < pieces.size(); i++)
   {
      if (pieces.at(i) == ' ' || pieces.at(i) == 'X') 
         continue;
      if (numPieces != 0) out << ',';
      out << pieces.at(i);
      out << _Alphabets.at(i%maxValues.first);
      out << (i/maxValues.first + 1);
      numPieces++;
   }
   out << "}";
   if (!paths.empty())
   {
      system("mkdir -p lg_temp");
      int offset(0);
      cout << "Generating temp files" << endl;
      for (Paths::const_iterator itr = paths.begin(); itr != paths.end(); ++itr)
      {
         stringstream ss;
         ss << "./lg_temp/" << offset;
         ofstream out(ss.str().c_str());

         const string &currentPath(*itr);
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
         offset++;
      }
      cout << "DONE Generating temp files" << endl;

      out << ",pgfstyle={straightmove},";
      out << "markmoves={";
      cout << "Generating moves" << endl;
      for (size_t i = 0; i < paths.size(); i++)
      {
         cout << "Generating move " << i << endl;
         stringstream ss;
         ss << "./lg_temp/" << i;
         ifstream in(ss.str().c_str());
         int a1(0), a2(0);
         int b1(0), b2(0);
         bool isFirst(true);
         while (!in.eof())
         {
            if (isFirst)
            {
               in >> a1;
               in >> a2;
            }
            in >> b1;
            in >> b2;
            if (in.eof()) break;
            if(!isFirst)
            {
               out << ',';
            }
            out << _Alphabets.at(a1-1) << a2 << "-" << _Alphabets.at(b1-1) << b2;
            a1 = b1;
            a2 = b2;
            isFirst = false;
         }
         cout << "DONE Generating move " << i << endl;
      }
      cout << "DONE Generating moves" << endl;
      //  f2-g3,g3-e5
      out << "}, arrow=to, linewidth=0.1em" << endl;
   }
   out << "]" << endl;

   out << "\\end{document}" << endl;
}

