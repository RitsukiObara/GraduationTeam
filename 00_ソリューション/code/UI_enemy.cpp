//*****************************************************
//
// 敵数表示のUI[UIEnemy.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "UI_enemy.h"
#include "number.h"
#include "enemy.h"
#include "UI.h"
#include "texture.h"
#include "inputkeyboard.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_INIT_ICON = { 0.9f,0.08f,0.0f };	// アイコンの初期位置
const D3DXVECTOR2 SIZE_INIT_ICON = { 0.02f,0.03f };	// 数字の初期サイズ
const string PATH_ICON = "data\\TEXTURE\\UI\\icon_seal.png";	// アイコンのパス

const int NUM_ROW = 2; // 行の数
const int NUM_COLUMN = 5; // 列の数
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CUIEnemy *CUIEnemy::s_pUIEnemy = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CUIEnemy::CUIEnemy()
{

}

//=====================================================
// デストラクタ
//=====================================================
CUIEnemy::~CUIEnemy()
{

}

//=====================================================
// 生成処理
//=====================================================
CUIEnemy *CUIEnemy::Create(void)
{
	if (s_pUIEnemy == nullptr)
	{// インスタンス生成
		s_pUIEnemy = new CUIEnemy;

		if (s_pUIEnemy == nullptr)
			return nullptr;

		s_pUIEnemy->Init();
	}

	return s_pUIEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CUIEnemy::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUIEnemy::Uninit(void)
{
	s_pUIEnemy = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CUIEnemy::Update(void)
{
#ifdef _DEBUG
	Debug();	// デバッグ処理
#endif
}

//=====================================================
// デバッグ処理
//=====================================================
void CUIEnemy::Debug(void)
{
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	if (pInputKeyboard == nullptr)
		return;

	if (pInputKeyboard->GetTrigger(DIK_UP))
	{
		AddEnemy();
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		DeleteEnemy();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CUIEnemy::Draw(void)
{

}

//=====================================================
// 敵の追加
//=====================================================
void CUIEnemy::AddEnemy(void)
{
	// アイコンを増やす
	CUI *pIcon = CUI::Create();

	if (pIcon == nullptr)
		return;

	// 位置番号の計算
	int nSizeArray = (int)m_apIcon.size();
	int nIdxRow = nSizeArray % NUM_COLUMN;
	int nIdxColumn = nSizeArray / NUM_COLUMN;

	// 位置番号を反映
	D3DXVECTOR3 posIcon = POS_INIT_ICON;
	posIcon += D3DXVECTOR3( -SIZE_INIT_ICON.x * nIdxRow * 2, SIZE_INIT_ICON.y * nIdxColumn * 2, 0.0f );
	pIcon->SetPosition(posIcon);

	// サイズ設定
	pIcon->SetSize(SIZE_INIT_ICON.x, SIZE_INIT_ICON.y);
	pIcon->SetVtx();

	// テクスチャ設定
	int nIdx = Texture::GetIdx(&PATH_ICON[0]);
	pIcon->SetIdxTexture(nIdx);

	m_apIcon.push_back(pIcon);
}

//=====================================================
// 敵の削除
//=====================================================
void CUIEnemy::DeleteEnemy(void)
{
	if (m_apIcon.empty())
		return;
	
	int nSizeArray = (int)m_apIcon.size();

	CUI* pIcon = m_apIcon[nSizeArray - 1];

	pIcon->Uninit();

	m_apIcon.erase(m_apIcon.end() - 1);
}