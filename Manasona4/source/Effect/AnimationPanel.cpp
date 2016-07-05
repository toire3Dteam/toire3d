#include "AnimationPanel.h"

/*�g��A�j��*/

AnimationPanel::AnimationPanel(char* _fileName, int _sizeX, int _sizeY, int _endCount, int _upflame, int _upCount, bool loop, int loopFlame )
{
	m_image = new tdn2DObj(_fileName);

	// �T�C�Y�w��
	m_sizeX = _sizeX;
	m_sizeY = _sizeY;

	// �I��̃J�E���g
	m_endCount = _endCount;

	// �X�s�[�h
	m_upflame = _upflame;

	// �オ��J�E���g
	m_upCount = _upCount;

	// �t���O
	isAction = false;
	m_loop = loop;
	m_loopMaxFlame = loopFlame;
	m_loopFlame = 0;
}

AnimationPanel::~AnimationPanel()
{
	delete m_image;
}

void AnimationPanel::Update()
{
	if (isAction == false)return;

	// �Ō�܂ŗ�����~�߂�
	if (isAction == true)
	{
		if (m_count >= m_endCount)
		{
			// ���[�v����
			if (m_loop)
			{
				// �ǉ��@���[�v�t���[���𒴂��Ă���I��
				if (m_loopFlame >= m_loopMaxFlame)
				{
					isAction = false;
				}
				m_count = 0;
				m_flame = 0;

			}
			else isAction = false;
		}
	}

	// �X�V
	m_flame++;
	m_loopFlame++;// �ǉ��@���[�v�̏I��
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

void AnimationPanel::Action()
{
	isAction = true;
	// ������
	m_count = 0;
	m_flame = 0;

}

bool AnimationPanel::GetisAction()
{
	return isAction;
}

void AnimationPanel::Stop()
{
	isAction = false;
	// ������
	m_count = 0;
	m_flame = 0;
}
