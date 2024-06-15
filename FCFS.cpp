#include<iostream>
#include<vector>
using namespace std;
// FCFS ALGORITHM

class Process{
public:
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rt;
    bool isready;

    Process(int i,int j,int k){
        pid=i;
        at=j;
        bt=k;
        ct=-1;
        tat=-1;
        wt=-1;
        rt=k;
        isready=false;
    }


};

bool cmp_at(Process* p1,Process* p2){
   if(p1->at>p2->at)return false;
   if(p1->at==p2->at){
     if(p1->pid>p2->pid)return false;
   }
   return true; 
}

void generateParams(vector<Process*>&process){
    for(int i=0;i<process.size();i++){
        process[i]->tat=process[i]->ct-process[i]->at;
        process[i]->wt=process[i]->tat-process[i]->bt;
    }
}

void fcfs(vector<Process*>process){
	sort(process.begin(),process.end(),cmp_at);
	int timer=process[0]->at;
    cout<<"Gant Chart"<<endl;
	for(int i=0;i<process.size();i++){
        int temp=timer;
		timer+=process[i]->bt;
		// cout<<process[i]->pid<<" | ";
       
        cout<<process[i]->pid<<" -> ["<<temp<<","<<timer<<"]"<<endl;
		process[i]->ct=timer;
	}

	 cout<<endl<<endl;

   generateParams(process);

   int schedule=0;
   int tt=0;
   int wt=0;
   for(auto i:process){
      schedule=max(schedule,i->ct);
      tt+=i->tat;
      wt+=i->wt;
   }
   

    
    cout<<"Pid"<<" "<<"A.T"<<" "<<"B.T"<<" "<<"C.T"<<" "<<"T.A.T"<<" "<<"W.T"<<endl;
   for(auto i:process){
    cout<<i->pid<<"  | "<<i->at<<" | "<<i->bt<<" | "<<i->ct<<" | "<<i->tat<<"   | "<<i->wt<<endl;
   }
   cout<<endl;
   cout<<"AVERAGE T.A.T -> "<<(float)tt/process.size()<<endl;
   cout<<"AVERAGE W.T -> "<<(float)wt/process.size()<<endl;
   cout<<"Scheduling Length -> "<<schedule<<endl;

}

int main(){
	int a;
	cin >> a;
	vector<Process *> process;
	if (a == 1)
	{//CASE 1
		int n;
		cin >> n;
		for (int i = 0; i < n; i++)
		{
			int pid, at, bt, priority;
			cin >> pid >> at >> bt >> priority;
			Process *p1 = new Process(pid, at, bt);
			process.push_back(p1);
		}
		fcfs(process);
	}
	else if (a == 0)
	{
		// CASE-2
		Process *p1 = new Process(1, 0, 6);
		Process *p2 = new Process(4, 5, 1);
		Process *p3 = new Process(2, 2, 1);
		Process *p4 = new Process(3, 2, 2);

		process.push_back(p1);
		process.push_back(p2);
		process.push_back(p3);
		process.push_back(p4);
		fcfs(process);
	}
    return 0;

}