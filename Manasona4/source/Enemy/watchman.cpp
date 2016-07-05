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
//    基底
//**************************************************
void Enemy::Base::MoveUpdate()
{
	m_pos += m_MoveVec * m_speed;
}

//**************************************************
//    狼
//**************************************************
Enemy::Wolf::Wolf(tdn2DObj *image, tdn2DObj *pniku, tdn2DObj *pHone, int floor, float speed, int nikustopTime, bool unlimited) :
Base(image, floor, speed), m_pNikukutteru(pniku), m_EAT_NIKU_TIMER(nikustopTime), m_OrgSpeed(speed), m_pHoneImage(pHone), m_seID(TDNSOUND_PLAY_NONE), m_alpha(255),
m_bUNLIMITED(unlimited), m_ZanzouFrame(0)
{
	// アンリミ
	if (unlimited)
	{
		m_OrgSpeed *= 2;	// 速度2倍
	}

	// イニシャライザ書けない
	W = 120;
	H = 120;

	// 座標初期化
	m_pos.x = 1280;
	m_pos.y = (float)STAGE_POS_Y[floor] + LANE_WIDTH / 8;

	// モードポインタ初期亜k
	ModeFunk[(int)MODE::RUN] = &Wolf::Run;
	ModeFunk[(int)MODE::NIKU] = &Wolf::Niku;
	ModeFunk[(int)MODE::DEAD] = &Wolf::Dead;

	// 最初はしる
	ChangeMode(MODE::RUN);
}

Enemy::Wolf::~Wolf()
{
	for (auto it : m_ZanzouList) delete it;
}

void Enemy::Wolf::Update()
{
	// モード関数実行
	(this->*ModeFunk[(int)m_mode])();

	// 座標更新
	Enemy::Base::MoveUpdate();

	// アンリミ
	if (m_bUNLIMITED)
	{
		// 残像
		if (++m_ZanzouFrame > 1)
		{
			m_ZanzouFrame = 0;
			m_ZanzouList.push_back(new Zanzou(m_pos, Vector2((float)((m_AnimePanel % 4) * W), (float)((m_AnimePanel / 4) * H))));
		}

		// パーティクル
		Particle2dManager::Effect_Unlimited(m_pos + Vector2(60,60));

		// 残像更新
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
	// アニメ更新
	if (++m_AnimeFrame > 5)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 4) m_AnimePanel = 0;
	}
}


void Enemy::Wolf::Niku()
{
	// アニメ更新
	if (++m_AnimeFrame > 10)
	{
		m_AnimeFrame = 0;
		if (++m_AnimePanel > 2) m_AnimePanel = 0;
	}

	// 肉時間
	if (++m_EatNikuTimer > m_EAT_NIKU_TIMER)
	{
		EffectMgr.AddEffect((int)m_pos.x + 50, (int)m_pos.y + 30, EFFECT_TYPE::ClEAR);
		EffectMgr.AddEffect((int)m_pos.x - 80, (int)m_pos.y + 60, EFFECT_TYPE::PUSH);
		EffectMgr.AddEffect((int)m_pos.x + 50, (int)m_pos.y - 24, EFFECT_TYPE::EAT);

		// 煙
		Particle2dManager::Effect_FatSmoke(GetCenterPos());

		// 太る
		EnemyMgr->CreateFatWolf(this, m_type, m_SheepType);

		// SEの再生
		se->Play("ふとる", GetCenterPos());
		if (m_seID != TDNSOUND_PLAY_NONE) se->Stop("パクモグ", m_seID);
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
		// α下げていく
		m_alpha -= 2;
		if (m_alpha < 2)
			m_bErase = true;	// 死ぬ
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

	// アンリミ残像の描画
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
		m_seID = se->Play("パクモグ", m_pos, Vector2(0, 0), true);
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
//    肉食って太った狼
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
	// ループしてるSEを止める
	if (m_ReceiveSE != TDNSOUND_PLAY_NONE) se->Stop("太った羊押す", m_ReceiveSE);
}

void FatWolf::Update()
{
	// 基底クラスの更新
	DebuBase::Update();

	if (m_type == FAT_WOLF_TYPE::SMALL)m_AnimPanel = 0;	// 小デブはアニメなし

	if (m_SheepType == SHEEP_TYPE::GOLD)
	{
		static int KiraKiraCoolTime = 0;
		// きらきらパーティクル
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

	// テキストからデータ読み込み
	std::fstream ifs("DATA/Text/Param/tekidata.txt");
	assert(ifs);	// ファイルがない！

	char skip[64];	// 読み飛ばし用変数

	// 敵作成時間読み込み
	ifs >> skip;
	ifs >> m_CREATETIME;

	// 狼の速度読み込み
	ifs >> skip;
	ifs >> m_EnemySpeed[(int)ENEMY_TYPE::WOLF];

	// にく
	ifs >> skip;
	ifs >> m_NikuTime;

	// 時短
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
	// 生成時間初期化
	m_CreateTimer = 0;

	if (g_bExtraStage)
	{
		m_CreateSpeed = 1.0f;
		m_UnlimitedPercent = 100;
		m_bUnlimitedCreate = true;
	}
	else
	{
		// 生成間隔(1.0倍)
		m_CreateSpeed = m_ChangeSpeedLineList[m_ChangeSpeedLineList.size() - 1]->speed;
		// アンリミパーセント
		m_UnlimitedPercent = m_ChangeSpeedLineList[m_ChangeSpeedLineList.size() - 1]->U_percent;
		// アンリミ率でアンリミフラグけってい
		m_bUnlimitedCreate = (tdnRandom::Get(0, 99) < m_UnlimitedPercent);
	}

	// わーにんぐフラグ初期化
	m_bWarning = false;


	// 敵画像の読み込み
	m_pImages[(int)ENEMY_TYPE::WOLF] = new tdn2DObj("DATA/CHR/「！」左移動.png");
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
	// 狼大量バグ直す(デストラクタに書く)
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
		assert(0);	// 例外処理
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
			// アンリミ率でアンリミフラグけってい
			m_bUnlimitedCreate = (tdnRandom::Get(0, 99) < m_UnlimitedPercent);

			// 肉センサー
			Niku *pNiku = NikuMgr->GetNiku();
			if (pNiku)
			{
				if (pNiku->isSeted())m_NextFloor = pNiku->GetFloor();
			}

			// 肉なかったら次は1/2の確率でデブ羊フロア
			else if (rand() % 2)
			{
				if (!g_pSheepMgr->GetFatList()->empty())
				{
					m_NextFloor = (*g_pSheepMgr->GetFatList()->begin())->GetFloor();
				}
			}

			// ポップアップ
			EffectMgr.AddEffect(1100, STAGE_POS_Y[m_NextFloor] + LANE_WIDTH / 2, (m_bUnlimitedCreate) ? EFFECT_TYPE::DARK_NOTICE : EFFECT_TYPE::NOTICE);

			// SEの再生
			if (m_bUnlimitedCreate)
			{
				se->Play("アンリミ!");
			}
			else se->Play("!", Vector2(1100, (float)STAGE_POS_Y[m_NextFloor] + LANE_WIDTH / 2));

			// わーにんぐON
			m_bWarning = true;
		}
	}

	// 敵生成タイマー
	if (++m_CreateTimer > (int)(m_CREATETIME * m_CreateSpeed))
	{
		m_CreateTimer = 0;
		Create(m_NextFloor, ENEMY_TYPE::WOLF);

		// 次のフロア作成
		m_NextFloor = tdnRandom::Get(0, 2);

		m_bWarning = false;
	}

	for (auto it = m_list.begin(); it != m_list.end();)
	{
		// 敵たち更新
		(*it)->Update();

		// 消去フラグ立ってたら
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_list.erase(it);
		}
		else it++;
	}

	for (auto it = m_FatList.begin(); it != m_FatList.end();)
	{
		// 敵たち更新
		(*it)->Update();

		// 消去フラグ立ってたら
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
	// アンリミ狼描画
	for (auto it : m_list) if (it->isUnlimited())it->Render();

	// でぶ狼たち描画
	for (auto it : m_FatList)
		it->Render();
}

void EnemyManager::Clear()
{
	// 敵データ全消去
	for (auto it : m_list) delete it;
	m_list.clear();
	for (auto it : m_FatList)delete it;
	m_FatList.clear();
}

void EnemyManager::CreateFatWolf(Enemy::Wolf *wolf, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType)
{
	wolf->Erase();	// 元の羊を消去
	FatWolf *set = new FatWolf(m_pFatWolfImages[(int)(wolf->isUnlimited() ? ENEMY_TYPE::UNLIMITED_WOLF : ENEMY_TYPE::WOLF)][(int)SheepType], Vector2(wolf->GetPos().x - 60, (float)STAGE_POS_Y[wolf->GetFloor()] - 30), type, SheepType);// 太った狼生成
	set->SetFloor(wolf->GetFloor());	// フロア設定
	m_FatList.push_back(set);			// リストに突っ込む
}

void EnemyManager::CheckChangeSpeed(int WolfKillCount)
{
	// 敵生成スピード上げ
	for (auto& it : m_ChangeSpeedLineList)
	{
		// ライン超えてたら
		if (it->line < WolfKillCount)
		{
			//if (m_UnlimitedPercent > it->U_percent) break;

			// 生成間隔
			m_CreateSpeed = it->speed;

			// アンリミパーセント
			if (!g_bExtraStage)m_UnlimitedPercent = it->U_percent;
			break;

			//// リストから消去
			//delete (*it);
			//m_ChangeSpeedLineList.erase(it);
		}
	}
}