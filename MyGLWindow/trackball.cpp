#include "trackball.h"
#include <glm/gtx/vector_angle.hpp>


// Default constructor, no rotation. No window registered
Trackball::Trackball() {
    // Identity quaternions (no rotation)
    m_baseRotation = glm::normalize(glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
    m_newRotation = glm::normalize(glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
    //State variable to keep track of mouse dragging event
    m_drag = false;
    m_startDrag = glm::vec2(0.0f, 0.0f);
    m_windowSize = glm::ivec2(0, 0);
    //Scale parameter for curve
    m_radius = 0.8f;
}

// Prevent auto-generation of destructor
Trackball::~Trackball() {

}

// Full Constructor. Initial window size and curve scale
Trackball::Trackball(const glm::ivec2& windowSize, float radius) : Trackball() {
    setWindowSize(windowSize);
    m_radius = glm::clamp(radius, 0.1f, 1.0f);
}

// Second Full Constructor. Initial window size and curve scale
Trackball::Trackball(int width, int height, float radius) : Trackball() {
    setWindowSize(glm::ivec2(width, height));
    m_radius = glm::clamp(radius, 0.1f, 1.0f);
}
// Reeset to identity rotation
void Trackball::resetRotation() {
    m_baseRotation = glm::normalize(glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
    m_newRotation = glm::normalize(glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
    m_drag = false;
}
// Register the start of a dragging event
void Trackball::startDrag(const glm::vec2& mousePos) {
    m_drag = true;
    m_startDrag = mousePos;
}

void Trackball::drag(const glm::vec2& mousePos) {
    using glm::vec2;
    using glm::vec3;

    if (m_drag) {
        //Transform mouse coordinates from pixels to world units
        vec2 mouse_current_in_pixels = mousePos;
        vec2 window_center = 0.5f * vec2(m_windowSize);
        vec2 scale_factors = vec2(2.0f / m_windowSize.x, -2.0f / m_windowSize.y);
        vec2 mouse_current_in_world = scale_factors * (mouse_current_in_pixels - window_center);
        vec2 mouse_start_in_world = scale_factors * (m_startDrag - window_center);
        //Now we have mouse_current and mouse_start in world coordinates
        //Use the algorithm from: https://www.khronos.org/opengl/wiki/Object_Mouse_Trackball
        vec3 v_1 = glm::normalize(vec3(mouse_current_in_world, projectionOnCurve(mouse_current_in_world)));
        vec3 v_2 = glm::normalize(vec3(mouse_start_in_world, projectionOnCurve(mouse_start_in_world)));
        vec3 axis = glm::cross(v_1, v_2);
        float angle = glm::angle(v_1, v_2);
        m_newRotation = glm::normalize(glm::quat(glm::cos(0.5f * angle), glm::sin(0.5f * angle) * axis));
    }
}
// Project (calculate z from (x, y)) into the hyperbolic sheet described in
// https://www.khronos.org/opengl/wiki/Object_Mouse_Trackball#Sit_and_spin
float Trackball::projectionOnCurve(const glm::vec2& projected) {
    float z = 0.0f;
    //m_radius is the distance where the curves changed in terms of the window size
    if (glm::length2(projected) <= (0.5f * m_radius * m_radius)) {
        //Inside the sphere
        z = glm::sqrt(m_radius * m_radius - glm::length2(projected));
    } else {
        //Outside of the sphere using hyperbolic sheet
        z = (0.5f * m_radius * m_radius) / glm::length(projected);
    }
    return z;
}
// Finish the manipulation
void Trackball::endDrag() {
    m_drag = false;
    /* Calculate the accumulated rotation: base rotation plus new one */
    m_baseRotation = glm::normalize(m_newRotation * m_baseRotation);
    /* Reset new rotation to identity */
    m_newRotation = glm::normalize(glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f)));
}
// Register a new size of the window
void Trackball::setWindowSize(const glm::ivec2& windowSize) {
    m_windowSize = windowSize;
}
// Register a new size of the window
void Trackball::setWindowSize(int width, int height) {
    setWindowSize(glm::ivec2(width, height));
}
// get current size of the window
glm::ivec2 Trackball::getWindowSize() const {
    return m_windowSize;
}
// Most important method in the class: get the current rotation as a 4x4 matrix
// so we can use it to construct a View matrix
glm::mat4 Trackball::getRotation() const {
    return glm::mat4_cast(m_newRotation * m_baseRotation);
}
