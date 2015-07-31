#ifndef UN_LIMITED_H_
#define UN_LIMITED_H_

//******************************************************************************
//
//
//		�������f�[�^�Ǘ��N���X(Singleton)
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
	//	�C���X�^���X�擾
	static UnlimitedData  *GetInstance()
	{
		static UnlimitedData instance;
		return &instance;
	}
};





#endif