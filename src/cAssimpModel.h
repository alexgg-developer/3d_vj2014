#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "cMesh.hpp"
#include "cTexture.hpp"
#include <glm/glm.hpp>
#include <gl/glew.h>


class cAssimpModel {
  struct cTexture {
	  cTexture(void);
	  ~cTexture(void);

	  bool Load(char *filename,int type = GL_RGBA,int wraps = GL_REPEAT,int wrapt = GL_REPEAT,
			    int magf = GL_LINEAR,int minf = GL_LINEAR,bool mipmap = true);
	  int  GetID();
	  void GetSize(int *w,int *h);

  private:
	  GLuint id;
	  int width;
	  int height;
  };
	glm::vec3 bbox[2];
	std::vector<cMesh> meshes;
	std::vector<cTexture*> textures;
	glm::vec3 center;
	float height;
	int displayList;

public:
	cAssimpModel();
	~cAssimpModel();

	bool LoadFromFile(const std::string &filename);
	glm::vec3 *GetBoundingBox();
	void Render() const;
  void RenderRaw() const;

private:
	void Clear();
	bool InitFromScene(const aiScene* pScene, const std::string &filename);
	void InitMesh(unsigned int index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& filename);
	void ComputeBoundingBox();
	void CompileDisplayList();
  void RenderInternal() const;
};
#include <map>
extern std::map<std::string, cAssimpModel> gLoadedModels;
