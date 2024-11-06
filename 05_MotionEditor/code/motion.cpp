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
#include "particle.h"
#include "universal.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include <string.h>
#include "effect3D.h"
#include "object3D.h"
#include "texture.h"
#include "manipulater.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_STRING	(256)	// 文字列の最大数
#define ROLL_SPEED	(0.01f)	// パーツが回る速度
#define MOVE_SPEED	(0.1f)	// パーツが動くスピード
namespace
{
const float SIZE_ICON = 5.f;   // アイコンのサイズ
const D3DXCOLOR COL_NOT_CURRENT = { 1.0f,1.0f,1.0f,1.0f };
const D3DXCOLOR COL_CURRENT = { 0.0f,1.0f,0.0f,1.0f };
const D3DXCOLOR COL_MOVE = { 1.0f,0.0f,0.0f,1.0f };
}

//=====================================================
// コンストラクタ
//=====================================================
CMotion::CMotion(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
	ZeroMemory(&m_keyInfoTemp, sizeof(m_keyInfoTemp));
	m_bLoopMotion = false;
	m_motionType = 0;
	m_motionTypeOld = 0;
	m_nCounterMotion = 0;
	m_nKey = 0;
	m_nFrame = 0;
	m_nNumKey = 0;
	m_nNumMotion = 0;
	m_nNumParts = 0;
	m_bFinish = false;
	m_bShadow = false;
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_posShadow = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_bInde = false;
    ZeroMemory(&m_aPathSave[0], sizeof(m_apParts));
    m_pManipulater = nullptr;
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
	SetMotion(0);
	InitPose(0);

    strcpy(&m_aPathSave[0], "data\\motion.txt");

    CreateManipulater(0);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMotion::Uninit(void)
{
	for (int nCntMotion = 0; nCntMotion < motion::MAX_MOTION; nCntMotion++)
	{// パーティクル情報の破棄
		if (m_aMotionInfo[nCntMotion].pEvent != nullptr)
		{
			delete m_aMotionInfo[nCntMotion].pEvent;
			m_aMotionInfo[nCntMotion].pEvent = nullptr;
		}
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMotion::Update(void)
{
	// 入力情報入手
	CInputMouse *pMouse = CManager::GetMouse();
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (m_bSetUp)
	{
		m_bMotion = false;
	}

	if (m_bMotion)
	{
		Motion();
	}

    // アイコンをパーツ原点に持っていく処理
    for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
    {
        D3DXMATRIX mtx = *m_apParts[nCntParts]->pParts->GetMatrix();

        D3DXVECTOR3 posParts = { mtx._41,mtx._42,mtx._43 };

        m_mapIcon[nCntParts]->SetPosition(posParts);
    }

    // 入力処理
	Input();

	if (pKeyboard->GetTrigger(DIK_F8))
	{// 保存
		SaveMotion();
	}

    if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == false)
    {
        ImGui::InputText("save path", &m_aPathSave[0], sizeof(m_aPathSave));
    }

	DrawMotionState();
}

//=====================================================
// 入力
//=====================================================
void CMotion::Input(void)
{
    // ブレンド用のキーにならないように補正
    if (m_bMotion == false && m_nKey == -1)
    {
        m_nKey = 0;
    }

    D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
    D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

    // 入力情報入手
    CInputKeyboard *pKeyboard = CManager::GetKeyboard();

    if (pKeyboard->GetTrigger(DIK_F2))
    {// モーションするかの切り替え
        m_bMotion = m_bMotion ? false : true;

        if (m_bMotion)
        {// ポーズのリセット
            InitPose(m_motionType);
        }
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

    if (m_bSetUp)
    {// セットアップモード
        pos = m_apParts[m_nIdxParts]->pParts->GetPosOrg();

        ImGui::DragFloat("posOrg.x", &pos.x, 0.1f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat("posOrg.y", &pos.y, 0.1f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat("posOrg.z", &pos.z, 0.1f, -FLT_MAX, FLT_MAX);

        // 位置設定
        m_apParts[m_nIdxParts]->pParts->SetPosOrg(pos);
        m_apParts[m_nIdxParts]->pParts->SetPosition(pos);
    }
    else
    {// モーションモード

        if (pKeyboard->GetTrigger(DIK_2))
        {// キーの選択
            m_nKey = (m_nKey + 1) % m_aMotionInfo[m_motionType].nNumKey;

            // ポーズ初期設定
            SetPose();

            m_nFrame = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;
        }
        else if (pKeyboard->GetTrigger(DIK_1))
        {
            m_nKey = (m_nKey + m_aMotionInfo[m_motionType].nNumKey - 1) % m_aMotionInfo[m_motionType].nNumKey;

            // ポーズ初期設定
            SetPose();

            m_nFrame = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;
        }
        // 選択============================================

        // フレームの増減============================================
        if (m_nKey != -1)
        {
            if (pKeyboard->GetRepeat(DIK_O) % 5 == 0 && pKeyboard->GetRepeat(DIK_O) != 0)
            {// フレームを増やす
                m_nFrame++;
                m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame++;
            }
            else if (pKeyboard->GetRepeat(DIK_L) % 5 == 0 && pKeyboard->GetRepeat(DIK_L) != 0)
            {// フレームを減らす
                m_nFrame--;
                m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame--;

                if (m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame < 1)
                {
                    m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame = 1;
                    m_nFrame = 1;
                }
            }
        }
        // フレームの増減============================================

        if (ImGui::SliderInt("Motion", &m_motionType, 0, m_nNumMotion - 1))
        {
            SetMotion(m_motionType);

            if (m_bMotion == false)
            {
                // ポーズ初期設定
                InitPose(m_motionType);
            }
        }

        if (ImGui::TreeNode("Key"))
        {
            // キーの増減============================================
            if (ImGui::InputInt("NumKey", &m_nNumKey))
            {
                universal::LimitValueInt(&m_nNumKey, motion::MAX_KEY, 1);

                m_aMotionInfo[m_motionType].nNumKey = m_nNumKey;
            }
            // キーの増減============================================

            if (ImGui::Button("CopyKey", ImVec2(80.0f, 20.0f)) && m_nKey != -1)
            {// キーコピー
                m_keyInfoTemp = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey];
            }

            if (ImGui::Button("PasteKey", ImVec2(80.0f, 20.0f)) && m_nKey != -1)
            {// キーペースト
                m_aMotionInfo[m_motionType].aKeyInfo[m_nKey] = m_keyInfoTemp;

                // ポーズ初期設定
                SetPose();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Event"))
        {// イベントの設定
            for (int i = 0; i < m_aMotionInfo[m_motionType].nNumEvent; i++)
            {
                if (m_aMotionInfo[m_motionType].pEvent == nullptr)
                    break;

                ImGui::DragInt("Key", &m_aMotionInfo[m_motionType].pEvent[i].nKey, 1, 0, m_aMotionInfo[m_motionType].nNumKey - 1);
                ImGui::DragInt("Frame", &m_aMotionInfo[m_motionType].pEvent[i].nFrame, 1, 0, m_aMotionInfo[m_motionType].aKeyInfo[m_aMotionInfo[m_motionType].pEvent[i].nKey].nFrame - 1);
                ImGui::DragFloat("NumFrame", &m_aMotionInfo[m_motionType].pEvent[i].fNumFrame, 1.0f, 0, FLT_MAX);
                ImGui::DragFloat("Radius", &m_aMotionInfo[m_motionType].pEvent[i].fRadius, 0.1f, 0, FLT_MAX);
                ImGui::DragInt("IdxParent", &m_aMotionInfo[m_motionType].pEvent[i].nIdxParent, 1, 0, m_nNumParts - 1);
                ImGui::DragFloat("Offset.x", &m_aMotionInfo[m_motionType].pEvent[i].offset.x, 0.1f, -FLT_MAX, FLT_MAX);
                ImGui::DragFloat("Offset.y", &m_aMotionInfo[m_motionType].pEvent[i].offset.y, 0.1f, -FLT_MAX, FLT_MAX);
                ImGui::DragFloat("Offset.z", &m_aMotionInfo[m_motionType].pEvent[i].offset.z, 0.1f, -FLT_MAX, FLT_MAX);
            }

            ImGui::TreePop();
        }

        if (ImGui::Button("ResetPose", ImVec2(80.0f, 20.0f)) && m_nKey != -1)
        {// ポーズリセット
            ZeroMemory(&m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey, sizeof(KEY) * motion::MAX_PARTS);

            // ポーズ初期設定
            SetPose();
        }

        if (ImGui::TreeNode("Parts Transform"))
        {
            pos = m_apParts[m_nIdxParts]->pParts->GetPosition();
            rot = m_apParts[m_nIdxParts]->pParts->GetRot();

            if (ImGui::DragFloat("pos.x", &pos.x, 0.1f, -FLT_MAX, FLT_MAX) ||
                ImGui::DragFloat("pos.y", &pos.y, 0.1f, -FLT_MAX, FLT_MAX) ||
                ImGui::DragFloat("pos.z", &pos.z, 0.1f, -FLT_MAX, FLT_MAX))
            {// 移動
                // パーツのトランスフォームを一時保存
                SetTransform(m_nIdxParts);

                m_apParts[m_nIdxParts]->pParts->SetPosition(pos);
            }

            if (ImGui::DragFloat("rot.x", &rot.x, 0.01f, -D3DX_PI, D3DX_PI) ||
                ImGui::DragFloat("rot.y", &rot.y, 0.01f, -D3DX_PI, D3DX_PI) ||
                ImGui::DragFloat("rot.z", &rot.z, 0.01f, -D3DX_PI, D3DX_PI))
            {// 回転
                // パーツのトランスフォームを一時保存
                SetTransform(m_nIdxParts);

                m_apParts[m_nIdxParts]->pParts->SetRot(rot);
            }

            ImGui::TreePop();
        }
    }

    // パーツの選択
    SelectParts();

    if (m_nIdxParts >= 0)
    {
        m_mapIcon[m_nIdxParts]->SetColor(COL_MOVE);
    }
}

//=====================================================
// 更新処理
//=====================================================
void CMotion::Motion(void)
{
	if (m_apParts[0] == nullptr)
	{
		return;
	}

	int nNextKey;

	// パーツのトランスフォーム
	D3DXVECTOR3 pos, rot;

	if (m_aMotionInfo[m_motionType].pEvent != nullptr)
	{
		for (int nCntEvent = 0; nCntEvent < m_aMotionInfo[m_motionType].nNumEvent; nCntEvent++)
		{
			if (m_nKey == m_aMotionInfo[m_motionType].pEvent[nCntEvent].nKey &&
				m_nCounterMotion == m_aMotionInfo[m_motionType].pEvent[nCntEvent].nFrame)
			{// イベントの開始
                m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer = 0.0f;
			}

            if (m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer <= m_aMotionInfo[m_motionType].pEvent[nCntEvent].fNumFrame)
            {// イベントの呼び出し
                Event(&m_aMotionInfo[m_motionType].pEvent[nCntEvent]);

                D3DXMATRIX mtxParent = *GetParts(m_aMotionInfo[m_motionType].pEvent[nCntEvent].nIdxParent)->pParts->GetMatrix();
                D3DXMATRIX mtx;

                universal::SetOffSet(&mtx, mtxParent, m_aMotionInfo[m_motionType].pEvent[nCntEvent].offset);

                D3DXVECTOR3 pos =
                {
                    mtx._41,
                    mtx._42,
                    mtx._43
                };

                float fRadius = m_aMotionInfo[m_motionType].pEvent[nCntEvent].fRadius;

                CEffect3D::Create(pos, fRadius, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

                m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer += 1.0f;
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
		pos = m_apParts[nCntParts]->pParts->GetPosOrg();
		rot = m_apParts[nCntParts]->pParts->GetRot();

		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey - 1)
		{
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

		universal::LimitRot(&DiffRotX);
		universal::LimitRot(&DiffRotY);
		universal::LimitRot(&DiffRotZ);

		//目的の値=======================================================================================================
		float DestPosX = pos.x + m_aKeyOld[nCntParts].fPosX +
			DiffPosX * (float)(1.0f / (float)m_nFrame) * m_nCounterMotion;

		float DestPosY = pos.y + m_aKeyOld[nCntParts].fPosY +
			DiffPosY * (float)(1.0f / (float)m_nFrame) * m_nCounterMotion;

		float DestPosZ = pos.z + m_aKeyOld[nCntParts].fPosZ +
			DiffPosZ * (float)(1.0f / (float)m_nFrame) * m_nCounterMotion;

		float DestRotX = m_aKeyOld[nCntParts].fRotX +
			DiffRotX * (float)(1.0f / (float)m_nFrame) * m_nCounterMotion;

		float DestRotY = m_aKeyOld[nCntParts].fRotY +
			DiffRotY * (float)(1.0f / (float)m_nFrame) * m_nCounterMotion;

		float DestRotZ = m_aKeyOld[nCntParts].fRotZ +
			DiffRotZ * (float)(1.0f / (float)m_nFrame) * m_nCounterMotion;

		//パーツの向き・位置設定
		m_apParts[nCntParts]->pParts->SetPosition(D3DXVECTOR3(DestPosX, DestPosY, DestPosZ));

		universal::LimitRot(&DestRotX);
		universal::LimitRot(&DestRotY);
		universal::LimitRot(&DestRotZ);

		rot = D3DXVECTOR3(DestRotX, DestRotY, DestRotZ);

		m_apParts[nCntParts]->pParts->SetRot(rot);
	}

	m_nCounterMotion++;

	if (m_nKey >= m_aMotionInfo[m_motionType].nNumKey - 1)
	{// モーションが終了していたら
		m_motionTypeOld = m_motionType;

		// 終了した状態にする
		m_bFinish = true;

		if (m_aMotionInfo[m_motionType].bLoop)
		{// ループするものは再設定
			SetMotion(m_motionType);
		}
	}

	if (m_nCounterMotion > m_nFrame)
	{//キーのフレーム数に達したら
		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey)
		{
			if (m_nKey < m_aMotionInfo[m_motionType].nNumKey - 1)
			{
				m_nKey++;
			}

			if (m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame <= 0)
			{
				m_nFrame = 1;
			}
			else
			{
				m_nFrame = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;
			}

			m_nCounterMotion = 0;

			SetKeyOld();
		}
	}
}

//=====================================================
// モーション設定
//=====================================================
void CMotion::SetMotion(int nMotionType)
{
	// 終了状態の解除
	m_bFinish = false;

	// 終了するモーションを保存
	m_motionTypeOld = m_motionType;
	SetKeyOld();

	if (m_nKey != -1)
	{
		if (m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame <= 0)
		{
			m_nFrame = 1;
		}
		else
		{
			m_nFrame = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;
		}
	}

	if (m_motionType != nMotionType)
	{
		m_nFrame = 10;
	}

	// モーション情報の設定
	m_motionType = nMotionType;
	m_nKey = -1;
	m_nCounterMotion = 0;
}

//=====================================================
// 終了するキーの情報保存
//=====================================================
void CMotion::SetKeyOld(void)
{
	for (int nCntPart = 0;nCntPart < m_nNumParts;nCntPart++)
	{
		m_aKeyOld[nCntPart].fPosX = m_apParts[nCntPart]->pParts->GetPosition().x - m_apParts[nCntPart]->pParts->GetPosOrg().x;
		m_aKeyOld[nCntPart].fPosY = m_apParts[nCntPart]->pParts->GetPosition().y - m_apParts[nCntPart]->pParts->GetPosOrg().y;
		m_aKeyOld[nCntPart].fPosZ = m_apParts[nCntPart]->pParts->GetPosition().z - m_apParts[nCntPart]->pParts->GetPosOrg().z;

		m_aKeyOld[nCntPart].fRotX = m_apParts[nCntPart]->pParts->GetRot().x;
		m_aKeyOld[nCntPart].fRotY = m_apParts[nCntPart]->pParts->GetRot().y;
		m_aKeyOld[nCntPart].fRotZ = m_apParts[nCntPart]->pParts->GetRot().z;
	}
}

//=====================================================
// ポーズの初期設定
//=====================================================
void CMotion::InitPose(int nMotion)
{
	for (int i = 0; i < m_aMotionInfo[nMotion].nNumKey;i++)
	{
		for (int nCntPart = 0; nCntPart < m_nNumParts; nCntPart++)
		{
			m_aKeyOld[nCntPart] = m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart];

			D3DXVECTOR3 rot =
			{
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotX,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotY,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotZ
			};

			D3DXVECTOR3 pos =
			{
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosX,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosY,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosZ
			};

			m_apParts[nCntPart]->pParts->SetRot(rot);
			//m_apParts[nCntPart]->pParts->SetPosition(pos);
		}
	}

	// キー情報保存
	SetKeyOld();
}

//=====================================================
// リセット処理
//=====================================================
void CMotion::Reset(void)
{
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		m_apParts[nCntParts]->pParts->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

//=====================================================
// アイコン生成
//=====================================================
void CMotion::CreateIcon(int nIdx)
{
    CObject3D *pIcon = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

    pIcon->SetSize(SIZE_ICON, SIZE_ICON);
    pIcon->SetMode(CObject3D::MODE::MODE_BILLBOARD);
    pIcon->SetVtx();

    // テクスチャの読込
    int nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\UI\\FK.png");
    pIcon->SetIdxTexture(nIdxTexture);
    pIcon->EnableZtest(true);

    m_mapIcon[nIdx] = pIcon;
}

//=====================================================
// パーツの選択
//=====================================================
void CMotion::SelectParts(void)
{
    CInputMouse *pMouse = CManager::GetMouse();

    if (pMouse == nullptr)
        return;

    D3DXVECTOR3 posNear;
    D3DXVECTOR3 posFar;
    D3DXVECTOR3 vecDiff;

    universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

    std::map<CObject3D*,int> mapIcon;

    for (int i = 0; i < m_nNumParts; i++)
    {
        D3DXVECTOR3 posIcon = m_mapIcon[i]->GetPosition();

        m_mapIcon[i]->SetColor(COL_NOT_CURRENT);

        bool bHit = universal::CalcRaySphere(posNear, vecDiff, posIcon, SIZE_ICON);

        if (bHit)
            mapIcon[m_mapIcon[i]] = i;
    }

    if (mapIcon.size() == 0)
        return;

    float fLengthMin = FLT_MAX;
    CObject3D *pIcon = nullptr;

    for (auto it : mapIcon)
    {
        D3DXVECTOR3 posIcon = it.first->GetPosition();
        D3DXVECTOR3 posCamera = Manager::GetPosVCamera();

        float fLength;

        bool bMin = universal::DistCmp(posCamera, posIcon, fLengthMin, &fLength);

        if (bMin)
        {
            fLengthMin = fLength;
            pIcon = it.first;
        }
    }

    if (pIcon != nullptr)
    {
        pIcon->SetColor(COL_CURRENT);
        
        if (pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
        {// クリックしたらパーツ番号の決定
            m_nIdxParts = mapIcon[pIcon];

            if (m_pManipulater != nullptr)
            {// マニピュレーターにパーツ番号を渡す
                m_pManipulater->SetIdxPart(m_nIdxParts);
            }
        }
    }
}

//=====================================================
// パーツのトランスフォーム設定
//=====================================================
void CMotion::SetTransform(int nIdx)
{
	if (m_nKey == -1)
	{
		return;
	}

    if (nIdx == -1)
    {
        nIdx = m_nIdxParts;
    }

	// 向きの設定
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nIdx].fRotX = m_apParts[nIdx]->pParts->GetRot().x;
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nIdx].fRotY = m_apParts[nIdx]->pParts->GetRot().y;
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nIdx].fRotZ = m_apParts[nIdx]->pParts->GetRot().z;

	// 位置の設定
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nIdx].fPosX = m_apParts[nIdx]->pParts->GetPosition().x - m_apParts[nIdx]->pParts->GetPosOrg().x;
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nIdx].fPosY = m_apParts[nIdx]->pParts->GetPosition().y - m_apParts[nIdx]->pParts->GetPosOrg().y;
	m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nIdx].fPosZ = m_apParts[nIdx]->pParts->GetPosition().z - m_apParts[nIdx]->pParts->GetPosOrg().z;
}

//=====================================================
// 自分のマトリックスの設定
//=====================================================
void CMotion::SetMatrix(void)
{
	//変数宣言
	D3DXMATRIX mtxRotModel, mtxTransModel;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

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
}

//=====================================================
// マトリックスをかけ合わせる処理
//=====================================================
void CMotion::MultiplyMtx(void)
{
	if (m_bInde == false)
	{
		// 親のマトリックス
		SetMatrix();
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX mtxRotModel, mtxTransModel;
	D3DXMATRIX *pMtxParent;
	D3DXMATRIX *pMtx;

	D3DXMATRIX mtxShadow;
	D3DLIGHT9 light;
	D3DXVECTOR4 posLight;
	D3DXVECTOR3 normal;
	D3DXPLANE plane;

	// ライトの位置設定
	pDevice->GetLight(2, &light);
	posLight = { -light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f };

	// 平面情報の生成
	normal = { 0.0f,1.0f,0.0f };
	D3DXPlaneFromPointNormal(&plane, &m_posShadow, &normal);

	for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
	{
		// マトリックスの取得
		pMtx = m_apParts[nCntParts]->pParts->GetMatrix();

		//ワールドマトリックス初期化
		D3DXMatrixIdentity(pMtx);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			m_apParts[nCntParts]->pParts->GetRot().y, m_apParts[nCntParts]->pParts->GetRot().x, m_apParts[nCntParts]->pParts->GetRot().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxRotModel);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			m_apParts[nCntParts]->pParts->GetPosition().x, m_apParts[nCntParts]->pParts->GetPosition().y, m_apParts[nCntParts]->pParts->GetPosition().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxTransModel);
		
		if (m_apParts[nCntParts]->nIdxParent != -1)
		{//親パーツがある場合
			// 親マトリックスの取得
			pMtxParent = m_apParts[m_apParts[nCntParts]->nIdxParent]->pParts->GetMatrix();
		}
		else
		{
			pMtxParent = &m_mtxWorld;
		}

		//親パーツとパーツのワールドマトリックスをかけ合わせる
		D3DXMatrixMultiply(pMtx, pMtx, pMtxParent);

		if (m_bShadow)
		{
			// シャドウマトリックス初期化
			D3DXMatrixIdentity(&mtxShadow);

			// シャドウマトリックスの作成
			D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
			D3DXMatrixMultiply(&mtxShadow, pMtx, &mtxShadow);

			// シャドウマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		}

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, pMtx);

		m_apParts[nCntParts]->pParts->Draw();
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
// 情報描画
//=====================================================
void CMotion::DrawMotionState(void)
{
	int nIdx = m_nIdxParts;
	D3DXVECTOR3 pos, rot;

	CManager::GetDebugProc()->Print("[F2]モーション再生");

	CManager::GetDebugProc()->Print("                                                 ");

	if (m_bSetUp)
	{
		CManager::GetDebugProc()->Print("モード[セットアップ]\n");
	}
	else
	{
		CManager::GetDebugProc()->Print("モード[モーション]\n");
	}

	CManager::GetDebugProc()->Print("[F3]セットアップモードに移行\n");
	CManager::GetDebugProc()->Print("[F8]保存[data\\motion.txt]\n");

	if (m_bSetUp)
	{
		CManager::GetDebugProc()->Print("[WASD]パーツ水平移動\n");
		CManager::GetDebugProc()->Print("[QE]パーツ上下移動\n");
		CManager::GetDebugProc()->Print("[LSHIFT]加速\n");
	}

	CManager::GetDebugProc()->Print("カメラ操作はUNREALと同じ\n");

	CManager::GetDebugProc()->Print("モーション再生[%d]\n", m_bMotion);
	CManager::GetDebugProc()->Print("セットアップ[%d]\n", m_bSetUp);
	CManager::GetDebugProc()->Print("選択パーツ[%d / %d]\n", m_nIdxParts, m_nNumParts - 1);
	CManager::GetDebugProc()->Print("選択モーション[%d / %d]\n", m_motionType, m_nNumMotion - 1);
	CManager::GetDebugProc()->Print("選択キー[%d / %d]\n", m_nKey, m_aMotionInfo[m_motionType].nNumKey - 1);
	CManager::GetDebugProc()->Print("フレーム数[%d]\n", m_nFrame);
}

//=====================================================
// ポーズ設定
//=====================================================
void CMotion::SetPose(void)
{
    if (m_nKey == -1)
    {
        return;
    }

	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	for (int nCntPart = 0; nCntPart < m_nNumParts; nCntPart++)
	{
		// 向きの取得
		rot.x = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fRotX;
		rot.y = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fRotY;
		rot.z = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fRotZ;
		// 位置の取得
		pos.x = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fPosX + m_apParts[nCntPart]->pParts->GetPosOrg().x;
		pos.y = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fPosY + m_apParts[nCntPart]->pParts->GetPosOrg().y;
		pos.z = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntPart].fPosZ + m_apParts[nCntPart]->pParts->GetPosOrg().z;

		m_apParts[nCntPart]->pParts->SetRot(rot);
		m_apParts[nCntPart]->pParts->SetPosition(pos);
	}

	SetKeyOld();
}

//=====================================================
// 読込処理
//=====================================================
void CMotion::Load(char *pPath)
{
	//変数宣言
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	int nCntModel = 0;

	//ファイルから読み込む
	FILE *pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		while(true)
		{
			//文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			//ファイル名読み込み=========================================
			if (strcmp(cTemp, "NUM_MODEL") == 0)
			{
				//"="読み込み
				(void)fscanf(pFile, "%s", &cTemp[0]);

				//モデル数読み込み
				(void)fscanf(pFile, "%d", &m_nNumParts);

				for (int nCntFile = 0; nCntFile < m_nNumParts;)
				{//ファイル名読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "MODEL_FILENAME") == 0)
					{//ファイル名読み込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						char aPath[MAX_STRING];

						// モデルパス読込
						(void)fscanf(pFile, "%s", &aPath[0]);

						m_apParts[nCntFile] = new Parts;

						m_apParts[nCntFile]->pParts = CParts::Create();

						int nIdx = CModel::Load(&aPath[0]);

						// モデル読込
						m_apParts[nCntFile]->pParts->SetIdxModel(nIdx);
						m_apParts[nCntFile]->pParts->BindModel(m_apParts[nCntFile]->pParts->GetIdxModel());

                        // アイコンの生成
                        CreateIcon(nCntFile);

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
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PARTSSET") == 0)
					{//パーツスタート
						while (strcmp(cTemp, "END_PARTSSET") != 0)
						{//終わりまでパーツ設定
							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "INDEX") == 0)
							{//番号読み込み
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdx);
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{//親番号読み込み
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdxParent);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								D3DXVECTOR3 pos;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									(void)fscanf(pFile, "%f", &pos[nCntPos]);
								}

								m_apParts[nCntModel]->pParts->SetPosition(pos);

								m_apParts[nCntModel]->pParts->SetPosOrg(pos);
							}

							if (strcmp(cTemp, "ROT") == 0)
							{//向き読み込み
								D3DXVECTOR3 rot;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntRot = 0; nCntRot < 3; nCntRot++)
								{
									(void)fscanf(pFile, "%f", &rot[nCntRot]);
								}

								m_apParts[nCntModel]->pParts->SetRot(rot);
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
			int nCntEvent = 0;

			//モーション設定===========================================
			if (strcmp(cTemp, "MOTIONSET") == 0)
			{//モーションスタート
				while (strcmp(cTemp, "END_MOTIONSET") != 0)
				{//終わりまでキャラクター設定

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "LOOP") == 0)
					{//ループ判断
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion].bLoop);
					}

					if (strcmp(cTemp, "NUM_KEY") == 0)
					{//キー数判断
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
					}

					if (strcmp(cTemp, "NUM_EVENT") == 0)
					{// イベント数判断
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumEvent);

						if (m_aMotionInfo[m_nNumMotion].nNumEvent != 0)
						{// イベント情報を生成
							m_aMotionInfo[m_nNumMotion].pEvent = new EVENT_INFO[m_aMotionInfo[m_nNumMotion].nNumEvent];

							ZeroMemory(m_aMotionInfo[m_nNumMotion].pEvent, sizeof(EVENT_INFO) * m_aMotionInfo[m_nNumMotion].nNumEvent);
						}
					}

					if (strcmp(cTemp, "EVENTSET") == 0 && m_aMotionInfo[m_nNumMotion].pEvent != 0)
					{// イベント情報設定
						while (strcmp(cTemp, "END_EVENTSET") != 0)
						{
							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "KEY") == 0)
							{// 再生キー取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nKey);
							}

							if (strcmp(cTemp, "FRAME") == 0)
							{// 再生フレーム取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nFrame);
							}

                            if (strcmp(cTemp, "NUM_FRAME") == 0)
                            {// 再生フレーム数取得
                                (void)fscanf(pFile, "%s", &cTemp[0]);

                                (void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fNumFrame);

                                m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fTimer = FLT_MAX;
                            }

                            if (strcmp(cTemp, "RADIUS") == 0)
                            {// 半径
                                (void)fscanf(pFile, "%s", &cTemp[0]);

                                (void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fRadius);
                            }

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								D3DXVECTOR3 pos;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									(void)fscanf(pFile, "%f", &pos[nCntPos]);
								}

								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].offset = pos;
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{// 親パーツ番号取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nIdxParent);
							}
						}

						nCntEvent++;
					}

					if (strcmp(cTemp, "KEYSET") == 0)
					{//キースタート
						while (strcmp(cTemp, "END_KEYSET") != 0)
						{//終わりまでキー設定

							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "FRAME") == 0)
							{//フレーム数取得
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
							}

							if (strcmp(cTemp, "KEY") == 0)
							{//フレーム数取得
								while (strcmp(cTemp, "END_KEY") != 0)
								{//終わりまでキー設定
									(void)fscanf(pFile, "%s", &cTemp[0]);

									if (strcmp(cTemp, "POS") == 0)
									{//位置取得
										(void)fscanf(pFile, "%s", &cTemp[0]);

										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
									}

									if (strcmp(cTemp, "ROT") == 0)
									{//向き取得
										(void)fscanf(pFile, "%s", &cTemp[0]);

										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
									}
								}

								nCntPart++;

							}//KEYif
						}//ENDKEYwhile

						nCntKey++;
						nCntPart = 0;
					}
				}//MOTIONwhile

				nCntMotion++;
				m_nNumMotion++;
			}
			//===========================================================

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}//while閉じ

		fclose(pFile);
	}
	else
	{
		assert(("モーションデータ読み込みに失敗", false));
	}
}

//=====================================================
// パーツの最大半径取得
//=====================================================
float CMotion::GetRadiusMax(void)
{
	float fRadiusMax = 0.0f;
	float fTemp;

	for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
	{
		if (m_apParts[nCntParts] != nullptr)
		{// パーツの半径取得
			fTemp = m_apParts[nCntParts]->pParts->GetRadius();

			if (fTemp > fRadiusMax)
			{// 最大半径設定
				fRadiusMax = fTemp;
			}
		}
	}

	return fRadiusMax;
}

//=====================================================
// パーツの絶対位置取得
//=====================================================
D3DXVECTOR3 CMotion::GetMtxPos(int nIdx)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (nIdx < 0 && 
		nIdx >= m_nNumParts)
	{// 範囲外制限
		return pos;
	}

	if (m_apParts[nIdx] != nullptr)
	{
		if (m_apParts[nIdx]->pParts != nullptr)
		{
			D3DXMATRIX mtx = *m_apParts[nIdx]->pParts->GetMatrix();

			pos = 
			{
				mtx._41,
				mtx._42,
				mtx._43,
			};
		}
	}

	return pos;
}

//=====================================================
// 残像の設定
//=====================================================
void CMotion::SetAfterImage(D3DXCOLOR col, int m_nLife)
{
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apParts[nCntParts] != nullptr)
		{// 残像設定
			D3DXMATRIX *pMtx = m_apParts[nCntParts]->pParts->GetMatrix();
			//CModel::Model *model = m_apParts[nCntParts]->pParts->GetModel();

			//CAfterImage::Create(*m_apParts[nCntParts]->pParts->GetMatrix(), m_apParts[nCntParts]->pParts->GetIdxModel(),col,m_nLife);
		}
	}
}

//=====================================================
// 全パーツの色設定
//=====================================================
void CMotion::SetAllCol(D3DXCOLOR col)
{
	m_col = col;

	for (int i = 0; i < m_nNumParts; i++)
	{
		if (m_apParts[i] != nullptr)
		{
			if (m_apParts[i]->pParts != nullptr)
			{
				//m_apParts[i]->pParts->SetEmissiveCol(col);
			}
		}
	}
}

//=====================================================
// 全パーツの色リセット
//=====================================================
void CMotion::ResetAllCol(void)
{
	for (int i = 0; i < m_nNumParts; i++)
	{
		if (m_apParts[i] != nullptr)
		{
			if (m_apParts[i]->pParts != nullptr)
			{
				//m_apParts[i]->pParts->ResetColor();
			}
		}
	}
}

//=====================================================
// 生成処理
//=====================================================
CMotion *CMotion::Create(char *pPath)
{
	CMotion *pCMotion = nullptr;

	if (pCMotion == nullptr)
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

	pFile = fopen(m_aPathSave, "w");

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
			fprintf(pFile, "MODEL_FILENAME = %s\n", CModel::GetModelPath(m_apParts[nCntModel]->pParts->GetIdxModel()));
		}

		fprintf(pFile, "\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# キャラクター情報\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "CHARACTERSET\n");
		fprintf(pFile, "    NUM_PARTS = %d\n\n", m_nNumParts);

		D3DXVECTOR3 posOrg, pos, rot;

		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// セットアップ情報書き出し
			posOrg = m_apParts[nCntModel]->pParts->GetPosOrg();
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

			if (m_aMotionInfo[nCntMotion].pEvent != NULL)
			{// イベント情報書き出し
				fprintf(pFile, "    NUM_EVENT = %d\n", m_aMotionInfo[nCntMotion].nNumEvent);
				for (int nCntParticle = 0; nCntParticle < m_aMotionInfo[nCntMotion].nNumEvent; nCntParticle++)
				{
					fprintf(pFile, "\n");

					fprintf(pFile, "    EVENTSET\n");
					fprintf(pFile, "        KEY = %d\n", m_aMotionInfo[nCntMotion].pEvent[nCntParticle].nKey);
                    fprintf(pFile, "        FRAME = %d\n", m_aMotionInfo[nCntMotion].pEvent[nCntParticle].nFrame);
                    fprintf(pFile, "        NUM_FRAME = %f\n", m_aMotionInfo[nCntMotion].pEvent[nCntParticle].fNumFrame);
                    fprintf(pFile, "        RADIUS = %f\n", m_aMotionInfo[nCntMotion].pEvent[nCntParticle].fRadius);
                    fprintf(pFile, "        PARENT = %d\n", m_aMotionInfo[nCntMotion].pEvent[nCntParticle].nIdxParent);
                    D3DXVECTOR3 pos = m_aMotionInfo[nCntMotion].pEvent[nCntParticle].offset;
                    fprintf(pFile, "        POS = %f %f %f\n", pos.x, pos.y, pos.z);
					fprintf(pFile, "    END_EVENTSET\n");
				}
			}

			fprintf(pFile, "\n");

			for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey; nCntKey++)
			{// キー情報書き出し
				fprintf(pFile, "    KEYSET     #---<< %d / %d >>\n", nCntKey, m_aMotionInfo[nCntMotion].nNumKey);
				fprintf(pFile, "        FRAME = %d\n", m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
				for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
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

        fclose(pFile);
	}
}

//=====================================================
// マニピュレーターの生成
//=====================================================
CManipulater *CMotion::CreateManipulater(int nIdxPart)
{
    if(m_pManipulater == nullptr)
        m_pManipulater = CManipulater::Create(nIdxPart,this);

    return m_pManipulater;
}

//=====================================================
// マニピュレーターの削除
//=====================================================
void CMotion::DeleteManipulater(void)
{
    if (m_pManipulater != nullptr)
    {
        m_pManipulater->Uninit();
        m_pManipulater = nullptr;
    }
}
