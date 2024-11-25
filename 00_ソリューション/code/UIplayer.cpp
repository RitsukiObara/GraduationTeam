//*****************************************************
//
// プリヤーUIの処理[UIplayer.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "UIplayer.h"
#include "gameManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
namespace icon
{
const float WIDTH = 0.05f;							// 幅
const float HEIGHT = 0.086f;						// 高さ
const D3DXVECTOR3 POS_INIT = { 0.5f,0.3f,0.0f };	// 初期位置
}
}

//=====================================================
// コンストラクタ
//=====================================================
CUIPlayer::CUIPlayer(int nPriority) : CObject(nPriority), m_apIconPlayer()
{

}

//=====================================================
// デストラクタ
//=====================================================
CUIPlayer::~CUIPlayer()
{

}

//=====================================================
// 生成処理
//=====================================================
CUIPlayer *CUIPlayer::Create()
{
	CUIPlayer *pUIPlayer = new CUIPlayer;

	if (pUIPlayer != nullptr)
		pUIPlayer->Init();

	return pUIPlayer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CUIPlayer::Init(void)
{
	// プレイヤーエントリーフラグ取得
	CGame::E_GameMode mode;
	vector<bool> abFrag;

	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		if (!abFrag[i])	// フラグが立っていなかったら通らない
			continue;

		m_apIconPlayer[i] = CUI::Create();

		if (m_apIconPlayer[i] == nullptr)
			continue;

		// UIの初期設定
		m_apIconPlayer[i]->SetSize(icon::WIDTH, icon::HEIGHT);
		m_apIconPlayer[i]->SetPosition(icon::POS_INIT);
		m_apIconPlayer[i]->SetVtx();
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUIPlayer::Uninit(void)
{
	Object::DeleteObject((CObject**)m_apIconPlayer, NUM_PLAYER);

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CUIPlayer::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CUIPlayer::Draw(void)
{

}