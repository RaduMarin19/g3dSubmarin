//
// Created by mitza on 10/15/24.
//

#ifndef INCLUDES_H
#define INCLUDES_H


#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <memory.h>
#include <fstream>
#include <sstream>
#include <random>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#if defined linux
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#else
#include "GLM.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"
#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")
#endif

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#endif //INCLUDES_H