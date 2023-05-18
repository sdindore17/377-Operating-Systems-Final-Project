#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

pqueue_arrival read_workload(string filename) {
  pqueue_arrival workload;
  Process p;
  std::fstream myfile;
  //open the file
  myfile.open(filename); 
  //read the file
  std::string line;
  char* num;
  char* a = new char[line.length() + 1];
  while(std::getline(myfile, line, '\n')) {
    int count = 0;
    strcpy(a, line.c_str());
    num = strtok(a, " ");
    while(num != NULL) {
      if (count == 0) {
        p.arrival = atoi(num);
      }
      else if(count == 1) {
        p.duration = atoi(num);
      }
      count++;
      num = strtok(NULL, " ");
    }
    workload.push(p);
  }
  return workload;
}

void show_workload(pqueue_arrival workload) {
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty()) {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes) {
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty()) {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion
         << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload) {
  list<Process> complete;
  int time = 0;
  //order the processes by arrival time
  pqueue_arrival copy = workload;
  while(copy.size() != 0) {
    Process p = copy.top();
    copy.pop();
    //fill in first_run and completion fields for process
    p.first_run = time;
    time = time + p.duration;
    p.completion = time;
    complete.push_back(p);
  }
  return complete;
}

list<Process> sjf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_duration arrived;
  int time = 0;
  //set time to first process' arrival time
  time = workload.top().arrival; 

  //add processes from workload to arrived
  while(workload.size() != 0) {
    Process p = workload.top();
    //initialize first_run
    p.first_run = -1; 
    arrived.push(p);
    workload.pop();
  }

  Process p;
  //to keep track of total duration time for duration field of process
  int count = 0; 
  while(arrived.size() != 0) {
    p = arrived.top();
    arrived.pop();

    if(p.first_run == -1) {
      //set first_run to current time
      p.first_run = time; 
    }
    
    //decrement duration by 1 for each time we loop through
    p.duration = p.duration - 1;
    time = time + 1;
    count = count + 1;

    //check if process is complete
    if(p.duration == 0) { 
      p.completion = time;
      //count is total duration time of process
      p.duration = count; 
      //add to completed list
      complete.push_back(p); 
      //reset count for next process
      count = 0; 
    }
    else {
      arrived.push(p);
    }
  }
  return complete;
}

list<Process> stcf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_duration arrived;
  int time = 0;
  //set time to first process' arrival time
  time = workload.top().arrival; 

  //add processes from workload to arrived
  while(workload.size() != 0) {
    Process p = workload.top();
    //initialize first_run 
    p.first_run = -1; 
    cout << p.arrival << '\n';
    arrived.push(p);
    workload.pop();
  }

  Process p;
  //to keep track of total duration time for duration field of process
  int count = 0; 
  while(arrived.size() != 0) {
    p = arrived.top();
    arrived.pop();


    if(p.first_run == -1) {
      //set first_run to current time
      p.first_run = time; 
    }
    
    //decrement duration by 1 for each time we loop through
    p.duration = p.duration - 1;
    time = time + 1;
    count = count + 1;

    //check if process is complete
    if(p.duration == 0) { 
      p.completion = p.arrival + time;
      //count is total duration time of process
      p.duration = count; 
      //add to completed list
      complete.push_back(p); 
      //reset count for next process
      count = 0; 
    }
    else {
      arrived.push(p);
    }
  }
  return complete;
}

list<Process> rr(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_arrival copy1 = workload;
  pqueue_arrival copy;
  pqueue_arrival other;
  
  //to keep track of total duration time for duration field of process
  int count = 0;
  int time = 0;

  int time1 = copy1.top().arrival;
  while(copy1.size() != 0){
    Process p = copy1.top();
    copy1.pop();
    p.first_run = p.arrival + time1;
    time1 = time1 + 1;
    copy.push(p);
  }

  time = copy.top().arrival;
  while(copy.size() != 0) {
    Process p = copy.top();
    copy.pop();
    
    p.duration = p.duration - 1;
    time = time + 1;
    p.completion = p.arrival + time;
    count = count + 1;
    
    //check if process is complete
    if(p.duration == 0) {
      //count is total duration time of process
      p.duration = count; 
      cout << p.first_run << "\n";
      //add to completed list
      complete.push_back(p); 
      //reset count for next process
      count = 0; 
    }
    else {
      other.push(p);
    }
    
    if(copy.size() == 0) {
      swap(copy, other);
    }
  }
  return complete;
}

float avg_turnaround(list<Process> processes) {
  float sum = 0;
  Process p;
  list<Process> copy = processes;
  while(copy.size() != 0) {
    p = copy.front();
    sum = sum + float(p.completion) - float(p.arrival);
    copy.pop_front();
  }
  float avg = sum / processes.size();
  return avg;
}

float avg_response(list<Process> processes) {
  float sum = 0;
  Process p;
  list<Process> copy = processes;
  while(copy.size() != 0) {
    p = copy.front();
    sum = sum + float(p.first_run) - float(p.arrival);
    copy.pop_front();
  }
  float avg = sum / processes.size();
  return avg;
}

void show_metrics(list<Process> processes) {
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_t << endl;
  cout << "Average Response Time:   " << avg_r << endl;
}


// *** TODO: IMPLEMENT THE MLFQ ALGORITHM ***

// main mlfq algorithm implementation
list<Process> mlfq(list<Process> processes) {
  pqueue_arrival copy;
  int minArrivalTime = -1;
  Process * p = NULL;
  int time = 0;

  // we are looking at the algorithm running on three queues
  int minLevel = 3;

  for (auto i = processes.begin(); i != processes.end(); i++) {
			// find the highest priority process that arrived at the earliest time
			if (i->arrival < minArrivalTime && i->queueLevel == minLevel) {
				minArrivalTime = i->arrival;
				p = &*i;
			}
		}

  // calculating the amount of time a process waits before priority gets reduced
		for (auto i = processes.begin(); i != processes.end(); i++) {
			if (&*i != p) {
				time++;
			}
		}
}

// method that finds the highest priority
int lowestVal(list<Process> processes) {
	int level = 3;
	for (auto i = processes.begin(); i != processes.end(); i++) {
		// finding the highest priority available given the parameter processes
		if (i->queueLevel < level) {
			level = i->queueLevel;
		}
	}
	return level;
}