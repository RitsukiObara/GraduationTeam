//*****************************************************
//
// パーティクル処理[particle.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "particle.h"
#include "effect3D.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "manager.h"
#include "debugproc.h"
#include <stdio.h>

//*****************************************************
// マクロ定義
//*****************************************************
#define ADD_SPEED	(0.01f)	// FLOAT型の加算量
#define ADD_INT	(1)	// INT型の加算量
#define ADD_COLOR	(0.002f)	// 色の加算量

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CParticle::m_nLife = 5;	// 寿命
int CParticle::m_nRate = 2;	// 加算倍率
int CParticle::m_nCurrentCol = 0;	// 色の選択要素
int CParticle::m_nLifeEffect = 10;	// エフェクトの寿命
float CParticle::m_fRadiusEffect = 5.0f;	// エフェクトの半径
int CParticle::m_nNumEffect = 5;	// エフェクトの数
float CParticle::m_fRangeSpeed = 10.0f;	// スピードの幅
float CParticle::m_fRangeRadius = 20.0f;	// サイズの幅
float CParticle::m_fSpeed = 5.0f;	// エフェクトの基準スピード
float CParticle::m_fGravity = 0.0f;	// エフェクトの基準スピード
D3DXCOLOR CParticle::m_col = { 1.0f,1.0f,1.0f,1.0f };	// 色
int CParticle::m_bAdd = true;	// 加算処理をするかどうか
int CParticle::m_bRot = true;	// 向きを反映するかどうか
bool CParticle::m_bLoop = true;	// ループするかどうか
bool CParticle::m_bTurn = true;	// 反転するかどうか
D3DXVECTOR3 CParticle::m_rot = { 0.0f,0.0f,0.0f };	// 向き
float CParticle::m_fRangeRot = 0.0f;	// 向きのランダム幅
float CParticle::m_fDecrease = 0.0f;	// 半径減衰量
int CParticle::m_nNumAll = 0;	// 総数
int CParticle::m_modeRender = 1;	// 描画モード

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CParticle::CParticle(int nPriority) : CObject(nPriority)
{
	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CParticle::~CParticle()
{
	m_nNumAll--;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CParticle::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CParticle::Uninit(void)
{
	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CParticle::Update(void)
{
	if (m_nLifeParticle < 0)
	{
		if (m_bLoop)
		{// 再生し続ける
			Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), CParticle::TYPE_NONE);
		}

		// 自身の破棄
		Uninit();

		return;
	}

	// 変数宣言
	float fRot,fRot2,fMove = 0.0f,fRadius = 0.0f;
	int nLife = 0;
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
	CEffect3D *pEffect3D = NULL;

	for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
	{
		if (m_bRot)
		{// 向きの反映
			if (m_fRangeRot > 0.01f)
			{
				fRot = m_rot.x + (float)(rand() % (int)(m_fRangeRot * 100.0f) - m_fRangeRot * 50.0f) / 100.0f;
				fRot2 = m_rot.y + (float)(rand() % (int)(m_fRangeRot * 100.0f) - m_fRangeRot * 50.0f) / 100.0f * 2;
			}
			else
			{
				fRot = m_rot.x + (rand() % (int)(0.1f * 100.0f) - 0.1f * 50.0f) / 100.0f;
				fRot2 = m_rot.y + (rand() % (int)(0.1f * 100.0f) - 0.1f * 50.0f) / 100.0f;
			}
		}
		else
		{// 全方位
			fRot = rand() % 628 - 314 / 100.0f;
			fRot2 = rand() % 628 - 314 / 100.0f;
		}

		if (m_fSpeed > 0.1f)
		{
			fMove = (float)(rand() % (int)(m_fSpeed * 10)) * 0.1f + m_fSpeed * 0.5f;
		}
		if (m_fRadiusEffect > 0.1f)
		{
			fRadius = (float)(rand() % (int)(m_fRadiusEffect * 10)) * 0.1f + m_fRadiusEffect * 0.5f;
		}
		if (m_nLifeEffect != 0)
		{
			nLife = rand() % m_nLifeEffect + m_nLifeEffect / 2;
		}

		move.x = sinf(fRot) * sinf(fRot2) * fMove;
		move.y = cosf(fRot) * fMove;
		move.z = sinf(fRot) * cosf(fRot2) * fMove;

		if (m_bTurn)
		{
			// エフェクト生成
			pEffect3D = CEffect3D::Create(move * nLife, fRadius, nLife, m_col, -move, m_fGravity, m_bAdd, m_fDecrease);
		}
		else
		{
			// エフェクト生成
			pEffect3D = CEffect3D::Create(m_pos, fRadius, nLife, m_col, move, m_fGravity, m_bAdd, m_fDecrease);
		}

		if (pEffect3D != nullptr)
		{
			pEffect3D->SetMode((CObject3D::MODE)m_modeRender);

			if (m_modeRender == CObject3D::MODE::MODE_STRETCHBILLBOARD)
			{
				pEffect3D->SetSize(fRadius, fRadius * 10.0f);
			}
		}
	}

	// 寿命減少
	m_nLifeParticle--;
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
CParticle *CParticle::Create(D3DXVECTOR3 pos, TYPE type)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{// インスタンス生成
		pParticle = new CParticle;

		if (pParticle != NULL)
		{
			pParticle->m_nLifeParticle = m_nLife;

			pParticle->m_pos = pos;
		}
	}

	return pParticle;
}

//=====================================================
// 読込処理
//=====================================================
void CParticle::Load(void)
{
	// 変数宣言
	char cTemp[256];

	// ファイルから読み込む
	FILE *pFile = fopen("data\\particle.txt", "r");

	if (pFile != NULL)
	{// ファイルが開けた場合
		while (1)
		{
			// 文字読み込み
			fscanf(pFile, "%s", &cTemp[0]);
			
			if (strcmp(cTemp, "LIFE_PARTICLE") == 0)
			{// パーティクル寿命取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_nLife);
			}

			if (strcmp(cTemp, "NUM_EFFECT") == 0)
			{// エフェクト数取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_nNumEffect);
			}

			if (strcmp(cTemp, "LIFE_EFFECT") == 0)
			{// エフェクト寿命取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_nLifeEffect);
			}

			if (strcmp(cTemp, "RADIUS_EFFECT") == 0)
			{// エフェクト半径取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%f", &m_fRadiusEffect);
			}

			if (strcmp(cTemp, "SPEED_EFFECT") == 0)
			{// エフェクト速度取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%f", &m_fSpeed);
			}

			if (strcmp(cTemp, "GRAVITY") == 0)
			{// 重力取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%f", &m_fGravity);
			}

			if (strcmp(cTemp, "DECREASE") == 0)
			{// 重力取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%f", &m_fDecrease);
			}

			if (strcmp(cTemp, "RANGEROT") == 0)
			{// 向きのランダム幅取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%f", &m_fRangeRot);
			}

			if (strcmp(cTemp, "IS_ADD") == 0)
			{// 加算合成かどうか取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_bAdd);
			}

			if (strcmp(cTemp, "IS_ROT") == 0)
			{// 向きを反映するかどうか取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_bRot);
			}

			if (strcmp(cTemp, "IS_TURN") == 0)
			{// 加算合成かどうか取得
				fscanf(pFile, "%s", &cTemp[0]);
				fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "1") == 0)
				{
					m_bTurn = true;
				}
				else
				{
					m_bTurn = false;
				}
			}

			if (strcmp(cTemp, "MODE_RENDER") == 0)
			{
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_modeRender);
			}

			if (strcmp(cTemp, "COLOR") == 0)
			{// 色取得
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%f", &m_col.r);
				fscanf(pFile, "%f", &m_col.g);
				fscanf(pFile, "%f", &m_col.b);
				fscanf(pFile, "%f", &m_col.a);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}
		}

		fclose(pFile);
	}
}

//=====================================================
// 保存処理
//=====================================================
void CParticle::Save(void)
{
	FILE *pFile = NULL;

	pFile = fopen("data\\particle.txt", "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# パーティクルスクリプトファイル[particle.txt]\n");
		fprintf(pFile, "# Author：Momoya Takayama\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "SCRIPT\n\n");

		fprintf(pFile, "    LIFE_PARTICLE = %d\n", m_nLife);
		fprintf(pFile, "    NUM_EFFECT = %d\n", m_nNumEffect);
		fprintf(pFile, "    LIFE_EFFECT = %d\n", m_nLifeEffect);
		fprintf(pFile, "    RADIUS_EFFECT = %.2f\n", m_fRadiusEffect);
		fprintf(pFile, "    SPEED_EFFECT = %.2f\n", m_fSpeed);
		fprintf(pFile, "    GRAVITY = %.2f\n", m_fGravity);
		fprintf(pFile, "    DECREASE = %.2f\n", m_fDecrease);
		fprintf(pFile, "    RANGEROT = %.2f\n", m_fRangeRot);
		fprintf(pFile, "    IS_ADD = %d\n", (int)m_bAdd);
		fprintf(pFile, "    IS_ROT = %d\n", (int)m_bRot);
		fprintf(pFile, "    IS_TURN = %d\n", (int)m_bTurn);
		fprintf(pFile, "    COLOR = %.2f %.2f %.2f %.2f\n", m_col.r,m_col.g,m_col.b,m_col.a);

		fprintf(pFile, "\nEND_SCRIPT\n");

		fclose(pFile);
	}
}

//=====================================================
// 入力処理
//=====================================================
void CParticle::Input(void)
{
	int nRate = 1;

	// 情報取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputMouse *pMouse = CManager::GetMouse();
	CDebugProc *pDebugProc = CManager::GetDebugProc();

	pDebugProc->Print("//=========================\n");
	pDebugProc->Print("// パラメーター\n");
	pDebugProc->Print("//=========================\n");
	pDebugProc->Print("[%d]パーティクルの寿命:[QA]\n", m_nLife);
	pDebugProc->Print("[%d]エフェクトの数:[WS]\n", m_nNumEffect);
	pDebugProc->Print("[%d]エフェクトの寿命:[ED]\n", m_nLifeEffect);
	pDebugProc->Print("[%f]エフェクトの半径:[RF]\n", m_fRadiusEffect);
	pDebugProc->Print("[%f]エフェクトの速度:[↑↓]\n", m_fSpeed);
	pDebugProc->Print("[%f]重力:[IK]\n", m_fGravity);
	pDebugProc->Print("[%f]半径減衰量:[56]\n", m_fDecrease);
	pDebugProc->Print("[%f]向きのランダム幅:[OL]\n", m_fRangeRot);
	pDebugProc->Print("[%d]加算合成するかどうか:[1]\n", m_bAdd);
	pDebugProc->Print("[%d]ループするかどうか:[2]\n", m_bLoop);
	pDebugProc->Print("[%d]向きを反映するかどうか:[4]\n", m_bRot);
	pDebugProc->Print("[%d]反転するかどうか:[7]\n", m_bTurn);
	pDebugProc->Print("[%f,%f,%f]色:[TG,YH,UJ]\n", m_col.r, m_col.g, m_col.b);
	pDebugProc->Print("[%f,%f,%f]向き:[ZM,CV,BN]\n", m_rot.x, m_rot.y, m_rot.z);
	pDebugProc->Print("[%d]総数\n",m_nNumAll);

	pDebugProc->Print("\n");

	pDebugProc->Print("//=========================\n");
	pDebugProc->Print("// 操作方法\n");
	pDebugProc->Print("//=========================\n");
	pDebugProc->Print("[F2] 保存\n");
	pDebugProc->Print("[F3] 読込\n");
	pDebugProc->Print("[RMB] 視点操作\n");
	pDebugProc->Print("[LMB,ENTER] パーティクル生成\n");

	if (pMouse != NULL)
	{
		if (pKeyboard != NULL)
		{
			if (pKeyboard->GetTrigger(DIK_RETURN) || pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
			{// パーティクル生成
				if (m_bLoop == false)
				{// ループ中は生成できない
					Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), CParticle::TYPE_NONE);
				}
			}
		}

		if (pMouse->GetPress(CInputMouse::BUTTON_RMB))
		{// 視点操作時
			pDebugProc->Print("[WASD] 移動\n");
			pDebugProc->Print("[QE] 上下移動\n");

			return;
		}
	}

	if (pKeyboard != NULL)
	{
		if (pKeyboard->GetPress(DIK_LSHIFT))
		{
			nRate *= 2;
		}

		if (pKeyboard->GetTrigger(DIK_Q))
		{// パーティクル寿命加算
			m_nLife += ADD_INT * nRate;
		}
		else if (pKeyboard->GetTrigger(DIK_A))
		{// パーティクル寿命減算
			m_nLife -= ADD_INT * nRate;
		}

		if (pKeyboard->GetTrigger(DIK_W))
		{// エフェクト数加算
			m_nNumEffect += ADD_INT * nRate;
		}
		else if (pKeyboard->GetTrigger(DIK_S))
		{// エフェクト数減算
			m_nNumEffect -= ADD_INT * nRate;
		}

		if (pKeyboard->GetTrigger(DIK_E))
		{// エフェクト寿命加算
			m_nLifeEffect += ADD_INT * nRate;
		}
		else if (pKeyboard->GetTrigger(DIK_D))
		{// エフェクト寿命減算
			m_nLifeEffect -= ADD_INT * nRate;
		}

		if (pKeyboard->GetPress(DIK_R))
		{// エフェクト半径加算
			m_fRadiusEffect += ADD_SPEED * nRate;
		}
		else if (pKeyboard->GetPress(DIK_F))
		{// エフェクト半径減算
			m_fRadiusEffect -= ADD_SPEED * nRate;
		}

		if (pKeyboard->GetPress(DIK_T))
		{// 赤加算
			m_col.r += ADD_COLOR * nRate;
		}
		else if (pKeyboard->GetPress(DIK_G))
		{// 赤減算
			m_col.r -= ADD_COLOR * nRate;
		}

		if (pKeyboard->GetPress(DIK_Y))
		{// 緑加算
			m_col.g += ADD_COLOR * nRate;
		}
		else if (pKeyboard->GetPress(DIK_H))
		{// 緑減算
			m_col.g -= ADD_COLOR * nRate;
		}

		if (pKeyboard->GetPress(DIK_U))
		{// 青加算
			m_col.b += ADD_COLOR * nRate;
		}
		else if (pKeyboard->GetPress(DIK_J))
		{// 青減算
			m_col.b -= ADD_COLOR * nRate;
		}

		if (pKeyboard->GetPress(DIK_I))
		{// 重力加算
			m_fGravity += ADD_COLOR * nRate;
		}
		else if (pKeyboard->GetPress(DIK_K))
		{// 重力減算
			m_fGravity -= ADD_COLOR * nRate;
		}

		if (pKeyboard->GetPress(DIK_O))
		{// 向きランダム幅加算
			m_fRangeRot += ADD_SPEED * nRate;
		}
		else if (pKeyboard->GetPress(DIK_L))
		{// 向きランダム幅減算
			m_fRangeRot -= ADD_SPEED * nRate;
		}

		if (pKeyboard->GetPress(DIK_UP))
		{// スピード加算
			m_fSpeed += ADD_SPEED * nRate;
		}
		else if (pKeyboard->GetPress(DIK_DOWN))
		{// スピード減算
			m_fSpeed -= ADD_SPEED * nRate;
		}

		if (pKeyboard->GetPress(DIK_5))
		{// 減衰量加算
			m_fDecrease += ADD_SPEED * nRate;
		}
		else if (pKeyboard->GetPress(DIK_6))
		{// 減衰量減算
			m_fDecrease -= ADD_SPEED * nRate;
		}

		if (pKeyboard->GetTrigger(DIK_1))
		{// 加算合成切り替え
			m_bAdd = m_bAdd ? false : true;
		}

		if (pKeyboard->GetTrigger(DIK_2))
		{// ループ切り替え
			m_bLoop = m_bLoop ? false : true;

			if (m_bLoop)
			{
				Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), CParticle::TYPE_NONE);
			}
		}

		if (pKeyboard->GetTrigger(DIK_4))
		{// 向き反映切り替え
			m_bRot = m_bRot ? false : true;
		}

		if (pKeyboard->GetTrigger(DIK_7))
		{// 加算合成切り替え
			m_bTurn = m_bTurn ? false : true;
		}

		// 回転=====================================
		if (pKeyboard->GetPress(DIK_Z))
		{// X軸回転
			m_rot.x += ADD_SPEED * nRate;
		}
		else if (pKeyboard->GetPress(DIK_X))
		{
			m_rot.x -= ADD_SPEED * nRate;
		}

		if (pKeyboard->GetPress(DIK_C))
		{// Y軸回転
			m_rot.y += ADD_SPEED * nRate;
		}
		else if (pKeyboard->GetPress(DIK_V))
		{
			m_rot.y -= ADD_SPEED * nRate;
		}

		if (pKeyboard->GetPress(DIK_B))
		{// Z軸回転
			m_rot.z += ADD_SPEED * nRate;
		}
		else if (pKeyboard->GetPress(DIK_N))
		{
			m_rot.z -= ADD_SPEED * nRate;
		}

		if (pKeyboard->GetTrigger(DIK_F2))
		{// 保存
			Save();
		}
		else if (pKeyboard->GetTrigger(DIK_F3))
		{// 読込
			Load();
		}
	}
}