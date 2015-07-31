#include	"iextreme.h"
#include	"system/system.h"

#include	"effect.h"

void Effect::Initialize()
{
	particle = new iexParticle;
	particle->Initialize("DATA\\Effect\\particle.PNG", 2048);
}

void Effect::Release()
{
	particle->Release();
	if (particle != NULL){
		delete particle;
	}
}

void Effect::Effect1(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	for (int i = 0; i < 20; i++){
		Pos = pos;

		Move.x = rand() % 9 - 4.0f;
		Move.y = rand() % 9 - 4.0f;
		Move.z = rand() % 9 - 4.0f;
		Power.x = .0f;
		Power.y = .0f;
		Power.z = .0f;

		particle->Set(
			0,
			0, 1.0f,
			30, .0f,
			15, .5f,
			&Pos, &Move, &Power, .5f, .2f, .05f, 4.0f, RS_ADD);
	}
}

void Effect::Effect2(const Vector3 &pos)			// 煙エフェクト
{
	Vector3 Pos, Move, Power;

	Pos.x = pos.x + (rand() % 6 - 3);
	Pos.y = pos.y;
	Pos.z = pos.z + (rand() % 6 - 3);
	Move.x = (rand() % 3 - 1.5f)*0.1f;
	Move.y = (rand() % 3 + 1.0f) * 0.1f;
	Move.z = (rand() % 3 - 1.5f)*0.1f;
	Power.x = .0f;
	Power.y = .001f;
	Power.z = .0f;
	particle->Set(5, 0, .5f, 100, .0f, 50, .25f, &Pos, &Move, &Power, .1f, .075f, .05f, 5.0f, RS_COPY);
}

void Effect::Effect3(const Vector3 &pos)			//エフェクト
{
	Vector3 Pos, Move, Power;

	Pos.x = pos.x + (rand() % 6 - 3);
	Pos.y = pos.y;
	Pos.z = pos.z + (rand() % 6 - 3);
	Move.x = (rand() % 3 - 1.5f)*0.1f;
	Move.y = (rand() % 3 + 1.0f) * 0.1f;
	Move.z = (rand() % 3 - 1.5f)*0.1f;
	Power.x = .0f;
	Power.y = .001f;
	Power.z = .0f;
	particle->Set(15, 0, .5f, 100, .0f, 50, .25f, &Pos, &Move, &Power, .5f, .2f, .05f, 5.0f, RS_ADD);
}

void Effect::Update()
{
	particle->Update();
}

void Effect::Render()
{
	particle->Render();
}

