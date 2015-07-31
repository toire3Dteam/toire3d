#include	"iextreme.h"
#include	"BasePlayer.h"


//******************************************************************************
//
//		初期化(キャラ共通情報はここで初期化し、objとかscaleみたいなキャラ固有は各自で設定でお願いします)
//
//******************************************************************************
void BasePlayer::Initialize(int pad)
{
	hp = 3;

	mode = process = 0;
	attack_again_time = 180;

	move = Vector3(.0f, .0f, .0f);

	died = isJump = isLand = invincible = false;

	mode = MOVE;

	//====================================================
	// プレイヤーの番号によって、出現位置と向きを設定
	pos.z = angle.x = angle.z = .0f;
	switch (pad)
	{
	case 0:
		pos.x = .0f;
		pos.y = .0f;
		angle.y = RIGHT_ANGLE;
		break;

	case 1:
		pos.x = 50.0f;
		pos.y = .0f;
		angle.y = LEFT_ANGLE;
		break;

	case 2:
		pos.x = -50.0f;
		pos.y = .0f;
		angle.y = RIGHT_ANGLE;
		break;

	case 3:
		pos.x = 100.0f;
		pos.y = .0f;
		angle.y = LEFT_ANGLE;
		break;
	}

	// 下記の情報はキャラごとに設定すると良いかもしれない。
	//NEAR_AGAIN = 60;
	//FAR_AGAIN = 30;
	//CHARA_AGAIN = 90;
	NEAR_AGAIN = 0;
	FAR_AGAIN = 0;
	CHARA_AGAIN = 0;
	speed = 5.0f;
	scale = 1.0f;
	gravity = 1.0f;


	// この関数の呼び出し後、各自メッシュををnewしてください。
	obj = nullptr;

}

void BasePlayer::Respawn(int pad)
{
	hp = 3;

	mode = process = 0;

	move = Vector3(.0f, .0f, .0f);

	died = isLand = false;

	// ジャンプ封印
	isJump = true;

	// 無敵状態で復活するべきか
	Set_invincible(60);


	mode = MOVE;

	//====================================================
	// プレイヤーの番号によって、出現位置と向きを設定
	pos.z = angle.x = angle.z = .0f;
	switch (pad)
	{
	case 0:
		pos.x = .0f;
		pos.y = 250.0f;
		angle.y = RIGHT_ANGLE;
		break;

	case 1:
		pos.x = 50.0f;
		pos.y = 250.0f;
		angle.y = LEFT_ANGLE;
		break;

	case 2:
		pos.x = -50.0f;
		pos.y = 250.0f;
		angle.y = RIGHT_ANGLE;
		break;

	case 3:
		pos.x = 100.0f;
		pos.y = 250.0f;
		angle.y = LEFT_ANGLE;
		break;
	}

	// 待機状態で復活
	Set_motion(0);
}

//******************************************************************************
//
//		更		新
//
//******************************************************************************
void BasePlayer::Update(int pad)
{
	// 無敵時間更新
	if (invincible){
		if (invincible_time-- < 0){
			invincible_time = 0;
			invincible = false;
		}
	}

	obj->Animation();
	obj->SetScale(scale);
	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->Update();
}

void BasePlayer::Compute_move()
{
	// 移動量を加える
	pos += move;


	// ジャンプ中の処理
	if (!isLand){

		// 着地
		if (pos.y <= .0f){
			move.y = .0f;
			pos.y = .0f;
			isJump = false;
			isLand = true;
			if (obj->GetMotion(0) < 3)
				Set_motion(3);
		}

		// まだ空中(重力を加える)
		else
			move.y -= gravity;
	}


	// 誤差修正(-1～1を0にする)
	if (move.x <= 1.0f && move.x >= -1.0f)
		move.x = .0f;

	// 摩擦
	else if (move.x > 0)move.x -= 1.0f;	// 右にいくと空気が０ににしようと頑張る。
	else if (move.x < 0)move.x += 1.0f;	// 左にいくと空気が０ににしようと頑張る。
}

void BasePlayer::Near_attack()
{

}

void BasePlayer::Far_attack()
{

}

void BasePlayer::Jump()
{

}

void BasePlayer::Burst()
{

}

void BasePlayer::Move()
{

}

void BasePlayer::Control(int pad)
{
	//===================================
	// 移動
	if (KEY_Get(KEY_LEFT, pad))
	{

	}
	else if (KEY_Get(KEY_RIGHT, pad))
	{

	}

	//===================================
	// ジャンプ
	if (KEY_Get(KEY_C, pad) == 3 && isJump == false)
	{

	}

	//===================================
	// 近距離攻撃
	if (KEY_Get(KEY_B, pad) == 3)
	{

	}


	//===================================
	// 遠距離攻撃
	if (KEY_Get(KEY_A, pad) == 3)
	{

	}



	//===================================
	// キャラ固有攻撃
	if (KEY_Get(KEY_D, pad) == 3)
	{

	}

}

void BasePlayer::Die()
{

}

//******************************************************************************
//
//		描		画
//
//******************************************************************************
void BasePlayer::Render()
{
	// 無敵状態ならちかちか
	if (invincible){
		static int flashing = 0;
		flashing++;
		if (flashing < 3) {
			obj->Render();
		}
		else if (flashing > 6){
			flashing = 0;
		}

	}
	else
		obj->Render();
}