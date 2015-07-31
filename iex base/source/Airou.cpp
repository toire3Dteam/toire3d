#include	"iextreme.h"

#include	"BasePlayer.h"
#include	"Airou.h"


// コメントアウトするとデバッグifndefに入れるようになる
#define DEBUG_AIROU


//******************************************************************************
//
//		初期化と解放
//
//******************************************************************************
Airou::Airou(int pad)
{
	// この関数で、座標とか角度とかキャラ共通系の初期化を行う
	BasePlayer::Initialize(pad);

	// objとscaleとかはキャラごとに設定。
	obj = new iex3DObj2("DATA/CHR/アイルー/airou_toire.IEM", 1);
	speed = 5.0f;
	scale = 5.0f;
	gravity = 1.0f;

	Set_motion(0);
}

Airou::~Airou()
{
	SAFE_DELETE(obj);
}


//******************************************************************************
//
//		更		新
//
//******************************************************************************
void Airou::Update(int pad)
{
	//// モード分岐
	switch (mode){
	case MOVE:

		Control(pad);
		break;

	case JUMP:
		Control(pad);
		Jump();
		break;

	case NEAR_ATTACK:
		Control(pad);
		Near_attack();
		break;

	case CHARA_ATTACK:
		Chara_attack();
		break;

	case BURST:
		Burst();
		break;

	case DIE:
		Die();
		break;
	}


	Compute_move();

	BasePlayer::Update(pad);
}

void Airou::Control(int pad)
{
	//===================================
	// 移動
	if (KEY_Get(KEY_LEFT, pad))
	{
		move.x = -speed;
		angle.y = LEFT_ANGLE;
		if (obj->GetMotion(0) == 0)
			Set_motion(1);
	}
	else if (KEY_Get(KEY_RIGHT, pad))
	{
		move.x = speed;
		angle.y = RIGHT_ANGLE;
		if (obj->GetMotion(0) == 0)
			Set_motion(1);
	}
	else
	{
		if (obj->GetMotion(0) == 1)
			Set_motion(0);
	}

	//===================================
	// ジャンプ
	if (KEY_Get(KEY_C, pad) == 3 && isJump == false)
	{
		move.y = 15.0f;
		isJump = true;
		isLand = false;

		if (obj->GetMotion(0) < 2)
			Set_motion(2);
	}

	//===================================
	// 近距離攻撃
	if (KEY_Get(KEY_B, pad) == 3)
	{
		if (attack_again_time >= NEAR_AGAIN){
			Set_mode(NEAR_ATTACK);
			attack_again_time = 0;
		}
	}


	//===================================
	// 遠距離攻撃
	if (KEY_Get(KEY_A, pad) == 3)
	{
		// 近距離攻撃をしていなかったら
		if (mode != NEAR_ATTACK){
			if (attack_again_time >= FAR_AGAIN){
				Set_motion(11);
				attack_again_time = 0;
			}
		}
	}


	//===================================
	// キャラ固有攻撃
	if (KEY_Get(KEY_D, pad) == 3)
	{
		if (attack_again_time >= CHARA_AGAIN){
			Set_mode(CHARA_ATTACK);
			attack_again_time = 0;
		}
	}


	//===================================
	// バースト
	if (KEY_Get(KEY_L2, pad) == 3)
	{
		Set_mode(BURST);
	}


	//===================================
	// 死ぬ(デバッグ)
	if (KEY_Get(KEY_ENTER, pad) == 3)
	{
		Set_mode(DIE);
	}

	attack_again_time++;
}

void Airou::Near_attack()
{
	switch (process)
	{
	case 0:
		// 準備
		Set_motion(10);
		process++;

		break;

	case 1:
		if (obj->GetParam(0, 0) == 1)
			process++;

		break;

	case 2:// 攻撃なう
		if (obj->GetParam(0, 0) == 2)	// 攻撃の終了
			process++;

		else{
			// ここで判定呼び出しお願いします
		}
		break;

	case 3:
		if (obj->GetParam(0, 0) == 3) // モード終了
		{
			Set_mode(MOVE);
			gravity = 1.0f;
		}
		break;
	}
}

void Airou::Chara_attack()
{
	switch (process)
	{
	case 0:
		// 準備
		Set_motion(15);
		pos.y += 10.0f;
		isLand = false;
		move = Vector3(0, 0, 0);
		gravity = .0f;
		process++;

		break;

	case 1:
		if (obj->GetParam(0, 0) == 1)
			process++;
		else{
			if (angle.y <= LEFT_ANGLE + .01f && angle.y >= LEFT_ANGLE - .01f) // 左向き
				move.x = -2.0f;
			else // 右向き
				move.x = 2.0f;
		}
		gravity = .0f;

		break;

	case 2:// 攻撃なう
		if (obj->GetParam(0, 0) == 2)	// 攻撃の終了
			process++;

		else{
			// ここで判定呼び出しお願いします


			if (angle.y <= LEFT_ANGLE + .01f && angle.y >= LEFT_ANGLE - .01f) // 左向き
				move.x = -speed * 2.0f;
			else // 右向き
				move.x = speed * 2.0f;
		}
		break;

	case 3:
		if (obj->GetParam(0, 0) == 3) // モード終了
		{
			Set_mode(MOVE);
			gravity = 1.0f;
		}

		else{
			if (angle.y <= LEFT_ANGLE + .01f && angle.y >= LEFT_ANGLE - .01f) // 左向き
				move.x = -speed;
			else // 右向き
				move.x = speed;

			if (isLand)
				move.x *= .5f;		// 地面についてたら少し摩擦を加える

			gravity = .15f;
		}
		break;
	}
}

void Airou::Special_attack()
{

}

void Airou::Burst()
{
	switch (process){
	case 0:
		Set_motion(12);
		process++;
		break;

	case 1:
		if (obj->GetParam(0, 0) == 1){
			Set_motion(0);
			Set_mode(MOVE);
		}
		break;
	}
}

void Airou::Move()
{
	BasePlayer::Move();
}

void Airou::Die()
{
	switch (process)
	{
	case 0:
		Set_motion(5);
		Set_invincible();
		process++;
		break;

	case 1:
		if (!invincible)
		{
			died = true;
		}
		break;
	}
}


//******************************************************************************
//
//		描		画
//
//******************************************************************************
void Airou::Render()
{

#ifndef DEBUG_AIROU
	char str[64];
	sprintf(str, "x:%d", pos.z);
	IEX_DrawText(str, 10, 10, 1000, 20, 0xFF00FF00);
	sprintf(str, "y:%.2f", pos.y);
	IEX_DrawText(str, 10, 30, 1000, 20, 0xFF00FF00);
	sprintf(str, "z:%.2f", pos.z);
	IEX_DrawText(str, 10, 50, 1000, 20, 0xFF00FF00);
#endif

	BasePlayer::Render();
}