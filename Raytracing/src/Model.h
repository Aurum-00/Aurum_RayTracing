#pragma once

//#include <glad/glad.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;
using namespace glm;

// 物体表面材质定义
struct Material {
	vec3 emissive = vec3(0, 0, 0);  // 作为光源时的发光颜色
	vec3 baseColor = vec3(1, 1, 1);
	float subsurface = 0.0;
	float metallic = 0.0;
	float specular = 0.0;
	float specularTint = 0.0;
	float roughness = 0.0;
	float anisotropic = 0.0;
	float sheen = 0.0;
	float sheenTint = 0.0;
	float clearcoat = 0.0;
	float clearcoatGloss = 0.0;
	float IOR = 1.0;
	float transmission = 0.0;
};

// 三角形定义
struct Triangle 
{
	vec3 p1, p2, p3;    // 顶点坐标
	vec3 n1, n2, n3;    // 顶点法线
	Material material;  // 材质
};

//三角形重编码
struct Triangle_encoded 
{
	vec3 p1, p2, p3;    // 顶点坐标
	vec3 n1, n2, n3;    // 顶点法线
	vec3 emissive;      // 自发光参数
	vec3 baseColor;     // 颜色
	vec3 param1;        // (subsurface, metallic, specular)
	vec3 param2;        // (specularTint, roughness, anisotropic)
	vec3 param3;        // (sheen, sheenTint, clearcoat)
	vec3 param4;        // (clearcoatGloss, IOR, transmission)
};

//三角形容器名
vector<Triangle> triangles;
vector<float> Trivertices;



bool Import(const std::string& pFile, std::vector<float> Trivertices, std::vector<Triangle>& triangles, Material material, bool smoothNormal)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	// If the import failed, report it
	if (!scene) {
		std::cout << importer.GetErrorString() << std::endl;
		return false;
	}
	std::cout << "导入成功" << std::endl;

	std::cout << scene->mNumMeshes << std::endl;

	aiMesh* Mesh = scene->mMeshes[0];
	std::cout << Mesh->mNumFaces << std::endl;
	//for (int i = 0; i < Mesh->mNumVertices; i++)
	//{
	//	std::cout <<"(" << Mesh->mVertices[i].x << "," << Mesh->mVertices[i].y << "," << Mesh->mVertices[i].z << ")" << std::endl;
	//}
	//第一个for遍历每一个面
	//for (int i = 0; i < Mesh->mNumFaces; i++)
	//{
	//	aiFace face = Mesh->mFaces[i];
	//	std::cout << i << ": (";
	//	//第二个for在第一个i确定一个值，找出一个面，再将这个面的位置索引输出，三个点face.mNumIndices = 3
	//	for (int j = 0; j < face.mNumIndices; j++)
	//	{
	//		std::cout << face.mIndices[j] << ",";
	//	}
	//	std::cout << ")" << std::endl;
	//}

	vector<vec3> vertices;
	vector<unsigned int> indices;
	
	//// 计算 AABB 盒，归一化模型大小
	//float maxx = -11451419.19;
	//float maxy = -11451419.19;
	//float maxz = -11451419.19;
	//float minx = 11451419.19;
	//float miny = 11451419.19;
	//float minz = 11451419.19;

	//读取顶点索引
	for (int i = 0; i < Mesh->mNumFaces; i++)
	{
		aiFace face = Mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	for (int i = 0; i < Mesh->mNumVertices; i++)
	{
	//std::cout <<"(" << Mesh->mVertices[i].x << "," << Mesh->mVertices[i].y << "," << Mesh->mVertices[i].z << ")" << std::endl;
	vertices.push_back(vec3(Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z));
	}

	for (int i = 0; i < Mesh->mNumVertices; i++)
	{
		//std::cout <<"(" << Mesh->mVertices[i].x << "," << Mesh->mVertices[i].y << "," << Mesh->mVertices[i].z << ")" << std::endl;
		Trivertices.push_back(Mesh->mVertices[i].x);
		Trivertices.push_back(Mesh->mVertices[i].y);
		Trivertices.push_back(Mesh->mVertices[i].z);

	}

	std::cout << "(" << Mesh->mVertices[1].x << "," << Mesh->mVertices[1].y << "," << Mesh->mVertices[1].z << ")" << std::endl;
	cout << "==========" << endl;
	std::cout << "(" << vertices[1].x << "," << vertices[1].y << "," << vertices[1].z << ")" << std::endl;


	// 生成法线
	std::vector<vec3> normals(vertices.size(), vec3(0, 0, 0));
	for (int i = 0; i < indices.size(); i += 3) {
		vec3 p1 = vertices[indices[i]];
		vec3 p2 = vertices[indices[i + 1]];
		vec3 p3 = vertices[indices[i + 2]];
		vec3 n = normalize(cross(p2 - p1, p3 - p1));
		normals[indices[i]] += n;
		normals[indices[i + 1]] += n;
		normals[indices[i + 2]] += n;
	}

	// 构建 Triangle 对象数组
	int offset = triangles.size();  // 增量更新
	triangles.resize(offset + indices.size() / 3);
	for (int i = 0; i < indices.size(); i += 3)
	{
		Triangle& t = triangles[offset + i / 3];
		// 传顶点属性
		t.p1 = vertices[indices[i]];
		t.p2 = vertices[indices[i + 1]];
		t.p3 = vertices[indices[i + 2]];
		if (!smoothNormal)
		{
			vec3 n = normalize(cross(t.p2 - t.p1, t.p3 - t.p1));
			t.n1 = n; t.n2 = n; t.n3 = n;
		}
		else
		{
			t.n1 = normalize(normals[indices[i]]);
			t.n2 = normalize(normals[indices[i + 1]]);
			t.n3 = normalize(normals[indices[i + 2]]);
		}

		// 传材质
		t.material = material;
	}
	return true;
}
