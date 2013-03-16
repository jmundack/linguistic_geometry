#pragma once

#include <string>
#include <list>
#include <utility>
#include <vector>

typedef std::vector<std::string> Paths;
typedef std::pair<int,int> Cordinate;

class LatexGenerator
{
public:
    LatexGenerator(const Paths &paths,
                   const Cordinate maxValues,
                   const std::vector<char> &pieces,
                   const std::vector<Cordinate> &obstacles,
                   const std::string &outputFilename);
};
