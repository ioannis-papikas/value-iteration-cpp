/*
 * Map.h
 *
 *  Created on: Jun 20, 2012
 *      Author: john
 */

#include <vector>
#include <cstring>
#include <string>
using std::string;

struct xyLoc {
  int16_t x;
  int16_t y;
};

class Map {
private:
	int width, height;
	std::vector<xyLoc> invalid;
	std::vector<double> U;

public:
	Map(int width, int height);
	std::vector<double> getU();
	int getW();
	int getH();
	bool isObstacle(xyLoc obs);
	void setU(std::vector<double> U);
	void setGoal(xyLoc goal);
	void setNonGoal(xyLoc ngoal);
	void setObstacles(std::vector<xyLoc> invalid);
	double getTotalReward();
	void printMap();
};


class Model {
private:
	double fwd, left, right;

public:
	Model(double fwd, double left, double right);
	double getModelT(Map mp, xyLoc s, xyLoc d);
	double getMax(Map mp, xyLoc s);
};
