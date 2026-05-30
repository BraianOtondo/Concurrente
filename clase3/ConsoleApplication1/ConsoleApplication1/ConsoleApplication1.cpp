//CONCURRENTE 
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
int contador = 0;  // Variable compartida
int contadorConMutex = 0; // Variable compartida para la prueba con mutex   
std::mutex mtx;
//SIN MUTEX
void incrementar() {
    for (int i = 0; i < 100000; i++) {
        contador++;
    }
}
//CON MUTEX
void incrementarConMutex() {
    for (int i = 0; i < 100000; i++) {
		mtx.lock(); // Bloquea el mutex antes de acceder a la variable compartida   
        contadorConMutex++;
		mtx.unlock(); // Desbloquea el mutex después de modificar la variable compartida    
    }
}

int main() {
    std::thread t1(incrementar);
    std::thread t2(incrementar);

    t1.join();
    t2.join();
	std::cout << "Prueba sin mutex: "<< std::endl;

    std::cout << "Valor final (sin mutex):" << contador << std::endl;
	std::cout << "Diferencia entre ambos resultados: "<< 200000 - contador << std::endl;

	std::cout << "Prueba con mutex: " << std::endl; 
	std::thread t3(incrementarConMutex);
	std::thread t4(incrementarConMutex);
    t3.join();
    t4.join();
    std::cout << "Valor final (con mutex):" << contadorConMutex << std::endl;
	std::cout<<"Diferencia entre ambos resultados: "<< 200000 - contadorConMutex << std::endl;

    return 0;
}