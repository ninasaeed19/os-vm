#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <cstdlib>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>

using namespace std;

extern char **environ;

void prompt() {
    char c[PATH_MAX];
    if (getcwd(c, sizeof(c)) != NULL) cout << "Current dir: " << c << " > ";
}

vector<string> split(string s) {
    stringstream ss(s);
    string t;
    vector<string> v;
    while(ss >> t) v.push_back(t);
    return v;
}

void cmd_cd(vector<string> a) {
    if(a.size()==1) cout << "You are at: " << getcwd(NULL,0) << endl;
    else {
        if(chdir(a[1].c_str())!=0) perror("oops cd failed");
        else setenv("PWD", a[1].c_str(),1);
    }
}

void cmd_dir(vector<string> a) {
    string p = ".";
    if(a.size()>1) p = a[1];
    DIR *d = opendir(p.c_str());
    if(!d){ perror("dir failed"); return;}
    struct dirent *e;
    while((e = readdir(d)) != NULL) cout << e->d_name << endl;
    closedir(d);
}

void cmd_env() {
    for(int i=0; environ[i]; i++) cout << "env: " << environ[i] << endl;
}

void cmd_set(vector<string> a) {
    if(a.size()<3){ cout << "Usage: set VAR VAL\n"; return; }
    setenv(a[1].c_str(), a[2].c_str(),1);
    cout << "Set " << a[1] << " = " << a[2] << endl;
}

void cmd_echo(vector<string> a) {
    for(size_t i=1;i<a.size();i++){
        cout << a[i];
        if(i!=a.size()-1) cout << " ";
    }
    cout << endl;
}

void cmd_help() {
    cout << "Hey, student shell commands:\n";
    cout << "cd [DIR]     - go to directory\n";
    cout << "dir [DIR]    - show files\n";
    cout << "environ      - show env vars\n";
    cout << "set VAR VAL  - set env var\n";
    cout << "echo [TXT]   - print text\n";
    cout << "pause        - wait a sec\n";
    cout << "quit         - exit shell\n";
    cout << "Supports >, >>, < and & for fun stuff\n";
}

void cmd_pause() {
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}

void run(vector<string> args, bool bg) {
    pid_t pid = fork();
    if(pid==0){
        for(size_t i=0;i<args.size();i++){
            if(args[i]=="<" && i+1<args.size()){ freopen(args[i+1].c_str(),"r",stdin); args.erase(args.begin()+i,args.begin()+i+2); i--; }
            else if(args[i]==">" && i+1<args.size()){ freopen(args[i+1].c_str(),"w",stdout); args.erase(args.begin()+i,args.begin()+i+2); i--; }
            else if(args[i]==">>" && i+1<args.size()){ freopen(args[i+1].c_str(),"a",stdout); args.erase(args.begin()+i,args.begin()+i+2); i--; }
        }
        vector<char*> cargs;
        for(auto &s:args) cargs.push_back(&s[0]);
        cargs.push_back(NULL);
        execvp(cargs[0],cargs.data());
        perror("exec fail");
        exit(1);
    } else if(pid>0){
        if(!bg) waitpid(pid,NULL,0);
    } else perror("fork fail");
}

int main(int argc,char* argv[]){
    string l;
    istream* in = &cin;
    ifstream bf;
    if(argc>1){ bf.open(argv[1]); if(!bf){ cerr<<"Cannot open file\n"; return 1;} in=&bf; }

    while(true){
        if(in==&cin) prompt();
        if(!getline(*in,l)) break;
        vector<string> args = split(l);
        if(args.empty()) continue;
        bool bg = (args.back()=="&");
        if(bg) args.pop_back();
        if(args[0]=="quit") break;
        else if(args[0]=="cd") cmd_cd(args);
        else if(args[0]=="dir") cmd_dir(args);
        else if(args[0]=="environ") cmd_env();
        else if(args[0]=="set") cmd_set(args);
        else if(args[0]=="echo") cmd_echo(args);
        else if(args[0]=="help") cmd_help();
        else if(args[0]=="pause") cmd_pause();
        else run(args,bg);
    }
    return 0;
}
