//********************************************************************
//																									
//		３Ｄ用シェーダ		
//
//********************************************************************

//------------------------------------------------------
//		変換行列
//------------------------------------------------------
float4x4 WMatrix;		// ワールド変換行列
float4x4 VMatrix;		// ビュー変換行列
float4x4 PMatrix;		// プロジェクション変換行列

float4x4 WVPMatrix;		// 投影変換行列
float4x4 VPMatrix;		// ワールド座標から→プロジェクション座標へ持っていく行列

float4x4 InvPMatrix;	// プロジェクション座標 ->View座標　へ戻す逆行列
float4x4 InvVPMatrix;	// プロジェクション座標 -> ワールド座標 へ戻す逆行列


//------------------------------------------------------
//		環境関連
//------------------------------------------------------
float3	ViewPos;		// ワールド座標の目のポジション

float3 LightVec;		// 平行光の向き

//------------------------------------------------------
//		テクスチャサンプラー	
//------------------------------------------------------

// MinFilter	縮小フィルタに使用されるメソッド
// MagFilter	拡大フィルタに使用されるメソッド
// MipFilter	MIPMAPに使用されるメソッド
// AddressU	0 - 1の範囲外にあるuテクスチャー座標の解決に使用されるメソッド
// AddressV	0 - 1の範囲外にあるvテクスチャー座標の解決に使用されるメソッド
// LINEAR		バイリニア補間フィルタリング。目的のピクセルに外接する2×2テクセルの重み付き平均領域を使用
// NONE		MIPMAPを無効にし、拡大フィルタを使用
// CLAMP		範囲[0.0, 1.0]の外にあるテクスチャー座標には、それぞれ0.0または1.0のテクスチャーカラーを設定
// MIPMAP		メインとなるテクスチャの画像を補完するよう事前計算され最適化された画像群。詳細さのレベルによって画像を切り替える

// WRAP...反対側から同じ絵を繰り返します
// CLAMP...端の色を繰り返します
// BORDER...透明色にします

texture Texture;
sampler DecaleSamp = sampler_state
{
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

// UV用
sampler DecaleSampUV = sampler_state
{
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	//BorderColor = 0x00FFFFFF;
	AddressU = WRAP;
	AddressV = WRAP;
};


texture NormalMap;	//	法線マップテクスチャ
sampler NormalSamp = sampler_state
{
	Texture = <NormalMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture LightMap;	//	光マップテクスチャ
sampler LightSamp = sampler_state
{
	Texture = <LightMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture MultiMap;		//	マルチマップテクスチャ
sampler MultiSamp = sampler_state
{
	Texture = <MultiMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};


//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float4 wPos			: TEXCOORD1;//　ピクセルに送る情報にワールドからのポジション追加
	float4 ProjectionPos: TEXCOORD2;//　ピクセルに送る情報にゲーム画面ラストのポジション追加
};

struct VS_INPUT
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

//********************************************************************
//
//		基本３Ｄシェーダー		
//
//********************************************************************

//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT VS_Basic(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//TransMatrixとPosを合成してwPosの情報生成
	Out.wPos = mul(In.Pos, WMatrix);

	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.ProjectionPos = Out.Pos;

	return Out;
}
//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Basic(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	ピクセル色決定
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);		

	OUT.r = 1.0f;
	OUT.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique copy
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様
	
		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic();
	}
}


/*ブレンディング オプション*/
/*
BLEND_ZERO			ソースの色は黒 (0, 0, 0, 0) です。ブレンディング前の処理はありません。
BLEND_ONE			ソースの色は白 (1, 1, 1, 1) です。ブレンディング前の処理はありません。
BLEND_SRC_COLOR		ピクセルシェーダーからのカラー データ (RGB) です。ブレンディング前の処理はありません。
BLEND_INV_SRC_COLOR ピクセルシェーダーからのカラー データ (RGB) です。ブレンディング前の処理によってデータが反転され、1 - RGB が生成されます。
BLEND_SRC_ALPHA		ピクセルシェーダーからのアルファ データ (A) です。ブレンディング前の処理はありません。
BLEND_INV_SRC_ALPHA ピクセルシェーダーからのアルファ データ (A) です。ブレンディング前の処理によってデータが反転され、1 - A が生成されます。
BLEND_DEST_ALPHA	レンダーターゲットからのアルファ データです。ブレンディング前の処理はありません。
*/


//**************************************
//
///		ＵＶアニメーション
//
//**************************************

// UV用
//sampler DecaleSampUV = sampler_state
//{
//	Texture = <Texture>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = NONE;
//
//	//BorderColor = 0x00FFFFFF;
//	AddressU = WRAP;
//	AddressV = WRAP;
//};


struct VS_INPUT_UV
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float2 Tex	  : TEXCOORD0;
	float4 Color	: COLOR0;
};
struct VS_OUTPUT_UV
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
};

// UV
float tuAnime = 0.0f;
float tvAnime = 0.0f;
float alphaUV = 1.0f;

VS_OUTPUT_UV VS_UvAnime(VS_INPUT_UV In)
{
	VS_OUTPUT_UV Out = (VS_OUTPUT_UV)0;

	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.Color = In.Color;// 頂点カラー取得
	Out.Tex = In.Tex + float2(tuAnime, tvAnime);//座標

	Out.Color.rgb = 1.0f;
	Out.Color.a *= alphaUV; //　透明度

	return Out;
}


//------------------------------------------------------
///		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_UvAnime(VS_OUTPUT_UV In) : COLOR
{
	float4	OUT;
//	ピクセル色決定
OUT = In.Color * tex2D(DecaleSampUV, In.Tex);
return OUT;
}

technique uvAnime
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;			// このオブジェクトはZバッファを考慮する
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}
}



