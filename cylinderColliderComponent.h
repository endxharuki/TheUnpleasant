#pragma once
//#include "component.h"
#include "gameObject.h"
#include "collider.h"
#include <tuple>

class CylinderColliderComponent : public Collider
{
private:

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


	bool IsCollision();
	std::tuple<bool, GameObject*,std::list<GameObject*>> GetCollision() override;

};

