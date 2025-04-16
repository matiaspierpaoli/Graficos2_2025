#include "Camera.h"
#include "../VectorUtils.h"

Camera::Camera(CameraMode mode, float distanceToTarget, float sensitivity)
    : mode(mode), distanceToTarget(distanceToTarget), sensitivity(sensitivity)
{
    position = glm::vec3(0.0f, 0.0f, 5.0f);
    lookTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {}


void Camera::FollowTarget(const Vector3& targetPos, float mouseDeltaX, float mouseDeltaY, bool rightClickHeld)
{
    if (rightClickHeld)
    {
        yaw += mouseDeltaX * 0.1f;   // Sensitivity
        pitch -= mouseDeltaY * 0.1f;
        pitch = glm::clamp(pitch, -89.0f, 89.0f); // Vertical Limits/Boundaries
    }

    glm::vec3 glmTarget = ToGLM(targetPos);

    float horizontalDist = distanceToTarget * cos(glm::radians(pitch));
    float verticalOffset = distanceToTarget * sin(glm::radians(pitch));

    float offsetX = horizontalDist * sin(glm::radians(yaw));
    float offsetZ = horizontalDist * cos(glm::radians(yaw));

    thirdPersonOffset = glm::vec3(-offsetX, verticalOffset, -offsetZ);

    // Additional Offset
    glm::vec3 lateralShift = glm::normalize(glm::cross(glm::vec3(0, 1, 0), thirdPersonOffset)) * 1.5f;
    thirdPersonOffset += lateralShift;

    position = glmTarget + thirdPersonOffset;
    lookTarget = glmTarget;
}

void Camera::Translate(float x, float y, float z)
{
    glm::vec3 translation(x, y, z);
    position += translation;
    lookTarget += translation;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, lookTarget, up);
}

glm::mat4 Camera::GetProjectionMatrix(Window* window) const
{
    float aspect = static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());
    return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

glm::vec3 Camera::GetThirdPersonOffset()
{
    return thirdPersonOffset;
}

glm::vec3 Camera::GetForward() const
{
    return glm::normalize(lookTarget - position);
}

void Camera::SetLookTarget(const glm::vec3& target) {
    lookTarget = target;
}

void Camera::SetPosition(const glm::vec3& newPos)
{
    position = newPos;
}

void Camera::UpdateFirstPersonView(float deltaX, float deltaY)
{
    yaw += deltaX * sensitivity;
    pitch -= deltaY * sensitivity;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    direction = glm::normalize(direction);

    lookTarget = position + direction;
}

void Camera::AdjustThirdPersonOffset(const glm::vec3& offsetChange)
{
    thirdPersonOffset += offsetChange;
}

glm::vec3 Camera::CalculateDirection() const {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(dir);
}

bool Camera::IsThirdPerson() const
{
    return mode == CameraMode::ThirdPerson;
}

void Camera::ToggleMode()
{
    if (mode == CameraMode::ThirdPerson)
        mode = CameraMode::FirstPerson;
    else
        mode = CameraMode::ThirdPerson;
}