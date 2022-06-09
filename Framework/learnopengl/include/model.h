#ifndef MODEL_H
#define MODEL_H



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl/include/mesh.h>
#include <learnopengl/include/shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
//fwd declare assimp variables
struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiScene;


class Model 
{
public:
    /*  Model Data */
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    Model(string const& a_path, bool a_gamma = false);

    // draws the model, and thus all its meshes
    void Draw(Shader a_shader);
    
private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void LoadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void ProcessNode(aiNode* a_node, const aiScene* a_scene);

    Mesh ProcessMesh(aiMesh* a_mesh, const aiScene* a_scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> LoadMaterialTextures(aiMaterial* a_mat, int a_type, string a_typeName);
    static unsigned int TextureFromFile(const char* a_path, const string& a_directory, bool a_gamma);
};
#endif

