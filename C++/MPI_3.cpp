#include <stdio.h>
#include "mpi.h"
#include <iostream>

using namespace std;
int comp1(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}
int main(int* argc, char** argv)
{
	const int str_cnt = 8, col_cnt = 7;
	int send_mas[str_cnt][col_cnt];
	int mas[7];
	int cnt;
	MPI_Status status;
	int thread, thread_size;
	int interval;
	int size = 7;
	double cpu_time_star, cpu_time_fini;

	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread);
	MPI_Comm_size(MPI_COMM_WORLD, &thread_size);

	if (thread == 0)
	{
		cout << "aaaaaa";
		srand(time(0));
		cout << endl << endl << "Process #" << thread << ", generate and sent matrix to threads: " << endl;
		for (int j = 0; j < str_cnt; j++) {
			for (int i = 0; i < col_cnt; i++) {
				send_mas[j][i] = (1 + rand() % 200) + 1;
				cout << send_mas[j][i] << " ";
			}
			cout << endl;
		}
		/*cout<<"aaaaa" << &send_mas[1];
		fflush(stdout);*/

		//cout << str_cnt / thread_size;
		if (str_cnt / thread_size == 0)
		{
			qsort(send_mas[0], col_cnt, sizeof(int), comp1);
			for (int i = 1; i < str_cnt; i++)
			{


				MPI_Send(&send_mas[i][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD);
				//MPI_Recv(&send_mas[i][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD, &status);
				/*cout << "kak";
				fflush(stdout);*/

			}
			for (int i = 1; i < str_cnt; i++)
			{


				//MPI_Send(&send_mas[i][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD);
				MPI_Recv(&send_mas[i][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD, &status);
				/*cout << "kak";
				fflush(stdout);*/

			}

		}
		if (str_cnt / thread_size > 0)
		{
			
			cnt = str_cnt;
			for (int i = 1; i < thread_size; i++)
			{
				/*for (int a=0;a< ((i)*str_cnt / thread_size);a++)
					qsort(send_mas[a], col_cnt, sizeof(int), comp1);*/
				//cout << "aaa";
			//	for (int j = ((i)*str_cnt / thread_size); j < ((i + 1) * str_cnt / thread_size); j++)
				for (int j = ((i-1)*str_cnt / (thread_size-1)); j < ((i ) * str_cnt / (thread_size-1)); j++)
				{
					
					/*cout << str_cnt / thread_size<<"     ooo     ";
					cout<<i << "ii" << (i) * str_cnt / thread_size<<"		str   " << (i+1) * str_cnt / thread_size<<endl;
					cout << j<<endl;
					fflush(stdout);*/
					//cout<<"___" << send_mas[j][0] << endl;
					MPI_Send(&send_mas[j][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD);
					//MPI_Recv(&send_mas[j][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD, &status);

				}
				/*for (int j = str_cnt - str_cnt % thread_size + 1; j <= str_cnt; j++)
				{
					MPI_Send(&send_mas[j][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD);
				}*/
			}
			for (int i = 1; i < thread_size; i++)
			{
				/*for (int a=0;a< ((i)*str_cnt / thread_size);a++)
					qsort(send_mas[a], col_cnt, sizeof(int), comp1);*/
					//cout << "aaa";
				//	for (int j = ((i)*str_cnt / thread_size); j < ((i + 1) * str_cnt / thread_size); j++)
				for (int j = ((i - 1) * str_cnt / (thread_size - 1)); j < ((i)*str_cnt / (thread_size - 1)); j++)
				{

					/*cout << str_cnt / thread_size<<"     ooo     ";
					cout<<i << "ii" << (i) * str_cnt / thread_size<<"		str   " << (i+1) * str_cnt / thread_size<<endl;
					cout << j<<endl;
					fflush(stdout);*/
					//cout<<"___" << send_mas[j][0] << endl;
					//MPI_Send(&send_mas[j][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD);
					MPI_Recv(&send_mas[j][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD, &status);

				}
				/*for (int j = str_cnt - str_cnt % thread_size + 1; j <= str_cnt; j++)
				{
					MPI_Send(&send_mas[j][0], col_cnt, MPI_INT, i, 100, MPI_COMM_WORLD);
				}*/
			}
		}
		cout << "New_maatrix" << endl;
		for (int j = 0; j < str_cnt; j++) {
			for (int i = 0; i < col_cnt; i++) {
				//send_mas[j][i] = (1 + rand() % 200) + 1;

				cout << send_mas[j][i] << " ";
			}
			cout << endl;
		}

	}
	if (thread > 0 && thread < thread_size && str_cnt / thread_size > 0)
	{
		for (int j = ((thread-1)*str_cnt / (thread_size-1)); j < ((thread) * str_cnt / (thread_size-1)); j++)
		{
			//cout<<endl << thread<< "     ooo     ";

			MPI_Recv(&mas, col_cnt, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &status);
			
			qsort(mas, col_cnt, sizeof(int), comp1);
			/*for (int i = 0; i < col_cnt; i++)
				cout << mas[i] << "   ";
			fflush(stdout);*/
			MPI_Send(&mas, col_cnt, MPI_INT, 0, 100, MPI_COMM_WORLD);
			/*for (int i = 0; i < col_cnt; i++)
			{*/
			/*cout << "#" << thread << "#" << mas[0] << " ";
			fflush(stdout);*/
			//	}
			//	cout << endl;

		}
		/*for (int j = str_cnt - str_cnt % thread_size + 1; j <= str_cnt; j++)
		{
			cout << endl << str_cnt / thread_size << "     ooo     ";
			cout << thread << "ii" << (thread)*str_cnt / thread_size << "		str   " << (thread + 1) * str_cnt / thread_size << endl;
			cout << j << endl;
			fflush(stdout);
			MPI_Recv(&mas, col_cnt, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &status);
			cout << mas[0];
			fflush(stdout);
		}*/
	}

	if (thread > 0 && thread < str_cnt && str_cnt / thread_size == 0)
	{
		MPI_Recv(&mas, col_cnt, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &status);
		qsort(mas, col_cnt, sizeof(int), comp1);
		MPI_Send(&mas, col_cnt, MPI_INT, 0, 100, MPI_COMM_WORLD);
		/*for (int i = 0; i < col_cnt; i++)
		{*/
		/*cout<< "#" <<thread<<"#" << mas[0] << " ";
		fflush(stdout);*/
		//}
	}
	//	if (thread == thread_size - 1)
	//	{
	//		interval = size / thread_size + size % thread_size;
	//		//interval = size / thread_size;
	//		//cout << "int" <<interval<< endl;
	//		fflush(stdout);
	//	}
	//	MPI_Send(&mas, 15, MPI_INT, 1, 100, MPI_COMM_WORLD);
	//}
	//else
	//{
	//	MPI_Recv(&mas, 15, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &status);
	//	cout << "Process #" << thread << ", recieve vector:";
	//	for (int i = 0; i < 15; i++) {

	//		cout << mas[i] << "  ";
	//	}
	//}
	////if (thread == thread_size - 1)
	////{
	////	cpu_time_fini = MPI_Wtime();
	////	
	////	//int time = cpu_time_fini - cpu_time_star;
	////	//printf("CPU Time: %lf ms\n", (cpu_time_fini - cpu_time_star) * 1000);
	////}



	MPI_Finalize();

}
