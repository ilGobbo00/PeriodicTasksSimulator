# Response Time Analysis for Concurrent Threads

This project is part of the "Concurrent and Real-Time Programming" course at the [University of Padua](https://unipd.it).

The main goal of the assignment is to:
```
Simulate dynamic periodic task execution. A pre-defined set of routines (each with a unique 
assigned name) shall be defined in advance with given processor usage, period, and deadline. 
Each routine shall be composed of a program loop followed by a nanosleep() call. The exact 
amount of CPU time and, consequently, the processor utilization can be determined in advance 
using the time Linux command. The execution supervisor shall listen in TCP/IP for requests 
for task activation/deactivation. The received message shall specify the name of the task to 
be activated. A given task can be activated multiple times, starting a new thread 
running the selected routine. Before accepting a request for a new task, a response time 
analysis shall be carried out to assess the schedulability of the system.
```

## Threads

The following threads have been created and analyzed:

| Thread Name           | Period [ms]   | Computation Time [ms] | Priority (1 = highest)    |
| -----------------     | -----------   | -----------           | ---------                 |
| READ                  | 500           | 10                    | 1                         | 
| STORE                 | 300           | 6                     | 1                         | 
| SEND                  | 200           | 10                    | 1                         | 

## Time Analyses
For the time analyses, the `time` command (Linux) has been used, and to obtain a mean value, the [Python script](https://github.com/ilGobbo00/repo/test.py) has been executed once for each task.

To check the correctness of the main program, some manual checks have been performed and are documented in the [info file](https://github.com/ilGobbo00/repo/info%20th).

## Usage
Compile the project using `make` and execute the following commands on two different terminals: 
- `./orchestrator <port>`
- `./client <port>`

Once the connection is established, launch or stop the threads with the command `<thread> <'start'/'end'> <id>`.
