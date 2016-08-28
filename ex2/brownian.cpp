#include <iostream>
#include <thread>
#include <random>
#include <time.h>
#include <chrono>
using namespace std;
using ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;

void main()
{
	random_device rd;
	double pos[10000][3];
	mt19937 mt(rd());
	std::uniform_real_distribution<double> dist (0,1.0);
	#pragma omp parallel for schedule(dynamic)
		for(i=0;i<10000;++i){
			pos[i][0] = dist(mt);
			pos[i][1] = dist(mt);
			pos[i][2] = dist(mt);
		}
	

}
