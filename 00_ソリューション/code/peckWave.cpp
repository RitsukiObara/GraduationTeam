//*****************************************************
//
// つっつき波紋の処理[peckWave.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "peckWave.h"
#include "manager.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
namespace polygon
{
const string PATH_TEX = "data\\TEXTURE\\UI\\peckWave.png";	// テクスチャパス
const float SIZE_INIT = 0.0f;								// 初期のサイズ
const float SIZE_DEST = 50.0f;								// 目標のサイズ
const float SIZE_DIFF = SIZE_DEST - SIZE_INIT;				// 初期のサイズ差分
const float TIME_SCALING = 1.0f;							// スケーリングにかかる時間
const float SPEED_ROTATE = 0.05f;							// 回転速度
}
}

//====================================================
// コンストラクタ
//====================================================
CPeckWave::CPeckWave(int nPriority) : CPolygon3D(nPriority), m_fTimer(0.0f)
{

}

//====================================================
// デストラクタ
//====================================================
CPeckWave::~CPeckWave()
{

}

//====================================================
// 生成処理
//====================================================
CPeckWave *CPeckWave::Create(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	CPeckWave *pPeckWave = nullptr;

	pPeckWave = new CPeckWave;

	if (pPeckWave != nullptr)
	{
		pPeckWave->Init();
		pPeckWave->SetPosition(pos);
		pPeckWave->SetColor(col);
		pPeckWave->SetVtx();
	}

	return pPeckWave;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CPeckWave::Init(void)
{
	CPolygon3D::Init();

	SetSize(polygon::SIZE_INIT, polygon::SIZE_INIT);

	int nIdxTexture = Texture::GetIdx(&polygon::PATH_TEX[0]);
	SetIdxTexture(nIdxTexture);

	EnableZtest(true);

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CPeckWave::Uninit(void)
{
	CPolygon3D::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CPeckWave::Update(void)
{
	CPolygon3D::Update();

	if (polygon::TIME_SCALING < m_fTimer)
	{// 時間を越えたら終了
		Uninit();
		return;
	}

	//-----------------------------
	// スケーリング
	//-----------------------------
	m_fTimer += CManager::GetDeltaTime();

	float fTime = m_fTimer / polygon::TIME_SCALING;	// タイマーの割合をイージング
	float fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	float fSize = polygon::SIZE_INIT + polygon::SIZE_DIFF * fRate;

	SetSize(fSize,fSize);

	//-----------------------------
	// 回転
	//-----------------------------
	Rotate(D3DXVECTOR3(0.0f, polygon::SPEED_ROTATE, 0.0f));
}

//====================================================
// 描画処理
//====================================================
void CPeckWave::Draw(void)
{
	CPolygon3D::Draw();
}