#include <iostream>
using namespace std;
#include "cpu.h"

int main() {
    CPU cpu;
    int dataChoice;

    cout << "Enter 1 to use sample data, 2 to input yourself:";
    cin >> dataChoice;

    switch (dataChoice) {
        case 1:
            setSampleData(cpu);
            break;
        case 2:
            cpu.getData();
            cpu.showState();
            break;
        default:
            cerr << "Invalid-Choice!\n";
            exit(-1);
    }

    testAlgorithms(cpu);

    return 0;
}