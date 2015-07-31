#ifndef COLLECTION_DATA_H_
#define COLLECTION_DATA_H_

//******************************************************************************
//
//
//		実績データ管理クラス(Singleton)
//
//
//******************************************************************************

class CollectionData
{
private:
	CollectionData(){}
	CollectionData(const CollectionData &l){}
	CollectionData &operator=(const CollectionData &l){}

public:

	//=========================================
	//	インスタンス取得
	static CollectionData  *GetInstance()
	{
		static CollectionData instance;
		return &instance;
	}
};





#endif