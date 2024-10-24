//*****************************************************
//
// 流氷の処理[flowIce.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "flowIce.h"
#include "ice.h"
#include "iceManager.h"
#include "effect3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_ICE = 4;	// 氷の数
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// コンストラクタ
//=====================================================
CFlowIce::CFlowIce(int nPriority) : CObject(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CFlowIce::~CFlowIce()
{

}

//=====================================================
// 生成処理
//=====================================================
CFlowIce *CFlowIce::Create(void)
{
	CFlowIce* pFlowice = nullptr;

	pFlowice = new CFlowIce;

	if (pFlowice == nullptr)
		return nullptr;

	pFlowice->Init();

	return pFlowice;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CFlowIce::Init(void)
{
	// 氷の生成
	//CreateIce();

	return S_OK;
}

//=====================================================
// 氷の生成
//=====================================================
void CFlowIce::CreateIce(void)
{
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
		m_apIce.push_back(pIce);
	}
}

//=====================================================
// 氷を配列に追加
//=====================================================
void CFlowIce::AddIceToArray(CIce *pIce)
{
	m_apIce.push_back(pIce);
}

//=====================================================
// 終了処理
//=====================================================
void CFlowIce::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CFlowIce::Update(void)
{
	// どれかの氷が止まっていないかのチェック
	CheckSomeIceStop();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// どれかの氷が止まっていないかのチェック
//=====================================================
void CFlowIce::CheckSomeIceStop(void)
{
	for (auto it : m_apIce)
	{
		if (it->IsStop())
		{// 止まっているなら全ての氷を止める
			StopAllIce();
			Uninit();	// 自身の破棄
			break;
		}
	}
}

//=====================================================
// 全ての氷を止める
//=====================================================
void CFlowIce::StopAllIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	for (auto it : m_apIce)
	{
		it->ChangeState(new CIceStaeteNormal);

		pIceManager->AddIce(it, it->GetPosition());
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CFlowIce::Debug(void)
{
	for (auto it : m_apIce)
	{
		D3DXVECTOR3 posIce = it->GetPosition();

		CEffect3D::Create(posIce, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=====================================================
// 描画処理
//=====================================================
void CFlowIce::Draw(void)
{

}