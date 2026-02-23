#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;
map<string,int> final_count;

void count_words(vector<string> words) {
    for(string w : words) {
        mtx.lock();
        final_count[w]++;
        mtx.unlock();
    }
}

int main() {
    ifstream file("input.txt");
    if(!file) {
        cout << "file not found\n";
        return 0;
    }

    vector<string> all_words;
    string word;

    while(file >> word) {
        all_words.push_back(word);
    }

    int n = 3;
    vector<thread> threads;

    int chunk_size = all_words.size() / n;

    for(int i=0; i<n; i++) {
        int start = i * chunk_size;
        int end = (i == n-1) ? all_words.size() : start + chunk_size;

        vector<string> part(all_words.begin()+start, all_words.begin()+end);
        threads.push_back(thread(count_words, part));
    }

    for(int i=0; i<n; i++) {
        threads[i].join();
    }

    cout << "final word frequency:\n";
    for(auto p : final_count) {
        cout << p.first << " : " << p.second << endl;
    }

    return 0;
}