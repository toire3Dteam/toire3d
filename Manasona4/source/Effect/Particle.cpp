#include "TDNLIB.h"
#include "Particle.h"


//*****************************************************************************
//	�y���\�i���������u��
//*****************************************************************************
void ParticleManager::EffectPersonaTrigger(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	FOR(20)
	{
		Pos.x = pos.x + rand() % 11 - 5.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 11 - 5.0f;
		const int M = 8;
		Move.x = (rand() % M - (M / 2))*0.25f;
		Move.y = (rand() % 3 + 2) * 0.1f;
		Move.z = (rand() % M - (M / 2))*0.25f;
		Power.x = -Move.x*.01f;
		Power.y = 0;
		Power.z = -Move.z*.01f;
	
		// �p�[�e�B�N���Z�b�g
		Set(6, 0, .0f, 60, .0f, 20, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, rand() % 2 + 1.5f, RS::ADD);
	}
}

//*****************************************************************************
//	�y���\�i�����I�[��
//*****************************************************************************
void ParticleManager::EffectPersonaAura(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	for (int i = 0; i < 2; i++)
	{

		Pos.x = pos.x + rand() % 21 - 10.0f;
		Pos.y = pos.y + (float)(rand() % 10);
		Pos.z = pos.z + rand() % 21 - 10.0f;
		const int M = 2;
		Move.x = (rand() % M - (M / 2))*0.091f;
		Move.y = (rand() % 2 + 2) * 0.05f;
		Move.z = (rand() % M - (M / 2))*0.091f;
		Power.x = 0;
		Power.y = 0.0f;
		Power.z = 0;

		const int type = (rand() % 2) ? 5 : 6;

		// �p�[�e�B�N���Z�b�g
		Set2(type, 0, .1f, 20, .0f, 10, .4f, Pos, Move, Power, .75f, .75f, .75f, (rand() % 3 - 1) * 0.025f, rand() % 314 * 0.01f, 1.0f, rand() % 3 + 5.0f, RS::ADD);

	}
}

//*****************************************************************************
//	�I�[�o�[�h���C�u�����I�[��
//*****************************************************************************
void ParticleManager::EffectOverDrive(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	for (int i = 0; i < 3; i++)
	{
		Pos.x = pos.x + rand() % 9 - 4.0f;
		Pos.y = pos.y + (float)(rand() % 7+2);
		Pos.z = pos.z + rand() % 9 - 4.0f;
		const int M = 2;
		Move.x = (rand() % M - (M / 2))*0.091f;
		Move.y = (rand() % 2 + 2) * 0.05f;
		Move.z = (rand() % M - (M / 2))*0.091f;
		Power.x = 0;
		Power.y = 0.0f;
		Power.z = 0;

		const int type = (rand() % 2) ? 13 : 13;

		// �p�[�e�B�N���Z�b�g
		Set2(type, 0, .1f, 20, .0f, 10, .4f, Pos, Move, Power, .75f, .75f, .75f, (rand() % 3 - 1) * 0.025f, rand() % 314 * 0.01f, 1.0f, rand() % 3 + 2.0f, RS::ADD);

	}
}

//*****************************************************************************
//	�I�[�o�[�h���C�u�i�o�[�X�g�j�����I�[��
//*****************************************************************************
void ParticleManager::EffectOverDriveBurst(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	for (int i = 0; i < 3; i++)
	{
		Pos.x = pos.x + rand() % 9 - 4.0f;
		Pos.y = pos.y + (float)(rand() % 7 + 2);
		Pos.z = pos.z + rand() % 9 - 4.0f;
		const int M = 2;
		Move.x = (rand() % M - (M / 2))*0.091f;
		Move.y = (rand() % 2 + 2) * 0.05f;
		Move.z = (rand() % M - (M / 2))*0.091f;
		Power.x = 0;
		Power.y = 0.0f;
		Power.z = 0;

		const int type = (rand() % 2) ? 2 : 2;

		// �p�[�e�B�N���Z�b�g
		Set2(type, 0, .1f, 20, .0f, 10, .4f, Pos, Move, Power, .75f, .75f, .75f, (rand() % 3 - 1) * 0.025f, rand() % 314 * 0.01f, 1.0f, rand() % 3 + 2.0f, RS::ADD);

	}
}


//*****************************************************************************
//	�t�B�j�b�V���q�b�g
//*****************************************************************************
void ParticleManager::EffectFinish(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	FOR(20)
	{
		Pos.x = pos.x + rand() % 11 - 5.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 11 - 5.0f;
		const int M = 16;
		Move.x = (rand() % M - (M / 2))*0.25f;
		Move.y = (rand() % 3 + 7) * 0.1f;
		Move.z = (rand() % M - (M / 2))*0.25f;
		Power.x = 0;
		Power.y = 0;
		Power.z = 0;
		// �ԁ�
		Set(7, 0, 1.0f, 60, .0f, 40, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, rand() % 2 + 1.5f, RS::COPY);

		// ��
		for (int j = 0; j < 2; j++)
		{
			Move.x = rand() % 9 - 4.0f;
			Move.y = rand() % 9 - 4.0f;
			Move.z = rand() % 9 - 4.0f;
			Set(1, 0, 1.0f, 50, .0f, 30, .75f, pos, Move, Power, rand() % 9 * .1f, rand() % 9 * .1f, rand() % 9 * .1f, 0, 1.0f, rand() % 2 + 1.5f, RS::ADD);
		}

		Pos.x = pos.x + rand() % 6 - 2.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 6 - 2.0f;
		Move.Zero();
		// ���݂Ȃ�
		Set2(11, 0, .75f, 20, .0f, 12, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, rand() % 314 * .01f, 1.01f, rand() % 10 + 1.5f, RS::ADD);
	}
}

//*****************************************************************************
//	������񂾎��̉�
//*****************************************************************************
void ParticleManager::EffectFlySmoke(const Vector3 &pos)
{
	static int set(0);

	if (++set < 4)
	{
		return;
	}

	set = 0;

	Vector3 Pos, Move, Power;

	Pos.x = pos.x + rand() % 8 - 3.0f;
	Pos.y = pos.y + (float)(rand() % 3);
	Pos.z = pos.z + rand() % 8 - 3.0f;
	Move.x = (rand() % 5 - 2) * 0.005f;
	Move.y = (rand() % 3 + 2) * 0.01f;
	Move.z = (rand() % 5 - 2) * 0.005f;
	Power.Zero();
	Set2(4, 0, 0.8f, 40, .0f, 10, 1.0f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, rand() % 628 * .01f, 1.005f, rand() % 2 + 6.0f, RS::COPY);
}

//*****************************************************************************
//	����͂��߂̉�
//*****************************************************************************
void ParticleManager::EffectRunSmoke(const Vector3 &pos, bool bLeft)
{
	Vector3 Pos, Move, Power(0, 0, 0);

	FOR(10)
	{
		Pos.x = pos.x + rand() % 8 - 3.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 8 - 3.0f;
		Move.x = (rand() % 30 + 20) * 0.005f;
		Move.x = (bLeft) ? -abs(Move.x) : abs(Move.x);
		Move.y = (rand() % 3 + 2) * 0.01f;
		Move.z = (rand() % 11 - 5) * 0.005f;
		Set2(4, 0, 0.8f, 25, .0f, 10, .65f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, (rand() % 3 - 1) * 0.025f, rand() % 628 * .01f, 1.005f, rand() % 2 + 2.0f, RS::COPY);
	}
}

//*****************************************************************************
//	���n�̉�
//*****************************************************************************
void ParticleManager::EffectLandSmoke(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;
	FOR(10)
	{
		Pos.x = pos.x + rand() % 8 - 3.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 8 - 3.0f;
		Move.x = (rand() % 41 - 20) * 0.005f;
		Move.y = (rand() % 3 + 2) * 0.01f;
		Move.z = (rand() % 11 - 5) * 0.005f;
		Set2(4, 0, 0.8f, 25, .0f, 10, .65f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, (rand() % 3 - 1) * 0.025f, rand() % 628 * .01f, 1.005f, rand() % 2 + 2.0f, RS::COPY);
	}
}

//*****************************************************************************
//	�U���q�b�g
//*****************************************************************************
void ParticleManager::EffectHit(const Vector3 &pos, const Vector3 &v)
{
	Vector3 Pos, Move, Power;

	FOR(20)
	{
		Pos.x = pos.x + rand() % 5 - 2.0f;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z + rand() % 5 - 2.0f;
		Pos = pos;
		const int M = 64;
		Move.x = (rand() % M - (M / 2))*0.05f;
		if(v.x != 0)Move.x = (v.x < 0) ? -abs(Move.x) : abs(Move.x);
		Move.y = (rand() % 5 + 6) * 0.1f;
		Move.y = (v.y < 0) ? -Move.y : Move.y;
		Move.z = (rand() % M - (M / 2))*0.05f;
		Power.x = 0;
		Power.y = (v.y < 0) ? -.0001f : -0.015f;
		Power.z = 0;
		Set(7, 0, 1.0f, 90, .0f, 30, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, tdnRandom::Get(0.25, 1.0f), RS::ADD);

		Power.x = 0;
		Power.y = -0.01f;
		Power.z = 0;

		// ��
		//for (int j = 0; j < 1; j++)
		{
			Pos.x = pos.x + rand() % 5 - 2.0f;
			Pos.y = pos.y + (float)(rand() % 3);
			Pos.z = pos.z + rand() % 5 - 2.0f;
			Pos = pos;
			const int M2 = 16;
			Move.x = (rand() % M2 - (M2 / 2))*0.05f;
			Move.y = (rand() % 3 + 7) * 0.1f;
			Move.z = (rand() % M2 - (M2 / 2))*0.05f;
			Set(6, 0, 1.0f, 40, .0f, 20, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, tdnRandom::Get(0.5f, 1.0f), RS::ADD);

			//Move.x = (rand() % 30 - 16.0f) * 0.25f;
			//Move.y = rand() % 9 - 4.0f;
			//Move.z = (rand() % 30 -16.0f) * 0.001f;
			//Set(7, 0, 1.0f, 16, .0f, 6, .75f, pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, rand() % 1 + 1.0f, RS::ADD);
		}
	}
}

void ParticleManager::EffectGuard(const Vector3 & pos, const Vector3 & v)
{
	Vector3 Pos = VECTOR_ZERO,
			Move = VECTOR_ZERO,
			Power = VECTOR_ZERO;

	FOR(3)
	{
		// �����ʒu
		Pos.x = pos.x + rand() % 6 - 3.0f;
		Pos.y = pos.y + rand() % 6 - 3.0f;
		Pos.z = pos.z + rand() % 6 - 3.0f;
		

		// �ړ��n
		// ��ԕ����̃x�N�g�������̂܂܎g���@���x�𒲐����邾��
		// �t�����ɔ�юU��Ȃ��̂����_
		Move = v;
		Move.Normalize();	
		
		if (abs(Move.x) <= 0.5f) 
		{
			Move.x = (tdnRandom::Get(-1.1f, 1.1f));
		}else
		{
			Move.x = v.x * (tdnRandom::Get(0.25f, 1.85f) + 0.5f);
		}
		
		if (abs(Move.y) <= 0.5f)
		{
			Move.y = (tdnRandom::Get(-1.1f, 1.1f));
		}else
		{
			Move.y = v.y * (tdnRandom::Get(0.25f, 0.85f) + 0.5f);
		}

		//Move.y = v.y * (tdnRandom::Get(0.0f, 0.5f) + 0.5f);
		//Move.z = v.z * (tdnRandom::Get(0.0f, 0.5f) + 0.5f);
		Move.z = (tdnRandom::Get(-1.5f, 1.75f));
		
		//const int M = 32;	
		//Move.x = (rand() % M - (M / 2))*0.05f;
		//if (v.x != 0)Move.x = (v.x < 0) ? -abs(Move.x) : abs(Move.x);
		//Move.y = (rand() % 6 - 3) * 0.1f;	
		//Move.y = (v.y < 0) ? -Move.y : Move.y;
		//Move.z = (rand() % M - (M / 2))*0.05f;

		// �d��
		//Power.x = 0;
		//Power.y = (v.y < 0) ? -.0001f : -0.015f;
		//Power.z = 0;
		Power.x = tdnRandom::Get(-0.0025f, 0.0025f);
		Power.y = -0.005f;
		Power.z = tdnRandom::Get(-0.0015f, 0.0015f);

		// �����悭��юU��
		Set(6, 0, 1.0f, 12, .0f, 7, 1.0f, Pos, Move*0.6f, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, tdnRandom::Get(0.25, 1.0f), RS::ADD);
		// �З͎��
		Set(6, 0, 1.0f, 16, .0f, 10, 1.0f, Pos, Move*0.25f , Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, tdnRandom::Get(0.25, 1.0f), RS::ADD);

	}


	// �U�����������ɔ�΂�
	FOR(25)
	{
		// �����ʒu
		Pos.x = pos.x + rand() % 6 - 3.0f;
		Pos.y = pos.y + rand() % 6 - 3.0f;
		Pos.z = pos.z + rand() % 6 - 3.0f;


		// �ړ��n
		// ��ԕ����̃x�N�g�������̂܂܎g���@���x�𒲐����邾��
		// �t�����ɔ�юU��Ȃ��̂����_
		Vector3 vMoveVec = -v;
		vMoveVec.Normalize();

		if (abs(Move.x) <= 0.5f)
		{
			Move.x = (tdnRandom::Get(-1.1f, 1.1f));
		}
		else
		{
			Move.x = vMoveVec.x * (tdnRandom::Get(0.25f, 1.85f) + 0.5f);
		}

		if (abs(Move.y) <= 0.5f)
		{
			Move.y = (tdnRandom::Get(-1.1f, 1.1f));
		}
		else
		{
			Move.y = vMoveVec.y * (tdnRandom::Get(0.25f, 0.85f) + 0.5f);
		}

		//Move.y = v.y * (tdnRandom::Get(0.0f, 0.5f) + 0.5f);
		//Move.z = v.z * (tdnRandom::Get(0.0f, 0.5f) + 0.5f);
		Move.z = (tdnRandom::Get(-1.5f, 1.75f));

		//const int M = 32;	
		//Move.x = (rand() % M - (M / 2))*0.05f;
		//if (v.x != 0)Move.x = (v.x < 0) ? -abs(Move.x) : abs(Move.x);
		//Move.y = (rand() % 6 - 3) * 0.1f;	
		//Move.y = (v.y < 0) ? -Move.y : Move.y;
		//Move.z = (rand() % M - (M / 2))*0.05f;

		// �d��
		//Power.x = 0;
		//Power.y = (v.y < 0) ? -.0001f : -0.015f;
		//Power.z = 0;
		Power.x = tdnRandom::Get(-0.0025f, 0.0025f);
		Power.y = -0.005f;
		Power.z = tdnRandom::Get(-0.0015f, 0.0015f);

		// �����悭��юU��
		Set(6, 0, 1.0f, 12, .0f, 7, 1.0f, Pos, Move*0.9f, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, tdnRandom::Get(0.25, 1.0f), RS::ADD);
		// �З͎��
		Set(6, 0, 1.0f, 16, .0f, 10, 1.0f, Pos, Move*0.45f, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, tdnRandom::Get(0.25, 1.0f), RS::ADD);

		//Power.x = 0;
		//Power.y = -0.01f;
		//Power.z = 0;

		//// ��
		////for (int j = 0; j < 1; j++)
		//{
		//	Pos.x = pos.x + rand() % 5 - 2.0f;
		//	Pos.y = pos.y + (float)(rand() % 3);
		//	Pos.z = pos.z + rand() % 5 - 2.0f;
		//	Pos = pos;
		//	const int M2 = 16;
		//	Move.x = (rand() % M2 - (M2 / 2))*0.05f;
		//	Move.y = (rand() % 3 + 7) * 0.1f;
		//	Move.z = (rand() % M2 - (M2 / 2))*0.05f;
		//	Set(7, 0, 1.0f, 40, .0f, 20, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, rand() % 1 + .5f, RS::COPY);

		//	//Move.x = (rand() % 30 - 16.0f) * 0.25f;
		//	//Move.y = rand() % 9 - 4.0f;
		//	//Move.z = (rand() % 30 -16.0f) * 0.001f;
		//	//Set(7, 0, 1.0f, 16, .0f, 6, .75f, pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, rand() % 1 + 1.0f, RS::ADD);
		//}
	}
}

//*****************************************************************************
//	���U���g�̌����Ԃ�
//*****************************************************************************
void ParticleManager::EffectResultSplash(const Vector3 &pos)
{
	Vector3 Pos, Move, Power;

	FOR(1)
	{
		Pos.x = pos.x;
		Pos.y = pos.y + (float)(rand() % 3);
		Pos.z = pos.z;
		Move.x = (rand() % 101 - 50)*0.003f;
		Move.y = (rand() % 100)*0.003f;
		Move.z = (rand() % 101 - 50)*0.003f;
		Power.x = .0f;
		Power.y = -0.01f;
		Power.z = .0f;
		Set2(14, 0, 1.0f, 50, 1.0f, 5, 1.0f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, (rand() % 3 - 1) * 0.025f, rand() % 628 * .01f, 
			.955f,	// �g�嗦
			rand() % 2 + 4.0f, RS::ADD);
	}
}

//*****************************************************************************
//	�����̉��̃G�t�F�N�g
//*****************************************************************************
void ParticleManager::EffectExplosion(const Vector3 &pos)
{
	Vector3 Pos, Move, Power(0, 0, 0);

	FOR(20)
	{
		Pos = pos;
		Move.x = (rand() % 101 - 50)*0.007f;
		Move.y = (rand() % 100)*0.007f;
		Move.z = (rand() % 101 - 50)*0.007f;
		Set2(9, 0, 0.9f, 80, .0f, 50, .75f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, rand() % 628 * .01f, 1.005f, rand() % 4 + 10.0f, RS::ADD);
	}

	// ���S�̔���
	//Set2(7, 0, 1.0f, 75, .0f, 60, 1.0f, Pos, Move, Power, 1, 1, 1, 0, 0, 1.1f, 4, RS::ADD);
}


//*****************************************************************************
//	�e�̋O�ՃG�t�F�N�g
//*****************************************************************************
void ParticleManager::EffectShotLocus(const Vector3 & pos)
{
	Vector3 Pos = VECTOR_ZERO , Move = VECTOR_ZERO, Power = VECTOR_ZERO;

	FOR(10)
	{
		Pos.x = pos.x + tdnRandom::Get(-3.0f, 3.0f);
		Pos.y = pos.y + +tdnRandom::Get(-2.5f, 2.5f);
		Pos.z = pos.z + tdnRandom::Get(-2.5f, 2.5f);
		const int M = 2;
		Move.x = tdnRandom::Get(-0.1f, 0.1f);
		Move.y = tdnRandom::Get(-0.125f, 0.125f);
		Move.z = tdnRandom::Get(-0.1f, 0.1f);
		//Move.x = (rand() % M - (M / 2))*0.25f;
		//Move.y = (rand() % M - (M / 2))*0.25f;
		//Move.z = (rand() % M - (M / 2))*0.25f;
		//Power.x = -Move.x*.01f;
		//Power.y = -Move.y*.01f;
		//Power.z = -Move.z*.01f;

		// �p�[�e�B�N���Z�b�g
		Set(6, 0, .0f, 50, .0f, 20, .9f, Pos, Move, Power, 1.0f, 1.0f, 1.0f, 0, 1.0f, rand() % 2 + 1.5f, RS::ADD);
		// id  sf  s   ef   e   mf   m
	}

}

//**************************************************************************************************
//
//		�p�[�e�B�N���Ǘ��N���X
//
//**************************************************************************************************

int				ParticleManager::m_NumParticles;	//	�p�[�e�B�N���ő吔
ParticleData	*ParticleManager::m_pParticleDatas;	//	�p�[�e�B�N���f�[�^
tdn2DObj*       ParticleManager::m_pTexture;	//	�e�N�X�`��

Matrix			ParticleManager::CurViewProjection;

//*****************************************************************************
//	�������E���
//*****************************************************************************
//------------------------------------------------------
//		������
//------------------------------------------------------
void ParticleManager::Initialize(char* filename, int NumParticles)
{
	//	�p�[�e�B�N���o�b�t�@�m��
	m_NumParticles = NumParticles;
	m_pParticleDatas = new ParticleData[NumParticles];
	//	�S���Z�b�g
	Reset();
	//	�e�N�X�`���ݒ�
	m_pTexture = new tdn2DObj(filename);
}

//------------------------------------------------------
//		���
//------------------------------------------------------
void ParticleManager::Release()
{
	delete[] m_pParticleDatas;
	delete m_pTexture;
}

//*****************************************************************************
//		�f�[�^�ݒ�
//*****************************************************************************
//------------------------------------------------------
//		�\���̎w��
//------------------------------------------------------
void ParticleManager::Set(LPPARTICLE pd)
{
	// �󂫃f�[�^����
	FOR(m_NumParticles)
	{
		// �󂫃f�[�^��������A�ݒ肵�Ĕ�����
		if (!m_pParticleDatas[i].isActive())
		{
			m_pParticleDatas[i].Set(pd);
			break;
		}
	}
}

//------------------------------------------------------
//		�f�[�^�ʎw��
//------------------------------------------------------
void ParticleManager::Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float rotate, float angle, float stretch, float scale, u8 flag)
{
	PARTICLE pd;

	pd.type = type;
	pd.aFrame = aFrame;
	pd.aColor = aColor;
	pd.eFrame = eFrame;
	pd.eColor = eColor;
	pd.mFrame = mFrame;
	pd.mColor = mColor;

	pd.pos = Pos;
	pd.move = Move;
	pd.power = Power;
	pd.rotate = rotate;
	pd.stretch = stretch;

	pd.scale = scale;
	pd.angle = angle;

	pd.flag = flag;

	Set(&pd);
}

void	ParticleManager::Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float r, float g, float b, float rotate, float stretch, float scale, u8 flag)
{
	Set2(type, aFrame, aAlpha, eFrame, eAlpha, mFrame, mAlpha, Pos, Move, Power, r, g, b, rotate, .0f, stretch, scale, flag);
}

void	ParticleManager::Set2(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float r, float g, float b, float rotate, float angle, float stretch, float scale, u8 flag)
{
	COLOR	color;
	DWORD	aa, ea, ma;

	aa = ((DWORD)(aAlpha*255.0f) << 24);
	ea = ((DWORD)(eAlpha*255.0f) << 24);
	ma = ((DWORD)(mAlpha*255.0f) << 24);
	color = ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | (DWORD)(b*255.0f);
	Set(type, aFrame, aa | color, eFrame, ea | color, mFrame, ma | color, Pos, Move, Power, rotate, angle, stretch, scale, flag);
}

//*****************************************************************************
//		�X�V
//*****************************************************************************
void ParticleManager::Update()
{
	FOR(m_NumParticles) if (m_pParticleDatas[i].isActive()) m_pParticleDatas[i].Update();	// �Z�b�g����Ă���X�V
}

//*****************************************************************************
//		���Z�b�g
//*****************************************************************************
void ParticleManager::Reset()
{
	for (int i = 0; i<m_NumParticles; i++) if (m_pParticleDatas[i].isActive()) m_pParticleDatas[i].SetFlag(0);	// �S���Z�b�g�t���O���I�t�ɂ���
}

//*****************************************************************************
//		�`��
//*****************************************************************************
//------------------------------------------------------
//		�Œ�@�\�`��
//------------------------------------------------------
void ParticleManager::Render()
{
	CurViewProjection = matView * matProjection;

	tdnRenderState::Set(RS::ADD, NULL, m_pTexture->GetTexture());
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	tdnSystem::GetDevice()->SetFVF(D3DFVF_TLVERTEX);
	// �A���t�@�u�����h�ݒ�
	tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// �p�[�e�B�N�������_�����O
	FOR(m_NumParticles) if (m_pParticleDatas[i].isActive()) m_pParticleDatas[i].Render();

	tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//------------------------------------------------------
//		�V�F�[�_�[�`��
//------------------------------------------------------
void ParticleManager::Render(tdnShader* shader, char* name)
{
	//int		i;
	//float	scale = -1.0f;
	//
	//CurViewProjection = matView * matProjection;
	//
	//shader->SetTexture(m_pTexture->GetTexture());
	//iexRenderState::Set(RS_ADD, NULL, lpTexture->GetTexture());
	//
	//iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//iexSystem::Device->SetFVF(D3DFVF_TLVERTEX);
	////	�A���t�@�u�����h�ݒ�
	//iexSystem::Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//iexSystem::Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//iexSystem::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//
	//for (i = 0; i<m_NumParticles; i++){
	//	//	�p�[�e�B�N�������_�����O
	//	if (ParticleData[i].isActive()) ParticleData[i].Render(shader, name);
	//}

	//iexSystem::Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}






//*****************************************************************************
//
//		�p�[�e�B�N���P��
//
//*****************************************************************************

//*****************************************************************************
//		�Z�b�g
//*****************************************************************************
void ParticleData::Set(LPPARTICLE pd)
{
	memcpy(&pdata, pd, sizeof(PARTICLE));
	CurFrame = 0;
	CurAlpha = 0;
	bFlags = 1;
}

//*****************************************************************************
//		�X�V
//*****************************************************************************
void ParticleData::Update()
{
	// �A�N�e�B�u����Ȃ�������A�łĂ������I
	if (!isActive()) return;

	float	work;
	int	a1, r1, g1, b1, a2, r2, g2, b2;

	//	���X�V
	pdata.pos += pdata.move;

	//if (pdata.isWork){
	//	if (pdata.Pos.y < pdata.work){
	//		pdata.Pos.y = pdata.work;
	//		pdata.Move.y = -pdata.Move.y*pdata.Reflection;
	//	}
	//}
	//
	//if (pdata.isDeceleration){
	//
	//	pdata.Move *= pdata.deceleration;
	//}

	pdata.move += pdata.power;
	pdata.angle += pdata.rotate;
	pdata.scale *= pdata.stretch;

	//	�J���[�v�Z
	if (CurFrame < pdata.aFrame) CurAlpha = .0f;
	else
	{
		if (CurFrame < pdata.mFrame)
		{
			work = (CurFrame - pdata.aFrame) / (float)(pdata.mFrame - pdata.aFrame);
			a1 = (pdata.aColor >> 24);
			r1 = ((pdata.aColor >> 16) & 0xFF);
			g1 = ((pdata.aColor >> 8) & 0xFF);
			b1 = ((pdata.aColor) & 0xFF);
		}
		else
		{
			work = (pdata.eFrame - CurFrame) / (float)(pdata.eFrame - pdata.mFrame);
			a1 = (pdata.eColor >> 24);
			r1 = ((pdata.eColor >> 16) & 0xFF);
			g1 = ((pdata.eColor >> 8) & 0xFF);
			b1 = ((pdata.eColor) & 0xFF);
		}

		a2 = (pdata.mColor >> 24);
		r2 = ((pdata.mColor >> 16) & 0xFF);
		g2 = ((pdata.mColor >> 8) & 0xFF);
		b2 = ((pdata.mColor) & 0xFF);
		//	�J���[�ݒ�
		CurAlpha = ((a2 - a1)*work + a1) / 255.0f;
		r = ((r2 - r1)*work + r1) / 255.0f;
		g = ((g2 - g1)*work + g1) / 255.0f;
		b = ((b2 - b1)*work + b1) / 255.0f;
	}

	//	�t���[���i�s
	CurFrame++;
	if (CurFrame >= pdata.eFrame) bFlags = 0;
}

//*****************************************************************************
//		�`��
//*****************************************************************************
//------------------------------------------------------
//		�|���S������
//------------------------------------------------------
inline bool ParticleData::SetVertex(TLVERTEX* v)
{
	D3DVIEWPORT9	vp;
	tdnSystem::GetDevice()->GetViewport(&vp);

	Matrix mat2 = ParticleManager::CurViewProjection;

	float z1 = pdata.pos.x * matView._13 + pdata.pos.y * matView._23 + pdata.pos.z * matView._33 + matView._43;

	//	�ʒu�ϊ�
	Vector3	pos;
	pos.z = pdata.pos.x * mat2._13 + pdata.pos.y * mat2._23 + pdata.pos.z * mat2._33 + mat2._43;
	if (pos.z <= 0) return false;

	pos.x = pdata.pos.x * mat2._11 + pdata.pos.y * mat2._21 + pdata.pos.z * mat2._31 + mat2._41;
	pos.y = pdata.pos.x * mat2._12 + pdata.pos.y * mat2._22 + pdata.pos.z * mat2._32 + mat2._42;

	float pw = pdata.pos.x * mat2._14 + pdata.pos.y * mat2._24 + pdata.pos.z * mat2._34 + mat2._44;

	float	x1, x2, y1, y2, s, c;
	s = sinf(pdata.angle) * pdata.scale;
	c = cosf(pdata.angle) * pdata.scale;
	//	���_�ݒ�
	x1 = s - c;
	x2 = c + s;
	y1 = c + s;
	y2 = c - s;
	//	���e�T�C�Y�v�Z
	float px, py, px2, py2;
	px = x1*matProjection._11 + y1*matProjection._21 + z1*matProjection._31 + matProjection._41;
	py = x1*matProjection._12 + y1*matProjection._22 + z1*matProjection._32 + matProjection._42;
	px2 = x2*matProjection._11 + y2*matProjection._21 + z1*matProjection._31 + matProjection._41;
	py2 = x2*matProjection._12 + y2*matProjection._22 + z1*matProjection._32 + matProjection._42;

	v[0].sx = (pos.x + px) / pw;
	v[0].sy = (pos.y + py) / pw;
	v[1].sx = (pos.x + px2) / pw;
	v[1].sy = (pos.y + py2) / pw;
	v[2].sx = (pos.x - px2) / pw;
	v[2].sy = (pos.y - py2) / pw;
	v[3].sx = (pos.x - px) / pw;
	v[3].sy = (pos.y - py) / pw;

	v[0].sz = v[1].sz = v[2].sz = v[3].sz = pos.z / pw;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	int w = vp.Width;
	int h = vp.Height;

	bool bRender = false;
	for (int i = 0; i<4; i++)
	{
		if (v[i].sx > -1.0f && v[i].sx < 1.0f) bRender = true;
		if (v[i].sy > -1.0f && v[i].sy < 1.0f) bRender = true;
		v[i].sx = (v[i].sx + 1.0f) * w / 2 + vp.X;
		v[i].sy = (-v[i].sy + 1.0f) * h / 2 + vp.Y;
	}

	if (bRender == false) return false;
	//	�^�C�v�ݒ�
	v[0].tu = v[2].tu = (float)(pdata.type % 4) * 0.25f + 0.001f;
	v[1].tu = v[3].tu = v[0].tu + 0.248f;
	v[0].tv = v[1].tv = (float)(pdata.type / 4) * 0.25f + 0.001f;
	v[3].tv = v[2].tv = v[0].tv + 0.248f;

	//	�F�ݒ�
	v[0].color = v[1].color = v[2].color = v[3].color = ((DWORD)(CurAlpha*255.0f) << 24) | ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | ((DWORD)(b*255.0f));

	return true;
}

//------------------------------------------------------
//		�Œ�@�\�`��
//------------------------------------------------------
void ParticleData::Render()
{
	TLVERTEX	v[4];

	if (SetVertex(v) == false) return;
	//	�����ݒ�
	//tdnRenderState::Set(pdata.flag, NULL, NULL);
	// ���Z
	if (pdata.flag == RS::SUB) tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	// ���Z
	else if (pdata.flag == RS::ADD)
	{
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ��ɕ`���Ă��镨�̂��S�͕`��
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	else if (pdata.flag == RS::ADD_NOZ)
	{
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ��ɕ`���Ă��镨�̂��S�͕`��
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
	}
	// COPY
	else
	{
		tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]���� + �]����
		tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �������Ă�����̂��ǂꂭ�炢������邩
		tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ��ɕ`���Ă������̂����̔��Ε����߂ĕ`��
		//tdnSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// �[�x�o�b�t�@�ւ̏������݂�L���ɂ���
		tdnSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	//	�����_�����O
	tdnSystem::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TLVERTEX));
}

//------------------------------------------------------
//		�V�F�[�_�[�`��
//------------------------------------------------------
void ParticleData::Render(tdnShader* shader, char* name)
{
	//if (pdata.flag != RS_SHADER){ Render(); return; }
	//
	//TLVERTEX	v[4];
	//SetVertex(v);
	//
	////	�V�F�[�_�[�̓K�p
	//u32 pass = shader->Begine(name);
	//
	//for (u32 p = 0; p<pass; p++)
	//{
	//	shader->BeginePass(p);
	//	shader->CommitChanges();
	//	//	�����_�����O
	//	iexSystem::Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TLVERTEX));
	//	shader->EndPass();
	//}
	//shader->End();
}