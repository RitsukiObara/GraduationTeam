//*****************************************************
//
// マルチ用の硬い氷の処理[iceHardMulti.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "iceHardMulti.h"
#include "player.h"
#include "iceManager.h"
#include "manager.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "MyEffekseer.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_SINK = 10.0f;						// 沈むまでの時間
const float SPEED_REPAIR = 0.3f;					// 回復の速度(1.0fで進行と同じ速度)
const D3DXCOLOR COL_INIT = { 1.0f,1.0f,1.0f,1.0f };	// 初期の色
const float SIZE_COLLIDE = Grid::SIZE * 0.5f;		// 判定のサイズ

const float POW_VIB_SINK = 0.7f;	// 沈みそうな時の振動強さ
const int TIME_VIB_SINK = 10;		// 沈みそうな時の振動時間
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// コンストラクタ
//=====================================================
CIceHardMulti::CIceHardMulti(int nPriority) : CIceHard(nPriority), m_fTimerSink(0.0f)
{

}

//=====================================================
// デストラクタ
//=====================================================
CIceHardMulti::~CIceHardMulti()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIceHardMulti::Init(void)
{
	CIceHard::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CIceHardMulti::Uninit(void)
{
	CIceHard::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CIceHardMulti::Update(void)
{
	// 継承クラスの更新
	CIceHard::Update();

	// プレイヤーが乗ってる判定
	IsOnPlayer();

	// 状態ごとの更新
	UpdateState();
}

//=====================================================
// プレイヤーが乗ってる判定
//=====================================================
void CIceHardMulti::IsOnPlayer(void)
{
	// プレイヤーの取得
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (CPlayer *pPlayer : apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		if (universal::DistCmpFlat(pos, posPlayer, SIZE_COLLIDE, nullptr))
		{// 一定距離以内にいたらカウンターを加算
			m_fTimerSink += CManager::GetDeltaTime();

			// コントローラーを振動させる
			float fRate = m_fTimerSink / TIME_SINK;
			pPlayer->VibJoypad(POW_VIB_SINK * fRate, TIME_VIB_SINK);

			break;
		}
		else
			m_fTimerSink -= CManager::GetDeltaTime() * SPEED_REPAIR;
	}
}

//=====================================================
// 状態ごとの更新
//=====================================================
void CIceHardMulti::UpdateState(void)
{
	if (m_fTimerSink > TIME_SINK)
	{// 一定時間乗ってたら削除
		Uninit();
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_ICEBREAK, GetPosition());
		return;
	}

	// 値の補正
	universal::LimitValuefloat(&m_fTimerSink, TIME_SINK, 0.0f);

	// 色の設定
	D3DXCOLOR col = COL_INIT;

	// 割合の計算
	float fRate = m_fTimerSink / TIME_SINK;

	// 赤以外の色を割合分抜いていく
	col.g -= fRate;
	col.b -= fRate;

	// 各メッシュに色を設定
	CMeshCylinder *pCylinder = GetMeshCyliner();
	CFan3D *pFan = GetFan3D();

	if (pCylinder == nullptr || pFan == nullptr)
		return;

	pCylinder->SetCol(col);
	pFan->SetCol(col);
}

//=====================================================
// 描画処理
//=====================================================
void CIceHardMulti::Draw(void)
{
	CIceHard::Draw();
}