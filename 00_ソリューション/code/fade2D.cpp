//*****************************************************
//
// 2Dフェードの処理[fade2D.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fade2D.h"
#include "polygon2D.h"
#include "manager.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CFade2D::FuncUpdateState CFade2D::s_aFuncUpdateState[] =
{
	nullptr,			// 何もしていない状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CFade2D::CFade2D() : m_state(E_State::STATE_NONE), m_fTimer(0.0f), m_bBouceIn(false), m_bBouceOut(false), m_pathNext(""), m_sizeNext()
{

}

//====================================================
// デストラクタ
//====================================================
CFade2D::~CFade2D()
{

}

//====================================================
// 生成処理
//====================================================
CFade2D *CFade2D::Create(CPolygon2D *pPolygon, float fTimeMax)
{
	CFade2D *pFade2D = nullptr;

	pFade2D = new CFade2D;

	if (pFade2D != nullptr)
	{
		pFade2D->m_pPoygon = pPolygon;
		pFade2D->m_fTimeMax = fTimeMax;
		pFade2D->Init();
	}

	return pFade2D;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CFade2D::Init(void)
{
	// 次のサイズを同じにする
	m_sizeNext = m_pPoygon->GetSize();

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CFade2D::Uninit(void)
{
	Release();
}

//====================================================
// 更新処理
//====================================================
void CFade2D::Update(void)
{
	switch (m_state)
	{
	case CFade2D::STATE_IN:
		m_fTimer += CManager::GetDeltaTime();

		if (m_fTimer > m_fTimeMax)
		{
			if (m_bBouceIn)
				m_state = CFade2D::STATE_OUT;
			else
				m_state = CFade2D::STATE_NONE;
		}

		
		break;
	case CFade2D::STATE_OUT:
		m_fTimer -= CManager::GetDeltaTime();
		
		if (m_fTimer < 0.0f)
		{
			if (m_bBouceOut)
			{
				if (m_pathNext != "")
				{// 次のテクスチャを貼る
					int nIdxTexture = Texture::GetIdx(&m_pathNext[0]);
					m_pPoygon->SetIdxTexture(nIdxTexture);
				}

				m_pPoygon->SetSize(m_sizeNext.x, m_sizeNext.y);
				m_pPoygon->SetVtx();

				m_state = CFade2D::STATE_IN;
			}
			else
				m_state = CFade2D::STATE_NONE;
		}
		break;
	default:
		break;
	}

	universal::LimitValuefloat(&m_fTimer, m_fTimeMax, 0.0f);

	float fRate = easing::EaseInOutExpo(m_fTimer);

	// 色を設定
	if (m_pPoygon != nullptr)
		m_pPoygon->SetAlpha(fRate);
}

//====================================================
// 状態による初期化
//====================================================
void CFade2D::InitByState(E_State state)
{
	switch (state)
	{
	case CFade2D::STATE_IN:
		m_fTimer = 0.0f;
		break;
	case CFade2D::STATE_OUT:
		m_fTimer = m_fTimeMax;
		break;
	default:
		break;
	}

	SetState(state);
}

//====================================================
// 描画処理
//====================================================
void CFade2D::Draw(void)
{

}