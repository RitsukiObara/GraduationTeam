//*****************************************************
//
// 流氷ファクトリー[flowIceFactory.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "flowIceFactory.h"
#include "iceManager.h"
#include "ice.h"
#include "flowIce.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_CREATE_FLOWICE = 5.0f;	// 流氷を作る時間
const int NUM_ICE = 4;	// 適当な初期アイス数
}

//=====================================================
// コンストラクタ
//=====================================================
CFlowIceFct::CFlowIceFct(int nPriority) : CObject(nPriority), m_fTimerCreateFlowIce(0.0f)
{

}

//=====================================================
// デストラクタ
//=====================================================
CFlowIceFct::~CFlowIceFct()
{

}

//=====================================================
// 生成処理
//=====================================================
CFlowIceFct *CFlowIceFct::Create(void)
{
	CFlowIceFct *pFct = nullptr;

	pFct = new CFlowIceFct;

	if (pFct != nullptr)
	{
		pFct->Init();
	}

	return pFct;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CFlowIceFct::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFlowIceFct::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CFlowIceFct::Update(void)
{
	// 経過時間加算
	m_fTimerCreateFlowIce += CManager::GetDeltaTime();

	// 一定時間で流氷を流す
	if (m_fTimerCreateFlowIce > TIME_CREATE_FLOWICE)
	{
		// 流氷の生成
		CreateFlowIce();
	}
}

//=====================================================
// 流氷の生成
//=====================================================
void CFlowIceFct::CreateFlowIce(void)
{
	// 流氷システムの生成
	CFlowIce *pFlowIce = CFlowIce::Create();

	if (pFlowIce == nullptr)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// 氷を生成し、システムに追加
	int nNumGridV = pIceMgr->GetNumGridV();
	int nNumGridH = pIceMgr->GetNumGridH();

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	int aV[NUM_ICE] =
	{// 縦のグリッド番号の配列
		3,4,5,6
	};
	int aH[NUM_ICE] =
	{// 横のグリッド番号の配列
		9,9,9,9
	};

	for (int i = 0; i < NUM_ICE; i++)
	{
		CIce *pIce = pIceManager->CreateIce(aV[i], aH[i]);
		pIce->ChangeState(new CIceStateFlow);
		pFlowIce->AddIceToArray(pIce);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CFlowIceFct::Draw(void)
{

}