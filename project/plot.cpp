#include "plot.h"
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

Plot::Plot(const Paths &paths,
           const Cordinate maxValues,
           const Cordinate start,
           const Cordinate end,
           const std::vector<Cordinate> &obstacles,
           const string &outputFilename)
{
   cout << "Plotting start : " << start.first << "," << start.second << " to end : " << end.first << "," << end.second << endl;
    system("mkdir -p temp");
   int offset(0);
   for (Paths::const_iterator itr = paths.begin(); itr != paths.end(); ++itr)
   {
       stringstream ss;
       ss << "./temp/" << offset;
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
   ofstream profile("profile.gnu");
   profile << "set term jpeg" << endl;
   profile << "set out \'" << outputFilename << "'" << endl;
   profile << "set xrange[0.5:" << float(maxValues.first) +0.5 << "]" << endl;
   profile << "set yrange[0.5:" << float(maxValues.second) +0.5 << "]" << endl;
   profile << "set grid" << endl;
//   profile << "set object circle at " << start.first << "," << start.second << " size 0.2 fillcolor rgbcolor \'red\' fill solid" << endl;
//   profile << "set object circle at " << end.first << "," << end.second << " size 0.2 fillcolor rgbcolor \'red\' fill solid" << endl;
   for (size_t i = 0; i < obstacles.size(); i++)
   {
       const pair<int,int> &ob(obstacles.at(i));
       profile << "set object rect from "  << ob.first-0.5 << "," << ob.second - 0.5 << " to "
                  << ob.first+0.5 << "," << ob.second + 0.5 << " fc rgbcolor \'gray\'" << endl;
   }

   profile << "set xtics (";
   for (int i = 0; i < maxValues.first; i++)
   {
       if (i!=0)
           profile << ',';
       profile << "'" << i+1 << "'" << (float(i)+0.5);
   }
   profile << ")" << endl;
   profile << "set ytics (";
   for (int i = 0; i < maxValues.second; i++)
   {
       if (i!=0)
           profile << ',';
       profile << "'" << i+1 << "'" << (float(i)+0.5);
   }
   profile << ")" << endl;
   profile << "plot ";
   for (size_t i = 0; i < paths.size(); i++)
   {
       if (i !=0)
           profile << ",";
       profile << "'" << "temp/" << i << "\' w linespoints lc \'red\' title \'\' ";
   }
   profile.close();
   system("gnuplot < profile.gnu");
   string openJpeg = "gnome-open " + outputFilename;
   system(openJpeg.c_str());
}
