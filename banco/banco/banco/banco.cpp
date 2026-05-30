
#include <iostream>
#include <mutex>
#include <thread>
std::mutex ing;
int cuenta[2] = { 100, 100 };

void transferir(int origen, int destino, int monto) {
	ing.lock();
	if (monto <= cuenta[origen]) {
		cuenta[origen] -= monto;	
		cuenta[destino] += monto;
		
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	ing.unlock();
}
void ingresar(int cuenta_destino, int monto) {

	if (monto >= 0) {
		ing.lock();
		cuenta[cuenta_destino] += monto;
		ing.unlock();
	}
}
int main(){

	std::thread t1(transferir, 0, 1, 50);
	std::thread t2(ingresar, 1, 50);
	std::thread t3(transferir, 0, 1, 50);
	std::thread t4(transferir, 0, 1, 30);
	std::thread t5(ingresar, 0, 200);
	std::thread t6(transferir, 1, 0, 30);
	std::thread t7(ingresar, 0, 30);

	t1.join();
	t2.join();	
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();

	std::cout << "Cuenta 0: " << cuenta[0] << std::endl;
	std::cout << "Cuenta 1: " << cuenta[1] << std::endl;


		
}

