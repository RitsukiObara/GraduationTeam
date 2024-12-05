//*****************************************************
//
// 背景氷の処理[BG_Ice.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "BG_Ice.h"
#include "ice.h"
#include "ocean.h"
#include "iceManager.h"
#include "MyEffekseer.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* MODEL[bgice::BGICE_MODEL] = { "data\\MODEL\\block\\ice_small001.x","data\\MODEL\\block\\ice_small002.x","data\\MODEL\\block\\ice_small003.x" };
const float MAX_HEIGHT = -300.0f;	// 氷が沈む高さ
const int MAX_FLOWTIMING = 12;		// 氷が沈む最大タイミング
const int MIN_FLOWTIMING = 1;		// 氷が沈む最小タイミング
const float RATE_SPEED_MOVE = 1.2f;	// 移動速度の割合
const int SCREEN_OUT_SINK = 180;	// 画面外に行ったときに消えるカウント
}

//====================================================
// コンストラクタ
//====================================================
CBgIce::CBgIce() : CObjectX(), m_state(STATE_FLOW), m_fSpeed(0.0f), m_bInscreen(false), m_nRippleCount(0)
{

}

//====================================================
// デストラクタ
//====================================================
CBgIce::~CBgIce()
{

}

//====================================================
// 生成処理
//====================================================
CBgIce* CBgIce::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,int type)
{
	CBgIce* pBgIce = new CBgIce;

	pBgIce->SetPosition(pos);
	pBgIce->SetRotation(rot);

	pBgIce->Init();
	
	pBgIce->BindModel(CModel::Load(MODEL[type]));

	return pBgIce;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CBgIce::Init(void)
{
	// 最初の波紋出現時間設定
	m_nRippleCount = bgice::RIPPLE_DEFAULT + universal::RandRange(bgice::RIPPLE_DEGREE, -bgice::RIPPLE_DEGREE);

	// 速度の設定
	m_fSpeed = (float)universal::RandRange(MAX_FLOWTIMING, MIN_FLOWTIMING) * 0.01f;

	// 継承クラスの初期化
	CObjectX::Init();

	// スクリーン外にいる時間を数える変数初期化
	m_nOutScreen = 0;

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CBgIce::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CBgIce::Update(void)
{
	//移動処理
	Move();

	// 波紋出現確認
	RippleCheck();

	CObjectX::Update();
}

//====================================================
// 描画処理
//====================================================
void CBgIce::Draw(void)
{
	CObjectX::Draw();
}

//=====================================================
// 読込処理
//=====================================================
void CBgIce::Load(char* pPath)
{
	//変数宣言
	char cTemp[MAX_STRING];

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	//ファイルから読み込む
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		while (true)
		{
			//文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "SET") == 0)
			{//キースタート
				while (strcmp(cTemp, "END_SET") != 0)
				{//終わりまでキー設定

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//位置取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						for (int nCntPos = 0; nCntPos < 3; nCntPos++)
						{
							(void)fscanf(pFile, "%f", &pos[nCntPos]);
						}
					}

					if (strcmp(cTemp, "ROT") == 0)
					{//向き取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						for (int nCntRot = 0; nCntRot < 3; nCntRot++)
						{
							(void)fscanf(pFile, "%f", &rot[nCntRot]);
						}
					}
				}

				Create(pos, rot, 0);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
				break;
		}//while
	}//file
	else
	{
		assert(("BgIce読み込みに失敗", false));
	}

	fclose(pFile);
}

//====================================================
// 移動処理
//====================================================
void CBgIce::Move(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// 海流のベクトル取得
	COcean::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	D3DXVECTOR3 pos = GetPosition();

	if (universal::IsInScreen(pos))
	{// 一度画面に入ったフラグを立てる
		m_bInscreen = true;
	}
	else if (m_bInscreen || m_nOutScreen >= SCREEN_OUT_SINK)
	{
		StartSink();

		m_nOutScreen = 0;
	}
	
	m_nOutScreen++;

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceManager->GetOceanLevel();

	switch (m_state)
	{
	case STATE_FLOW:
		// 海流による移動
		D3DXVec3Normalize(&vecStream, &vecStream);
		vecStream *= fSpeedFlow * RATE_SPEED_MOVE;
		Translate(vecStream);

		//海面に沿わせる
		Flow();

		// 足場の氷との判定
		CollideIce();

		break;

	case STATE_SINK:

		m_bInscreen = false;

		fSpeedGravity += m_fSpeed;

		pos.y -= fSpeedGravity;

		if (pos.y <= MAX_HEIGHT)
			Uninit();

		SetPosition(pos);

		break;
	}
}

//====================================================
// 海面に沿わせる処理
//====================================================
void CBgIce::Flow(void)
{
	COcean* pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
	{
		return;
	}

	// 海と一緒に氷を動かす処理
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = pOcean->GetHeight(pos, &move);

	SetPosition(pos);
}

//====================================================
// 波紋出現確認処理
//====================================================
void CBgIce::RippleCheck(void)
{
	m_nRippleCount--;	// カウント減らす

	if (m_nRippleCount <= 0)
	{
		// 海の上を特定
		D3DXVECTOR3 pos = GetPosition();
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLE, D3DXVECTOR3(pos.x, 30.0f, pos.z),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(80.0f, 80.0f, 80.0f));	// 波紋出現
		m_nRippleCount = bgice::RIPPLE_DEFAULT + universal::RandRange(bgice::RIPPLE_DEGREE, -bgice::RIPPLE_DEGREE);	// 波紋出現時間設定
	}
}

//====================================================
// 足場氷との判定
//====================================================
void CBgIce::CollideIce(void)
{
	vector<CIce*> apIce = CIce::GetInstance();

	for (CIce *pIce : apIce)
	{
		if (pIce == nullptr)
			continue;

		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posIce = pIce->GetPosition();

		if (universal::DistCmpFlat(pos, posIce, Grid::SIZE, nullptr))
		{// 氷と当たってたら沈み始める
			StartSink();
			return;
		}
	}
}

//====================================================
// 沈み始める処理
//====================================================
void CBgIce::StartSink(void)
{
	m_state = STATE_SINK;
	MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLE, GetPosition());
}