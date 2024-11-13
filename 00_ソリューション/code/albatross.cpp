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
#include "iceManager.h"
#include "particle.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionAlbatross.txt";	// アホウドリのパス

	const float HEIGHT_APPER = -400.0f;	// 出現時の高さ
	const float WIDTH_APPER = -340.0f;	// 出現時の横のずれ
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
CAlbatross* CAlbatross::Create(void)
{
	CAlbatross* pAlbatross = nullptr;

	pAlbatross = new CAlbatross;

	if (pAlbatross != nullptr)
	{
		pAlbatross->Init();
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

	return S_OK;
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
	CIceManager* pIce = CIceManager::GetInstance();

	m_DirStream = pIce->GetDirStream();	// 現在の海流の方向を取得
	m_DirStreamNext = pIce->GetDirStreamNext();	// 次の海流の方向を取得

	if (m_DirStream != m_DirStreamNext)	// 現在と次の海流が違うときアホウドリを次の海流の向きに飛ばす
	{
		if (m_DirStreamNext == CIceManager::STREAM_UP)	// 上
		{
			m_FlyDirection = FLYDIRECTION_UP;
		}

		if (m_DirStreamNext == CIceManager::STREAM_RIGHT)	// 右
		{
			m_FlyDirection = FLYDIRECTION_RIGHT;
		}

		if (m_DirStreamNext == CIceManager::STREAM_DOWN)	// 下
		{
			m_FlyDirection = FLYDIRECTION_DOWN;
		}

		if (m_DirStreamNext == CIceManager::STREAM_LEFT)	// 左
		{
			m_FlyDirection = FLYDIRECTION_LEFT;
		}
	}

	if (m_FlyDirection == FLYDIRECTION_UP)
	{

	}

	if (m_FlyDirection == FLYDIRECTION_RIGHT)
	{

	}

	if (m_FlyDirection == FLYDIRECTION_DOWN)
	{

	}

	if (m_FlyDirection == FLYDIRECTION_LEFT)
	{

	}

	// モーションの管理
	ManageMotion();
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

}