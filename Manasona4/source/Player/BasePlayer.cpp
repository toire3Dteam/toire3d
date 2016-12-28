#include "TDNLIB.h"
#include "BasePlayer.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"
#include "../Stand/Stand.h"
#include "../Sound/SoundManager.h"
#include "AI\AI.h"
#include "../Effect/Particle.h"
#include "../system/System.h"
#include "../PointLight/PointLight.h"
#include "../DeferredEx/DeferredEx.h"
#include "../Data/SelectData.h"

//_/_/_/_/_/_/__/_/__/_/__/_/__
// 定数
//_/_/_/_/__/_/__/_/__/_/__/_/_
const float BasePlayer::c_END_MOVE_LINE = .25f;		// 移動が終わって待機に戻る移動値ライン
const int	BasePlayer::c_RUSH_AHEAD_START = 5;		// ラッシュ中の先行入力受付開始フレーム
const float BasePlayer::c_GRAVITY = .1f;
const float BasePlayer::c_MAX_JUMP = 1.9f;

const int BasePlayer::c_RECOVERY_FLAME = 32;			// リカバリーステートにいる時間

const int BasePlayer::c_OVERDRIVE_MAX_GAGE = 100;	// 覚醒ゲージの最大値
const int BasePlayer::c_OVERDRIVE_MAX_TIME = 480;	// 覚醒が切れるまでの時間

const int BasePlayer::c_THROW_ESCAPE_FRAME = 15;	// 投げぬけの猶予フレーム
const int BasePlayer::c_THROW_MISS_FRAME = 30;	// 投げ外しロスのフレーム(全キャラ共通だろうという考え)
const int BasePlayer::c_THROW_RELEASE_FRAME = 15;	// 投げ抜けで、パシンてなってる間のフレーム(これも全キャラ共通だろう)
const int BasePlayer::c_WINNER_TIME = 180;
const float BasePlayer::c_GUARD_DISTANCE = 32.0f;
const int BasePlayer::c_FIRST_HIT_ADD_DAMAGE = 300;	// 初段ヒット加算ダメージ

// キャラクター基本情報読み込み
void BasePlayer::LoadCharacterParam(LPSTR filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "parameterのテキスト入ってない");

	std::string sDirectory(tdnFile::GetDirectoryPath(filename));

	char skip[128];	// 読み飛ばし用変数

	// メッシュ
	char path[MAX_PATH];
	ifs >> skip;
	ifs >> path;
	assert(!m_pDefaultObj);	// ここでnullじゃないならおかしい
	m_pDefaultObj = new iex3DObj((char*)(sDirectory + "/" + path).c_str());
	m_pDefaultObj->SetPos(m_vPos);
	m_pDefaultObj->Update();

	// 必殺用メッシュ
	ifs >> skip;
	ifs >> path;
	assert(!m_pHHDOFObj);	// ここでnullじゃないならおかしい
	m_pHHDOFObj = new iex3DObj((char*)(sDirectory + "/" + path).c_str());
	m_pHHDOFObj->SetAngle(PI);
	m_pHHDOFObj->Update();


	// 2Pカラーテクスチャ(ごみくそ)
	//ifs >> skip;
	//ifs >> path;
	//if (m_side == SIDE::RIGHT)
	//{
	//	m_pDefaultObj->SetTexture(tdnTexture::Load((char*)(sDirectory + "/" + path).c_str()), 0);
	//	m_pHHDOFObj->SetTexture(tdnTexture::Load((char*)(sDirectory + "/" + path).c_str()), 0);
	//}

	// 本体の判定
	ifs >> skip;
	ifs >> m_tagCharacterParam.HitSquare.width;
	ifs >> m_tagCharacterParam.HitSquare.height;
	ifs >> m_tagCharacterParam.HitSquare.pos.y;

	// 最大速度(ダッシュ速度
	ifs >> skip;
	ifs >> m_tagCharacterParam.fMaxSpeed;

	// 空中ダッシュ速度
	ifs >> skip;
	ifs >> m_tagCharacterParam.fAerialDashSpeed;

	// 最大HP
	ifs >> skip;
	ifs >> m_tagCharacterParam.iMaxHP;
	m_iHP = m_tagCharacterParam.iMaxHP;

	// 初期情報を保存
	memcpy_s(&m_tagOrgCharacterParam, sizeof(CharacterParam), &m_tagCharacterParam, sizeof(CharacterParam));
}

// アクションフレーム情報読み込み
void BasePlayer::LoadAttackFrameList(LPSTR filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "アタックフレームのテキスト入ってない");

	// ★ポインタ型の比較は「持っているアドレスが正しいかどうか」(★文字列自体の比較をしてくれない)で判断するので、意図した動作はしない
	static const std::map<std::string, BASE_ACTION_STATE> IDList
	{
		{ "[[エスケープ]]", BASE_ACTION_STATE::ESCAPE },
		{ "[[バックステップ]]", BASE_ACTION_STATE::BACKSTEP },
		{ "[[ワンモア覚醒]]", BASE_ACTION_STATE::OVERDRIVE_ONEMORE },
		{ "[[スタンド発動]]", BASE_ACTION_STATE::STAND },
		{ "[[バースト覚醒]]", BASE_ACTION_STATE::OVERDRIVE_BURST },
		{ "[[通常攻撃1段目]]", BASE_ACTION_STATE::RUSH1 },
		{ "[[通常攻撃2段目]]", BASE_ACTION_STATE::RUSH2 },
		{ "[[通常攻撃3段目]]", BASE_ACTION_STATE::RUSH3 },
		{ "[[対空攻撃]]", BASE_ACTION_STATE::ANTI_AIR },
		{ "[[中段攻撃]]", BASE_ACTION_STATE::DOKKOI_ATTACK },
		{ "[[しゃがみ下段攻撃]]", BASE_ACTION_STATE::SQUAT_ATTACK },
		{ "[[あしばらい攻撃]]", BASE_ACTION_STATE::DOWN_ATTACK },
		{ "[[空中攻撃]]", BASE_ACTION_STATE::AERIAL_ATTACK },
		{ "[[空中下攻撃]]", BASE_ACTION_STATE::AERIALDROP },
		{ "[[逆切れ攻撃]]", BASE_ACTION_STATE::INVINCIBLE_ATTACK },
		{ "[[キャラ固有地上ニュートラル]]", BASE_ACTION_STATE::SKILL },
		{ "[[キャラ固有2]]", BASE_ACTION_STATE::SKILL2 },
		{ "[[キャラ固有地上しゃがみ]]", BASE_ACTION_STATE::SKILL_SQUAT },
		{ "[[キャラ固有空中ニュートラル]]", BASE_ACTION_STATE::SKILL_AERIAL },
		{ "[[キャラ固有空中下]]", BASE_ACTION_STATE::SKILL_AERIALDROP },
		{ "[[掴み]]", BASE_ACTION_STATE::THROW },
		{ "[[投げ成功]]", BASE_ACTION_STATE::THROW_SUCCESS },
		{ "[[必殺技]]", BASE_ACTION_STATE::HEAVEHO_DRIVE },
		{ "[[超必殺技]]", BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW }
	};

	while (!ifs.eof())
	{
		// ID読み込み
		char read[64];
		ifs >> read;

		// エラーチェック
		MyAssert(read[0] == '[', "攻撃データのテキストがずれてる");
		//MyAssert(IDList.count(ID) != 0, "攻撃データのIDが間違えている可能性\n%s", ID);

		std::string ID(read);

		// 始動、持続、硬直フレームを読み込み
		int count(0);
		int iActionType((int)IDList.at(ID));

		FOR((int)FRAME_STATE::END)
		{
			int frame;
			ifs >> frame;
			for (int j = 0; j < frame; j++)
			{
				m_ActionFrameList[iActionType][count++] = (FRAME_STATE)i;
				MyAssert(count < 256, "アクションフレームの合計は255以下にしてください");
			}
		}
		// 終端(全部ENDで初期化してるから大丈夫だけど一応)
		m_ActionFrameList[iActionType][count] = FRAME_STATE::END;
	}

	// フレームカウント
	{
		FOR(FRAME_MAX - 1) m_ActionFrameList[(int)BASE_ACTION_STATE::FRAMECOUNT][i] = FRAME_STATE::START;
		m_ActionFrameList[(int)BASE_ACTION_STATE::FRAMECOUNT][FRAME_MAX - 1] = FRAME_STATE::END;
	}
}

void BasePlayer::LoadAttackDatas(LPSTR filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "攻撃データのテキスト入ってない");

	static const std::map<LPSTR, BASE_ACTION_STATE> IDList
	{
		{ "[[通常攻撃1段目]]",				BASE_ACTION_STATE::RUSH1 },
		{ "[[通常攻撃2段目]]",				BASE_ACTION_STATE::RUSH2 },
		{ "[[通常攻撃3段目]]",				BASE_ACTION_STATE::RUSH3 },
		{ "[[対空攻撃]]",					BASE_ACTION_STATE::ANTI_AIR },
		{ "[[中段攻撃]]",					BASE_ACTION_STATE::DOKKOI_ATTACK },
		{ "[[しゃがみ下段攻撃]]",			BASE_ACTION_STATE::SQUAT_ATTACK },
		{ "[[あしばらい攻撃]]",				BASE_ACTION_STATE::DOWN_ATTACK },
		{ "[[空中攻撃]]",					BASE_ACTION_STATE::AERIAL_ATTACK },
		{ "[[空中下攻撃]]",					BASE_ACTION_STATE::AERIALDROP },
		{ "[[逆切れ攻撃]]",					BASE_ACTION_STATE::INVINCIBLE_ATTACK },
		{ "[[キャラ固有地上ニュートラル]]", BASE_ACTION_STATE::SKILL },
		{ "[[キャラ固有地上しゃがみ]]",		BASE_ACTION_STATE::SKILL_SQUAT },
		{ "[[キャラ固有空中ニュートラル]]", BASE_ACTION_STATE::SKILL_AERIAL },
		{ "[[キャラ固有空中下]]",			BASE_ACTION_STATE::SKILL_AERIALDROP },
		{ "[[掴み]]",						BASE_ACTION_STATE::THROW },
		{ "[[投げ成功]]",					BASE_ACTION_STATE::THROW_SUCCESS },
		{ "[[必殺技]]",						BASE_ACTION_STATE::HEAVEHO_DRIVE },
		{ "[[超必殺技]]",					BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW }
	};

	while (!ifs.eof())
	{
		// 読み飛ばし用変数
		//char skip[128];

		// ID読み込み
		{
			char ID[64];
			ifs >> ID;	// 読み飛ばし用だけど再利用

			// エラーチェック
			MyAssert(ID[0] == '[', "攻撃データのテキストがずれてる");
			MyAssert(IDList.count(ID) != 0, "攻撃データのIDが間違えている可能性\n%s", ID);
		}

		// ここにあるエリアは地上ヒットと空中ヒットとか関係なく共通の情報
		//int HitScore;				// 加算されるスコア
		//int damage;					// 与えるダメージ
		//bool bHit;					// 当たったかどうか(★多段ヒット防止用)多段ヒットしてほしい攻撃だと、また考える必要がある
		//bool bHitSuccess;			// 攻撃がガードされずに当たったかどうか
		//int WhiffDelayFrame;		// 何フレーム後に空振りのSEを再生するか(WhiffSEを使わない(nullptr)なら別に設定しなくてもいい)
		//EFFECT_TYPE HitEffectType;	// エフェクトのタイプ
		//LPCSTR HitSE;				// 当たったときに鳴らすSE
		//LPCSTR WhiffSE;				// 空振りSE
		//EFFECT_TYPE WhiffEffectType;	// 振りエフェクト
		//int pierceLV;				// 貫通レベル
		//bool bAntiAir;				// 対空攻撃かどうか　(追加)
		//bool bFinish;				// フィニッシュになる攻撃かどうか
		//ANTIGUARD_ATTACK AntiGuard;	// ガードを突き破る攻撃のタイプ
		//ATTACK_ATTRIBUTE attribute;	// 攻撃の属性(弾、投げ、打撃)
		//SHAKE_CAMERA_INFO ShakeCameraInfo;	// カメラ振動用構造体
		//int GuardRecoveryFrame;		// ガードされたときに「ガードしている相手に与える」硬直時間
		//float fGuardKnockBackPower;	// ガードさせたときのけぞり力
		//float fComboRate;			// コンボ補正(0.8なら次の攻撃に0.8倍の補正がかかる、補正は掛け合わされる)
		//
		//
		//CollisionShape::Square *pCollisionShape;	// ★あたり判定形状(四角にする)
	}
}

/*
コンストラクタにおいて、初期化子リストとして各メンバーのデフォルトコンストラクタを呼び出す形式
「memberVariableName()」で記述することにより、
組込の値型、enum型やポインタ型だけでなく、
固定長配列もPOD型構造体もすべてゼロクリアできます

（ただし配列やPOD構造体のメンバーをゼロ以外の数で初期化するには、
後述するようにC++11のinitializer list/uniform initializationにフル対応したコンパイラが必要になります）。
*/
BasePlayer::BasePlayer(SIDE side, const SideData &data) :
BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + (int)side)),

// m_tagCharacterParam{} だともれなくアウト
m_tagCharacterParam(CharacterParam{}),
m_tagOrgCharacterParam(CharacterParam{}),

m_vPos(0,0,0),
m_vMove(0,0,0),
m_bMoveUpdate(true),

m_fAngleY(0),
m_dir(DIR::LEFT),
m_TargetDir(DIR::LEFT),

//m_ActionFrameList(),
//m_ActionDatas(),
m_eActionState(BASE_ACTION_STATE::NO_ACTION),
m_iCurrentActionFrame(0),
m_iRushStep(0),
m_eSkillActionType(SKILL_ACTION_TYPE::NO_ACTION),

m_bLand(true),
m_bSquat(false),
m_bAerialJump(true),
m_bAerialDash(false),
m_iAerialDashFrame(0),

//m_RecoveryDamageCount(),
m_iRecoveryFrame(0),
m_iHitStopFrame(0),// ★★★バグの原因　ひっとすトップ初期化のし忘れ。
m_bDown(false),

m_eGuardState(GUARD_STATE::NO_GUARD),
m_bGuardSuccess(false),

m_iInvincibleLV(0),
m_iInvincibleTime(0),
m_fInvincibleColRate(0),
m_iInvincibleColRateFlame(0),
m_bInvincibleColRateUpFlag(false),
m_bEscape(false),

//m_iMotionNumbers(),

m_iHP(0),
m_fDamageRate(0),

m_iScore(0),
m_iCollectScore(0),

//m_iInputList(),
//m_wCommandHistory(),
m_wAheadCommand(0),

m_pStand(nullptr),

m_pTargetPlayer(nullptr),

m_pDefaultObj(nullptr),
m_pHHDOFObj(nullptr),
m_pObj(nullptr),
m_pSpeedLine(nullptr),
m_pFacePic(nullptr),

m_pPanelEffectMGR(nullptr),
m_pUVEffectMGR(nullptr),
m_pThrowMark(nullptr),

m_OverDriveGage(0),
m_bOverDrive(false),
m_OverDriveFrame(0),
m_OverDriveType(OVERDRIVE_TYPE::ONEMORE),

m_bWillPower(false),
m_iSpGettingFrame(0),

m_iDeviceID(data.iDeviceID),
m_side(side),
m_pStateMachine(nullptr),
m_pAI(nullptr),
m_bAI(data.bAI),

m_iDashFrame(0),

m_iHeavehoStopTimer(0),
m_iHeaveHoDriveOverFlowFrame(0),
m_bLastOneWin(false),

m_fOrangeColRate(0),
m_fMagentaColRate(0),

m_bGameTimerStopFlag(false),

m_iWinNum(0),

m_bNotOverDrive(false),

m_pCutinPic(nullptr),
m_pName("None")
{
	// スタンド
	switch (data.ePartner)
	{
	case PARTNER::MOKOI:
		m_pStand = new Stand::Mokoi(this);
		break;
	case PARTNER::MAYA:
		m_pStand = new Stand::Maya(this);
		break;
	default:
		assert(0);
		break;
	}

	// エフェクトマネージャー
	m_pPanelEffectMGR = new PanelEffectManager();
	m_pUVEffectMGR	 = new UVEffectManager();
	m_pThrowMark = new tdn2DAnim("Data/Effect/ThrowMark.png");
	m_pThrowMark->OrderShrink(6, 1.0f, 2.5f);


	// 初期化
	memset(m_iInputList, 0, sizeof(m_iInputList));
	ZeroMemory(&m_jump, sizeof(Jump));
	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<BasePlayer>(this);
	m_pStateMachine->SetGlobalState(BasePlayerState::Global::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(BasePlayerState::Wait::GetInstance());
	m_pStateMachine->SetPreviousState(BasePlayerState::Wait::GetInstance());

	// AIかどうかで　分岐
	//if(bAI) m_pAI = new AI(m_deviceID, this);
	m_pAI = nullptr;

	// IDで座標の分岐
	switch ((int)m_side)
	{
	case (int)SIDE::LEFT:
		m_vPos.Set(-20, 0, 0);
		m_TargetDir = DIR::RIGHT;
		break;

	case (int)SIDE::RIGHT:
		m_vPos.Set(20, 0, 0);
		m_TargetDir = DIR::LEFT;
		break;
	}

	m_fAngleY = DIR_ANGLE[(int)m_dir];

	m_RecoveryDamageCount.clear();

	// 入力リスト初期化
	memset(m_iInputList, 0, sizeof(m_iInputList));

	// コマンド履歴初期化
	memset(m_wCommandHistory, (int)PLAYER_COMMAND_BIT::NEUTRAL, sizeof(m_wCommandHistory));

	// モーション初期化(キャラごとの初期化を設定したい→Reset関数をキャラクター固有ロードより下に書くことに→モーションの初期化が来る前にモーションを設定してしまう→じゃあ先ここで全部0にしよう→最初の待機モーションが0であることが前提のコードになってしまう)
	memset(m_iMotionNumbers, 0, sizeof(m_iMotionNumbers));
	
	// アクションフレームリスト初期化(memsetでやるとなぜか変な値が入ってた)
	FOR((int)BASE_ACTION_STATE::END)
	{
		m_ActionDatas[i] = ActionData{};
		for (int j = 0; j < FRAME_MAX; j++) m_ActionFrameList[i][j] = FRAME_STATE::END;
	}

	//tdnDebug::OutPutLog("[初期化チェック}\n");
	//tdnDebug::OutPutLog("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, ");
}

void BasePlayer::Reset()
{
	/* [int型]の初期化 */

	/* 0で初期化 */
	m_iInvincibleLV =
	m_iInvincibleTime =
	m_iCurrentActionFrame =
	m_iRecoveryFrame =
	m_iScore =
	m_iCollectScore =
	m_iHitStopFrame =
	m_iInvincibleColRateFlame =
	//m_OverDriveGage =			// ラウンド引継ぎ
	m_OverDriveFrame =
	m_iHP =
	m_iHeavehoStopTimer =
	m_iHeaveHoDriveOverFlowFrame =
	m_iAerialDashFrame =
	m_iRushStep = 
	m_wAheadCommand =
	m_iDashFrame=
	m_iSpGettingFrame = 
	0;

	m_iHP = m_tagCharacterParam.iMaxHP;	// キャラごとのHP

	/* [float型]の初期化 */

	/* 0で初期化 */
	m_fInvincibleColRate =
	0.0f;

	/* 1で初期化 */
	m_fDamageRate =
	1.0f;


	/* [bool型]の初期化 */

	/* trueで初期化ゾーン */
	m_bAerialJump =
	m_bMoveUpdate =
	true;

	/* falseで初期化ゾーン */
	m_bEscape =
	m_bInvincibleColRateUpFlag =
	m_bOverDrive =
	m_bGameTimerStopFlag =
	m_bAerialDash =
	m_bLand=
	m_bSquat=
	m_bWillPower=
	m_bDown=
	m_bNotOverDrive=
	m_bGuardSuccess=
	false;


	/* [ポインタ型]の初期化(newしてるやつは極力そのまま) */
	//m_pTargetPlayer = nullptr;
	if (m_pDefaultObj) m_pDefaultObj->SetMotion(m_iMotionNumbers[(int)MOTION_TYPE::WAIT]);
	m_pObj = m_pDefaultObj;
	//m_pStateMachine->SetCurrentState(BasePlayerState::Wait::GetInstance()); 
	m_pStateMachine->ChangeState(BasePlayerState::Wait::GetInstance());			// [11/28] リセット時はもうステートが設定されているのチェンジで
	m_pStateMachine->SetPreviousState(BasePlayerState::Wait::GetInstance());

	/* [列挙型]の初期化 */
	m_eSkillActionType = SKILL_ACTION_TYPE::NO_ACTION;
	m_eActionState = BASE_ACTION_STATE::NO_ACTION;
	m_OverDriveType = OVERDRIVE_TYPE::BURST;
	m_eGuardState = GUARD_STATE::NO_GUARD;

	/* [構造体]の初期化 */
	memcpy_s(&m_tagCharacterParam, sizeof(CharacterParam), &m_tagOrgCharacterParam, sizeof(CharacterParam));
	m_vMove.Set(0, 0, 0);

	// IDで座標の分岐
	switch ((int)m_side)
	{
	case (int)SIDE::LEFT:
		m_vPos.Set(-20, 0, 0);
		m_TargetDir = DIR::RIGHT;
		break;

	case (int)SIDE::RIGHT:
		m_vPos.Set(20, 0, 0);
		m_TargetDir = DIR::LEFT;
		break;
	}


	/* [配列]の初期化 */
	memset(m_iInputList, 0, sizeof(m_iInputList));
	memset(m_wCommandHistory, (int)PLAYER_COMMAND_BIT::NEUTRAL, sizeof(m_wCommandHistory));



	// コンボ補正初期化
	m_RecoveryDamageCount.clear();

	// パートナーのゲージも初期化
	m_pStand->SetStandGage(0);
	m_pStand->SetStandStock(0);

}

void BasePlayer::InitAI(AI_TYPE type)
{
	// AIフラグかつ、AIがnewされてなかったら
	//if (m_pAI) return;
	// 仮でAIの種類タイプ
	if (m_bAI) m_pAI = new AI(m_side, this, type);
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pDefaultObj);
	SAFE_DELETE(m_pStateMachine);
	SAFE_DELETE(m_pStand);
	SAFE_DELETE(m_pPanelEffectMGR);
	SAFE_DELETE(m_pUVEffectMGR);
	SAFE_DELETE(m_pThrowMark);
	SAFE_DELETE(m_pAI);
	//SAFE_DELETE(m_pComboUI);
	SAFE_DELETE(m_pHHDOFObj);
	SAFE_DELETE(m_pFacePic);
	SAFE_DELETE(m_pSpeedLine);
	SAFE_DELETE(m_pCutinPic);
}

void BasePlayer::Update(PLAYER_UPDATE flag)
{
	//bool bOutLog(false);

	// デバッグ
	if (KeyBoardTRG(KB_8))
	{
		// 50回復
		m_OverDriveGage += 50;
		AddEffectAction(m_vPos, EFFECT_TYPE::WILL_POWER);
	}
	if (KeyBoardTRG(KB_9))
	{
		m_iHP = m_tagCharacterParam.iMaxHP;
	}
	if (KeyBoardTRG(KB_0))
	{
		m_iHP = 3000;
		//bOutLog = true;
	}

	//if (bOutLog) tdnStopWatch::Start();
	// 覚醒の更新
	OverDriveUpdate();

	// ガードエフェクト更新
	GuardEffectUpdate();

	// 根性値発動用の更新
	WillPowerUpdate(flag);

	// 1more覚醒していたらスタンドの動きを止める
	if (GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == false)
	{
		// スタンド更新
		m_pStand->Update((flag == PLAYER_UPDATE::CONTROL_OK));
	}

	// 無敵時間の更新
	if (KeyBoardTRG(KB_H))
	{
		SetInvincible(40, 1);
	}
	InvincibleUpdate();

	// 光色の更新
	ColorUpdate();

	//if (bOutLog)
	//{
	//	tdnStopWatch::End();
	//	tdnDebug::OutPutLog("プレイヤー%s:\n更新1にかかった時間 %.1lf\n",(m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
	//}

	//if (bOutLog) tdnStopWatch::Start();

	// 入力受付
	if (flag != PLAYER_UPDATE::CONTROL_NO)
	{
		// 0フレーム目にコマンドフラグを入れるので、それの前にコマンドビットリストを上に押し上げる

		// まず、入れる前のやつを保存
		WORD temp(m_wCommandHistory[0]);

		for (int i = 1; i < c_COMMAND_FRAME_MAX; i++)
		{
			// 繰り下げ処理
			//for (int i2 = i + 1; i2 < c_COMMAND_FRAME_MAX; i2++)
			{
				WORD save(m_wCommandHistory[i]);
				m_wCommandHistory[i] = temp;

				temp = save;
			}
		}

		// 最初neutralに戻す(★0フレーム目から)
		m_wCommandHistory[0] &= 0;

		if (m_bAI)	AIControl();
		else		Control();
	}
	else
	{
		memset(m_iInputList, 0, sizeof(m_iInputList));
		memset(m_wCommandHistory, 0, sizeof(m_wCommandHistory));
	}

	//if (bOutLog)
	//{
	//	tdnStopWatch::End();
	//	tdnDebug::OutPutLog("プレイヤー%s:\n入力にかかった時間 %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
	//}

	// ★ヒットストップ中
	if (m_iHitStopFrame > 0)
	{
		m_iHitStopFrame--;

		// ★先行入力受付(押した瞬間だけ取得に変更)
		AheadCommandUpdate();
	}
	// ヒットストップが言わるまで動かない
	else
	{
		// ★硬直時間のデクリメント
		if (m_iRecoveryFrame > 0)
			m_iRecoveryFrame--;
		else m_fDamageRate = 1.0f;	// (TODOコンボ切れる1フレームで補正が切れてるのにコンボが入る(アイルー1,2,下,スキル下で3400がでる))コンボ切れたら、ダメージレートを戻す

		// アクションフレームの更新
		if (isFrameAction())
		{
			// 攻撃フレームなら
			if (m_ActionDatas[(int)m_eActionState].isAttackData())
			{
				// 今の経過時間とディレイフレームになったら
				if (m_ActionDatas[(int)m_eActionState].pAttackData->WhiffDelayFrame == m_iCurrentActionFrame)
				{
					// (A列車)ここで攻撃判定が発動した瞬間を取ってきてる
					
					// 振りエフェクト発動（仮）！
					AddEffectAction(m_vPos + Vector3(0, 5, -3) , m_ActionDatas[(int)m_eActionState].pAttackData->WhiffEffectType);

					LPCSTR SE_ID = m_ActionDatas[(int)m_eActionState].pAttackData->WhiffSE;
					// 空振りSE入ってたら
					if (SE_ID)
					{
						// ディレイフレーム経過したら再生
						se->Play((LPSTR)SE_ID);
					}
				}

				// 判定復活フレーム
				if (m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame] == FRAME_STATE::RECOVERY_HIT)
				{
					// 判定リセット
					GetAttackData()->bHit = GetAttackData()->bHitSuccess = false;
				}

			}

			// モーションのフレームを更新！
			m_iCurrentActionFrame++;

			// フレーム最後まで再生したら
			if (m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame] == FRAME_STATE::END)
			{
				// ★アクションステート解除
				m_eActionState = BASE_ACTION_STATE::NO_ACTION;
			}
		}



		// ★自分の向きを設定
		// このif文に入ってるやつ以外だったら振り向く
		if (!m_pStateMachine->isInState(*BasePlayerState::Skill::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::SquatAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::DownAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::KnockDown::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::Run::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::DownFall::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::Jump::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialJump::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::Fall::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::Escape::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialDash::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialBackDash::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::InvincibleAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialAttack::GetInstance())
			)
		{
			m_dir = m_TargetDir;

			// アングル補間処理
			const float AnglePercentage = (m_pStateMachine->isInState(*BasePlayerState::Escape::GetInstance())) ? .9f : .65f;

			if (m_pStateMachine->isInState(*BasePlayerState::Wait::GetInstance()) || 
				m_pStateMachine->isInState(*BasePlayerState::Intro::GetInstance()))
			{
				m_fAngleY = DIR_ANGLE[(int)m_dir];
				m_fAngleY += (m_dir == DIR::LEFT) ? PI * -.1f : PI * .1f;
			}

			// 勝ってる時以外なら補正
			else if (!m_pStateMachine->isInState(*BasePlayerState::Win::GetInstance()))
			{
				m_fAngleY = m_fAngleY * AnglePercentage + DIR_ANGLE[(int)m_dir] * (1 - AnglePercentage);
			}
		}

		//if (bOutLog) tdnStopWatch::Start();

		// 入力受付後にステートマシン更新
		if (flag != PLAYER_UPDATE::NO_FSM) m_pStateMachine->Update();

		//if (bOutLog)
		//{
		//	tdnStopWatch::End();
		//	tdnDebug::OutPutLog("プレイヤー%s:\nステートマシン更新にかかった時間 %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
		//}

		// 動きの制御
		MoveUpdate();

		// メッシュの更新
		//if (bOutLog) tdnStopWatch::Start();
		if (!m_pStateMachine->isInState(*BasePlayerState::KnockBack::GetInstance()))m_pObj->Animation();
		else if (m_iRecoveryFrame < 30)m_pObj->Animation();
		//if (bOutLog)
		//{
		//	tdnStopWatch::End();
		//	tdnDebug::OutPutLog("プレイヤー%s:\nメッシュAnimation()にかかった時間 %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
		//}

		//if (bOutLog) tdnStopWatch::Start();
		m_pObj->SetAngle(m_fAngleY);	// 左右のアングルのセット
		m_pObj->SetPos(m_vPos);
		//if (bOutLog)
		//{
		//	tdnStopWatch::End();
		//	tdnDebug::OutPutLog("プレイヤー%s:\nメッシュSetPos()とかにかかった時間 %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
		//}

		//if (bOutLog) tdnStopWatch::Start();

		// モーション補間を掛けるのだが、攻撃モーションが若干遅れてしまうので、攻撃モーションのときは補間を掛けない
		const float slerp((isAttackState()) ? 0 : .5f);
		m_pObj->Update();
		//if (bOutLog)
		//{
		//	tdnStopWatch::End();
		//	tdnDebug::OutPutLog("プレイヤー%s:\nメッシュUpdate()にかかった時間 %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
		//}

	}

	//if (bOutLog) tdnStopWatch::Start();

	// エフェクトマネージャー更新 (ヒットストップ無視)
	m_pPanelEffectMGR->Update();
	m_pUVEffectMGR->Update();
	m_pThrowMark->Update();

	//if (bOutLog)
	//{
	//	tdnStopWatch::End();
	//	tdnDebug::OutPutLog("プレイヤー%s:\nエフェクト更新にかかった時間 %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
	//}
}

void BasePlayer::UpdateDrive()
{
	// 強制地面
	m_bLand = true;

	// スピードラインの更新
	m_pSpeedLine->Update();

	// 覚醒の更新
	OverDriveUpdate();

	// ガードエフェクト更新
	GuardEffectUpdate();

	// 1more覚醒していたらスタンドの動きを止める
	if (GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == false)
	{
		// スタンド更新
		m_pStand->Update(false);
	}

	// 無敵時間の更新
	if (KeyBoardTRG(KB_H))
	{
		SetInvincible(240, 1);
	}
	InvincibleUpdate();


	// ★硬直時間のデクリメント
	if (m_iRecoveryFrame > 0)
		m_iRecoveryFrame--;

	// ★ヒットストップ中
	if (m_iHitStopFrame > 0)
	{
		m_iHitStopFrame--;
	}
	// ヒットストップが言わるまで動かない
	else
	{
		// アクションフレームの更新
		if (isFrameAction())
		{
			// 攻撃フレームなら
			if (m_ActionDatas[(int)m_eActionState].isAttackData())
			{
				// 今の経過時間とディレイフレームになったら
				if (m_ActionDatas[(int)m_eActionState].pAttackData->WhiffDelayFrame == m_iCurrentActionFrame)
				{
					// (A列車)ここで攻撃判定が発動した瞬間を取ってきてる

					// 振りエフェクト発動（仮）！
					AddEffectAction(m_vPos + Vector3(0, 5, -3), m_ActionDatas[(int)m_eActionState].pAttackData->WhiffEffectType);

					LPCSTR SE_ID = m_ActionDatas[(int)m_eActionState].pAttackData->WhiffSE;
					// 空振りSE入ってたら
					if (SE_ID)
					{
						// ディレイフレーム経過したら再生
						se->Play((LPSTR)SE_ID);
					}
				}

			}

			// モーションのフレームを更新！
			m_iCurrentActionFrame++;

			// フレーム最後まで再生したら
			if (m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame] == FRAME_STATE::END)
			{
				// ★アクションステート解除
				m_eActionState = BASE_ACTION_STATE::NO_ACTION;
			}
		}

		// 入力受付後にステートマシン更新
		m_pStateMachine->Update();

		// 動きの制御
		//Move();
		m_vMove.Set(0, 0, 0);

		// メッシュの更新
		m_pObj->Animation();
		m_pObj->SetAngle(m_fAngleY);	// (TODO)今は固定
		m_pObj->SetPos(m_vPos);	// 原点固定
		m_pObj->Update();
	}

	// エフェクトマネージャー更新 (ヒットストップ無視)
	m_pPanelEffectMGR->Update();
	m_pUVEffectMGR->Update();

	// コンボUI
	//m_pComboUI->Update();

}

void BasePlayer::MoveUpdate() 
{
	if (m_bAerialDash)
	{
		m_vMove.y *= .8f;
		if (m_iAerialDashFrame-- > 0)
		{
			//// 移動
			//if (m_dir == DIR::RIGHT)
			//{
			//	// わっしょい
			//	m_vMove.Set(1.5f, 0, 0);
			//}
			//else
			//{
			//	// わっしょい
			//	m_vMove.Set(-1.5f, 0, 0);
			//}
		}
		else m_bAerialDash = false;

		//m_vMove *= 0.8f;	// 減速(A列車:この値はキャラ固有の値)

		return;
	}
	// 移動量更新
	if (
		//m_pStateMachine->isInState(*BasePlayerState::StandAction::GetInstance()) == false
		m_bMoveUpdate
		)
	{	// ペルソナ発動中じゃなかったら移動
		m_vMove.y -= c_GRAVITY;
		//if (m_vMove.y <= -3.0f) { m_vMove.y = -3.0f; } // 落ちる速度を抑制
		m_vMove.y = max(-2.75f, m_vMove.y);// 落ちる速度を抑制

		// 空気抵抗(xの値を徐々に減らしていく)
		if (m_bLand)
		{
			//if (m_vMove.x > 0)		m_vMove.x = max(m_vMove.x - .055f, 0);
			//else if (m_vMove.x < 0)	m_vMove.x = min(m_vMove.x + .055f, 0);
			m_vMove.x *= 0.92f;	// 減速(A列車:この値はキャラ固有の値)
		}
		else
		{
			//if (m_vMove.x > 0)		m_vMove.x = max(m_vMove.x - .035f, 0);
			//else if (m_vMove.x < 0)	m_vMove.x = min(m_vMove.x + .035f, 0);
			//m_vMove.x *= 0.98f;	// 減速(A列車:この値はキャラ固有の値)
		}

		// 左右のMove値
		//m_vMove.x = Math::Clamp(m_vMove.x, -m_maxSpeed, m_maxSpeed);
	}

	// ペルソナ発動中移動
	else
	{
		m_vMove *= 0.8f;	// 減速(A列車:この値はキャラ固有の値)
	}

}

// 無敵の制御
void BasePlayer::InvincibleUpdate()
{
	// ここだけのお名前
	enum 
	{
		FLASH_SPEED = 15
	};

	// 無敵時間の更新
	if (m_iInvincibleTime > 0)
	{
		//m_InvincibleLV = (--m_InvincibleTime <= 0) ? 0 : m_InvincibleLV;
		m_iInvincibleTime--;

		
		m_iInvincibleColRateFlame++;
		m_iInvincibleColRateFlame = m_iInvincibleColRateFlame % FLASH_SPEED;
		
		// 上げ下げのフラグ切り替え
		if (m_iInvincibleColRateFlame == 0)
		{
			m_bInvincibleColRateUpFlag = (m_bInvincibleColRateUpFlag == false) ? true : false;
		}

		// 上げフラグがONならだんだん白く
		if (m_bInvincibleColRateUpFlag == false)
		{
			m_fInvincibleColRate = ((float)(m_iInvincibleColRateFlame) / FLASH_SPEED);
		}
		else
		{
			m_fInvincibleColRate = 1.0f - ((float)(m_iInvincibleColRateFlame) / FLASH_SPEED);
		}
	

		// ★　色の調整 (ここはセンスの問題)
		m_fInvincibleColRate *= 0.25f;	// ちょっと薄く
		m_fInvincibleColRate += 0.1f;    // けど点滅がない状態でも白っぽい

	}
	else
	{
		// タイムが0以下に達したらおしまい　初期化
		m_iInvincibleLV = 0;
		m_iInvincibleTime = 0;

		m_fInvincibleColRate = 0.0f;
		m_iInvincibleColRateFlame = 0;
		m_bInvincibleColRateUpFlag = false;
		
	}

}

// 覚醒の制御
void BasePlayer::OverDriveUpdate()
{
	// 覚醒しているか
	if (m_bOverDrive == false)
	{
		//m_OverDriveGage++;
		// ゲージが最大まで溜まったら
		if (m_OverDriveGage >= c_OVERDRIVE_MAX_GAGE)
		{
			m_OverDriveGage = c_OVERDRIVE_MAX_GAGE;
		}

	}
	else // 覚醒中
	{
		// 覚醒の種類に応じて
		switch (m_OverDriveType)
		{
		case OVERDRIVE_TYPE::ONEMORE:

			// 青いオーラのパーティクル
			ParticleManager::EffectOverDrive(Vector3(GetPos().x, GetPos().y, -0.5f));	// 若干手前

			break;
		case OVERDRIVE_TYPE::BURST:
			// 黄色いオーラのパーティクル
			ParticleManager::EffectOverDriveBurst(Vector3(GetPos().x, GetPos().y, -0.5f));	// 若干手前

			break;
		default:
			break;
		}

		// ★★★覚醒中のときはゲージが溜まらないよ!!!!!!!
		m_OverDriveGage = 0;

		m_OverDriveFrame--;
		// 守りのバーストならゲージが早く減る
		if (m_OverDriveType== OVERDRIVE_TYPE::BURST)
		{
			m_OverDriveFrame -= 1;
		}

		if (m_OverDriveFrame <= 0) // 覚醒時間が0になったら覚醒終わり
		{
			m_OverDriveFrame = 0;
			m_bOverDrive = false;
		}
	}

}

void BasePlayer::ActionOverDrive(OVERDRIVE_TYPE type)
{
	// 覚醒の種類
	m_OverDriveType = type;

	// アクション
	m_bOverDrive = true;
	
	m_OverDriveGage = 0; //空に
	m_OverDriveFrame = c_OVERDRIVE_MAX_TIME;

	// ジャンプの権利も復活
	m_bAerialJump = true;
}

void BasePlayer::ColorUpdate()
{
	// オレンジの光の更新
	m_fOrangeColRate -= 0.03f;
	m_fOrangeColRate = max(0.0f, m_fOrangeColRate);

	// マゼンタの光の更新
	m_fMagentaColRate -= 0.05f;
	m_fMagentaColRate = max(0.0f, m_fMagentaColRate);

}

void BasePlayer::Control()
{
	// キーボード表
	static const KEYCODE KeyCodeList[(int)PLAYER_INPUT::MAX] = {
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_RIGHT,
		KEY_LEFT,
		KEY_UP,
		KEY_DOWN,
		KEY_R1,
		KEY_R2,
		KEY_R3,
		KEY_L1,
		KEY_L2,
		KEY_L3,
		KEY_START,
		KEY_SELECT,
	};

	// 押したキー判定
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		m_iInputList[i] = tdnInput::KeyGet(KeyCodeList[i], m_iDeviceID);

		// 押してたら！
		if (m_iInputList[i] & 0x01)
		{
			m_wCommandHistory[0] |= (int)c_BitList[i];
		}
	}

	// スティックの取得
	float x, y;
	tdnInput::GetAxisXYf(&x, &y, m_iDeviceID);
	
	static const float gosa(.2f);
	if (x > .5f - gosa)
	{
		m_wCommandHistory[0] |= (int)PLAYER_COMMAND_BIT::RIGHT;
	}
	else if (x < -.5f + gosa)
	{
		m_wCommandHistory[0] |= (int)PLAYER_COMMAND_BIT::LEFT;
	}
	if (y > .5f - gosa)
	{
		m_wCommandHistory[0] |= (int)PLAYER_COMMAND_BIT::DOWN;
	}
	else if (y < -.5f + gosa)
	{
		m_wCommandHistory[0] |= (int)PLAYER_COMMAND_BIT::UP;
	}
}

void BasePlayer::AIControl()
{
	//static int frame(0);
	//if (++frame > 90)
	//{
	//	frame = 0;
	//	m_InputList[(int)PLAYER_INPUT::C] = 3;
	//	//m_InputList[(int)PLAYER_INPUT::LEFT] = 3;
	//}

	////static int frame2(0);
	////if (++frame2 > 65)
	////{
	////	frame2 = 0;
	////	m_InputList[(int)PLAYER_INPUT::B] = 3;
	////	m_InputList[(int)PLAYER_INPUT::DOWN] = 3;
	////}
	//m_InputList[(int)PLAYER_INPUT::LEFT] = 1;	// Aおしっぱ


	// AIのステートマシンを更新
	m_pAI->Update();

}

void BasePlayer::UpdatePos()
{
	// (TODO)ヒットストップしてなかったら
	if (m_iHitStopFrame <= 0)
	{
		//if (m_pStateMachine->isInState(*BasePlayerState::StandAction::GetInstance()) == false)// ペルソナ発動中じゃなかったら移動
		{
			// 座標更新
			m_vPos += m_vMove;
		}
	}
}

// ★基本描画
void BasePlayer::Render()
{
	// スタンド描画
	m_pStand->Render(shaderM, "Persona");

	// シェーダへ送るデータ
	ShaderSendParam();


	// ∵だと30コスト
	m_pObj->Render(shaderM, "PlayerToon");
	
	// 無敵なら加算で重ねて描画
	//if (m_iInvincibleTime > 0) m_pObj->Render(RS::ADD);
	
	// ここで現在のステートマシンの状態を確認
	if (m_iDeviceID == 0)
	{
		//tdnText::Draw(30, 320, 0xffffffff, "フレーム:%d", m_CurrentActionFrame);
		//m_pStateMachine->Render();// ステートマシンでの描画
		//
		//if (m_pAI != nullptr)
		//{
		//	 m_pAI->GetFSM()->Render();// AIステートマシンでの描画
		//}
	}



	// エフェクトマネージャー描画
	m_pPanelEffectMGR->Render3D();
	m_pUVEffectMGR->Render();
	m_pPanelEffectMGR->Render();	
	Vector2 vScreenPos = Math::WorldToScreen(m_vPos);// (TODO)頭のポジションの座標を使う
	m_pThrowMark->Render((int)vScreenPos.x - 56, (int)vScreenPos.y - 324, RS::COPY_NOZ);

	

#ifdef _DEBUG
	// ここで現在のステートマシンの状態を確認
	if (m_iDeviceID == 0)
	{
		m_pStateMachine->Render();// ステートマシンでの描画

		if (m_pAI != nullptr)
		{
			m_pAI->GetFSM()->Render();// AIステートマシンでの描画
		}
	}

	// コマンド履歴の描画
	FOR(c_COMMAND_FRAME_MAX)
	{
		char *c	= "";


		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::LEFT)
		{
			c = "←";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::RIGHT)
		{
			c = "→";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::UP)
		{
			c = "↑";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::DOWN)
		{
			c = "↓";
		}
		//if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
		//	m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		//{
		//	c = "┏";
		//}
		//if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
		//	m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		//{
		//	c = "┓";
		//}
		if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
			m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "／";
		}
		if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
			m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "＼";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::A)
		{
			c = "×";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::B)
		{
			c = "○";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::C)
		{
			c = "□";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::D)
		{
			c = "△";
		}
		
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::L1)
		{
			c = "◇";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R1)
		{
			c = "▽";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::L2)
		{
			c = "㊧";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R2)
		{
			c = "㊨";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R3)
		{
			c = "◎";
		}

		//if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::A)
		//{
		//
		//}

		//switch (m_CommandHistory[i])
		//{
		//case PLAYER_INPUT::NEUTRAL:
		//	c = "・";
		//	break;
		//case PLAYER_INPUT::A:
		//	c = "A";
		//	break;
		//case PLAYER_INPUT::B:
		//	c = "B";
		//	break;
		//case PLAYER_INPUT::C:
		//	c = "C";
		//	break;
		//case PLAYER_INPUT::D:
		//	c = "D";
		//	break;
		//case PLAYER_INPUT::RIGHT:
		//	c = "→";
		//	break;
		//case PLAYER_INPUT::LEFT:
		//	c = "←";
		//	break;
		//case PLAYER_INPUT::UP:
		//	c = "↑";
		//	break;
		//case PLAYER_INPUT::DOWN:
		//	c = "↓";
		//	break;
		//case PLAYER_INPUT::R1:
		//	c = "R1";
		//	break;
		//case PLAYER_INPUT::R2:
		//	c = "R2";
		//	break;
		//case PLAYER_INPUT::R3:
		//	c = "R3";
		//	break;
		//case PLAYER_INPUT::L1:
		//	c = "L1";
		//	break;
		//case PLAYER_INPUT::L2:
		//	c = "L2";
		//	break;
		//case PLAYER_INPUT::L3:
		//	c = "L3";
		//	break;
		//case PLAYER_INPUT::START:
		//	c = "START";
		//	break;
		//case PLAYER_INPUT::SELECT:
		//	c = "SELECT";
		//	break;
		//case PLAYER_INPUT::MAX:
		//	break;
		//default:
		//	break;
		//}

		//tdnText::Draw(10 + (i % 30) * 30, 640 + (i / 30 * 30), 0xffffffff, "%s", c);
		tdnFont::RenderString(c, "メイリオ", 32, 10 + (i % 64) * 36, 610 + (i / 32 * 32) + ((int)m_side * 32), 0xffffffff, RS::COPY);

	}

	// 判定の描画
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), &m_tagCharacterParam.HitSquare, sizeof(CollisionShape::Square));
	square.pos += m_vPos;

	Vector3 wv[3];	// ワールドバーテックス
	wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
	wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
	wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

	Vector2 sv[3];	// スクリーンバーテックス
	FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

	tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ffffff);

	/* 攻撃判定の描画 */
	if (isAttackState())
	{
		if (isActiveFrame())
		{
			memcpy_s(&square, sizeof(CollisionShape::Square), GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			if (m_dir == DIR::LEFT) square.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			square.pos += m_vPos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}

	Vector2 pos2d = Math::WorldToScreen(m_vPos);
	if (m_bAI)
	{
		tdnText::Draw((int)pos2d.x - 20, (int)pos2d.y - 128, 0xffffffff, "硬直: %d", m_iRecoveryFrame);
	}

#endif

	// デバッグ
	//tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	//tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	//tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "スタンドゲージ : %d", m_pStand->GetStandGage());
	//tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "スタンドストック: %d", m_pStand->GetStandStock());
	
	//tdnText::Draw(32 + m_deviceID * 250, 430, 0xff00ff80, "OD ゲージ: %d", m_OverDriveGage);
	//tdnText::Draw(32 + m_deviceID * 250, 460, 0xffff8000, "OD残り時間: %d", m_OverDriveFrame);
}

void BasePlayer::Render(tdnShader* shader, char* name)
{
	// スタンド描画
	m_pStand->Render(shader, name);

	// 無敵の白色を送る
	//shader->SetValue("g_InvincibleColRate", m_InvincibleColRate);
	// デバッグ
	//tdnText::Draw(332 + m_deviceID * 350, 260, 0xff00ff80, "m_InvColRate : %.2f", m_InvincibleColRate);

	m_pObj->Render(shader, name);
	// 無敵なら加算で重ねて描画
	if (m_iInvincibleTime > 0) m_pObj->Render(RS::ADD);

	if (m_iDeviceID == 3)
	{
		m_pStateMachine->Render();// ステートマシンでの描画
		m_pAI->GetFSM()->Render();// AIステートマシンでの描画
	}
	

	// エフェクトマネージャー描画
	m_pPanelEffectMGR->Render3D();
	m_pUVEffectMGR->Render();

#ifdef _DEBUG
	// 判定の描画
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), &m_tagCharacterParam.HitSquare, sizeof(CollisionShape::Square));
	square.pos += m_vPos;

	Vector3 wv[3];	// ワールドバーテックス
	wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
	wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
	wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

	Vector2 sv[3];	// スクリーンバーテックス
	FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

	tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ffffff);

	/* 攻撃判定の描画 */
	if (isAttackState())
	{
		if (isActiveFrame())
		{
			memcpy_s(&square, sizeof(CollisionShape::Square), GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			if (m_dir == DIR::LEFT) square.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			square.pos += m_vPos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}

#endif

	// デバッグ
	//tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	//tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	//tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "スタンドゲージ : %d", m_pStand->GetStandGage());
	//tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "スタンドストック: %d", m_pStand->GetStandStock());

}

void BasePlayer::RenderShadow()
{
	m_pObj->Render(shaderM, "ShadowBuf");
}


void BasePlayer::RenderDrive()
{
	m_pObj->Render();

	// エフェクトマネージャー描画
	m_pPanelEffectMGR->Render3D();
	m_pUVEffectMGR->Render();

	// スピードラインの描画
	m_pSpeedLine->Render();
}

void BasePlayer::RenderDeferred()
{
	// スタンド描画
	m_pStand->Render(shaderM, "G_Buffer");

	m_pObj->Render(shaderM,"G_Buffer");
}

void BasePlayer::RenderUI()
{
	// コンボUI
	//m_pComboUI->Render(100 + (m_deviceID * 950), 200);

}

// シェーダに送るデータ
void BasePlayer::ShaderSendParam()
{
	// [1206] (最適化)　10->5
	// [1206] 最適化のためレートの送る回数を減らすためまとめて送ることに
	// 他の人が見て何やってるのかわからないと思うので処理が安定したらもどします。
	Vector3 l_vColDesc = VECTOR_ZERO;
	Vector3 l_vColDesc2 = VECTOR_ZERO;

	// 無敵の白色チカチカレートを送る
	//shaderM->SetValue("g_InvincibleColRate", m_fInvincibleColRate);
	l_vColDesc.x = m_fInvincibleColRate;

	// 無敵技のオレンジ色を送る
	//shaderM->SetValue("g_OrangeColRate", m_fOrangeColRate);
	l_vColDesc.y = m_fOrangeColRate;

	// 無敵技のマゼンタ色を送る
	//shaderM->SetValue("g_MagentaColRate", m_fMagentaColRate);
	l_vColDesc.z = m_fMagentaColRate;


	// 覚醒しているか// 覚醒時のカラー
	if (m_bOverDrive == true ||
		m_pStateMachine->isInState(*BasePlayerState::HeavehoDrive::GetInstance()) ||
		m_pStateMachine->isInState(*BasePlayerState::HeavehoDriveOverFlow::GetInstance())
		)
	{
		//shaderM->SetValue("g_OverDriveColRate", 1.0f);
		//shaderM->SetValue("g_WillPowerRate", 0.0f);
		l_vColDesc2.x = 1.0f;
		l_vColDesc2.y = 0.0f;
	}
	else
	{
		shaderM->SetValue("g_OverDriveColRate", 0.0f);
		l_vColDesc2.x = 0.0f;

		// 根性状態か
		if (m_bWillPower == true)
		{
			//shaderM->SetValue("g_WillPowerRate", 1.0f);
			l_vColDesc2.y = 1.0f;
		}
		else
		{
			//shaderM->SetValue("g_WillPowerRate", 0.0f);
			l_vColDesc2.y = 0.0f;
		}

	}

	// サイドに応じて縁取りの色を変える。
	//Vector3 vEdgeCol = VECTOR_ZERO;
	if (m_side == SIDE::LEFT)
	{
		//vEdgeCol = Vector3(0.95f, 0.1f, 0.0f);
		l_vColDesc2.z = 1.0f;
	}
	else
	{
		//vEdgeCol = Vector3(0.0f, 0.65f, 1.0f);
		l_vColDesc2.z = 0.0f;
	}
	//shaderM->SetValue("g_EdgeColor", vEdgeCol);

	// まとめてシェーダへ
	shaderM->SetValue("g_PlayerColDesc", l_vColDesc);
	shaderM->SetValue("g_PlayerColDesc2", l_vColDesc2);

}

void BasePlayer::RenderCommandFrame(int x, int y)
{
	// コマンド履歴の描画
	FOR(c_COMMAND_FRAME_MAX)
	{
		char *c = "";

		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::LEFT)
		{
			c = "←";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::RIGHT)
		{
			c = "→";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::UP)
		{
			c = "↑";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::DOWN)
		{
			c = "↓";
		}
		//if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
		//	m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		//{
		//	c = "┏";
		//}
		//if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
		//	m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		//{
		//	c = "┓";
		//}
		if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
			m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "／";
		}
		if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
			m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "＼";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::A)
		{
			c = "×";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::B)
		{
			c = "○";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::C)
		{
			c = "□";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::D)
		{
			c = "△";
		}

		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::L1)
		{
			c = "◇";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R1)
		{
			c = "▽";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::L2)
		{
			c = "㊧";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R2)
		{
			c = "㊨";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R3)
		{
			c = "◎";
		}

		tdnFont::RenderString(c, "メイリオ", 34, x + ((i % 64) * 44), y, 0xffffffff, RS::COPY);
	}

}

// ステートマシンへの他から来るメッセージ
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

// ★エフェクト発動★　（全てのエフェクトの発動場所）
void BasePlayer::AddEffectAction(Vector3 pos, EFFECT_TYPE effectType, Vector3 AttackVec)
{
	// 何のエフェクトを重ねるかはここで俺が決める。

	// 攻撃する向きから角度を変える
	float zAngle = atan2(-AttackVec.x, AttackVec.y);	

	// キャラクター向きによる向き
	float diaAngle = (m_dir == DIR::RIGHT) ? 0.0f : PI;

	// 斜めに描画して見やすくする用
	//float supportAngleY = (m_dir == DIR::RIGHT) ? -(PI * 0.0625f) : (PI * 0.0625f);
	float supportAngleY = (m_dir == DIR::RIGHT) ? -(PI * 0.08f) : (PI * 0.08f);

	// 集まるエフェクトの場所補正
	Vector3 convAddPos = Vector3(0, -10, 0);

	switch (effectType)
	{
	case EFFECT_TYPE::DAMAGE:
	{
		//m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURN);
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::HIT);
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::DAMAGE);

		// 相手のMove値考慮
		float z = atan2(-m_vMove.x, m_vMove.y);
		//m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::WAVE, 1, 1, Vector3(0, supportAngleY, z), Vector3(0, supportAngleY, z));

		// 貫通波
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::HIT_IMPACT,
			1, 2, Vector3(0, 0, z), Vector3(0, 0, z));

		// ヒットリング
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::HIT_RING, 2, 3, Vector3(0, supportAngleY, z), Vector3(0, supportAngleY, z));

		// 
		PointLightMgr->AddPointLight(pos + Vector3(0, 5, 0), Vector3(1.4f, 0.8f, 0.0f), 20, 4, 20, 4, 15);// ポイントライトエフェクト！
		Vector3 FlyVector(m_vMove);
		FlyVector.Normalize();
		ParticleManager::EffectHit(pos, FlyVector);

		
		// ブラ―エフェクト
		DeferredManagerEx.SetRadialBlur(m_vPos, 3.25f);

	}
		break;
	case EFFECT_TYPE::WHIFF:
	{
		Vector2 screenPos;
		screenPos = VECTOR2_ZERO;

		screenPos=Math::WorldToScreen(pos);

		//m_PanelEffectMGR->AddEffect((int)screenPos.x, (int)screenPos.y, PANEL_EFFECT_TYPE::WEAK);
	

		//nt delayTimer = 0;// ～秒遅延
		//_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::AIROU_CIRCLE,
		//   3.0f, 5.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, -3), delayTimer);

	}	break;
	case EFFECT_TYPE::RECOVERY:
		m_pPanelEffectMGR->AddEffect(pos + Vector3(0, 5, 0), PANEL_EFFECT_TYPE::ClEAR);


		break;
	case EFFECT_TYPE::PERSONA:
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PERSONA,
			1, 2.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectPersonaTrigger(pos);
		PointLightMgr->AddPointLight(pos + Vector3(0, 3, 0), Vector3(0, .4f, 2.0f), 50, 4, 40, 10, 30);// ポイントライトエフェクト！

		break;
	case EFFECT_TYPE::DROP_IMPACT:
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::IMPACT,
			1.0f, 1.35f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::SHOCK_WAVE,
			1.0f, 1.55f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		break;
	case EFFECT_TYPE::UPPER:
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::UPPER,
			1.0f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));

		break;

	case EFFECT_TYPE::FINISH_HIT:
		ParticleManager::EffectFinish(pos);

		break;
	case EFFECT_TYPE::BURST:
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURST);

		break;
	case EFFECT_TYPE::ONEMORE_BURST:
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::ONEMORE_BURST);

		break;
	case EFFECT_TYPE::ONEMORE_BURST_START:

		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURST_PREV);

		//m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::BURST_BALL, 0.9, 0.75
		//		, Vector3(0, 0, 0), Vector3(0, 0, 0));

		m_pUVEffectMGR->AddEffect(pos + convAddPos, UV_EFFECT_TYPE::CONV, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0));
		m_pUVEffectMGR->AddEffect(pos + convAddPos + Vector3(0, -5, 0), UV_EFFECT_TYPE::CONV2, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 4);
		m_pUVEffectMGR->AddEffect(pos + convAddPos + Vector3(-0.5, -7, 0), UV_EFFECT_TYPE::CONV3, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 8);
		m_pUVEffectMGR->AddEffect(pos + convAddPos, UV_EFFECT_TYPE::CONV4, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 12);
		break;
	case EFFECT_TYPE::RUN:
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_vPos, (m_dir != DIR::LEFT));

		break;
	case EFFECT_TYPE::BACK_STEP:
	{
		float l_fAngle = (m_dir == DIR::LEFT) ? 0.0f : PI;

		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, l_fAngle, 0), Vector3(0, l_fAngle, 0));
		ParticleManager::EffectRunSmoke(m_vPos, (m_dir == DIR::LEFT));

	}	break;
	case EFFECT_TYPE::GUARD_BREAK:
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::GLASS);

		break;
	case EFFECT_TYPE::AIROU_DRILL:
	{
									 int delayTimer = 15;// ～秒遅延
									 m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::AIROU_DRILL,
										 1.0f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0), delayTimer);

	}	break;
	case EFFECT_TYPE::AIROU_CIRCLE:
	{
									  int delayTimer = 0;// ～秒遅延
									  m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::AIROU_CIRCLE,
										  3.0f, 5.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, -3), delayTimer);

	}	break;
	case EFFECT_TYPE::GUARD_WAVE:
	{
		Vector3 l_Pos = pos;
		l_Pos = pos;
		if (AttackVec.y >= 0.1f)// ↑向きのベクトルなら
		{
			l_Pos.y += 5; //　ガードエフェクトを↑に上げる
		}
		// 相手と自分のベクトル値を考慮
		float z = atan2(-AttackVec.x, AttackVec.y);

		// [12/04] のけぞり後の離されないためにMove値考慮
		l_Pos.x += (m_vMove.x * 7.0f);

		// ガードウェ―ブ
		m_pUVEffectMGR->AddMultipleEffect(l_Pos, UV_EFFECT_MULTIPLE_TYPE::GUARD_WAVE, 0.5f, 0.5f, Vector3(0, -supportAngleY*1.5f, z), Vector3(0, -supportAngleY*1.5f, z));
		m_pUVEffectMGR->AddMultipleEffect(l_Pos, UV_EFFECT_MULTIPLE_TYPE::GUARD_GRID, 0.2f, 0.5f, Vector3(0, -supportAngleY*1.5f, z), Vector3(0, -supportAngleY*1.5f, z));

		// 青い光
		PointLightMgr->AddPointLight(l_Pos + Vector3((m_vMove.x * 2.0f) , 1.5f, -1), Vector3(0, 0.35f, 0.65f), 18, 12, 12, 3, 6);// ポイントライトエフェクト！

		// 飛び散るパーティクル
		ParticleManager::EffectGuard(l_Pos, AttackVec);

		// [1204] ガードエフェクトブラッシュアップ完成 

	}	break;
	case EFFECT_TYPE::MULTIPLE_HIT:
	{
		// ヒットリップ
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_pUVEffectMGR->AddMultipleEffect(pos, UV_EFFECT_MULTIPLE_TYPE::HIT_RIP, 1.0f, 2.0f, Vector3(ram, -supportAngleY*1.5f, 0), Vector3(ram, -supportAngleY*1.5f, 0));
		
		float slashRam = tdnRandom::Get(-3.14f, 3.14f);
		m_pUVEffectMGR->AddMultipleEffect(pos + Vector3(0, 0, -5) , UV_EFFECT_MULTIPLE_TYPE::HIT_SLASH, 2.0f, 4.0f, Vector3(0, 0, slashRam), Vector3(0, 0, slashRam));

		// ブラ―エフェクト
		DeferredManagerEx.SetRadialBlur(m_vPos, 2.55f);

	}	break;
	case EFFECT_TYPE::OVER_DRIVE_ACTION:
	{

		m_pPanelEffectMGR->AddEffect
			(pos, PANEL_EFFECT_TYPE::OVER_DRIVE_START);

		// OVER_DRIVE_RING
		m_pPanelEffectMGR->AddEffect
			(pos, PANEL_EFFECT_TYPE::OVER_DRIVE, 7);

		// OVER_DRIVE_RING
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::OVER_DRIVE_RING, 0.5f, 3.5f,
			Vector3(ram, -supportAngleY*1.5f, 0),
			Vector3(ram, -supportAngleY*1.5f, 0), 7);

	}	break;
	case EFFECT_TYPE::JUMP:
	{
		// ジャンプエフェクト
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.25f, 0.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));
		// ジャンプエフェクト線
		// 相手のMove値考慮
		float z = atan2(-m_vMove.x, m_vMove.y);
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_SPEED_LINE, 0.5f, 0.35f, Vector3(0, 0, z), Vector3(0, 0, z));


	}	break;
	case EFFECT_TYPE::AERIAL_JUMP:
	{

		float diaAngle = (m_vMove.x >= 0) ? PI / 2 : -PI / 2;

		// ジャンプエフェクト
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.25f, 0.5f, Vector3(0, 0, diaAngle), Vector3(0, 0, diaAngle));
		// ジャンプエフェクト線
		// 相手のMove値考慮
		//float z = atan2(-m_vMove.x, m_vMove.y);
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_SPEED_LINE, 0.5f, 0.35f, Vector3(0, 0, diaAngle), Vector3(0, 0, diaAngle));


	}	break;
	case EFFECT_TYPE::ESCAPE:
	{
		float ecpAngle = (m_dir == DIR::RIGHT) ? PI / 2 : -PI / 2;

		// ジャンプエフェクト
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.25f, 0.5f, Vector3(0, 0, ecpAngle), Vector3(0, 0, ecpAngle));
		// ジャンプエフェクト線
		// 相手のMove値考慮
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_SPEED_LINE, 0.5f, 0.35f, Vector3(0, 0, ecpAngle), Vector3(0, 0, ecpAngle));


		// 煙
		m_pUVEffectMGR->AddEffect(m_vPos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_vPos, (m_dir != DIR::LEFT));

	}	break;
	case EFFECT_TYPE::INVINCIBLE_ATTACK:
	{
		// プレッシャー
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PRESSURE, 0.5f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));
	
		// オレンジの光
		m_pPanelEffectMGR->AddEffect
			(pos + Vector3(0, 10, -4) , PANEL_EFFECT_TYPE::ORANGE_LIGHT, 0);

		// オレンジのリング
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_pUVEffectMGR->AddEffect(pos + Vector3(0, 10, -2), UV_EFFECT_TYPE::ORANGE_BURST, 0.1f, 1.75f,
			Vector3(ram, -supportAngleY*1.5f, 0),
			Vector3(ram, -supportAngleY*1.5f, 0), 0);

		// キャラクター自体をオレンジに
		m_fOrangeColRate = 1.0f;
	}	break;
	case EFFECT_TYPE::DOKKOI:
	{
		// プレッシャー
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PRESSURE, 0.5f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		// どっこいの光
		m_pPanelEffectMGR->AddEffect
			(pos + Vector3(0, 8, -2), PANEL_EFFECT_TYPE::DOKKOI, 0);

		// キャラクター自体をマゼンタに
		m_fMagentaColRate = 1.0f;

	}	break;
	case EFFECT_TYPE::MUZZLE_FLASH:
	{
		// マズルフラッシュ
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::MUZZLE_FLASH, 0.90f, 1.0f, Vector3(0, 0, zAngle), Vector3(0, 0, zAngle));

		// 飛び散る光
		//m_PanelEffectMGR->AddEffect
		//	(pos + Vector3(0, 8, -2), PANEL_EFFECT_TYPE::DOKKOI, 0);

	}	break;
	case EFFECT_TYPE::THROW:
	{
		// 投げの光
		m_pPanelEffectMGR->AddEffect
			(pos + Vector3(0, 8, -4), PANEL_EFFECT_TYPE::INEFFECT_MINI, 0);

	}	break;
	case EFFECT_TYPE::WILL_POWER:
	{
		// プレッシャー
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PRESSURE, 0.5f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		// 赤い光
		m_pPanelEffectMGR->AddEffect
			(pos + Vector3(0, 10, -4), PANEL_EFFECT_TYPE::RED_RIGHT, 0);

		// 赤のリング
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_pUVEffectMGR->AddEffect(pos + Vector3(0, 10, -2), UV_EFFECT_TYPE::RED_BURST, 0.1f, 1.85f,
			Vector3(ram, -supportAngleY*1.5f, 0),
			Vector3(ram, -supportAngleY*1.5f, 0), 0);

		// 赤い波紋
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RED_WAVE, 0.75f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}	break;
	case EFFECT_TYPE::DROP_IMPACT_SUPER:
	{
		// プレッシャー
		m_pUVEffectMGR->AddEffect(m_vPos, UV_EFFECT_TYPE::PRESSURE, 1.5f, 2.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		// 風
		m_pUVEffectMGR->AddEffect(m_vPos + Vector3(0, -5, 0) , UV_EFFECT_TYPE::CYCLONE_SHORT, 1.15f, 2.35f);

		// 波紋
		m_pUVEffectMGR->AddEffect(m_vPos, UV_EFFECT_TYPE::WAVE, 1.0f, 1.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}	break;
	case EFFECT_TYPE::CYCLONE_RUN:
	{
		// ダッシュエフェクト
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			1.3f, 1.5f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_vPos, (m_dir != DIR::LEFT));

		// ベクトル値を考慮
		Vector3 v = m_vMove;
		v.Normalize();
		float z = (m_vMove.x >= 0) ? PI / 2 : -PI / 2;

		// 風
		m_pUVEffectMGR->AddEffect(pos + Vector3(0, 5, 1), UV_EFFECT_TYPE::CYCLONE_SHORT, 1.0f, 1.2f, Vector3(0, 0, z), Vector3(0, 0, z));

		// 波紋
		//m_pUVEffectMGR->AddEffect(m_vPos, UV_EFFECT_TYPE::WAVE, 1.0f, 1.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));
		// ジャンプエフェクト
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.55f, 0.8f, Vector3(0, 0, z), Vector3(0, 0, z));

	}	break;
	default:
		MyAssert(0,"そんなエフェクトは存在しない ");	// そんなエフェクトは存在しない AddEffectAction()
		break;
	}

}

// ガードEffect
void BasePlayer::GuardEffectAction()
{
	// ガードエフェクト発動
	m_pUVEffectMGR->AddEffectRoop(GetCenterPos(), UV_EFFECT_TYPE::GUARD);

	m_pPanelEffectMGR->AddEffect(GetCenterPos()+Vector3(0,0,-5), PANEL_EFFECT_TYPE::GUARD);

}

void BasePlayer::GuardEffectStop()
{
	// ガードエフェクト終了
	m_pUVEffectMGR->StopEffectRoop(UV_EFFECT_TYPE::GUARD);
}

void BasePlayer::GuardEffectUpdate()
{
	// キャラクターに追従
	// ガードエフェクト更新
	m_pUVEffectMGR->GetBaseUVEffect(UV_EFFECT_TYPE::GUARD)->SetPos(GetCenterPos());
}

void BasePlayer::WillPowerUpdate(PLAYER_UPDATE updateFlag)
{	
	// 操作できる時ならば
	if (updateFlag != PLAYER_UPDATE::CONTROL_OK) return;	
	
	// HPのレートを計算
	float HpRate = (float)m_iHP/ m_tagCharacterParam.iMaxHP;

	// 体力が35%以下になれば根性発動
	if (m_bWillPower == false)
	{
		/*******************/
		//	 根性値発動前
		/*******************/

		// 死んでない状態でかつHPが35%以上なら
		if (HpRate <= 0.35f &&
			GetFSM()->isInState(*BasePlayerState::Die::GetInstance()) == false /*&& 
			GetFSM()->isInState(*BasePlayerState::Die::GetInstance()) == false*/)
		{
			// リカバリーフレームが切れ動けるようになった瞬間+
			// ダウンフラグFalseの時がトリガー
			if (m_iRecoveryFrame <= 0 && m_bDown == false)
			{
				// 根性発動
				m_bWillPower = true;

				// SPが25回復
				m_OverDriveGage += 25;

				// 演出
				AddEffectAction(m_vPos, EFFECT_TYPE::WILL_POWER);
				
				// SPゲージ上昇フレーム初期化
				m_iSpGettingFrame = 0;

				// オーラ発動
				m_pUVEffectMGR->AddEffectRoop(GetCenterPos(), UV_EFFECT_TYPE::WILL_POWER_AURA);

			}

		}
	}
	else
	{
		/*******************/
		//	 根性値発動中
		/*******************/

		// HPが増えると根性がなくなる　または　死んだら切れる
		if (HpRate > 0.35f || GetFSM()->isInState(*BasePlayerState::Die::GetInstance()) == true)
		{
			m_bWillPower = false;
		}
		// オーラ終了
		m_pUVEffectMGR->StopEffectRoop(UV_EFFECT_TYPE::WILL_POWER_AURA);
	
		m_iSpGettingFrame++;
		if (m_iSpGettingFrame >= 60)//1202
		{
			m_iSpGettingFrame = 0;
			m_OverDriveGage += 1;// SPゲージを足す
		}

	}

	// キャラクターに追従
	// オーラエフェクト更新
	m_pUVEffectMGR->GetBaseUVEffect(UV_EFFECT_TYPE::WILL_POWER_AURA)->SetPos(GetCenterPos());

	//throw gcnew System::NotImplementedException();

}
