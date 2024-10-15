//*****************************************************
//
// モーションの処理[motion.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"
#include "model.h"
#include <stdio.h>
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "particle.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_STRING	(256)	// 文字列の最大数
#define ROLL_SPEED	(0.01f)	// パーツが回る速度
#define MOVE_SPEED	(0.1f)	// パーツが動くスピード
#define NUM_DISP	(10)	// 情報を表示するパーツ数

//=====================================================
// コンストラクタ
//=====================================================
CMotion::CMotion()
{
	ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
	m_bLoopMotion = false;
	m_motionType = 0;
	m_motionTypeOld = 0;
	m_nCounterMotion = 0;
	m_nKey = 0;
	m_nNumKey = 0;
	m_nNumMotion = 0;
	m_nNumParts = 0;
	m_nIdxParts = 0;
	m_bMotion = false;
	m_bSetUp = false;

	for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
	{
		m_aMotionInfo[nCntMotion].pParticle = NULL;
	}
}

//=====================================================
// デストラクタ
//=====================================================
CMotion::~CMotion()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMotion::Uninit(void)
{
	for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
	{
		if (m_aMotionInfo[nCntMotion].pParticle != NULL)
		{
			delete m_aMotionInfo[nCntMotion].pParticle;
			m_aMotionInfo[nCntMotion].pParticle = NULL;
		}
	}

	Release();
}

//=====================================================
// リセット処理
//=====================================================
void CMotion::Reset(void)
{
	for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
	{
		m_apParts[nCntParts]->m_pParts->SetRot(D3DXVECTOR3(0.0f,0.0f,0.0f));
	}
}

//=====================================================
// 更新処理
//=====================================================
void CMotion::Update(void)
{
//	// 入力情報入手
//	CInputMouse *pMouse = CManager::GetMouse();
//	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
//
//	if (m_bSetUp)
//	{
//		m_bMotion = false;
//	}
//
//	if (m_bMotion)
//	{
//		Motion();
//	}
//
//	if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == false)
//	{// 入力処理
//		Input();
//	}
//
//	if (pKeyboard->GetTrigger(DIK_F7))
//	{// 保存
//		SaveMotion();
//	}
//
//	DrawMotionState();
}

//=====================================================
// 入力
//=====================================================
void CMotion::Input(void)
{
	// 変数宣言
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	// 入力情報入手
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_F2))
	{// モーションするかの切り替え
		m_bMotion = m_bMotion ? false : true;
	}

	if (pKeyboard->GetTrigger(DIK_F3))
	{// セットアップするかの切り替え
		if (m_bSetUp)
		{
			m_bSetUp = false;
		}
		else
		{
			Reset();
			m_bSetUp = true;
		}
	}

	// 選択============================================
	if (pKeyboard->GetTrigger(DIK_UP))
	{// パーツの選択
		m_apParts[m_nIdxParts]->m_pParts->SetCurrent(false);

		m_nIdxParts = (m_nIdxParts + m_nNumParts - 1) % m_nNumParts;

		m_apParts[m_nIdxParts]->m_pParts->SetCurrent(true);
	}
	else if (pKeyboard->GetTrigger(DIK_DOWN))
	{
		m_apParts[m_nIdxParts]->m_pParts->SetCurrent(false);

		m_nIdxParts = (m_nIdxParts + 1) % m_nNumParts;

		m_apParts[m_nIdxParts]->m_pParts->SetCurrent(true);
	}

	if (m_bSetUp)
	{// セットアップかどうか
		if (pKeyboard->GetPress(DIK_W))
		{// 前進
			pos.z += MOVE_SPEED;
		}
		if (pKeyboard->GetPress(DIK_A))
		{// 左
			pos.x -= MOVE_SPEED;
		}
		if (pKeyboard->GetPress(DIK_D))
		{// 右
			pos.x += MOVE_SPEED;
		}
		if (pKeyboard->GetPress(DIK_S))
		{// 手前
			pos.z -= MOVE_SPEED;
		}

		if (pKeyboard->GetPress(DIK_E))
		{// 上
			pos.y += MOVE_SPEED;
		}
		if (pKeyboard->GetPress(DIK_Q))
		{// 下
			pos.y -= MOVE_SPEED;
		}

		if (pKeyboard->GetPress(DIK_LSHIFT))
		{// 下
			pos *= 5;
		}

		// 位置設定
		m_apParts[m_nIdxParts]->m_pParts->SetPosOrg(m_apParts[m_nIdxParts]->m_pParts->GetPosOrg() + pos);
		m_apParts[m_nIdxParts]->m_pParts->SetPosition(m_apParts[m_nIdxParts]->m_pParts->GetPosOrg());
	}
	else
	{// モーションモードの入力
		if (pKeyboard->GetTrigger(DIK_RIGHT))
		{// モーションの選択
			m_motionType = (m_motionType + 1) % m_nNumMotion;
			SetMotion(m_motionType);

			if (m_bMotion == false)
			{
				// ポーズ初期設定
				SetPose();
			}
		}
		else if (pKeyboard->GetTrigger(DIK_LEFT))
		{
			m_motionType = (m_motionType + m_nNumMotion - 1) % m_nNumMotion;
			SetMotion(m_motionType);

			if (m_bMotion == false)
			{
				// ポーズ初期設定
				SetPose();
			}
		}

		if (pKeyboard->GetTrigger(DIK_2))
		{// キーの選択
			m_nKey = (m_nKey + 1) % m_aMotionInfo[m_motionType].nNumKey;

			// ポーズ初期設定
			SetPose();
		}
		else if (pKeyboard->GetTrigger(DIK_1))
		{
			m_nKey = (m_nKey + m_aMotionInfo[m_motionType].nNumKey - 1) % m_aMotionInfo[m_motionType].nNumKey;

			// ポーズ初期設定
			SetPose();

		}
		// 選択============================================

		// パーツの動き============================================
		if (pKeyboard->GetPress(DIK_T))
		{// X軸回転
			rot.x += ROLL_SPEED;

			// パーツのトランスフォームを一時保存
			SetTransform();
		}
		else if (pKeyboard->GetPress(DIK_G))
		{
			rot.x -= ROLL_SPEED;

			// パーツのトランスフォームを一時保存
			SetTransform();
		}

		if (pKeyboard->GetPress(DIK_Y))
		{// Y軸回転
			rot.y += ROLL_SPEED;

			// パーツのトランスフォームを一時保存
			SetTransform();
		}
		else if (pKeyboard->GetPress(DIK_H))
		{
			rot.y -= ROLL_SPEED;

			// パーツのトランスフォームを一時保存
			SetTransform();
		}

		if (pKeyboard->GetPress(DIK_U))
		{// Z軸回転
			rot.z += ROLL_SPEED;

			// パーツのトランスフォームを一時保存
			SetTransform();
		}
		else if (pKeyboard->GetPress(DIK_J))
		{
			rot.z -= ROLL_SPEED;

			// パーツのトランスフォームを一時保存
			SetTransform();
		}
		// パーツの動き============================================
	}

	// 向き設定
	m_apParts[m_nIdxParts]->m_pParts->SetRot(m_apParts[m_nIdxParts]->m_pParts->GetRot() + rot);
}

//=====================================================
// モーション更新
//=====================================================
void CMotion::Motion(void)
{
	//変数宣言
	int nNextKey;

	// パーツのトランスフォーム
	D3DXVECTOR3 pos, rot;

	if (m_aMotionInfo[m_motionType].pParticle != NULL)
	{
		for (int nCntParticle = 0; nCntParticle < m_aMotionInfo[m_motionType].nNumParticle; nCntParticle++)
		{// 全てのパーティクルを確認
			if (m_nKey == m_aMotionInfo[m_motionType].pParticle[nCntParticle].nKey &&
				m_nCounterMotion == m_aMotionInfo[m_motionType].pParticle[nCntParticle].nFrame)
			{// パーティクル生成
			 // 親パーツの位置取得
				D3DXMATRIX *pMtx = m_apParts[m_aMotionInfo[m_motionType].pParticle[nCntParticle].nIdxParent]->m_pParts->GetMatrix();
				D3DXVECTOR3 pos = D3DXVECTOR3(pMtx->_41, pMtx->_42, pMtx->_43) + m_aMotionInfo[m_motionType].pParticle[nCntParticle].offset;

				CParticle::Create(pos, (CParticle::TYPE)m_aMotionInfo[m_motionType].pParticle[nCntParticle].nType);
			}
		}
	}

	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_nKey >= m_aMotionInfo[m_motionType].nNumKey - 1)
		{
			break;
		}

		// パーツのトランスフォーム取得
		pos = m_apParts[nCntParts]->m_pParts->GetPosOrg();
		rot = m_apParts[nCntParts]->m_pParts->GetRot();

		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey - 1)
		{// 次のキー設定
			nNextKey = m_nKey + 1;
		}
		else
		{
			nNextKey = m_aMotionInfo[m_motionType].nNumKey - 1;
		}

		//前回と次のキーの差分取得
		float DiffPosX = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosX -
			m_aKeyOld[nCntParts].fPosX;

		float DiffPosY = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosY -
			m_aKeyOld[nCntParts].fPosY;

		float DiffPosZ = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosZ -
			m_aKeyOld[nCntParts].fPosZ;

		float DiffRotX = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotX -
			m_aKeyOld[nCntParts].fRotX;

		float DiffRotY = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotY -
			m_aKeyOld[nCntParts].fRotY;

		float DiffRotZ = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotZ -
			m_aKeyOld[nCntParts].fRotZ;

		//目的の値=======================================================================================================
		float DestPosX = pos.x + m_aKeyOld[nCntParts].fPosX +
			DiffPosX * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestPosY = pos.y + m_aKeyOld[nCntParts].fPosY +
			DiffPosY * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestPosZ = pos.z + m_aKeyOld[nCntParts].fPosZ +
			DiffPosZ * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestRotX = m_aKeyOld[nCntParts].fRotX +
			DiffRotX * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestRotY = m_aKeyOld[nCntParts].fRotY +
			DiffRotY * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		float DestRotZ = m_aKeyOld[nCntParts].fRotZ +
			DiffRotZ * (float)(1.0f / (float)m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame) * m_nCounterMotion;

		//パーツの向き・位置設定
		m_apParts[nCntParts]->m_pParts->SetPosition(D3DXVECTOR3(DestPosX, DestPosY, DestPosZ));

		m_apParts[nCntParts]->m_pParts->SetRot(D3DXVECTOR3(DestRotX, DestRotY, DestRotZ));
	}

	m_nCounterMotion++;

	if (m_nKey >= m_aMotionInfo[m_motionType].nNumKey - 1)
	{// モーションが終了していたら
		m_motionTypeOld = m_motionType;
		SetMotion(m_motionType);
	}

	if (m_nCounterMotion > m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame)
	{//キーのフレーム数に達したら
		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey)
		{
			m_nKey++;

			m_nCounterMotion = 0;

			SetKeyOld();
		}
	}
}

//=====================================================
// パーツのトランスフォーム設定
//=====================================================
void CMotion::SetTransform(void)
{
	// 向きの設定
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[m_nIdxParts].fRotX = m_apParts[m_nIdxParts]->m_pParts->GetRot().x;
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[m_nIdxParts].fRotY = m_apParts[m_nIdxParts]->m_pParts->GetRot().y;
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[m_nIdxParts].fRotZ = m_apParts[m_nIdxParts]->m_pParts->GetRot().z;

	// 位置の設定
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[m_nIdxParts].fPosX = m_apParts[m_nIdxParts]->m_pParts->GetPosition().x - m_apParts[m_nIdxParts]->m_pParts->GetPosOrg().x;
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[m_nIdxParts].fPosY = m_apParts[m_nIdxParts]->m_pParts->GetPosition().y - m_apParts[m_nIdxParts]->m_pParts->GetPosOrg().y;
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[m_nIdxParts].fPosZ = m_apParts[m_nIdxParts]->m_pParts->GetPosition().z - m_apParts[m_nIdxParts]->m_pParts->GetPosOrg().z;
}

//=====================================================
// 情報描画
//=====================================================
void CMotion::DrawMotionState(void)
{
//	int nIdx = m_nIdxParts;
//	D3DXVECTOR3 pos, rot;
//
//	CManager::GetDebugProc()->Print("[F2]モーション再生");
//	
//	CManager::GetDebugProc()->Print("                                                 ");
//
//	if (m_bSetUp)
//	{
//		CManager::GetDebugProc()->Print("モード[セットアップ]\n");
//	}
//	else
//	{
//		CManager::GetDebugProc()->Print("モード[モーション]\n");
//	}
//
//	CManager::GetDebugProc()->Print("[F3]セットアップモードに移行\n");
//	CManager::GetDebugProc()->Print("[F7]保存[data\\motion.txt]\n");
//
//	if (m_bSetUp)
//	{
//		CManager::GetDebugProc()->Print("[WASD]パーツ水平移動\n");
//		CManager::GetDebugProc()->Print("[QE]パーツ上下移動\n");
//		CManager::GetDebugProc()->Print("[LSHIFT]加速\n");
//	}
//
//	CManager::GetDebugProc()->Print("カメラ操作はUNREALと同じ\n");
//
//	CManager::GetDebugProc()->Print("モーション再生[%d]\n", m_bMotion);
//	CManager::GetDebugProc()->Print("セットアップ[%d]\n", m_bSetUp);
//	CManager::GetDebugProc()->Print("選択パーツ[%d / %d]\n", m_nIdxParts, m_nNumParts - 1);
//	CManager::GetDebugProc()->Print("選択モーション[%d / %d]\n", m_motionType, m_nNumMotion - 1);
//	CManager::GetDebugProc()->Print("選択キー[%d / %d]\n", m_nKey, m_aMotionInfo[m_motionType].nNumKey - 1);
//
//	CManager::GetDebugProc()->Print("//===============================\n");
//	CManager::GetDebugProc()->Print("// パーツ情報\n");
//	CManager::GetDebugProc()->Print("//===============================\n");
//
//	for (int nCntModel = 0;nCntModel < NUM_DISP;nCntModel++)
//	{// モデル数分の情報表示
//		pos = m_apParts[nIdx]->m_pParts->GetPosition();
//		rot = m_apParts[nIdx]->m_pParts->GetRot();
//
//		if (nCntModel == 0)
//		{
//			CManager::GetDebugProc()->Print("  ");
//		}
//		CManager::GetDebugProc()->Print("[%d]POS %f,%f,%f\n    ROT %f,%f,%f\n",nIdx,pos.x,pos.y,pos.z,rot.x,rot.y,rot.z);
//
//		nIdx = (nIdx + 1) % m_nNumParts;
//	}
}

//=====================================================
//モーション設定
//=====================================================
void CMotion::SetMotion(int nMotionType)
{
	// 終了するモーションを保存
	m_motionTypeOld = m_motionType;
	SetKeyOld();

	// モーション情報の設定
	m_motionType = nMotionType;
	m_nKey = 0;
	m_nCounterMotion = 0;
}

//=====================================================
// ポーズ設定
//=====================================================
void CMotion::SetPose(void)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	for (int nCntPart = 0; nCntPart < m_nNumParts; nCntPart++)
	{
		// 向きの取得
		rot.x = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fRotX;
		rot.y = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fRotY;
		rot.z = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fRotZ;
		// 位置の取得
		pos.x = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fPosX + m_apParts[nCntPart]->m_pParts->GetPosOrg().x;
		pos.y = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fPosY + m_apParts[nCntPart]->m_pParts->GetPosOrg().y;
		pos.z = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fPosZ + m_apParts[nCntPart]->m_pParts->GetPosOrg().z;

		m_apParts[nCntPart]->m_pParts->SetRot(rot);
		m_apParts[nCntPart]->m_pParts->SetPosition(pos);
	}
}

//=====================================================
// 終了するキーの情報保存
//=====================================================
void CMotion::SetKeyOld(void)
{
	for (int nCntPart = 0;nCntPart < m_nNumParts;nCntPart++)
	{
		m_aKeyOld[nCntPart].fPosX = m_apParts[nCntPart]->m_pParts->GetPosition().x - m_apParts[nCntPart]->m_pParts->GetPosOrg().x;
		m_aKeyOld[nCntPart].fPosY = m_apParts[nCntPart]->m_pParts->GetPosition().y - m_apParts[nCntPart]->m_pParts->GetPosOrg().y;
		m_aKeyOld[nCntPart].fPosZ = m_apParts[nCntPart]->m_pParts->GetPosition().z - m_apParts[nCntPart]->m_pParts->GetPosOrg().z;

		m_aKeyOld[nCntPart].fRotX = m_apParts[nCntPart]->m_pParts->GetRot().x;
		m_aKeyOld[nCntPart].fRotY = m_apParts[nCntPart]->m_pParts->GetRot().y;
		m_aKeyOld[nCntPart].fRotZ = m_apParts[nCntPart]->m_pParts->GetRot().z;
	}
}

//=====================================================
// マトリックスをかけ合わせる処理
//=====================================================
void CMotion::MultiplyMtx(void)
{
	// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//変数宣言
	D3DXMATRIX mtxRotModel, mtxTransModel;
	D3DXMATRIX *pMtxParent;
	D3DXMATRIX *pMtx;

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
	{
		// マトリックスの取得
		pMtx = m_apParts[nCntParts]->m_pParts->GetMatrix();

		//ワールドマトリックス初期化
		D3DXMatrixIdentity(pMtx);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			m_apParts[nCntParts]->m_pParts->GetRot().y, m_apParts[nCntParts]->m_pParts->GetRot().x, m_apParts[nCntParts]->m_pParts->GetRot().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxRotModel);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			m_apParts[nCntParts]->m_pParts->GetPosition().x, m_apParts[nCntParts]->m_pParts->GetPosition().y, m_apParts[nCntParts]->m_pParts->GetPosition().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxTransModel);
		
		if (m_apParts[nCntParts]->nIdxParent != -1)
		{//親パーツがある場合
			// 親マトリックスの取得
			pMtxParent = m_apParts[m_apParts[nCntParts]->nIdxParent]->m_pParts->GetMatrix();
		}
		else
		{
			pMtxParent = &m_mtxWorld;
		}

		//親パーツとパーツのワールドマトリックスをかけ合わせる
		D3DXMatrixMultiply(pMtx, pMtx, pMtxParent);

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, pMtx);

		m_apParts[nCntParts]->m_pParts->Draw();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CMotion::Draw(void)
{
	// マトリックスをかけ合わせる処理
	MultiplyMtx();
}

//=====================================================
// 読込処理
//=====================================================
void CMotion::Load(char *pPath)
{
	//変数宣言
	char cTemp[MAX_STRING];
	int nCntModel = 0;

	//ファイルから読み込む
	FILE *pFile = fopen(pPath, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		while (1)
		{
			//文字読み込み
			fscanf(pFile, "%s", &cTemp[0]);

			//ファイル名読み込み=========================================
			if (strcmp(cTemp, "NUM_MODEL") == 0)
			{
				//"="読み込み
				fscanf(pFile, "%s", &cTemp[0]);

				//モデル数読み込み
				fscanf(pFile, "%d", &m_nNumParts);

				for (int nCntFile = 0; nCntFile < m_nNumParts;)
				{//ファイル名読み込み

					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "MODEL_FILENAME") == 0)
					{//ファイル名読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						char aPath[MAX_STRING];

						// モデルパス読込
						fscanf(pFile, "%s", &aPath[0]);

						m_apParts[nCntFile] = new Parts;

						m_apParts[nCntFile]->m_pParts = CParts::Create();

						int nIdx = CModel::Load(&aPath[0]);

						// モデル読込
						m_apParts[nCntFile]->m_pParts->SetIdxModel(nIdx);
						m_apParts[nCntFile]->m_pParts->BindModel(m_apParts[nCntFile]->m_pParts->GetIdxModel());

						nCntFile++;
					}
				}
			}
			//===========================================================

			//キャラクター設定===========================================
			if (strcmp(cTemp, "CHARACTERSET") == 0)
			{//キャラクタースタート
				nCntModel = 0;

				while (strcmp(cTemp, "END_CHARACTERSET") != 0)
				{//終わりまでキャラクター設定
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PARTSSET") == 0)
					{//パーツスタート
						while (strcmp(cTemp, "END_PARTSSET") != 0)
						{//終わりまでパーツ設定
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "INDEX") == 0)
							{//番号読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdx);
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{//親番号読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdxParent);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								D3DXVECTOR3 pos;

								fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									fscanf(pFile, "%f", &pos[nCntPos]);
								}

								m_apParts[nCntModel]->m_pParts->SetPosition(pos);

								m_apParts[nCntModel]->m_pParts->SetPosOrg(pos);
							}

							if (strcmp(cTemp, "ROT") == 0)
							{//向き読み込み
								D3DXVECTOR3 rot;

								fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntRot = 0; nCntRot < 3; nCntRot++)
								{
									fscanf(pFile, "%f", &rot[nCntRot]);
								}

								m_apParts[nCntModel]->m_pParts->SetRot(rot);
							}

						}//END_PART

						nCntModel++;
					}
				}//END_CHAR
			}
			//===========================================================

			//変数宣言
			int nCntKey = 0;
			int nCntPart = 0;
			int nCntParticle = 0;

			//モーション設定===========================================
			if (strcmp(cTemp, "MOTIONSET") == 0)
			{//モーションスタート
				while (strcmp(cTemp, "END_MOTIONSET") != 0)
				{//終わりまでキャラクター設定

					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "LOOP") == 0)
					{//ループ判断
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%d", (int*)&m_aMotionInfo[m_nNumMotion].bLoop);
					}

					if (strcmp(cTemp, "NUM_KEY") == 0)
					{//キー数判断
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumKey);
					}

					if (strcmp(cTemp, "NUM_PARTICLE") == 0)
					{//パーティクル数判断
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumParticle);

						if (m_aMotionInfo[m_nNumMotion].nNumParticle != 0)
						{
							// パーティクル情報を生成
							m_aMotionInfo[m_nNumMotion].pParticle = new PARTICLE_INFO[m_aMotionInfo[m_nNumMotion].nNumParticle];

							// パーティクル情報初期化
							ZeroMemory(m_aMotionInfo[m_nNumMotion].pParticle, sizeof(PARTICLE_INFO) * m_aMotionInfo[m_nNumMotion].nNumParticle);
						}
					}

					if (strcmp(cTemp, "PARTICLESET") == 0 && m_aMotionInfo[m_nNumMotion].pParticle != 0)
					{// パーティクル情報設定
						while (strcmp(cTemp, "END_PARTICLESET") != 0)
						{//終わりまでパーティクル設定
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "KEY") == 0)
							{// 再生キー取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nKey);
							}

							if (strcmp(cTemp, "FRAME") == 0)
							{// 再生フレーム取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nFrame);
							}

							if (strcmp(cTemp, "TYPE") == 0)
							{// 種類取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nType);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								D3DXVECTOR3 pos;

								fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].offset[nCntPos]);
								}
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{// 親パーツ番号取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pParticle[nCntParticle].nIdxParent);
							}
						}

						nCntParticle++;
					}

					if (strcmp(cTemp, "KEYSET") == 0)
					{//キースタート
						while (strcmp(cTemp, "END_KEYSET") != 0)
						{//終わりまでキー設定

							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "FRAME") == 0)
							{//フレーム数取得
								fscanf(pFile, "%s", &cTemp[0]);

								fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].aKeyInfo[nCntKey].nFrame);
							}

							if (strcmp(cTemp, "KEY") == 0)
							{//フレーム数取得
								while (strcmp(cTemp, "END_KEY") != 0)
								{//終わりまでキー設定
									fscanf(pFile, "%s", &cTemp[0]);

									if (strcmp(cTemp, "POS") == 0)
									{//位置取得
										fscanf(pFile, "%s", &cTemp[0]);

										fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
										fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
										fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
									}

									if (strcmp(cTemp, "ROT") == 0)
									{//向き取得
										fscanf(pFile, "%s", &cTemp[0]);

										fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
										fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
										fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
									}
								}

								nCntPart++;

							}//KEYif
						}//ENDKEYwhile

						nCntKey++;
						nCntPart = 0;
					}
				}//MOTIONwhile

				m_nNumMotion++;
			}
			//===========================================================



			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}//while閉じ
	}
	else
	{
	}
}

//=====================================================
// 生成処理
//=====================================================
CMotion *CMotion::Create(char *pPath)
{
	CMotion *pCMotion = NULL;

	if (pCMotion == NULL)
	{
		// インスタンス生成
		pCMotion = new CMotion;

		// 読込処理
		pCMotion->Load(pPath);

		// 初期化処理
		pCMotion->Init();
	}

	return pCMotion;
}

//=====================================================
// 保存処理
//=====================================================
void CMotion::SaveMotion(void)
{
	FILE *pFile = NULL;

	pFile = fopen("data\\motion.txt", "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# モーションスクリプトファイル[motion.txt]\n");
		fprintf(pFile, "# Author：Momoya Takayama\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "SCRIPT\n\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# モデル数\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "NUM_MODEL = %d\n\n", m_nNumParts);

		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# モデルファイル名\n");
		fprintf(pFile, "#====================================================================\n");

		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// ファイル名書き出し
			fprintf(pFile, "MODEL_FILENAME = %s\n", CModel::GetModelPath(nCntModel));
		}

		fprintf(pFile, "\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# キャラクター情報\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "CHARACTERSET\n");
		fprintf(pFile, "    NUM_PARTS = %d\n\n", m_nNumParts);

		D3DXVECTOR3 posOrg,pos,rot;

		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// セットアップ情報書き出し
		posOrg = m_apParts[nCntModel]->m_pParts->GetPosOrg();
		fprintf(pFile, "    PARTSSET\n");
		fprintf(pFile, "        INDEX = %d\n", nCntModel);
		fprintf(pFile, "        PARENT = %d\n", m_apParts[nCntModel]->nIdxParent);
		fprintf(pFile, "        POS = %.1f %.1f %.1f\n", posOrg.x, posOrg.y, posOrg.z);
		fprintf(pFile, "        ROT = 0.0 0.0 0.0\n");
		fprintf(pFile, "    END_PARTSSET\n");
		}
		fprintf(pFile, "END_CHARACTERSET\n\n");

		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# モーション情報\n");
		fprintf(pFile, "#====================================================================\n");
		for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
		{// モーション情報書き出し
		fprintf(pFile, "#===============================\n");
		fprintf(pFile, "# [%d]番目のモーション\n", nCntMotion);
		fprintf(pFile, "#===============================\n");
		fprintf(pFile, "MOTIONSET\n");
		fprintf(pFile, "    LOOP = %d\n", (int)m_aMotionInfo[nCntMotion].bLoop);
		fprintf(pFile, "    NUM_KEY = %d\n", m_aMotionInfo[nCntMotion].nNumKey);

		if (m_aMotionInfo[nCntMotion].pParticle != NULL)
		{// パーティクルを設定するなら
		fprintf(pFile, "    NUM_PARTICLE = %d\n", m_aMotionInfo[nCntMotion].nNumParticle);
		for (int nCntParticle = 0;nCntParticle < m_aMotionInfo[nCntMotion].nNumParticle;nCntParticle++)
		{
			pos =
			{// 位置保存
				m_aMotionInfo[nCntMotion].pParticle[nCntParticle].offset.x,
				m_aMotionInfo[nCntMotion].pParticle[nCntParticle].offset.y,
				m_aMotionInfo[nCntMotion].pParticle[nCntParticle].offset.z,
			};

			fprintf(pFile, "\n");

		fprintf(pFile, "    PARTICLESET\n");
		fprintf(pFile, "        KEY = %d\n", m_aMotionInfo[nCntMotion].pParticle[nCntParticle].nKey);
		fprintf(pFile, "        FRAME = %d\n", m_aMotionInfo[nCntMotion].pParticle[nCntParticle].nFrame);
		fprintf(pFile, "        TYPE = %d\n", m_aMotionInfo[nCntMotion].pParticle[nCntParticle].nType);
		fprintf(pFile, "        POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
		fprintf(pFile, "        PARENT = %d\n", m_aMotionInfo[nCntMotion].pParticle[nCntParticle].nIdxParent);
		fprintf(pFile, "    END_PARTICLESET\n");
		}
		}

		fprintf(pFile, "\n");


		for (int nCntKey = 0;nCntKey < m_aMotionInfo[nCntMotion].nNumKey;nCntKey++)
		{// キー情報書き出し
		fprintf(pFile, "    KEYSET     #---<< %d / %d >>\n", nCntKey, m_aMotionInfo[nCntMotion].nNumKey);
		fprintf(pFile, "        FRAME = %d\n", m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
		for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
		{// パーツ分のキー情報書き出し
		pos =
		{// パーツの位置保存
			m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosX,
			m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosY,
			m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ
		};
		rot =
		{// パーツの向き保存
			m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotX,
			m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotY,
			m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ
		};
		fprintf(pFile, "        KEY # -----[%d]----\n", nCntParts);
		fprintf(pFile, "            POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
		fprintf(pFile, "            ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);
		fprintf(pFile, "        END_KEY\n");
		}
		fprintf(pFile, "    END_KEYSET\n");
		}
		fprintf(pFile, "END_MOTIONSET\n");
		}

		fprintf(pFile, "END_SCRIPT\n");
	}

	fclose(pFile);
}