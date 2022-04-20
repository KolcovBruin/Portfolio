// cpp_compiler_options_openmp.cpp
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <cmath> 
#include <iostream> 
#include <ctime>
using namespace std;
//const int n = 300;
//int dst = 0;
//int src[n];

int main(int argc, char* argv[]) {
    const int  el_cnt = 300;
    //int _mas[str_cnt][col_cnt];
    int mas[el_cnt];
    int cnt;
    int buf;
    int sum=0;
    int start;
   // MPI_Status status;
    srand(time(0));
    cout << "Generate"<<endl;
    for (int i = 0; i < el_cnt; i++) {
            //mas[i] = (1 + rand() %200 ) + 1;
        mas[i] = el_cnt - i;
            cout << mas[i] << "  ";
            sum += mas[i];
           

    }
    cout << endl;
    cout << "SUM:" << sum << endl;
   /* for (int i = 0; i < n; i++)
    {
        src[i] = i + 1;
        cout << "  " << src[i];
    }
    cout << endl;*/
    for (int i = 0; i < el_cnt; i++)
    {
        //cout << " i " << i;
      
       /* if (i % 2 == 0)
        {
            cnt = el_cnt - 1;
            start = 1;
            cout << "cnt " << cnt << " start " << start<<endl;
        }
        else
        {
            cnt = el_cnt - 1;
            start = 0;
            cout << "cnt " << cnt << " start " << start<<endl;
        }*/
       
#pragma omp parallel for private(buf) //num_threads(3)

        for (int j = 1 - i % 2; j < el_cnt-1; j=j+2)
        {
           //  cout<<endl<<" num:" << omp_get_thread_num()<<endl;
            if (mas[j] > mas[j + 1])
            {
                buf=mas[j];
                mas[j] = mas[j + 1];
                mas[j + 1] = buf;
            }
        }
        // еще один коряввый алгоритм пузырька
        //for (int j = 0; j < el_cnt-1; j ++)
        //{
        //    // cout<<endl<<" num:" << omp_get_thread_num()<<endl;
        //    if (mas[j] > mas[j + 1])
        //    {
        //        buf = mas[j];
        //        mas[j] = mas[j + 1];
        //        mas[j + 1] = buf;
        //    }
        //}
        //стандартный алгоритм пузырька
        //for (int j = el_cnt - 1; j > i; j--)
        //{
        //    if (mas[j-1] > mas[j])
        //    {
        //       // cout<<"omp" << omp_get_thread_num()<<endl;
        //       buf=mas[j-1];
        //       mas[j-1] = mas[j];
        //       mas[j] = buf;
        //    }
        //}

    }
    cout << "Sort"<<endl;
    sum = 0;
    for (int i = 0; i < el_cnt; i++) {
       // mas[i] = (1 + rand() % 200) + 1;
        cout << mas[i] << "  ";
        sum += mas[i];

    }

    cout << endl;
    cout << "SUM:" << sum << endl;
//#pragma omp parallel  
//    {
//        // cout << omp_get_num_threads();
//#pragma omp for //reduction(^:dst) 
//        for (int i = 0; i < n; i++)
//        {
//            //cout << omp_get_thread_num();
//            dst ^= src[i];
//        }
//
//
//#pragma omp single
//        {
//            cout << "dst: " << dst << endl;
//        }
//    }
    //unsigned int end_time = clock();
    //cout << "Time - " << end_time - start_time;
}
