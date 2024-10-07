//*****************************************************
//
// ゲージの処理[gauge.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "gauge.h"
#include "UI.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR2 SIZE_GAUGE_INITIAL = { 0.07f, 0.05f };	// ゲージの初期サイズ
const D3DXVECTOR2 SIZE_FRAME_INITIAL = { SIZE_GAUGE_INITIAL.x * 1.0f, SIZE_GAUGE_INITIAL.y * 1.0f };	// フレームの初期サイズ
const D3DXVECTOR3 POS_INITIAL = { SIZE_FRAME_INITIAL.x,SIZE_FRAME_INITIAL.y,0.0f };	// 初期位置
const float TIMELIMIT_TRANSGAUGE_INITIAL = 2.0f;	// ゲージの変動にかかる時間の初期値
}

//=====================================================
// コンストラクタ
//=====================================================
CGauge::CGauge() : m_fParam(0.0f), m_fParamMax(0.0f), m_fTimerTransGauge(0.0f), m_fTimeLimitTransGauge(0.0f), m_fWidthOld(0.0f)
{

}

//=====================================================
// デストラクタ
//=====================================================
CGauge::~CGauge()
{

}

//=====================================================
// 生成処理
//=====================================================
CGauge *CGauge::Create(float fParamMax)
{
	CGauge *pGauge = nullptr;

	pGauge = new CGauge;

	if (pGauge != nullptr)
	{
		pGauge->m_fParamMax = fParamMax;
		pGauge->Init();
	}

	return pGauge;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGauge::Init(void)
{
	// ゲージ本体の初期設定
	SetPosition(POS_INITIAL);
	m_fTimeLimitTransGauge = TIMELIMIT_TRANSGAUGE_INITIAL;

	// ポリゴンの生成
	CreatePolygon();

	return S_OK;
}

//=====================================================
// ポリゴンの生成
//=====================================================
void CGauge::CreatePolygon(void)
{
	m_aPolygon.resize(E_Polygon::POLYGON_MAX);

	m_sizeGauge = SIZE_GAUGE_INITIAL;

	D3DXVECTOR2 aSize[E_Polygon::POLYGON_MAX] = { SIZE_FRAME_INITIAL,SIZE_GAUGE_INITIAL };	// ポリゴンのサイズ
	string aPath[E_Polygon::POLYGON_MAX] = 
	{
		"data\\TEXTURE\\UI\\frame00.png",
		"data\\TEXTURE\\UI\\gauge00.png"
	};

	// ポリゴンの生成
	for (int i = 0; i < E_Polygon::POLYGON_MAX; i++)
	{
		m_aPolygon[i] = CUI::Create();

		if (m_aPolygon[i] == nullptr)
			continue;

		m_aPolygon[i]->SetSize(aSize[i].x, aSize[i].y);
		m_aPolygon[i]->SetPosition(POS_INITIAL);
		m_aPolygon[i]->SetVtx();

		int nIdx = Texture::GetIdx(&aPath[i][0]);
		m_aPolygon[i]->SetIdxTexture(nIdx);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CGauge::Uninit(void)
{
	for (auto it : m_aPolygon)
	{
		if (it != nullptr)
		{
			it->Uninit();
			it = nullptr;
		}
	}

	m_aPolygon.clear();

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CGauge::Update(void)
{
	// ゲージポリゴンの管理
	ManageGaugePolygon();
}

//=====================================================
// ゲージポリゴンの管理
//=====================================================
void CGauge::ManageGaugePolygon(void)
{
	if (m_aPolygon.empty())
		return;

	if (m_aPolygon[E_Polygon::POLYGON_GAUGE] == nullptr)
		return;

	// 最初はとりあえずベタに幅を設定
	float fRate = m_fParam / m_fParamMax;
	float fWidth = m_sizeGauge.x * fRate;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posGauge = pos;
	posGauge.x += fWidth;
	posGauge.x -= m_sizeGauge.x;

	// トランスフォームの設定
	m_aPolygon[E_Polygon::POLYGON_GAUGE]->SetSize(fWidth, m_sizeGauge.y);
	m_aPolygon[E_Polygon::POLYGON_GAUGE]->SetPosition(posGauge);
	m_aPolygon[E_Polygon::POLYGON_GAUGE]->SetVtx();

	// テクスチャ座標の設定
	m_aPolygon[E_Polygon::POLYGON_GAUGE]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(fRate, 1.0f));
}

//=====================================================
// 描画処理
//=====================================================
void CGauge::Draw(void)
{

}

//=====================================================
// 位置の設定
//=====================================================
void CGauge::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// フレームの位置合わせ
	if (m_aPolygon.empty())
		return;

	if (m_aPolygon[E_Polygon::POLYGON_FRAME] == nullptr)
		return;

	m_aPolygon[E_Polygon::POLYGON_FRAME]->SetPosition(pos);
	m_aPolygon[E_Polygon::POLYGON_FRAME]->SetVtx();
}

//=====================================================
// パラメーターの加算処理
//=====================================================
void CGauge::AddParam(float fValue)
{
	m_fParam += fValue;

	// ゲージが動くように設定
	if (m_aPolygon[E_Polygon::POLYGON_GAUGE] == nullptr)
		return;

	m_fWidthOld = m_aPolygon[E_Polygon::POLYGON_GAUGE]->GetWidth();
}