#pragma once
#include "gameObject.h"
#include "collider.h"
#include <tuple>
#include <cmath>

class BoxColliderComponent : public Collider
{
private:


	Box m_OBB =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}}
	};


public:

	BoxColliderComponent() = default;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void MoveCollision() override;
	Box GetOBB() const
	{
		return m_OBB;
	}
	

	std::tuple<bool, GameObject*,std::list<GameObject*>> GetCollision()  override ;

	
	

};


