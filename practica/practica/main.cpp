#include <iostream>
#include <string.h>
#include <thread>
#include <chrono>
using namespace std;

void generarReporte(string nombreTarea, int ms){
    for(int i=1;i<=5;i++){
        cout<<nombreTarea<<" - Iteracion "<<i<<endl;
        this_thread::sleep_for(milliseconds(ms));

    }

}
int main(){

    //<cout << "Hello world!" << endl;


generarReporte("Generar Reporte",500);

    return 0;
}
