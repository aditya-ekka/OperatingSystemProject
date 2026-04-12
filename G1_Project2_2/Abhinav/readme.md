# CPU Scheduling Simulator (Multicore)

This project is a terminal-based CPU scheduling simulator written in **C** that demonstrates how different scheduling algorithms behave in a **multiprocessor environment**.

It supports:
- **Round Robin Scheduling (RR)**
- **Multilevel Feedback Queue (MLFQ)**

The simulator prints a **tick-by-tick execution timeline (Gantt Chart)** and calculates performance metrics such as turnaround time, waiting time, and response time.

---


## Project Structure

```
main.c - Handles initialization and execution flow
algorithms.c - Contains scheduling logic (RR, MLFQ) and output functions
types.h - Data structures (Process, CPU, states)
Makefile - Build and run configuration
README.md - Documentation
```

---


---

## Algorithms

### 1. Round Robin (RR)

- Uses **time quantum = 4**
- Processes are scheduled in **cyclic order**
- Each process gets CPU for a fixed time slice
- If not completed, it goes back to the ready queue

**Key Points:**
- Fair scheduling
- Prevents starvation
- Suitable for time-sharing systems

---

### 2. Multilevel Feedback Queue (MLFQ)

- Uses **multiple priority queues**
- Higher priority queues execute first
- If a process uses full quantum → it is **demoted**
- Lower queue behaves like **FCFS**

**Queue Configuration:**
- Q0 → Quantum = 4  
- Q1 → Quantum = 8  
- Q2 → FCFS (no quantum limit)

**Key Points:**
- Adapts based on process behavior  
- Short processes finish quickly  
- Long processes gradually move to lower priority  

---

## How to Build and Run

### Using Makefile

```bash
make
./scheduler
```

## Usage

The tasks used in the simulation are **predefined** in the program.  
So, you don’t need to enter process details manually.

When you run the program:

1. Scheduling algorithm is executed

2. System simulates execution across multiple cores

3. Output includes::
   - Tick-by-tick Gantt chart
   - CPU allocation per core
   - Final performance statistics (Turnaround Time & Waiting Time)

## Sample Test Runs

### 1 : Round Robin (Quantum = 4)

#### Execution Timeline

```
--- Commencing Round Robin ----

Tick    Core 0  Core 1
---------------------------------
00      [P1]    [--]
01      [P1]    [P2]
02      [P1]    [P2]
03      [P1]    [P2]
04      [P1]    [P2]
05      [P1]    [P2]
06      [P1]    [P2]
07      [P1]    [P2]
08      [P3]    [P2]
09      [P3]    [P2]
10      [P3]    [P2]
11      [P4]    [P2]
12      [P4]    [P2]
13      [P4]    [P2]
14      [P4]    [P2]
15      [P4]    [P2]
16      [P4]    [P2]
...
127     [P19]   [--]
128     [P19]   [--]
```

---

#### Performance Statistics

```
TID     Arrival Burst   Start   Finish  Turnaround   Waiting
---     ------- -----   -----   ------  ----------   -------
P1          0      8      8       8       0            0
P2          1      25     26      25      0            0
P3          2      3      11      9       6            6
P4          5      12     23      18      6            6
P5          8      2      25      17      15           15
P6          10     40     65      55      15           15
P7          12     5      31      19      14           14
P8          15     6      37      22      16           16
P9          20     15     52      32      17           17
P10         22     4      56      34      30           30
P11         25     8      64      39      31           31
P12         30     50     114     84      34           34
P13         35     2      67      32      30           30
P14         40     9      76      36      27           27
P15         45     3      79      34      31           31
P16         50     18     97      47      29           29
P17         55     5      102     47      42           42
P18         60     7      109     49      42           42
P19         65     22     131     66      44           44
P20         70     4      118     48      44           44

```

---

#### Averages

```
Avg Turnaround Time : 36.05
Avg Waiting Time    : 23.65
Avg Response Time   : 23.65

```

---

### 2: MLFQ Scheduling 

#### Execution Timeline

```
--- Commencing Lottery Scheduling (Quantum = 4) ---
Tick    Core 0  Core 1
---------------------------------
00      [P1]    [--]
01      [P1]    [P2]
02      [P1]    [P2]
03      [P1]    [P2]
04      [P3]    [P2]
05      [P3]    [P4]
06      [P3]    [P4]
07      [P1]    [P4]
08      [P1]    [P4]
09      [P1]    [P5]
10      [P1]    [P5]
11      [P6]    [P2]
12      [P6]    [P2]
13      [P6]    [P2]
14      [P6]    [P2]
...
118     [P19]   [P12]
119     [P19]   [P12]
```

---

#### Performance Statistics

```
TID     Arrival Burst   Start   Finish  Turnaround   Waiting
---     ------- -----   -----   ------  ----------   -------
P1          0      8      11      11      3           0
P2          1      25     87      86      61          0
P3          2      3      7       5       2           2
P4          5      12     27      22      10          0
P5          8      2      11      3       1           1
P6          10     40     105     95      55          1
P7          12     5      38      26      21          3
P8          15     6      40      25      19          4
P9          20     15     90      70      55          3
P10         22     4      31      9       5           5
P11         25     8      48      23      15          2
P12         30     50     128     98      48          1
P13         35     2      37      2       0           0
P14         40     9      59      19      10          0
P15         45     3      50      5       2           2
P16         50     18     111     61      43          0
P17         55     5      65      10      5           1
P18         60     7      70      10      3           0
P19         65     22     121     56      34          0
P20         70     4      74      4       0           0
```

---

#### Averages

```
Avg Turnaround Time : 32.00
Avg Waiting Time    : 19.60
Avg Response Time   : 1.25
```

#### Observations
- Round Robin ensures fair CPU distribution among processes
- MLFQ improves performance by prioritizing shorter tasks
- Response time is minimized for high-priority processes
- Long processes are gradually pushed to lower priority queues

## Limitations
- No support for I/O or blocked states (all processes are CPU-bound)
- MLFQ does not implement aging (possible starvation in lower queues)
- Fixed number of queues and quantum values


---

