#include <iostream>
#include <queue>
using namespace std;
class Process
{
public:
	int pid;
	int at;
	int bt;
	int priority;
	int ct;
	int rt;
	int tat;
	int wt;
	bool isready;

	Process(int i, int j, int k, int l)
	{
		pid = i;
		at = j;
		bt = k;
		priority = l;
		ct = -1;
		tat = -1;
		wt = -1;
		isready = false;
		rt = k;
	}
};
// custom comparator for sorting
bool cmp_at(Process *p1, Process *p2)
{
	if (p1->at > p2->at)
		return false;
	if (p1->at == p2->at)
	{
		if (p1->pid > p2->pid)
			return false;
	}
	return true;
}
// custom comparator for priority queue
class Compare
{
public:
	bool operator()(Process *p1, Process *p2)
	{
		if (p1->priority < p2->priority)
			return true;
		if (p1->priority == p2->priority)
		{
			if (p1->at > p2->at)
				return true;
			if (p1->at == p2->at)
			{
				if (p1->pid > p2->pid)
					return true;
			}
		}
		return false;
	}
};

void generateParams(vector<Process *> &process)
{
	for (int i = 0; i < process.size(); i++)
	{
		process[i]->tat = process[i]->ct - process[i]->at;
		process[i]->wt = process[i]->tat - process[i]->bt;
	}
}

void Non_Pre_Priority(vector<Process *> &process)
{
	sort(process.begin(), process.end(), cmp_at);
	priority_queue<Process *, vector<Process *>, Compare> ready;

	int timer = process[0]->at;

	for (int i = 0; i < process.size(); i++)
	{
		if (process[i]->at > timer)
			break;
		ready.push(process[i]);
		process[i]->isready = true;
	}
	cout<<"Gant Chart"<<endl;
	while (!ready.empty())
	{
		int temp=timer;
		auto execution = ready.top();
		ready.pop();
		timer += execution->bt;
		execution->rt = 0;
		execution->ct = timer;

		// cout << execution->pid << " | ";
		cout<<execution->pid<<" -> ["<<temp<<","<<timer<<"]"<<endl;
		for (int l = 0; l < process.size(); l++)
		{
			if (process[l]->isready)
				continue;
			if (process[l]->at <= timer)
			{
				if (process[l]->rt > 0)
				{
					ready.push(process[l]);
					process[l]->isready = true;
				}
			}
			else
				break;
		}
	}
	cout << endl
		 << endl;
	generateParams(process);
	int schedule = 0;
	int tt = 0;
	int wt = 0;
	for (auto i : process)
	{
		schedule = max(schedule, i->ct);
		tt += i->tat;
		wt += i->wt;
	}
	cout << "Pid"
		 << " "
		 << "A.T"
		 << " "
		 << "B.T"
		 << " "
		 << "C.T"
		 << " "
		 << "T.A.T"
		 << " "
		 << "W.T" << endl;
	for (auto i : process)
	{
		cout << i->pid << "  | " << i->at << " | " << i->bt << " | " << i->ct << " | " << i->tat << "   | " << i->wt << endl;
	}
	cout << endl;
	cout << "AVERAGE T.A.T -> " << (float)tt / process.size() << endl;
	cout << "AVERAGE W.T -> " << (float)wt / process.size() << endl;
	cout << "Scheduling Length -> " << schedule << endl;
}
int main()
{
	int a;
	cin >> a;
	vector<Process *> process;
	if (a == 1)
	{

		int n;
		cin >> n;

		for (int i = 0; i < n; i++)
		{
			int pid, at, bt, priority;
			cin >> pid >> at >> bt >> priority;
			Process *p1 = new Process(pid, at, bt, priority);
			process.push_back(p1);
		}
	}
	else if (a == 0)
	{
		Process *p1 = new Process(1, 0, 6, 10);
		Process *p2 = new Process(4, 5, 1, 20);
		Process *p3 = new Process(2, 2, 1, 30);
		Process *p4 = new Process(3, 2, 2, 40);

		process.push_back(p1);
		process.push_back(p2);
		process.push_back(p3);
		process.push_back(p4);
		Non_Pre_Priority(process);
	}
	return 0;
}
