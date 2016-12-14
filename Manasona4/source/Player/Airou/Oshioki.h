#pragma once

#include "Airou.h"
#include "../../Effect/UVEffect/BaseUVEffect.h"
#include "../../Effect/LocusEffect.h"
#include "../../PointLight/PointLight.h"
#include "../../Effect/Particle.h"
#include "../../Sound/SoundManager.h"
#include "../../BaseEntity/Message/MessageDispatcher.h"


//*************************************************************
//	���������A�C���[
namespace OshiokiAirou
{
	class Base
	{
	public:
		Base(Airou *pUser, BasePlayer *pTarget, float speed, float radius) :m_pUser(pUser), m_vPos(0, 0, 0), m_pTarget(pTarget), m_vMoveVec(0, 0, 0), m_bErase(false), c_SPEED(speed), c_APP_RADIUS(radius){}
		virtual ~Base(){}
		virtual void Update() = 0;
		virtual void Render(iex3DObj *pObj) = 0;
		bool EraseOK(){ return m_bErase; }
	protected:
		Vector3 m_vPos;			// �A�C���[���W
		Airou *m_pUser;	// ���̂����������g���v���C���[�̃|�C���^
		BasePlayer *m_pTarget;		// �U���Ώۂ̃|�C���^
		Vector3 m_vMoveVec;		// �ړ��̌���
		bool m_bErase;			// �����t���O
		float c_SPEED;	// �ړ����x
		const float c_APP_RADIUS;	// ������Ă��ɑ΂��鏉�����W�̔��a
	};

	class OverDrive : public Base
	{
	private:
		const float c_APP_RADIUS = 30.0f;	// ������Ă��ɑ΂��鏉�����W�̔��a
		const float c_ALPHA_SPEED = .05f;	// �A���t�@�l�̉��Z���x
		float m_fAlpha;					// �A���t�@�l
		int m_iStep;					// �X�e�b�v
		Vector3 m_vOrgPos;
		float m_speed;
		float m_accel;
		bool m_bHit;
		bool m_bLast;
	public:
		OverDrive(Airou *pUser, BasePlayer *pTarget, bool bLast) :Base(pUser, pTarget, 6, 30), m_fAlpha(0), m_iStep(0), m_speed(0), m_accel(.1f), m_bHit(false), m_bLast(bLast)
		{
			// �S���������_���x�N�g���쐬
			//tdnRandom::GetRandomVector3(m_vMoveVec);
			const float ra(rand() % 618 * .01f);
			m_vMoveVec.x = sinf(ra);
			m_vMoveVec.y = cosf(ra);
			m_vMoveVec.z = 0;

			// �v���C���[�̍��W
			m_vPos = pTarget->GetCenterPos();

			// ���̃x�N�g��������r������
			m_vPos += -m_vMoveVec * c_APP_RADIUS;
			m_vOrgPos = m_vPos;
		}
		void Update()
		{
			switch (m_iStep)
			{
			case 0:
				// 0����A���t�@�l���Z
				if ((m_fAlpha += c_ALPHA_SPEED) > 1)
				{
					m_fAlpha = 1;
					m_iStep++;
				}
				break;
			case 1:
			{
					  // �ړ�
					  m_accel += .01f;
					  m_speed = min(m_speed + m_accel, c_SPEED);
					  const float LengthSq(((m_vPos += m_vMoveVec*m_speed) - m_vOrgPos).LengthSq());

					  if (LengthSq > (c_APP_RADIUS * 2) * (c_APP_RADIUS * 2))
					  {
						  m_iStep++;
					  }
					  else if (!m_bHit)
					  {
						  if (LengthSq > c_APP_RADIUS * 2)
						  {
							  // �v���C���[�Ƀ��b�Z�[�W���M
							  HIT_DAMAGE_INFO DamageInfo;
							  DamageInfo.fComboRate = 1;
							  DamageInfo.bOverDrive = true;

							  // �Ō�̃A�C���[�̃_���[�W
							  if (m_bLast)
							  {
								  DamageInfo.BeInvincible = true;	// ���G�ɂȂ邩�ǂ���
								  DamageInfo.damage = 427;				// �_���[�W(�X�R�A)
								  DamageInfo.FlyVector.Set((m_pUser->GetTargetDir() == DIR::LEFT) ? -2.5f : 2.5f, 2.5f);			// ������уx�N�g��
								  DamageInfo.iHitStopFrame = 8;		// �q�b�g�X�g�b�v
								  DamageInfo.HitRecoveryFrame = 60;		// �d������
								  DamageInfo.HitEffectType = (int)EFFECT_TYPE::DAMAGE;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
								  DamageInfo.iAttackType = (int)BASE_ACTION_STATE::HEAVEHO_DRIVE;						// ���̍U�����̃^�C�v(�R���{���ɓ����U�����g�킹�Ȃ��悤��)
								  DamageInfo.HitSE = "������q�b�g";
								  DamageInfo.DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;


								  // �Ō��1�������t�B�j�b�V������
								  DamageInfo.bFinishOK = true;

								  // �����������Ă�����
								  m_pTarget->SetMoveUpdate(true);
							  }

							  // ����ȊO�̃A�C���[�̃_���[�W
							  else
							  {
								  DamageInfo.BeInvincible = false;	// ���G�ɂȂ邩�ǂ���
								  DamageInfo.damage = 20;				// �_���[�W(�X�R�A)
								  DamageInfo.FlyVector.Set(0, 0);			// ������уx�N�g��
								  DamageInfo.iHitStopFrame = 1;		// �q�b�g�X�g�b�v
								  DamageInfo.HitRecoveryFrame = 45;		// �d������
								  DamageInfo.HitEffectType = (int)EFFECT_TYPE::MULTIPLE_HIT;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
								  DamageInfo.iAttackType = (int)BASE_ACTION_STATE::HEAVEHO_DRIVE;						// ���̍U�����̃^�C�v(�R���{���ɓ����U�����g�킹�Ȃ��悤��)
								  DamageInfo.HitSE = "������q�b�g";
								  DamageInfo.bFinishOK = false;
								  DamageInfo.DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
							  }
							  MsgMgr->Dispatch(0, m_pUser->GetID(), m_pTarget->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &DamageInfo);

							  m_bHit = true;
						  }
					  }
			}
				break;
			case 2:
				// �����Ă���
				if ((m_fAlpha -= (c_ALPHA_SPEED * 2)) < 0)
				{
					m_fAlpha = 0;
					m_bErase = true;
				}
				break;
			}
		}
		void Render(iex3DObj *pObj)
		{
			// ����&�쐬
			Math::SetTransMatrixFrontVec(&pObj->TransMatrix, m_vPos, m_vMoveVec);

			//MyAssert(pObj->GetPos().x != 0 && pObj->GetPos().y != 0 && pObj->GetPos().z != 0, "���������A�C���[�����_�ɂ���");

			// �`��
			pObj->SetPos(m_vPos);
			//pObj->SetAngle(m_fAngleX, m_fAngleY, 0);
			//pObj->Update();
			pObj->Render(RS::COPY, m_fAlpha);
		}
	};

	class OverFlow :public Base
	{
	private:
		const Vector3 c_VEC = Vector3(0, -0.5f, -0.5f);
		const float c_STOPLINE = 5;
		const int c_EXPLOSION_TIME = 60;
		const int m_ID;					// �����_������Ȃ����Ђ傤�ɂ���悤
		float m_fAngleX, m_fAngleY;		// �������ĉ�]���邩��ϐ�����
		float m_fRandAddAngle;
		int m_iExplosionTimer;			// �������Ă��������܂ł̎���
		bool m_bTsukisasaru;				// �˂��h����A�C���[���ǂ���
		bool m_bTsukisasatta;			// �˂��h�������u�Ԃ����p

		BaseUVEffect *m_pBoost;		// 覐�UV�G�t�F�N�g
		BaseUVEffect *m_pImpact;	// �˂��h�������u�Ԃ̕ɃG�t�F�N�g
		LocusEffect *m_pLocus;		// 覐΂̋O�ՃG�t�F�N�g
		Vector3 m_vLocusPos, m_vLocusPos2;
	public:
		OverFlow(Airou *pUser, BasePlayer *pTarget, bool bTsukisasaru, int ID = 0) :Base(pUser, pTarget, 9, 60), m_bTsukisasaru(bTsukisasaru), m_iExplosionTimer(0), m_fAngleY(atan2f(c_VEC.x, c_VEC.z)), m_fAngleX(PI * .25f), m_fRandAddAngle((rand() % 618)*.01f), m_pBoost(new AirouBoostEffect), m_pLocus(new LocusEffect("DATA/UVeffect/Airou/Locus.png", 6, 2)), m_bTsukisasatta(false), m_pImpact(nullptr), m_ID(ID)
		{
			m_vPos = -(m_vMoveVec = c_VEC) * (bTsukisasaru ? 400.0f : 600.0f);

			// �˂�������A�C���[�Ȃ烉���_������Ȃ�
			if (bTsukisasaru)
			{
				static const Vector2 add[8] =
				{
					{ 30, 30 },
					{ -30, 30 },
					{ 30, -30 },
					{ -30, -30 },
					{ 45, 30 },
					{ 15, 45 },
					{ -20, 40 },
					{ 20, -20 }
				};
				m_vPos.x += add[ID].x;
				m_vPos.y += add[ID].y;
				m_vPos.z -= 50;

				c_SPEED *= 1.5f;
			}

			// �����_��
			else
			{
				const float r(c_APP_RADIUS * (tdnRandom::Get(.0f, 0.75f) + 0.25f));
				m_vPos.x += sinf(m_fRandAddAngle) * r;
				m_vPos.y += cosf(m_fRandAddAngle) * r;

				// Y������
				m_vPos.y -= 10;
			}

			// ���[�J�X
			m_vLocusPos = m_vPos + Vector3(0, 5, 0);
			m_vLocusPos2 = m_vPos + Vector3(0, 10, 0);
			m_pLocus->Action(&m_vLocusPos, &m_vLocusPos2);

			// �G�t�F�N�g�A�N�V����
			m_pBoost->ActionRoop(m_vPos, 1, 1, Vector3(m_fAngleX + PI * .5f, m_fAngleY, 0), Vector3(m_fAngleX + PI * .5f, m_fAngleY, 0));
			m_pBoost->Update();
		}
		~OverFlow()
		{
			delete m_pBoost;
			delete m_pLocus;
			if (m_pImpact) delete m_pImpact;
		}
		void Update()
		{
			// ������
			if (m_iExplosionTimer > 0)
			{
				// �����I�����������t���OON
				if (--m_iExplosionTimer == 0) m_bErase = true;
				m_vPos.y -= .001f;	// �\������x�̏d��
				m_vPos += m_vMoveVec * (c_SPEED * .1f);

				// �A���O���߂��Ⴍ����ɑ���
				m_fAngleX += sinf(m_fRandAddAngle);
				m_fAngleY += cosf(m_fRandAddAngle);
			}
			if (m_vPos.y > c_STOPLINE)
			{
				// �ړ�
				m_vPos += m_vMoveVec * c_SPEED;
				m_vLocusPos += m_vMoveVec * c_SPEED;
				m_vLocusPos2 += m_vMoveVec*c_SPEED;

				// 覐΍X�V
				m_pBoost->SetPos(m_vLocusPos);
				m_pBoost->Update();

				// ���[�J�X�X�V
				m_pLocus->Update();
			}
			else
			{
				if (!m_bTsukisasaru)m_bErase = true;	// �˂��h����A�C���[����Ȃ������������
				else if (!m_bTsukisasatta)
				{
					// �˂��h����G�t�F�N�g����
					m_pImpact = new AirouImpactEffect;
					m_pImpact->Action(m_vPos, .5f, 1);
					m_pImpact->Update();

					// �|�C���g���C�g����
					const Vector3 LightPos(m_vPos.x, 5, m_vPos.z - 5.0f);
					PointLightMgr->AddPointLight(LightPos, Vector3(.0f, 1.0f, .7f), 65, 2, 180, 2, 170);

					m_pBoost->Stop();
					m_pLocus->Stop();

					m_bTsukisasatta = true;

					// �_���[�W
					// �R���{�p
					COMBO_DESK comboDesk;
					comboDesk.side = m_pTarget->GetSide();
					comboDesk.damage = 427;
					comboDesk.recoveryFrame = 10000;
					MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

					// �G�t�F�N�g
					se->Play("������q�b�g");
				}
				else
				{
					// �A�C���[�˂��h����G�t�F�N�g�X�V
					m_pImpact->SetPos(m_vPos);
					m_pImpact->Update();
				}
			}
		}
		void Render(iex3DObj *pObj)
		{
			pObj->SetPos(m_vPos);
			pObj->SetAngle(m_fAngleX, m_fAngleY, 0);
			pObj->Update();
			pObj->Render();

			// �G�t�F�N�g�`��
			m_pLocus->Render();
			m_pBoost->Render();
			if (m_pImpact)m_pImpact->Render();
		}

		// ����
		void Explosion()
		{
			// �����_���x�N�g���쐬
			//tdnRandom::GetRandomVector3(m_vMoveVec);
			//m_vMoveVec.y = fabsf(m_vMoveVec.y);	// ��Βl
			//m_vMoveVec.z = fabsf(m_vMoveVec.z);

			static const Vector3 v[8] =
			{
				{ -.1f, .4f, .5f },
				{ 0, .4f, .53f },
				{ .1f, .3f, .5f },
				{ .1f, .3f, .5f },
				{ .5f, .4f, .25f },
				{ .45f, .45f, .25f },
				{ -.25f, .5f, .25f },
				{ -.35f, .5f, .25f }
			};
			m_vMoveVec = (m_ID < 8) ? v[m_ID] : Vector3(.25f, .25f, .25f);

			m_iExplosionTimer = c_EXPLOSION_TIME;

			// ���[�J�X�~�߂�
			m_pLocus->Stop();
		}
	};
}

//*************************************************************
//	���������Ǘ��p
namespace OshiokiManager
{
	class Base
	{
	public:
		Base(Airou *pAirouPlayer, BasePlayer *pTarget) :m_pObj(new iex3DObj("DATA/CHR/Airou/airou_motion.IEM")), m_pAirouPlayer(pAirouPlayer), m_pTarget(pTarget), m_bAction(false){ m_pObj->SetMotion(0); }
		virtual ~Base()
		{
			delete m_pObj;
		}
		virtual void Update() = 0;
		virtual void Render() = 0;
		void Action(){ m_bAction = true; }
		bool isAction(){ return m_bAction; }
		virtual void Explosion(){}
	protected:

		// �����������s���v���C���[�A�����v���C���[
		Airou *m_pAirouPlayer;
		BasePlayer *m_pTarget;

		iex3DObj *m_pObj;

		// �A�N�V�����t���O
		bool m_bAction;

		// ���X�g�̒��g���������鏈��
		void ClearList(std::list<OshiokiAirou::OverDrive*> *pList)
		{
			for (auto it = pList->begin(); it != pList->end();)
			{
				delete (*it);
				it = pList->erase(it);
			}
		}
		void ClearList(std::list<OshiokiAirou::OverFlow*> *pList)
		{
			for (auto it = pList->begin(); it != pList->end();)
			{
				delete (*it);
				it = pList->erase(it);
			}
		}

		// ���X�g�̒��g���X�V���鏈��
		void UpdateList(std::list<OshiokiAirou::OverDrive*> *pList)
		{
			for (auto it = pList->begin(); it != pList->end();)
			{
				// �A�C���[�X�V
				(*it)->Update();

				// �����`�F�b�N
				if ((*it)->EraseOK())
				{
					delete (*it);
					it = pList->erase(it);
				}
				else it++;
			}
		}
		void UpdateList(std::list<OshiokiAirou::OverFlow*> *pList)
		{
			for (auto it = pList->begin(); it != pList->end();)
			{
				// �A�C���[�X�V
				(*it)->Update();

				// �����`�F�b�N
				if ((*it)->EraseOK())
				{
					delete (*it);
					it = pList->erase(it);
				}
				else it++;
			}
		}

		// ���X�g�̒��g��`�悷�鏈��
		void RenderList(std::list<OshiokiAirou::OverDrive*> *pList)
		{
			for (auto it : *pList) it->Render(m_pObj);
		}
		void RenderList(std::list<OshiokiAirou::OverFlow*> *pList)
		{
			for (auto it : *pList) it->Render(m_pObj);
		}
	};

	class OverDrive :public Base
	{
	private:
		int m_iCreateTimer;					// �A�C���[�����p�̎���
		int m_iCreateCount;					// �A�C���[���C����������
		const int c_NUM_CREATE = 73;		// �A�C���[�����̍�邩
		const Vector3 c_OSHIOKI_POS = Vector3(0, 30, 0);		// ��������d�u��������W
		int m_CreateInterval;	// 0.5�b��1��

		std::list <OshiokiAirou::OverDrive*> m_list;
	public:
		OverDrive(Airou *pAirouPlayer, BasePlayer *pTarget) :Base(pAirouPlayer, pTarget), m_iCreateTimer(0), m_CreateInterval(30), m_iCreateCount(0)
		{
			// ����̓����𕕂���
			pTarget->SetMoveUpdate(false);
		}
		~OverDrive()
		{
			ClearList(&m_list);
		}
		void Update()
		{
			// �A�C���[�v���C���[�̈ʒu�␳
			Vector3 AirouPlayerPos(m_pAirouPlayer->GetPos());
			const float ToTargetDistX(fabsf(AirouPlayerPos.x));
			if (ToTargetDistX < 10)
				AirouPlayerPos.x = (m_pAirouPlayer->GetTargetDir() == DIR::LEFT) ? 20.0f : -20.0f;			// �߂��̂ŋ����𗣂�
			else if (ToTargetDistX > 30)
				AirouPlayerPos.x = (m_pAirouPlayer->GetTargetDir() == DIR::LEFT) ? 20.0f : -20.0f;			// �����̂ŋ������k�߂�
			if (AirouPlayerPos.y < 30) AirouPlayerPos.y += .1f;
			m_pAirouPlayer->SetPos(AirouPlayerPos);

			// ����̈ʒu�␳
			m_pTarget->SetPos(m_pTarget->GetPos() * .5f + c_OSHIOKI_POS * .5f);

			// �I�u�W�F�A�j���[�V����
			m_pObj->Animation();

			// �A�C���[���X�g�X�V
			Base::UpdateList(&m_list);

			// �I��萶���񐔂܂ŒB���ĂȂ�������
			if (m_iCreateCount < c_NUM_CREATE)
			{
				// �������ԃJ�E���g�A�������Ԃ������琶��
				if (++m_iCreateTimer % m_CreateInterval == 0)
				{
					// ���񂾂�Ԋu�Z�����Ă���
					if (m_iCreateCount % 2 == 0) m_CreateInterval = max((int)(m_CreateInterval * .9f), 2);

					m_iCreateTimer = 0;
					m_list.push_back(new OshiokiAirou::OverDrive(m_pAirouPlayer, m_pTarget, (++m_iCreateCount == c_NUM_CREATE)));
				}
			}

			// �S�������Ă���I���
			else if (m_list.empty())
			{
				m_bAction = false;
			}
		}
		void Render()
		{
			// �A�C���[���X�g�`��
			Base::RenderList(&m_list);
		}
	};

	class OverFlow :public Base
	{
	private:
		int m_iCreateTimer;					// �A�C���[�����p�̎���
		int m_iCreateCount;					// �A�C���[���C����������
		const int c_NUM_CREATE = 14;		// �˂��h����Ȃ��A�C���[�����̍�邩
		const int c_NUM_CREATE_TSUKISASARU = 8;// �˂��h����A�C���[�����̍�邩
		const int c_CERATE_INTERVAL = 9;	// 0.5�b��1��
		bool m_bCreate;
		bool m_bFirstExplosion;
		bool m_bTsukisasiCreate;			// �˂��h����A�C���[����邩�ǂ����̃t���O
		std::list <OshiokiAirou::OverFlow*> m_list;

		BaseUVEffect *m_pExplosion;			// ����UV�G�t�F�N�g
		BaseUVEffect *m_pFireRing;
		BaseUVEffect *m_pFireWave;

	public:
		OverFlow(Airou *pAirouPlayer, BasePlayer *pTarget) :Base(pAirouPlayer, pTarget), m_bFirstExplosion(true), m_bCreate(true), m_iCreateTimer(0), m_bTsukisasiCreate(false), m_iCreateCount(0), m_pExplosion(new BreakImpactEffect), m_pFireRing(new FireRingEffect), m_pFireWave(new WaveEffect){}
		~OverFlow()
		{
			delete m_pExplosion;
			delete m_pFireRing;
			delete m_pFireWave;
			ClearList(&m_list);
		}
		void Update()
		{
			if (!m_bAction) return;

			// �A�C���[���X�g�X�V
			Base::UpdateList(&m_list);

			// �����G�t�F�N�g�X�V
			m_pExplosion->Update();
			m_pFireRing->Update();
			m_pFireWave->Update();

			// �������ԃJ�E���g�A�������Ԃ������琶��
			if (m_bCreate)
			{
				// �A�j���[�V����
				if (!m_bTsukisasiCreate)m_pObj->Animation();

				if (++m_iCreateTimer > (m_bTsukisasiCreate ? 0 : c_CERATE_INTERVAL))
				{
					m_iCreateTimer = 0;	// �^�C�}�[������
					m_iCreateCount++;	// ��������C���N�������g&�A�C���[�Z�b�g

					if (m_bTsukisasiCreate)
					{
						if (m_iCreateCount > c_NUM_CREATE_TSUKISASARU) m_bCreate = false;
					}
					else if (m_iCreateCount > c_NUM_CREATE)
					{
						m_iCreateCount = 0;
						m_bTsukisasiCreate = true;
						return;
					}
					m_list.push_back(new OshiokiAirou::OverFlow(m_pAirouPlayer, m_pTarget, m_bTsukisasiCreate, m_iCreateCount - 1));
				}
			}
		}
		void Render()
		{
			if (!m_bAction) return;

			// �A�C���[���X�g�`��
			Base::RenderList(&m_list);

			// �����G�t�F�N�g�`��
			m_pExplosion->Render();
			m_pFireWave->Render();
			m_pFireRing->Render();
		}
		void Explosion()
		{
			// �S����юU��[
			for (auto it : m_list) it->Explosion();

			if (m_bFirstExplosion)
			{
				// �����G�t�F�N�g����
				m_pExplosion->Action(Vector3(0, 0, 0), .5f, 1);
				m_pExplosion->Update();
				m_pFireWave->Action(Vector3(10, 5, 0), 0.5f, 1);
				m_pFireWave->Update();
				m_pFireRing->Action(Vector3(10, 10, 0), 2.5f, 3.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));
				m_pFireRing->Update();
				PointLightManager::GetInstance()->AddPointLight(Vector3(10, 10, 0), Vector3(1, 0.5, 0), 125, 2, 10, 2, 6);
				ParticleManager::EffectExplosion(Vector3(0, 0, -68));
				m_bFirstExplosion = false;
			}
			else
			{
				// �����G�t�F�N�g����
				m_pExplosion->Action(Vector3(0, -7, -74), 0.5f, 0.5f, Vector3(0.785f, 0.3f, 0), Vector3(0.785f, 0.3f, 0));
				m_pExplosion->Update();
				m_pFireWave->Action(Vector3(10, 5, -68), 0.5f, 1.0f, Vector3(0, 0, 1), Vector3(0, 0, 1));
				m_pFireWave->Update();
				m_pFireRing->Action(Vector3(10, 5, -68), .75f, 0.75f, Vector3(0, 0.4f, 0), Vector3(0, 0.4f, 0));
				m_pFireRing->Update();
				PointLightManager::GetInstance()->AddPointLight(Vector3(10, 10, 0), Vector3(1, 0.5, 0), 125, 2, 10, 2, 6);
			}
		}
	};

}