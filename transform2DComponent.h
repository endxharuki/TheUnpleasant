#pragma once
#include "transform.h"
#include "renderer.h"
#include "modelRenderer.h"

class Transform2DComponent : public Transform
{

private:

	VERTEX_3D m_Vertex[4] = {};

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_3DTexture{};

	float m_Alpha = 1.0f;
	bool bilboad = false;

	int m_FrameCount = 0;

public:

	Transform2DComponent() = default;

	void AddTextureData(ID3D11ShaderResourceView* texture)
	{
		m_3DTexture = texture;
	}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	VERTEX_3D GetVertex(int i) { return m_Vertex[i]; }
	void SetVertex(VERTEX_3D Vertex[4]);

	void SetIsBilboad(bool isBilboad)
	{
		bilboad = isBilboad;
	}

	void SetAlpha(float a) { m_Alpha = a; }
	float GetAlpha() { return m_Alpha; }

};

