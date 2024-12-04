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
#include "player.h"

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
const D3DXCOLOR COL_INIT = { 1.0f,1.0f,1.0f,1.0f };	// 初期色
const D3DXCOLOR COL_DEST = { 0.7f,0.7f,0.7f,1.0f };	// 目標色
const D3DXCOLOR COL_DIFF = COL_DEST - COL_INIT;		// 差分色
const float TIME_FADE_ICON = 2.0f;					// アイコンが暗くなる時間
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
CUIPlayer::CUIPlayer(int nPriority) : CObject(nPriority), m_state(E_State::STATE_NONE), m_fTimerScatter(0.0f)
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

		S_Icon *pIcon = new S_Icon;

		if (pIcon == nullptr)
			continue;

		pIcon->pPolygon = CUI::Create();

		if (pIcon->pPolygon == nullptr)
			continue;

		// UIの初期設定
		pIcon->pPolygon->SetSize(icon::WIDTH, icon::HEIGHT);
		D3DXVECTOR3 pos = icon::POS_INIT;
		pos.x += icon::WIDTH * i * 2;
		pIcon->pPolygon->SetPosition(pos);
		pIcon->pPolygon->SetVtx();

		int nIdxTexture = Texture::GetIdx(&icon::PATH_TEX[i][0]);
		pIcon->pPolygon->SetIdxTexture(nIdxTexture);

		m_aIcon.push_back(pIcon);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUIPlayer::Uninit(void)
{
	for (int i = 0; i < (int)m_aIcon.size(); i++)
	{
		if (m_aIcon[i]->pPolygon != nullptr)
		{
			m_aIcon[i]->pPolygon->Uninit();
			m_aIcon[i]->pPolygon = nullptr;
		}

		delete m_aIcon[i];
	}

	m_aIcon.clear();

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

	// アイコンのフェード
	UpdateFadeIcon();
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

	for (int i = 0; i < (int)m_aIcon.size(); i++)
	{
		if (m_aIcon[i]->pPolygon == nullptr)
			continue;

		D3DXVECTOR3 vecDiff = icon::POS_DEST[i] - icon::POS_INIT;

		D3DXVECTOR3 pos = icon::POS_INIT + vecDiff * fRate;

		m_aIcon[i]->pPolygon->SetPosition(pos);
		m_aIcon[i]->pPolygon->SetVtx();
	}
}

//=====================================================
// アイコンフェード
//=====================================================
void CUIPlayer::UpdateFadeIcon(void)
{
	//------------------------------
	// 死んだプレイヤーの検出
	//------------------------------
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (CPlayer *pPlayer : apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		CPlayer::E_State state = pPlayer->GetState();

		if (state == CPlayer::E_State::STATE_DEATH)
		{
			int nIDPlayer = pPlayer->GetID();

			m_aIcon[nIDPlayer]->bFade = true;
		}
	}

	//------------------------------
	// アイコンのフェード
	//------------------------------
	for (S_Icon *pIcon : m_aIcon)
	{
		if (!pIcon->bFade)
			continue;

		pIcon->fTimerFade += CManager::GetDeltaTime();

		float fTime = pIcon->fTimerFade / icon::TIME_FADE_ICON;
		float fRate = easing::EaseOutExpo(fTime);
		universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

		D3DXCOLOR col = icon::COL_INIT + icon::COL_DIFF * fRate;

		pIcon->pPolygon->SetCol(col);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CUIPlayer::Draw(void)
{

}