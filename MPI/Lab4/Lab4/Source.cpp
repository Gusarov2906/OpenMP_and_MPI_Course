#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <memory>
#include <vector>
#include <string>

using namespace std;

int* multiplyPolynoms(int* p1, int* p2, int size)
{
	int* result = new int[size];
	for (int i = 0; i < size; i++)
	{
		result[i] = 0;
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (p1[i] != 0 && p2[j] != 0)
			{
				result[i + j] += p1[i] * p2[j];
			}
		}
	}
	return result;
}

string showPoly(int* poly, int size)
{
	string str = "";
	bool prevNull = false;
	for (int i = 0; i < size; i++)
	{
		if (poly[i] != 0)
		{
			if ((i != 0) && (!prevNull))
			{
				str += " + ";
			}
			str += to_string(poly[i]) + "*x^" + to_string(i);
			prevNull = false;
		}
		else
		{
			prevNull = true;
		}
	}
	str += '\n';
	return str;
}

void removeNumbers(vector<int>& v, int numtasks)
{
	for (int i = v.size() - 1; i > 0; i--)
	{
		if (i % 2 != 0)
		{
			v.erase(v.begin() + i);
		}
	}
}

int main(int* argc, char** argv)
{
	int numtasks, rank;
	int* result = new int[1];
	*result = 0;
	int dataLength = atoi(argv[2]);

	MPI_Status status;
	MPI_Init(argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	int currentLength = pow(dataLength, numtasks);
	MPI_Datatype MPI_POLYNOM;
	MPI_Type_contiguous(10, MPI_INT, &MPI_POLYNOM);
	MPI_Type_commit(&MPI_POLYNOM);

	srand(time(NULL) + rank);
	int* p1 = new int[currentLength];
	int* p2 = new int[currentLength];
	for (int i = 0; i < currentLength; i++)
	{
		if (i < dataLength)
		{
			p1[i] = rand() % 2 + 1;
			p2[i] = rand() % 2 + 1;
			//cout << "C " << p1[i] << " " << p2[i] << endl;
		}
		else
		{
			p1[i] = 0;
			p2[i] = 0;
		}
	}
	//cout << "A "<< p1[currentLength - 16] <<" "<< currentLength << endl;
	//cout << "B " << p2[currentLength - 16] << " " << currentLength << endl;
	cout << to_string(rank) + "\t" + showPoly(p1, currentLength) + "\t" + showPoly(p2, currentLength) + "\n";

	Sleep(100);
	//cout << "hello from rank " + to_string(rank) + "\n";
	int* res = multiplyPolynoms(p1, p2, currentLength);

	vector<int> procNumbers;
	for (int i = 0; i < numtasks; i++)
	{
		procNumbers.push_back(i);
	}
	Sleep(1000);
	//cout << procNumbers.size() << endl;
	while (procNumbers.size() != 1)
	{

		for (int i = 0; i < procNumbers.size(); i++)
		{
			//cout << to_string(procNumbers[i]) + " " + to_string(i) + " " + to_string(rank) + "\n";
			if (procNumbers[i] == rank)
			{
				//cout << "hello from rank " + to_string(rank) + "\n";
				if (i % 2 == 0)
				{
					p1 = res;
					//cout << "send" + to_string(procNumbers[i]) + " to " + to_string(procNumbers[i]) + "\n";
					//MPI_Send(&res, 1, MPI_POLYNOM, procNumbers[i], 0, MPI_COMM_WORLD);
				}
				else
				{
					//cout << "send" + to_string(procNumbers[i]) + " to " + to_string(procNumbers[i - 1]) + "\n";
					MPI_Send(res, 1, MPI_POLYNOM, procNumbers[i - 1], 0, MPI_COMM_WORLD);
				}
				break;
			}
		}
		Sleep(1000);
		for (int i = 0; i < procNumbers.size(); i++)
		{
			if (procNumbers[i] == rank)
			{
				if (i % 2 == 0)
				{
					//cout << "recv" + to_string(procNumbers[i]) + " from " + to_string(procNumbers[i+1]) + "\n";

					//MPI_Recv(&p1, 1, MPI_POLYNOM, procNumbers[i], 0, MPI_COMM_WORLD, &status);
					MPI_Recv(p2, 1, MPI_POLYNOM, procNumbers[i + 1], 0, MPI_COMM_WORLD, &status);

					res = multiplyPolynoms(p1, p2, currentLength);

					//cout << "\n !!! " + to_string(rank) + showPoly(res, currentLength);
				}
			}
		}
		Sleep(100);
		removeNumbers(procNumbers, numtasks);
	}

	if (rank == 0)
	{
		cout << "\nresult:\t" + showPoly(res, currentLength);
	}

	MPI_Finalize();
	return 0;
}