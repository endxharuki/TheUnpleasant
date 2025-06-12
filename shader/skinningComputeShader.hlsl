#include "common.hlsl"

struct Vertex
{
    float4 position;
    float4 normal;
    float4 diffuse;
    float2 uv;
    
    int BoneIndices[4];
    float BoneWeights[4];
};

cbuffer ModelBuffer : register(b0)
{
    float4x4 boneMatrix[256]; // �e�{�[���̕ϊ��s��
    uint maxVertexCount; // ���f���̍ő咸�_��  
};

StructuredBuffer<Vertex> InputVertexBuffer : register(t0);
RWStructuredBuffer<VS_IN> OutputVertexBuffer : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x; // �X���b�hid�𒸓_id�Ƃ���
    
    if (index >= maxVertexCount)
        return;
       
    Vertex inputVertex = InputVertexBuffer[index];
    
    // �X�L�j���O�v�Z����
    float4x4 skinMatrix;
   
    skinMatrix = mul(boneMatrix[inputVertex.BoneIndices[0]], inputVertex.BoneWeights[0]);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices[1]], inputVertex.BoneWeights[1]);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices[2]], inputVertex.BoneWeights[2]);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices[3]], inputVertex.BoneWeights[3]);

    // �o��
    VS_IN outputVertex;
    outputVertex.Position = mul(inputVertex.position, skinMatrix);

    // �@���p�Ɉړ�������
    skinMatrix[0][3] = 0.0f;
    skinMatrix[1][3] = 0.0f;
    skinMatrix[2][3] = 0.0f;
    
    //skinMatrix = transpose(skinMatrix);
    outputVertex.Normal = mul(inputVertex.normal, skinMatrix);
    outputVertex.Normal = inputVertex.normal;
    
    outputVertex.Diffuse = inputVertex.diffuse;
    outputVertex.TexCoord = inputVertex.uv;

    // �i�[�p�o�b�t�@�ɓ���Ă���
    OutputVertexBuffer[index] = outputVertex;
}
