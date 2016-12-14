#pragma once

#include "Airou.h"
#include "../../Effect/UVEffect/BaseUVEffect.h"
#include "../../Effect/LocusEffect.h"
#include "../../PointLight/PointLight.h"
#include "../../Effect/Particle.h"
#include "../../Sound/SoundManager.h"
#include "../../BaseEntity/Message/MessageDispatcher.h"


//*************************************************************
//	おしおきアイルー
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
		Vector3 m_vPos;			// アイルー座標
		Airou *m_pUser;	// このおしおきを使うプレイヤーのポインタ
		BasePlayer *m_pTarget;		// 攻撃対象のポインタ
		Vector3 m_vMoveVec;		// 移動の向き
		bool m_bErase;			// 消去フラグ
		float c_SPEED;	// 移動速度
		const float c_APP_RADIUS;	// くらってるやつに対する初期座標の半径
	};

	class OverDrive : public Base
	{
	private:
		const float c_APP_RADIUS = 30.0f;	// くらってるやつに対する初期座標の半径
		const float c_ALPHA_SPEED = .05f;	// アルファ値の加算速度
		float m_fAlpha;					// アルファ値
		int m_iStep;					// ステップ
		Vector3 m_vOrgPos;
		float m_speed;
		float m_accel;
		bool m_bHit;
		bool m_bLast;
	public:
		OverDrive(Airou *pUser, BasePlayer *pTarget, bool bLast) :Base(pUser, pTarget, 6, 30), m_fAlpha(0), m_iStep(0), m_speed(0), m_accel(.1f), m_bHit(false), m_bLast(bLast)
		{
			// 全方向ランダムベクトル作成
			//tdnRandom::GetRandomVector3(m_vMoveVec);
			const float ra(rand() % 618 * .01f);
			m_vMoveVec.x = sinf(ra);
			m_vMoveVec.y = cosf(ra);
			m_vMoveVec.z = 0;

			// プレイヤーの座標
			m_vPos = pTarget->GetCenterPos();

			// そのベクトル方向にrかける
			m_vPos += -m_vMoveVec * c_APP_RADIUS;
			m_vOrgPos = m_vPos;
		}
		void Update()
		{
			switch (m_iStep)
			{
			case 0:
				// 0からアルファ値加算
				if ((m_fAlpha += c_ALPHA_SPEED) > 1)
				{
					m_fAlpha = 1;
					m_iStep++;
				}
				break;
			case 1:
			{
					  // 移動
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
							  // プレイヤーにメッセージ送信
							  HIT_DAMAGE_INFO DamageInfo;
							  DamageInfo.fComboRate = 1;
							  DamageInfo.bOverDrive = true;

							  // 最後のアイルーのダメージ
							  if (m_bLast)
							  {
								  DamageInfo.BeInvincible = true;	// 無敵になるかどうか
								  DamageInfo.damage = 427;				// ダメージ(スコア)
								  DamageInfo.FlyVector.Set((m_pUser->GetTargetDir() == DIR::LEFT) ? -2.5f : 2.5f, 2.5f);			// 吹っ飛びベクトル
								  DamageInfo.iHitStopFrame = 8;		// ヒットストップ
								  DamageInfo.HitRecoveryFrame = 60;		// 硬直時間
								  DamageInfo.HitEffectType = (int)EFFECT_TYPE::DAMAGE;			// この攻撃のヒットエフェクトを相手に送る
								  DamageInfo.iAttackType = (int)BASE_ACTION_STATE::HEAVEHO_DRIVE;						// 何の攻撃かのタイプ(コンボ中に同じ攻撃を使わせないように)
								  DamageInfo.HitSE = "ぐちょヒット";
								  DamageInfo.DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;


								  // 最後の1撃だけフィニッシュ判定
								  DamageInfo.bFinishOK = true;

								  // 相手を解放してあげる
								  m_pTarget->SetMoveUpdate(true);
							  }

							  // それ以外のアイルーのダメージ
							  else
							  {
								  DamageInfo.BeInvincible = false;	// 無敵になるかどうか
								  DamageInfo.damage = 20;				// ダメージ(スコア)
								  DamageInfo.FlyVector.Set(0, 0);			// 吹っ飛びベクトル
								  DamageInfo.iHitStopFrame = 1;		// ヒットストップ
								  DamageInfo.HitRecoveryFrame = 45;		// 硬直時間
								  DamageInfo.HitEffectType = (int)EFFECT_TYPE::MULTIPLE_HIT;			// この攻撃のヒットエフェクトを相手に送る
								  DamageInfo.iAttackType = (int)BASE_ACTION_STATE::HEAVEHO_DRIVE;						// 何の攻撃かのタイプ(コンボ中に同じ攻撃を使わせないように)
								  DamageInfo.HitSE = "ぐちょヒット";
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
				// 消えていく
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
			// 向き&作成
			Math::SetTransMatrixFrontVec(&pObj->TransMatrix, m_vPos, m_vMoveVec);

			//MyAssert(pObj->GetPos().x != 0 && pObj->GetPos().y != 0 && pObj->GetPos().z != 0, "おしおきアイルーが原点にいる");

			// 描画
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
		const int m_ID;					// ランダムじゃないざひょうにするよう
		float m_fAngleX, m_fAngleY;		// 爆発して回転するから変数いる
		float m_fRandAddAngle;
		int m_iExplosionTimer;			// 爆発してから消えるまでの時間
		bool m_bTsukisasaru;				// 突き刺さるアイルーかどうか
		bool m_bTsukisasatta;			// 突き刺さった瞬間を取る用

		BaseUVEffect *m_pBoost;		// 隕石UVエフェクト
		BaseUVEffect *m_pImpact;	// 突き刺さった瞬間の碧エフェクト
		LocusEffect *m_pLocus;		// 隕石の軌跡エフェクト
		Vector3 m_vLocusPos, m_vLocusPos2;
	public:
		OverFlow(Airou *pUser, BasePlayer *pTarget, bool bTsukisasaru, int ID = 0) :Base(pUser, pTarget, 9, 60), m_bTsukisasaru(bTsukisasaru), m_iExplosionTimer(0), m_fAngleY(atan2f(c_VEC.x, c_VEC.z)), m_fAngleX(PI * .25f), m_fRandAddAngle((rand() % 618)*.01f), m_pBoost(new AirouBoostEffect), m_pLocus(new LocusEffect("DATA/UVeffect/Airou/Locus.png", 6, 2)), m_bTsukisasatta(false), m_pImpact(nullptr), m_ID(ID)
		{
			m_vPos = -(m_vMoveVec = c_VEC) * (bTsukisasaru ? 400.0f : 600.0f);

			// 突きささるアイルーならランダムじゃない
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

			// ランダム
			else
			{
				const float r(c_APP_RADIUS * (tdnRandom::Get(.0f, 0.75f) + 0.25f));
				m_vPos.x += sinf(m_fRandAddAngle) * r;
				m_vPos.y += cosf(m_fRandAddAngle) * r;

				// Y微調整
				m_vPos.y -= 10;
			}

			// ローカス
			m_vLocusPos = m_vPos + Vector3(0, 5, 0);
			m_vLocusPos2 = m_vPos + Vector3(0, 10, 0);
			m_pLocus->Action(&m_vLocusPos, &m_vLocusPos2);

			// エフェクトアクション
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
			// 爆発中
			if (m_iExplosionTimer > 0)
			{
				// 爆発終わったら消去フラグON
				if (--m_iExplosionTimer == 0) m_bErase = true;
				m_vPos.y -= .001f;	// 申し訳程度の重力
				m_vPos += m_vMoveVec * (c_SPEED * .1f);

				// アングルめちゃくちゃに足す
				m_fAngleX += sinf(m_fRandAddAngle);
				m_fAngleY += cosf(m_fRandAddAngle);
			}
			if (m_vPos.y > c_STOPLINE)
			{
				// 移動
				m_vPos += m_vMoveVec * c_SPEED;
				m_vLocusPos += m_vMoveVec * c_SPEED;
				m_vLocusPos2 += m_vMoveVec*c_SPEED;

				// 隕石更新
				m_pBoost->SetPos(m_vLocusPos);
				m_pBoost->Update();

				// ルーカス更新
				m_pLocus->Update();
			}
			else
			{
				if (!m_bTsukisasaru)m_bErase = true;	// 突き刺さるアイルーじゃなかったら消える
				else if (!m_bTsukisasatta)
				{
					// 突き刺さりエフェクト発動
					m_pImpact = new AirouImpactEffect;
					m_pImpact->Action(m_vPos, .5f, 1);
					m_pImpact->Update();

					// ポイントライト発動
					const Vector3 LightPos(m_vPos.x, 5, m_vPos.z - 5.0f);
					PointLightMgr->AddPointLight(LightPos, Vector3(.0f, 1.0f, .7f), 65, 2, 180, 2, 170);

					m_pBoost->Stop();
					m_pLocus->Stop();

					m_bTsukisasatta = true;

					// ダメージ
					// コンボ用
					COMBO_DESK comboDesk;
					comboDesk.side = m_pTarget->GetSide();
					comboDesk.damage = 427;
					comboDesk.recoveryFrame = 10000;
					MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

					// エフェクト
					se->Play("ぐちょヒット");
				}
				else
				{
					// アイルー突き刺さりエフェクト更新
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

			// エフェクト描画
			m_pLocus->Render();
			m_pBoost->Render();
			if (m_pImpact)m_pImpact->Render();
		}

		// 爆発
		void Explosion()
		{
			// ランダムベクトル作成
			//tdnRandom::GetRandomVector3(m_vMoveVec);
			//m_vMoveVec.y = fabsf(m_vMoveVec.y);	// 絶対値
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

			// ローカス止める
			m_pLocus->Stop();
		}
	};
}

//*************************************************************
//	おしおき管理用
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

		// おしおきを行うプレイヤー、されるプレイヤー
		Airou *m_pAirouPlayer;
		BasePlayer *m_pTarget;

		iex3DObj *m_pObj;

		// アクションフラグ
		bool m_bAction;

		// リストの中身を消去する処理
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

		// リストの中身を更新する処理
		void UpdateList(std::list<OshiokiAirou::OverDrive*> *pList)
		{
			for (auto it = pList->begin(); it != pList->end();)
			{
				// アイルー更新
				(*it)->Update();

				// 消去チェック
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
				// アイルー更新
				(*it)->Update();

				// 消去チェック
				if ((*it)->EraseOK())
				{
					delete (*it);
					it = pList->erase(it);
				}
				else it++;
			}
		}

		// リストの中身を描画する処理
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
		int m_iCreateTimer;					// アイルー生成用の時間
		int m_iCreateCount;					// アイルー何匹生成したか
		const int c_NUM_CREATE = 73;		// アイルーを何体作るか
		const Vector3 c_OSHIOKI_POS = Vector3(0, 30, 0);		// 相手をお仕置きする座標
		int m_CreateInterval;	// 0.5秒に1回

		std::list <OshiokiAirou::OverDrive*> m_list;
	public:
		OverDrive(Airou *pAirouPlayer, BasePlayer *pTarget) :Base(pAirouPlayer, pTarget), m_iCreateTimer(0), m_CreateInterval(30), m_iCreateCount(0)
		{
			// 相手の動きを封じる
			pTarget->SetMoveUpdate(false);
		}
		~OverDrive()
		{
			ClearList(&m_list);
		}
		void Update()
		{
			// アイループレイヤーの位置補正
			Vector3 AirouPlayerPos(m_pAirouPlayer->GetPos());
			const float ToTargetDistX(fabsf(AirouPlayerPos.x));
			if (ToTargetDistX < 10)
				AirouPlayerPos.x = (m_pAirouPlayer->GetTargetDir() == DIR::LEFT) ? 20.0f : -20.0f;			// 近いので距離を離す
			else if (ToTargetDistX > 30)
				AirouPlayerPos.x = (m_pAirouPlayer->GetTargetDir() == DIR::LEFT) ? 20.0f : -20.0f;			// 遠いので距離を縮める
			if (AirouPlayerPos.y < 30) AirouPlayerPos.y += .1f;
			m_pAirouPlayer->SetPos(AirouPlayerPos);

			// 相手の位置補正
			m_pTarget->SetPos(m_pTarget->GetPos() * .5f + c_OSHIOKI_POS * .5f);

			// オブジェアニメーション
			m_pObj->Animation();

			// アイルーリスト更新
			Base::UpdateList(&m_list);

			// 終わり生成回数まで達してなかったら
			if (m_iCreateCount < c_NUM_CREATE)
			{
				// 生成時間カウント、生成時間たったら生成
				if (++m_iCreateTimer % m_CreateInterval == 0)
				{
					// だんだん間隔短くしていく
					if (m_iCreateCount % 2 == 0) m_CreateInterval = max((int)(m_CreateInterval * .9f), 2);

					m_iCreateTimer = 0;
					m_list.push_back(new OshiokiAirou::OverDrive(m_pAirouPlayer, m_pTarget, (++m_iCreateCount == c_NUM_CREATE)));
				}
			}

			// 全員消えてたら終わり
			else if (m_list.empty())
			{
				m_bAction = false;
			}
		}
		void Render()
		{
			// アイルーリスト描画
			Base::RenderList(&m_list);
		}
	};

	class OverFlow :public Base
	{
	private:
		int m_iCreateTimer;					// アイルー生成用の時間
		int m_iCreateCount;					// アイルー何匹生成したか
		const int c_NUM_CREATE = 14;		// 突き刺さらないアイルーを何体作るか
		const int c_NUM_CREATE_TSUKISASARU = 8;// 突き刺さるアイルーを何体作るか
		const int c_CERATE_INTERVAL = 9;	// 0.5秒に1回
		bool m_bCreate;
		bool m_bFirstExplosion;
		bool m_bTsukisasiCreate;			// 突き刺さるアイルーを作るかどうかのフラグ
		std::list <OshiokiAirou::OverFlow*> m_list;

		BaseUVEffect *m_pExplosion;			// 爆発UVエフェクト
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

			// アイルーリスト更新
			Base::UpdateList(&m_list);

			// 爆発エフェクト更新
			m_pExplosion->Update();
			m_pFireRing->Update();
			m_pFireWave->Update();

			// 生成時間カウント、生成時間たったら生成
			if (m_bCreate)
			{
				// アニメーション
				if (!m_bTsukisasiCreate)m_pObj->Animation();

				if (++m_iCreateTimer > (m_bTsukisasiCreate ? 0 : c_CERATE_INTERVAL))
				{
					m_iCreateTimer = 0;	// タイマー初期化
					m_iCreateCount++;	// 作った数インクリメント&アイルーセット

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

			// アイルーリスト描画
			Base::RenderList(&m_list);

			// 爆発エフェクト描画
			m_pExplosion->Render();
			m_pFireWave->Render();
			m_pFireRing->Render();
		}
		void Explosion()
		{
			// 全員飛び散れー
			for (auto it : m_list) it->Explosion();

			if (m_bFirstExplosion)
			{
				// 爆発エフェクト発動
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
				// 爆発エフェクト発動
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