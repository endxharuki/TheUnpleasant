#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "slashFirst.h"
#include "texturePool.h"
#include "capsuleColliderComponent.h"
#include "textDamage.h"
#include <random>
#include "explosion.h"

std::random_device rd;
std::mt19937 gen(rd());

float random(float low, float high)
{
	std::uniform_real_distribution<>dist(low, high);
	return dist(gen);

}


void SlashFirst::Init()
{

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-3.0f, 3.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(3.0f, 3.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-3.0f, -3.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(3.0f, -3.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//�G�t�F�N�g�e�N�X�`���̓o�^
	AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("slashFirst"));
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	GetComponent<Transform2DComponent>()->SetIsBilboad(true);

	for (auto com : ComponentList) {
		com->Init();
	}
	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(1.0f, 1.0f, 1.0f));

	length = 2.0f;

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 playerRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();
	XMFLOAT3 texpos = { playerPos.x + (length * sinf(playerRot.y)),playerPos.y + 1.5f ,playerPos.z + (length * cosf(playerRot.y)) };
	XMFLOAT3 capsulePos = { playerPos.x + (length * sinf(playerRot.y)),playerPos.y + 1.5f ,playerPos.z + (length * cosf(playerRot.y)) };

	AddComponent<CapsuleColliderComponent>()->SetTransform(capsulePos,XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.5f, 1.0f, 0.5f));
	GetComponent<CapsuleColliderComponent>()->NoUpdate();

	GetComponent<CapsuleColliderComponent>()->SetRotationAngle(XMFLOAT3(90.0f,0.0f,0.0f));
	XMFLOAT3 r = GetComponent<CapsuleColliderComponent>()->GetRotation();

	GetComponent<CapsuleColliderComponent>()->SetRotation(XMFLOAT3(r.x,playerRot.y,r.z));

	XMFLOAT3 vector = { capsulePos.x - playerPos.x,(capsulePos.y - playerPos.y) - 1.0f,capsulePos.z - playerPos.z };

	////�΂߂ɂ���
	float Rot = PI / 4;

	//�����W�^�C�v
	GetComponent<Collider>()->RotateAroundInit(playerPos);
	GetComponent<Collider>()->RotateAround(-Rot, -Rot);

	XMFLOAT3 capsule = GetComponent<Collider>()->GetPosition();
	
	capsule = GetComponent<Collider>()->GetRotatePosition(playerPos);

	GetComponent<CapsuleColliderComponent>()->SetPosition(capsule);
	
	//�J�v�Z���̐ݒ�
	GetComponent<CapsuleColliderComponent>()->SetCenterPosLnegth(1.0f);

	//�J�v�Z����ݒu
	GetComponent<CapsuleColliderComponent>()->MoveCollision();

	GetComponent<CapsuleColliderComponent>()->DrawImGui();

	GetComponent<CapsuleColliderComponent>()->OnDraw();


	GetComponent<Transform2DComponent>()->SetPosition(texpos);

	//���b�g�̏�����
	rot = 0.0f;

	m_AttackPoint = 120;

	Tag = "Attack";

}

void SlashFirst::Uninit()
{
	for (auto obj : m_HitObjectList)
	{
		obj->SetHit(false);
		obj->SetHitCount(0);
	}

	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void SlashFirst::Update()
{
	//�q�b�g�X�g�b�v���͏��������Ȃ�
	if (m_ParentObject->GetHitStopState() == true) return;

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 playerRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();

	XMFLOAT3 capsulePos = GetComponent<Collider>()->GetPosition();
	
	//�����W�^�C�v
	float rotScale = PI * 0.05f;

	XMFLOAT3 p = GetComponent<Collider>()->GetPosition();
	GetComponent<Collider>()->RotateAround(rotScale, rotScale);

	p = GetComponent<Collider>()->GetRotatePosition(playerPos);

	GetComponent<CapsuleColliderComponent>()->SetPosition(p);

	GetComponent<CapsuleColliderComponent>()->MoveCollision();

	HitAttackCollision();

	//�A�j���[�V����
	{

		VERTEX_3D vertex[4];
		for (int i = 0;i < 4;i++) {
			vertex[i] = GetComponent<Transform2DComponent>()->GetVertex(i);
		}

		float x = 1.0f / 9.0f;
		float y = 1.0f;
		float TexX = frameCount % 9 * x;
		float TexY = 0.0f;

		vertex[0].TexCoord = XMFLOAT2(TexX, TexY);
		vertex[1].TexCoord = XMFLOAT2(TexX + x, TexY);
		vertex[2].TexCoord = XMFLOAT2(TexX, TexY + y);
		vertex[3].TexCoord = XMFLOAT2(TexX + x, TexY + y);
		GetComponent<Transform2DComponent>()->SetVertex(vertex);

		frameCount++;
	}

	if (frameCount >= 10.0f) isDelete = true;

	DeleteRecest();

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void SlashFirst::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}

void SlashFirst::HitAttackCollision()
{
	XMFLOAT3 pos = GetComponent<Collider>()->GetPosition();

	std::tuple<bool, GameObject*, std::list<GameObject*>> date = GetComponent<CapsuleColliderComponent>()->GetCollision();

	if (std::get<0>(date) == true)
	{
		std::list<GameObject*> objList = std::get<2>(date);

		bool hit = false;

		for (auto obj : objList)
		{
			if (obj->GetTag() == "Enemy")
			{
				if (obj->GetStates()->hit == false)
				{
					m_ParentObject->SetHitStopState(true);

					Explosion* explosion = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<Explosion>(1);
					//�v���C���[��o�^
					explosion->AddParentGameObject(m_ParentObject);
					Transform * ex =  explosion->GetComponent<Transform>();
					
					ex->SetPosition(pos);
					ex->SetSize(XMFLOAT3(1.0f, 1.0f, 1.0f));

					Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
					//MP���񕜂�����
					States* sta = player->GetStates();
		
					float mp = sta->MP + 1.0f;
					if (mp > 50.0f)
					{
						mp = 50.0f;
					}

					player->SetStates(sta->HP, mp, sta->hit, sta->hitCount);

					int count = obj->GetStates()->hitCount;
					count++;

					obj->SetHitCount(count);
					if (count >= 2) obj->SetHit(true);
					m_HitObjectList.push_back(obj);

					XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
					XMFLOAT3 playerRot = player->GetComponent<Transform>()->GetRotation();

					pos.y += 2.0f;

					XMFLOAT3 anglePos = GetComponent<Collider>()->GetAddAnglePosition(playerRot, XMFLOAT3(1.0f, 0.0f, 0.0f));
					anglePos = { anglePos.x * 0.5f,anglePos.y * 0.5f ,anglePos.z * 0.5f };
					
					int digits = std::log10(m_AttackPoint);

					digits++;

					//HP�����炷
					float hp = obj->GetStates()->HP;
					hp -= m_AttackPoint;
					obj->SetHp(hp);

					//�_���[�W�\���𐶐�
					TextAttackPoint(digits);

					hit = true;

				}
			}

			if (hit == true) m_HitCount++;
			
		}
	}


}

void SlashFirst::TextAttackPoint(int n)
{
	//�v���C���[���擾
	Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
	Transform* playerTrans = player->GetComponent<Transform>();
	XMFLOAT3 playerRot = playerTrans->GetRotation();
	XMFLOAT3 playerFoward = playerTrans->GetFoward();
	XMFLOAT3 playerRight = playerTrans->GetRight();

	float textToWiedth = 0.5f;


	XMFLOAT3 anglePos = GetComponent<Collider>()->GetPosition();
	XMFLOAT3 textPos = { anglePos.x - playerFoward.x,anglePos.y - playerFoward.y ,anglePos.z - playerFoward.z };
	

	TextDamage* text[4];
	
	int num = 0;

	for (int i = 0; i < n; i++)
	{
		if (i == 0) num = m_AttackPoint % 10;
		if (i == 1) num = (m_AttackPoint / 10) % 10;
		if (i == 2) num = (m_AttackPoint / 100) % 10;
		if (i == 3) num = m_AttackPoint / 1000;

		text[i] = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<TextDamage>(2);
		text[i]->GetComponent<Transform>()->SetTransform(
			textPos, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f));
		text[i]->SetTextUV(num);

		textPos = { textPos.x - (playerRight.x * textToWiedth),textPos.y ,textPos.z - (playerRight.z * textToWiedth) };

	}
	
}
