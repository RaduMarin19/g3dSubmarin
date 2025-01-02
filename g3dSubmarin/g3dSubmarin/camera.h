//
// Created by mitza on 12/18/24.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "includes.h"

enum ECameraMovementType
{
	UNKNOWN,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class camera
{
private:
	// Default camera values
	const float zNEAR = 1.f;
	const float zFAR = 5000.f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float FOV = 45.0f;
	glm::vec3 startPosition;

	const float minCameraY = 1.f;
	const float maxCameraY = 125.f;

public:
	camera(int width, int height, const glm::vec3& position);

	void Set(int width, int height, const glm::vec3& position);

	void Reset(int width, int height);

	void Reshape(int windowWidth, int windowHeight);

	[[nodiscard]] const glm::mat4 GetViewMatrix() const;

	[[nodiscard]] const glm::vec3 GetPosition() const;

	[[nodiscard]] const glm::mat4 GetProjectionMatrix() const;

	void ProcessKeyboard(ECameraMovementType direction, double deltaTime);

	void MouseControl(GLFWwindow* window, double xpos, double ypos);

	void ProcessMouseScroll(GLFWwindow* window, double xoffset, double yOffset);

	double getDeltaTime();

	void updateDeltaTime(double dTime);

	void applyMovement();

	glm::mat4 GetModelPos();

	glm::vec3 GetForward();

	glm::vec3 GetRight();

	glm::vec3 GetUp();

	void SwitchCameraPerspective();

private:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

	void UpdateCameraVectors();

protected:
	const float cameraSpeedFactor = 250.0f;
	const float mouseSensitivity = 0.1f;
	double deltaTime;

	// Perspective properties
	float zNear;
	float zFar;
	float FoVy;
	int width;
	int height;
	bool isPerspective;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	glm::vec3 worldVelocity;
	const float friction = 0.01f;
	const float accelerationFactor = 0.05f;
	const float maxSpeed = 9.0f;

	bool isCameraFirstPerson = false;

	// Euler Angles
	float yaw;
	float pitch;

	bool bFirstMouseMove = true;
	float lastX = 0.f, lastY = 0.f;
};




#endif //CAMERA_H
