#include <iostream>
#include <thread>

using namespace std;

template <class T>
T busqueda_paralela(){

}

int main() {
    unsigned int n = thread::hardware_concurrency();
    cout << n << endl;
    return 0;
}