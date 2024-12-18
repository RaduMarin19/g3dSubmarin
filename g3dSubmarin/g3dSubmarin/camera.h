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
	const float zNEAR = 0.1f;
	const float zFAR = 500.f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float FOV = 45.0f;
	glm::vec3 startPosition;

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

	// Euler Angles
	float yaw;
	float pitch;

	bool bFirstMouseMove = true;
	float lastX = 0.f, lastY = 0.f;
};




#endif //CAMERA_H
