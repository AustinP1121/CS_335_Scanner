/*
* Problem 1 
* Homework 1
* CS-440
* Austin pedigo
*
*/

#include <omp.h>
#include <iostream>
#include <random>
#include <math.h>

using namespace std;

int main()
{
	//vars for the start, end, and total time
	double _startTime = 0;
	double _endTime = 0;
	double _totalTime = 0;
	
	//var for the number of threads, set to 1 
	int _numOfThreads = 1;

	//vars for the total darts thrown and the number of darts hitting the target
	long long int _totalTosses = 1;
	long long int _totalHitsInCircle = 0;

	//instantiate the random number generator
	default_random_engine generator;

	//set bounds for -1 and 1
	uniform_real_distribution<double> distribution(-1, 1);

	//set the number of threads to use
	_numOfThreads = omp_get_max_threads();

	cout << "Pi Estimator Using the Monte Carlo Method" << endl;

	//ingest user input
	cout << "\nEnter the number of tosses: ";

	cin >> _totalTosses;  

	cout << "\nProcessing...\n\n";

	//loop to go through the multiple cores 
	for (int i = 1; i < _numOfThreads+1; i++)
	{
		//get the start time
		_startTime = omp_get_wtime();

		//begin parallel part of the program
		//distribute the private var to each thread
		//set the number of threads equal to the current iteration of i
		#pragma omp parallel private(_totalHitsInCircle) num_threads(i)
		{
			//each thread will process this block
			for (int toss = 0; toss < _totalTosses; toss++)
			{
				double _x, _y;
				double _distanceSquared;

				_x = distribution(generator);

				_y = distribution(generator);

				#pragma omp critical
				_distanceSquared = (_x * _x) + (_y * _y);
				
				if (_distanceSquared <= 1)
				{
					_totalHitsInCircle++;
				}
			}
		}
		//get the end time
		_endTime = omp_get_wtime();
		//estimate pi
		double _piEstimate = (4 * (double)_totalHitsInCircle) / (double)_totalTosses;

		//compute the total time
		_totalTime = _endTime - _startTime;

		//report
		cout << "\nUsing " << i << " threads...\n";
		cout << "\nToal hits: " << _totalHitsInCircle << endl;
		cout << "Pi estimate: " << _piEstimate << endl;
		cout << "Total elasped time in seconds: " << _totalTime << endl;
		
		//reset
		_totalHitsInCircle = 0;
	}
}