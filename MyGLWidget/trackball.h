#ifndef TRACKBALL_H
#define TRACKBALL_H

#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

//! A class that let you control a trackball camera
/*!
  This class represents a trackball camera. It is created with only GLM as
  dependence, (so it can be easily pulled out form the template) and will
  return you a rotation matrix that you can apply between the Model and View
  transformation for representing the camera's rotation.
  It will require you that provide it with an interface with the mouse and
  window system. So you need to tell it every time the window changes size,
  when you start dragging (in which position), when you are dragging (also the
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

      Optionally, it let you specify the radius of the trackball in terms of
      the window proportion
    */
    Trackball(const glm::ivec2& windowSize, float radius = 0.8f);
    //! Constructor of the trackball
    /*!
      This constructor needs the window size.

      Optionally, it let you specify the radius of the trackball in terms of
      the window proportion
    */
    Trackball(int width, int height, float radius = 0.8f);
    //! Resets the trackball to the identity rotation
    void resetRotation();
    //! Provide the starting point of a dragging in pixel coordinates
    void startDrag(const glm::vec2& mousePos);
    //! Provide the current point of a dragging in pixel coordinates
    void drag(const glm::vec2& mousePos);
    //! Let the trackball know that the dragging is finished
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
    //! Current rotation before any drag
    glm::quat m_baseRotation;
    //! Temporal delta of rotation during the dragging
    glm::quat m_newRotation;
    //! Project the click ray into the surface we use as ball
    float projectionOnCurve(const glm::vec2& projected);
    //! Relative size of the surface to with respect to the widget size
    float m_radius;
    //! Size of the container widget (or window)
    glm::ivec2 m_windowSize;
    //For keep track of mouse dragging event
    //! Are we in the dragging event
    bool m_drag;
    //! Starting position of the dragging event
    glm::vec2 m_startDrag;
};

#endif // TRACKBALL_H
