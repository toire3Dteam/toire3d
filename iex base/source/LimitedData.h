#ifndef LIMITED_H_
#define LIMITED_H_

//******************************************************************************
//
//
//		有限データ管理クラス(Singleton)
//
//
//******************************************************************************

class LimitedData
{
private:
	LimitedData(){}
	LimitedData(const LimitedData &l){}
	LimitedData &operator=(const LimitedData &l){}

public:

	//=========================================
	//	インスタンス取得
	static LimitedData  *GetInstance()
	{
		static LimitedData instance;
		return &instance;
	}
};





#endif