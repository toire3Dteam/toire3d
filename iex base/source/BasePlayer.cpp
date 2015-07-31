#include	"iextreme.h"
#include	"BasePlayer.h"


//******************************************************************************
//
//		������(�L�������ʏ��͂����ŏ��������Aobj�Ƃ�scale�݂����ȃL�����ŗL�͊e���Őݒ�ł��肢���܂�)
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
	// �v���C���[�̔ԍ��ɂ���āA�o���ʒu�ƌ�����ݒ�
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

	// ���L�̏��̓L�������Ƃɐݒ肷��Ɨǂ���������Ȃ��B
	//NEAR_AGAIN = 60;
	//FAR_AGAIN = 30;
	//CHARA_AGAIN = 90;
	NEAR_AGAIN = 0;
	FAR_AGAIN = 0;
	CHARA_AGAIN = 0;
	speed = 5.0f;
	scale = 1.0f;
	gravity = 1.0f;


	// ���̊֐��̌Ăяo����A�e�����b�V������new���Ă��������B
	obj = nullptr;

}

void BasePlayer::Respawn(int pad)
{
	hp = 3;

	mode = process = 0;

	move = Vector3(.0f, .0f, .0f);

	died = isLand = false;

	// �W�����v����
	isJump = true;

	// ���G��Ԃŕ�������ׂ���
	Set_invincible(60);


	mode = MOVE;

	//====================================================
	// �v���C���[�̔ԍ��ɂ���āA�o���ʒu�ƌ�����ݒ�
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

	// �ҋ@��Ԃŕ���
	Set_motion(0);
}

//******************************************************************************
//
//		�X		�V
//
//******************************************************************************
void BasePlayer::Update(int pad)
{
	// ���G���ԍX�V
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
	// �ړ��ʂ�������
	pos += move;


	// �W�����v���̏���
	if (!isLand){

		// ���n
		if (pos.y <= .0f){
			move.y = .0f;
			pos.y = .0f;
			isJump = false;
			isLand = true;
			if (obj->GetMotion(0) < 3)
				Set_motion(3);
		}

		// �܂���(�d�͂�������)
		else
			move.y -= gravity;
	}


	// �덷�C��(-1�`1��0�ɂ���)
	if (move.x <= 1.0f && move.x >= -1.0f)
		move.x = .0f;

	// ���C
	else if (move.x > 0)move.x -= 1.0f;	// �E�ɂ����Ƌ�C���O�ɂɂ��悤�Ɗ撣��B
	else if (move.x < 0)move.x += 1.0f;	// ���ɂ����Ƌ�C���O�ɂɂ��悤�Ɗ撣��B
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
	// �ړ�
	if (KEY_Get(KEY_LEFT, pad))
	{

	}
	else if (KEY_Get(KEY_RIGHT, pad))
	{

	}

	//===================================
	// �W�����v
	if (KEY_Get(KEY_C, pad) == 3 && isJump == false)
	{

	}

	//===================================
	// �ߋ����U��
	if (KEY_Get(KEY_B, pad) == 3)
	{

	}


	//===================================
	// �������U��
	if (KEY_Get(KEY_A, pad) == 3)
	{

	}



	//===================================
	// �L�����ŗL�U��
	if (KEY_Get(KEY_D, pad) == 3)
	{

	}

}

void BasePlayer::Die()
{

}

//******************************************************************************
//
//		�`		��
//
//******************************************************************************
void BasePlayer::Render()
{
	// ���G��ԂȂ炿������
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