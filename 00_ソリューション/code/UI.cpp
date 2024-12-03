//*****************************************************
//
// UIの処理[UI.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "UI.h"
#include "UIManager.h"
#include "blur.h"

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CUI::CUI(int nPriority) : CPolygon2D(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CUI::~CUI()
{

}

//=====================================================
// 生成処理
//=====================================================
CUI *CUI::Create(void)
{
	CUI *pUI = nullptr;

	if (pUI == nullptr)
	{
		// インスタンス生成
		pUI = new CUI;

		if (pUI != nullptr)
		{
			// 初期化処理
			pUI->Init();
		}
	}

	return pUI;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CUI::Init(void)
{
	CPolygon2D::Init();
	
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUI::Uninit(void)
{
	CPolygon2D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CUI::Update(void)
{
	CPolygon2D::Update();
}

//=====================================================
// 描画
//=====================================================
void CUI::Draw(void)
{
	bool bDisp = true;

	CUIManager *pUIManager = CUIManager::GetInstance();

	if (pUIManager != nullptr)
	{// 描画するかの取得
		bDisp = pUIManager->IsDisp();
	}

	if (bDisp)
	{
		CPolygon2D::Draw();
	}
}

//=====================================================
// サイズの設定
//=====================================================
void CUI::SetSize(float width, float height)
{
	float fWidth;
	float fHeigth;

	fWidth = SCREEN_WIDTH * width;
	fHeigth = SCREEN_HEIGHT * height;

	CPolygon2D::SetSize(fWidth, fHeigth);
}

//=====================================================
// サイズの取得
//=====================================================
D3DXVECTOR2 CUI::GetSize(void)
{
	D3DXVECTOR2 size = CPolygon2D::GetSize();

	size.x /= SCREEN_WIDTH;
	size.y /= SCREEN_HEIGHT;

	return size;
}

//=====================================================
// 幅の設定
//=====================================================
void CUI::SetWidth(float width)
{
	float fWidth = SCREEN_WIDTH * width;
	CPolygon2D::SetSize(fWidth, CPolygon2D::GetHeight());
}

//=====================================================
// 高さの設定
//=====================================================
void CUI::SetHeight(float height)
{
	float fHeigth = SCREEN_HEIGHT * height;
	CPolygon2D::SetSize(CPolygon2D::GetWidth(), fHeigth);
}

//=====================================================
// 幅の取得
//=====================================================
float CUI::GetWidth(void)
{
	return GetSize().x;
}

//=====================================================
// 高さの取得
//=====================================================
float CUI::GetHeight(void)
{
	return GetSize().y;
}

//=====================================================
// 位置設定
//=====================================================
void CUI::SetPosition(D3DXVECTOR3 pos)
{
	pos.x *= SCREEN_WIDTH;
	pos.y *= SCREEN_HEIGHT;

	CGameObject::SetPosition(pos);
}

//=====================================================
// 位置取得
//=====================================================
D3DXVECTOR3 CUI::GetPosition(void)
{
	D3DXVECTOR3 pos = CPolygon2D::GetPosition();

	pos.x /= SCREEN_WIDTH;
	pos.y /= SCREEN_HEIGHT;

	return pos;
}