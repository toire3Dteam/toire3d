#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseMovie.h"

//+--------------------------
//	���[�r�[�̊Ǘ��N���X
//+--------------------------

// ���[�r�[�̎��
enum class MOVIE_TYPE
{
	OP,					//	�I�[�v�j���O
	OLD_OP,				//	�ߋ��I�[�v�j���O
	STAFF_ROLL,			//	�X�^�b�t���[��
	ARRAY_END
};

class MovieManager : public BaseGameEntity
{
public:

	MovieManager();
	~MovieManager();

	void InitExe(MOVIE_TYPE eSelectType);
	bool UpdateExe();
	void CtrlExe(int iDevice);
	void RenderExe();
	void RenderExeInfo(int iMovieType, int iX, int iY);
	void RereaseExe();
	//void RenderExeInfo(int iMovieType, int iX, int iY);

	void RenderIcon(int iMovieType, int iX, int iY);
	void RenderIconInfo(int iMovieType, int iX, int iY);

	// �S�f�[�^����Ȃ�					
	void AllReset();

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

	// �A�N�Z�T

	// �C���X�g�̃A�C�R���̒��g
	MOVIE_ICON_DATA GetMovieIconData(int iMovieType) { return m_pMovie[iMovieType]->GetMovieIcon(); }
	// �C���X�g�̒��g
	MOVIE_DATA GetMovieData(int iMovieType) { return m_pMovie[iMovieType]->GetMovie(); }

	BaseMovie* GetMovie(int iMovieType) { return m_pMovie[iMovieType]; };

	// �C���X�g�ő吔
	int GetMaxMovieNum() { return (int)MOVIE_TYPE::ARRAY_END; };
	// �C���X�g������
	int GetMovieOwned();

	MOVIE_TYPE GetSelectType() { return m_eSelectMovie; }
	void SetSelectType(MOVIE_TYPE eSelectType) { m_eSelectMovie = eSelectType; }

private:

	// �C���X�g�B
	BaseMovie* m_pMovie[(int)MOVIE_TYPE::ARRAY_END];

	// �I�����Ă���C���X�g
	MOVIE_TYPE m_eSelectMovie;

	// ���b�N�A�C�R��
	tdn2DObj* m_pRockIcon;

};

// ���̉�
//#define MovieMgr (MovieManager::GetInstance())