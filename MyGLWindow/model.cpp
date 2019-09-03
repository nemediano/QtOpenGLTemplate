#include <QDebug>
#include "model.h"


Model::Model() : Mesh() {

}

Model::Model(const QString& fileName) : Mesh(){
    load(fileName);
}

bool Model::load(const QString& fileName) {
    // Create an instance of the Importer class
    Assimp::Importer importer;

    const aiScene* scenePtr = importer.ReadFile( fileName.toStdString(),
          aiProcess_GenNormals       |
          aiProcess_Triangulate            |
          aiProcess_JoinIdenticalVertices);

    /* If the import failed, report it (I am guessing that he will be able
     to check if the file exist and if its writable itself. */
    if(!scenePtr || scenePtr->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scenePtr->mRootNode) {
        qDebug() << importer.GetErrorString();
        return false;
    }

    mIndices.clear();
    mVertices.clear();
    mSeparators.clear();
    //Start the recursivelly process at the root
    processNode(scenePtr->mRootNode, scenePtr);
    updateBoundingBox();

    return true;
}

std::vector<MeshData> Model::getSeparators() const {
    return mSeparators;
}

int Model::numMeshes() {
    //We have separator at the bigining and at the end
    return static_cast<int>(mSeparators.size() - 1);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process all the meshes (if any) in this node
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        addMeshData(mesh, scene);
    }

    // Then, recursivelly procees the child nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::addMeshData(const aiMesh* mesh, const aiScene* scene) {
    //Parse Mesh data
    if (!mesh || !mesh->HasPositions() || !scene) {
        qDebug() << "Weird mesh without vertex positions!";
        return;
    }

    MeshData bookMark;
    /* First the indices */
    unsigned int numFaces = mesh->mNumFaces;
    //I believe that I need to calculate an offset
    //unsigned int offset = mVertices.size();
    unsigned int indicesBefore = static_cast<unsigned int>(mIndices.size());
    for (unsigned int t = 0; t < numFaces; ++t) {
        const aiFace* face = &mesh->mFaces[t];
        for (unsigned int i = 0; i < face->mNumIndices; ++i) {
            mIndices.push_back(face->mIndices[i]);
        }
    }
    unsigned int indicesAfter = static_cast<unsigned int>(mIndices.size());
    bookMark.startIndex = int(indicesBefore);
    bookMark.howMany = int(indicesAfter - indicesBefore);

    /* Now, the Vertices */
    Vertex v;
    mHasNormals = mesh->HasNormals();
    mHasTexture = mesh->HasTextureCoords(0);
    bookMark.startVertex = int(mVertices.size());
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        if (mHasNormals) {
            v.normal.x = mesh->mNormals[i].x;
            v.normal.y = mesh->mNormals[i].y;
            v.normal.z = mesh->mNormals[i].z;
        }

        if (mHasTexture) {
            v.textCoords.s = mesh->mTextureCoords[0][i].x;
            v.textCoords.t = mesh->mTextureCoords[0][i].y;
        }

        mVertices.push_back(v);
    }

    int specularTexture = -1;
    int diffuseTexture = -1;
    if (mesh->mMaterialIndex > 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        diffuseTexture = addDiffuseTexture(material);
        specularTexture = addSpecularTexture(material);
    }

    bookMark.diffuseIndex = diffuseTexture;
    bookMark.specIndex = specularTexture;
    mSeparators.push_back(bookMark);
}

int Model::addDiffuseTexture(const aiMaterial* mat) {
    if (!mat) {
        return -1;
    }

    std::string textPath = "";
    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString fileName;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
        textPath = std::string(fileName.C_Str());
    } else {
        //NO diffuse texture for this Mesh
        return -1;
    }

    //Check if this texture is already in the vector
    for (size_t i = 0; i < mTexturesData.size(); ++i) {
        if (mTexturesData[i].filePath == textPath) {
            return int(i);
        }
    }

    //It's is not then create it an push it into the vector
    TextureImage text;
    text.type = DIFFUSE;
    text.filePath = textPath;
    mTexturesData.push_back(text);
    return static_cast<int>(mTexturesData.size() - 1);
}

int Model::addSpecularTexture(const aiMaterial* mat) {
    if (!mat) {
        return -1;
    }

    std::string textPath = "";
    if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
        aiString fileName;
        mat->GetTexture(aiTextureType_SPECULAR, 0, &fileName);
        textPath = std::string(fileName.C_Str());
    } else {
        //NO specular texture for this Mesh
        return -1;
    }

    //Check if this texture is already in the vector
    for (size_t i = 0; i < mTexturesData.size(); ++i) {
        if (mTexturesData[i].filePath == textPath) {
            return int(i);
        }
    }

    //It's is not then create it an push it into the vector
    TextureImage text;
    text.type = SPECULAR;
    text.filePath = textPath;
    mTexturesData.push_back(text);
    return static_cast<int>(mTexturesData.size() - 1);
}


std::vector<TextureImage> Model::getTextures() const {
    return mTexturesData;
}
