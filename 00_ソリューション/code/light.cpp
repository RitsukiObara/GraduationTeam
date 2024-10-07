//*****************************************************
//
// ライトの処理[light.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_CHANGE_COL = 0.1f;	// 色を変える速度
const D3DXCOLOR INTIIAL_COLOR[MAX_LIGHT] =
{// 初期の色
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f),
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f),
	D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f)
};
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CLight::s_nNumAll = 0;	// 総数
vector<CLight*> CLight::s_aLight;	// ライトの格納用配列

//====================================================
// コンストラク
//====================================================
CLight::CLight()
{
	m_nID = s_nNumAll;
	s_nNumAll++;
	s_aLight.push_back(this);
}

//====================================================
// デストラク
//====================================================
CLight::~CLight()
{
	s_nNumAll--;
}

//====================================================
// 生成
//====================================================
CLight *CLight::Create(void)
{
	CLight *pLight = nullptr;

	pLight = new CLight;

	if (pLight == nullptr)
		return nullptr;

	pLight->Init();

	return pLight;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	//ライトの種類設定
	m_light.Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光の設定
	m_light.Diffuse = INTIIAL_COLOR[0];

	//ライトの方向設定
	D3DXVECTOR3 vecDir = D3DXVECTOR3(-1.4f, 0.24f, -2.21f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
	m_light.Direction = vecDir;

	//ライト設定
	pDevice->SetLight(m_nID, &m_light);

	//ライト有効化
	pDevice->LightEnable(m_nID, TRUE);
	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CLight::Uninit(void)
{
	Release();
}

//====================================================
// 更新処理
//====================================================
void CLight::Update(void)
{

}

//====================================================
// ライト情報設定
//====================================================
void CLight::SetLightInfo(D3DLIGHT9 light)
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	m_light = light;

	pDevice->SetLight(m_nID, &m_light);
}

//====================================================
// ライトの有効化切り替え
//====================================================
void CLight::EnableLight(bool bFrag)
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// ライト有効化
	pDevice->LightEnable(m_nID, bFrag);
}

//====================================================
// 全ライトの解放
//====================================================
void CLight::ReleaseAll(void)
{
	s_aLight.clear();
}