#include "TipsCard.h"

TipsCard::TipsCard(LPSTR string)
{
	// •¶Žš
	m_pString = string;
	m_pCardPic = new tdn2DObj("DATA/UI/Menu/TipsCard.png");

	m_bRender = false;
}

TipsCard::~TipsCard()
{
	SAFE_DELETE(m_pCardPic);
}

void TipsCard::Update()
{
	if (m_bRender == false)return;

}

void TipsCard::Render()
{
	if (m_bRender == false)return;

	m_pCardPic->Render(227, 252);

	// •¶Žš•`‰æ
	//tdnFont::RenderString(m_pString, "HGºÞ¼¯¸E", 26, 340, 287, 0xffffffff, RS::COPY);
	tdnFont::RenderStringCentering(m_pString, "HGºÞ¼¯¸E", 26, (1280 / 2) + 16 , 287, 0xffffffff, RS::COPY);

}

void TipsCard::Action()
{
	m_bRender = true;
}

void TipsCard::End()
{
	m_bRender = false;
}
