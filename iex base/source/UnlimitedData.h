#ifndef UN_LIMITED_H_
#define UN_LIMITED_H_

//******************************************************************************
//
//
//		無制限データ管理クラス(Singleton)
//
//
//******************************************************************************

class UnlimitedData
{
private:
	UnlimitedData(){}
	UnlimitedData(const UnlimitedData &l){}
	UnlimitedData &operator=(const UnlimitedData &l){}

public:

	//=========================================
	//	インスタンス取得
	static UnlimitedData  *GetInstance()
	{
		static UnlimitedData instance;
		return &instance;
	}
};





#endif