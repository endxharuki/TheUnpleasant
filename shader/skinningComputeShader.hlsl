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
    float4x4 boneMatrix[256]; // 各ボーンの変換行列
    uint maxVertexCount; // モデルの最大頂点数  
};

StructuredBuffer<Vertex> InputVertexBuffer : register(t0);
RWStructuredBuffer<VS_IN> OutputVertexBuffer : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x; // スレッドidを頂点idとする
    
    if (index >= maxVertexCount)
        return;
       
    Vertex inputVertex = InputVertexBuffer[index];
    
    // スキニング計算処理
    float4x4 skinMatrix;
   
    skinMatrix = mul(boneMatrix[inputVertex.BoneIndices[0]], inputVertex.BoneWeights[0]);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices[1]], inputVertex.BoneWeights[1]);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices[2]], inputVertex.BoneWeights[2]);
    skinMatrix += mul(boneMatrix[inputVertex.BoneIndices[3]], inputVertex.BoneWeights[3]);

    // 出力
    VS_IN outputVertex;
    outputVertex.Position = mul(inputVertex.position, skinMatrix);

    // 法線用に移動を消す
    skinMatrix[0][3] = 0.0f;
    skinMatrix[1][3] = 0.0f;
    skinMatrix[2][3] = 0.0f;
    
    //skinMatrix = transpose(skinMatrix);
    outputVertex.Normal = mul(inputVertex.normal, skinMatrix);
    outputVertex.Normal = inputVertex.normal;
    
    outputVertex.Diffuse = inputVertex.diffuse;
    outputVertex.TexCoord = inputVertex.uv;

    // 格納用バッファに入れていく
    OutputVertexBuffer[index] = outputVertex;
}
