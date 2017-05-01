#include "AnimationPanel.h"

/*�g��A�j��*/

AnimationPanel::AnimationPanel(char* _fileName, int _sizeX, int _sizeY, 
	int _endCount, int _upflame, int _upCount, bool loop, int loopCount)
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
	m_bEndFlag = false; // ()�����ł͏I����Ă��Ȃ����Ƃ�

	m_loop = loop;
	m_loopMaxCount= loopCount; // ���񃋁[�v�����邩
	m_loopCount= 0;
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
				// �ǉ��@���[�v�J�E���g�����[�v�ő�J�E���g�𒴂��Ă���I��
				if (m_loopCount >= m_loopMaxCount)
				{
					isAction = false;
					m_bEndFlag = true;
					return;
				}
				else
				{
					m_loopCount++; // ���[�v�J�E���g�X�V
				}
				// �J�E���g�ƃt���[����0�ɂ��Ė������[�v��
				m_count = 0;
				m_flame = 0;

			}
			else //���[�v����Ȃ�������I�� 
			{
				isAction = false;
				m_bEndFlag = true;
				return;
			}
				
		}
	}

	// �X�V
	m_flame++;
	//m_loopFlame++;// �ǉ��@���[�v�̏I��
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

	// ������
	m_count = 0;
	m_flame = 0;
	m_loopCount = 0;// [4/29 ���[�v����񐔂�������]
}

bool AnimationPanel::GetisAction()
{
	return isAction;
}

void AnimationPanel::Stop()
{
	isAction = false;
	m_bEndFlag = true;

	// ������
	m_count = 0;
	m_flame = 0;
}
