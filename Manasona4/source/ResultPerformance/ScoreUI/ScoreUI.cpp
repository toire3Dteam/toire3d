#include "ScoreUI.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"
#include "Effect\PanelEffect\PanelEffect.h"

//+--------------------------
//	�X�R�A�\��UI
//+--------------------------

ScoreUI::ScoreUI(ResultData data)
{

	// �X�R�A������
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{

		// �X�R�A������
		switch ((SCORE_TYPE)i)
		{
		case ScoreUI::DAMAGE:
			// �ꏊ
			m_tagScore[i].x = 150;
			m_tagScore[i].iAddNumX = 396;
			m_tagScore[i].iAddNumY = 100;
			m_tagScore[i].y = 24;

			m_tagScore[i].iPoint = data.iMaxDamage;
			m_tagScore[i].pFont = new tdn2DAnim("Data/Result/MaxDamage.png");
			//m_tagScore[i].pFont->OrderMoveAppeared(14, 1280, 24);
		
			break;
		case ScoreUI::HP:
			// �ꏊ
			m_tagScore[i].x = 25;
			m_tagScore[i].iAddNumX = 218;
			m_tagScore[i].iAddNumY = 100;
			m_tagScore[i].y = 230;

			m_tagScore[i].iPoint = data.iRemainingHP;
			m_tagScore[i].pFont = new tdn2DAnim("Data/Result/Life.png");
			//m_tagScore[i].pFont->OrderMoveAppeared(14, 1280, 230);
		
			break;
		case ScoreUI::TIME:
			// �ꏊ
			m_tagScore[i].x = 300;
			m_tagScore[i].iAddNumX = 256;
			m_tagScore[i].iAddNumY = 100;
			m_tagScore[i].y = 420;

			m_tagScore[i].iPoint = data.iElapsedTime;
			m_tagScore[i].pFont = new tdn2DAnim("Data/Result/Time.png");
			//m_tagScore[i].pFont->OrderMoveAppeared(14, 1280, 420);
		
			break;
		default:
			break;
		}
		m_tagScore[i].pFont->OrderShrink(12, 1, 2.5f);


		m_tagScore[i].pEffect = new CloudEffect();
		m_tagScore[i].pEffect->GetPanel()->GetPic()->SetScale(3.0f);

		m_tagScore[i].pNumber = new Number("Data/Number/Number5.png", 64, Number::NUM_KIND::RESULT); 
		//m_tagScore[i].pNumber->GetAnim()->OrderMoveAppeared(14, 1280+ m_tagScore[i].iAddNumX, m_tagScore[i].y);
		m_tagScore[i].pNumber->GetAnim()->OrderShrink(12, 1, 2.5f);
		
		m_tagScore[i].pFlashNumber = new Number("Data/Number/Number6.png", 64, Number::NUM_KIND::RESULT);
		m_tagScore[i].pFlashNumber->GetAnim()->OrderAlphaMove(90, 44, 45);


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

	// �����N������
	m_tagRank.pRank = new tdn2DAnim("Data/Result/rank.png");
	m_tagRank.pRank->OrderShrink(8, 1.0f, 2.5f);
	m_tagRank.pRankRip = new tdn2DAnim("Data/Result/rank.png");
	m_tagRank.pRankRip->OrderRipple(12, 1.0f, 0.1f);
	m_tagRank.pLight = new ClearEffect();
	m_tagRank.pLight->GetPanel()->GetPic()->SetScale(3.0f);
	m_tagRank.x = 1280 - 306;
	m_tagRank.y = 720 - 320;

	// �����N�̃^�C�v
	int score = CalcRankScore();
	if (score >= 14)
	{
		m_tagRank.eType = RANK_TYPE::SS;
	}
	else if (score >= 11)
	{
		m_tagRank.eType = RANK_TYPE::S;
	}
	else if (score >= 9)
	{
		m_tagRank.eType = RANK_TYPE::A;
	}
	else if (score >= 6)
	{
		m_tagRank.eType = RANK_TYPE::B;
	}
	else 
	{
		m_tagRank.eType = RANK_TYPE::C;
	}
	

	// �����̌v�Z
	PlayerDataMgr->m_PlayerInfo.coin += CalcCoin();

	// ���o�����t���O
	m_bEnd = false;
	//m_bActive = false;
	m_iFrame = 0;
}

ScoreUI::~ScoreUI()
{
	// �X�R�A���
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_tagScore[i].pFont);
		SAFE_DELETE(m_tagScore[i].pNumber);
		SAFE_DELETE(m_tagScore[i].pFlashNumber);
		SAFE_DELETE(m_tagScore[i].pEffect);
	}

	// �����N���
	SAFE_DELETE(m_tagRank.pRank);
	SAFE_DELETE(m_tagRank.pRankRip);
	SAFE_DELETE(m_tagRank.pLight);

}

void ScoreUI::Update()
{
	//if (m_bActive == false)return;

	enum { END_FRAME = 90 };
	m_iFrame++;
	// �I��̃t���[���܂ŗ�����I�����ā@�G���h�t���O�𗧂Ă�
	if (m_iFrame >= END_FRAME)
	{
		m_iFrame = 0;
		//m_bActive = false;
		m_bEnd = true;
		return;
	}

	// �X�R�A�X�V
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		m_tagScore[i].pFont->Update();
		m_tagScore[i].pNumber->Update();

		if (m_tagScore[i].pFlashNumber->GetAnim()->GetAction()->IsEnd() == true)
		{
			m_tagScore[i].pFlashNumber->GetAnim()->Action();
		}
		m_tagScore[i].pFlashNumber->Update();
		m_tagScore[i].pEffect->Update();
	}

	// �����N�X�V
	m_tagRank.pRank->Update();
	m_tagRank.pRankRip->Update();
	m_tagRank.pLight->Update();
}

void ScoreUI::Render()
{
	//if (m_bActive == false)return;
	
	// �X�R�A�`��
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		m_tagScore[i].pFont->Render(m_tagScore[i].x , m_tagScore[i].y);
		

		// �G�t�F�N�g
		m_tagScore[i].pEffect->Render();

		if (i== SCORE_TYPE::HP)
		{
			// %����
			m_tagScore[i].pNumber->Render(m_tagScore[i].x + m_tagScore[i].iAddNumX,
				m_tagScore[i].y + m_tagScore[i].iAddNumY, m_tagScore[i].iPoint,Number::NUM_KIND::PARSENT);

			m_tagScore[i].pFlashNumber->Render(m_tagScore[i].x + m_tagScore[i].iAddNumX,
				m_tagScore[i].y + m_tagScore[i].iAddNumY, m_tagScore[i].iPoint, Number::NUM_KIND::PARSENT, RS::ADD);

		}
		else if (i == SCORE_TYPE::TIME)
		{
			// �b����
			m_tagScore[i].pNumber->Render(m_tagScore[i].x + m_tagScore[i].iAddNumX,
				m_tagScore[i].y + m_tagScore[i].iAddNumY, m_tagScore[i].iPoint, Number::NUM_KIND::SECOND);

			m_tagScore[i].pFlashNumber->Render(m_tagScore[i].x + m_tagScore[i].iAddNumX,
				m_tagScore[i].y + m_tagScore[i].iAddNumY, m_tagScore[i].iPoint, Number::NUM_KIND::SECOND, RS::ADD);
		}
		else
		{
			m_tagScore[i].pNumber->Render(m_tagScore[i].x + m_tagScore[i].iAddNumX, 
				m_tagScore[i].y+ m_tagScore[i].iAddNumY, m_tagScore[i].iPoint, Number::NUM_KIND::RESULT);

			m_tagScore[i].pFlashNumber->Render(m_tagScore[i].x + m_tagScore[i].iAddNumX,
				m_tagScore[i].y + m_tagScore[i].iAddNumY, m_tagScore[i].iPoint, Number::NUM_KIND::RESULT, RS::ADD);
		}

		
	}

	// �����N�`��
	m_tagRank.pRank->Render(m_tagRank.x, m_tagRank.y, 256, 256, (int)m_tagRank.eType * 256 , 0, 256, 256);
	m_tagRank.pRankRip->Render(m_tagRank.x, m_tagRank.y, 256, 256, (int)m_tagRank.eType * 256, 0, 256, 256, RS::ADD);
	m_tagRank.pLight->Render();
}

void ScoreUI::Action()
{
	// �X�R�A���o�J�n
	for (int i = 0; i < SCORE_TYPE::ARRAY_END; i++)
	{
		m_tagScore[i].pFont->Action((i * 10));
		m_tagScore[i].pNumber->GetAnim()->Action((i * 10));
		m_tagScore[i].pEffect->Action(m_tagScore[i].x + 156, m_tagScore[i].y + 128, i * 10);

		// ����
		m_tagScore[i].pFlashNumber->GetAnim()->Action(40);
	}

	// �����N���o�J�n
	m_tagRank.pRank->Action(60);
	m_tagRank.pRankRip->Action(68);
	m_tagRank.pLight->Action(m_tagRank.x + 128, m_tagRank.y + 128 , 68);
	
	// ���o�����t���O��߂�
	m_bEnd = false;
	m_iFrame = 0;

	//m_bActive = true;
}

// �����N�v�Z
int ScoreUI::CalcRankScore()
{
	int ScorePoint = 0;

	//+---------------------------------
	//	�_���[�W�ł̃X�R�A
	//+---------------------------------
	if (m_tagScore[SCORE_TYPE::DAMAGE].iPoint >= 4000)
	{
		ScorePoint += 5+1;
	}
	else if (m_tagScore[SCORE_TYPE::DAMAGE].iPoint >= 3000)
	{
		ScorePoint += 4+1;
	}
	else if (m_tagScore[SCORE_TYPE::DAMAGE].iPoint >= 2500)
	{
		ScorePoint += 3+1;
	}
	else if (m_tagScore[SCORE_TYPE::DAMAGE].iPoint >= 1000)
	{
		ScorePoint += 2+1;
	}
	else
	{
		ScorePoint += 1;
	}


	//+---------------------------------
	//	 �c��HP�Ń|�C���g�ϊ�
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
	//	 �o�ߎ��ԂŃ|�C���g�ϊ�
	//+---------------------------------
	if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 30)
	{
		// �o�ߎ��Ԃ�30�b���œ|������
		ScorePoint += 5+1;	
	}
	else if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 50)
	{
		// �o�ߎ��Ԃ�45�b���œ|������
		ScorePoint += 4+1;		
	}
	else if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 70)
	{
		// �o�ߎ��Ԃ�60�b���œ|������
		ScorePoint += 3;		
	}
	else  if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 85)
	{
		// �o�ߎ��Ԃ�75�b���œ|������
		ScorePoint += 2;		
	}
	else
	{
		ScorePoint += 1;
	}

	// [����]�@14�ȏ�SS	11�ȏ�S 9�ȏ�A 6�ȏ�B �ȉ�C

	return ScorePoint;
}

// ����
int ScoreUI::CalcCoin()
{
	int addCoin = 0;

	addCoin += (m_tagScore[SCORE_TYPE::DAMAGE].iPoint / 10);
	addCoin += m_tagScore[SCORE_TYPE::HP].iPoint;
	//+---------------------------------
	//	 �o�ߎ��ԂŃ|�C���g�ϊ�
	//+---------------------------------
	if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 30)
	{
		// �o�ߎ��Ԃ�30�b���œ|������
		addCoin += 100;
	}
	else if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 50)
	{
		// �o�ߎ��Ԃ�45�b���œ|������
		addCoin += 50;
	}
	else if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 70)
	{
		// �o�ߎ��Ԃ�60�b���œ|������
		addCoin += 25;
	}
	else  if (m_tagScore[SCORE_TYPE::TIME].iPoint <= 85)
	{
		// �o�ߎ��Ԃ�75�b���œ|������
		addCoin += 5;
	}
	else
	{
		addCoin += 1;
	}

	return addCoin;
}
