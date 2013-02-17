#include "twoD.h"
#include <iostream>
#include <iomanip>
#include <math.h> 
#include <stdlib.h> 

using namespace std;

twoD::twoD(const int h, const int w, const int xs, const int ys,
           map<std::string, std::vector<std::pair<int,int> > > gps):genericPieces(gps)
{
	step=0;
    height=h;
    width=w;

	// Transform coordinates into c++ format
    starty= ys-1;
    startx=xs-1;
	index_checked=0;
	num_obs=0;

	// Create 2-dimensional array
	array2D.resize(height);
	for (int i=0; i<height; ++i)
		array2D[i].resize(width);

	for (int j=0; j<height; ++j)
		for (int k=0; k<width; ++k)
            array2D[j][k]=0;
}

void twoD::set_obstacles(const int numobs, vector<pair<int,int> > v)
{
	for (size_t i=0; i<v.size(); ++i)
	{
      array2D[v[i].first-1][abs(v[i].second-1)]=999;
	}

	num_obs=numobs;
}
void twoD::compute_distances(const string& piece)
{
   cout << "JJM  - x,y : " << startx << "," << starty << endl;
	// Start filling out distance 1 from first position
	step_one (piece);
	index_checked++;
	
	size_t dimension;
	dimension = height*width;

	// Check reachability of squares that have distance until all squares in board are filled out. 
	while ((index_checked< dimension) && ((checked.size()+ num_obs)<=dimension) && ( index_checked< checked.size()))
	{
		check_reach(piece, checked[index_checked].first, checked[index_checked].second);
		index_checked++;
	}
	/*	for (int i=0; i<checked.size(); ++i)
			cout << "first " << checked[i].first << " second "<< checked[i].second << '\n';
	*/
}

void twoD::compute_distances(const string& piece, const int x, const int y)
{
   cout << "JJM - x,y passed in : " << x << "," << y << endl;
   startx = x-1;
   starty = y-1;
   compute_distances(piece);

   cout << "from compute_distances : 4,4: " << array2D[4][4] << endl;

}
	
void twoD::step_one (const string& piece)
{
	checked.push_back(make_pair(startx, starty));
	
    if (piece=="Rook")
		rook(startx, starty);

    else if (piece=="King")
		king(startx,starty);
	
    else if (piece=="Bishop")
		bishop(startx, starty);

    else if (piece=="Queen")
		queen(startx,starty);
	
    else if (piece =="Knight")
		knight(startx,starty);

    else if (piece == "Pawn")
		pawn(startx,starty);

	else 
        generic_piece(piece,startx, starty);
}

void twoD::check_reach (const string& piece, const int x, const int y)
{
        if (piece=="Rook")
			rook(x,y);

        else if (piece=="King")
			king(x,y);

        else if (piece == "Bishop")
			bishop(x,y);

        else if (piece == "Queen")
            queen(x,y);

        else if (piece =="Knight")
			knight(x,y);

        else if (piece == "Pawn")
			pawn(x,y);
		
		else 
            generic_piece(piece,x, y);
}

void twoD::generic_piece (const string &name,const int x, const int y)
{
    const vector<pair<int, int> > &v(genericPieces.at(name));

    int xi(v.at(0).first), yi(v.at(0).second); // initial positions of sample board where user input reachability

	step=array2D[x][y]+1;

	int changex, changey;
    for (size_t i=1; i < v.size(); ++i)
	{
		changex= (v[i].first - xi)+x;
		changey= (v[i].second - yi)+y;

		cout << "x,y " << changex << "," <<changey << '\n';
		if ((changex)<width && (changey)<height && changex>=0 && changey>=0)
		{	
			if(array2D[changex][changey] == 0 && (changex!= startx|| changey != starty)) // here there's a problem with ones that continue like queen
			{
				array2D[changex][changey]= step;
				add_checked(changex, changey);
			}
		}
	}
}

void twoD::add_checked(const int x, const int y)
{
	checked.push_back(make_pair(x,y));
}

void twoD::pawn(const int x, const int y)
{
	step=array2D[x][y]+1;
	
	if (y-1 >=0 && array2D[x][y-1]==0) 
	{
		array2D[x][y-1]= step;
		add_checked(x,y-1);

	}
}

void twoD::knight(const int x, const int y )
{
		step=array2D[x][y]+1;

		int addx=x+1;
		int lessx=x-1;
		int addy=y+1;
		int lessy=y-1;
		int addtwox=x+2;
		int lesstwox=x-2;
		int addtwoy=y+2;
		int lesstwoy=y-2;

	if ( addtwox < width)
	{
		if (lessy>=0 && array2D[addtwox][lessy] == 0 && (addtwox!=startx || lessy != starty))
		{
			array2D[addtwox][lessy]= step;
			add_checked(addtwox,lessy);
		}

		if (addy<height && array2D[addtwox][addy] == 0 && (addtwox!=startx || addy != starty))
		{
			array2D[addtwox][addy]= step;
			add_checked(addtwox,addy);
		}

	}

	if ( lesstwox >= 0)
	{
		if (lessy>=0 && array2D[lesstwox][lessy] == 0 && (lesstwox!=startx || lessy != starty))
		{
			array2D[lesstwox][lessy]= step;
			add_checked(lesstwox,lessy);
		}

		if (addy<height && array2D[lesstwox][addy] == 0 && (lesstwox!=startx || addy != starty))
		{
			array2D[lesstwox][addy]= step;
			add_checked(lesstwox,addy);
		}

	}

	if ( addtwoy < height)
	{
		if (addx<width && array2D[addx][addtwoy] == 0 && (addx!=startx || addtwoy != starty))
		{
			array2D[addx][addtwoy]= step;
			add_checked(addx,addtwoy);
		}

		if (lessx>=0 && array2D[lessx][addtwoy] == 0 && (lessx!=startx || addtwoy != starty))
		{
			array2D[lessx][addtwoy]= step;
			add_checked(lessx,addtwoy);
		}

	}

	if ( lesstwoy >= 0)
	{
		if (addx<width && array2D[addx][lesstwoy] == 0 && (addx!=startx || lesstwoy != starty))
		{
			array2D[addx][lesstwoy]= step;
			add_checked(addx,lesstwoy);
		}

		if (lessx>=0 && array2D[lessx][lesstwoy] == 0 && (lessx!=startx || lesstwoy != starty))
		{
			array2D[lessx][lesstwoy]= step;
			add_checked(lessx,lesstwoy);
		}

	}

}
void twoD::bishop( const int x, const int y)
{
	step=array2D[x][y]+1;
	
	int addx=x+1;
	int addy=y+1;
	int lessx=x-1;
	int lessy=y-1;
	bool stop=false;

	while ( lessy>=0 && lessx>=0 && !stop) // while square on left-up diagonal is within bound and conditions below are met
	{	
			// if square in left-up diagonal is not blocked, it's not the start position, and it was not already evaluated,
			// update distance and add to a vector of pairs that stores evaluated squares; otherwise exit loop
			if ((array2D[lessx][lessy]!=999) &&((array2D[lessx][lessy]==0)||(array2D[lessx][lessy]>=step))&& (lessx!=startx || lessy!=starty))
			{
				if (array2D[lessx][lessy]==0)
				{
					array2D[lessx][lessy]=step;
					add_checked(lessx,lessy);
				}
				lessx--;
				lessy--;
			}
			else
				stop=true;				
	}

	lessx=x-1;
	lessy=y-1;
	stop=false;

	while ( addy<height && addx<width && !stop) // while square on right-down diagonal is within bound and conditions below are met
	{	
			// if square in rigth-down diagonal is not blocked, it's not the start position, and it was not already evaluated,
			// update distance and add to a vector of pairs that stores evaluated squares; otherwise exit loop
			if ((array2D[addx][addy]!=999) &&((array2D[addx][addy]==0)||(array2D[addx][addy]>=step))&& (addx!=startx || addy!=starty))
			{
				if (array2D[addx][addy]==0)
				{
					array2D[addx][addy]=step;
					add_checked(addx,addy);
				}
				addx++;
				addy++;
			}
			else
				stop=true;				
	}

	addx=x+1;
	addy=y+1;
	stop=false;
	
	while ( addy<height && lessx >=0 && !stop) // while square on left-down diagonal is within bound and conditions below are met
	{	
			// if square in left-down diagonal is not blocked, it's not the start position, and it was not already evaluated,
			// update distance and add to a vector of pairs that stores evaluated squares; otherwise exit loop
			if ((array2D[lessx][addy]!=999) &&((array2D[lessx][addy]==0)||(array2D[lessx][addy]>=step))&& (lessx!=startx || addy!=starty))
			{
				if (array2D[lessx][addy]==0)
				{
					array2D[lessx][addy]=step;
					add_checked(lessx,addy);
				}
				lessx--;
				addy++;
			}
			else
				stop=true;				
	}

	addy=y+1;
	lessx=x-1;
	stop=false;

	while ( lessy>=0 && addx<width && !stop) // while square on right-up diagonal is within bound and conditions below are met
	{	
			// if square in rigth-up diagonal is not blocked, it's not the start position, and it was not already evaluated,
			// update distance and add to a vector of pairs that stores evaluated squares; otherwise exit loop
			if ((array2D[addx][lessy]!=999) &&((array2D[addx][lessy]==0)||(array2D[addx][lessy]>=step))&& (addx!=startx || lessy!=starty))
			{
				if (array2D[addx][lessy]==0)
				{
					array2D[addx][lessy]=step;
					add_checked(addx,lessy);
				}
				addx++;
				lessy--;
			}
			else
				stop=true;				
	}

}

void twoD::queen (const int x, const int y)
{
	rook(x,y);
	bishop(x,y);

}
void twoD::king(const int x, const int y)
{
	step=array2D[x][y]+1;

	int addx=x+1;
	int lessx=x-1;
	int addy=y+1;
	int lessy=y-1;

	// update distance if square adjacent to x,y is equal to 0 and is not the start position
	if ( addx < width)
	{
		if (array2D[addx][y] == 0 && (addx!=startx || y != starty))
		{
			array2D[addx][y]= step;
			add_checked(addx,y);
		}
		if (lessy>=0 && array2D[addx][lessy]==0 &&  (addx!=startx || lessy!= starty))
		{
			array2D[addx][lessy]=step;
			add_checked(addx,lessy);
		}
		
		if (addy<height && array2D[addx][addy]==0 &&  (addx!=startx || addy!= starty))
		{
			array2D[addx][addy]=step;
			add_checked(addx,addy);	
		}
	}
	if ( lessx >=0)
	{
		if (array2D[lessx][y] == 0 && (lessx!=startx || y != starty))
		{
			array2D[lessx][y]= step;
			add_checked(lessx,y);
		}
		if (lessy>=0 && array2D[lessx][lessy]==0 &&  (lessx!=startx || lessy!= starty))
		{
			array2D[lessx][lessy]=step;
			add_checked(lessx,lessy);
		}
		
		if (addy<height && array2D[lessx][addy]==0 &&  (lessx!=startx || addy!= starty))
		{
			array2D[lessx][addy]=step;
			add_checked(lessx,addy);	
		}
	}

	if (lessy>=0  && array2D[x][lessy]==0 && (x!=startx || lessy!= starty))
	{
			array2D[x][lessy]=step;
			add_checked(x,lessy);	
	}

	if (addy<height  && array2D[x][addy]==0 && (x!=startx || addy!= starty))
	{
			array2D[x][addy]=step;
			add_checked(x,addy);	
	}

}
void twoD::rook(const int x, const int y)
{
	step=array2D[x][y]+1;
	
	pair<int, int> elem;
	int copyx=x;
	int copyy=y;
	bool stop=false;
	while ((copyy+1)<width && !stop) // while square below is within bound and conditions below are met
	{	
			// if square below is not blocked, it's not the start position, and it was not already evaluated,
			// update distance and add to a vector of pairs that stores evaluated squares; otherwise exit loop
			if ((array2D[copyx][copyy+1]!=999) &&((array2D[copyx][copyy+1]==0)||(array2D[copyx][copyy+1]>=step))&& (copyx!=startx || (copyy+1)!=starty))
			{
				if (array2D[copyx][copyy+1]==0)
				{
					array2D[copyx][copyy+1]=step;
					elem.first = copyx;
					elem.second= (copyy+1);
					checked.push_back(elem);
				}
				copyy++;
			}
			else
				stop=true;				
	}

	copyx=x;
	copyy=y;
	stop=false;
	
	while ((copyy-1)>=0 && !stop)
	{	
		if (array2D[copyx][copyy-1]!=999 &&((array2D[copyx][copyy-1]==0)||(array2D[copyx][copyy-1]>=step))&& (copyx!=startx || (copyy-1)!=starty))
		{
				
				if (array2D[copyx][copyy-1]==0)
				{
					array2D[copyx][copyy-1]=step;			
					elem.first = copyx;
					elem.second= (copyy-1);
					checked.push_back(elem);
				}
					copyy--;
		}

		else 
			stop=true;
	}

	copyx=x;
	copyy=y;
	stop=false;
	
	while ((copyx-1)>=0 && !stop)
	{	
		if (array2D[copyx-1][copyy]!=999 &&((array2D[copyx-1][copyy]==0)||(array2D[copyx-1][copyy]>=step))&& ((copyx-1)!=startx || copyy!=starty))
		{
				if (array2D[copyx-1][copyy]==0)
				{
					array2D[copyx-1][copyy]=step;
					elem.first = (copyx-1);
					elem.second= copyy;
					checked.push_back(elem);
				}
				copyx--;
		}

		else 
			stop=true;
	}

	copyx=x;
	copyy=y;
	stop=false;
	
	while ((copyx+1)<width && !stop)
	{	
		if (array2D[copyx+1][copyy]!=999 &&((array2D[copyx+1][copyy]==0)||(array2D[copyx+1][copyy]<=step))&& ((copyx+1)!=startx || copyy!=starty))
		{
				if (array2D[copyx+1][copyy]==0)
				{
					array2D[copyx+1][copyy]=step;
					elem.first = (copyx+1);
					elem.second= copyy;
					checked.push_back(elem);
				}
					copyx++;			
		}
		else 
			stop=true;
	}
	
}

string twoD::GetTable()
{
    ostringstream s;
    _Display(s);
    return s.str();
}

void twoD::display()
{
    _Display(cout);
}

void twoD::_Display(std::ostream &os)
{
    for (int j=0; j<height; ++j)
    {
        os << setw(5) << left << abs(height-j);
        for (int k=0; k<width; ++k)
            os << setw(4) << array2D[j][k];
        os << "\n\n";
    }

            os <<'\n';
    os << setw(5) << '.';
    for (int k=1; k<=height; ++k)
        os << setw(4) << k;

    os << "\n\n";

}


void twoD::display(int)
{
		array2D[startx][starty]= 88;
		for (int j=0; j<height; ++j)
		{	 
			cout << setw(5) << left << abs(height-j);
			for (int k=0; k<width; ++k)
                cout << setw(4) << array2D[j][k];
			cout<< "\n\n";
		}

				cout<<'\n';
		cout<< setw(5) << '.';
		for (int k=1; k<=height; ++k)
			cout << setw(4) << k;

		cout<< "\n\n";
		array2D[startx][starty]= 0;
}
