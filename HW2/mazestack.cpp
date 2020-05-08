#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	stack<Coord> coordStack;
	Coord a(sr, sc);
	coordStack.push(a);
	maze[sr][sc] = 'X';
	while (!coordStack.empty())
	{
		Coord c = coordStack.top();
		coordStack.pop();
	
		int row = c.r();
		int col = c.c();
		if (row == er && col == ec)
			return true;

		if ((col + 1) < nCols && maze[row][col + 1] == '.') //east
		{
			Coord b(row, col + 1);
			coordStack.push(b);
			maze[row][col + 1] = 'X';
		}

		
		if ((row + 1) < nRows && maze[row + 1][col] == '.') //south
		{
			Coord b(row + 1, col);
			coordStack.push(b);
			maze[row + 1][col] = 'X';
		}

		if ((col - 1) >= 0 && maze[row][col - 1] == '.') //west
		{
			Coord b(row, col - 1);
			coordStack.push(b);
			maze[row][col - 1] = 'X';
		}

		
		if ((row - 1) >= 0 && maze[row - 1][col] == '.') //north
		{
			Coord b(row - 1, col);
			coordStack.push(b);
			maze[row - 1][col] = 'X';
		}
		
		
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X...X..X.X",
		"X..XX....X",
		"X.X.XXXX.X",
		"XXX......X",
		"X...X.XX.X",
		"X.X.X..X.X",
		"X.XXXX.X.X",
		"X..X...X.X",
		"XXXXXXXXXX"
	};

	string maze2[2] = {
		"..",
		".."
	};

	if(pathExists(maze, 10, 10, 4, 3, 1, 8))
	//if(pathExists(maze2, 2, 2, 0, 0, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}