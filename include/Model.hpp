/*
 * Created by Sarthak Rai on 10 Apr 2025.
*/

#pragma once

#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>

namespace STARBORN {

  unsigned int texture_from_file(const char *path, const std::string &directory, bool gamma = false);

  Material load_material(aiMaterial *mat) {
    Material material{};
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.shininess = shininess;

    return material;
  }

  class Model {
  private:
    // ---- Private Methods ----
    void load_model(const std::string& path) {
      Assimp::Importer importer;
      const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Failed to load model");
      }

      directory_ = path.substr(0, path.find_last_of('/'));

      process_node(scene->mRootNode, scene);
    }

    void process_node(aiNode *node, const aiScene *scene) {
      for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(process_mesh(mesh, scene));
      }

      for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
      }
    }

    Mesh process_mesh(aiMesh *mesh, const aiScene *scene) {
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      std::vector<Texture> textures;

      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        // ---- Positions ----
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // ---- Normals ----
        if (mesh->HasNormals()) {
          vector.x = mesh->mNormals[i].x;
          vector.y = mesh->mNormals[i].y;
          vector.z = mesh->mNormals[i].z;
          vertex.normal = vector;
        }

        // ---- Texture Coordinates ----
        if (mesh->mTextureCoords[0]) {
          glm::vec2 vec;
          vec.x = mesh->mTextureCoords[0][i].x;
          vec.y = mesh->mTextureCoords[0][i].y;
          vertex.tex_coords = vec;

          // ---- Tangent ----
          vector.x = mesh->mTangents[i].x;
          vector.y = mesh->mTangents[i].y;
          vector.z = mesh->mTangents[i].z;
          vertex.tangent = vector;

          // ---- Bitangent ----
          vector.x = mesh->mBitangents[i].x;
          vector.y = mesh->mBitangents[i].y;
          vector.z = mesh->mBitangents[i].z;
          vertex.bitangent = vector;
        } else vertex.tex_coords = glm::vec2(0.0f, 0.0f);

        if (scene->mNumMaterials > mesh->mMaterialIndex) {
          const auto &mat = scene->mMaterials[mesh->mMaterialIndex];
          aiColor4D diffuse;

          if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
            vertex.color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
          }

          if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) vertex.use_diffuse_texture = 1.0f;
          else vertex.use_diffuse_texture = 0.0f;
        }

        vertices.push_back(vertex);
      }

      // ---- Walk Through Mesh Faces ----
      for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
          indices.push_back(face.mIndices[j]);
        }
      }

      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

      // ---- Diffuse Maps ----
      std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

      // ---- Specular Maps ----
      std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

      // ---- Normal Maps ----
      std::vector<Texture> normal_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
      textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

      // ---- Height Maps ----
      std::vector<Texture> height_maps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
      textures.insert(textures.end(), height_maps.begin(), height_maps.end());

      return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> load_material_textures(const aiMaterial *mat,
                                                const aiTextureType type, const std::string& type_name) {
      std::vector<Texture> textures;

      for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded_.size(); j++) {
          if (std::strcmp(textures_loaded_[j].path.data(), str.C_Str()) == 0) {
            textures.push_back(textures_loaded_[j]);
            skip = true;
            break;
          }
        }
        if (!skip) {
          Texture texture;
          texture.id = texture_from_file(str.C_Str(), this->directory_);
          texture.type = type_name;
          texture.path = str.C_Str();
          textures.push_back(texture);
          textures_loaded_.push_back(texture);
        }
      }
      return textures;
    }

  public:
    // ---- Model Data ----
    std::vector<Texture> textures_loaded_;
    std::vector<Mesh> meshes_;
    std::string directory_;
    bool gamma_correction_;

    explicit Model(const std::string &path, bool gamma = false) : gamma_correction_(gamma) {
      load_model(path);
    }

    void draw(Shader &shader) {
      for (auto & mesh : meshes_) {
        mesh.draw(shader);
      }
    }
  };

  unsigned int texture_from_file(const char *path, const std::string &directory, bool gamma) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nr_components;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nr_components, 0);

    if (data) {
      GLenum format;
      if (nr_components == 1) format = GL_RED;
      else if (nr_components == 3) format = GL_RGB;
      else if (nr_components == 4) format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, texture_id);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    } else {
      stbi_image_free(data);
      throw std::runtime_error("Failed to load texture from file " + filename);
    }

    return texture_id;
  }
} // STARBORN
