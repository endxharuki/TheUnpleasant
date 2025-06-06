#include "main.h"
#include "house.h"
#include "transform3DComponent.h"
#include "boxColliderComponent.h"


void House::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\Model\\HouseModel\\House_3.obj");
	AddComponent<BoxColliderComponent>();

	GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	GetComponent<Transform3DComponent>()->SetSize(XMFLOAT3(0.10f, 0.10f, 0.10f));

	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	GetComponent<BoxColliderComponent>()->SetSize(XMFLOAT3(size.x / 2, size.y / 2, size.z / 2));
	Tag = "Block";

}

void House::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void House::Update()
{

	XMFLOAT3 ColiederOldPos = GetComponent<BoxColliderComponent>()->GetPosition();

	XMFLOAT3 TransformPos = GetComponent<Transform3DComponent>()->GetPosition();
	XMFLOAT3 ColiederPos = GetComponent<BoxColliderComponent>()->GetPosition();
	
	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	GetComponent<BoxColliderComponent>()->SetSize(XMFLOAT3(size.x / 2, size.y / 2, size.z / 2));

	GetComponent<BoxColliderComponent>()->SetPosition(ColiederPos);

	for (auto com : ComponentList)
	{
		com->Update();

	}
}

void House::Draw()
{
	
	for (auto com : ComponentList)
	{
		com->Draw();
	}
}

