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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_PLACE = 2;	// 数字の桁数
const D3DXVECTOR3 POS_INIT_NUMBER = { 0.9f,0.08f,0.0f };	// 数字の初期位置
const D3DXVECTOR2 SIZE_INIT_NUMBER = { 0.02f,0.05f };	// 数字の初期サイズ

const D3DXVECTOR3 POS_INIT_ICON = { 0.83f,0.08f,0.0f };	// アイコンの初期位置
const D3DXVECTOR2 SIZE_INIT_ICON = { 0.04f,0.06f };	// 数字の初期サイズ
const string PATH_ICON = "data\\TEXTURE\\UI\\icon_seal.png";	// アイコンのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CUIEnemy *CUIEnemy::m_pUIEnemy = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CUIEnemy::CUIEnemy() : m_pNumber(nullptr), m_pIcon(nullptr)
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
	if (m_pUIEnemy == nullptr)
	{// インスタンス生成
		m_pUIEnemy = new CUIEnemy;

		if (m_pUIEnemy == nullptr)
			return nullptr;

		m_pUIEnemy->Init();
	}

	return m_pUIEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CUIEnemy::Init(void)
{
	// 数字の生成
	m_pNumber = CNumber::Create(NUM_PLACE, 0);

	if (m_pNumber == nullptr)
		return E_FAIL;

	// 数字の初期化
	m_pNumber->SetPosition(POS_INIT_NUMBER);
	m_pNumber->SetSizeAll(SIZE_INIT_NUMBER.x, SIZE_INIT_NUMBER.y);

	// アイコンの生成
	m_pIcon = CUI::Create();

	if (m_pIcon == nullptr)
		return E_FAIL;

	// アイコンの初期化
	m_pIcon->SetPosition(POS_INIT_ICON);
	m_pIcon->SetSize(SIZE_INIT_ICON.x, SIZE_INIT_ICON.y);
	m_pIcon->SetVtx();

	int nIdx = Texture::GetIdx(&PATH_ICON[0]);
	m_pIcon->SetIdxTexture(nIdx);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUIEnemy::Uninit(void)
{
	m_pUIEnemy = nullptr;

	Object::DeleteObject((CObject**)&m_pNumber);
	Object::DeleteObject((CObject**)&m_pIcon);

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CUIEnemy::Update(void)
{
	// 数字の管理
	ManageNumber();
}

//=====================================================
// 数字の管理
//=====================================================
void CUIEnemy::ManageNumber(void)
{
	if (m_pNumber == nullptr)
		return;

	vector<CEnemy*> aEnemy = CEnemy::GetArray();

	int nSize = (int)aEnemy.size();

	m_pNumber->SetValue(nSize);
}

//=====================================================
// 描画処理
//=====================================================
void CUIEnemy::Draw(void)
{
#ifdef _DEBUG

#endif
}