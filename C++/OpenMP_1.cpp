// cpp_compiler_options_openmp.cpp
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <cmath> 
#include <iostream> 
#include <ctime>
using namespace std;
const int n = 1000;
int dst=0;
int dst_2 = 0;
int cnt = 0;
short int src[n];

int main(int argc, char* argv[]) {
    unsigned int start_time = clock();
    for (int i = 0; i < n; i++)
    {
        src[i] = i + 1;
      //  cout<<"  " << src[i]<<endl;
       /* for (int j = 15; j >= 0; --j)
            cout << ((src[i] >> j) & 1);*/
      //  cout << endl;

    }
    cout << endl;
#pragma omp parallel  
    {
       // cout << omp_get_num_threads();
#pragma omp for schedule(dynamic,5) reduction(+:dst) 
        for (int i = 0; i < n; i++)
        {
            if (omp_get_thread_num() == 1)
            {
                dst_2 += src[i];
                cnt++;
             }
            dst += src[i];
        }


#pragma omp single
        {   
            cout << endl;
            cout<<"Sum in 1 thread = " << dst_2<<endl;
            cout << "CNT in 1 thread = " << cnt << endl;
           /* cout << dst;
           
                for (int j = 15; j >= 0; --j)
                    cout << ((dst >> j) & 1);
                cout << endl;*/

           
            cout<<"dst: " << dst<<endl;
        }
    }
    unsigned int end_time = clock();
    cout << "Time - " << end_time-start_time;
}
