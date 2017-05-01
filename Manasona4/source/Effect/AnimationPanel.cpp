#include "AnimationPanel.h"

/*波紋アニメ*/

AnimationPanel::AnimationPanel(char* _fileName, int _sizeX, int _sizeY, 
	int _endCount, int _upflame, int _upCount, bool loop, int loopCount)
{
	m_image = new tdn2DObj(_fileName);

	// サイズ指定
	m_sizeX = _sizeX;
	m_sizeY = _sizeY;

	// 終りのカウント
	m_endCount = _endCount;

	// スピード
	m_upflame = _upflame;

	// 上がるカウント
	m_upCount = _upCount;

	// フラグ
	isAction = false;
	m_bEndFlag = false; // ()ここでは終わっていないことに

	m_loop = loop;
	m_loopMaxCount= loopCount; // 何回ループさせるか
	m_loopCount= 0;
}

AnimationPanel::~AnimationPanel()
{
	delete m_image;
}

void AnimationPanel::Update()
{
	if (isAction == false)return;

	// 最後まで来たら止める
	if (isAction == true)
	{
		if (m_count >= m_endCount)
		{
			// ループ分岐
			if (m_loop)
			{
				// 追加　ループカウントがループ最大カウントを超えてたら終り
				if (m_loopCount >= m_loopMaxCount)
				{
					isAction = false;
					m_bEndFlag = true;
					return;
				}
				else
				{
					m_loopCount++; // ループカウント更新
				}
				// カウントとフレームを0にして無限ループへ
				m_count = 0;
				m_flame = 0;

			}
			else //ループじゃなかったら終了 
			{
				isAction = false;
				m_bEndFlag = true;
				return;
			}
				
		}
	}

	// 更新
	m_flame++;
	//m_loopFlame++;// 追加　ループの終り
	if (m_flame >= m_upflame)
	{
		m_flame = 0;
		m_count++;
	}

}
void AnimationPanel::Render(int x, int y, DWORD dwFlag)
{
	if (isAction == false)return;

	int tx, ty;
	tx = ty = 0;
	tx = m_count*m_sizeX;

	int kuriage = (int)(m_count / m_upCount);
	ty = m_sizeY*kuriage;


	m_image->Render(x, y, m_sizeX, m_sizeY, tx, ty, m_sizeX, m_sizeY, dwFlag);
}

void AnimationPanel::Render3D(Vector3 pos, DWORD dwFlag)
{
	if (isAction == false)return;

	int tx, ty;
	tx = ty = 0;
	tx = m_count*m_sizeX;

	int kuriage = (int)(m_count / m_upCount);
	ty = m_sizeY*kuriage;


	m_image->Render3D(pos, m_sizeX, m_sizeY, tx, ty, m_sizeX, m_sizeY, dwFlag);


}

void AnimationPanel::Action()
{
	isAction = true;
	m_bEndFlag = false;

	// 初期化
	m_count = 0;
	m_flame = 0;
	m_loopCount = 0;// [4/29 ループする回数も初期化]
}

bool AnimationPanel::GetisAction()
{
	return isAction;
}

void AnimationPanel::Stop()
{
	isAction = false;
	m_bEndFlag = true;

	// 初期化
	m_count = 0;
	m_flame = 0;
}
