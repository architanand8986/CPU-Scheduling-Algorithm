#include <iostream>
#include <queue>
#include <vector>
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
    bool isready;

    Process(int i, int j, int k)
    {
        pid = i;
        at = j;
        bt = k;
        ct = -1;
        tat = -1;
        wt = -1;
        rt = k;
        isready = false;
    }
};

// comparator function for custom sorting
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

void generateParams(vector<Process *> &process)
{
    for (int i = 0; i < process.size(); i++)
    {
        process[i]->tat = process[i]->ct - process[i]->at;
        process[i]->wt = process[i]->tat - process[i]->bt;
    }
}

void Round_Robin(vector<Process *> &process, int quantum)
{
    sort(process.begin(), process.end(), cmp_at);
    queue<Process *> ready;

    int timer = process[0]->at;
    for (int i = 0; i < process.size(); i++)
    {
        if (process[i]->at > timer)
            break;
        ready.push(process[i]);
        process[i]->isready = true;
    }

    while (!ready.empty())
    {
        auto execution = ready.front();
        ready.pop();
        int temp=timer;
        execution->isready = false;
        // cout<<execution->pid<<"|"<<timer<<" ";
         

        if (execution->rt > quantum)
        {
            timer += quantum;
            execution->rt -= quantum;
        }
        else if (execution->rt <= quantum)
        {
            timer += execution->rt;
            execution->rt = 0;
            execution->ct = timer;
        }

        cout<<execution->pid<<" -> ["<<temp<<","<<timer<<"]"<<endl;

        for (int l = 0; l < process.size(); l++)
        {
            if (process[l]->isready || process[l] == execution)
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

        if (execution->rt > 0)
        {
            ready.push(execution);
            execution->isready = true;
        }
    }

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
cout<<endl;
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
        int n, tq;
        cin >> n >> tq;
        if(tq<0 || tq>10){
            cout<<"Enter valid Time quantum"<<endl;
        }
        for (int i = 0; i < n; i++)
        {
            int id, at, bt;
            cin >> id >> at >> bt;
            Process *p1 = new Process(id, at, bt);
            process.push_back(p1);
        }

        Round_Robin(process, tq);
    }
    else if (a == 0)
    {
        int tq = 2;
        Process *p1 = new Process(1, 0, 6);
        Process *p2 = new Process(4, 5, 1);
        Process *p3 = new Process(2, 2, 1);
        Process *p4 = new Process(3, 2, 2);

        process.push_back(p1);
        process.push_back(p2);
        process.push_back(p3);
        process.push_back(p4);
        Round_Robin(process, tq);
    }
    return 0;
}
