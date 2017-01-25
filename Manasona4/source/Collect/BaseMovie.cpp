#include "BaseMovie.h"



//+------------------------------
//	��{�̃R���N�V�����p�̃C���X�g
//+------------------------------

// �A�C�R���p�̃f�[�^�̏�����
MOVIE_ICON_DATA::MOVIE_ICON_DATA()
{
	pIcon = nullptr;
	sTitle = "�Ȃ�";
	sText = "�Ȃ�";
	vPos = VECTOR2_ZERO;
	iAlpha = 1.0f;
}

// �摜�p�̃f�[�^�̏�����
MOVIE_DATA::MOVIE_DATA()
{
	pMovie = nullptr;
	pPic = nullptr;
	sTitle = "�Ȃ�";
	sText = "�Ȃ�";

}

//+-------------------------
//	�������E���
//+-------------------------
BaseMovie::BaseMovie()
{

	m_bRock = false;
	m_iPrice = 9999;
}

BaseMovie::~BaseMovie()
{
	SAFE_DELETE(m_tagMovieIcon.pIcon);
}

//+-------------------------
//	�X�V�E�`��	(�A�C�R��)
//+-------------------------
//void BaseMovie::UpdateIcon()
//{
//
//
//}

void BaseMovie::RenderIcon(int iX, int iY)
{
	// �A�C�R��
	m_tagMovieIcon.pIcon->Render(
		(int)m_tagMovieIcon.vPos.x + iX,
		(int)m_tagMovieIcon.vPos.y + iY);

}

//	���w���ŃA�C�R��
void BaseMovie::RenderIconNonPurchase(int iX, int iY)
{
	// �Â�
	m_tagMovieIcon.pIcon->SetARGB(0xff777777);
	
	// �A�C�R��
	m_tagMovieIcon.pIcon->Render(
		(int)m_tagMovieIcon.vPos.x + iX,
		(int)m_tagMovieIcon.vPos.y + iY);

	// ���ɖ߂�
	m_tagMovieIcon.pIcon->SetARGB(0xffffffff);

}

void BaseMovie::RenderIconInfo(int iX, int iY)
{
	// �^�C�g��
	tdnFont::RenderString(m_tagMovieIcon.sTitle.c_str(), "HG�޼��E",
		22, iX, iY, 0xffffffff, RS::COPY);
	// ����
	tdnFont::RenderString(m_tagMovieIcon.sText.c_str(), "HG�޼��E",
		19, iX, iY + 30, 0xffffffff, RS::COPY);

}

// �������z�֐���
//void BaseMovie::InitExe()
//{
//
//}

// �摜���
void BaseMovie::RereaseExe()
{
	SAFE_DELETE(m_tagMovie.pPic);
	SAFE_DELETE(m_tagMovie.pMovie);
}

//+-------------------------
//	�X�V�E�`��	(�N�������Ƃ�)
//+-------------------------
bool BaseMovie::UpdateExe()
{
	// ���[�r�[�X�V(�e�A�����O)
	m_tagMovie.pMovie->Update();

	// ���[�r�[���Ō�܂ŗ�����
	if (m_tagMovie.pMovie->isEndPos())
	{
		// ���[�r�[�~�߂�
		m_tagMovie.pMovie->Stop();
		
		// ���j���[��
		return true;
	}

	return false;
}

void BaseMovie::CtrlExe(int iDevice)
{


}

void BaseMovie::RenderExe()
{
	// �ӏܗp
	m_tagMovie.pPic->Render(0,0);
}

void BaseMovie::RenderExeInfo(int iX, int iY)
{
	// �^�C�g��
	tdnFont::RenderString(m_tagMovie.sTitle.c_str(), "HG�޼��E",
		22, iX, iY, 0xffffffff, RS::COPY);
	// ����
	tdnFont::RenderString(m_tagMovie.sText.c_str(), "HG�޼��E",
		18, iX+8, iY + 32, 0xffffffff, RS::COPY);
}

//+---------------------------------
//	����̂n�o
//+---------------------------------
OpMovie::OpMovie()
{
	m_tagMovieIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Op.png");
	m_tagMovieIcon.sTitle = "�y���[�r�[�z�I�[�v�j���O";
	m_tagMovieIcon.sText = "���[�r�[�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void OpMovie::InitExe()
{
	// ����֘A������
	m_tagMovie.pMovie = new tdnMovie("DATA/Title/op.wmv", true, false);
	m_tagMovie.pPic = new tdn2DObj(m_tagMovie.pMovie->GetTexture());
	m_tagMovie.sTitle = "�y���[�r�[�z�I�[�v�j���O";
	m_tagMovie.sText = "�W���W���̊�Ȗ`���̂n�o�p���B\n�F�B�̋��͂̂������łł����Ӑg�̈�i�B";

	// ���[�r�[�Đ�
	m_tagMovie.pMovie->Play();
}

//+-------------------------------------
// �̂̂n�o
OldOpMovie::OldOpMovie()
{
	m_tagMovieIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/OldOp.png");
	m_tagMovieIcon.sTitle = "�y���[�r�[�z�O��̃I�[�v�j���O";
	m_tagMovieIcon.sText = "���[�r�[�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void OldOpMovie::InitExe()
{
	m_tagMovie.pMovie = new tdnMovie("DATA/Movie/oldop.wmv", true, false);
	m_tagMovie.pPic = new tdn2DObj(m_tagMovie.pMovie->GetTexture());
	m_tagMovie.sTitle = "�y���[�r�[�z�O��̃I�[�v�j���O";
	m_tagMovie.sText = "���V�L�����ɍ�����n�o�A�ċx�ݒ��C���ō�����Ȃ����̈�i�B";


	// ���[�r�[�Đ�
	m_tagMovie.pMovie->Play();
}