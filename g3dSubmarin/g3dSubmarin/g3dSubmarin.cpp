#ifndef __linux__
#include <Windows.h>
#endif
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
	 //glEnable(GL_CULL_FACE);
     //glCullFace(GL_FRONT_AND_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);


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
	shader shadowShader = shader("../shadowShader.vs", "../shadowShader.fs");
	shader shadowDepthShader = shader("../shadowDepthShader.vs", "../shadowDepthShader.fs");
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

	glm::vec3 sunPosition = {10.f, 350.f, 10.f};
	glm::vec3 sunColor = {0.8f, 0.8f, 0.8f};

	//shadows
	const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowShader.use();
	shadowShader.setInt("texture_diffuse1", 0);
	shadowShader.setInt("shadowMap", 1);

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



    	basicSunShader.use();
    	basicSunShader.SetVec3("lightColor", sunColor);
    	basicSunShader.SetVec3("lightPos", sunPosition);
    	basicSunShader.SetVec3("viewPos", pCamera->GetPosition());
    	basicSunShader.setInt("texture_diffuse1", 0);

    	basicSunShader.setMat4("projection", pCamera->GetProjectionMatrix());
    	basicSunShader.setMat4("view", pCamera->GetViewMatrix());

    	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), sunPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f));
		drawableObject tmp = {&sun, modelMatrix, basicSunShader};
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
    				if(obj.Id == 2) {
    					modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
    				}
    				else if(obj.Id == 1) {
    					modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), obj.rotation);
    					modelMatrix = glm::scale(modelMatrix, obj.scale * 5.f);
    				} else {
    					modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), obj.rotation);
    					modelMatrix = glm::scale(modelMatrix, obj.Id == 2 ? obj.scale / 6.f : obj.scale);
    				}
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

    	// 1. render depth of scene to texture (from light's perspective)
    	glm::mat4 lightProjection, lightView;
    	glm::mat4 lightSpaceMatrix;
    	float near_plane = 1.0f, far_plane = 750.5f;
    	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    	lightView = glm::lookAt(sunPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    	lightSpaceMatrix = lightProjection * lightView;

    	// render scene from light's point of view
    	shadowDepthShader.use();
    	shadowDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    	glClear(GL_DEPTH_BUFFER_BIT);
    	glEnable(GL_CULL_FACE);
    	glCullFace(GL_FRONT);

    	for(std::map<float, drawableObject>::reverse_iterator rIt = sortedObjects.rbegin(); rIt != sortedObjects.rend(); rIt++) {

    		shadowDepthShader.setMat4("model", rIt->second.modelMatrix);
    		//rIt->second.modelShader.setMat4("model", rIt->second.modelMatrix);
    		rIt->second.mod->Draw(shadowDepthShader);
    	}

    	glCullFace(GL_BACK);
    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    	// reset viewport
    	glViewport(0, 0, 1280, 720);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	shadowShader.use();
    	glm::mat4 projection = pCamera->GetProjectionMatrix();
    	glm::mat4 view = pCamera->GetViewMatrix();
    	shadowShader.setMat4("projection", projection);
    	shadowShader.setMat4("view", view);
    	// set light uniforms
    	shadowShader.SetVec3("viewPos", pCamera->GetPosition());
    	shadowShader.SetVec3("lightPos", sunPosition);
    	shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    	glActiveTexture(GL_TEXTURE1);
    	glBindTexture(GL_TEXTURE_2D, depthMap);
    	glDisable(GL_CULL_FACE);

    	for(std::map<float, drawableObject>::reverse_iterator rIt = sortedObjects.rbegin(); rIt != sortedObjects.rend(); rIt++) {
    		shadowShader.setMat4("model", rIt->second.modelMatrix);
    		//rIt->second.modelShader.setMat4("model", rIt->second.modelMatrix);
    		rIt->second.mod->Draw(shadowShader);
    	}

    	// for(auto & [k, v] : sortedObjects) {
    	// 	v.modelShader.setMat4("model", v.modelMatrix);
    	// 	v.mod->Draw(v.modelShader);
    	// }

    	//std::cout << pCamera->GetPosition().y << std::endl;

    	basicShader.use();
    	basicShader.SetVec3("lightColor", sunColor);
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
    	sunPosition.x = world.getChunks()[1][1].x;
    	sunPosition.z = world.getChunks()[1][1].z;


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;

}
