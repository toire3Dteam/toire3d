#pragma once
#include "TDNLIB.h"
#include "../Entity/EntityEnum.h"

// メッセージタイプ
enum MESSAGE_TYPE
{
	EFFECT_CAMERA,		// エフェクトカメラ発動メッセージ
	WINNER_CAMERA,
	SHAKE_CAMERA,		// カメラ振動メッセージ
	HIT_ATTACK,			// 攻撃をヒットさせた人に送るメッセージ！
	HIT_DAMAGE,			// 攻撃がヒットした相手に送るメッセージ
	FALL,				// プレイヤーが地面についていないときに送られるメッセージ
	//THROW_SUCCESS,		// 掴んだ人に送るメッセージ
	BE_THROWN,			// 掴まれた相手に送るメッセージ(★掴んだ相手のIDを送ってあげる)
	CAN_THROW_RELEASE,	// 掴まれた人が投げぬけコマンドを入力したときに掴んだやつに対して送るメッセージ
	THROW_RELEASE,		// 投げぬけしていいよと送るメッセージ
	HEAVE_HO_OVER_DRIVE_HIT,	// ヒーホーフィニッシュヒット
	PERSONA_CARD_COUNT_UP,// ペルソナカードが増えた
	PERSONA_CARD_COUNT_DOWN,// ペルソナカードが減った
	APP_WIN_ICON,// 勝利数アイコン点灯
	CUTIN_ACTION,// カットイン発動！
	HEAVE_HO_OVERFLOW_START,	// ヒーホーオーバーフロー開始！
	END_ROUNDCALL,
	END_FINISHCALL,
	END_WINNER,			// 勝者の演出終了
	OVER_DRIVE_STAGE,	// オーバードライブのステージ
	KO,					// HPがゼロになったときに送信
	OVER_DRIVE_CUTIN,   // 必殺技のカットイン
	KAKEAI_END,			// 掛け合い終了
	COMBO_COUNT,		// コンボカウント
	COMBO_GUARD,		// コンボガードされたぜ
	ADD_SHOT,			// ショットの実体を格納してね
	ERASE_SIDE_SHOT,	// 片方の出したショットを消す
	BACK_MENU,			// メニュー画面にもどりたいです。
	TROPHY_GET,			// トロフィーゲット
	PICTURE_GET,		// イラストゲット
	MOVIE_GET,			// ムービーゲット
	OTHER				// その他。

};


// キャラクターサイド
enum class SIDE
{
	LEFT,
	RIGHT,
	ARRAY_MAX
};

// コンボ用
struct COMBO_DESK
{
	SIDE side;
	int damage;
	int recoveryFrame;
	bool bCounter;
	COMBO_DESK() :side(SIDE::LEFT), damage(0), recoveryFrame(0), bCounter(false) {}
};

// KOのタイプ
enum class FINISH_TYPE
{
	NORMAL,
	OVER_DRIVE
};

// KOしたときに送るメッセージの構造体
struct KO_INFO
{
	FINISH_TYPE FinishType;		// KOのタイプ
	int iNumWinnerRound;		// KOしたやつの勝った数(ラウンド設定した分を取ったらリザルトに行く)
	SIDE WinnerSide;			// KOしたやつのサイド
	SIDE LoseSide;				// KOされたやつのサイド
};

enum class DAMAGE_MOTION
{
	KNOCK_BACK,	// のけぞり
	KNOCK_DOWN,	// 吹っ飛び
	DOWN_FALL,	// 足払いくらい
};

// 攻撃喰らった相手用
struct HIT_DAMAGE_INFO
{
	bool BeInvincible;	// 無敵になるかどうか(たとえば、通常の1.2段目ならfalseだし、3段目ならtrue)
	int damage;			// 受けるダメ―ジ(スコア)
	Vector2 FlyVector;	// 吹っ飛びベクトル(強さもある)
	int iHitStopFrame;		// ヒットストップフレーム
	int HitRecoveryFrame;	// ヒット硬直フレーム
	int GuardRecoveryFrame;	// ガード硬直フレーム
	int HitEffectType;		// HITした相手に送るエフェクト (A列車)←をintじゃなくてEFFECT_TYPEにしたい。どうしたものか・・
	bool bOverDrive;		// フィニッシュアタック
	int iAttackType;		// 何の攻撃を当てられたか(おんなじ攻撃だったらその分遅くしたいため)
	int iAntiGuard;			// ガード突き破る攻撃の種類
	LPCSTR HitSE;			// ヒットするSEのID
	bool bFinishOK;			// フィニッシュする攻撃か
	float fGuardKnockBackPower;	// ガードのけぞりの力
	DAMAGE_MOTION DamageMotion;	// 喰らったモーションタイプ
	float fComboRate;		// コンボ補正
	int iAttribute;			// 属性
	float fRepeatAttackRate;// 同技補正値(技ごとに持つことにした)
	bool bStandAttack;		// スタンド攻撃かどうか
	HIT_DAMAGE_INFO() { ZeroMemory(this, sizeof(this)); bStandAttack = false; }
};

// 攻撃ヒットしたプレイヤー用
struct HIT_ATTACK_INFO
{
	int iHitStopFrame;		// ヒットストップフレーム
	int HitScore;			// 加算されるスコア
	bool bOverDrive;		// フィニッシュアタック
	bool bFirstAttack;		// (12/27)最初の一撃かどうか
};

// プレイヤーInfo
struct PLAYER_INFO
{
	Vector3 pos;
	ENTITY_ID id;
};

/* エフェクトカメラ発動時に送る時の構造体 */
struct EFFECT_CAMERA_INFO
{
	int scriptID;			// カメラスクリプトのID
	Vector3 *pOrgPos;		// エフェクトカメラの原点の座標(nullptrならVector3(0,0,0))
	EFFECT_CAMERA_INFO() :scriptID(0), pOrgPos(nullptr){}
};

/* カメラ振動時に送る時の構造体 */
struct SHAKE_CAMERA_INFO
{
	float ShakePower;	// 振動の力(0～1)
	int ShakeFrame;			// 揺れてる時間(前の闘魂だと3にしてた)
	void Set(float power, int frame){ ShakePower = power; ShakeFrame = frame; }
};


/*
	メッセージはシンプルな列挙型。
	内容はなんでもあり。
	
	・誰が
	・誰に
	・メッセージの列挙
	・メッセージの追加情報(Void*)
	・ディレイ用
*/

/*電報構造体*/
struct Message
{
	// このTelegramを送ったエンティティ
	ENTITY_ID          Sender;

	// このTelegramを受け取るエンティティ
	ENTITY_ID          Receiver;

	// メッセージの内容。　（列挙型に設定されているもの）
	MESSAGE_TYPE          Msg;

	// メッセージの追加情報 (キャラクターが動く→ [追加情報] Xに２移動 )
	// 追加情報を事前に知ることが不可能なのでVoid*で提供
	void*        ExtraInfo;

	// メッセージの遅延時間
	int       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(ENTITY_ID::ID_ERROR),
		Receiver(ENTITY_ID::ID_ERROR),
		Msg(MESSAGE_TYPE::OTHER)
	{}

	Message(int time,
		ENTITY_ID    sender,
		ENTITY_ID    receiver,
		MESSAGE_TYPE    msg,
		void*  info = NULL) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}

};