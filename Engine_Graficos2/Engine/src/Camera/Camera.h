#pragma once

#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../Window/Window.h"
#include "Vector3.h"
#include "../Entity/Entity.h"

enum class  CameraMode {
    FirstPerson,
    ThirdPerson
};

class GraficosEngine_API Camera {
private:
    CameraMode mode;
    float distanceToTarget;
    float sensitivity;

    glm::vec3 position;
    glm::vec3 lookTarget;
    glm::vec3 up;

    glm::vec3 thirdPersonOffset = glm::vec3(0.0f, 2.0f, 5.0f);

    float yaw = 90.0f;
    float pitch = 0.0f;

public:
    Camera(CameraMode mode, float distanceToTarget, float sensitivity);
    ~Camera();

    void FollowTarget(const Vector3& targetPos, float mouseDeltaX, float mouseDeltaY, bool rightClickHeld);
    void Translate(float x, float y, float z);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(class Window* window) const;
    glm::vec3 GetThirdPersonOffset();
    glm::vec3 GetForward() const;
    glm::vec3 GetLookTarget() const { return lookTarget; }
    inline float GetYaw() const { return yaw; }
    inline float GetPitch() const { return pitch; }
    inline CameraMode GetMode() const { return mode; }
    inline glm::vec3 GetPosition() const { return position; }

    inline void SetYaw(float value) { yaw = value; }
    inline void SetPitch(float value) { pitch = value; }
    void SetLookTarget(const glm::vec3& target);
    void SetPosition(const glm::vec3& newPos);

    void UpdateFirstPersonView(float deltaX, float deltaY);
    void AdjustThirdPersonOffset(const glm::vec3& offsetChange);
    glm::vec3 CalculateDirection() const;

    bool IsThirdPerson() const;
    void ToggleMode();
};
