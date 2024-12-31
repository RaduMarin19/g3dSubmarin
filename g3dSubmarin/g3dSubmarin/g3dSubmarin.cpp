
#include "includes.h"
#include "model.h"
#include "shader.h"
#include "camera.h"


camera *pCamera = new camera(1280, 720, glm::vec3(0.0, 0.0, 0.0));;
int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(1280, 720, "SMMG3D-Submarin", NULL, NULL);
    if (window == NULL) {
        const char* errDescription;
        int code = glfwGetError(&errDescription);
        fprintf(stderr, "Error: %s\n", errDescription);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, nullptr);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		pCamera->MouseControl(window, xpos, ypos);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yOffset) {
		pCamera->ProcessMouseScroll(window, xoffset, yOffset);
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_A && action == GLFW_PRESS) {

	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, pCamera->getDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, pCamera->getDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, pCamera->getDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, pCamera->getDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, pCamera->getDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, pCamera->getDeltaTime());

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);
	}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			pCamera->SwitchCameraPerspective();
		}

	});

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewInit();

    double lastFrame = 0.0;

#if defined linux
    shader basicShader = shader("../basicShader.vs", "../basicShader.fs");
    model basicModel = model("../Models/Submarine/submarine.obj", true);
	model basicGround = model("../Models/Grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj", true);
#else
	shader basicShader = shader("basicShader.vs", "basicShader.fs");
	model basicModel = model("..\\Models\\test\\FlyingCube.obj", true);
#endif

    while (!glfwWindowShouldClose(window)) {

        double currentFrame = glfwGetTime();
    	pCamera->updateDeltaTime(currentFrame - lastFrame);
        lastFrame = currentFrame;

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	basicShader.use();
    	basicShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    	basicShader.SetVec3("lightPos", {0.2f, 1.f, 0.3f});
    	basicShader.SetVec3("viewPos", pCamera->GetPosition());

    	basicShader.setMat4("projection", pCamera->GetProjectionMatrix());
    	basicShader.setMat4("view", pCamera->GetViewMatrix());

    	//render a basic floor
    	basicShader.SetVec3("objectColor", 0.2f, 1.0f, 0.31f);
    	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), {0.f, -10.f, 0.f});
    	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    	modelMatrix = glm::scale(modelMatrix, glm::vec3(5.f));
    	basicShader.setMat4("model", modelMatrix);
    	basicGround.Draw(basicShader);

    	// render the model
    	basicShader.SetVec3("objectColor", 1.f, 1.0f, 0.31f);
    	glm::mat4 model = pCamera->GetModelPos();

    	model = glm::scale(model, glm::vec3(0.8f));
    	basicShader.setMat4("model", model);
    	basicModel.Draw(basicShader);

    	pCamera->applyMovement();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;

}
