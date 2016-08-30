#include <iostream>
#include <thread>
#include <random>
#include <time.h>
#include <chrono>
using namespace std;
using ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;

int main()
{
	random_device rd;
	//int i;
	double pos[10000][3];
	double dist[10000][10];
	double del_t = .0001;
	mt19937 mt(rd());
	std::uniform_real_distribution<double> dist_x (0,1.0);
	std::uniform_real_distribution<double> dist_y (-1.0,1.0);
	#pragma omp parallel for schedule(static,1)
		for(int i=0;i<10000;++i){
			pos[i][0] = dist_x(mt);
			pos[i][1] = dist_x(mt);
			pos[i][2] = dist_x(mt);
		}
    auto wall_t1 = get_time::now();
	#pragma omp parallel for schedule(runtime)
		for(int i=0; i<10000; ++i)
		{
			pos[i][0] += sqrt(2*del_t) * dist_y(mt);
		}


	auto wall_t2 = get_time::now();
	
    auto diff = wall_t2 - wall_t1;
    std::cout<<"\nComputation time: "<<chrono::duration_cast<std::chrono::nanoseconds>(diff).count()<<" ns.\n";

	
	return 0;
}
