#include <thread>
#include <iostream>
#include <time.h>
#include <chrono>
using namespace std;
//using ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;

void worker(int* x, int* y,int id,int cnt) {
    //std::cout << "Hello from " << (id/cnt) << std::endl;
    for(int i=id; i<cnt; ++i){
        x[i] = x[i] + (2*y[i]);
        ++i;
    }
}

//Work 

int main()
{
    int thrd_cnt = 0;
    //int* arr_x, arr_y;
    std::thread thrd_ref[1000];
    int inp_x[100000],inp_y[100000];
    int index, ptr, i, k = 5;
    int cnt_per_thrd;
    //std::chrono::high_resolution_clock::time_point wall_t1;
    //std::chrono::high_resolution_clock::time_point wall_t2;
    //std::chrono::high_resolution_clock::time_point diff;
    //timespec wall_t1, wall_t2;
    //timespec comp_t1, comp_t2;
    //std::chrono::high_resolution_clock::time_point comp_t1;
    //std::chrono::high_resolution_clock::time_point comp_t2;
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();
    std::cout<<"Enter the number of threads to launch: ";
    std::cin>>thrd_cnt;
    std::cout<<"Enter size of array to compute for (< 100000): ";
    std::cin>>index;
    //arr_x = (int*) new int[index];
    //arr_y = (int*) new int[index];
    for (ptr=0; ptr<index; ++ptr){
        //arr_x[ptr] = 1;
        //arr_y[ptr] = 1;
        inp_x[ptr] = 1;
        inp_y[ptr] = 1;
    }
    cnt_per_thrd = index/thrd_cnt;
    auto wall_t1 = get_time::now();
    //clock_gettime(CLOCK_REALTIME,&wall_t1);
    for(ptr=0, i=0; ptr < index; ptr += cnt_per_thrd, ++i){
        //thrd_ref[i] = std::thread(worker, arr_x, arr_y, ptr, cnt_per_thrd);
        thrd_ref[i] = std::thread(worker, inp_x, inp_y, ptr, cnt_per_thrd);
    }
    for(ptr=0; ptr < i; ++ptr){
        thrd_ref[ptr].join();
    }
    auto wall_t2 = get_time::now();

    //clock_gettime(CLOCK_REALTIME,&wall_t2);
    auto diff = wall_t2 - wall_t1;
    std::cout<<"\nComputation time for "<<index<<" number of elements is: "<<chrono::duration_cast<std::chrono::nanoseconds>(diff).count()<<" ns.\n";
    //std::cout<<"\nComputation time for "<<index<<" number of elements is: "<<chrono::duration_cast<std::chrono::microseconds>(diff).count()<<" us.\n";
    //std::cout<<"\nComputation time for "<<index<<" number of elements is: "<<chrono::duration_cast<std::chrono::milliseconds>(diff).count()<<" ms.\n";

    return 0;
}
