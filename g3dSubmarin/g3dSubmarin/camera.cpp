//
// Created by mitza on 12/18/24.
//

#include "camera.h"

camera::camera(const int width, const int height, const glm::vec3 &position) {
        startPosition = position;
        Set(width, height, position);
}

void camera::Set(const int width, const int height, const glm::vec3 &position) {
        this->isPerspective = true;
        this->yaw = YAW;
        this->pitch = PITCH;

        this->FoVy = FOV;
        this->width = width;
        this->height = height;
        this->zNear = zNEAR;
        this->zFar = zFAR;

        this->worldUp = glm::vec3(0, 1, 0);
        this->position = position;

        this->worldVelocity = glm::vec3(0, 0, 0);

        lastX = width / 2.0f;
        lastY = height / 2.0f;
        bFirstMouseMove = true;

        UpdateCameraVectors();
}

void camera::Reset(const int width, const int height) {
        Set(width, height, startPosition);
}

void camera::Reshape(int windowWidth, int windowHeight) {
        width = windowWidth;
        height = windowHeight;

        // define the viewport transformation
        glViewport(0, 0, windowWidth, windowHeight);
}

const glm::mat4 camera::GetViewMatrix() const {
        // Returns the View Matrix
        return glm::lookAt(position, position + forward, up);
}

const glm::vec3 camera::GetPosition() const {
        return position;
}

const glm::mat4 camera::GetProjectionMatrix() const {
        glm::mat4 Proj = glm::mat4(1);
        if (isPerspective) {
                float aspectRatio = ((float)(width)) / height;
                Proj = glm::perspective(glm::radians(FoVy), aspectRatio, zNear, zFar);
        }
        else {
                float scaleFactor = 2000.f;
                Proj = glm::ortho<float>(
                        -width / scaleFactor, width / scaleFactor,
                        -height / scaleFactor, height / scaleFactor, -zFar, zFar);
        }
        return Proj;
}

void camera::ProcessKeyboard(ECameraMovementType direction, double deltaTime) {
        float velocity = (float)(cameraSpeedFactor * deltaTime);
        // switch (direction) {
        //         case ECameraMovementType::FORWARD:
        //                 position += forward * velocity;
        //         break;
        //         case ECameraMovementType::BACKWARD:
        //                 position -= forward * velocity;
        //         break;
        //         case ECameraMovementType::LEFT:
        //                 position -= right * velocity;
        //         break;
        //         case ECameraMovementType::RIGHT:
        //                 position += right * velocity;
        //         break;
        //         case ECameraMovementType::UP:
        //                 position += up * velocity;
        //         break;
        //         case ECameraMovementType::DOWN:
        //                 position -= up * velocity;
        //         break;
        // }
        switch (direction) {
                case ECameraMovementType::FORWARD:
                        worldVelocity.z -= accelerationFactor * velocity;
                break;
                case ECameraMovementType::BACKWARD:
                        worldVelocity.z += accelerationFactor * velocity;
                break;
                case ECameraMovementType::LEFT:
                        worldVelocity.x -= accelerationFactor * velocity;
                break;
                case ECameraMovementType::RIGHT:
                        worldVelocity.x +=  accelerationFactor * velocity;
                break;
                case ECameraMovementType::UP:
                        worldVelocity.y += accelerationFactor * velocity;
                break;
                case ECameraMovementType::DOWN:
                        worldVelocity.y -= accelerationFactor * velocity;
                break;
        }
        //clamp worldVelocity;
        worldVelocity = glm::clamp(worldVelocity, -3.0f, 3.0f);
}

void camera::MouseControl(GLFWwindow* window, double xpos, double ypos) {
        if (bFirstMouseMove) {
                lastX = xpos;
                lastY = ypos;
                bFirstMouseMove = false;
        }

        float xChange = xpos - lastX;
        float yChange = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        if (fabs(xChange) <= 1e-6 && fabs(yChange) <= 1e-6) {
                return;
        }
        xChange *= mouseSensitivity;
        yChange *= mouseSensitivity;

        ProcessMouseMovement(xChange, yChange);
}

void camera::ProcessMouseScroll(GLFWwindow* window, double xoffset, const double yOffset) {
        if (FoVy >= 1.0f && FoVy <= 90.0f) {
                FoVy -= yOffset;
        }
        if (FoVy <= 1.0f)
                FoVy = 1.0f;
        if (FoVy >= 90.0f)
                FoVy = 90.0f;
}

double camera::getDeltaTime() {
        return this->deltaTime;
}

void camera::updateDeltaTime(double dTime) {
        this->deltaTime = dTime;
}

void camera::applyMovement() {
        //apply the velocity vector to our position and then friction to the velocity vector;
        yaw += worldVelocity.x;
        pitch += worldVelocity.y;

        //clamp yaw and pitch so we dont get jerky movement
        yaw = glm::clamp(yaw, -120.0f, -60.0f);
        pitch = glm::clamp(pitch, -30.0f, 30.0f);

        position -= worldVelocity.z * forward;

        if(worldVelocity.x >= 0) worldVelocity.x -= friction;
        if(worldVelocity.x <= 0) worldVelocity.x += friction;

        if(worldVelocity.y >= 0) worldVelocity.y -= friction;
        if(worldVelocity.y <= 0) worldVelocity.y += friction;

        if(worldVelocity.z >= 0) worldVelocity.z -= friction;
        if(worldVelocity.z <= 0) worldVelocity.z += friction;

        UpdateCameraVectors();

}

glm::vec3 camera::GetModelPos() {
        glm::vec3 modelPos = GetPosition();
        if(!isCameraFirstPerson)
                modelPos -= glm::vec3(0, 3, 7.5);
        else modelPos -= glm::vec3(0, 0.6, -1);
        return modelPos;
}

glm::vec3 camera::GetForward() {
        return this->forward;
}

glm::vec3 camera::GetRight() {
        return this->right;
}

glm::vec3 camera::GetUp() {
        return this->up;
}

void camera::SwitchCameraPerspective() {
        isCameraFirstPerson = !isCameraFirstPerson;
}

void camera::ProcessMouseMovement(const float xOffset, const float yOffset, const bool constrainPitch) {
        return;
        yaw += xOffset;
        pitch += yOffset;

        //std::cout << "yaw = " << yaw << std::endl;
        //std::cout << "pitch = " << pitch << std::endl;

        // Avem grijã sã nu ne dãm peste cap
        if (constrainPitch) {
                if (pitch > 89.0f)
                        pitch = 89.0f;
                if (pitch < -89.0f)
                        pitch = -89.0f;
        }

        // Se modificã vectorii camerei pe baza unghiurilor Euler
        UpdateCameraVectors();
}

void camera::UpdateCameraVectors() {
        // Calculate the new forward vector
        this->forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->forward.y = sin(glm::radians(pitch));
        this->forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->forward = glm::normalize(this->forward);
        // Also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(forward, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, forward));
}

