#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
std::mutex toner, papel,inyectores;
void proceso_impresion() {

	papel.lock();
    std::cout << "[IMP] Papel detectado. Verificando tóner...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
	papel.unlock();

	toner.lock();   
    std::cout << "[IMP] Tóner OK. Imprimiendo ticket...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
	toner.unlock();

    std::cout << "[IMP] Ticket impreso...\n";

}

void proceso_mantenimiento() {
	papel.lock();
	
	
    std::cout << "[MANT] Limpiando inyectores. Verificando rodillos...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    papel.unlock();
    toner.lock();


    std::cout << "[MANT] Rodillos limpios. Mantenimiento finalizado.\n";
    toner.unlock();
}

int main() {

   while (true) {

        std::thread t1(proceso_impresion);

        std::thread t2(proceso_mantenimiento);
		t1.join();
        t2.join();
		

    }

}