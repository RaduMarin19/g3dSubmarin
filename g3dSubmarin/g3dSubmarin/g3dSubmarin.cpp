#include <Windows.h>
#include <locale>
#include <codecvt>
#include <map>

#include "includes.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "worldChunks.h"


camera *pCamera = new camera(1280, 720, glm::vec3(0.0, 100.0, 0.0));;
int main()
{

	srand(time(NULL));

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

     glFrontFace(GL_CCW);
	// glEnable(GL_CULL_FACE);
 //    glCullFace(GL_FRONT_AND_BACK);
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);


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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewInit();

    double lastFrame = 0.0;

	worldChunks world(1.f, -1.f, 1.f);

#ifdef __linux__
    shader basicShader = shader("../basicShader.vs", "../basicShader.fs");
	shader basicTexShader = shader("../basicTextureShader.vs", "../basicTextureShader.fs");
	shader basicWaterShader = shader("../basicWaterShader.vs",  "../basicWaterShader.fs");
	shader basicSunShader = shader("../basicSunShader.vs",  "../basicSunShader.fs");
    model basicModel = model("../Models/Submarine/submarine.obj", true);
	model basicGround = model("../Models/sand/sand.obj", true);
	model water = model("../Models/water/water.obj", true);
	model sun = model("../Models/sun/13913_Sun_v2_l3.obj", false);
	model fish1 = model("../Models/fish1/fish.obj", true);
	model fish2 = model("../Models/fish2/13009_Coral_Beauty_Angelfish_v1_l3.obj", true);
	model coral1 = model("../Models/coral1/21487_Gorgonian_Soft_Coral_v1.obj", true);
	model coral2 = model("../Models/coral2/10010_Coral_v1_L3.obj", true);
	model coral3 = model("../Models/coral3/21488_Tree_Coral_v2_NEW.obj", true);
#else
    wchar_t buffer[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, buffer);

    std::wstring executablePath(buffer);
    std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string currentPath = converter.to_bytes(wscurrentPath);

	shader basicShader = shader("basicShader.vs", "basicShader.fs");
	shader basicTexShader = shader("basicTextureShader.vs", "basicTextureShader.fs");
	shader basicWaterShader = shader("basicWaterShader.vs",  "basicWaterShader.fs");
	shader basicSunShader = shader("basicSunShader.vs",  "basicSunShader.fs");
	model basicModel = model(currentPath + "\\Models\\Submarine\\submarine.obj", true);
	model basicGround = model(currentPath + "\\Models\\sand\\sand.obj", true);
	model water = model(currentPath + "\\Models\\water\\water.obj", true);
	model sun = model(currentPath + "\\Models\\sun\\13913_Sun_v2_l3.obj", true);
	model fish1 = model(currentPath + "\\Models\\fish1\\fish.obj", true);
	model fish2 = model(currentPath + "\\Models\\fish2\\13009_Coral_Beauty_Angelfish_v1_l3.obj", true);
	model coral1 = model(currentPath + "\\Models\\coral1\\21487_Gorgonian_Soft_Coral_v1.obj", true);
	model coral2 = model(currentPath + "\\Models\\coral2\\10010_Coral_v1_L3.obj", true);
	model coral3 = model(currentPath + "\\Models\\coral3\\21488_Tree_Coral_v2_NEW.obj", true);
#endif

	glm::vec3 sunPosition = {0.f, 350.f, 0.f};

    while (!glfwWindowShouldClose(window)) {

    	struct drawableObject {
    		model *mod = nullptr;
    		glm::mat4 modelMatrix;
    		shader modelShader;
    	};

    	std::multimap<float, drawableObject> sortedObjects;

        double currentFrame = glfwGetTime();
    	pCamera->updateDeltaTime(currentFrame - lastFrame);
        lastFrame = currentFrame;

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 0.0f);



    	basicTexShader.use();
    	basicTexShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    	basicTexShader.SetVec3("lightPos", sunPosition);
    	basicTexShader.SetVec3("viewPos", pCamera->GetPosition());
    	basicTexShader.setInt("texture_diffuse1", 0);

    	basicTexShader.setMat4("projection", pCamera->GetProjectionMatrix());
    	basicTexShader.setMat4("view", pCamera->GetViewMatrix());

    	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), sunPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f));
		drawableObject tmp = {&sun, modelMatrix, basicTexShader};
		float dist = glm::distance(pCamera->GetPosition(), sunPosition);
		sortedObjects.emplace(dist, tmp);


    	//render our chunks
    	for(const auto & chunkRow : world.getChunks()) {
    		for(const auto & chunk : chunkRow) {
    			basicTexShader.SetVec3("objectColor", chunk.r, chunk.g, chunk.b);
    			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), {chunk.x, chunk.y, chunk.z});
    			modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    			//modelMatrix = glm::rotate(modelMatrix, glm::radians(180.f), glm::vec3(1.f, 1.f, 1.f));
    			modelMatrix = glm::scale(modelMatrix, glm::vec3(5.5f));
    			//basicTexShader.setMat4("model", modelMatrix);
    			//basicGround.Draw(basicTexShader);
    			drawableObject tmp = {&basicGround, modelMatrix, basicTexShader};
    			float dist = glm::distance(pCamera->GetPosition(), glm::vec3(chunk.x, chunk.y, chunk.z));
    			sortedObjects.emplace(dist, tmp);

    			modelMatrix = glm::translate(glm::mat4(1.0), {chunk.x, chunk.y + 170.f, chunk.z});
    			modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    			modelMatrix = glm::scale(modelMatrix, glm::vec3(5.5f));
    			tmp = {&water, modelMatrix, basicWaterShader};
    			dist = glm::distance(pCamera->GetPosition(), glm::vec3(chunk.x, chunk.y + 170.f, chunk.z));
    			sortedObjects.emplace(dist, tmp);

    			for(const auto & obj : chunk.objects) {
    				modelMatrix = glm::translate(glm::mat4(1.0), obj.position);
    				//modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), obj.rotation);
    				if(obj.Id == 2) {
    					modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    				}
    				if(obj.Id == 1) {
    					modelMatrix = glm::scale(modelMatrix, obj.scale * 5.f);
    				} else modelMatrix = glm::scale(modelMatrix, obj.Id == 2 ? obj.scale / 6.f : obj.scale);
    				model *accesory=nullptr;
    				switch(obj.Id) {
    					case 0:
    						accesory = &fish1;
    					break;
    					case 1:
    						accesory = &fish2;
    					break;
    					case 2:
    						accesory = &coral2;
    					break;
    					case 3:
    						accesory = &coral1;
    					break;
    					case 4:
    						accesory = &coral3;
    					break;
    				}
    				tmp = {accesory, modelMatrix, basicTexShader};
    				dist = glm::distance(pCamera->GetPosition(), obj.position);
    				sortedObjects.emplace(dist, tmp);
    			}
    		}
    	}

    	for(std::map<float, drawableObject>::reverse_iterator rIt = sortedObjects.rbegin(); rIt != sortedObjects.rend(); rIt++) {
    		rIt->second.modelShader.setMat4("model", rIt->second.modelMatrix);
    		rIt->second.mod->Draw(rIt->second.modelShader);
    	}

    	// for(auto & [k, v] : sortedObjects) {
    	// 	v.modelShader.setMat4("model", v.modelMatrix);
    	// 	v.mod->Draw(v.modelShader);
    	// }

    	//std::cout << pCamera->GetPosition().y << std::endl;

    	basicShader.use();
    	basicShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    	basicShader.SetVec3("lightPos", sunPosition);
    	basicShader.SetVec3("viewPos", pCamera->GetPosition());

    	basicShader.setMat4("projection", pCamera->GetProjectionMatrix());
    	basicShader.setMat4("view", pCamera->GetViewMatrix());

    	// render the model
    	basicShader.SetVec3("objectColor", 1.f, 1.0f, 0.31f);
    	glm::mat4 model = pCamera->GetModelPos();

    	model = glm::scale(model, glm::vec3(0.8f));
    	basicShader.setMat4("model", model);
    	basicModel.Draw(basicShader);

    	pCamera->applyMovement();

    	world.validateChunks(pCamera->GetPosition());


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;

}
