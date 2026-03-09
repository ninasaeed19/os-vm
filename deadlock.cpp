#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {

    ifstream file("deadlock.txt");

    int p,r;
    file>>p>>r;

    vector<int> E(r);
    for(int i=0;i<r;i++)
        file>>E[i];

    vector<vector<int>> C(p,vector<int>(r));
    for(int i=0;i<p;i++)
        for(int j=0;j<r;j++)
            file>>C[i][j];

    vector<vector<int>> R(p,vector<int>(r));
    for(int i=0;i<p;i++)
        for(int j=0;j<r;j++)
            file>>R[i][j];

    vector<int> allocated(r,0);
    for(int i=0;i<p;i++)
        for(int j=0;j<r;j++)
            allocated[j]+=C[i][j];

    vector<int> available(r);
    for(int i=0;i<r;i++)
        available[i]=E[i]-allocated[i];

    vector<bool> finished(p,false);

    bool progress=true;

    while(progress){
        progress=false;

        for(int i=0;i<p;i++){

            if(finished[i]) continue;

            bool can_run=true;

            for(int j=0;j<r;j++){
                if(R[i][j] > available[j]){
                    can_run=false;
                    break;
                }
            }

            if(can_run){

                for(int j=0;j<r;j++)
                    available[j]+=C[i][j];

                finished[i]=true;
                progress=true;
            }
        }
    }

    bool deadlock=false;

    for(int i=0;i<p;i++){
        if(!finished[i]){
            deadlock=true;
        }
    }

    if(deadlock){
        cout<<"Deadlock detected in processes: ";
        for(int i=0;i<p;i++)
            if(!finished[i])
                cout<<"P"<<i<<" ";
        cout<<endl;
    }
    else{
        cout<<"No deadlock detected"<<endl;
    }

    return 0;
}