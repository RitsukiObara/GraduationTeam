//*****************************************************
//
// スタート表示の処理[start.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "start.h"
#include "UI.h"
#include "game.h"
#include "texture.h"
#include "sound.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define CAPTION_WIDTH	(0.3f)	// 見出しの幅
#define CAPTION_HEIGHT	(0.2f)	// 見出しの高さ
#define CAPTION_PATH_HAKKEYOI "data\\TEXTURE\\UI\\start00.png"	// はっけよいのパス
#define CAPTION_PATH_NOKOTTA "data\\TEXTURE\\UI\\start01.png"	// のこったのパス
#define SPEED_HAKKEYOI (0.005f)	// はっけよいのスピード
#define SPEED_NOKOTTA (0.05f)	// のこったのスピード

#define TIME_HAKKEYOI (1.02f)	// はっけよいの時間
#define TIME_NOKOTTA  (0.3f)	// のこったの時間

#define SPEED_EXTEND_NOKOTTA (1.1f)	// のこったのでかくなる割合

//*****************************************************
// 静的メンバ変数
//*****************************************************
CStart *CStart::s_pStart = nullptr;	// 自身のポインタ

//====================================================
// コンストラクタ
//====================================================
CStart::CStart()
{
	m_state = STATE_NONE;
	m_fTimer = 0.0f;
}

//====================================================
// デストラクタ
//====================================================
CStart::~CStart()
{

}

//====================================================
// 生成処理
//====================================================
CStart *CStart::Create(void)
{
	if (s_pStart == nullptr)
	{
		s_pStart = new CStart;

		if (s_pStart != nullptr)
		{
			s_pStart->Init();
		}
	}

	return s_pStart;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CStart::Init(void)
{
	m_state = STATE_HAKKEYOI;

	CGame::SetState(CGame::STATE::STATE_START);

	// 見出しの生成
	m_pCaption = CUI::Create();

	if (m_pCaption != nullptr)
	{
		m_pCaption->SetPosition(D3DXVECTOR3(0.5f, 0.4f, 0.0f));

		m_pCaption->SetSize(CAPTION_WIDTH, CAPTION_HEIGHT);

		m_pCaption->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

		int nIdxTexture = CTexture::GetInstance()->Regist(CAPTION_PATH_HAKKEYOI);

		m_pCaption->SetIdxTexture(nIdxTexture);

		m_pCaption->SetVtx();
	}

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CStart::Uninit(void)
{
	s_pStart = nullptr;

	if (m_pCaption != nullptr)
	{
		m_pCaption->Uninit();
		m_pCaption = nullptr;
	}

	Release();
}

//====================================================
// 更新処理
//====================================================
void CStart::Update(void)
{
	m_fTimer += SPEED_HAKKEYOI;

	switch (m_state)
	{
	case CStart::STATE_HAKKEYOI:
	{// 不透明度を上げていく
		float fAlpha = easing::OutQuint(m_fTimer);

		universal::LimitValuefloat(&fAlpha, 1.0f, 0.0f);

		if (m_pCaption != nullptr)
		{
			m_pCaption->SetAlpha(fAlpha);

			if (m_fTimer > TIME_HAKKEYOI)
			{// のこったに移行
				m_state = STATE::STATE_NOKOTTA;

				// テクスチャ切り替え
				int nIdxTexture = CTexture::GetInstance()->Regist(CAPTION_PATH_NOKOTTA);
				m_pCaption->SetIdxTexture(nIdxTexture);
				m_fTimer = 0.0f;
			}
		}
	}
		break;
	case CStart::STATE_NOKOTTA:
	{// 拡大する
		if (m_pCaption == nullptr)
			return;

		D3DXVECTOR2 size = m_pCaption->GetSize();

		size *= SPEED_EXTEND_NOKOTTA;

		m_pCaption->SetSize(size.x, size.y);
		m_pCaption->SetVtx();

		float fAlpha = 1.0f - easing::OutQuint(m_fTimer);

		universal::LimitValuefloat(&fAlpha, 1.0f, 0.0f);

		m_pCaption->SetAlpha(fAlpha);

		if (m_fTimer > TIME_NOKOTTA)
		{// のこったに移行
			CGame::SetState(CGame::STATE::STATE_NORMAL);

			// BGM再生
			CSound* pSound = CSound::GetInstance();

			if (pSound != nullptr)
			{
				pSound->Play(pSound->LABEL_BGM_GAME01);
				pSound->Play(pSound->LABEL_SE_PAUSE_ENTER01);
			}


			Uninit();
		}
	}
		break;
	default:
		break;
	}
}

//====================================================
// 描画処理
//====================================================
void CStart::Draw(void)
{

}