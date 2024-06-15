#include <iostream>
#include <queue>
using namespace std;
using namespace std;
class Process
{
public:
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rt;
    // bool isready;

    Process(int i, int j, int k)
    {
        pid = i;
        at = j;
        bt = k;
        ct = -1;
        tat = -1;
        wt = -1;
        rt = bt;
        // isready=false;
    }
};
int cur_time = 0;

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

// custom comparator for priority_queue
class Compare
{
public:
    bool operator()(Process *p1, Process *p2)
    {
        if (ResponseRatio(p1) < ResponseRatio(p2))
            return true;
        if (ResponseRatio(p1) == ResponseRatio(p2))
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

    float ResponseRatio(Process *p1)
    {
        int w = cur_time - p1->at + p1->bt;
        return (float)w / p1->bt;
    }
};

void generateParams(vector<Process *> &process)
{
    for (int i = 0; i < process.size(); i++)
    {
        process[i]->tat = process[i]->ct - process[i]->at;
        process[i]->wt = process[i]->tat - process[i]->bt;
        // process[i]->rt = process[i]->ct - process[i]->at - process[i]->bt;
    }

}

void HRRN(vector<Process *> &process)
{
    sort(process.begin(), process.end(), cmp_at);
    priority_queue<Process *, vector<Process *>, Compare> ready;
    int timer = process[0]->at;
    cur_time = timer;
    for (int i = 0; i < process.size(); i++)
    {
        if (process[i]->at > timer)
            break;
        ready.push(process[i]);
    }
    cout<<"Gant Chart"<<endl;
    while (!ready.empty())
    {
        
        auto execution = ready.top();
        ready.pop();
        auto temp=timer;
        timer += execution->bt;
        execution->rt = 0;
        execution->ct = timer;
        cur_time = timer;
        cout << execution->pid << " -> ["<<temp<<","<<execution->ct<<"]"<<endl;
        
        // cout<<execution->pid<<"";
        
        // cout<<"["<<temp<<" , "<<execution->ct<<"]   ";
        
        while (!ready.empty())
        {
            ready.pop();
        }
        for (int l = 0; l < process.size(); l++)
        {
            if (process[l]->at <= timer)
            {
                if (process[l]->rt > 0)
                {
                    ready.push(process[l]);
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
         << "W.T"
         <<"  R.T"<< endl;
    for (auto i : process)
    {
        cout << i->pid << "  | " << i->at << " | " << i->bt << " | " << i->ct << " | " << i->tat << "   | " << i->wt <<"  |  "<<i->rt<< endl;
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
            int pid, at, bt;
            cin >> pid >> at >> bt;
            Process *p1 = new Process(pid, at, bt);
            process.push_back(p1);
        }
    }
    else if (a == 0)
    {
        Process *p1 = new Process(1, 0, 6);
        Process *p2 = new Process(4, 5, 1);
        Process *p3 = new Process(2, 2, 1);
        Process *p4 = new Process(3, 2, 2);

        process.push_back(p1);
        process.push_back(p2);
        process.push_back(p3);
        process.push_back(p4);
        HRRN(process);
    }

    return 0;
}