#pragma once
#include <string>
#include <list>
#include <utility>
#include <vector>

typedef std::vector<std::string> Paths;
typedef std::pair<int,int> Cordinate;
class Plot
{
public:
    Plot(const Paths &paths,
         const Cordinate maxValues,
         const Cordinate start,
         const Cordinate end,
         const std::vector<Cordinate> &obstacles,
         const std::string &outputFilename);
private:
};

