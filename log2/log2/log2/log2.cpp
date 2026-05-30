#include <iostream>
#include <fstream>
#include <string>

// Nuestra función auxiliar para automatizar el log
void registrar_evento(int id_tarea, std::string evento) {
    std::ofstream archivo("actividad.log", std::ios::app);

    if (archivo.is_open()) {
        // Guardamos los datos con un formato prolijo
        archivo << "Tarea ID: " << id_tarea
            << " - Evento: " << evento << std::endl;

        archivo.close();
    }
}

int main() {
    // Simulamos que ocurren eventos en nuestro programa
    registrar_evento(101, "CREADO");
    registrar_evento(101, "EN_COLA");

    std::cout << "Eventos registrados en el archivo de texto." << std::endl;
    return 0;
}