//*****************************************************
//
// フェード処理（氷が落ちてくる）[fade_fallice.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fade_fallice.h"
#include "blur.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
	std::vector<std::vector<POINT>> FALLICE_PATTERN =
	{
		{// 1
			{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0}
		},
		{// 2
			{0,1},{1,1},{2,1},{4,1},{5,1},{6,1},
		},
		{// 3
			{0,2},{1,2},{2,2},{3,1},{5,2},{6,2},{7,2},
		},
		{// 4
			{0,3},{1,3},{3,2},{4,2},{5,3},{6,3},
		},
		{// 5
			{0,4},{1,4},{2,3},{4,3},{6,4},{7,4},
		},
		{// 6
			{0,5},{2,4},{3,3},{5,4},{6,5},
		},
		{// 7
			{1,5},{3,4},{4,4},{5,5},
		},
		{// 8
			{2,5},{4,5},
		},
		{// 9
			{3,5},
		},
	};
	const int FALLICE_COUNT = 10;
	const float POLYGON_WIDTH = 216.0f;
	const float POLYGON_HEIGHT = 250.0f;
	const char* FALLICE_TEX_PATH = "data\\TEXTURE\\UI\\Fade_FallIce_NoAnti.png";
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CFade_FallIce* CFade_FallIce::m_pFade = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CFade_FallIce::CFade_FallIce()
{
	m_nCounterFrame = 0;
	m_nCounterPattern = 0;
	m_pTexture = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CFade_FallIce::~CFade_FallIce()
{

}

//=====================================================
// 生成処理
//=====================================================
CFade_FallIce* CFade_FallIce::Create(void)
{
	if (m_pFade == nullptr)
	{
		m_pFade = new CFade_FallIce;

		if (m_pFade != nullptr)
		{
			m_pFade->Init();
		}
	}

	return m_pFade;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CFade_FallIce::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();	// デバイスの取得

	//テクスチャ読み込み
	m_pTexture = nullptr;
	if (m_pTexture == nullptr)
	{
		D3DXCreateTextureFromFile
		(pDevice, FALLICE_TEX_PATH, &m_pTexture);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFade_FallIce::Uninit(void)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CFade_FallIce::Update(void)
{
	CFade::FADE fade = CFade::GetState();

	if (fade != FADE_NONE)
	{
		if (fade == FADE_IN)
		{//フェードイン状態
			m_nCounterFrame++;

			if (m_nCounterFrame == FALLICE_COUNT)
			{
				// カウンタ設定
				m_nCounterFrame = 0;
				m_nCounterPattern++;

				// 次の氷出す
				LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();	// デバイスの取得
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = nullptr;
				if (pVtxBuff == nullptr)
				{
					//頂点バッファの生成
					pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * FALLICE_PATTERN[m_nCounterPattern].size(),
						D3DUSAGE_WRITEONLY,
						FVF_VERTEX_2D,
						D3DPOOL_MANAGED,
						&pVtxBuff,
						NULL);
				}

				//頂点情報のポインタ
				VERTEX_2D* pVtx;

				//頂点バッファをロックし、頂点情報へのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				for (auto itr = FALLICE_PATTERN[m_nCounterPattern].begin(); itr != FALLICE_PATTERN[m_nCounterPattern].end(); itr++)
				{
					// 配置位置計算
					D3DXVECTOR3 pos;
					pos.x = ((((*itr).y % 2 == 0) ? 0 : 0.5f) + (*itr).x) * POLYGON_WIDTH;
					pos.y = SCREEN_HEIGHT - (POLYGON_HEIGHT - (POLYGON_WIDTH / 2.0f / sqrtf(3))) * (*itr).y;
					pos.z = 0.0f;

					// ポリゴン情報設定
					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3(pos.x - POLYGON_WIDTH * 0.5f, pos.y - POLYGON_HEIGHT * 0.5f, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(pos.x + POLYGON_WIDTH * 0.5f, pos.y - POLYGON_HEIGHT * 0.5f, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(pos.x - POLYGON_WIDTH * 0.5f, pos.y + POLYGON_HEIGHT * 0.5f, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(pos.x + POLYGON_WIDTH * 0.5f, pos.y + POLYGON_HEIGHT * 0.5f, 0.0f);

					//rhwの設定
					pVtx[0].rhw = 1.0f;
					pVtx[1].rhw = 1.0f;
					pVtx[2].rhw = 1.0f;
					pVtx[3].rhw = 1.0f;

					//頂点カラー設定
					pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

					//テクスチャ設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

					// 頂点バッファ移動
					pVtx += 4;
				}
				//頂点バッファをアンロック
				pVtxBuff->Unlock();

				// 頂点バッファを保存
				m_icePolygon.push_back(pVtxBuff);

				if (m_nCounterPattern == FALLICE_PATTERN.size() - 1)
				{//氷で埋めたら
					SetState(FADE_OUT);
					m_nCounterFrame = 0;
					m_nCounterPattern = -1;

					if (IsTrans())
					{
						Blur::ResetBlur();

						//モード設定
						CManager::SetMode(GetModeNext());
					}
				}
			}
		}
		else if (fade == FADE_OUT)
		{//フェードアウト状態
			m_nCounterFrame++;

			if (m_nCounterFrame == FALLICE_COUNT)
			{
				// カウンタ設定
				m_nCounterFrame = 0;
				m_nCounterPattern++;

				// 氷削除（後々落下）
				if (m_icePolygon[m_nCounterPattern] != nullptr)
				{
					m_icePolygon[m_nCounterPattern]->Release();
					m_icePolygon[m_nCounterPattern] = nullptr;
				}
			}

			if (m_nCounterPattern == FALLICE_PATTERN.size() - 1)
			{//氷が落ち切ったら
				SetState(FADE_NONE);
				m_icePolygon.clear();
			}
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CFade_FallIce::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	for (int cntVtx = 0; cntVtx < (int)m_icePolygon.size(); cntVtx++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_icePolygon[cntVtx], 0, sizeof(VERTEX_2D));

		//ポリゴンの描画
		for (int cntSize = 0; cntSize < (int)FALLICE_PATTERN[cntVtx].size(); cntSize++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, cntSize * 4, 2);
		}
	}
}

//=====================================================
// 設定処理
//=====================================================
void CFade_FallIce::SetFade(CScene::MODE modeNext, bool bTrans)
{
	CFade::FADE fade = CFade::GetState();
	if (fade == FADE_NONE)
	{
		SetState(FADE_IN);
		SetModeNext(modeNext);
		SetIsTrans(bTrans);
		m_nCounterFrame = 0;
		m_nCounterPattern = -1;
	}
}
