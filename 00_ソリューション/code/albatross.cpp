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
#include "ocean.h"
#include "manager.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionAlbatross.txt";				// アホウドリのパス
const std::string PATH_BODY_OBARA = "data\\MOTION\\motionAlbatrossObara.txt";	// 小原アホウドリのパス

const float HEIGHT_APPER = 400.0f;	// 出現時の高さ
const float WIDTH_APPER = 340.0f;	// 出現時の横のずれ
const float POS_X = 1750.0f;	// アホウドリの出現位置X
const float POS_Z = 1300.0f;	// アホウドリの出現位置Z
const float POS_Y = 500.0f;	// アホウドリの出現位置Y

const float PLAYSE_TIME = 1.2f;	// 鳴き声流す間隔
const float RATE_OBARA = 0.03f;	// 小原になる確率
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CAlbatross::CAlbatross(int nPriority) : m_Move(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_fPlaySETime(0.0f)
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
CAlbatross* CAlbatross::Create(COcean::E_Stream dir)
{
	CAlbatross* pAlbatross = nullptr;

	pAlbatross = new CAlbatross;

	if (pAlbatross != nullptr)
	{
		pAlbatross->Init();

		pAlbatross->Stream(dir);
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

	D3DXVECTOR3 pos = CAlbatross::GetPosition();

	switch (COcean::GetInstance()->GetNextDirStream())
	{
	case COcean::STREAM_UP:	// 上

		pos = D3DXVECTOR3(0.0f, POS_Y, -POS_Z);

		break;
	case COcean::STREAM_RIGHT:	// 右

		pos = D3DXVECTOR3(-POS_X, POS_Y, 0.0f);

		break;
	case COcean::STREAM_DOWN:	// 下

		pos = D3DXVECTOR3(0.0f, POS_Y, POS_Z);

		break;
	case COcean::STREAM_LEFT:	// 左

		pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);

		break;
	default:

		assert(false);
		break;
	}

	CAlbatross::SetPosition(pos);

	SetMotion(MOTION_FLY);

	// 頭を変える処理
	ChangeHead();

	return S_OK;
}

//=====================================================
// 頭を変える処理
//=====================================================
void CAlbatross::ChangeHead(void)
{
	float fRate = (float)rand() / (float)RAND_MAX;

	if(fRate < RATE_OBARA)
		ReLoadModel(&PATH_BODY_OBARA[0]);
}

//=====================================================
// アホウドリの向きを決める処理
//=====================================================
void CAlbatross::Stream(COcean::E_Stream dir)
{
	D3DXVECTOR3 rot = CAlbatross::GetRotation();

	switch (dir)
	{
	case COcean::STREAM_UP:	// 上

		rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

		break;
	case COcean::STREAM_RIGHT:	// 右

		rot = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);

		break;
	case COcean::STREAM_DOWN:	// 下

		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		break;
	case COcean::STREAM_LEFT:	// 左

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
	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CAlbatross::Update(void)
{
	D3DXVECTOR3 pos = CAlbatross::GetPosition();
	D3DXVECTOR3 rot = CAlbatross::GetRotation();

	m_Move = D3DXVECTOR3(sinf(rot.y + D3DX_PI), 0.0f, cosf(rot.y + D3DX_PI));	// 向いてる方向に移動する

	pos += m_Move * 6.0f;

	CAlbatross::SetPosition(pos);

	//// モーションの管理
	//ManageMotion();

	// アホウドリの画面外処理
	if (pos.x > 2000.0f || pos.x < -2000.0f ||
		pos.z > 2000.0f || pos.z < -2000.0f)
	{
		Uninit();
	}

	// モーション更新
	CMotion::Update();

	// SE再生処理
	CheckPlaySE();

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

//=====================================================
// 鳴き声流す処理
//=====================================================
void CAlbatross::CheckPlaySE(void)
{
	CManager* pManager = CManager::GetInstance();
	CSound* pSound = CSound::GetInstance();
	if (pManager == nullptr || pSound == nullptr)
		return;

	// カウント
	m_fPlaySETime += pManager->GetDeltaTime();
	if (m_fPlaySETime >= PLAYSE_TIME)
	{
		m_fPlaySETime -= PLAYSE_TIME;
		pSound->Play(CSound::LABEL_SE_ALBATROSS);	// 流す
	}
}