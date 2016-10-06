#pragma once
#include "TDNLIB.h"
#include "../Entity/EntityEnum.h"

// メッセージタイプ
enum MESSAGE_TYPE
{
	EFFECT_CAMERA,		// エフェクトカメラ発動メッセージ
	SHAKE_CAMERA,		// カメラ振動メッセージ
	HIT_ATTACK,			// 攻撃をヒットさせた人に送るメッセージ！
	HIT_DAMAGE,			// 攻撃がヒットした相手に送るメッセージ
	FALL,				// プレイヤーが地面についていないときに送られるメッセージ
	THROW_SUCCESS,		// 掴んだ人に送るメッセージ
	BE_THROWN,			// 掴まれた相手に送るメッセージ(★掴んだ相手のIDを送ってあげる)
	CAN_THROW_RELEASE,	// 掴まれた人が投げぬけコマンドを入力したときに掴んだやつに対して送るメッセージ
	THROW_RELEASE,		// 投げぬけしていいよと送るメッセージ
	HEAVE_HO_OVER_DRIVE_HIT,	// ヒーホーフィニッシュヒット
	PERSONA_CARD_COUNT_UP,// ペルソナカードが増えた
	PERSONA_CARD_COUNT_DOWN,// ペルソナカードが減った
	OTHER				// その他。
};

// 攻撃喰らった相手用
struct HIT_DAMAGE_INFO
{
	bool BeInvincible;	// 無敵になるかどうか(たとえば、通常の1.2段目ならfalseだし、3段目ならtrue)
	int damage;			// 受けるダメ―ジ(スコア)
	Vector2 FlyVector;	// 吹っ飛びベクトル(強さもある)
	int hitStopFlame;		// ヒットストップフレーム
	int recoveryFlame;		// 硬直フレーム
	int HitEffectType;		// HITした相手に送るエフェクト (A列車)←をintじゃなくてEFFECT_TYPEにしたい。どうしたものか・・
	bool bFinishAttack;		// フィニッシュアタック
	int iAttackType;		// 何の攻撃を当てられたか(おんなじ攻撃だったらその分遅くしたいため)
};

// 攻撃ヒットしたプレイヤー用
struct HIT_ATTACK_INFO
{
	int HitPlayerDeviceID;	// ダメージを与えた相手の番号
	int hitStopFlame;		// ヒットストップフレーム
	int HitScore;			// 加算されるスコア
	bool bFinishAttack;		// フィニッシュアタック
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
	int ShakeFrame;			// 揺れてる時間(前の真夏だと3にしてた)
	void Set(float power, int frame){ ShakePower = power; ShakeFrame = frame; }
};

/* 掴まれた相手に送る情報構造体 */
struct BE_THROWN_INFO
{
	ENTITY_ID ThrowPlayerID;	// つかんだやつのID
	int iThrowPlayerDir;		// つかんだやつの向き(受けた側はこれの反対を向く)
};

// チーム
enum class TEAM
{
	A,
	B
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
	double       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(ENTITY_ID::ID_ERROR),
		Receiver(ENTITY_ID::ID_ERROR),
		Msg(MESSAGE_TYPE::OTHER)
	{}

	Message(double time,
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