#include "main.h"
#include "player.h"
#include "transform3DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "PlayerCamera.h"
#include "boxColliderComponent.h"
#include "input.h"
#include "audio.h"
#include "resultScene.h"
#include "meshField.h"
#include "transform3DAnimationComponent.h"
#include "capsuleColliderComponent.h"
#include "slashFirst.h"
#include "slashSecound.h"
#include "circle2DCollider.h"
#include "playerIdleState.h"
#include "playerMoveState.h"
#include "playerJumpState.h"
#include "playerAttack1State.h"
#include "playerAttack2State.h"
#include "playerSkill1State.h"
#include "timer.h"
#include "sord.h"


void Player::Init()
{
	Transform3DAnimationComponent* trans = AddComponent<Transform3DAnimationComponent>();

	GetComponent<Transform3DAnimationComponent>()->AddModelData("asset\\Model\\PlayerTest.fbx");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Idle.fbx", "Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\RunTest.fbx", "Run");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Slash1.fbx", "Slash1");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Slash2.fbx", "Slash2");


	GetComponent<Transform>()->SetSize(XMFLOAT3(0.01f, 0.01f, 0.01f));

	/*GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");*/

	AddComponent<CapsuleColliderComponent>();
	GetComponent<Collider>()->SetSize(XMFLOAT3(0.5f, 0.5f, 0.5f));
	GetComponent<CapsuleColliderComponent>()->SetCenterPosLnegth(0.5f);

	//コライダーを描画
	GetComponent<CapsuleColliderComponent>()->OnDraw();
	//デバッグを表示
	GetComponent<CapsuleColliderComponent>()->OnImGui();

	AddComponent<Audio>()->InitMaster();
	GetComponent<Audio>()->Load("asset\\Audio\\wan.wav");

	//このオブジェクトは動的
	m_GameObjectKind = Dynamic;
	m_Weight = 10;

	Velocity = 1.0f;

	//HPとMP
	m_States =
	{
		500.0f,
		50.0f,
		false,
		0
	};

	Tag = "Player";

	RegisterState(StateType::Idle, std::make_shared<PlayerIdleState>());
	RegisterState(StateType::Move, std::make_shared<PlayerMoveState>());
	RegisterState(StateType::Jump, std::make_shared<PlayerJumpState>());
	RegisterState(StateType::Attack1, std::make_shared<PlayerAttack1State>());
	RegisterState(StateType::Attack2, std::make_shared<PlayerAttack2State>());
	RegisterState(StateType::Skill1, std::make_shared<PlayerSkill1State>());

	//アイドル状態にしておく
	ChangeState(StateType::Idle);
	m_PlayerTime = new Timer; 


	XMFLOAT3 pos = trans->GetSize();

	//武器を追加
	Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<Sord>(1);

	//シャドウボリューム
	m_Shadow = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<Polygon2D>(2);
	m_ShadowVolume = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<ShadowVolume>(2);

}

void Player::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
	
}

void Player::Update()
{
	m_PlayerTime->Update();

	if (m_HitStop == true)
	{
		m_HitStopSecond += m_PlayerTime->GetDeltaTime();
		if (m_HitStopSecond >= 0.05f)
		{
			m_HitStop = false;
			m_HitStopSecond = 0.0f;
		}
		else return;
	}

	ComponentListSort();

	//プレイヤー操作
	PlayerControl();

	//プレイヤーの当たり判定と移動の反映
	PlayerCollision();

	if (m_States.HP <= 0.0f)
	{

		GetComponent<Collider>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));
		GetComponent<Transform>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));

		//HPとMP
		m_States =
		{
			500.0f,
			50.0f,
			false,
			0
		};
	}

	//プレイヤーを動かしてから各コンポーネントのアップデート
	for (auto com : ComponentList)
	{
		com->Update();
	}


	//MP自動回復
	if (m_States.MP < 50.0f)
	{
		m_States.MP += 0.01f;
	}
	if (m_States.MP > 50.0f)
	{
		m_States.MP = 50.0f;
	}
	
	//影のポジション
	XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
	m_ShadowVolume->SetPosition(pos);


}

void Player::Draw()
{
	for (auto com : ComponentList)
	{
		com->Draw();
	}

	DrawImGui();

	//XMMATRIX chaildScale, chaildRot, childTrans, childWorld;
	//
	//chaildRot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	//chaildScale = XMMatrixScaling(1.0f / m_Size.x, 1.0f / m_Size.y, 1.0f / m_Size.z);
	//childTrans = XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	//childWorld = childTrans * chaildScale *  world;
	////子のワールド　＝子のローカル * 親のワールド

	//Renderer::SetWorldMatrix(childWorld);

	/*m_CildModel->Draw();*/
}

void Player::PlayerControl()
{
	XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	XMFLOAT3 TransRot = GetComponent<Transform>()->GetRotation();
	XMFLOAT3 pos = GetComponent<Collider>()->GetPosition();
	XMFLOAT3 rot = GetComponent<Collider>()->GetRotation();
	XMFLOAT3 vel = GetComponent<Transform>()->GetVelocity();

	float posRot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 cameraRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<PlayerCamera>()->GetComponent<Transform>()->GetRotation();
	float PI = 3.141592;

	//通常スキル
	if (Input::GetInstance()->GetKeyTrigger(VK_SPACE))
	{
		ChangeState(StateType::Attack1);
	}

	if (sonick == false) {

		if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || GetKeyState('A') & 0x8000 || GetKeyState('D') & 0x8000)
		{
			ChangeState(StateType::Move);
		}
		else {
			ChangeState(StateType::Idle);
		}

	}


	//ジャンプ処理
	if (GetKeyState('Q') & 0x8000 && m_IsGravity == false) {

		ChangeState(StateType::Jump);
		//GetComponent<Audio>()->Play(false);

	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		ChangeState(StateType::Skill1);
	}
	
	
	//重力処理
	if (m_CurrentState != nullptr)
	{
		if (m_CurrentState->GetName() == StateType::Jump)
		{
			m_IsGravity = false;
		}
	}
	if (m_IsGravity == true) {
		TransPos.y -= m_GravityScale;
		m_GravityScale += 0.01f;
	}
	else {
		m_GravityScale = 0.05f;
	}

	if (Input::GetInstance()->GetKeyTrigger('G'))
	{
		m_IsGravity = !m_IsGravity;
	}

	GetComponent<Collider>()->SetPosition(TransPos);
	GetComponent<Collider>()->SetRotation(rot);
	GetComponent<Collider>()->SetVelocity(vel);
	//GetComponent<CapsuleColliderComponent>()->MoveCollision();

	GetComponent<Transform>()->SetPosition(TransPos);
	GetComponent<Transform>()->SetRotation(rot);
	GetComponent<Transform>()->SetVelocity(vel);


	if (m_CurrentState == nullptr) return;
	m_CurrentState->Update();

	
}

void Player::PlayerCollision()
{
	Transform* trans = GetComponent<Transform>();
	Collider* col = GetComponent<Collider>();

	XMFLOAT3 TransPos = trans->GetPosition();
	XMFLOAT3 TransRot = trans->GetRotation();
	XMFLOAT3 pos = col->GetPosition();
	XMFLOAT3 rot = col->GetRotation();
	float posRot = trans->GetRot();

	float hight = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<MeshField>()->GetHeight(pos);

	
	if (NoCollicion == false)
	{
		if (col->GetIsCollision() == true)
		{
			m_IsGravity = true;
			if(m_CurrentState->GetName() != StateType::Jump)
			{
				if (TransPos.y <= hight + 0.1f) {
					TransPos.y = hight;
					m_IsGravity = false;
				}
			}

			trans->SetPosition(TransPos);
			trans->SetRotation(rot);
			trans->SetRot(posRot);
		}
		else 
		{
		
			m_IsGravity = true;

			if (m_CurrentState->GetName() != StateType::Jump) 
			{
				if (TransPos.y <= hight + 0.1f) {
					TransPos.y = hight;
					m_IsGravity = false;
				}
			}
			else 
			{
				m_IsGravity = false;
			}

			trans->SetPosition(TransPos);
			trans->SetRotation(rot);
			trans->SetRot(posRot);
		}
	

	}
}
float currentTime = 0.0f;

void Player::DrawImGui()
{

	XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	XMFLOAT3 TransRot = GetComponent<Transform>()->GetRotation();
	XMFLOAT3 ColliderPos = GetComponent<Collider>()->GetPosition();
	XMFLOAT3 CroiderRot = GetComponent<Collider>()->GetRotation();
	currentTime += m_PlayerTime->GetDeltaTime();

	{

		ImguiManager* imgui = Scene::GetInstance()->GetScene<GameScene>()->GetImguiManager();

		ImGui::Begin("Player");                          // Create a window called "Hello, world!" and append into it.              // Display some text (you can use a format strings too)

		static bool draw;

		ImGui::Checkbox("DrawCollider", &draw);

		GetComponent<CapsuleColliderComponent>()->CapsuleDrawState(draw);

		ImGui::Text("playerPos , x = %.1f , y = %.1f , z = %.1f", TransPos.x, TransPos.y, TransPos.z);

		static  float add[3] = {0.0f,0.0f,0.0f};

		ImGui::SliderFloat3("AddPos", add, -1.0f, 1.0f);

		GetComponent<Collider>()->SetAddPosition(XMFLOAT3(add[0],add[1],add[2]));

		ImGui::Text("PlayerHP = %.1f", m_States.HP);

		ImGui::Text("FPS = %.1f", imgui->FrameRate);

		ImGui::Text("deltaTime = %.1f", currentTime);

		ImGui::End();

	}

}




