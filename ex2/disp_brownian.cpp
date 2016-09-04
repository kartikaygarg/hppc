#include <iostream>
#include <thread>
#include <random>
#include <time.h>
#include <chrono>
#include <omp.h>
using namespace std;
using ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;

int main()
{
	random_device rd;
	double pos[10000][3];
	double init_pos[10000][3];
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
			//std::cout<<"Hello from thread id: "<<omp_get_thread_num()<<"\n";
			for(int j=0;j<5000;++j)
			{
				pos[i][0] = pos[i][0] + (sqrt(2*del_t) * dist_y(mt));
				pos[i][1] = pos[i][1] + (sqrt(2*del_t) * dist_y(mt));
				pos[i][2] = pos[i][2] + (sqrt(2*del_t) * dist_y(mt));
				if(((j+1)/500 > 0) && ((j+1)%500 == 0))
				{
					dist[i][((j+1)/500)] += sqrt( pow((pos[i][0] - init_pos[i][0]),2) + pow((pos[i][1] - init_pos[i][1]),2) + pow((pos[i][2] - init_pos[i][2]),2) );
					//dist[i][(j/500)-1] += sqrt( pow((pos[i][0] - init_pos[i][0]),2) + pow((pos[i][1] - init_pos[i][1]),2) + pow((pos[i][2] - init_pos[i][2]),2) );
				}
				//dist[j/500] += sqrt( pow((temp_x - pos[i][0]),2) + pow((temp_y - pos[i][1]),2) + pow((temp_z - pos[i][2]),2) );
				//pos[i][0] = temp_x;
				//pos[i][1] = temp_y;
				//pos[i][2] = temp_z;
			}
		}
	auto wall_t3 = get_time::now();
	std::cout<<"Simulation complete! Calculating average distances.\n";
    auto diff_2 = wall_t3 - wall_t1;
    std::cout<<"\nSimulation time: "<<chrono::duration_cast<std::chrono::nanoseconds>(diff_2).count()<<" ns.\n";
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
	
    auto diff_3 = wall_t2 - wall_t3;
    std::cout<<"\nAverage calculation time: "<<chrono::duration_cast<std::chrono::nanoseconds>(diff_3).count()<<" ns.\n";
    auto diff = wall_t2 - wall_t1;
    std::cout<<"\nTotal Computation time: "<<chrono::duration_cast<std::chrono::nanoseconds>(diff).count()<<" ns.\n";
	std::cout<<"Average Distances travelled by the particles for this scheduling policy are: ";
	for(int i=0; i<10;++i)
	{
		std::cout<<"\n\tAfter "<<(500*(i+1))<<" timesteps: "<<dist_tot[i];
	}
	std::cout<<"\n";

	return 0;
}
