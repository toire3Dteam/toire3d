#pragma once

//*****************************************************************************************************************************
//
//		�J�����N���X
//
//*****************************************************************************************************************************

class TimeLineCamera
{
public:
	int ID;			// �����̃p�^�[���ԍ�
	int NumPattern;	// �f�[�^�̎����Ă鐔
	class Pattern
	{
	public:
		int EndFrame;			// �^�C�����C����ł̏I���t���[��
		Vector3 *pPosArray;		// ���W(�J�n�ƏI��肾���łȂ��A���ԂƂ������̂ŉϒ�)
		Vector3 *pTargetArray;	// �����_(�������ϒ�)
		float *pRollArray;		// Roll�l(������)
		int NumPoints;		// ��L�ϒ��̗v�f��
		float FrameSpeed;		// �t���[�����Z���x
		bool fix;
		Pattern() :pPosArray(nullptr), pTargetArray(nullptr), pRollArray(nullptr){}
		~Pattern(){ ClearArrays(); }
		void InstanceArrays(){ ClearArrays(); pPosArray = new Vector3[NumPoints], pTargetArray = new Vector3[NumPoints]; pRollArray = new float[NumPoints]; }
	private:
		void ClearArrays(){if (pPosArray){ delete[] pPosArray; }if (pTargetArray){ delete[] pTargetArray; } if (pRollArray){ delete[] pRollArray; }}
	}*pPatterns;

	TimeLineCamera() :pPatterns(nullptr), NumPattern(0){}
	~TimeLineCamera(){ if (pPatterns)delete[] pPatterns; }
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
	float m_CurrentFrame;	// ���݂̃t���[����
	std::vector<TimeLineCamera*> m_list;

	//------------------------------------------------------
	//	���䂷��J��������
	//------------------------------------------------------
	Camera *m_pCamera;

	void GetTimeLineCameraPos(Vector3 *OutPos, Vector3 *OutTarget, float *OutRoll);

public:

	//------------------------------------------------------
	//	�������E���
	//------------------------------------------------------
	EffectCamera(Camera *me);
	~EffectCamera();

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	void Update();

	//------------------------------------------------------
	//	�s���Z�b�g
	//------------------------------------------------------
	void Start(int id);
};