#pragma once
#include "TDNLIB.h"
#include "../system/System.h"

/*
*UVアニメーションクラス
*/

/* 使い方 */
/* ・UVアニメーションをさせたいメッシュを読み込む				*/
/* ・更新にUpdate関数・描画にRender関数をそれぞれ置く			*/
/* ・最後にAction関数を実行するとUVアニメーションが始まります。	*/

class AnimationUV
{
public:
	// 過去の仕様
	// メッシュの名前　1フレームに動くX＆Y　終了フレーム　最後に透明になるかのフラグ　透明になっていく始まりのフレーム　ループをするか
	//AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool AlphaFlag = false, int AlphaNear = 0, bool IsRoop = false);
	
	// 現在の仕様
	AnimationUV(char* name, float moveTU, float moveTV, int EndFlame,bool IsRoop = false);
	AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool IsRoop, int AlphaNear, int AlphaFar);
	~AnimationUV();

	void Action();		// アニメ実行
	void Stop();		// アニメ停止

	void ActionRoop();  // ループアニメ実行
	void StopRoop();	// ループアニメストップ

	void Update(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f));	// 更新
	void Update(Vector3 pos ,Vector3 angle , Vector3 scale);			// 更新スケールをvector型に変えたもの
	void Render();			// 描画
	void Render_ADD();			// 乗算描画
	void Render_ADD_NOZ();			// 乗算描画Z値無効
	void Render_Guard();			// ガード用描画
	void Render_Soul();				// 魂用描画
	void Render_AreaWall();			// エリアの壁用

	// スケール・アニメーション　最初から　最後まで　の大きさを書く　変化させたくなければ　どっちも同じ値を入れる
	void ScaleAnimation(float StartScale, float EndScale);

    // アングル・アニメーション　最初から　最後まで　の値を書く　変化させたくなければ　どっちも同じ値を入れる
    void AngleAnimation(Vector3 StartAngle, Vector3 EndAngle);


	// ゲッター・セッター
	iexMesh* GetObj(){ return obj; }

	bool IsAction() { return isAction; }
	bool IsEnd() { return isEnd; }
private:
	iexMesh*	obj;
	float		tu, tv;			// 毎フレームの横縦のUVの座標
	float		moveTu, moveTv;	// 毎フレームの横縦のUVの動き
	int			endFlame;		// 描画終りのフレーム
	int			nowFlame;		// 今のフレーム

	bool		isAction;		// アニメを実行しているか
	bool		isRoop;			// ループエフェクトか

	bool		alphaFlag;		// 最後透明にするフラグ
	int			alphaNear;		// 透明じゃなくなる　始めるフレーム
	int			alphaFar;		// 透明じゃなくなる　終わるフレーム　
	float		alpha;			// 透明度

	bool		isEnd;		// おわり

	float		m_scale;		// 拡大
	float		m_startScale;	// 初期 拡大率
	float		m_endScale;		// 最大 拡大率

    Vector3		m_angle;		        // アングル
    Vector3		m_startAngle;   	// 初期 アングル
    Vector3		m_endAngle;		// 最大 アングル
};
