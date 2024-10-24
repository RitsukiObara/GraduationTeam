//*****************************************************
//
// 流氷の処理[flowIce.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "iceSpeedManager.h"
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
CIceSpeedManager::CIceSpeedManager(int nPriority) : CObject(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CIceSpeedManager::~CIceSpeedManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CIceSpeedManager* CIceSpeedManager::Create(void)
{
	CIceSpeedManager* pFlowice = nullptr;

	pFlowice = new CIceSpeedManager;

	if (pFlowice == nullptr)
		return nullptr;

	pFlowice->Init();

	return pFlowice;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIceSpeedManager::Init(void)
{
	// 氷の生成
	//CreateIce();

	return S_OK;
}

//=====================================================
// 氷の生成
//=====================================================
void CIceSpeedManager::CreateIce(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

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
		CIce* pIce = pIceManager->CreateIce(aV[i], aH[i]);
		pIce->ChangeState(new CIceStateFlow);
		m_apIce.push_back(pIce);
	}
}

//=====================================================
// 氷を配列に追加
//=====================================================
void CIceSpeedManager::AddIceToArray(CIce* pIce)
{
	m_apIce.push_back(pIce);
}

//=====================================================
// 終了処理
//=====================================================
void CIceSpeedManager::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CIceSpeedManager::Update(void)
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
void CIceSpeedManager::CheckSomeIceStop(void)
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
void CIceSpeedManager::StopAllIce(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

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
void CIceSpeedManager::Debug(void)
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
void CIceSpeedManager::Draw(void)
{

}