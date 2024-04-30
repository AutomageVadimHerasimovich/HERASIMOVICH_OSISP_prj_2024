#include "clipboard_handler.h"

int main() {
    createFile("input.txt");
    createFile("output.txt");
    //запустить клиента
    //проврить есть ли подключение
    //если нет, запустить сервер
    //runServerProgram();
    checkChangesInCB();
    return 0;
}
