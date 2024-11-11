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
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_DELETE = 20.0f;	// 氷が消えるまでの時間
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
vector<CFlowIce*> CFlowIce::s_vector;	// 格納用の配列

//=====================================================
// コンストラクタ
//=====================================================
CFlowIce::CFlowIce(int nPriority) : CObject(nPriority), m_fTimerDelete(0.0f)
{
	s_vector.push_back(this);
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
	return S_OK;
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
	for (auto itr = s_vector.begin(); itr < s_vector.end(); itr++)
	{
		if (*itr == this)
		{
			s_vector.erase(itr);

			break;
		}
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CFlowIce::Update(void)
{
	// どれかの氷が止まっていないかのチェック
	CheckSomeIceStop();

	// 削除確認
	CheckDelete();

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
// 上にあるものを止める
//=====================================================
void CFlowIce::StopOnTopObject(void)
{

}

//=====================================================
// 全氷の削除
//=====================================================
void CFlowIce::DeleteAllIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;

		pIceManager->DeleteIce(m_apIce[i]);
		m_apIce[i]->Uninit();
	}

	m_apIce.clear();
}

//=====================================================
// 氷削除の確認
//=====================================================
void CFlowIce::CheckDelete(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	m_fTimerDelete += CManager::GetDeltaTime();

	if (m_fTimerDelete > TIME_DELETE)
	{
		for (int i = 0; i < (int)m_apIce.size(); i++)
		{
			if (m_apIce[i] == nullptr)
				continue;

			// 画面内判定
			D3DXVECTOR3 pos = m_apIce[i]->GetPosition();
			if (universal::IsInScreen(pos, nullptr))
				return;	// どれか一つでも画面内にあれば関数を終了
		}

		// ここまで通ったら氷を削除
		DeleteAllIce();
		Uninit();
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

		//CEffect3D::Create(posIce, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=====================================================
// 描画処理
//=====================================================
void CFlowIce::Draw(void)
{

}