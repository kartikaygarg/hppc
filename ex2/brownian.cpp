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
	double dist_tot[10];
	double del_t = .0001;
	short int cnt = 0;
	mt19937 mt(rd());
	std::uniform_real_distribution<double> dist_x (0,1.0);
	std::uniform_real_distribution<double> dist_y (-1.0,1.0);
	#pragma omp parallel for schedule(static,1)
		for(int i=0; i<10;++i)
			dist_tot[i] = 0;
	#pragma omp parallel for schedule(static,1)
		for(int i=0;i<10000;++i){
			pos[i][0] = dist_x(mt);
			pos[i][1] = dist_x(mt);
			pos[i][2] = dist_x(mt);
			for(int j=0; j<10;++j)
			{
				dist[i][j] = 0;
			}
		}
	std::cout<<"Initialization of structures is complete!\n";
    auto wall_t1 = get_time::now();
	//#pragma omp parallel for schedule(runtime) reduction(+:dist_tot)
	#pragma omp parallel for schedule(runtime)
		for(int i=0; i<10000; ++i)
		{
			for(int j=0;j<5000;++j)
			{
				double temp_x = pos[i][0] + (sqrt(2*del_t) * dist_y(mt));
				double temp_y = pos[i][1] + (sqrt(2*del_t) * dist_y(mt));
				double temp_z = pos[i][2] + (sqrt(2*del_t) * dist_y(mt));
				dist[i][j/500] += sqrt( pow((temp_x - pos[i][0]),2) + pow((temp_y - pos[i][1]),2) + pow((temp_z - pos[i][2]),2) );
				//dist[j/500] += sqrt( pow((temp_x - pos[i][0]),2) + pow((temp_y - pos[i][1]),2) + pow((temp_z - pos[i][2]),2) );
				pos[i][0] = temp_x;
				pos[i][1] = temp_y;
				pos[i][2] = temp_z;
			}
		}
	std::cout<<"Simulation complete! Calculating average distances.\n";
	#pragma omp parallel for schedule(static)
		for(int i=0; i<10; ++i)
		{
			for(int j=0; j<10000;++j)
			{
				dist_tot[i] += dist[j][i];
			}
			dist_tot[i] = dist_tot[i]/10000.0;
		}
	auto wall_t2 = get_time::now();
	
    auto diff = wall_t2 - wall_t1;
    std::cout<<"\nComputation time: "<<chrono::duration_cast<std::chrono::nanoseconds>(diff).count()<<" ns.\n";
	std::cout<<"Average Distances travelled by the particles for this scheduling policy are: ";
	for(int i=0; i<10;++i)
	{
		std::cout<<"\n\tAfter "<<(500*(i+1))<<" timesteps: "<<dist_tot[i];
	}
	std::cout<<"\n";

	return 0;
}
