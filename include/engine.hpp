#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cstdlib>

extern GLFWwindow *window;

void init(int width, int height, const char *name);

void update();
void render();
void clean();
