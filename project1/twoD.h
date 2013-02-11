#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>

class twoD
{

friend class Table;

public:
    typedef std::vector<std::vector<double> > Array2D;
    twoD (const int, const int, const int, const int,
          std::map<std::string, std::vector<std::pair<int,int> > > genericPieces);
	
	void compute_distances(const std::string& piece);
	
	void step_one (const std::string&);
	void check_reach (const std::string& piece, const int x, const int y);
	void add_checked (const int x, const int y);
	void set_obstacles(const int numobs, std::vector<std::pair<int,int> > v);
    void generic_piece (const std::string &name, const int x, const int y);

	void rook(const int, const int);
	void king (const int, const int);
	void bishop (const int, const int);
	void queen (const int, const int);
	void knight (const int, const int);
	void pawn (const int, const int);
	void display();
	void display(int);
    std::string GetTable();
    const Array2D &GetData() const { return array2D; }

private:
    void _Display(std::ostream &os);
    Array2D array2D;
	std::vector<std::pair<int, int> > checked;
	std::vector<std::pair<int, int> > obstacles;
   std::pair<int, int>  element;
	int height, width;
	int startx, starty;
	int step;
	size_t index_checked;
	int num_obs;
    std::map<std::string, std::vector<std::pair<int,int> > > genericPieces;
};

