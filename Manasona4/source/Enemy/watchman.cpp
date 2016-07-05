#include "IEX_Expansion.h"
#include "../CharaBase/CharaBase.h"
#include "../Stage/StageMNG.h"
#include "../Sheep/Sheep.h"
#include "watchman.h"
#include "../Data/DataMNG.h"
#include "../Effect/EffectManager.h"
#include "../Niku/Niku.h"
#include "../Sound/SoundManager.h"
#include "../particle_2d/particle_2d.h"
#include "../UI/UIManager.h"
#include "../system/Framework.h"
#include "../Scene/Title.h"

//**************************************************
//    ���
//**************************************************
void Enemy::Base::MoveUpdate()
{
	m_pos += m_MoveVec * m_speed;
}

//**************************************************
//    �T
//**************************************************
Enemy::Wolf::Wolf(tdn2DObj *image, tdn2DObj *pniku, tdn2DObj *pHone, int floor, float speed, int nikustopTime, bool unlimited) :
Base(image, floor, speed), m_pNikukutteru(pniku), m_EAT_NIKU_TIMER(nikustopTime), m_OrgSpeed(speed), m_pHoneImage(pHone), m_seID(TDNSOUND_PLAY_NONE), m_alpha(255),
m_bUNLIMITED(unlimited), m_ZanzouFrame(0)
{
	// �A�����~
	if (unlimited)
	{
		m_OrgSpeed *= 2;	// ���x2�{
	}

	// �C�j�V�����C�U�����Ȃ�
	W = 120;
	H = 120;

	// ���W������
	m_pos.x = 1280;
	m_pos.y = (float)STAGE_POS_Y[floor] + LANE_WIDTH / 8;

	// ���[�h�|�C���^������k
	ModeFunk[(int)MODE::RUN] = &Wolf::Run;
	ModeFunk[(int)MODE::NIKU] = &Wolf::Niku;
	ModeFunk[(int)MODE::DEAD] = &Wolf::Dead;

	// �ŏ��͂���
	ChangeMode(MODE::RUN);
}

Enemy::Wolf::~Wolf()
{
	for (auto it : m_ZanzouList) delete it;
}

void Enemy::Wolf::Update()
{
	// ���[�h�֐����s
	(this->*ModeFunk[(int)m_mode])();

	// ���W�X�V
	Enemy::Base::MoveUpdate();

	// �A�����~
	if (m_bUNLIMITED)
	{
		// �c��
		if (++m_ZanzouFrame > 1)
		{
			m_ZanzouFrame = 0;
			m_ZanzouList.push_back(new Zanzou(m_pos, Vector2((float)((m_AnimePanel % 4) * W), (float)((m_AnimePanel / 4) * H))));
		}

		// �p�[�e�B�N��
		Particle2dManager::Effect_Unlimited(m_pos + Vector2(60,60));

		// �c���X�V
		for (auto it = m_ZanzouList.begin(); it != m_ZanzouList.end();)
		{
			if ((*it)->Update())
			{
				delete (*it);
				it = m_ZanzouList.erase(it);
			}
			else it++;
		}
	}
}

void Enemy::Wolf::Run()
{
	// �A�j���X�V
	if (++m_AnimeFrame > 5)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 4) m_AnimePanel = 0;
	}
}


void Enemy::Wolf::Niku()
{
	// �A�j���X�V
	if (++m_AnimeFrame > 10)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 2) m_AnimePanel = 0;
	}

	// ������
	if (++m_EatNikuTimer > m_EAT_NIKU_TIMER)
	{
		EffectMgr.AddEffect((int)m_pos.x + 50, (int)m_pos.y + 30, EFFECT_TYPE::ClEAR);
		EffectMgr.AddEffect((int)m_pos.x - 80, (int)m_pos.y + 60, EFFECT_TYPE::PUSH);
		EffectMgr.AddEffect((int)m_pos.x + 50, (int)m_pos.y - 24, EFFECT_TYPE::EAT);

		// ��
		Particle2dManager::Effect_FatSmoke(GetCenterPos());

		// ����
		EnemyMgr->CreateFatWolf(this, m_type, m_SheepType);

		// SE�̍Đ�
		se->Play("�ӂƂ�", GetCenterPos());
		if (m_seID != TDNSOUND_PLAY_NONE) se->Stop("�p�N���O", m_seID);
	}
}

void Enemy::Wolf::Dead()
{
	if (m_AnimePanel < 5)
	{
		if (++m_AnimeFrame > 4)
		{
			m_AnimeFrame = 0;
			m_AnimePanel++;
		}
	}
	else
	{
		// �������Ă���
		m_alpha -= 2;
		if (m_alpha < 2)
			m_bErase = true;	// ����
	}
}


void Enemy::Wolf::Render()
{
	tdn2DObj *pImage;
	switch (m_mode)
	{
	case Enemy::Wolf::MODE::RUN:
		pImage = m_pImage;
		break;
	case Enemy::Wolf::MODE::DEAD:
		pImage = m_pHoneImage;
		break;
	case Enemy::Wolf::MODE::NIKU:
		pImage = m_pNikukutteru;
		break;
	}

	// �A�����~�c���̕`��
	if(m_bUNLIMITED) for (auto& it : m_ZanzouList) it->Render(pImage);

	pImage->SetARGB(m_alpha, (BYTE)255, (BYTE)255, (BYTE)255);
	pImage->Render((int)m_pos.x, (int)m_pos.y, W, H, (m_AnimePanel % 4) * W, (m_AnimePanel / 4) * H, W, H);
}

void Enemy::Wolf::ChangeMode(MODE m)
{
	m_AnimeFrame = m_AnimePanel = 0;
	m_mode = m;
	switch (m)
	{
	case MODE::RUN:
		m_MoveVec = Vector2(-1, 0);
		m_speed = m_OrgSpeed;
		break;
	case MODE::NIKU:
		m_MoveVec = Vector2(-1, 0);
		m_speed = 0;
		m_EatNikuTimer = 0;
		m_seID = se->Play("�p�N���O", m_pos, Vector2(0, 0), true);
		break;
	case MODE::DEAD:
		m_MoveVec = Vector2(1, 0);
		m_speed = 0;
		break;
	default:
		break;
	}
}

//**************************************************
//    ���H���đ������T
//**************************************************
FatWolf::FatWolf(tdn2DObj *image, const Vector2 &pos, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType) :DebuBase(image, pos), m_type(type), m_SheepType(SheepType)
{
	switch (type)
	{
	case FAT_WOLF_TYPE::SMALL:
		m_radius = 90;
		break;
	case FAT_WOLF_TYPE::MIDDLE:
		m_radius = 90;
		break;

	case FAT_WOLF_TYPE::LARGE:
		m_radius = 120;
		break;
	}
}

FatWolf::~FatWolf()
{
	// ���[�v���Ă�SE���~�߂�
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("�������r����", m_ReceiveSE);
}

void FatWolf::Update()
{
	// ���N���X�̍X�V
	DebuBase::Update();

	if (m_type == FAT_WOLF_TYPE::SMALL)m_AnimPanel = 0;	// ���f�u�̓A�j���Ȃ�

	if (m_SheepType == SHEEP_TYPE::GOLD)
	{
		static int KiraKiraCoolTime = 0;
		// ���炫��p�[�e�B�N��
		if (++KiraKiraCoolTime > 4)
		{
			KiraKiraCoolTime = 0;
			Particle2dManager::Effect_KiraKira(GetCenterPos(), Vector2(m_radius, m_radius), 25.0f, 10.0f);
		}
	}
}

void FatWolf::Render()
{
	m_image->SetAngle(m_angle);
	m_image->Render((int)m_pos.x, (int)m_pos.y, 240, 240, 0, (int)m_type * 240, 240, 240);
}



//**************************************************

// manager

//**************************************************

EnemyManager::EnemyManager() :m_CreateTimer(0)
{
	m_list.clear();
	m_FatList.clear();

	// �e�L�X�g����f�[�^�ǂݍ���
	std::fstream ifs("DATA/Text/Param/tekidata.txt");
	assert(ifs);	// �t�@�C�����Ȃ��I

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	// �G�쐬���ԓǂݍ���
	ifs >> skip;
	ifs >> m_CREATETIME;

	// �T�̑��x�ǂݍ���
	ifs >> skip;
	ifs >> m_EnemySpeed[(int)ENEMY_TYPE::WOLF];

	// �ɂ�
	ifs >> skip;
	ifs >> m_NikuTime;

	// ���Z
	ifs >> skip;
	while (!ifs.eof())
	{
		ChangeSpeedLine *set = new ChangeSpeedLine;
		ifs >> set->line;
		ifs >> set->speed;
		ifs >> set->U_percent;
		m_ChangeSpeedLineList.push_back(set);
	}

	m_NextFloor = tdnRandom::Get(0, 2);
}

void EnemyManager::Initialize()
{
	// �������ԏ�����
	m_CreateTimer = 0;

	if (g_bExtraStage)
	{
		m_CreateSpeed = 1.0f;
		m_UnlimitedPercent = 100;
		m_bUnlimitedCreate = true;
	}
	else
	{
		// �����Ԋu(1.0�{)
		m_CreateSpeed = m_ChangeSpeedLineList[m_ChangeSpeedLineList.size() - 1]->speed;
		// �A�����~�p�[�Z���g
		m_UnlimitedPercent = m_ChangeSpeedLineList[m_ChangeSpeedLineList.size() - 1]->U_percent;
		// �A�����~���ŃA�����~�t���O�����Ă�
		m_bUnlimitedCreate = (tdnRandom::Get(0, 99) < m_UnlimitedPercent);
	}

	// ��[�ɂ񂮃t���O������
	m_bWarning = false;


	// �G�摜�̓ǂݍ���
	m_pImages[(int)ENEMY_TYPE::WOLF] = new tdn2DObj("DATA/CHR/�u�I�v���ړ�.png");
	m_pImages[(int)ENEMY_TYPE::UNLIMITED_WOLF] = new tdn2DObj("DATA/CHR/unlimited.png");
	m_pNikukutteru[(int)ENEMY_TYPE::WOLF] = new tdn2DObj("DATA/CHR/kuruma back.png");
	m_pNikukutteru[(int)ENEMY_TYPE::UNLIMITED_WOLF] = new tdn2DObj("DATA/CHR/Ukuruma back.png");
	m_pHoneImage = new tdn2DObj("DATA/CHR/hone_motion.png");
	m_pFatWolfImages[(int)ENEMY_TYPE::WOLF][(int)SHEEP_TYPE::NOMAL] = new tdn2DObj("DATA/CHR/sinnnyou tubureru.png");
	m_pFatWolfImages[(int)ENEMY_TYPE::WOLF][(int)SHEEP_TYPE::GOLD] = new tdn2DObj("DATA/CHR/sinnnyou hajike.png");
	m_pFatWolfImages[(int)ENEMY_TYPE::WOLF][(int)SHEEP_TYPE::REAL] = new tdn2DObj("DATA/CHR/sinnnyou detekuru.png");
	m_pFatWolfImages[(int)ENEMY_TYPE::UNLIMITED_WOLF][(int)SHEEP_TYPE::NOMAL] = new tdn2DObj("DATA/CHR/Usinnnyou tubureru.png");
	m_pFatWolfImages[(int)ENEMY_TYPE::UNLIMITED_WOLF][(int)SHEEP_TYPE::GOLD] = new tdn2DObj("DATA/CHR/Usinnnyou hajike.png");
	m_pFatWolfImages[(int)ENEMY_TYPE::UNLIMITED_WOLF][(int)SHEEP_TYPE::REAL] = new tdn2DObj("DATA/CHR/Usinnnyou detekuru.png");
}

EnemyManager::~EnemyManager()
{
	// �T��ʃo�O����(�f�X�g���N�^�ɏ���)
	for (auto it : m_ChangeSpeedLineList)delete it;
}

void EnemyManager::Release()
{
	FOR ((int)ENEMY_TYPE::MAX)
	{
		delete m_pImages[i];
		delete m_pNikukutteru[i];
		for (int j = 0; j < (int)SHEEP_TYPE::MAX; j++) delete m_pFatWolfImages[i][j];
	}
	Clear();
	delete m_pHoneImage;
	//FOR((int)SHEEP_TYPE::MAX) delete m_pFatWolfImages[i];
}

//**************************************************

void EnemyManager::Create(int floor, ENEMY_TYPE type)
{
	Enemy::Wolf *set = nullptr;
	int GazounoSoeji = (m_bUnlimitedCreate) ? (int)ENEMY_TYPE::UNLIMITED_WOLF : (int)ENEMY_TYPE::WOLF;

	switch (type)
	{
	case ENEMY_TYPE::WOLF:
		set = new Enemy::Wolf(m_pImages[GazounoSoeji], m_pNikukutteru[GazounoSoeji], m_pHoneImage, floor, m_EnemySpeed[(int)ENEMY_TYPE::WOLF], m_NikuTime, m_bUnlimitedCreate);
		break;
	default:
		assert(0);	// ��O����
		break;
	}
	m_list.push_back(set);
}

void EnemyManager::Update()
{
	int rest = (int)(m_CREATETIME * m_CreateSpeed) - m_CreateTimer;
	if (!m_bWarning)
	{
		if (rest <= 80 && rest >= 50)
		{
			// �A�����~���ŃA�����~�t���O�����Ă�
			m_bUnlimitedCreate = (tdnRandom::Get(0, 99) < m_UnlimitedPercent);

			// ���Z���T�[
			Niku *pNiku = NikuMgr->GetNiku();
			if (pNiku)
			{
				if (pNiku->isSeted())m_NextFloor = pNiku->GetFloor();
			}

			// ���Ȃ������玟��1/2�̊m���Ńf�u�r�t���A
			else if (rand() % 2)
			{
				if (!g_pSheepMgr->GetFatList()->empty())
				{
					m_NextFloor = (*g_pSheepMgr->GetFatList()->begin())->GetFloor();
				}
			}

			// �|�b�v�A�b�v
			EffectMgr.AddEffect(1100, STAGE_POS_Y[m_NextFloor] + LANE_WIDTH / 2, (m_bUnlimitedCreate) ? EFFECT_TYPE::DARK_NOTICE : EFFECT_TYPE::NOTICE);

			// SE�̍Đ�
			if (m_bUnlimitedCreate)
			{
				se->Play("�A�����~!");
			}
			else se->Play("!", Vector2(1100, (float)STAGE_POS_Y[m_NextFloor] + LANE_WIDTH / 2));

			// ��[�ɂ�ON
			m_bWarning = true;
		}
	}

	// �G�����^�C�}�[
	if (++m_CreateTimer > (int)(m_CREATETIME * m_CreateSpeed))
	{
		m_CreateTimer = 0;
		Create(m_NextFloor, ENEMY_TYPE::WOLF);

		// ���̃t���A�쐬
		m_NextFloor = tdnRandom::Get(0, 2);

		m_bWarning = false;
	}

	for (auto it = m_list.begin(); it != m_list.end();)
	{
		// �G�����X�V
		(*it)->Update();

		// �����t���O�����Ă���
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_list.erase(it);
		}
		else it++;
	}

	for (auto it = m_FatList.begin(); it != m_FatList.end();)
	{
		// �G�����X�V
		(*it)->Update();

		// �����t���O�����Ă���
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_FatList.erase(it);
		}
		else it++;
	}
}

void EnemyManager::Render()
{
	for (auto it: m_list) if(!it->isUnlimited())it->Render();
}

void EnemyManager::RenderFat()
{
	// �A�����~�T�`��
	for (auto it : m_list) if (it->isUnlimited())it->Render();

	// �łԘT�����`��
	for (auto it : m_FatList)
		it->Render();
}

void EnemyManager::Clear()
{
	// �G�f�[�^�S����
	for (auto it : m_list) delete it;
	m_list.clear();
	for (auto it : m_FatList)delete it;
	m_FatList.clear();
}

void EnemyManager::CreateFatWolf(Enemy::Wolf *wolf, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType)
{
	wolf->Erase();	// ���̗r������
	FatWolf *set = new FatWolf(m_pFatWolfImages[(int)(wolf->isUnlimited() ? ENEMY_TYPE::UNLIMITED_WOLF : ENEMY_TYPE::WOLF)][(int)SheepType], Vector2(wolf->GetPos().x - 60, (float)STAGE_POS_Y[wolf->GetFloor()] - 30), type, SheepType);// �������T����
	set->SetFloor(wolf->GetFloor());	// �t���A�ݒ�
	m_FatList.push_back(set);			// ���X�g�ɓ˂�����
}

void EnemyManager::CheckChangeSpeed(int WolfKillCount)
{
	// �G�����X�s�[�h�グ
	for (auto& it : m_ChangeSpeedLineList)
	{
		// ���C�������Ă���
		if (it->line < WolfKillCount)
		{
			//if (m_UnlimitedPercent > it->U_percent) break;

			// �����Ԋu
			m_CreateSpeed = it->speed;

			// �A�����~�p�[�Z���g
			if (!g_bExtraStage)m_UnlimitedPercent = it->U_percent;
			break;

			//// ���X�g�������
			//delete (*it);
			//m_ChangeSpeedLineList.erase(it);
		}
	}
}