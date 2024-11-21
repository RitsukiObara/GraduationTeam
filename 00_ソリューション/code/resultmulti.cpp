//*****************************************************
//
// マルチモードのリザルト処理[resultmulti.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultmulti.h"
#include "object.h"
#include "inputManager.h"
#include "manager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "CameraState.h"
#include "renderer.h"
#include "sound.h"
#include "polygon3D.h"
#include "objectX.h"
#include "skybox.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "particle.h"
#include "orbit.h"
#include "debugproc.h"
#include "UI.h"
#include "gameManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	namespace Penguin
	{
		const D3DXVECTOR3 WINNER_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 勝者の位置
		const vector<D3DXVECTOR3> LOSER_POS =
		{
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		};
	}
	namespace manual
	{
		const char* PATH = "data\\TEXTURE\\UI\\tutorial00.jpg";	// パス
		const float WIDTH		= 0.5f;	// 幅
		const float HEIGHT		= 0.5f;	// 高さ
		const D3DXVECTOR3 POS	= D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// 位置
	}
}

//=====================================================
// コンストラクタ
//=====================================================
CResultMulti::CResultMulti()
{

}

//=====================================================
// デストラクタ
//=====================================================
CResultMulti::~CResultMulti()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CResultMulti::Init(void)
{
	// シーンの初期化
	if (FAILED(CScene::Init()))
		return E_FAIL;

	// カメラ
	Camera::ChangeState(new CCameraStateSelectMode);

	// 勝者UI生成
	//m_pManual = CUI::Create();
	//if (m_pManual != nullptr)
	//{
	//	// 説明の設定
	//	m_pManual->SetIdxTexture(CTexture::GetInstance()->Regist(manual::PATH));	// テクスチャ割当
	//	m_pManual->SetPosition(manual::POS);				// 位置
	//	m_pManual->SetSize(manual::WIDTH, manual::HEIGHT);	// 大きさ
	//	m_pManual->SetVtx();	// 頂点反映
	//}

	// メッシュフィールド
	CMeshField* pMeshField = CMeshField::Create();
	pMeshField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMeshField->SetIdxTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\MATERIAL\\ice000.jpg"));
	pMeshField->SetDivTex(128);

	// かまくら
	/*CObjectX *pIgloo = CObjectX::Create();
	pIgloo->BindModel(CModel::Load("data\\MODEL\\object\\Snowdome.x"));
	pIgloo->SetPosition(D3DXVECTOR3(800.0f, -10.0f, 600.0f));*/

	// BGMの再生
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	//// 遊ぶペンギン
	//CResultMultiPenguin* pPenguin = nullptr;
	//// 右中心
	//pPenguin = CResultMultiPenguin::Create(new CResultMultiPenguinState_Stand);
	//pPenguin->SetPosition(D3DXVECTOR3(400.0f, 10.0f, -1000.0f));
	//// あの辺1
	//pPenguin = CResultMultiPenguin::Create(new CResultMultiPenguinState_Stand);
	//pPenguin->SetPosition(D3DXVECTOR3(-500.0f, 10.0f, 800.0f));
	//// あの辺2
	//pPenguin = CResultMultiPenguin::Create(new CResultMultiPenguinState_Stand);
	//pPenguin->SetPosition(D3DXVECTOR3(2200.0f, 10.0f, 800.0f));
	//// かまくら
	//pPenguin = CResultMultiPenguin::Create(new CResultMultiPenguinState_Stand);
	//pPenguin->SetPosition(D3DXVECTOR3(950.0f, 10.0f, 700.0f));

	// 入力マネージャー生成
	CInputManager::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CResultMulti::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pWinner);

	// オブジェクト全破棄
	CObject::ReleaseAll();

	// シーンの終了
	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CResultMulti::Update(void)
{
	CInputManager *pInputMgr = CInputManager::GetInstance();
	assert(pInputMgr != nullptr);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	// 完了
	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
	{
		// フェード中の場合抜ける
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr)
			assert(false);
		if (pFade->GetState() != CFade::FADE_NONE)
			return;

		// タイトル遷移
		pFade->SetFade(CScene::MODE_TITLE);

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_DECISION);
	}

	// シーンの更新
	CScene::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CResultMulti::Draw(void)
{
	// シーンの描画
	CScene::Draw();
}

