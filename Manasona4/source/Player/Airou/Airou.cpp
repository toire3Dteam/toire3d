#include "Airou.h"

Airou::Airou(int deviceID) :BasePlayer(deviceID)
{
	// キャラ固有の情報の設定
	m_Gravity = .14f;
	m_maxSpeed = 1.0f;

	// アイルーメッシュ
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou_motion.IEM");
}