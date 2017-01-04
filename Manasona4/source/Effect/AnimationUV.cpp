
#include	"AnimationUV.h"
#include "system\System.h"

/*
*
*	UVアニメーションクラス
*
*/

// 過去の仕様
//AnimationUV::AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool AlphaFlag, int AlphaNear, bool IsRoop)
//{
//	obj = new iexMesh(name);
//
//	tu = 0.0f; tv = 0.0f;
//	moveTu = moveTU, moveTv = moveTV;
//	endFlame = EndFlame;
//	nowFlame = 0;			// 初期フレームを0に
//	isAction = false;		// アニメ実行するか
//
//	// α
//	alphaFlag = AlphaFlag;
//	alphaNear = AlphaNear;
//	alpha = 1.0f;
//
//	isRoop = IsRoop;
//}

// 簡易ver
AnimationUV::AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool IsRoop )
{

	obj = new iexMesh(name);

	tu = 0.0f; tv = 0.0f;
	moveTu = moveTU, moveTv = moveTV;
	endFlame = EndFlame;
	nowFlame = 0;			// 初期フレームを0に
	isAction = false;		// アニメ実行するか

	// α
	alphaFlag = false;
	alphaNear = 0;
	alphaFar = EndFlame; // 適当
	alpha = 1.0f;

	isRoop = IsRoop;

	isEnd = false;

	// オブジェクトステータス
	m_scale = 1.0f;
	m_startScale = 1.0f;
	m_endScale = 1.0f;

    m_angle = VECTOR_ZERO;
    m_startAngle = VECTOR_ZERO;
    m_endAngle = VECTOR_ZERO;
}

// α考慮
AnimationUV::AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool IsRoop, int AlphaNear, int AlphaFar)
{
	obj = new iexMesh(name);

	tu = 0.0f; tv = 0.0f;
	moveTu = moveTU, moveTv = moveTV;
	endFlame = EndFlame;
	nowFlame = 0;			// 初期フレームを0に
	isAction = false;		// アニメ実行するか

	// α
	alphaFlag = true;
	alphaNear = AlphaNear;
	alphaFar = AlphaFar;
	alpha = 1.0f;

	isRoop = IsRoop;

	isEnd = false;

	// オブジェクトステータス
	m_scale = 1.0f;
	m_startScale  = 1.0f;
	m_endScale   =	1.0f;

	// [10/07日]アングル初期化
	m_angle = VECTOR_ZERO;
	m_startAngle = VECTOR_ZERO;
	m_endAngle = VECTOR_ZERO;
}


AnimationUV::~AnimationUV()
{
	delete obj;
}


void  AnimationUV::Action()
{
	isAction = true;//起動
	tu = 0.0f; tv = 0.0f;// 初期のuvに戻す
	nowFlame = 0;// 最初のフレームへ

	isEnd = false;
}

void AnimationUV::Stop()
{
	isAction = false;//止める
	nowFlame = 0;
};

void AnimationUV::ActionRoop()	// ループアニメ実行
{
	isAction = true;		// 起動
	tu = 0.0f; tv = 0.0f;	// 初期のuvに戻す
	nowFlame = 0;			// 最初のフレームへ
	isRoop = true;			// ループ
}

// ループストップ
void AnimationUV::StopRoop()	// ループアニメストップ
{
	isRoop = false;				// ループストップ
	nowFlame = alphaFar + 1;	// 透明になる部分へワープ
}

void AnimationUV::Update(Vector3 pos)
{
	if (isAction == false)return;//実行されてないなら出てけ！！

	// UV更新
	tu += moveTu;
	tv += moveTv;

	//if (isRoop == false)//ループじゃなかったら
	{
		// フレーム更新
		nowFlame++;
		if (isRoop == true)// ループだったらalphaFarの前のとこでループし続ける処理
		{
			if (nowFlame >= (alphaFar - 1))
			{
				nowFlame = (alphaFar-1);
			}
		}

		if (nowFlame >= endFlame)
		{
			isAction = false;

			// 追加
			isEnd = true;
		}
	}

	// 透明度更新
	if (alphaFlag == true)
	{
		//ラムダ式Min~Maxの範囲に抑える　２～０
		auto Clamp = [](float val, float Min, float Max){
			return min(Max, max(val, Min));
		};

		// 0が透明　1が不透明にするように設定
		//float A = (endFlame - nowFlame);
		//float B =(endFlame - alphaNear);
		//alpha = A / B;
		//alpha = Clamp(alpha, 0.0f, 1.0f);

		// α二アーの前か後ろで判定を変える
		if (nowFlame >= alphaNear)
		{		
		// 100-100=0  100-50=50   0/50
		float A = (float)(endFlame - nowFlame);
		float B = (float)(endFlame - alphaFar);
		alpha = A / B;
		//alpha = Clamp(alpha, 0.0f, 1.0f);

		}
		else
		{
		// 最初の
		alpha = (float)nowFlame / (float)alphaNear;//   0/30=0   60/30=2   1-(0~1)  

		}

		//alpha = (alphaFar - nowFlame) / (alphaFar - alphaNear);
		alpha = Clamp(alpha, 0.0f, 1.0f);//指定された値を 0 ～ 1 の範囲にクランプします

		
	}

	// スケール更新
	float scareRate = (float)(nowFlame) / (float)(endFlame); // 最初にレートを出す
	float calcScare = (m_startScale * (1.0f - scareRate)) + (m_endScale * scareRate);

    // 回転更新
    float angleRate = (float)(nowFlame) / (float)(endFlame); // 最初にレートを出す
    Vector3 calcAngle = (m_startAngle * (1.0f - angleRate)) + (m_endAngle * angleRate);


	// メッシュの更新
	obj->SetScale(calcScare);
    obj->SetAngle(calcAngle);
	obj->SetPos(pos);
	obj->Update();


}

void AnimationUV::Update(Vector3 pos, Vector3 angle, Vector3 scale)
{
	if (isAction == false)return;//実行されてないなら出てけ！！

	// UV更新
	tu += moveTu;
	tv += moveTv;

	if (isRoop == false)//ループじゃなかったら
	{
		// フレーム更新
		nowFlame++;
		if (nowFlame >= endFlame)
		{
			isAction = false;
		}
	}

	// 透明度更新
	if (alphaFlag == true)
	{
		//ラムダ式Min~Maxの範囲に抑える　２～０
		auto Clamp = [](float val, float Min, float Max){
			return min(Max, max(val, Min));
		};
		// α二アーの前か後ろで判定を変える
		if (nowFlame >= alphaNear)
		{
			// 100-100=0  100-50=50   0/50
			float A = (float)(endFlame - nowFlame);
			float B = (float)(endFlame - alphaFar);
			alpha = A / B;
			//alpha = Clamp(alpha, 0.0f, 1.0f);

		}
		else
		{
			// 最初の
			alpha = (float)nowFlame / (float)alphaNear;//   0/30=0   60/30=2   1-(0~1)  

		}

		//alpha = (alphaFar - nowFlame) / (alphaFar - alphaNear);
		alpha = Clamp(alpha, 0.0f, 1.0f);//指定された値を 0 ～ 1 の範囲にクランプします


	}

	// メッシュの更新
	obj->SetScale(scale);
	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->Update();

}

void AnimationUV::Render()
{
	if (isAction == false)return;//実行されてないなら出てけ！！

	// 描画の前に情報を送る
	//　シェーダーに描画毎UV座標を送る
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// 透明度

	// 描画
	obj->Render(shaderM, "uvAnime");


}

void AnimationUV::Render_ADD()
{
	if (isAction == false)return;//実行されてないなら出てけ！！

	// 描画の前に情報を送る
	//　シェーダーに描画毎UV座標を送る
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// 透明度

	// 描画
	obj->Render(shaderM, "uvAnime_add");

}

void AnimationUV::Render_ADD_NOZ()
{
	if (isAction == false)return;//実行されてないなら出てけ！！

	// 描画の前に情報を送る
	//　シェーダーに描画毎UV座標を送る
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// 透明度

	// 描画
	obj->Render(shaderM, "uvAnime_add_noz");

}

// ガード用描画
void AnimationUV::Render_Guard()			
{
	if (isAction == false)return;//実行されてないなら出てけ！！

	// 描画の前に情報を送る
	//　シェーダーに描画毎UV座標を送る
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// 透明度

	// 描画
	obj->Render(shaderM, "uvAnime_guard");

}

// エリアの壁用
void AnimationUV::Render_AreaWall()
{
	if (isAction == false)return;//実行されてないなら出てけ！！

	 // 描画の前に情報を送る
	 //　シェーダーに描画毎UV座標を送る
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// 透明度

										// 描画
	obj->Render(shaderM, "uvAnime_areaWall");
}


// 拡大アニメ
void AnimationUV::ScaleAnimation(float StartScale, float EndScale)
{
	m_startScale = StartScale;
	m_endScale = EndScale;
}

// 回転アニメ
void AnimationUV::AngleAnimation(Vector3 StartAngle, Vector3 EndAngle)
{
    m_startAngle = StartAngle;
    m_endAngle = EndAngle;
}
