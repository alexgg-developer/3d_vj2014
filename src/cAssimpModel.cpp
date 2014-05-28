#include <iostream>
#include "cAssimpModel.h"


cAssimpModel::cAssimpModel()
{
}

cAssimpModel::~cAssimpModel()
{
	Clear();
}


bool cAssimpModel::LoadFromFile(const std::string &filename)
{
	bool retCode = false;
	Assimp::Importer Importer;
	const aiScene* pScene;

	Clear();
	pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if(pScene)
		retCode = InitFromScene(pScene, filename);
	else
		std::cerr << "Error parsing '" << filename << "': '" << Importer.GetErrorString() << std::endl;
	ComputeBoundingBox();
	CompileDisplayList();

	return retCode;
}

glm::vec3 *cAssimpModel::GetBoundingBox()
{
	return bbox;
}

void cAssimpModel::Render()
{
	glPushMatrix();
	glScalef(1/height, 1/height, 1/height);
	glTranslatef(-center.x, -center.y, -center.z);
	glCallList(displayList);
	glPopMatrix();
}

void cAssimpModel::CompileDisplayList()
{
	unsigned int index;
	glm::vec3 V, N;
	glm::vec2 tC;

	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
	// Render all submeshes
	for(unsigned int i=0; i<meshes.size(); i++)
	{
		if(textures[meshes[i].textureIndex] != NULL)
      //textures[meshes[i].textureIndex]->bind();
			glBindTexture(GL_TEXTURE_2D, textures[meshes[i].textureIndex]->GetID());
		else
			glBindTexture(GL_TEXTURE_2D, 0);
		// Render submesh i
		glBegin(GL_TRIANGLES);
		for(unsigned int j=0; j<meshes[i].triangles.size(); j++)
		{
			index = meshes[i].triangles[j];
			V = meshes[i].vertices[index];
			N = meshes[i].normals[index];
			tC = meshes[i].texCoords[index];
			glNormal3f(N.x, N.y, N.z);
			glTexCoord2f(tC.x, tC.y);
			glVertex3f(V.x, V.y, V.z);
		}
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();
}

void cAssimpModel::Clear()
{
	meshes.clear();
	textures.clear();
}

bool cAssimpModel::InitFromScene(const aiScene* pScene, const std::string &filename)
{
	meshes.resize(pScene->mNumMeshes);
	textures.resize(pScene->mNumMaterials);

	for(unsigned int i=0; i<meshes.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, filename);
}

void cAssimpModel::InitMesh(unsigned int index, const aiMesh* paiMesh)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	const aiVector3D* pPos;
	const aiVector3D* pNormal;
	const aiVector3D* pTexCoord;

	meshes[index].textureIndex = paiMesh->mMaterialIndex;

	// Init vertices properties
	for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++)
	{
		pPos = &(paiMesh->mVertices[i]);
		pNormal = &(paiMesh->mNormals[i]);
		pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		meshes[index].vertices.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		meshes[index].normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		meshes[index].texCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	// Init faces indices
	for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		meshes[index].triangles.push_back(Face.mIndices[0]);
		meshes[index].triangles.push_back(Face.mIndices[1]);
		meshes[index].triangles.push_back(Face.mIndices[2]);
	}
}

bool cAssimpModel::InitMaterials(const aiScene* pScene, const std::string& filename)
{
    bool retCode = true;
	
    std::string::size_type SlashIndex = filename.find_last_of("/");
    std::string dir;

    // Extract the directory part from the file name
    if (SlashIndex == std::string::npos)
        dir = ".";
    else if (SlashIndex == 0)
        dir = "/";
    else
        dir = filename.substr(0, SlashIndex);

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
      textures[i] = NULL;
      aiMaterial const*const pMaterial = pScene->mMaterials[i];

        if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
          aiString Path;

          if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
              std::string sFullPath = dir + "/" + Path.data;
				      char fullPath[200];
				      strcpy_s(fullPath, sFullPath.c_str());
				
				      textures[i] = new cTexture();
              if(!textures[i]->Load(fullPath, GL_RGB)) {
              //if(!textures[i]->load(fullPath)) {
                  std::cerr << "Error loading texture '" << fullPath << "'" << std::endl;
                  delete textures[i];
                  textures[i] = NULL;
                  retCode = false;
              }
            }
       }
    }

    return retCode;
}

void cAssimpModel::ComputeBoundingBox()
{
	bbox[0] = glm::vec3(1e10, 1e10, 1e10);
	bbox[1] = glm::vec3(-1e10, -1e10, -1e10);
	for(unsigned int i=0; i<meshes.size(); i++)
	{
		for(unsigned int j=0; j<meshes[i].vertices.size(); j++)
		{
			bbox[0].x = std::min(bbox[0].x, meshes[i].vertices[j].x);
			bbox[0].y = std::min(bbox[0].y, meshes[i].vertices[j].y);
			bbox[0].z = std::min(bbox[0].z, meshes[i].vertices[j].z);
			bbox[1].x = std::max(bbox[1].x, meshes[i].vertices[j].x);
			bbox[1].y = std::max(bbox[1].y, meshes[i].vertices[j].y);
			bbox[1].z = std::max(bbox[1].z, meshes[i].vertices[j].z);
		}
	}
	height = bbox[1].y-bbox[0].y;
	center = glm::vec3((bbox[1].x+bbox[0].x)/2.f, (bbox[1].y+bbox[0].y)/2.f, (bbox[1].z+bbox[0].z)/2.f);
}




///cTexture version
#include "corona.h"
cAssimpModel::cTexture::cTexture(void) {}
cAssimpModel::cTexture::~cTexture(void){}

bool cAssimpModel::cTexture::Load(char *filename,int type,int wraps,int wrapt,int magf,int minf,bool mipmap)
{
	corona::Image* img;
	int components;

	img = corona::OpenImage(filename);
	if(type==GL_RGB)
	{
		//img = corona::OpenImage(filename,corona::PF_R8G8B8);
		components = 3;
	}
	else if(type==GL_RGBA)
	{
		//img = corona::OpenImage(filename,corona::PF_R8G8B8A8);
		components = 4;
	}
	else return false;

	if(img==NULL) return false;

	width  = img->getWidth();
	height = img->getHeight();

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D,id);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wraps);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapt);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magf);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minf);

	if(!mipmap)
	{
		glTexImage2D(GL_TEXTURE_2D,0,components,width,height,0,type,
					 GL_UNSIGNED_BYTE,img->getPixels());
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D,components,width,height,type,
						  GL_UNSIGNED_BYTE,img->getPixels());
	}

	return true;
}
int cAssimpModel::cTexture::GetID()
{
	return id;
}
void cAssimpModel::cTexture::GetSize(int *w,int *h)
{
	*w = width;
	*h = height;
}