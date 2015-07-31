#ifndef COLLECTION_DATA_H_
#define COLLECTION_DATA_H_

//******************************************************************************
//
//
//		���уf�[�^�Ǘ��N���X(Singleton)
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
	//	�C���X�^���X�擾
	static CollectionData  *GetInstance()
	{
		static CollectionData instance;
		return &instance;
	}
};





#endif