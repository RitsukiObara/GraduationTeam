//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "seals.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionSeals.txt";	// ボディのパス
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CSeals::CSeals(int nPriority) : CEnemy()
{
	m_State = STATE_STOP;
}

//=====================================================
// デストラクタ
//=====================================================
CSeals::~CSeals()
{

}

//=====================================================
// 生成処理
//=====================================================
CSeals* CSeals::Create(void)
{
	CSeals* pSeals = nullptr;

	pSeals = new CSeals;

	if (pSeals != nullptr)
	{
		pSeals->Init();
	}

	return pSeals;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSeals::Init(void)
{
	// 読込
	Load((char*)&PATH_BODY[0]);

	if (FAILED(CEnemy::Init()))
	{ 
		return E_FAIL;
	}

	// ポーズ初期化
	InitPose(0);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSeals::Uninit(void)
{
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSeals::Update(void)
{
	switch (m_State)
	{
	case STATE_STOP:

		 break;

	case STATE_MOVE:

		 break;

	case STATE_ATTACK:

		 break;

	case STATE_DRIFT:

		break;

	default:
		assert(false);
		break;
	}

	//CIceManager* pIceManager = CIceManager::GetInstance();

	/*if (pIceManager != nullptr)
	{
		CIce* pIce = pIceManager->GetGridObject(&m_nGridV, &m_nGridH);
		if (pIce != nullptr)
		{
			D3DXVECTOR3 posObject = pIce->GetPosition();
			SetPosition(posObject);
		}
	}*/

	CEnemy::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CSeals::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}