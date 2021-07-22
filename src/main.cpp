#include "../include/engine.hpp"

int main(int argc, char *argv[]){
    init(1280, 720, "OpenGL");

    while (!glfwWindowShouldClose(window)){
        update();
        render();
    }

    clean();
    return 0;
}
