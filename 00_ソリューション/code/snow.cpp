//*****************************************************
//
// 雪処理[snow.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "snow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "slow.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(7.0f)	// 移動速度

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float SIZE_RATIO = 0.01f;	// サイズ倍率
	const int MAX_ROT_MOVE = 1;		// 最大向き速度
	const int MIN_ROT_MOVE = -3;	// 最低向き速度
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CSnow::m_nsnowCnt = 0;	// 雪が来るまでの頻度

//=====================================================
// コンストラクタ
//=====================================================
CSnow::CSnow(int nPriority) : CPolygon3D(nPriority)
{
	m_nLife = 0;
	m_move = { 0.0f,0.0f,0.0f };
}

//=====================================================
// デストラクタ
//=====================================================
CSnow::~CSnow()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSnow::Init(void)
{
	// 継承クラスの初期化
	CPolygon3D::Init();

	SetMode(CPolygon3D::MODE_BILLBOARD);
	EnableZtest(true);

	m_nsnowCnt = 0;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSnow::Uninit(void)
{
	// 継承クラスの終了
	CPolygon3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSnow::Update(void)
{
	// 継承クラスの更新
	CPolygon3D::Update();

	// 寿命減衰
	m_nLife--;

	float fWidth = GetWidth();
	float fHeight = GetHeight();

	D3DXVECTOR3 pos = GetPosition();

	CSlow* pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		// 位置の更新
		pos += m_move * fScale;
	}
	else
	{
		// 位置の更新
		pos += m_move;
	}

	// 位置更新
	SetPosition(pos);

	// 動き処理
	Move();

	if (m_nLife < 0)
	{// 自分の削除
		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CSnow::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// カリングを無効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CPolygon3D::Draw();

	// ライティングを有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングを有効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// 生成処理
//=====================================================
CSnow* CSnow::Create(D3DXVECTOR3 pos, float fRadius, int nLife,D3DXVECTOR3 move)
{
	CSnow* pEffect3D = nullptr;

	if (pEffect3D == nullptr)
	{// インスタンス生成
		pEffect3D = new CSnow;

		if (pEffect3D != nullptr)
		{
			// 初期化処理
			pEffect3D->Init();

			pEffect3D->SetPosition(pos);
			pEffect3D->SetSize(fRadius, fRadius);

			pEffect3D->SetMode(MODE_NORMAL);

			// テクスチャの読込
			int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\EFFECT\\snow.png");
			pEffect3D->SetIdxTexture(nIdx);

			pEffect3D->m_nLife = nLife;

			pEffect3D->m_move = move;

			pEffect3D->SetVtx();
		}
	}

	return pEffect3D;
}

//=====================================================
// 動き処理
//=====================================================
void CSnow::Move(void)
{
	D3DXVECTOR3 rot = GetRotation();

	// 向きの動き設定
	float rot_move = (float)universal::RandRange(MAX_ROT_MOVE, MIN_ROT_MOVE) * SIZE_RATIO;

	// 移動量
	rot.x += rot_move;
	//rot.y += rot_move;
	/*rot.z += rot_move;*/

	// 向きのノーマライズ
	universal::LimitRot(&rot.x);
	//universal::LimitRot(&rot.y);
	/*universal::LimitRot(&rot.z);*/

	// 向き更新
	SetRotation(rot);

	// 頂点情報設定
	SetVtx();
}

//=====================================================
// 雪の設置処理
//=====================================================
void CSnow::SetSnow(int nMaxPos_X, int nMinPos_X, float fHeight, int nMaxRadius, int nMinRadius, int nLife, D3DXVECTOR3 move, int nFrequency)
{
	// 雪の場所を設定
	float snowPos = (float)universal::RandRange(nMaxPos_X, nMinPos_X);

	// 雪の半径を設定
	float snowRadius = (float)universal::RandRange(nMaxRadius, nMinRadius);

	m_nsnowCnt++;

	if (m_nsnowCnt >= nFrequency)
	{
		// 雪を生成
		CSnow::Create(D3DXVECTOR3(snowPos, fHeight, 0.0f), snowRadius, nLife,
			D3DXVECTOR3(move.x, move.y, move.z));

		m_nsnowCnt = 0;
	}
}
