#include <string>
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
	if (sr == er && sc == ec)
		return true;

	else
	{
		maze[sr][sc] = 'X';

		if ((sc + 1) < nCols && maze[sr][sc + 1] == '.') //east
		{
			if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
				return true;
		}

		if ((sr + 1) < nRows && maze[sr + 1][sc] == '.') //south
		{
			if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
				return true;
		}

		if ((sc - 1) >= 0 && maze[sr][sc - 1] == '.') //west
		{
			if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
				return true;
		}

		if ((sr - 1) >= 0 && maze[sr - 1][sc] == '.') //north
		{
			if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
				return true;
		}
	}

	return false;

}

