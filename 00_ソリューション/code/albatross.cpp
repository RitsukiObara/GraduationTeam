//*****************************************************
//
// アホウドリの処理[albatross.cpp]
// Author:若木一真
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "albatross.h"
#include "particle.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionAlbatross.txt";	// アホウドリのパス

	const float HEIGHT_APPER = 400.0f;	// 出現時の高さ
	const float WIDTH_APPER = 340.0f;	// 出現時の横のずれ
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CAlbatross::CAlbatross(int nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CAlbatross::~CAlbatross()
{

}

//=====================================================
// 生成処理
//=====================================================
CAlbatross* CAlbatross::Create(CIceManager::E_Stream dir)
{
	CAlbatross* pAlbatross = nullptr;

	pAlbatross = new CAlbatross;

	if (pAlbatross != nullptr)
	{
		CIceManager::E_Stream OceanFlow = CIceManager::GetInstance()->GetDirStreamNext();

		pAlbatross->Init();

		pAlbatross->Stream(OceanFlow);
	}

	return pAlbatross;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CAlbatross::Init(void)
{
	// 読込
	Load((char*)&PATH_BODY[0]);

	// モーションを初期設定
	SetMotion(E_Motion::MOTION_FLY);

	// ポーズ初期化
	InitPose(0);

	// 継承クラスの初期化
	CMotion::Init();

	return S_OK;
}

//=====================================================
// アホウドリの向きを決める処理
//=====================================================
void CAlbatross::Stream(CIceManager::E_Stream dir)
{
	D3DXVECTOR3 rot = CAlbatross::GetRotation();

	switch (dir)
	{
	case CIceManager::STREAM_UP:	// 上

		rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

		break;
	case CIceManager::STREAM_RIGHT:	// 右

		rot = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);

		break;
	case CIceManager::STREAM_DOWN:	// 下

		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		break;
	case CIceManager::STREAM_LEFT:	// 左

		rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

		break;
	default:

		assert(false);
		break;
	}

	CAlbatross::SetRotation(rot);
}

//=====================================================
// 終了処理
//=====================================================
void CAlbatross::Uninit(void)
{

}

//=====================================================
// 更新処理
//=====================================================
void CAlbatross::Update(void)
{
	D3DXVECTOR3 pos = CAlbatross::GetPosition();
	D3DXVECTOR3 rot = CAlbatross::GetRotation();

	m_Move = D3DXVECTOR3(sinf(rot.y + D3DX_PI), 0.5f, cosf(rot.y + D3DX_PI));

	pos += m_Move;

	CAlbatross::SetPosition(pos);

	//// モーションの管理
	//ManageMotion();

	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	pDebugProc->Print("\nアホウドリ==========================");
	pDebugProc->Print("\n位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
}

//=====================================================
// モーションの管理
//=====================================================
void CAlbatross::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (nMotion == E_Motion::MOTION_FLY)
	{// ジャンプ開始モーション
		if (bFinish)	// 終わり次第滞空モーションへ移行
			SetMotion(E_Motion::MOTION_FLY);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CAlbatross::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}