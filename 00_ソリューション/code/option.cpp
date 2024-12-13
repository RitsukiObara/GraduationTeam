//*****************************************************
//
// オプション処理[option.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "option.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	
}

//=====================================================
// コンストラクタ
//=====================================================
COption::COption()
{
	m_pOptionText = nullptr;
	for (int cnt = 0; cnt < sizeof(m_aSoundUIObj) / sizeof(SoundUIObject); cnt++)
	{
		m_aSoundUIObj[cnt].text = nullptr;
		m_aSoundUIObj[cnt].bar = nullptr;
		m_aSoundUIObj[cnt].point = nullptr;
		m_aSoundUIObj[cnt].penguin = nullptr;
	}
	m_pBibeText = nullptr;
	for (int cnt = 0; cnt < sizeof(m_apBibePenguin) / sizeof(CUI*); cnt++) { m_apBibePenguin[cnt] = nullptr; }
	m_pButtonUI = nullptr;

	m_optionParam = PARAM_BGM;
	m_fBGMVolume = 0.0f;
	m_fSEVolume = 0.0f;
	m_Bibe = BIBE_ON;
}

//=====================================================
// デストラクタ
//=====================================================
COption::~COption()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT COption::Init(void)
{
	CScene::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void COption::Uninit(void)
{
	// オブジェクト全破棄
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void COption::Update(void)
{
	// シーンの更新
	CScene::Update();
}

//=====================================================
// 描画処理
//=====================================================
void COption::Draw(void)
{
	CScene::Draw();
}
