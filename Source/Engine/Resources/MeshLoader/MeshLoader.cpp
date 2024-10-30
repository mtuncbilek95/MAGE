#include "MeshLoader.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace MAGE
{
	Vector<GeometryData> Importer::LoadGeometry(const String& path)
	{
		Vector<GeometryData> geometries;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.CharString(), aiProcess_Triangulate | aiProcess_FlipUVs | 
			aiProcess_CalcTangentSpace | aiProcess_SortByPType | aiProcess_OptimizeMeshes);

		if (!scene || scene->mNumMeshes == 0)
			spdlog::error("Failed to load mesh: {}", path.CharString());

		for (u32 m = 0; m < scene->mNumMeshes; ++m) {
			GeometryData geometry;
			aiMesh* mesh = scene->mMeshes[m];

			for (u32 i = 0; i < mesh->mNumVertices; ++i) {
				geometry.position.push_back({ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

				if (mesh->HasNormals()) {
					geometry.normal.push_back({ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
				}

				if (mesh->HasTangentsAndBitangents()) {
					geometry.tangent.push_back({ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z });
					geometry.bitangent.push_back({ mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z });
				}

				if (mesh->HasTextureCoords(0)) {
					geometry.uv.push_back({ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
				}

				if (mesh->HasVertexColors(0)) {
					geometry.color.push_back({ mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a });
				}
			}

			// Load index data
			for (u32 i = 0; i < mesh->mNumFaces; ++i) {
				const aiFace& face = mesh->mFaces[i];
				for (u32 j = 0; j < face.mNumIndices; ++j) {
					geometry.indices.push_back(face.mIndices[j]);
				}
			}

			geometries.push_back(std::move(geometry));
		}

		return geometries;
	}
}
