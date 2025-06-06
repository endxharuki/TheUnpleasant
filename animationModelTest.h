#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "gameObjectComponent.h"
#include "gameObject.h"
#include <map>
#define MAX_MESH (100)

class AnimationModelTest: public GameObjectComponent
{
private:
	//変形後頂点構造体
	struct DEFORM_VERTEX
	{
		aiVector3D Position;
		aiVector3D Normal;
		int				BoneNum;
		int             BoneIndices[4];
		std::string		BoneName[4];//本来はボーンインデックスで管理するべき
		float			BoneWeight[4];
	};

	//ボーン構造体
	struct BONE
	{
		aiMatrix4x4 Matrix = { 1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 1.0f };
		aiMatrix4x4 AnimationMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 1.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f };
		aiMatrix4x4 OffsetMatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, 1.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f };

	};

	//ボーンデータ
	struct BoneVertex
	{
		XMFLOAT4 Position;  // 頂点位置
		XMFLOAT4 Normal;    // 法線
		XMFLOAT4 Diffuse;   // 色
		XMFLOAT2 TexCoord;  // テクスチャ座標
		int BoneIndices[4];          // ボーンインデックス（最大4つ）
		float BoneWeights[4];        // ボーンウェイト（最大4つ）

	};
	struct Vertex
	{
		XMFLOAT4 Position;  // 頂点位置
		XMFLOAT4 Normal;    // 法線
		XMFLOAT4 Diffuse;   // 色
		XMFLOAT2 TexCoord;  // テクスチャ座標
	};
	struct BoneMatrices
	{
		// 頂点数
		
		XMFLOAT4X4 boneMatrices[256];
		UINT vertexCount;
		
	};


	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;
	ID3D11Buffer* m_IndexAllBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;

//	std::unordered_map<std::string, BONE> m_Bone;//ボーンデータ（名前で参照）
	std::unordered_map<std::string, BONE> m_Bone;//ボーンデータ（名前で参照）
	BoneMatrices m_BoneMatrices;
	// ボーンIDを割り当てるマップ
	std::unordered_map<std::string, int> boneNameToID;

	ID3D11Buffer* m_BoneBuffer = nullptr;
	ID3D11ShaderResourceView* m_BoneSRV = nullptr;

	//GPUスキニング用バッファとSRVとUAV
	//CS
	ID3D11Buffer* m_BeforeVertexBuffer[MAX_MESH];
	ID3D11Buffer* m_AfterVertexBuffer[MAX_MESH];

	ID3D11ShaderResourceView* m_BeforeVertexShaderSRV[MAX_MESH];
	ID3D11UnorderedAccessView* m_AfterVertexShaderUAV[MAX_MESH];
	//VS
	ID3D11Buffer* m_SkinningUpdateBuffer[MAX_MESH];
	ID3D11ShaderResourceView* m_SkinningUpdateSRV[MAX_MESH];

	ID3D11Buffer* m_ConstantBuffer{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11ComputeShader* m_ComputeShader{};

	int m_BoneCount = 0;
	float m_Alpha = 0.0f;
	int m_VertexCount = 0;

	XMMATRIX m_RightHandMatrix = {};

public:
	using GameObjectComponent::GameObjectComponent;

	void Load(const char* FileName);
	void Uninit() override;
	void Draw() override;

	void LoadAnimation(const char* FileName, const char* Name);
	void CreateBone(aiNode* node,// ボーン名 → ボーンID のマッピング
		std::map<std::string, int>& boneNameIndex,
	int& boneCount);
	void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRatio);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
	void UpdateBoneBuffer(std::vector<aiMatrix4x4> boneMatrices);

	int GetBoneIndex(const std::string& boneName);

	void CreateBeforeVertexBuffer(int index, BoneVertex* vertex ,int mesh);
	void CreateBeforeVertexSRV(int index, int mesh);
	void CreateAfterVertexBuffer(int index, Vertex* vertex, int mesh);

	void CreateUAV(int index, int mesh);
	void CreateAfterVertexSRV(int index, int mesh);
	void CreateConstantBuffer();

	void SetBoneMatrices(const BoneMatrices* matrices);


	std::unordered_map<std::string, BONE> GetBone() { return m_Bone; }

	void SetAlpha(float a) { m_Alpha = a; }
	XMMATRIX GetRightHandMatrix() const { return m_RightHandMatrix; }
	

};