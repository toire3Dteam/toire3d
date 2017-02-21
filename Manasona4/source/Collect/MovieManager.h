#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseMovie.h"

//+--------------------------
//	ムービーの管理クラス
//+--------------------------

// ムービーの種類
enum class MOVIE_TYPE
{
	OP,					//	オープニング
	OLD_OP,				//	過去オープニング
	STAFF_ROLL,			//	スタッフロール
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

	// 全データさよなら					
	void AllReset();

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

	// アクセサ

	// イラストのアイコンの中身
	MOVIE_ICON_DATA GetMovieIconData(int iMovieType) { return m_pMovie[iMovieType]->GetMovieIcon(); }
	// イラストの中身
	MOVIE_DATA GetMovieData(int iMovieType) { return m_pMovie[iMovieType]->GetMovie(); }

	BaseMovie* GetMovie(int iMovieType) { return m_pMovie[iMovieType]; };

	// イラスト最大数
	int GetMaxMovieNum() { return (int)MOVIE_TYPE::ARRAY_END; };
	// イラスト所持数
	int GetMovieOwned();

	MOVIE_TYPE GetSelectType() { return m_eSelectMovie; }
	void SetSelectType(MOVIE_TYPE eSelectType) { m_eSelectMovie = eSelectType; }

private:

	// イラスト達
	BaseMovie* m_pMovie[(int)MOVIE_TYPE::ARRAY_END];

	// 選択しているイラスト
	MOVIE_TYPE m_eSelectMovie;

	// ロックアイコン
	tdn2DObj* m_pRockIcon;

};

// 実体化
//#define MovieMgr (MovieManager::GetInstance())