#include "TDNLIB.h"

//+----------------------
//	�X�g�b�v�E�H�b�`
//+----------------------

// �O���錾
//double tdnStopWatch::m_dStart = 0;
//double tdnStopWatch::m_dEnd	 = 0;
LARGE_INTEGER tdnStopWatch::m_dStart;
LARGE_INTEGER tdnStopWatch::m_dEnd;

// �J�n����
void tdnStopWatch::Start()
{
	//m_dStart = clock();
	//m_dStart = std::chrono::system_clock::now();
	QueryPerformanceCounter(&m_dStart);
}

// �I������
void tdnStopWatch::End()
{
	//m_dEnd = clock();
	//m_dEnd = std::chrono::system_clock::now();
	QueryPerformanceCounter(&m_dEnd);
}

// ���ʂ�Ԃ�
double tdnStopWatch::GetResult()
{
	return (double)(m_dEnd.QuadPart - m_dStart.QuadPart);
}

void tdnStopWatch::DrawResult(int x, int y)
{

	// �`��
	//const double l_dResult = (m_dEnd - m_dStart);
	//auto l_dResult = (m_dEnd - m_dStart);
	//auto msec = std::chrono::duration<std::chrono::milliseconds>(l_dResult);
	double l_dResult = (double)(m_dEnd.QuadPart - m_dStart.QuadPart) /*/ freq.QuadPart*/;

	DWORD l_dCol = 0xff00ffff;
	//if (l_dResult > 9.0f)	l_dCol = 0xffff0000;
	//else	l_dCol = 0xffff00ff;	
	
	//tdnText::Draw(x, y, l_dCol, "�v������ %lf", (m_dEnd - m_dStart));
	tdnText::Draw(x, y, l_dCol, "�v������ %.1lf", l_dResult);
	//return m_dEnd - m_dStart;
}
