#ifndef LIMITED_H_
#define LIMITED_H_

//******************************************************************************
//
//
//		�L���f�[�^�Ǘ��N���X(Singleton)
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
	//	�C���X�^���X�擾
	static LimitedData  *GetInstance()
	{
		static LimitedData instance;
		return &instance;
	}
};





#endif