#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process{
    int id;
    int arrival;
    int burst;
};

double fcfs(vector<Process> p){
    int time=0,total_wait=0;

    for(auto &pr:p){
        if(time<pr.arrival)
            time=pr.arrival;

        total_wait+=time-pr.arrival;
        time+=pr.burst;
    }

    return (double)total_wait/p.size();
}

double sjf(vector<Process> p){
    sort(p.begin(),p.end(),[](Process a,Process b){
        return a.burst<b.burst;
    });

    int time=0,total_wait=0;

    for(auto &pr:p){
        total_wait+=time;
        time+=pr.burst;
    }

    return (double)total_wait/p.size();
}

double rr(vector<Process> p,int q){
    queue<pair<int,int>> qu;
    int n=p.size();

    for(int i=0;i<n;i++)
        qu.push({i,p[i].burst});

    int time=0;
    vector<int> finish(n);

    while(!qu.empty()){
        auto cur=qu.front();
        qu.pop();

        int id=cur.first;
        int rem=cur.second;

        if(rem>q){
            time+=q;
            rem-=q;
            qu.push({id,rem});
        }else{
            time+=rem;
            finish[id]=time;
        }
    }

    int total_wait=0;

    for(int i=0;i<n;i++)
        total_wait+=finish[i]-p[i].burst;

    return (double)total_wait/n;
}

void bar(string name,double value){
    cout<<name<<" ";
    int len=value*2;
    for(int i=0;i<len;i++)
        cout<<"#";
    cout<<" "<<value<<endl;
}

int main(){

    vector<Process> p={
        {1,0,5},
        {2,1,3},
        {3,2,8},
        {4,3,6}
    };

    double w1=fcfs(p);
    double w2=sjf(p);
    double w3=rr(p,2);

    cout<<"Average Waiting Time"<<endl;

    bar("FCFS",w1);
    bar("SJF",w2);
    bar("RoundRobin",w3);

    return 0;
}