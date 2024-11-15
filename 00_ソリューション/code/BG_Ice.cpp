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

//*****************************************************
// マクロ定義
//*****************************************************
#define RESULT_WIDTH	(0.18f)	// 項目の幅
#define RESULT_HEIGHT	(0.05f)	// 項目の高さ
#define MOVE_FACT	(0.15f)	// 移動速度
#define LINE_ARRIVAL	(0.05f)	// 到着したとされるしきい値
#define LINE_UNINIT	(3.0f)	// 終了するまでのしきい値

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const char* MODEL[CBgIce::TYPE_MAX] = { "data\\MODEL\\block\\Drift_ice.x","data\\MODEL\\block\\Drift_ice_small.x" };
	const float GRAVITY_SPEED = 0.1f;	// 背景氷が沈んでいく速度
	const float MAX_HEIGHT = -300.0f;	// 氷が沈む高さ
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//====================================================
// コンストラクタ
//====================================================
CBgIce::CBgIce()
{
	m_type = TYPE_BIG;
	m_state = STATE_FLOW;
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
CBgIce* CBgIce::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,TYPE type)
{
	CBgIce* pBgIce = new CBgIce;

	pBgIce->SetPosition(pos);
	pBgIce->SetRotation(rot);

	pBgIce->m_type = type;

	pBgIce->Init();

	return pBgIce;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CBgIce::Init(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return S_OK;

	// 海流のベクトル取得
	CIceManager::E_Stream dir = pIceManager->GetDirStream();

	//現在の海流情報取得
	m_streamOld = dir;

	CObjectX::Init();

	CObjectX::BindModel(CModel::Load(MODEL[m_type]));

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
	TYPE type;

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

									if (strcmp(cTemp, "TYPE") == 0)
									{//タイプ取得
										(void)fscanf(pFile, "%s", &cTemp[0]);

										(void)fscanf(pFile, "%d", &type);
									}
								}
						CBgIce::Create(pos, rot, type);
					}

					if (strcmp(cTemp, "END_SCRIPT") == 0)
					{
						break;
					}
		}//while
	}//file
	else
	{
		assert(("モーションデータ読み込みに失敗", false));
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
	CIceManager::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	if (pIceManager->GetDirStream() != m_streamOld)
	{
		m_state = STATE_SINK;
	}

	//現在の海流情報取得
	m_streamOld = dir;

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceManager->GetOceanLevel();

	switch (m_state)
	{
	case STATE_FLOW:
		D3DXVec3Normalize(&vecStream, &vecStream);
		/*vecStream *= fSpeedFlow;*/
		AddPosition(vecStream);

		//海面に沿わせる
		Flow();

		break;

	case STATE_SINK:

		D3DXVECTOR3 pos = GetPosition();

		fgravity_speed += GRAVITY_SPEED;

		pos.y -= fgravity_speed;

		if (pos.y <= MAX_HEIGHT)
		{
			Uninit();
		}

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