# Priority-Queues
Implementation of Johnson's APSP algorithm using 4 different priority queues and a comparision of their relative performance.

## Steps to run
Compile the program by-
```
g++ main.cpp
```
Execute
```
./a.out 1 # for simple array-based heap execution  
```
``` 
./a.out 2 # for binary heap based execution
``` 
```
./a.out 3 # For binomial heap based execution
``` 
```
./a.out 4 # For fibonacci heap based execution
```
If no argument is supplied, fibonacci heap based Johnson algorithm will be executed by default. 

## Input format
First line of the input will specify the number of test cases.\
For each test case, supply two integers N and D in the first line, specifying the number of vertices N, and D=0 for an undirected graph and D=1 for a directed graph.
The next N lines of the test case should contain the NxN adjacency matrix of the graph, with each element specifying the edge weight (-999<W<999). 
If an edge does not exit between two vertices, enter the edge weight as '999999'.

## Output format
For each test case,
1. -1 is printed if a negative cycle exists in the graph.
2. Else, N lines are printed, in which the k<sup>th</sup> entry in the i<sup>th</sup> line tells the lenght of the shortest path to the k<sup>th</sup> vertex from the i<sup>th</sup> vertex.

The last line of the output contains the time taken for each of the testcases to complete, with the k<sup>th</sup> entry corresponding to the k<sup>th</sup> testcase.
