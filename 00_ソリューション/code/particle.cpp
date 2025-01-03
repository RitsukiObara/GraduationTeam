//*****************************************************
//
// パーティクル処理[particle.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "particle.h"
#include "effect3D.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CParticle::PARTICLE_INFO *CParticle::m_apParticleInfo[CParticle::TYPE_MAX + 1] = {};

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CParticle::CParticle(int nPriority) : CObject(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_type = TYPE_NONE;
	m_pPosOwner = nullptr;
	m_nPriorityEffect = 0;
	m_nLife = 0;
}

//=====================================================
//	デストラクタ
//=====================================================
CParticle::~CParticle()
{

}

//=====================================================
//	初期化処理
//=====================================================
HRESULT CParticle::Init(void)
{
	return S_OK;
}

//=====================================================
//	終了処理
//=====================================================
void CParticle::Uninit(void)
{
	Release();
}

//=====================================================
//	破棄処理
//=====================================================
void CParticle::Unload(void)
{
	for (int nCntParticle = 0;nCntParticle < TYPE_MAX;nCntParticle++)
	{
		if (m_apParticleInfo[nCntParticle] != nullptr)
		{
			delete m_apParticleInfo[nCntParticle];
			m_apParticleInfo[nCntParticle] = nullptr;
		}
	}
}

//=====================================================
//	更新処理
//=====================================================
void CParticle::Update(void)
{
	// 変数宣言
	float fRot,fRot2;
	float fMove = 0.0f;
	float fRadius = 0.0f;
	int nLife = 0;
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
	CEffect3D *pEffect3D = nullptr;

	for (int nCntEffect = 0; nCntEffect < m_apParticleInfo[m_type]->nNumEffect; nCntEffect++)
	{
		if (m_apParticleInfo[m_type]->nRot)
		{// 向きの反映
			if (m_apParticleInfo[m_type]->fRangeRot > 0.1f)
			{
				fRot = m_rot.x + (float)(rand() % (int)(m_apParticleInfo[m_type]->fRangeRot * 100.0f) - m_apParticleInfo[m_type]->fRangeRot * 50.0f) / 100.0f;
				fRot2 = m_rot.y + (float)(rand() % (int)(m_apParticleInfo[m_type]->fRangeRot * 100.0f) - m_apParticleInfo[m_type]->fRangeRot * 50.0f) / 100.0f * 2;
			}
			else
			{
				fRot = m_rot.x + (rand() % (int)(0.1f * 100.0f) - 0.1f * 50.0f) / 100.0f;
				fRot2 = m_rot.y + (rand() % (int)(0.1f * 100.0f) - 0.1f * 50.0f) / 100.0f * 2;
			}
		}
		else
		{// 全方位
			fRot = rand() % 628 - 314 / 100.0f;
			fRot2 = rand() % 628 - 314 / 100.0f;
		}

		if (m_apParticleInfo[m_type]->fSpeed > 0.1f)
		{
			fMove = (float)(rand() % (int)(m_apParticleInfo[m_type]->fSpeed * 10)) * 0.1f + m_apParticleInfo[m_type]->fSpeed * 0.5f;
		}
		if (m_apParticleInfo[m_type]->fRadiusEffect > 0.1f)
		{
			fRadius = (float)(rand() % (int)(m_apParticleInfo[m_type]->fRadiusEffect * 10)) * 0.1f + m_apParticleInfo[m_type]->fRadiusEffect * 0.5f;
		}
		if (m_apParticleInfo[m_type]->nLifeEffect != 0)
		{
			nLife = rand() % m_apParticleInfo[m_type]->nLifeEffect + m_apParticleInfo[m_type]->nLifeEffect / 2;
		}

		move.x = sinf(fRot) * sinf(fRot2) * fMove;
		move.y = cosf(fRot) * fMove;
		move.z = sinf(fRot) * cosf(fRot2) * fMove;

		if (m_apParticleInfo[m_type]->bTurn)
		{
			// エフェクト生成
			pEffect3D = CEffect3D::Create(
				&m_apParticleInfo[m_type]->acTexPath[0],
				m_pos + move * (float)nLife, 
				fRadius, 
				nLife, 
				m_apParticleInfo[m_type]->col, 
				-move, 
				m_apParticleInfo[m_type]->fGravity, 
				m_apParticleInfo[m_type]->nAdd != 0, 
				m_apParticleInfo[m_type]->fDecrease, 
				m_pPosOwner, 
				m_nPriorityEffect,
				true);
		}
		else
		{
			// エフェクト生成
			pEffect3D = CEffect3D::Create(
				&m_apParticleInfo[m_type]->acTexPath[0],
				m_pos,
				fRadius,
				nLife,
				m_apParticleInfo[m_type]->col, 
				move, 
				m_apParticleInfo[m_type]->fGravity, 
				m_apParticleInfo[m_type]->nAdd != 0, 
				m_apParticleInfo[m_type]->fDecrease, 
				m_pPosOwner, 
				m_nPriorityEffect);
		}

		if (pEffect3D != nullptr)
		{
			pEffect3D->SetMode((CPolygon3D::MODE)m_apParticleInfo[m_type]->modeRender);

			if (m_apParticleInfo[m_type]->modeRender == CPolygon3D::MODE_STRETCHBILLBOARD)
			{
				pEffect3D->SetSize(fRadius, fRadius * 10.0f);
			}

			pEffect3D->SetVtx();
			pEffect3D->EnableBlur(false);
		}
	}

	m_nLife--;

	if (m_nLife < 0)
	{
		// 自身の破棄
		Uninit();
	}
}

//=====================================================
//	位置設定処理
//=====================================================
void CParticle::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
//	生成処理
//=====================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, TYPE type,D3DXVECTOR3 rot, D3DXVECTOR3 *pPosOwner,int nPriority)
{
	CParticle *pParticle = nullptr;

	if (pParticle == nullptr)
	{// インスタンス生成
		pParticle = new CParticle;

		pParticle->m_type = type;

		pParticle->m_pos = pos;

		pParticle->m_rot = rot;

		pParticle->m_nLife = m_apParticleInfo[type]->nLife;
		
		pParticle->m_pPosOwner = pPosOwner;

		pParticle->m_nPriorityEffect = nPriority;
	}

	return pParticle;
}

//=====================================================
//	読込処理
//=====================================================
void CParticle::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntParticle = 1;
	PARTICLE_INFO *pInfo = nullptr;

	// ファイルから読み込む
	FILE *pFile = fopen("data\\TEXT\\particle.txt", "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			if (fscanf(pFile, "%s", &cTemp[0]) == EOF)
				break;

			if (strcmp(cTemp, "PARTICLESET") == 0)
			{// 読込開始
				if (m_apParticleInfo[nCntParticle] == nullptr)
				{
					// インスタンス生成
					m_apParticleInfo[nCntParticle] = new PARTICLE_INFO;

					if (m_apParticleInfo[nCntParticle] != nullptr)
					{
						m_apParticleInfo[nCntParticle]->modeRender = 1;
					}
				}

				pInfo = m_apParticleInfo[nCntParticle];

				while (true)
				{
					// 文字読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "END_PARTICLESET") == 0)
					{// パーティクル情報終了条件
						break;
					}

					if (strcmp(cTemp, "TEXTURE_PATH") == 0)
					{// パーティクル寿命取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%s", &pInfo->acTexPath[0]);
					}

					if (strcmp(cTemp, "LIFE_PARTICLE") == 0)
					{// パーティクル寿命取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nLife);
					}

					if (strcmp(cTemp, "NUM_EFFECT") == 0)
					{// エフェクト数取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nNumEffect);
					}

					if (strcmp(cTemp, "LIFE_EFFECT") == 0)
					{// エフェクト寿命取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nLifeEffect);
					}

					if (strcmp(cTemp, "RADIUS_EFFECT") == 0)
					{// エフェクト半径取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->fRadiusEffect);
					}

					if (strcmp(cTemp, "SPEED_EFFECT") == 0)
					{// エフェクト速度取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->fSpeed);
					}

					if (strcmp(cTemp, "GRAVITY") == 0)
					{// 重力取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->fGravity);
					}

					if (strcmp(cTemp, "DECREASE") == 0)
					{// 重力取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->fDecrease);
					}

					if (strcmp(cTemp, "RANGEROT") == 0)
					{// 向きのランダム幅取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->fRangeRot);
					}

					if (strcmp(cTemp, "IS_ADD") == 0)
					{// 加算合成かどうか取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nAdd);
					}

					if (strcmp(cTemp, "IS_ROT") == 0)
					{// 加算合成かどうか取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->nRot);
					}

					if (strcmp(cTemp, "IS_TURN") == 0)
					{// 加算合成かどうか取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "1") == 0)
						{
							pInfo->bTurn = true;
						}
						else
						{
							pInfo->bTurn = false;
						}
					}

					if (strcmp(cTemp, "MODE_RENDER") == 0)
					{// 加算合成かどうか取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &pInfo->modeRender);
					}

					if (strcmp(cTemp, "COLOR") == 0)
					{// 色取得
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &pInfo->col.r);
						(void)fscanf(pFile, "%f", &pInfo->col.g);
						(void)fscanf(pFile, "%f", &pInfo->col.b);
						(void)fscanf(pFile, "%f", &pInfo->col.a);
					}
				}

				// パーティクル数加算
				nCntParticle++;
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		assert(("パーティクル情報読み込みに失敗", false));
	}
}