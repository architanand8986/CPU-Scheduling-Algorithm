import tkinter as tk
from tkinter import ttk

class SchedulerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("CPU Scheduling Algorithms")

        self.processes = []
        self.burst_times = []
        self.arrival_times = []
        self.priorities = []
        self.time_quantum = 0

        self.setup_gui()

    def setup_gui(self):
        # Input Frame
        input_frame = tk.Frame(self.root, bd=2, relief=tk.SUNKEN)
        input_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")

        tk.Label(input_frame, text="Process ID").grid(row=0, column=0)
        tk.Label(input_frame, text="Arrival Time").grid(row=0, column=1)
        tk.Label(input_frame, text="Burst Time").grid(row=0, column=2)
        tk.Label(input_frame, text="Priority").grid(row=0, column=3)

        self.process_id_entry = tk.Entry(input_frame)
        self.arrival_time_entry = tk.Entry(input_frame)
        self.burst_time_entry = tk.Entry(input_frame)
        self.priority_entry = tk.Entry(input_frame)
        self.priority_entry.config(state=tk.DISABLED)

        self.process_id_entry.grid(row=1, column=0)
        self.arrival_time_entry.grid(row=1, column=1)
        self.burst_time_entry.grid(row=1, column=2)
        self.priority_entry.grid(row=1, column=3)

        add_button = tk.Button(input_frame, text="Add Process", command=self.add_process)
        add_button.grid(row=1, column=4, padx=10)

        algorithm_label = tk.Label(input_frame, text="Select Algorithm:")
        algorithm_label.grid(row=2, column=0, pady=10)

        self.algorithm_var = tk.StringVar()
        self.algorithm_var.set("FCFS")
        self.algorithm_menu = tk.OptionMenu(input_frame, self.algorithm_var, "FCFS", "SJF", "SRTF", "HRRN", "RR", "Priority Non-Preemptive", "Priority Preemptive")
        self.algorithm_menu.grid(row=2, column=1, pady=10)

        self.algorithm_var.trace('w', self.update_priority_field)

        tk.Label(input_frame, text="Time Quantum").grid(row=3, column=0)
        self.time_quantum_entry = tk.Entry(input_frame)
        self.time_quantum_entry.grid(row=3, column=1)
        self.time_quantum_entry.config(state=tk.DISABLED)

        compute_button = tk.Button(input_frame, text="Compute Schedule", command=self.compute_schedule)
        compute_button.grid(row=2, column=2, pady=10)

        clear_button = tk.Button(input_frame, text="Clear", command=self.clear_data)
        clear_button.grid(row=2, column=3, pady=10)

        # Result Frame
        result_frame = tk.Frame(self.root, bd=2, relief=tk.SUNKEN)
        result_frame.grid(row=3, column=0, padx=10, pady=10, sticky="nsew")

        self.result_tree = ttk.Treeview(result_frame, columns=("Process ID", "Arrival Time", "Burst Time", "Priority", "Waiting Time", "Turnaround Time"), show='headings')
        for col in self.result_tree["columns"]:
            self.result_tree.heading(col, text=col)
        self.result_tree.grid(row=0, column=0, sticky="nsew")

        self.gantt_chart_label = tk.Label(self.root, text="Gantt Chart:\n")
        self.gantt_chart_label.grid(row=4, column=0, padx=10, pady=10, sticky="nsew")

        self.root.grid_rowconfigure(3, weight=1)
        self.root.grid_columnconfigure(0, weight=1)

    def add_process(self):
        process_id = self.process_id_entry.get()
        arrival_time = int(self.arrival_time_entry.get())
        burst_time = int(self.burst_time_entry.get())
        priority = int(self.priority_entry.get()) if self.priority_entry.get() else 0

        self.processes.append(process_id)
        self.arrival_times.append(arrival_time)
        self.burst_times.append(burst_time)
        self.priorities.append(priority)

        self.process_id_entry.delete(0, tk.END)
        self.arrival_time_entry.delete(0, tk.END)
        self.burst_time_entry.delete(0, tk.END)
        self.priority_entry.delete(0, tk.END)

        self.update_process_list()

    def update_process_list(self):
        self.result_tree.delete(*self.result_tree.get_children())
        for i in range(len(self.processes)):
            self.result_tree.insert("", "end", values=(self.processes[i], self.arrival_times[i], self.burst_times[i], self.priorities[i], "-", "-"))

    def clear_data(self):
        self.processes.clear()
        self.burst_times.clear()
        self.arrival_times.clear()
        self.priorities.clear()
        self.result_tree.delete(*self.result_tree.get_children())
        self.gantt_chart_label.config(text="Gantt Chart:\n")

    def compute_schedule(self):
        algorithm = self.algorithm_var.get()
        if algorithm == "FCFS":
            self.compute_fcfs()
        elif algorithm == "SJF":
            self.compute_sjf()
        elif algorithm == "SRTF":
            self.compute_srtf()
        elif algorithm == "HRRN":
            self.compute_hrrn()
        elif algorithm == "RR":
            self.compute_rr()
        elif algorithm == "Priority Non-Preemptive":
            self.compute_priority_non_preemptive()
        elif algorithm == "Priority Preemptive":
            self.compute_priority_preemptive()

    def update_priority_field(self, *args):
        algorithm = self.algorithm_var.get()
        if algorithm in ["Priority Non-Preemptive", "Priority Preemptive"]:
            self.priority_entry.config(state=tk.NORMAL)
            self.time_quantum_entry.config(state=tk.DISABLED)
            self.time_quantum_entry.delete(0, tk.END)
        elif algorithm == "RR":
            self.priority_entry.config(state=tk.DISABLED)
            self.priority_entry.delete(0, tk.END)
            self.time_quantum_entry.config(state=tk.NORMAL)
        else:
            self.priority_entry.config(state=tk.DISABLED)
            self.priority_entry.delete(0, tk.END)
            self.time_quantum_entry.config(state=tk.DISABLED)
            self.time_quantum_entry.delete(0, tk.END)

    def compute_priority_non_preemptive(self):
        n = len(self.processes)
        completed = [False] * n
        current_time = 0
        waiting_times = [0] * n
        turnaround_times = [0] * n
        gantt_chart = "Gantt Chart:\n"

        while not all(completed):
            max_priority = -1
            index = -1
            for i in range(n):
                if self.arrival_times[i] <= current_time and not completed[i] and self.priorities[i] > max_priority:
                    max_priority = self.priorities[i]
                    index = i

            if index == -1:
                current_time += 1
                continue

            gantt_chart += f"{self.processes[index]}({current_time}-{current_time + self.burst_times[index]}) "
            current_time += self.burst_times[index]
            waiting_times[index] = current_time - self.arrival_times[index] - self.burst_times[index]
            turnaround_times[index] = waiting_times[index] + self.burst_times[index]
            completed[index] = True

        self.update_result_tree(waiting_times, turnaround_times)
        self.gantt_chart_label.config(text=gantt_chart)

    def compute_priority_preemptive(self):
        n = len(self.processes)
        remaining_times = [bt for bt in self.burst_times]
        completed = [False] * n
        current_time = 0
        waiting_times = [0] * n
        turnaround_times = [0] * n
        gantt_chart = "Gantt Chart:\n"

        while not all(completed):
            max_priority = -1
            index = -1
            for i in range(n):
                if self.arrival_times[i] <= current_time and not completed[i] and self.priorities[i] > max_priority:
                    max_priority = self.priorities[i]
                    index = i

            if index == -1:
                gantt_chart += f"Idle({current_time}-{current_time + 1}) "
                current_time += 1
                continue

            gantt_chart += f"{self.processes[index]}({current_time}-{current_time + 1}) "
            remaining_times[index] -= 1
            current_time += 1

            if remaining_times[index] == 0:
                waiting_times[index] = current_time - self.arrival_times[index] - self.burst_times[index]
                turnaround_times[index] = waiting_times[index] + self.burst_times[index]
                completed[index] = True

        self.update_result_tree(waiting_times, turnaround_times)
        self.gantt_chart_label.config(text=gantt_chart)

    def compute_fcfs(self):
        n = len(self.processes)
        waiting_times = [0] * n
        turnaround_times = [0] * n
        completion_time = [0] * n
        gantt_chart = "Gantt Chart:\n"
        
        for i in range(n):
            if i == 0:
                completion_time[i] = self.arrival_times[i] + self.burst_times[i]
            else:
                if self.arrival_times[i] > completion_time[i-1]:
                    completion_time[i] = self.arrival_times[i] + self.burst_times[i]
                else:
                    completion_time[i] = completion_time[i-1] + self.burst_times[i]
            
            turnaround_times[i] = completion_time[i] - self.arrival_times[i]
            waiting_times[i] = turnaround_times[i] - self.burst_times[i]
            gantt_chart += f"{self.processes[i]}({completion_time[i]-self.burst_times[i]}-{completion_time[i]}) "

        self.update_result_tree(waiting_times, turnaround_times)
        self.gantt_chart_label.config(text=gantt_chart)

    def compute_rr(self):
        time_quantum = int(self.time_quantum_entry.get())
        n = len(self.processes)
        remaining_times = [bt for bt in self.burst_times]
        current_time = 0
        waiting_times = [0] * n
        turnaround_times = [0] * n
        gantt_chart = "Gantt Chart:\n"

        queue = [i for i in range(n)]
        while queue:
            i = queue.pop(0)
            if remaining_times[i] > time_quantum:
                gantt_chart += f"{self.processes[i]}({current_time}-{current_time + time_quantum}) "
                current_time += time_quantum
                remaining_times[i] -= time_quantum
                for j in range(n):
                    if j != i and self.arrival_times[j] <= current_time and remaining_times[j] > 0 and j not in queue:
                        queue.append(j)
                queue.append(i)
            else:
                gantt_chart += f"{self.processes[i]}({current_time}-{current_time + remaining_times[i]}) "
                current_time += remaining_times[i]
                remaining_times[i] = 0
                waiting_times[i] = current_time - self.arrival_times[i] - self.burst_times[i]
                turnaround_times[i] = waiting_times[i] + self.burst_times[i]

        self.update_result_tree(waiting_times, turnaround_times)
        self.gantt_chart_label.config(text=gantt_chart)

    def update_result_tree(self, waiting_times, turnaround_times):
        self.result_tree.delete(*self.result_tree.get_children())
        for i in range(len(self.processes)):
            self.result_tree.insert("", "end", values=(self.processes[i], self.arrival_times[i], self.burst_times[i], self.priorities[i], waiting_times[i], turnaround_times[i]))

    def compute_sjf(self):
        n = len(self.processes)
        completed = [False] * n
        current_time = 0
        waiting_times = [0] * n
        turnaround_times = [0] * n
        gantt_chart = "Gantt Chart:\n"

        while not all(completed):
            min_burst = float('inf')
            index = -1
            for i in range(n):
                if self.arrival_times[i] <= current_time and not completed[i] and self.burst_times[i] < min_burst:
                    min_burst = self.burst_times[i]
                    index = i

            if index == -1:
                current_time += 1
                continue

            gantt_chart += f"{self.processes[index]}({current_time}-{current_time + self.burst_times[index]}) "
            current_time += self.burst_times[index]
            waiting_times[index] = current_time - self.arrival_times[index] - self.burst_times[index]
            turnaround_times[index] = waiting_times[index] + self.burst_times[index]
            completed[index] = True

        self.update_result_tree(waiting_times, turnaround_times)
        self.gantt_chart_label.config(text=gantt_chart)

    def compute_srtf(self):
        n = len(self.processes)
        remaining_times = [bt for bt in self.burst_times]
        completed = [False] * n
        current_time = 0
        waiting_times = [0] * n
        turnaround_times = [0] * n
        gantt_chart = "Gantt Chart:\n"

        while not all(completed):
            min_burst = float('inf')
            index = -1
            for i in range(n):
                if self.arrival_times[i] <= current_time and not completed[i] and remaining_times[i] < min_burst:
                    min_burst = remaining_times[i]
                    index = i

            if index == -1:
                gantt_chart += f"Idle({current_time}-{current_time + 1}) "
                current_time += 1
                continue

            gantt_chart += f"{self.processes[index]}({current_time}-{current_time + 1}) "
            remaining_times[index] -= 1
            current_time += 1

            if remaining_times[index] == 0:
                waiting_times[index] = current_time - self.arrival_times[index] - self.burst_times[index]
                turnaround_times[index] = waiting_times[index] + self.burst_times[index]
                completed[index] = True

        self.update_result_tree(waiting_times, turnaround_times)
        self.gantt_chart_label.config(text=gantt_chart)

    def compute_hrrn(self):
        n = len(self.processes)
        completed = [False] * n
        current_time = 0
        waiting_times = [0] * n
        turnaround_times = [0] * n
        gantt_chart = "Gantt Chart:\n"

        while not all(completed):
            response_ratio = -1
            index = -1
            for i in range(n):
                if self.arrival_times[i] <= current_time and not completed[i]:
                    rr = (current_time - self.arrival_times[i] + self.burst_times[i]) / self.burst_times[i]
                    if rr > response_ratio:
                        response_ratio = rr
                        index = i

            if index == -1:
                current_time += 1
                continue

            gantt_chart += f"{self.processes[index]}({current_time}-{current_time + self.burst_times[index]}) "
            current_time += self.burst_times[index]
            waiting_times[index] = current_time - self.arrival_times[index] - self.burst_times[index]
            turnaround_times[index] = waiting_times[index] + self.burst_times[index]
            completed[index] = True

        self.update_result_tree(waiting_times, turnaround_times)
        self.gantt_chart_label.config(text=gantt_chart)


if __name__ == "__main__":
    root = tk.Tk()
    app = SchedulerApp(root)
    root.mainloop()
