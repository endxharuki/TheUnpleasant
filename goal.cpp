#include "main.h"
#include "goal.h"
#include "transform3DComponent.h"
#include "boxColliderComponent.h"


void Goal::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\Model\\Block.obj");
	GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 220.0f));
	GetComponent<Transform3DComponent>()->SetSize(XMFLOAT3(10.0f, 10.0f, 10.0f));

	AddComponent<BoxColliderComponent>();

	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	GetComponent<BoxColliderComponent>()->SetSize(XMFLOAT3(size.x / 2, size.y / 2, size.z / 2));
	Tag = "Goal";

}

void Goal::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void Goal::Update()
{

	XMFLOAT3 ColiederOldPos = GetComponent<BoxColliderComponent>()->GetPosition();

	XMFLOAT3 TransformPos = GetComponent<Transform3DComponent>()->GetPosition();
	XMFLOAT3 ColiederPos = GetComponent<BoxColliderComponent>()->GetPosition();
	
	GetComponent<BoxColliderComponent>()->SetPosition(TransformPos);
	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	GetComponent<BoxColliderComponent>()->SetSize(XMFLOAT3(size.x / 2, size.y / 2, size.z / 2));

	/*GetComponent<BoxColliderComponent>()->SetPosition(ColiederPos);*/

	for (auto com : ComponentList)
	{
		com->Update();

	}
}

void Goal::Draw()
{
	
	for (auto com : ComponentList)
	{
		com->Draw();
	}
}

