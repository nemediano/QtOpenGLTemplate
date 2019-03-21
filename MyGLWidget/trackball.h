#ifndef TRACKBALL_H
#define TRACKBALL_H

#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

//! A class that let you controll a trackball camera
/*!
  This class represents a trackball camera. It is created with only GLM as
  dependece, and will return you a rotation matrix that you can apply between
  the Model and View transformation for representing the camera.

  It will require you that provide it with an interface with the mouse and
  window system. So you need to tell it everytime the window changes size,
  when you start dragging (in which position), when you are draging (also the
  position) and when you stop dragging.
*/
class Trackball {
public:
    //! Constructor of the trackball
    /*!
       The \class Trackball is not usable until a window size is also specified
    */
    Trackball();
    ~Trackball();
    //! Constructor of the trackball
    /*!
      This constructor needs the window size.

      Optionally, it let you specify the radious of the trackball in terms of
      the window proportion
    */
    Trackball(const glm::ivec2& windowSize, float radius = 0.8f);
    //! Constructor of the trackball
    /*!
      This constructor needs the window size.

      Optionally, it let you specify the radious of the trackball in terms of
      the window proportion
    */
    Trackball(int width, int height, float radius = 0.8f);
    //! Resets the trackball to the identity rotation
    void resetRotation();
    //! Provide the starting point of a drag in pixel coordinates
    void startDrag(const glm::vec2& mousePos);
    //! Provide the current point of a drag in pixel coordinates
    void drag(const glm::vec2& mousePos);
    //! Let the trackball know the drag is finished
    void endDrag();
    //! Update the window size for the \class Trackball
    void setWindowSize(const glm::ivec2& windowSize);
    //! Update the window size for the \class Trackball
    void setWindowSize(int width, int height);
    //! Get the register window size for this \class Trackball
    glm::ivec2 getWindowSize() const;
    //! Get the rotation as a 4x4 matrix
    glm::mat4 getRotation() const;

protected:
    //Quaternion trackball related
    glm::quat m_baseRotation;
    glm::quat m_newRotation;
    float projectionOnCurve(const glm::vec2& projected);
    float m_radius;
    //For mouse dragging
    bool m_drag;
    glm::vec2 m_startDrag;
    glm::ivec2 m_windowSize;
};

#endif // TRACKBALL_H
