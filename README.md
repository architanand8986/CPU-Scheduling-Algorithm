# CPU_Scheduling_Algorithms

This project contains the implementation of 7 basic CPU Scheduling Algorithms in C++ for terminal or code editor execution, along with a graphical user interface (GUI) built using Python's Tkinter library.

## Algorithms Included:
1. **FCFS** (First Come First Serve)
2. **SJF** (Shortest Job First)
3. **HRRN** (Highest Response Ratio Next)
4. **SRTF** (Shortest Remaining Time First)
5. **Preemptive Priority Scheduling**
6. **Non-Preemptive Priority Scheduling**
7. **Round Robin**

## GUI Implementation:
The GUI for these scheduling algorithms is built using the Tkinter library in Python. It allows users to input processes with their respective burst times, arrival times, and priorities. Based on the selected scheduling algorithm, it computes and displays the waiting times, turnaround times, and a Gantt chart for visualization.

### Features of the GUI:
- **Process Input:** Allows users to enter Process ID, Arrival Time, Burst Time, and Priority (if applicable).
- **Algorithm Selection:** Dropdown menu to select one of the 7 scheduling algorithms.
- **Time Quantum Input:** Input field for time quantum when Round Robin is selected.
- **Compute Schedule:** Button to compute and display the schedule.
- **Clear Data:** Button to clear all inputs and outputs.

### Python Library Used:
- **Tkinter:** Used for creating the graphical user interface.
