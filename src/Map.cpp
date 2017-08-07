/*
 * Map.cpp
 *
 *  Created on: Jun 20, 2012
 *      Author: john
 */

#include "Map.h"
#include <iostream>
using namespace std;

Map::Map(int width, int height) {
	this->width = width;
	this->height = height;

	this->U.resize(width*height);
}
void Map::setGoal(xyLoc goal) {
	this->U[goal.y*this->width+goal.x] = 1;
}
void Map::setNonGoal(xyLoc ngoal) {
	this->U[ngoal.y*this->width+ngoal.x] = -1;
}

void Map::setObstacles(std::vector<xyLoc> invalid) {
	this->invalid = invalid;
}
bool Map::isObstacle(xyLoc obs) {
	for (unsigned i=0; i<this->invalid.size(); i++)
		if (this->invalid[i].x == obs.x && this->invalid[i].y == obs.y)
			return true;
	return false;
}
std::vector<double> Map::getU() {
	return this->U;
}
void Map::setU(std::vector<double> U) {
	this->U = U;
}
int Map::getW() {
	return this->width;
}
int Map::getH() {
	return this->height;
}

double Map::getTotalReward() {
	double sum;
	for (int j=0; j<this->height; j++)
		for (int i=0; i<this->width; i++)
			sum += this->U[j*this->width + i];

	return sum;
}

void Map::printMap() {
	cout << "...Printing Map..." << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	for (int j=0; j<this->height; j++) {
		for (int i=0; i<this->width; i++) {
			printf("%0.3f\t", this->U[j*this->width + i]);
		}
		cout << endl;
	}
	cout << "--------------------------------------------------------------------------------" << endl;
}




Model::Model(double fwd, double left, double right) {
	this->fwd = fwd;
	this->left = left;
	this->right = right;
}

double Model::getModelT(Map mp, xyLoc s, xyLoc d) {
	std::vector<double> U = mp.getU();
	int width = mp.getW();
	int height = mp.getH();
	double result = 0;
	// Check if position is inside borders
	xyLoc t;
	t.x = (d.x < 0 ? 0 : d.x);
	t.x = (d.x >= width ? width-1 : d.x);
	t.y = (d.y < 0 ? 0 : d.y);
	t.y = (d.y >= height ? height-1 : d.y);
	if (!mp.isObstacle(t))
		result += this->fwd * U[t.y*width+t.x];
	if (s.x == d.x) {
		xyLoc l,r;
		l.y = s.y;
		l.x = (d.x - 1 < 0 ? 0 : d.x - 1);
		if (!mp.isObstacle(l))
			result += this->left*U[l.y*width+l.x];

		r.y = s.y;
		r.x = (d.x + 1 >= width ? width-1 : d.x + 1);
		if (!mp.isObstacle(r))
			result += this->right*U[r.y*width+r.x];

		return result;

	} else if (s.y == d.y) {
		xyLoc l,r;
		l.x = s.x;
		l.y = (d.y - 1 < 0 ? 0 : d.y - 1);
		if (!mp.isObstacle(l))
			result += this->left*U[l.y*width+l.x];

		r.x = s.x;
		r.y = (d.y + 1 >= height ? height-1 : d.y + 1);
		if (!mp.isObstacle(l))
			result += this->right*U[r.y*width+r.x];

		return result;
	}

	return 0;
}

double Model::getMax(Map mp, xyLoc s) {
	std::vector<double> maxes;
	maxes.resize(4);
	double max = 0;

	xyLoc d;
	d.x = s.x;
	d.y = s.y;
	// Starting from above and going left
	d.y++;
	d.y = (d.y >= mp.getH() ? mp.getH() : d.y);
	maxes[0] = this->getModelT(mp, s, d);

	d.x = s.x;
	d.y = s.y;

	d.x--;
	d.x = (d.x < 0 ? 0 : d.x);
	maxes[1] = this->getModelT(mp, s, d);

	d.x = s.x;
	d.y = s.y;

	d.y--;
	d.y = (d.y < 0 ? 0 : d.y);
	maxes[2] = this->getModelT(mp, s, d);


	d.x = s.x;
	d.y = s.y;

	d.x++;
	d.x = (d.x >= mp.getW() ? mp.getW() : d.x);
	maxes[3] = this->getModelT(mp, s, d);

	// Get max
	max = maxes[0];
	for (int i=1; i<4; i++)
		if (maxes[i] > max)
			max = maxes[i];

	return max;
}
