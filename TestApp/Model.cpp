#include "Model.h"
#include "Log.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Texture.h"



Model::Model(DirectXDevice* device, const std::string path)
{
	mpDevice = device;
	mbGenerateMipMaps = true;
	LoadModel(path);
}

Model::~Model()
{
}

void Model::Draw(DirectXDevice* device)
{
	for (int i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i]->Draw(device);
	}
}

void Model::LoadModel(const std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR << "Error loading model: " << path;
		LOG_ERROR << "ERROR::ASSIMP::" << importer.GetErrorString();
		return;
	}
	mDirectory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{
	// process all the node's meshes (if any)
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* modelMesh = ProcessMesh(mesh, scene);
		modelMesh->SetupMesh(mpDevice);
		mMeshes.push_back(modelMesh);
	}
	// then do the same for each of its children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureDetail> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;
		
		vertex.nx = mesh->mNormals[i].x;
		vertex.ny = mesh->mNormals[i].y;
		vertex.nz = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vertex.u = mesh->mTextureCoords[0][i].x;
			vertex.v = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<TextureDetail> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<TextureDetail> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	Mesh* modelMesh = new Mesh(vertices, indices, textures);
	return modelMesh;
}

std::vector<TextureDetail> Model::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureDetail> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < mTexturesLoaded.size(); j++)
		{
			if (std::strcmp(mTexturesLoaded[j].mPath.data(), str.C_Str()) == 0)
			{
				textures.push_back(mTexturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			TextureDetail texture;
			texture.mTexture = TextureFromFile(str.C_Str(), mDirectory);
			texture.mType = typeName;
			texture.mPath = str.C_Str();
			textures.push_back(texture);
			mTexturesLoaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

Texture* Model::TextureFromFile(const std::string path, const std::string directory)
{
	// Generate mip maps on textures with width and heights above or equal to this value
	static const int MIP_MAPS_ABOVE = 512;

	// Get the full file name
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	Texture* texture = new Texture();

	// Get the data from stbi
	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 4);

	// Create the texture
	if (data)
	{
		// If the size is above the constant then set all the properties needed to generate mip maps
		if (mbGenerateMipMaps)
		{
			if (width >= MIP_MAPS_ABOVE && height >= MIP_MAPS_ABOVE)
			{
				texture->SetBindFlags(D3D11_BIND_RENDER_TARGET);
				texture->SetMiscFlags(D3D11_RESOURCE_MISC_GENERATE_MIPS);
				int numLevels = 1 + floor(log2(max(width, height)));
				texture->SetMipLevels(numLevels);
				texture->SetUsage(D3D11_USAGE_DEFAULT);
			}
		}

		// Create and init the texture
		texture->SetDimensions(width, height);
		texture->SetFormat(DXGI_FORMAT_R8G8B8A8_UNORM);
		texture->SetInitialData(data, width * 4, 0);
		texture->Initialise(mpDevice);

		// Actually generate the mip maps
		if (mbGenerateMipMaps)
			if (width >= MIP_MAPS_ABOVE && height >= MIP_MAPS_ABOVE)
				mpDevice->GetContext()->GenerateMips(texture->GetShaderResourceView());
	}
	else
	{
		LOG_ERROR << "Texture failed to load at path: " << path;
	}

	// Free the image data
	stbi_image_free(data);
	return texture;
}
