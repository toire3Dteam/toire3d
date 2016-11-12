#pragma once

#include "../BaseEntity/Message/Message.h"

//*****************************************************************************************************************************
//
//		�J�����N���X
//
//*****************************************************************************************************************************

class TimeLineCamera
{
public:
	struct ShakeEvent
	{
		SHAKE_CAMERA_INFO ShakeInfo;
		int TriggerFrame;
		ShakeEvent &operator = (const ShakeEvent &event){ ShakeInfo = event.ShakeInfo; TriggerFrame = event.TriggerFrame; return *this; }
	}*pShakeEventArray;
	int NumShakeEvent;
	void PartitioningShakeEvent(int num)
	{
		NumShakeEvent = num;

		// �����̃o�b�t�@�̊J��
		if (pShakeEventArray) { delete[] pShakeEventArray; }

		if (num == 0)
		{
			pShakeEventArray = nullptr;
			return;
		}

		//�z��m��
		pShakeEventArray = new ShakeEvent[num];
	}

	int ID;			// �����̃p�^�[���ԍ�
	int NumPattern;	// �f�[�^�̎����Ă鐔
	class Pattern
	{
	public:
		int EndFrame;			// �^�C�����C����ł̏I���t���[��
		//Vector3 *pPosArray;		// ���W(�J�n�ƏI��肾���łȂ��A���ԂƂ������̂ŉϒ�)
		//Vector3 *pTargetArray;	// �����_(�������ϒ�)
		//float *pRollArray;		// Roll�l(������)
		ViewData *pViewDataArray;
		int NumPoints;		// ��L�ϒ��̗v�f��
		float FrameSpeed;		// �t���[�����Z���x
		bool fix;
		Pattern() :pViewDataArray(nullptr){}
		~Pattern(){ ClearArrays(); }
		void InstanceArrays(){ ClearArrays(); pViewDataArray = new ViewData[NumPoints]; }
	private:
		void ClearArrays(){ if (pViewDataArray){ delete[] pViewDataArray; } }
	}*pPatterns;

	TimeLineCamera() :pPatterns(nullptr), NumPattern(0), NumShakeEvent(0), pShakeEventArray(nullptr){}
	~TimeLineCamera()
	{
		if (pPatterns){delete[] pPatterns;} 
		if(pShakeEventArray)
		{
			delete[] pShakeEventArray;
		}
	}
};

class Camera;

class EffectCamera
{
private:
	//------------------------------------------------------
	//	�X�N���v�g�֌W
	//------------------------------------------------------
	int m_CurrentScriptID;	// ���Đ����Ă���J�����X�N���v�gID(m_list�̓Y�����̖���)
	int m_PatternCursor;		// �p�^�[���̃J�[�\��(PatternCursor >= NumPattern)��������I��
	int m_EventFrame;	// �U���Ƃ��p�̃C�x���g����
	int m_EventCursor;	// �C�x���g�̃J�[�\��
	float m_CurrentFrame;	// ���݂̃t���[����
	bool m_bAction;			// �A�N�V�����t���O
	Vector3 m_vOrgPos;
	std::vector<TimeLineCamera*> m_list;

	//------------------------------------------------------
	//	���䂷��J��������
	//------------------------------------------------------
	//Camera *m_pCamera;
	ViewData *m_pViewData;

	void GetTimeLineViewData(ViewData *out);

public:

	//------------------------------------------------------
	//	�������E���
	//------------------------------------------------------
	EffectCamera(ViewData *pViewData, LPCSTR filename, const Vector3 &OrgPos=VECTOR_ZERO);
	~EffectCamera();

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	void Update();

	//------------------------------------------------------
	//	�s���Z�b�g
	//------------------------------------------------------
	void Start(int id);

	bool isAction(){ return m_bAction; }
	int GetEventTime(){ return m_EventFrame; }
};