#pragma once

#include "MathVector.h"

enum class MoveDir
{
    Forward,
    Backward,
    Right,
    Left,
};

class Camera
{
public:
    Camera (float yaw = -90.0f, float pitch = 0.0f,
            float cameraSpeed = 0.3f, float zoomSpeed = 0.02f, float mouseSpeed = 0.5f);

    void MouseMovement(float xoff, float yoff);
    void MoveCamera(MoveDir dir, float deltaTime);
    void OnMouseScroll(float yoff);

    ~Camera()
    {}

public:
    Vec3f Position;
    Vec3f Front;
    Vec3f Up;
    Vec3f Right;
    float Zoom;

private:
    float m_Yaw;
    float m_Pitch;
    float m_CameraSpeed;
    float m_ZoomSpeed;
    float m_MouseSpeed;

};
