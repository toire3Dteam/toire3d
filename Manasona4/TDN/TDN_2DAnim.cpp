#include	"TDNLIB.h"
#include    "../source/system/System.h"

/****************************************************/
//	2DObjをラップして様々な特殊機能をつけた演出
/****************************************************/

// アクションチェック
bool AnimAction::Base::ActionCheckUpdate()
{

	// アクションフラグがたっていないと返す
	if (m_bActionFlag == false) return false;
	
	m_bFirstUpdateCheak = true; // 更新が一回以上入った

	// ?
	// ディレイタイマーが0になるまで通さない
	m_iDelayFrame--;
	if (m_iDelayFrame > 0)
	{
		return false;
	}
	m_iDelayFrame = 0;

	return true; //成功
}

bool AnimAction::Base::ActionCheakRender()
{
	if (m_bActionFlag == false)return false;
	if (m_bFirstUpdateCheak == false)return false;
	if (m_iDelayFrame > 0)return false;

	return true;

}

/******************************************/
// アニメ用描画
/******************************************/
// 2D
void AnimAction::Base::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;
	
	pic->Render(x, y, dwFlags);
}

void AnimAction::Base::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	if (ActionCheakRender() == false)return;
	pic->Render(x, y, shader, name);
}

void AnimAction::Base::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;
	pic->Render(x, y, w, h, tx, ty, tw, th, dwFlags);

}

void AnimAction::Base::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	if (ActionCheakRender() == false)return;
	pic->Render(x, y, w, h, tx, ty, tw, th, shader, name);
}

// 3D
void AnimAction::Base::Render3D(tdn2DObj* pic, float x, float y, float z, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;
	pic->Render3D(x, y, z, dwFlags);
}

void AnimAction::Base::Render3D(tdn2DObj* pic, Vector3 pos, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;
	pic->Render3D(pos, dwFlags);
}

void AnimAction::Base::Render3D(tdn2DObj* pic, float x, float y, float z, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;
	pic->Render3D(x, y, z, w, h, tx, ty, tw, th, dwFlags);
}

void AnimAction::Base::Render3D(tdn2DObj* pic, Vector3 pos, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;
	pic->Render3D(pos, w, h, tx, ty, tw, th, dwFlags);
}

// 一部の特殊演出用
void AnimAction::Base::RenderSpecial(tdn2DObj* pic, int x, int y)
{
	if (ActionCheakRender() == false)return;

	pic->Render(x, y);
}

/**********************/
// 波紋
/**********************/

AnimAction::Ripple::Ripple(int endFlame, float startScale, float moveScale)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_startscale = startScale;
	m_movescale = moveScale;

}

AnimAction::Ripple::~Ripple()
{

}

void AnimAction::Ripple::Update(tdn2DObj * pic)
{
	// アクションフラグがたっていないと返す
	//if (m_bActionFlag == false)return;
	if (ActionCheckUpdate() == false)return;

	// フレーム更新
	m_nowFlame++;
	// エンドフレームまで来たら終わる
	if (m_nowFlame >= m_endFlame)
	{
		m_bActionFlag = false;
		m_bEndFlag = true; // 終りフラグON
		// 拡大率更新
		//pic->SetScale(m_startscale);
	}

	// アルファ処理
	float alpha = (float)m_nowFlame / (float)m_endFlame;//   0/30=0   60/30=2   1-(0~1)  
	alpha = 1.0f - alpha;
	pic->SetARGB((int)(alpha * 255), 255, 255, 255);

	// 拡大率更新
	pic->SetScale(pic->GetScale() + m_movescale);


}

void AnimAction::Ripple::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // 実行フラグOn

	// 初期化
	m_nowFlame = 0;
	pic->SetScale(m_startscale);

}


/******************************************/
// 移動しながら登場
/******************************************/

AnimAction::MoveAppeared::MoveAppeared(int endflame, int startX, int startY)
{
	m_nowFlame = 0;
	m_endFlame= endflame;

	m_startX = startX;
	m_startY = startY;

	m_rate = 0.0f;
}

AnimAction::MoveAppeared::~MoveAppeared()
{

}

void AnimAction::MoveAppeared::Update(tdn2DObj * pic)
{
	// アクションフラグがたっていないと返す
	if (ActionCheckUpdate() == false)return;

	// フレーム更新
	m_nowFlame++;
	// エンドフレームまで来たらそのまま待機
	if (m_nowFlame >= m_endFlame)
	{
		//m_bActionFlag = false;
		m_nowFlame = m_endFlame;

		m_bEndFlag = true; // 終りフラグON
	}
	//else
	//{

	//}

	// アルファ処理
	m_rate = (float)m_nowFlame / (float)m_endFlame;//   0~なにかへ
	Math::Clamp(m_rate, 0.0f, 1.0f);//   0~1.0fへ

	pic->SetARGB((int)(m_rate * 255), 255, 255, 255);

	

}

void AnimAction::MoveAppeared::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);

	// 初期化
	m_nowFlame = 0;
	m_rate = 0.0f;
}

// 2D
void AnimAction::MoveAppeared::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;
	pic->Render(
		(int)(Math::Blend(m_rate,(float)m_startX, (float)x)),
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		dwFlags);
}

void AnimAction::MoveAppeared::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	if (ActionCheakRender() == false)return;
	pic->Render(
		(int)(Math::Blend(m_rate, (float)m_startX, (float)x)),
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		shader, name);
	//つぎここから
}

void AnimAction::MoveAppeared::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;
	pic->Render(
		(int)(Math::Blend(m_rate, (float)m_startX, (float)x)),
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		w, h, tx, ty, tw, th, dwFlags);

}

void AnimAction::MoveAppeared::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	if (ActionCheakRender() == false)return;
	pic->Render(
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		w, h, tx, ty, tw, th, shader, name);
}

// MoveAppendの特殊演出用
void AnimAction::MoveAppeared::RenderSpecial(tdn2DObj* pic, int x, int y)
{
	if (ActionCheakRender() == false)return;

	// 移動ブラー
	if (m_rate < 1.0f)
	{
		Vector2 vec;
		vec.x = (float)x - (float)m_startX;
		vec.y = (float)y - (float)m_startY;

		vec.Normalize();

		// シェーダ側に転送
		shaderM->SetValue("DirectionalX", vec.x);
		shaderM->SetValue("DirectionalY", vec.y);

		pic->Render(
			(int)(Math::Blend(m_rate, (float)m_startX, (float)x)),
			(int)(Math::Blend(m_rate, (float)m_startY, (float)y))
			,shaderM, "DirectionalBlur");
	}
	else
	{
		pic->Render(
			(int)(Math::Blend(m_rate, (float)m_startX, (float)x)),
			(int)(Math::Blend(m_rate, (float)m_startY, (float)y))
			);
	}


}

/**********************/
// シェイク
/**********************/

AnimAction::Shake::Shake(int endFlame, float x, float y, int cycle)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_shakeX = x / 2.0f;
	m_shakeY = y / 2.0f;
	m_cycle = cycle;
	m_powerX = x / cycle; //10/1=10 10/2=5;
	m_powerY = y / cycle; //10/1=10 10/2=5;
	
	m_orgX = x;
	m_orgY = y;

	m_bTrunOver = false;
}

AnimAction::Shake::~Shake()
{

}

void AnimAction::Shake::Update(tdn2DObj * pic)
{
	// アクションフラグがたっていないと返す
	//if (m_bActionFlag == false)return;
	if (ActionCheckUpdate() == false)return;

	// フレーム更新
	m_nowFlame++;
	// エンドフレームまで来たら終わる
	if (m_nowFlame >= m_endFlame)
	{
		m_bActionFlag = false;
		m_bEndFlag = true; // 終りフラグON
		m_shakeX = (m_orgX / 2.0f);
		m_shakeY = (m_orgY / 2.0f);
		return;
	}

	// サイクル処理　そのサイクルまでに最大到達点にする
	//float rate = m_shakeX / m_cycle; //10/1=10 10/2=5

	// 0になったらフラグチェンジ！
	if (m_nowFlame%m_cycle == 0)
	{
		if (m_bTrunOver)
		{
			m_bTrunOver = false;
		}
		else
		{
			m_bTrunOver = true;
		}
	}
	
	// 更新
	if (m_bTrunOver)
	{
		// 更新
		m_shakeX -= m_powerX;
		m_shakeY -= m_powerY;
		if (m_shakeX <= 0)m_shakeX = 0;
		if (m_shakeY <= 0)m_shakeY = 0;
	}
	else
	{
		// 更新
		m_shakeX += m_powerX;
		m_shakeY += m_powerY;

		if (m_shakeX >= m_orgX)m_shakeX = m_orgX;
		if (m_shakeY >= m_orgY)m_shakeY = m_orgY;
	}


}

void AnimAction::Shake::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // 実行フラグOn

	// 初期化
	m_nowFlame = 0;

}

// 2D
void AnimAction::Shake::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	pic->Render(x+(int)(m_shakeX-(m_orgX/2.0f)), y + (int)(m_shakeY - (m_orgY / 2)), dwFlags);
}

void AnimAction::Shake::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	pic->Render(x + (int)(m_shakeX - (m_orgX / 2.0f)), y + (int)(m_shakeY - (m_orgY / 2)), shader, name);
}

void AnimAction::Shake::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	pic->Render(x + (int)(m_shakeX - (m_orgX / 2.0f)), y + (int)(m_shakeY - (m_orgY / 2)), w, h, tx, ty, tw, th, dwFlags);

}

void AnimAction::Shake::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	pic->Render(x + (int)(m_shakeX - (m_orgX / 2.0f)), y + (int)(m_shakeY - (m_orgY / 2)), w, h, tx, ty, tw, th, shader, name);
}





/**********************/
// 飛び出す
/**********************/

AnimAction::Jump::Jump(int endFlame, float startScale, float maxScale)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_startscale = startScale;

	m_maxScale = maxScale;
	m_moveScale = maxScale / endFlame; //10/1=10 10/2=5;

//	m_orgAlpha = 1.0f;
//	m_alpha = 1.0f;
}

AnimAction::Jump::~Jump()
{

}

void AnimAction::Jump::Update(tdn2DObj * pic)
{
	// アクションフラグがたっていないと返す
	if (ActionCheckUpdate() == false)return;

	// フレーム更新
	m_nowFlame++;
	// エンドフレームまで来たら終わる
	//if (m_nowFlame >= m_endFlame)
	//{
	//	m_bActionFlag = false;
	//	// 拡大率更新
	//	//pic->SetScale(m_startscale);
	//}

	// アルファ処理
	//float rate = (float)m_nowFlame / (float)m_endFlame;//   0/30=0   60/30=2   1-(0~1)  


	if (m_nowFlame >= m_endFlame)
	{
		// 小さく
		pic->SetScale(pic->GetScale() - m_moveScale);

		// スタート時よりも小さくなったら終了
		if (pic->GetScale() <= m_startscale)
		{
			// もどす
			pic->SetScale(m_startscale);
			m_bEndFlag = true; // 終りフラグON
			//m_bActionFlag = false;
			//m_alpha = (float)m_nowFlame / (float)m_endFlame;
			//m_alpha=
			return;
		}


	}
	else
	{		
		// 大きく
		pic->SetScale(pic->GetScale() + m_moveScale);
	
	//	m_alpha = (float)m_nowFlame / (float)m_endFlame;
	}




}

void AnimAction::Jump::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);

	// 初期化
	m_nowFlame = 0;
	pic->SetScale(m_startscale);

}


// 2D
void AnimAction::Jump::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	//pic->SetARGB(255, 255, 255, 255);
	pic->Render(x, y, dwFlags);
	
	//pic->SetARGB((int)(m_alpha * 255), 255, 255, 255);
	//pic->Render(x, y, RS::ADD);

}

void AnimAction::Jump::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	pic->Render(x, y, shader, name);
}

void AnimAction::Jump::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	pic->Render(x, y, w, h, tx, ty, tw, th, dwFlags);

}

void AnimAction::Jump::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	pic->Render(x, y, w, h, tx, ty, tw, th, shader, name);
}



/**********************/
// 集まる用に出現
/**********************/

AnimAction::Shrink::Shrink(int endFlame, float startScale, float maxScale)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_startScale = startScale;
	m_maxScale = maxScale;

	m_nowScale = m_maxScale;

	float scaleRata = maxScale - startScale;

	m_addScale = scaleRata / endFlame;

	m_alpha = 0;
}

AnimAction::Shrink::~Shrink()
{

}

void AnimAction::Shrink::Update(tdn2DObj * pic)
{
	// アクションフラグがたっていないと返す
	//if (m_bActionFlag == false)return;
	if (ActionCheckUpdate() == false)return;

	// フレーム更新
	m_nowFlame++;
	// エンドフレームまで来たら終わる
	if (m_nowFlame >= m_endFlame)
	{
		m_bEndFlag = true; // 終りフラグON
		//m_bActionFlag = false;
		// 拡大率更新
		//pic->SetScale(m_startscale);
		//return;
	}

	// アルファ処理
	m_alpha = (float)m_nowFlame / (float)m_endFlame;//   0/30=0   60/30=2   1-(0~1)
	Math::Clamp(m_alpha, 0.0f, 1.0f);

	//alpha = 1.0f - alpha;
	pic->SetARGB((int)(m_alpha * 255), 255, 255, 255);

	// スケールを引いていく
	m_nowScale -= m_addScale;
	if (m_nowScale <= m_startScale)
	{
		m_nowScale = m_startScale;
	}

	// 拡大率更新
	pic->SetScale(m_nowScale);

}

void AnimAction::Shrink::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // 実行フラグOn

	// 初期化
	m_nowFlame = 0;

	m_nowScale = m_maxScale;
	pic->SetScale(m_nowScale);

}

// 2D
void AnimAction::Shrink::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	if (ActionCheakRender() == false)return;

	pic->Render(x, y, dwFlags);
}

/**********************/
// 大きくなる
/**********************/

AnimAction::Grow::Grow(int endFlame, float startScale, float moveScale)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_startscale = startScale;
	m_movescale = moveScale;

}

AnimAction::Grow::~Grow()
{

}

void AnimAction::Grow::Update(tdn2DObj * pic)
{
	// アクションフラグがたっていないと返す
	//if (m_bActionFlag == false)return;
	if (ActionCheckUpdate() == false)return;

	// フレーム更新
	m_nowFlame++;
	// エンドフレームまで来たら終わる
	if (m_nowFlame >= m_endFlame)
	{
		//m_bActionFlag = false;
		m_bEndFlag = true; // 終りフラグON
						   // 拡大率更新
						   //pic->SetScale(m_startscale);

		return; // 終り
	}

	// アルファ処理
	float alpha = (float)m_nowFlame / (float)m_endFlame;//   0/30=0   60/30=2   1-(0~1)  
	//alpha = 1.0f - alpha;
	pic->SetARGB((int)(alpha * 255), 255, 255, 255);

	// 拡大率更新
	pic->SetScale(pic->GetScale() + m_movescale);


}

void AnimAction::Grow::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // 実行フラグOn

	// 初期化
	m_nowFlame = 0;
	pic->SetScale(m_startscale);

}

/**********************/
// なし
/**********************/

AnimAction::None::None()
{
}

AnimAction::None::~None()
{

}

void AnimAction::None::Update(tdn2DObj * pic)
{
	// なんもしない　しいてあるとすればディレイのみ
	if (ActionCheckUpdate() == false)return;

}

void AnimAction::None::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
}

/**********************/
// アルファ変更
/**********************/

AnimAction::AlphaMove::AlphaMove(int endFlame, int arrivalFrame, int vanishFrame)
{
	m_iNowFrame = 0;
	m_iEndFrame = endFlame;
	m_iArrivalFrame = arrivalFrame;
	m_iVanishFrame = vanishFrame;
}

AnimAction::AlphaMove::~AlphaMove()
{

}

void AnimAction::AlphaMove::Update(tdn2DObj * pic)
{
	// アクションフラグがたっていないと返す
	if (ActionCheckUpdate() == false)return;

	// フレーム更新
	m_iNowFrame++;
	// エンドフレームまで来たら終わる
	if (m_iNowFrame >= m_iEndFrame)
	{
		// アニメーション終り
		m_bEndFlag = true;
	}

	// アルファ更新
	if (m_iNowFrame <= m_iArrivalFrame)
	{
		// アルファ処理
		float alpha = (float)m_iNowFrame / (float)m_iArrivalFrame;
		pic->SetARGB((int)(alpha * 255), 255, 255, 255);
	}
	else if (m_iNowFrame >= m_iVanishFrame)
	{
		float l_fParam = (float)(m_iEndFrame - m_iNowFrame);
		float l_fMax = (float)(m_iEndFrame - m_iVanishFrame);
		float l_fAlpha = l_fParam / l_fMax;
		pic->SetARGB((int)(l_fAlpha * 255), 255, 255, 255);
	}
	else
	{
		// 真ん中は何もしない
		pic->SetARGB((int)(255), 255, 255, 255);
	}



}

void AnimAction::AlphaMove::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);

	// 初期化
	m_iNowFrame = 0;
}


/**********************/
// 引き伸ばし
/**********************/
void AnimAction::Stretch::Update(tdn2DObj * pic)
{
	// アクションフラグがたっていないと返す
	if (ActionCheckUpdate() == false)return;

	// エンドフレームまで来たら終わる
	if (++m_nowFrame >= m_endFrame)
	{
		m_bEndFlag = true; // 終りフラグON
		return; // 終り
	}

	const float rate((float)(m_nowFrame / (m_endFrame - 1.0f)));// 終わる前にレートが最大になるように-1
	m_CurrentScale.x = Math::Blend(rate, m_StartScale.x, 1);
	m_CurrentScale.y = Math::Blend(rate, m_StartScale.y, 1);
}

void AnimAction::Stretch::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // 実行フラグOn

	// 初期化
	m_nowFrame = 0;
	m_CurrentScale = m_StartScale;
}

// 2D
void AnimAction::Stretch::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	int X(m_width / 2 + x), Y(m_height / 2 + y);
	X -= (int)(m_width / 2 * m_CurrentScale.x);
	Y -= (int)(m_height / 2 * m_CurrentScale.y);
	pic->Render(X, Y, (int)(m_width * m_CurrentScale.x), (int)(m_height * m_CurrentScale.y), 0, 0, m_width, m_height, dwFlags);
}

void AnimAction::Stretch::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	int X(m_width / 2 + x), Y(m_height / 2 + y);
	X -= (int)(m_width / 2 * m_CurrentScale.x);
	Y -= (int)(m_height / 2 * m_CurrentScale.y);
	pic->Render(X, Y, (int)(m_width * m_CurrentScale.x), (int)(m_height * m_CurrentScale.y), 0, 0, m_width, m_height, shader, name);
}

void AnimAction::Stretch::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	int X(m_width / 2 + x), Y(m_height / 2 + y);
	X -= (int)(m_width / 2 * m_CurrentScale.x);
	Y -= (int)(m_height / 2 * m_CurrentScale.y);
	pic->Render(X, Y, (int)(m_width * m_CurrentScale.x), (int)(m_height * m_CurrentScale.y), tx, ty, tw, th, dwFlags);
}

void AnimAction::Stretch::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	int X(m_width / 2 + x), Y(m_height / 2 + y);
	X -= (int)(m_width / 2 * m_CurrentScale.x);
	Y -= (int)(m_height / 2 * m_CurrentScale.y);
	pic->Render(X, Y, (int)(m_width * m_CurrentScale.x), (int)(m_height * m_CurrentScale.y), tx, ty, tw, th, shader, name);
}


/******************************************/
// 描画をアニメ用に加工
/******************************************/

void tdn2DAnim::OrderRipple(int endFlame, float startScale, float moveScale)
{
	if (m_pAction != nullptr) delete m_pAction; 
	m_pAction = new AnimAction::Ripple(endFlame, startScale, moveScale); 

}

void tdn2DAnim::OrderMoveAppeared(int endFlame, int startX, int startY)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::MoveAppeared(endFlame, startX, startY);
}

void tdn2DAnim::OrderShake(int endFlame, float shakeX, float shakeY, int cycle)
{	
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::Shake(endFlame, shakeX, shakeY, cycle);
}

void tdn2DAnim::OrderJump(int endFlame, float startScale, float addScale)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::Jump(endFlame, startScale, addScale);
}
// 集まる用に出現
void tdn2DAnim::OrderShrink(int endFlame, float startScale, float maxScale)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::Shrink(endFlame, startScale, maxScale);
}

void tdn2DAnim::OrderGrow(int endFlame, float startScale, float moveScale)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::Grow(endFlame, startScale, moveScale);
}

void tdn2DAnim::OrderNone()
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::None();
}

void tdn2DAnim::OrderAlphaMove(int endFlame, int arrivalFrame, int vanishFrame)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::AlphaMove(endFlame, arrivalFrame, vanishFrame);
}