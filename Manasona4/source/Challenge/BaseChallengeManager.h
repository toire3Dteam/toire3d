#pragma once
#include "BaseChallenge.h"
#include "Data\SelectData.h"

//+------------------------------
// �x�[�X�`�������W�}�l�[�W���[
//+------------------------------

class BaseChallengeManager
{
public:
	
	BaseChallengeManager();
	virtual ~BaseChallengeManager();

	// ������
	void Init(int iSelectType,int iDeviceID);
	void Init(int iDeviceID);
	void Update();
	void TaskUpdate(BasePlayer* pPerson);
	void Render();

	// �A�N�Z�T�@
	BaseChallenge* GetChallenge() { return m_pChallenge[m_iSelectType]; };
	int GetArrayEnd() { return m_iArrayEnd; }

	// �I�����Ă���`���[�g���A����ݒ肷��
	int GetSelectType() { return m_iSelectType; }
	void SetSelectType(int iSelectType) { m_iSelectType = iSelectType; }

	void NectStep();
	void BackStep();

	// �^�C�g����
	std::string GetTitleString();
	std::string GetTitleString(int iNo);

protected:
	// ���ꂼ��̃`�������W�f�[�^������
	// (1/22) �����Ɋe�L�����̃`�������W�f�[�^�����Ă���
	// ���i�̃`�������W���[�h���S���������Ȃ͔̂z��̊֌W�����邩��
	BaseChallenge* m_pChallenge[(int)CHALLENGE_TYPE::ARRAY_END];
	
	int m_iSelectType;
	int m_iArrayEnd;// ���z��̏I��

};

//+-------------------------------
// �A�C���\�`�������W�}�l�[�W���[
//+-------------------------------

class AirouChallengeManager :public BaseChallengeManager
{
public:
	AirouChallengeManager();
	~AirouChallengeManager() {};

private:

};

//+-------------------------------
// �e�L�`�������W�}�l�[�W���[
//+-------------------------------

class TekiChallengeManager :public BaseChallengeManager
{
public:
	TekiChallengeManager();
	~TekiChallengeManager() {};

private:

};

//+-------------------------------
// (��)�`�������W�}�l�[�W���[
//+-------------------------------

class NazenarabaChallengeManager :public BaseChallengeManager
{
public:
	NazenarabaChallengeManager();
	~NazenarabaChallengeManager() {};

private:

};

//+-------------------------------
// �A���~�^�}�`�������W�}�l�[�W���[
//+-------------------------------

class AramitamaChallengeManager :public BaseChallengeManager
{
public:
	AramitamaChallengeManager();
	~AramitamaChallengeManager() {};

private:

};
