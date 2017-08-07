//============================================================================
// Name        : valueIteration.cpp
// Author      : Papikas Ioannis
// Version     :
// Copyright   : 
// Description : Value Iteration Implementation
//============================================================================

#include <iostream>
#include <math.h>
#include "Map.h"
#include "Timer.h"
using namespace std;

std::vector<double> vi(Map mp, Model t_model, double R, double g, double e);

int main() {

	Map test(18,16);// = new Map(4,3);

	// Initialize Map Environment

	// Set obstacles
	std::vector<xyLoc> obstacles;
	xyLoc s;
	s.x = 1;
	s.y = 1;
	obstacles.push_back(s);
	//xyLoc s;
	s.x = 2;
	s.y = 0;
	obstacles.push_back(s);
	//xyLoc s;
	s.x = 3;
	s.y = 0;
	obstacles.push_back(s);
	//xyLoc s;
	s.x = 12;
	s.y = 8;
	obstacles.push_back(s);
	//xyLoc s;
	s.x = 6;
	s.y = 10;
	obstacles.push_back(s);
	test.setObstacles(obstacles);

	// Set Goal and nonGoal
	xyLoc goal, nongoal;
	goal.x = 10;
	goal.y = 10;
	test.setGoal(goal);
	nongoal.x = 12;
	nongoal.y = 9;
	test.setNonGoal(nongoal);

	// Initialize Model
	Model t_model(0.8, 0.1, 0.1);

	Timer t;
	test.printMap();
	t.StartTimer();
	std::vector<double> result = vi(test, t_model, -0.04, 0.99, 0.05);
	t.EndTimer();
	test.setU(result);
	test.printMap();
	cout << "Total elapsed time: "<<t.GetElapsedTime()<<endl;
	//cout << "total reward: "<<test.getTotalReward()<< endl;
}

std::vector<double> vi(Map mp, Model t_model, double R, double g, double e) {
	std::vector<double> tempU;
	double d = 0;
	int w = mp.getW();
	int h = mp.getH();
	int counter = 0;
	do {
		tempU = mp.getU();
		counter++;
		d = -1;
		for (int j=0; j<h; j++)
			for (int i=0; i<w; i++) {
				xyLoc s;
				s.x = i;
				s.y = j;
				if (tempU[s.y*w+s.x] == 1 || tempU[s.y*w+s.x] == -1 || mp.isObstacle(s))
					continue;

				//cout<<t_model.getMax(mp, s);
				tempU[j*w+i] = R + g*t_model.getMax(mp, s);
				if (fabs(tempU[j*w+i] - mp.getU()[j*w+i]) > d)
					d = fabs(tempU[j*w+i] - mp.getU()[j*w+i]);
			}
		mp.setU(tempU);

		// Set Upper limit for counter in case of g very close to 1
		if (counter > 10000)
			d = -1;
	} while (d >= e*(1-g)/g);
	cout << "total rounds: "<<counter << ", error: " << d<< endl;

	return tempU;
}
