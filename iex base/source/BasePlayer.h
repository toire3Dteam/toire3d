#ifndef CHARABASE_H_
#define CHARABASE_H_

//******************************************************************************
//
//
//		Player��{�N���X
//
//
//******************************************************************************

class BasePlayer
{
protected:

	int hp;					/*	�Ђ��Ƃۂ���Ƃ����ӂ�����Ȃ炫��������Ƃق����傭��	*/
	int mode;				/*	Update�֐���switch����Ŏg��(state�p�^�[��)			*/
	int process;			/*	Update�֐��ŌĂяo���֐��̒��Ŏg��(mode�̒���state)	*/
	int attack_again_time;	/*	�U���ɕK�v�ȑ҂�����(�������͂������Ă邵�ߋ����͏������Ԃ�������)*/

	/*	�萔�̂悤�Ɏg��(�A�^�b�N�A�Q�C���^�C���ɕK�v�Ȏ���)	*/
	int NEAR_AGAIN;
	int FAR_AGAIN;
	int CHARA_AGAIN;

	float scale;		/*	�X�P�[���l	*/
	float speed;		/*	�X�s�[�h�l	*/

	// �����g��Ȃ�
	//float max_speed;	/*	�ō����x	*/
	//float accel;		/*	�����x	if( speed < max_speed ) speed += accel		*/

	float gravity;		/*	�d��		*/

	Vector3 pos;		/*	���W	*/
	Vector3 angle;		/*	����	*/
	Vector3 move;		/*	�ړ��l	*/

	bool died;			/*	���񂾂��ǂ���				*/
	bool isJump;		/* 2�i�W�����v�h�~�p(�K�v�H)	*/
	bool isLand;		/* �n�ʂɂ��Ă��邩			*/


	bool invincible;	/* ���G���	*/
	int invincible_time;/* ���G����	*/

	iex3DObj2 *obj;

	virtual void Near_attack();				/*	�ߋ����U��				*/
	virtual void Far_attack();				/*	�������U��				*/
	virtual void Jump();					/*	�W�����v				*/
	virtual void Chara_attack() = 0;		/*	�L�����ŗL				*/
	virtual void Special_attack() = 0;		/*	�L�����K�E�Z			*/
	virtual void Burst();					/*	�o�[�X�g����			*/
	virtual void Move();					/*	�ړ�					*/
	virtual void Die();						/*	����					*/
	virtual void Control(int pad);			/*	�L�[���͌n�͂�����		*/

	void Compute_move();					/*	�ړ��ʂƂ��d�͂ɏ]���č��W���X�V	*/


	//=====================================================================
	// �萔
	// angle.y�ɂ��ꂼ�������Ă�������
	const float LEFT_ANGLE = 270.0f * .0174532925f;
	const float RIGHT_ANGLE = 90.0f * .0174532925f;

public:

	//===========================================
	//	�������Ɖ��
	BasePlayer(){}
	void Initialize(int pad);		// ���W�Ƃ�state���Ƃ��L�����ŋ��ʂ��Ă�����̏�����( �S���������ݒ肷��̂��߂�ǂ������l�p )
	void Respawn(int pad);			//	���X�|�[��(����)

	virtual ~BasePlayer(){}


	//===========================================
	//	�X�V�ƕ`��
	virtual void Update(int pad);
	virtual void Render();



	//===========================================
	//	�Q�b�^�[�E�Z�b�^�[
	void Damage(int value)
	{
		hp -= value;
	}

	void Set_invincible()
	{
		invincible = true;
		invincible_time = 120;
	}
	void Set_invincible(int frame)
	{
		invincible = true;
		invincible_time = frame;
	}

	// ���W
	void Get_pos(Vector3 &out){ out = pos; }
	Vector3 Get_pos(){ return pos; }
	void Set_pos(const Vector3 &p){ pos = p; }



	// obj�̊e�x�N�g��
	void Get_obj_front(Vector3 &out){ out = Vector3(obj->TransMatrix._31, obj->TransMatrix._32, obj->TransMatrix._33); }
	Vector3 Get_obj_front(){ return Vector3(obj->TransMatrix._31, obj->TransMatrix._32, obj->TransMatrix._33); }

	void Get_obj_up(Vector3 &out){ out = Vector3(obj->TransMatrix._21, obj->TransMatrix._22, obj->TransMatrix._23); }
	Vector3 Get_obj_up(){ return Vector3(obj->TransMatrix._21, obj->TransMatrix._22, obj->TransMatrix._23); }


	// �A���O��
	void Get_angle(Vector3 &out){ out = angle; }
	Vector3 Get_angle(){ return angle; }
	void Set_angle(const Vector3 &a){ angle = a; }


	// ���[�h(state)
	int Get_mode(){ return mode; }
	void Set_mode(int num)
	{
		mode = num;
		process = 0;
	}


	// ���[�h�̒��̃��[�h
	int Get_process(){ return process; }
	void Set_process(int p){ process = p; }


	// ����
	bool isDied(){ return died; }
	void Set_die(bool d){ died = d; }


	// ���x
	float Get_speed(){ return speed; }
	void Set_speed(float s){ speed = s; }


	// ���[�V����
	void Set_motion(int num){ if (obj->GetMotion(0) != num) obj->SetMotion(0, num); }


	//======================================
	//	�萔(����Ȃɂ���Ȃ�����)
	enum PLAYER_MODE
	{
		WAIT,			/*	�ҋ@			*/
		MOVE,			/*	�ړ�			*/
		NEAR_ATTACK,	/*	�ߋ���			*/
		FAR_ATTACK,		/*	������			*/
		CHARA_ATTACK,	/*	�L�����ŗL		*/
		JUMP,			/*	�W�����v		*/
		BURST,			/*	�o�[�X�g��		*/
		SPECIAL_ATTACK,	/*	�K�E�Z			*/
		GETTING_ITEM,	/*	�A�C�e���擾���*/
		DIE				/*	���ɂ�����		*/
	};
};

#endif