#include "main.h"
#include "cylinderColliderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "transform3DComponent.h"


void CylinderColliderComponent::Init()
{
	m_Position = GetGameObject()->GetComponent<Transform3DComponent>()->GetPosition();
	m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();
	m_Rotation = GetGameObject()->GetComponent<Transform3DComponent>()->GetRotation();
}

void CylinderColliderComponent::Uninit()
{
}

void CylinderColliderComponent::Update()
{
	m_Position = GetGameObject()->GetComponent<Transform3DComponent>()->GetPosition();
	m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();
	m_Rotation = GetGameObject()->GetComponent<Transform3DComponent>()->GetRotation();
}

void CylinderColliderComponent::Draw()
{
}


//あった判定と最初に当たったオブジェクトと当たっているオブジェクトリストを返します
std::tuple<bool, GameObject*, std::list<GameObject*>> CylinderColliderComponent::GetCollision()
{
	int objSize = 0;
	std::list<GameObject*> objList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

	//自分以外のコライダーのポジションとサイズ
	XMFLOAT3 pos;
	XMFLOAT3 size;

	for (auto obj : Scene::GetInstance()->GetScene<GameScene>()->GetGameObjectList(1))
	{
		if (obj->GetComponent<Collider>() == nullptr) {
			continue;
		}
		if (GetGameObject() == obj) {
			continue;
		}
		pos = obj->GetComponent<Collider>()->GetPosition();
		size = obj->GetComponent<Collider>()->GetSize();

		//お互いの距離
		XMFLOAT3 direction;
		direction.x = pos.x - m_Position.x;
		direction.y = pos.y - m_Position.y;
		direction.z = pos.z - m_Position.z;

		float length;
		length = sqrtf(direction.x * direction.x + direction.z * direction.z);


		if (length < size.x)
		{

			objList.push_back(obj);
			objSize = objList.size();
			if (-direction.y > size.y - 0.5f) {
				
			}
		}
	}
	
	if (objSize != 0) 
	{
		auto itr =objList.begin();
		GameObject* gameObject = (*itr);
	
		std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject = std::make_tuple(true,gameObject,objList);
		return OnCollisionObject;
	}
	else if (objSize == 0) 
	{

		std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject = std::make_tuple(false, nullptr, objList);
		return OnCollisionObject;

	}

}
