//*****************************************************
//
// ３Dアニメーションエフェクト処理[animEffect3D.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "animEffect3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "anim3D.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CAnimEffect3D *CAnimEffect3D::m_pAnimEffect3D = nullptr;	// 自身のポインタ

//*****************************************************
// マクロ定義
//*****************************************************
#define FILE_PATH	"data\\TEXT\\animEffect3D.txt"	// ファイルのパス

//=====================================================
// コンストラクタ
//=====================================================
CAnimEffect3D::CAnimEffect3D(int nPriority) : CObject(nPriority)
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		m_apAnimEffect[i] = nullptr;
	}
}

//=====================================================
// デストラクタ
//=====================================================
CAnimEffect3D::~CAnimEffect3D()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CAnimEffect3D::Init(void)
{
	// 読込
	Load();

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CAnimEffect3D::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntEffect = 0;

	// ファイルから読み込む
	FILE *pFile = fopen(FILE_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "EFFECTSET") == 0)
			{// 読込開始
				if (m_apAnimEffect[nCntEffect] == nullptr)
				{
					// インスタンス生成
					m_apAnimEffect[nCntEffect] = new SInfoAnimEffect;

					ZeroMemory(m_apAnimEffect[nCntEffect],sizeof(SInfoAnimEffect));
				}

				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "END_EFFECTSET") == 0)
					{// 終了条件
						nCntEffect++;

						break;
					}

					if (strcmp(cTemp, "PATH") == 0)
					{// パス
						(void)fscanf(pFile, "%s", &cTemp[0]);
						
						(void)fscanf(pFile, "%s", &m_apAnimEffect[nCntEffect]->acPath[0]);
					}

					if (strcmp(cTemp, "NUM_ANIM") == 0)
					{// アニメーション数
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apAnimEffect[nCntEffect]->nNumAnim);
					}

					if (strcmp(cTemp, "SPEED_ANIM") == 0)
					{// アニメーション速度
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apAnimEffect[nCntEffect]->nSpeedAnim);
					}

					if (strcmp(cTemp, "SIZE") == 0)
					{// サイズ
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->fSize);
					}

					if (strcmp(cTemp, "COL") == 0)
					{// 色読み込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->col.r);
						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->col.g);
						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->col.b);
						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->col.a);
					}

					if (strcmp(cTemp, "IS_ADD") == 0)
					{// 加算合成するかどうか
						int i;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &i);

						if (i == 1)
						{
							m_apAnimEffect[nCntEffect]->bAdd = true;
						}
						else
						{
							m_apAnimEffect[nCntEffect]->bAdd = false;
						}
					}

					if (strcmp(cTemp, "IS_LOOP") == 0)
					{// ループするかどうか
						int i;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &i);

						if (i == 1)
						{
							m_apAnimEffect[nCntEffect]->bLoop = true;
						}
						else
						{
							m_apAnimEffect[nCntEffect]->bLoop = false;
						}
					}

					if (strcmp(cTemp, "IS_BILLBOARD") == 0)
					{// ビルボードにするかどうか
						int i;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &i);

						if (i == 1)
						{
							m_apAnimEffect[nCntEffect]->bBillboard = true;
						}
						else
						{
							m_apAnimEffect[nCntEffect]->bBillboard = false;
						}
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CAnimEffect3D::Uninit(void)
{
	m_pAnimEffect3D = nullptr;

	for (int i = 0; i < TYPE_MAX; i++)
	{
		if (m_apAnimEffect[i] != nullptr)
		{
			delete m_apAnimEffect[i];
			m_apAnimEffect[i] = nullptr;
		}
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CAnimEffect3D::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CAnimEffect3D::Draw(void)
{

}

//=====================================================
// 生成処理
//=====================================================
CAnimEffect3D *CAnimEffect3D::Create(void)
{
	if (m_pAnimEffect3D == nullptr)
	{
		m_pAnimEffect3D = new CAnimEffect3D;

		if (m_pAnimEffect3D != nullptr)
		{
			// 初期化処理
			m_pAnimEffect3D->Init();
		}
	}

	return m_pAnimEffect3D;
}

//=====================================================
// アニメーションの生成
//=====================================================
CAnim3D *CAnimEffect3D::CreateEffect(D3DXVECTOR3 pos, TYPE type)
{
	if ((type >= TYPE_MAX || type < 0 ) &&
		m_apAnimEffect[type] != nullptr)
	{// エラー
		return nullptr;
	}

	CAnim3D *pAnim3D = nullptr;

	// インスタンス生成
	pAnim3D = CAnim3D::Create(pos, m_apAnimEffect[type]->nNumAnim, m_apAnimEffect[type]->nSpeedAnim, m_apAnimEffect[type]->bLoop);

	if (pAnim3D != nullptr)
	{
		// 色の設定
		pAnim3D->SetColor(m_apAnimEffect[type]->col);

		pAnim3D->SetSize(m_apAnimEffect[type]->fSize, m_apAnimEffect[type]->fSize);
		pAnim3D->EnableAdd(m_apAnimEffect[type]->bAdd);

		CTexture *pTexture = CTexture::GetInstance();

		if (pTexture != nullptr)
		{// テクスチャの設定
			int nIdx = pTexture->Regist(&m_apAnimEffect[type]->acPath[0]);
			pAnim3D->SetIdxTexture(nIdx);

			if (m_apAnimEffect[type]->bBillboard)
			{
				pAnim3D->SetMode(CAnim3D::MODE_BILLBOARD);
			}

			pAnim3D->SetVtx();
		}
	}

	return pAnim3D;
}

namespace Anim3D
{
CAnim3D *CreateAnim(D3DXVECTOR3 pos, CAnimEffect3D::TYPE type)
{
	CAnimEffect3D *pManager = CAnimEffect3D::GetInstance();

	if (pManager == nullptr)
		return nullptr;

	CAnim3D *pAnim = pManager->CreateEffect(pos, type);

	return pAnim;
}
}