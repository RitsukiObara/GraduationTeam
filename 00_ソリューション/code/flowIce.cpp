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
#include "sound.h"
#include "player.h"
#include "camera.h"
#include "inputjoypad.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_DELETE = 20.0f;	// 氷が消えるまでの時間

const float POW_CAMERAQUAKE_DEFAULT = 0.1f;		// カメラの揺れのデフォルト値
const int FRAME_CAMERAQUAKE_DEFAULT = 30;		// カメラの揺れのデフォルトフレーム数

const float POW_VIB_CHAIN = 0.6f;	// 連結時のコントローラー揺れ強さ
const int TIME_VIB_CHAIN = 40;		// 連結時のコントローラー揺れ長さ
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
vector<CFlowIce*> CFlowIce::s_vector;	// 格納用の配列

//=====================================================
// コンストラクタ
//=====================================================
CFlowIce::CFlowIce(int nPriority) : CObject(nPriority), m_fTimerDelete(0.0f), m_bInScrnAllIce(false)
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
	CFlowIce *pFlowice = nullptr;

	pFlowice = new CFlowIce;

	if (pFlowice != nullptr)
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

	// 他の流氷との判定
	CollideOtherFlowIce();

	if (!m_bInScrnAllIce)
		CheckInAllIce();	// 全ての氷が映ったかの判定
	else
		CheckDelete();	// 削除確認

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 全ての氷が画面に映ったかのチェック
//=====================================================
void CFlowIce::CheckInAllIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;

		// 画面内判定
		D3DXVECTOR3 pos = m_apIce[i]->GetPosition();
		if (universal::IsInScreen(pos, nullptr))
		{// どれか一つでも画面内にあればフラグを立てる
			m_bInScrnAllIce = true;
		}
	}
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
// 他の流氷との判定
//=====================================================
void CFlowIce::CollideOtherFlowIce(void)
{
	
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

	Sound::Play(CSound::LABEL_SE_ICE_CHAIN);

	// カメラを揺らす
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
		pCamera->SetQuake(POW_CAMERAQUAKE_DEFAULT, POW_CAMERAQUAKE_DEFAULT, FRAME_CAMERAQUAKE_DEFAULT);

	// コントローラーを振動させる
	CInputJoypad* pInputJoypad = CInputJoypad::GetInstance();

	if (pInputJoypad == nullptr)
		return;

	pInputJoypad->Vibration(POW_VIB_CHAIN, TIME_VIB_CHAIN);
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

	// プレイヤーの取得
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;
		
		for (CPlayer *pPlayer : apPlayer)
		{// プレイヤーが上に乗ってたらHitする
			D3DXVECTOR3 pos = pPlayer->GetPosition();
			D3DXVECTOR3 posIce = m_apIce[i]->GetPosition();

			if (universal::DistCmpFlat(posIce, pos, Grid::SIZE, nullptr))
				pPlayer->Hit(0.0f);
		}

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