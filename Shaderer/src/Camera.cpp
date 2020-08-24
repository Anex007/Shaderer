#include "Camera.h"

// DEBUG: REMOVE ASAP
#include <iostream>


#define TORADIANS(deg) ((deg)*M_PI/180.0f)

Camera::Camera (float yaw, float pitch, float cameraSpeed, float zoomSpeed, float mouseSpeed)
    : m_Yaw(yaw), m_Pitch(pitch), m_CameraSpeed(cameraSpeed), m_ZoomSpeed(zoomSpeed), m_MouseSpeed(mouseSpeed)
{
    Position = Vec3f(0.0f, 0.0f, -1.0f);
    Front = Vec3f(0.0f, 0.0f, 1.0f);
    Up = Vec3f(0.0f, 1.0f, 0.0f);
    Right = Vec3f(1.0f, 0.0f, 0.0f);
    Zoom = 1.0f;
}

void Camera::MoveCamera(MoveDir dir, float deltaTime)
{

    float velocity = m_CameraSpeed * deltaTime;
    switch (dir) {
        case MoveDir::Forward:
            Position -= Front * velocity;
            break;
        case MoveDir::Backward:
            Position += Front * velocity;
            break;
        case MoveDir::Right:
            Position += Right * velocity;
            break;
        case MoveDir::Left:
            Position -= Right * velocity;
            break;
        default:
            return;
    }
}

void Camera::MouseMovement(float xoff, float yoff)
{
    xoff *= m_MouseSpeed;
    yoff *= m_MouseSpeed;

    m_Yaw += xoff;
    m_Pitch += yoff;

    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    else if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    Front.x = cosf(TORADIANS(m_Yaw)) * cosf(TORADIANS(m_Pitch));
    Front.y = sinf(TORADIANS(m_Pitch));
    Front.z = sinf(TORADIANS(m_Yaw)) * cosf(TORADIANS(m_Pitch));
    Front.Normalize();

    Right.Cross(Front, Vec3f(0.0f, 1.0f, 0.0f)); // Cross product with world up to make right
    Up.Cross(Right, Front);
}

void Camera::OnMouseScroll(float yoff)
{
    Zoom -= yoff * m_ZoomSpeed;
    if (Zoom < 0.0f) {
        Zoom = 0.00001f;
    } else if (Zoom > 10.0f) {
        Zoom = 10.0f;
    }
#ifdef DEBUG
    //std::cerr << "Zoom :" << Zoom << std::endl;
#endif
}
