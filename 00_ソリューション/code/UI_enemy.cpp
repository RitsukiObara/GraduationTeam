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
#include "game.h"
#include "inputkeyboard.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_INIT_ICON = { 0.9f,0.08f,0.0f };	// アイコンの初期位置
const D3DXVECTOR2 SIZE_INIT_ICON = { 0.02f,0.03f };	// 数字の初期サイズ
const string PATH_ICON[CEnemy::TYPE_MAX] =			// アイコンのパス
{
	"data\\TEXTURE\\UI\\icon_seals.png",		// アザラシ
	"data\\TEXTURE\\UI\\icon_whitebear.png"		// シロクマ
};

const int NUM_ROW = 2; // 行の数
const int NUM_COLUMN = 5; // 列の数

const float GRAVITY_ICON = 0.98f;	// アイコンにかかる重力
const D3DXVECTOR3 MOVE_STARTFALL_ICON = { 0.0f,-10.0f,0.0f };	// アイコンが落ち始めるときにかかる移動量
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
	// アイコンの管理
	ManageIcon();

#ifdef _DEBUG
	Debug();	// デバッグ処理
#endif
}

//=====================================================
// アイコンの管理
//=====================================================
void CUIEnemy::ManageIcon(void)
{

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
		AddEnemy(0);
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		DeleteEnemy(0);
		DeleteEnemy(1);
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
void CUIEnemy::AddEnemy(int nType)
{
	// アイコンを増やす
	CIcon *pIcon = CIcon::Create(nType);

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
	pIcon->SetVtx();

	// テクスチャ設定
	int nIdx = Texture::GetIdx(PATH_ICON[nType].c_str());
	pIcon->SetIdxTexture(nIdx);

	m_apIcon.push_back(pIcon);
}

//=====================================================
// 敵の削除
//=====================================================
void CUIEnemy::DeleteEnemy(int nType)
{
	if (m_apIcon.empty())
		return;
	
	for (int i = 0; i < (int)m_apIcon.size(); i++)
	{
		CIcon *pIcon = m_apIcon[i];

		if (nType == pIcon->GetType())
		{
			pIcon->StartFall();	// 落下を開始させる

			m_apIcon.erase(m_apIcon.begin() + i);

			// ゲームの敵の最大数減少
			CGame *pGame = CGame::GetInstance();

			if (pGame != nullptr)
				pGame->DecreaseNumEnemy();

			break;
		}
	}

}

//******************************************************************
// アイコンの処理
//******************************************************************
//=====================================================
// 生成
//=====================================================
CIcon *CIcon::Create(int nType)
{
	CIcon *pIcon = new CIcon;

	if (pIcon == nullptr)
		return nullptr;

	pIcon->m_nType = nType;
	pIcon->Init();

	return pIcon;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CIcon::Init(void)
{
	// 継承クラスの初期化
	CUI::Init();

	// サイズ設定
	SetSize(SIZE_INIT_ICON.x, SIZE_INIT_ICON.y);

	return S_OK;
}

//=====================================================
// 更新
//=====================================================
void CIcon::Update(void)
{
	// 継承クラスの更新
	CUI::Update();

	if (m_state == E_State::STATE_FALL)
		UpdateFall();	// 落下時の更新
}

//=====================================================
// 落下し始める処理
//=====================================================
void CIcon::StartFall(void)
{
	// 落下状態にする
	m_state = E_State::STATE_FALL;

	// 上方向に移動量を足す
	m_move = MOVE_STARTFALL_ICON;
}

//=====================================================
// 落下時の更新
//=====================================================
void CIcon::UpdateFall(void)
{
	m_move.y += GRAVITY_ICON;

	// 位置の加算
	Translate(m_move);
	SetVtx();

	// 画面外に出たら終了
	D3DXVECTOR3 pos = GetPosition();

	if (pos.y > 1.0f + SIZE_INIT_ICON.y * 2)
		Uninit();
}