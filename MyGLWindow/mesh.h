#ifndef MESH_H
#define MESH_H
#include <QOpenGLBuffer>
#include <QString>
#include <glm/glm.hpp>

#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//! A struct that represents a simple \struct Vertex.
/*!
  It contains a simple vertex with interleaved data of the most common
  attributes for rendering. A position, a normal vector and texture coordinates.
  The \class Mesh a \class Model classes will use it as interface with the user
  classes
*/
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textCoords;
};
//! A struct that represents a \struct Triangle.
/*!
  It is used to as an interface. If you create a vector of this strcut you
  can use it to construct a mesh object. See the load from triangles
*/
struct Triangle {
    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec3 p2;
};
//! A class  that can be used to load a \class Mesh from file and do basic operations with it
/*!
  This class only deals with simple model that contain just a single mesh. If you
  want to load a scene that contains several meshes. Then the \class Model class
  should be used.

  This class only pulls the data form a file and provide a way to interact with
  it and pass it to another class that can render it. In this sense, this calss
  has nothing to do with GPU or OpenGL rendering

  Given said the above, you can use it as a helper for a user class that
  actually does render. And Mesh is designed to facilitate all the data in the
  a format that is appropaita so the user class can pass it to the GPU and then
  render it using OpenGL
*/
class Mesh {
protected:
    bool mHasNormals;
    bool mHasTexture;
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    glm::vec3 mUpperCorner;
    glm::vec3 mLowerCorner;
    std::string mDiffuseText;
    void updateBoundingBox();
    void addTexture(const aiMaterial* mat);
public:
    //! Simple constructor that does nothing.
    /*!
      It can be used if you want to use the class to create ans save a mesh
      that you create by giving triangles.
    */
    Mesh();
    //! Main constructor for the class.
    /*!
      Create a mesh loading it from a file. Then you can query the object to
      give you the date you will need to render it.
    */
    explicit Mesh(const QString& fileName);
    ~Mesh();
    //! Erases the data and then load a new \class Mesh form the file.
    bool loadFromFile(const QString& fileName);
    //! Queries if this Mesh has no data.
    bool empthy() const;
    //! Queries if the Vertices of the Mesh has texture coordintes data
    bool hasTexture() const;
    //! Queries if the Vertices of the Mesh has normal vectors
    bool hasNormals() const;
    //! Release the memmory on this Mesh
    void clear();
    //! Transform all the vertices of the mesh by T
    void transform(const glm::mat4& T);
    //! Center and scale this Mesh. So it if thigly contained by a unit cube
    //! center at the origin.
    void toUnitCube();
    //! Not yet implemented!
    void recalculateNormals();
    //! get the indices needed for glElementDraw* commands in a vector
    /*!
      One of the important interface functions. Since Model always stores data
      as an indexed array. This will give you an array with the indexes that you
      can use it to draw if you bound a corresponding VBO (See getVertices)
    */
    std::vector<unsigned int> getIndices() const;
    //! get the vertices needed to create a VBO for getiing this mesh into the GPU
    /*!
      One of the important interface functions. Since Mesh always stores data
      in an indexed buffer. This method will give you the buffer that you
      can use it to draw if you bound an use the corresponding indices buffer
      See (getIndices)

      Remember that the Mesh is always a triangular mesh so the number of
      indices is number of triangles times three
    */
    std::vector<Vertex> getVertices() const;
    //! Clear and cretes a new Mesh using the data provided
    /*!
      Recreates the object by providing data. The Mesh are indexed, so they
      need to provide two array one with the indexes and one with the vertices

      Since the Vertex format is fixed they user needs to declare if the normals
      and texture corrdinates are present
    */
    bool loadVerticesAndIndices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, bool normals = false, bool textCoords = false);
    //! Clear and cretes a new Mesh using the data provided
    /*!
      Recreates the object by providing data. Since the mesh is triangulated
      you can provide a set of triangles. The format of the triangles is fixed

      In order to create an indexed mesh for triangles, the Mesh will join all
      the vertex that are DELTA apart and will consider them the same vertex.
      This tends to create smoth meshes.

      It will produce a mesh witout normal nor texture coordinates.
    */
    bool loadFromTriangles(const std::vector<Triangle>& triangles);
    //! Calculate the scale factor tha will make this an
    /*!
      Recreates the object by providing data. Since the mesh is triangulated
      you can provide a set of triangles. The format of the triangles is fixed

      In order to create an indexed mesh for triangles, the Mesh will join all
      the vertex that are DELTA apart and will consider them the same vertex.
      This tends to create smoth meshes.

      It will produce a mesh witout normal nor texture coordinates.
    */
    float scaleFactor() const;
    //! Get the center of the axis aligned boundig box of the Mesh
    glm::vec3 getBBCenter() const;
    //! Get the lenght for the sides of an axis aligned bounding box
    glm::vec3 getBBSize() const;
    //! Get the number of triangles in this Mesh
    size_t trianglesCount() const;
    //! Get the number of indices in this Mesh (Three per triangle)
    size_t indicesCount() const;
    //! Get the number of vertices in the Mesh
    size_t vertexCount() const;
    //! Save the mesh on a file
    bool save(const QString& fileName = QString("")) const;
    //! Get the file (name and path) of the diffuse texture.
    /*!
      Get the path of an image file that represents the diffuse texture map.
      or an empthy string in case there is non defined.
      Note: that this info is usually extracted from the 3D model file,
      not from the filesystem. So, there is no garantee that the file exist.
     */
    std::string getDiffuseTexture() const;
};

#endif // MESH_H
