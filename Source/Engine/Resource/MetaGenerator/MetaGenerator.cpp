#include "MetaGenerator.h"

#include "Engine/Data/DayTime.h"
#include "Engine/Data/Guid.h"
#include "Engine/Platform/PlatformFile.h"
#include "Engine/Configuration/ConfigCreator.h"

#include "Engine/Resource/Data/Geometry.h"
#include "Engine/Resource/Core/Resource.h"

#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Texture/Texture.h"

#include "Engine/Resource/Factory/ResourceFactory.h"

#include <magic_enum.hpp>
#include <stb_image.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace MAGE
{
	static json GenerateTextureMeta(const path& srcPath, const path& dstPath)
	{
		int width, height, channels;
		stbi_uc* data = stbi_load(srcPath.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (!data)
			spdlog::critical("Failed to read image file: {}", stbi_failure_reason());

		PlatformFile::Write(Config::GetAppConfig().projectPath.string() + dstPath.string() + srcPath.stem().string() + ".bin", { data, static_cast<usize>(width * height * channels) });
		stbi_image_free(data);

		json jFile;
		jFile["Properties"]["ID"] = Guid::GenerateID().ToString();
		jFile["Properties"]["Name"] = srcPath.stem().string();
		jFile["Properties"]["Path"] = dstPath.string();
		jFile["Properties"]["LastModified"] = DayTime::GetCurrentDayTime().ToString();
		jFile["Properties"]["Size"] = static_cast<usize>(width * height * channels);
		jFile["Properties"]["Type"] = "Texture";
		jFile["Details"]["Dimensions"][0] = width;
		jFile["Details"]["Dimensions"][1] = height;
		jFile["Details"]["ChannelCount"] = channels;
		jFile["Details"]["ColorDepth"] = 8;
		jFile["Details"]["SubType"] = "2D";
		PlatformFile::Write(Config::GetAppConfig().projectPath.string() + dstPath.string() + srcPath.stem().string() + ".mage", jFile.dump(4));

		return jFile;
	}

	static json GenerateMeshMeta(const path& srcPath, const path& dstPath)
	{
		Geometry geometry;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(srcPath.string(),
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_SortByPType);

		if (!scene || !scene->HasMeshes())
			spdlog::critical("Failed to read mesh file: {}", importer.GetErrorString());

		aiMesh* mesh{};
		if (scene && scene->mNumMeshes > 0)
		{
			mesh = scene->mMeshes[0];
			geometry.vertices.reserve(mesh->mNumVertices);
			geometry.indices.reserve(mesh->mNumFaces * 3);

			for (u32 i = 0; i < mesh->mNumVertices; ++i)
			{
				Vertex vertex = {};
				vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y , mesh->mVertices[i].z };

				if (mesh->HasNormals())
					vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
				else
					vertex.normal = { 0.0f, 0.0f, 0.0f };

				if (mesh->HasTangentsAndBitangents())
				{
					vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
					vertex.bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
				}
				else
				{
					vertex.tangent = { 0.0f, 0.0f, 0.0f };
					vertex.bitangent = { 0.0f, 0.0f, 0.0f };
				}

				if (mesh->HasVertexColors(0))
					vertex.color = { mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a };
				else
					vertex.color = { 0.0f, 0.0f, 0.0f, 0.0f };

				if (mesh->HasTextureCoords(0))
					vertex.uv = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
				else
					vertex.uv = { 0.0f, 0.0f };

				geometry.vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
			{
				aiFace& face = mesh->mFaces[i];
				if (face.mNumIndices == 3)
				{
					geometry.indices.push_back(static_cast<u32>(face.mIndices[0]));
					geometry.indices.push_back(static_cast<u32>(face.mIndices[1]));
					geometry.indices.push_back(static_cast<u32>(face.mIndices[2]));
				}
			}
		}

		usize actualSize = sizeof(Geometry);
		actualSize += geometry.vertices.capacity() * sizeof(Vertex);
		actualSize += geometry.indices.capacity() * sizeof(u32);

		PlatformFile::Write(Config::GetAppConfig().projectPath.string() + dstPath.string() + srcPath.stem().string() + ".bin", { &geometry, actualSize });

		json jFile;
		jFile["Properties"]["ID"] = Guid::GenerateID().ToString();
		jFile["Properties"]["Name"] = srcPath.stem().string();
		jFile["Properties"]["Path"] = dstPath.string();
		jFile["Properties"]["LastModified"] = DayTime::GetCurrentDayTime().ToString();
		jFile["Properties"]["Size"] = actualSize;
		jFile["Properties"]["Type"] = "Mesh";
		jFile["Details"]["IndexCount"] = geometry.indices.size();
		jFile["Details"]["VertexCount"] = geometry.vertices.size();
		jFile["Details"]["SubType"] = "Static";
		PlatformFile::Write(Config::GetAppConfig().projectPath.string() + dstPath.string() + srcPath.stem().string() + ".mage", jFile.dump(4));

		return jFile;
	}

	map<string, function<json(const path&, const path&)>> GenerateMetaMaps()
	{
		map<string, function<json(const path&, const path&)>> result;

		result.emplace(".png", &GenerateTextureMeta);
		result.emplace(".tga", &GenerateTextureMeta);
		result.emplace(".tiff", &GenerateTextureMeta);

		result.emplace(".fbx", &GenerateMeshMeta);
		result.emplace(".gltf", &GenerateMeshMeta);

		return result;
	}

	const map<string, function<json(const path&, const path&)>> metaMaps = GenerateMetaMaps();

	json MetaHandler::GetResourceMeta(const path& srcPath, const path& dstPath)
	{
		auto meta = metaMaps.find(srcPath.extension().string());
		if (meta == metaMaps.end()) [[unlikely]]
		{
			spdlog::critical("Extension {} is not being supported.", srcPath.extension().string());
			exit(-1);
		}

		return meta->second(srcPath, dstPath);
	}

	static Shared<Resource> GenerateTextureResource(const path& srcPath, const path& dstPath)
	{
		return MakeShared<Texture>(MetaHandler::GetResourceMeta(srcPath, dstPath));
	}

	static Shared<Resource> GenerateMeshResource(const path& srcPath, const path& dstPath)
	{
		return MakeShared<Mesh>(MetaHandler::GetResourceMeta(srcPath, dstPath));
	}

	map<string, function<Shared<Resource>(const path&, const path&)>> GenerateResourceMaps()
	{
		map<string, function<Shared<Resource>(const path&, const path&)>> result;

		result.emplace(".png", &GenerateTextureResource);
		result.emplace(".tga", &GenerateTextureResource);
		result.emplace(".tiff", &GenerateTextureResource);

		result.emplace(".fbx", &GenerateMeshResource);
		result.emplace(".gltf", &GenerateMeshResource);

		return result;
	}

	const map<string, function<Shared<Resource>(const path&, const path&)>> resourceMaps = GenerateResourceMaps();

	Shared<Resource> ResourceHandler::GetResourceData(const path& srcPath, const path& dstPath)
	{
		auto res = resourceMaps.find(srcPath.extension().string());
		if (res == resourceMaps.end()) [[unlikely]]
		{
			spdlog::critical("Extension {} is not being supported.", srcPath.extension().string());
			exit(-1);
		}
		auto ctx = res->second(srcPath, dstPath);
		ResourceFactory::Get().RegisterResource(ctx);
		return ctx;
	}
}
