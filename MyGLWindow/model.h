#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

//!  A simple struct that act as a separator of the Meshes in this model.
/*!
  This struct encapsulate all the data needed to draw an individual Mesh
  of a model. If a model is compose of n meshes, it will also contain a
  vector of n MeshData elements where each one can be used as a helper
  to render an specific Mesh.
*/
typedef struct MeshData {
    //! The place of the first vertex of this mesh.
    GLint startVertex;
    //! The place of the first index of this mesh.
    GLint startIndex;
    //! Number of indexes in this mesh.
    /*! The number of triangles is this number
        divided by three.
    */
    GLsizei howMany;
    //! Index of the specular texture
    /*! The index of this mesh specular texture in the textures vector
     * or -1 if this mesh does not have a specular texture
    */
    int specIndex;
    //! Index of the diffuse texture
    /*! The index of this mesh diffuse texture in the textures vector
     * or -1 if this mesh does not have a diffuse texture
    */
    int diffuseIndex;
} MeshData;

enum TextType {DIFFUSE, SPECULAR, NORMALS, OTHER};

//!  Container of all the images of the mesh.
/*!

*/
struct TextureImage {
    //! The place of the first vertex of this mesh.
    std::string filePath;
    //! The place of the first index of this mesh.
    TextType type;

    bool operator==(const TextureImage& lhs) {
        return this->filePath == lhs.filePath;
    }
};


//!  This class loads a 3D model that are composed of more than one mesh.
/*!
  This is an specialized version of the Mesh class used to load a 3D model
  that is composed by more than one Mesh.

  This class only loads model from the file to main memmory in CPU. Therefore
  1) Is decouppled of any rendering code.
  2) It does not use the GPU at all.

  Saying the above, it is designed to provide you with the needed interface
  for a class that will use this data to render it using OpenGL.

  All the meshes are combined into a single Vertex array and a single indices
  array. Therefore this class can also providea vector of separators.

  If the file has a model with only one Mesh this class also works. However,
  in this case, is better to use the Mesh base class.
*/
class Model : public Mesh {

protected:
    std::vector<TextureImage> mTexturesData;
    void processNode(aiNode* node, const aiScene* scene);
    void addMeshData(const aiMesh* mesh, const aiScene* scene);
    int addDiffuseTexture(const aiMaterial* material);
    int addSpecularTexture(const aiMaterial* material);
    std::vector<MeshData> mSeparators;

public:
    //! Simple constructor that only initialices the data struct.
    Model();
    //! Loads this 3D model from the fileName
    explicit Model(const QString& fileName);
    //! Clears the current data. Then loads this 3D model from the fileName
    bool load(const QString& fileName);
    //! Get a vector of MeshData that act as a separator of the meshes.
    /*!
      Get a vector of MeshData, since all the model data is contained in a 
      single buffer. This vector act as a series of seprators of each mesh.
      They provide with all the date nneded to render each mesh using
      glDrawElementsBaseVertex.
    */
    std::vector<MeshData> getSeparators() const;
    //! Get the filenames of each of the textures in this model.
    /*!
      Get the filenames of each of the textures in this model.
      If a mesh in the model does not contain a certain texture an empthy
      string is placed. So the vector always contain as many elements as mesh
      are in the model
    */
    std::vector<TextureImage> getTextures() const;
    //! Get the number of meshes in this Model.
    int numMeshes();
};

#endif // MODEL_H
