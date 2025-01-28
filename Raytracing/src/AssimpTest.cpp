//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <map>
//#include <vector>
//#include <string>
//
//bool Import(const std::string& pFile)
//{
//	Assimp::Importer importer;
//
//	const aiScene* scene = importer.ReadFile(pFile,
//		aiProcess_CalcTangentSpace |
//		aiProcess_Triangulate |
//		aiProcess_JoinIdenticalVertices |
//		aiProcess_SortByPType);
//	// If the import failed, report it
//	if (!scene) {
//		std::cout << importer.GetErrorString() << std::endl;
//		return false;
//	}
//	std::cout << "����ɹ�" << std::endl;
//
//	std::cout << scene->mNumMeshes << std::endl;
//
//	aiMesh* Mesh = scene->mMeshes[0];
//	std::cout << Mesh->mNumFaces << std::endl;
//	//for (int i = 0; i < Mesh->mNumVertices; i++)
//	//{
//	//	std::cout <<"(" << Mesh->mVertices[i].x << "," << Mesh->mVertices[i].y << "," << Mesh->mVertices[i].z << ")" << std::endl;
//	//}
//	//��һ��for����ÿһ����
//	for (int i = 0; i < Mesh->mNumFaces; i++)
//	{
//		aiFace face = Mesh->mFaces[i];
//		std::cout << i << ": (";
//		//�ڶ���for�ڵ�һ��iȷ��һ��ֵ���ҳ�һ���棬�ٽ����������������������face.mNumIndices = 3
//		for (int j = 0; j < face.mNumIndices; j++)
//		{
//			std::cout << face.mIndices[j] << ",";
//		}
//		std::cout << ")" << std::endl;
//	}
//
//
//	// We're done. Everything will be cleaned up by the importer destructor
//	return true;
//}
//
//int main()
//{
//	Import("Hydrant.obj");
//
//	return 0;
//}