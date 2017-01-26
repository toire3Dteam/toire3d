#include "TDNLIB.h"
#include "../Player/BasePlayer.h"
#include "CommandData.h"

void CommandSaver::Start(BasePlayer *pPlayer)
{
	// コマンド保存領域初期化
	m_vCommandBuffer.clear();

	m_pRefPlayer = pPlayer;
}

void CommandSaver::End()
{
	// 参照をオフにする
	m_pRefPlayer = nullptr;
}

void CommandSaver::Write(LPCSTR lpcFileName)
{
	// 何も入ってなかったら出ていけぇ！！
	if (m_vCommandBuffer.empty()) return;

	FILE *fp;
	MyAssert(fopen_s(&fp, lpcFileName, "wb") == 0, "デデドン(絶望)\nセーブデータ書き出しに失敗した！");	// まず止まることはないと思うが…

	// フレーム数書き出し
	const int l_ciNumCommand((int)m_vCommandBuffer.size());
	fwrite((LPSTR)&l_ciNumCommand, 1, sizeof(int), fp);

	// コマンドデータ書き出し
	for (int i = 0; i < l_ciNumCommand; i++)
	{
		fwrite((LPSTR)&m_vCommandBuffer[i], 1, sizeof(WORD), fp);
	}

	// ファイル閉じる
	fclose(fp);
}

void CommandSaver::Update()
{
	// このフレームのプレイヤーのコマンドを格納
	if (m_pRefPlayer)m_vCommandBuffer.push_back(m_pRefPlayer->GetCommandHistory());
}