#include "ScoreUI.h"

//+--------------------------
//	スコア表示UI
//+--------------------------

ScoreUI::ScoreUI(ResultData data)
{

	// スコア初期化
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		// 場所
		m_tagScore[i].x = 50;
		m_tagScore[i].y = (i * 128) + 96;
		m_tagScore[i].iAddNumX = 448;

		// スコアを入れる
		switch ((SCORE_TYPE)i)
		{
		case ScoreUI::DAMAGE:
			m_tagScore[i].iPoint = data.iMaxDamage;
			break;
		case ScoreUI::HP:
			m_tagScore[i].iPoint = data.iRemainingHP;
			break;
		case ScoreUI::TIME:
			m_tagScore[i].iPoint = data.iElapsedTime;
			break;
		default:
			break;
		}


		m_tagScore[i].pFont = new tdn2DAnim("Data/Result/Font.png");
		m_tagScore[i].pFont->OrderMoveAppeared(14, 1280, m_tagScore[i].y);

		m_tagScore[i].pNumber = new Number(); // (TODO)後で引数でデザインの合う数字に
		m_tagScore[i].pNumber->GetAnim()->OrderMoveAppeared(14, 1280+ m_tagScore[i].iAddNumX, m_tagScore[i].y);

		switch ((SCORE_TYPE)i)
		{
		case ScoreUI::DAMAGE:
		
			break;
		case ScoreUI::HP:
		
			break;
		case ScoreUI::TIME:
		
			break;
		case ScoreUI::ARRAY_END:
			
			break;
		default:
			break;
		}

	}

	// ランク初期化
	m_tagRank.pRank = new tdn2DAnim("Data/Result/rank.png");
	m_tagRank.pRank->OrderShrink(8, 1.0f, 2.5f);
	m_tagRank.pRankRip = new tdn2DAnim("Data/Result/rank.png");
	m_tagRank.pRankRip->OrderRipple(12, 1.0f, 0.1f);
	m_tagRank.pLight = new ClearEffect();
	m_tagRank.pLight->GetPanel()->GetPic()->SetScale(3.0f);
	m_tagRank.x = 1280 - 306;
	m_tagRank.y = 720 - 320;

	// ランクのタイプ
	int score = CalcRankScore();
	if (score >= 14)
	{
		m_tagRank.iType = RANK_TYPE::SS;
	}
	else if (score >= 11)
	{
		m_tagRank.iType = RANK_TYPE::S;
	}
	else if (score >= 9)
	{
		m_tagRank.iType = RANK_TYPE::A;
	}
	else if (score >= 6)
	{
		m_tagRank.iType = RANK_TYPE::B;
	}
	else 
	{
		m_tagRank.iType = RANK_TYPE::C;
	}
	

	

	// 演出完了フラグ
	m_bEnd = false;
	//m_bActive = false;
	m_iFrame = 0;
}

ScoreUI::~ScoreUI()
{
	// スコア解放
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_tagScore[i].pFont);
		SAFE_DELETE(m_tagScore[i].pNumber);
	}

	// ランク解放
	SAFE_DELETE(m_tagRank.pRank);
	SAFE_DELETE(m_tagRank.pRankRip);
	SAFE_DELETE(m_tagRank.pLight);

}

void ScoreUI::Update()
{
	//if (m_bActive == false)return;

	enum { END_FRAME = 90 };
	m_iFrame++;
	// 終りのフレームまで来たら終了して　エンドフラグを立てる
	if (m_iFrame >= END_FRAME)
	{
		m_iFrame = 0;
		//m_bActive = false;
		m_bEnd = true;
		return;
	}

	// スコア更新
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		m_tagScore[i].pFont->Update();
		m_tagScore[i].pNumber->Update();
	}

	// ランク更新
	m_tagRank.pRank->Update();
	m_tagRank.pRankRip->Update();
	m_tagRank.pLight->Update();
}

void ScoreUI::Render()
{
	//if (m_bActive == false)return;
	
	// スコア描画
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		m_tagScore[i].pFont->Render(m_tagScore[i].x , m_tagScore[i].y, 512, 64, 0, 64 * (SCORE_TYPE)i, 512, 64);
		
		if (i== SCORE_TYPE::HP)
		{
			// %あり
			m_tagScore[i].pNumber->Render(m_tagScore[i].x + m_tagScore[i].iAddNumX, m_tagScore[i].y, m_tagScore[i].iPoint,Number::NUM_KIND::PARSENT);
		}
		else
		{
			m_tagScore[i].pNumber->Render(m_tagScore[i].x + m_tagScore[i].iAddNumX, m_tagScore[i].y, m_tagScore[i].iPoint);

		}
	}

	// ランク描画
	m_tagRank.pRank->Render(m_tagRank.x, m_tagRank.y, 256, 256, m_tagRank.iType * 256 , 0, 256, 256);
	m_tagRank.pRankRip->Render(m_tagRank.x, m_tagRank.y, 256, 256, m_tagRank.iType * 256, 0, 256, 256, RS::ADD);
	m_tagRank.pLight->Render();
}

void ScoreUI::Action()
{
	// スコア演出開始
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		m_tagScore[i].pFont->Action((i * 8));
		m_tagScore[i].pNumber->GetAnim()->Action((i * 8));
	}

	// ランク演出開始
	m_tagRank.pRank->Action(60);
	m_tagRank.pRankRip->Action(68);
	m_tagRank.pLight->Action(m_tagRank.x + 128, m_tagRank.y + 128 , 68);
	
	// 演出完了フラグを戻す
	m_bEnd = false;
	m_iFrame = 0;

	//m_bActive = true;
}

// ランク計算
int ScoreUI::CalcRankScore()
{
	int ScorePoint = 0;

	//+---------------------------------
	//	ダメージでのスコア
	//+---------------------------------
	if (m_tagScore[SCORE_TYPE::DAMAGE].iPoint >= 4000)
	{
		ScorePoint += 5;
	}
	else if (m_tagScore[SCORE_TYPE::DAMAGE].iPoint >= 3000)
	{
		ScorePoint += 4;
	}
	else if (m_tagScore[SCORE_TYPE::DAMAGE].iPoint >= 2500)
	{
		ScorePoint += 3;
	}
	else if (m_tagScore[SCORE_TYPE::DAMAGE].iPoint >= 1000)
	{
		ScorePoint += 2;
	}
	else
	{
		ScorePoint += 1;
	}


	//+---------------------------------
	//	 残りHPでポイント変換
	//+---------------------------------
	if (m_tagScore[SCORE_TYPE::HP].iPoint >= 100)
	{
		ScorePoint += 5;
	}
	else if (m_tagScore[SCORE_TYPE::HP].iPoint >= 70)
	{
		ScorePoint += 4;
	}
	else if (m_tagScore[SCORE_TYPE::HP].iPoint >= 50)
	{
		ScorePoint += 3;
	}
	else if (m_tagScore[SCORE_TYPE::HP].iPoint >= 25)
	{
		ScorePoint += 2;
	}
	else
	{
		ScorePoint += 1;
	}

	//+---------------------------------
	//	 経過時間でポイント変換
	//+---------------------------------
	if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 30)
	{
		// 経過時間が30秒内で倒せたら
		ScorePoint += 5;	
	}
	else if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 50)
	{
		// 経過時間が45秒内で倒せたら
		ScorePoint += 4;		
	}
	else if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 70)
	{
		// 経過時間が60秒内で倒せたら
		ScorePoint += 3;		
	}
	else  if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 85)
	{
		// 経過時間が75秒内で倒せたら
		ScorePoint += 2;		
	}
	else
	{
		ScorePoint += 1;
	}

	// [メモ]　14以上SS	11以上S 9以上A 6以上B 以下C

	return ScorePoint;
}
