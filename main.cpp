#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <stdlib.h>
#include <time.h>

const int NUM_HILOS = 5;

using namespace std;

mutex mtx;

typedef double TipoNumerico;

template <class T>
void busqueda_paralela(vector<T>& V, int lim_inf, int lim_sup, promise<TipoNumerico> promise){
    mtx.lock();
    ///Busqueda en el vector
    int mayor_valor = V[lim_inf];
    for (int i = lim_inf; i < lim_sup; i++){
        if (V[i] > mayor_valor)
            mayor_valor = V[i];
    }
    promise.set_value(mayor_valor);
    mtx.unlock();
}

int main() {
    srand(time(NULL));
    vector<thread> hilos;
    vector<promise<TipoNumerico>> promesas(NUM_HILOS);
    vector<TipoNumerico> Vector;
    vector<TipoNumerico> MayoresValores;
    TipoNumerico mayor_valor = 0;

    ///Llenar el vector de numeros aleatorios
    for (int i = 0; i < 10; ++i)
        Vector.push_back(rand()%51);

    ///Imprimir todos los valores del vector
    cout << "Valores del vector: ";
    for (int i = 0; i < 10; ++i)
        cout << Vector[i] << " ";
    ///Temporal para definir los limites de los hilos
    int temp = Vector.size()/NUM_HILOS;

    //cout << "temp = " << temp;

    for (int i = 0; i < NUM_HILOS; ++i) {
        future <TipoNumerico> future = promesas[i].get_future();
        hilos.emplace_back(busqueda_paralela<TipoNumerico>, ref(Vector), temp*i, temp*i + temp, move(promesas[i]));
        MayoresValores.push_back(future.get());
    }

    for (int i = 0; i < MayoresValores.size(); i++){
        if (MayoresValores[i] > mayor_valor)
            mayor_valor = MayoresValores[i];
    }

    for (int j = 0; j < NUM_HILOS; j++) {
        hilos[j].join();
    }

    cout << "\n\nEl mayor valor es: " << mayor_valor << endl;
    return 0;
}