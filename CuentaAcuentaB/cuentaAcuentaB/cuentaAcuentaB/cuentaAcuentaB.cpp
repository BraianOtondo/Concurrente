#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>

std::mutex mutex_cuentaA, mutex_cuentaB;
int usuarioA = 1000, usuarioB = 1000;

void tranferirAaB(int cantidad) {
	mutex_cuentaA.lock();
	usuarioA = usuarioA - cantidad;
	mutex_cuentaA.unlock();
	mutex_cuentaB.lock();	
	usuarioB = usuarioB + cantidad;
	mutex_cuentaB.unlock();	
}
void trandferirBaA(int cantidad) {
	mutex_cuentaA.lock();
	usuarioA = usuarioA + cantidad;
	mutex_cuentaA.unlock();
	mutex_cuentaB.lock();
	usuarioB = usuarioB - cantidad;
	mutex_cuentaB.unlock();
}

int main() {
	std::thread usuarioa(tranferirAaB, 100);
	std::thread usuariob(trandferirBaA, 200);
	usuarioa.join();
	usuariob.join();

	std::cout << "Usuario A: " << usuarioA << std::endl;
	std::cout << "Usuario B: " << usuarioB << std::endl;
	
	return 0;
}