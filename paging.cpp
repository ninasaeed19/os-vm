 #include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;

int main() {
    ifstream file("pages.txt");
    if (!file) {
        cout << "file not found\n";
        return 0;
    }

    vector<int> pages;
    int p;
    while (file >> p) pages.push_back(p);

    int total_refs = pages.size();
    int max_frames = 10;

    cout << "frames,faults_per_1000\n";

    for (int frames = 1; frames <= max_frames; frames++) {
        unordered_map<int, unsigned int> age;
        int faults = 0;
        int MSB = sizeof(unsigned int) * 8 - 1;

        for (int i = 0; i < total_refs; i++) {
            for (auto &x : age)
                x.second >>= 1;

            int page = pages[i];

            if (age.count(page)) {
                age[page] |= (1u << MSB);
            } else {
                faults++;

                if ((int)age.size() < frames) {
                    age[page] = (1u << MSB);
                } else {
                    int victim = -1;
                    unsigned int min_age = UINT_MAX;

                    for (auto &x : age) {
                        if (x.second < min_age) {
                            min_age = x.second;
                            victim = x.first;
                        }
                    }

                    age.erase(victim);
                    age[page] = (1u << MSB);
                }
            }
        }

        double faults_per_1000 = (double)faults * 1000 / total_refs;
        cout << frames << "," << faults_per_1000 << endl;
    }

    return 0;
}