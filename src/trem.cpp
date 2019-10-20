#include <stdio.h>
#include <iostream>
#include <queue>

using namespace std;


//struct defining time with attributes hora, min and a boolean indicating if it is a time of arrival
typedef struct time {
	int hora;
	int min;
	int isArrival;

	//operator defined for utilizing template of priority_queue
	//It is inverted because we want a min-heap instead of a max-heap
	//Note arrivals will come first in the priority_queue
	bool operator<(const struct time& b) const {
		return hora > b.hora || (hora == b.hora && min > b.min) || (hora ==b.hora && min == b.min && isArrival); 
	}
} Time;

int main(int argc, char const *argv[]) {
	Time t;
	int n;
	int current = 0; //amount of trains in a given moment
	int max = -1; //maximum amount of trans in a given moment
	priority_queue<Time> horarios;
	FILE* input;
	
	//Begin - Reading input file
	input = fopen(argv[1], "r");
	if (input == NULL) {
		cout << "Error in file reading" << endl;
		exit(1);
	}


	fscanf(input, "%d", &n);
	
	t.isArrival = true; //this indicates the first n readings are about the arrivals
	for (int i = 0; i < n; i++) {
		fscanf(input, "%d:%d", &t.hora, &t.min);
		horarios.push(t);
	}
	t.isArrival = false; //this indicates the last n reading are about departures
	for (int i = 0; i < n; i++) {
		fscanf(input, "%d:%d", &t.hora, &t.min);
		horarios.push(t);
	}
	//End - Reading input file

	//Begin - Algorithm

	//While the priority queue is not empty, we keep verifying how many trains will
	//be at the station in the next instant
	//If the next "Time" refers to an arrival, the amount increases an unit, otherwise
	//the amount decreases. In the end, "max" has the value of the maximum number of
	//trains staying simultaneously at the station.
	while (!horarios.empty()) {
		if (horarios.top().isArrival) {
			current++;
			if (current > max) max = current;
		}
		else current--;

		horarios.pop();
	}
	
	cout << max << endl;
	//End - Algorithm
	return 0;
}