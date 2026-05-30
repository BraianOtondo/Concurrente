#include <iostream>
#include <thread>
#include <mutex>

int vida = 100;
std::mutex mtx;

void daniar(int cant) {
    for (int i = 0; i < cant; i++) {
        int aux = vida;
      mtx.lock();
        
        aux = aux - 1;
        vida = aux;
      mtx.unlock();

        std::cout << "Vida: +1" << vida << std::endl;
    }
    std::cout << "VIDA ACTUAL AL DANIAR:---> " << vida << std::endl;
}
void curar(int cant) {
    for (int j = 0; j < cant; j++) {
        int aux = vida;
        mtx.lock();
        
        aux = aux + 1;
       vida = aux;
      mtx.unlock();
        
        std::cout << "Vida: -1 " << vida << std::endl;
    }
    std::cout << "VIDA ACTUAL AL CURAR:---> " << vida << std::endl;

}
using namespace std;

int main() {
    
    std::thread t1(daniar, 100);
   
	std::thread t2(curar, 100);  
	 
    t1.join();
    
	
	t2.join();
    


    std::cout << "VIDA ESPERADA 100: " << vida << std::endl;



}