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
#include "manager.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
namespace icon
{
const float WIDTH = 0.05f;							// 幅
const float HEIGHT = 0.086f;						// 高さ
const D3DXVECTOR3 POS_INIT = { 0.35f,0.3f,0.0f };	// 初期位置
const D3DXVECTOR3 POS_DEST[] =						// 目標位置
{
	{ WIDTH,HEIGHT,0.0f },
	{ 1.0f - WIDTH,HEIGHT,0.0f },
	{ WIDTH,1.0f - HEIGHT,0.0f },
	{ 1.0f - WIDTH,1.0f - HEIGHT,0.0f },
};
const float TIME_MOVE = 3.0f;	// 移動にかかる時間
const string PATH_TEX[] =		// テクスチャパス
{
	"data\\TEXTURE\\UI\\playericon_00.png",
	"data\\TEXTURE\\UI\\playericon_01.png",
	"data\\TEXTURE\\UI\\playericon_02.png",
	"data\\TEXTURE\\UI\\playericon_03.png",
};
}
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CUIPlayer::FuncUpdateState CUIPlayer::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,					// 何でもない状態
	&CUIPlayer::UpdateScatter,	// 散らばる状態
};

//=====================================================
// コンストラクタ
//=====================================================
CUIPlayer::CUIPlayer(int nPriority) : CObject(nPriority), m_apIconPlayer(), m_state(E_State::STATE_NONE), m_fTimerScatter(0.0f)
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
		D3DXVECTOR3 pos = icon::POS_INIT;
		pos.x += icon::WIDTH * i * 2;
		m_apIconPlayer[i]->SetPosition(pos);
		m_apIconPlayer[i]->SetVtx();

		int nIdxTexture = Texture::GetIdx(&icon::PATH_TEX[i][0]);
		m_apIconPlayer[i]->SetIdxTexture(nIdxTexture);
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
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合
		// 各状態ごとの更新
		(this->*(s_aFuncUpdateState[m_state]))();
	}
}

//=====================================================
// 散らばり始める処理
//=====================================================
void CUIPlayer::StartScatter(void)
{
	m_state = E_State::STATE_SCATTER;
}

//=====================================================
// 散らばる更新
//=====================================================
void CUIPlayer::UpdateScatter(void)
{
	if (m_fTimerScatter < icon::TIME_MOVE)
		m_fTimerScatter += CManager::GetDeltaTime();

	float fRate = easing::EaseOutExpo(m_fTimerScatter);

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if (m_apIconPlayer[i] == nullptr)
			continue;

		D3DXVECTOR3 vecDiff = icon::POS_DEST[i] - icon::POS_INIT;

		D3DXVECTOR3 pos = icon::POS_INIT + vecDiff * fRate;

		m_apIconPlayer[i]->SetPosition(pos);
		m_apIconPlayer[i]->SetVtx();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CUIPlayer::Draw(void)
{

}