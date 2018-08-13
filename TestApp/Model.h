#pragma once
#include "Mesh.h"
#include <string>
#include <vector>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "DirectXDevice.h"

class Model
{
public:
	Model(DirectXDevice* device, std::string path);
	~Model();

	void Draw(DirectXDevice* device);

private:
	void LoadModel(const std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh* ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<TextureDetail> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	Texture* TextureFromFile(const std::string path, const std::string directory);

public:
	std::vector<Mesh*> mMeshes;
	std::string mDirectory;
	std::vector<TextureDetail> mTexturesLoaded;

	DirectXDevice* mpDevice;
	bool mbGenerateMipMaps;
};

