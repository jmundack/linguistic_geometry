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
      out << ",pgfstyle={straightmove},";
      out << "markmoves={";
      string p1,p2;
      for (size_t i = 0; i < paths.size(); i++)
      {
         if (p1.empty())
         {
            // read p1
         }
         else
         {
            out << ',';
         }
         // read p2
         out << p1 << "-" << p2;
         p1 = p2;
      }
      //  f2-g3,g3-e5
      out << "}, arrow=to, linewidth=0.1em" << endl;
   }
   out << "]" << endl;

   out << "\\end{document}" << endl;
}

