#include "HW.h"
#include <iostream>
#include <limits.h>
#include <vector>
using namespace std;

RomeoAndJuliet::RomeoAndJuliet(int m, int n, int **Maze, pair<int, int> startp, pair<int, int> endp)
{
	this->Maze = Maze;
	this->Romeo = startp;
	this->Juliet = endp;
	this->m = m;
	this->n = n;
	current_turn = -1;
	prior_direction = -1;
	current_direction = -1;
	best_turn = INT_MAX;

	pair<int, int> c[8] = {{1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}};
	corrdinate = &c[8];
}

bool RomeoAndJuliet::IsCoverAllRoom()
{
	bool isCoverAll = true;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (Maze[i][j] == 0)
			{
				isCoverAll = false;
				break;
			}
		}
		if (!isCoverAll)
			break;
	}
	return isCoverAll;
}

void RomeoAndJuliet::NextStep(int i, int j)
{
	if (i + 1 > m || i - 1 == 0 || j + 1 > n || j - 1 == 0 ||
		Maze[i][j] == -1 || current_turn > best_turn || 
		(!IsCoverAllRoom() && i == Juliet.first && j == Juliet.second))
	{
		return;
	}

	if (IsCoverAllRoom() && i == Juliet.first && j == Juliet.second &&
		current_turn < best_turn)
	{
		best_turn = current_turn;
		return;
	}
	else
	{
		for (int k = 0; k < 8; k++)
		{
			current_direction = k;
			if (prior_direction != k)
			{
				current_turn++;
				NextStep(i + corrdinate[k].first, j + corrdinate[k].second);
				current_turn--;
			}
			else
			{
				NextStep(i + corrdinate[k].first, j + corrdinate[k].second);
			}
			current_direction = k;
		}
	}
}

float Loading::GetMax()
{
	return bestw;
}
Loading::Loading(int n, float *w, float c, float cw, float best)
{
	this->n = n;
	this->w = w;
	this->c = c;
	this->cw = cw;
	this->bestw = best;
}

void Loading::BackTrack(int i)
{
	if (i >= n)
	{
		if (cw > bestw)
			bestw = cw;
		return;
	}

	if (cw + w[i] <= c)
	{
		cw += w[i];
		BackTrack(i + 1);
		cw -= w[i];
	}
	BackTrack(i + 1);
}

int main()
{
	float weight[4] = {10, 50, 32, 41};
	Loading test(4, weight, 200);
	test.BackTrack(0);
	cout << test.GetMax() << endl;
}
