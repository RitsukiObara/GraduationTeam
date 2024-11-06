//*****************************************************
//
// マニピュレーター[manipulater.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manipulater.h"
#include "motion.h"
#include "inputmouse.h"
#include "manager.h"
#include "effect3D.h"
#include "debugProc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float LENGTH_TRANSLATE_MANIPULATER = 30.0f;  // トラスレートマニピュレーターの長さ
const float RADIUS_TRANSLATE_MANIPULATER = 10.0f;  // トラスレートマニピュレーターの半径
}

//=====================================================
// コンストラクタ
//=====================================================
CManipulater::CManipulater(int nPriority)
{
    m_nIdxPart = 0;
    m_pState = nullptr;
    m_pMotion = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CManipulater::~CManipulater()
{

}

//=====================================================
// 生成処理
//=====================================================
CManipulater* CManipulater::Create(int nIdxPart, CMotion *pMotion)
{
    CManipulater *pManipulater = nullptr;

	if (pManipulater == nullptr)
	{
        pManipulater = new CManipulater;

		if (pManipulater != nullptr)
		{
			// 初期化
            pManipulater->Init();

            pManipulater->m_nIdxPart = nIdxPart;
            pManipulater->m_pMotion = pMotion;
		}
	}

	return pManipulater;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CManipulater::Init(void)
{
    ChangeState(new CStateManipulaterTranslate);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CManipulater::Uninit(void)
{
    if (m_pState != nullptr)
    {
        m_pState->Uninit(this);
        m_pState = nullptr;
    }

    Release();
}

//=====================================================
// 更新処理
//=====================================================
void CManipulater::Update(void)
{
    // パーツの位置の取得
    D3DXVECTOR3 posPart = m_pMotion->GetMtxPos(m_nIdxPart);
    D3DXVECTOR3 rotPart = universal::ExtractEulerAngles(*m_pMotion->GetParts(m_nIdxPart)->pParts->GetMatrix());
    int nIdxPartParent = m_pMotion->GetParts(m_nIdxPart)->nIdxParent;

    if (nIdxPartParent != -1)
    {// 親がいる場合は親のマトリックスから向きを取得
        rotPart = universal::ExtractEulerAngles(*m_pMotion->GetParts(nIdxPartParent)->pParts->GetMatrix());
    }
    else
    {// 親無しなら向きをリセット
        rotPart = { 0.0f,0.0f,0.0f };
    }

    SetPosition(posPart);
    SetRotation(rotPart);

    if (m_pState != nullptr)
    {
        m_pState->Update(this);
    }
}

//=====================================================
// 描画処理
//=====================================================
void CManipulater::Draw(void)
{

}

//=====================================================
// ステイトの変更
//=====================================================
void CManipulater::ChangeState(CStateManipulater *pState)
{
    if (m_pState != nullptr)
    {
        m_pState->Uninit(this);
        m_pState = nullptr;
    }

    m_pState = pState;

    if (m_pState != nullptr)
    {
        m_pState->Init(this);
    }
}

//********************************************************************************
// 移動のステイト
//********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CStateManipulaterTranslate::CStateManipulaterTranslate()
{
    m_pManipulater = nullptr;
    m_mode = E_MODE::MODE_NONE;
}

//=====================================================
// デストラクタ
//=====================================================
CStateManipulaterTranslate::~CStateManipulaterTranslate()
{

}

//=====================================================
// 初期化
//=====================================================
void CStateManipulaterTranslate::Init(CManipulater *pManipulater)
{
    m_pManipulater = CObjectX::Create();

    int nIdx = CModel::Load("data\\MODEL\\manipulator.x");
    m_pManipulater->BindModel(nIdx);
    m_pManipulater->EnableZtest(true);
}

//=====================================================
// 終了
//=====================================================
void CStateManipulaterTranslate::Uninit(CManipulater *pManipulater)
{

}

//=====================================================
// 更新
//=====================================================
void CStateManipulaterTranslate::Update(CManipulater *pManipulater)
{
    // 入力
    Input(pManipulater);

    // モデルの追従
    FollowModel(pManipulater);

    // モードごとの動き
    ModeMove(pManipulater);
}

//=====================================================
// 入力処理
//=====================================================
void CStateManipulaterTranslate::Input(CManipulater *pManipulater)
{
    CInputMouse *pMouse = CManager::GetMouse();

    if (pMouse == nullptr)
        return;

    D3DXVECTOR3 posNear;
    D3DXVECTOR3 posFar;
    D3DXVECTOR3 vecDiff;

    universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

    D3DXVECTOR3 posManipulater = pManipulater->GetPosition();
    D3DXVECTOR3 rotManipulater = pManipulater->GetRotation();

    D3DXVECTOR3 posEnter;
    D3DXVECTOR3 posExit;

    // Y軸の円筒の生成
    D3DXVECTOR3 vecCylinderY = posManipulater + universal::PolarCoordinates(rotManipulater) * LENGTH_TRANSLATE_MANIPULATER;

    bool bHit = universal::RayCollideCylinder(posNear, posFar, posManipulater, vecCylinderY, RADIUS_TRANSLATE_MANIPULATER,
        &posEnter, &posExit, false);

    if (bHit)
    {
        CEffect3D::Create(posEnter, 10.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

        if (pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LMB))
        {
            m_mode = E_MODE::MODE_Y;
        }
    }

    if(pMouse->GetRelease(CInputMouse::BUTTON::BUTTON_LMB))
        m_mode = E_MODE::MODE_NONE;

    CEffect3D::Create(posManipulater, 10.0f, 3, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
    CEffect3D::Create(vecCylinderY, 10.0f, 3, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
}

//=====================================================
// モデルの追従
//=====================================================
void CStateManipulaterTranslate::FollowModel(CManipulater *pManipulater)
{
    // モデルの追従
    D3DXVECTOR3 posManipulater = pManipulater->GetPosition();
    D3DXVECTOR3 rotManipulater = pManipulater->GetRotation();

    m_pManipulater->SetPosition(posManipulater);
    m_pManipulater->SetRot(rotManipulater);
}

//=====================================================
// モードごとの動き
//=====================================================
void CStateManipulaterTranslate::ModeMove(CManipulater *pManipulater)
{
    D3DXVECTOR3 posNear;
    D3DXVECTOR3 posFar;
    D3DXVECTOR3 vecDiffCast;

    universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiffCast);

    D3DXVECTOR3 vecAxis = {};
    D3DXVECTOR3 vecAxis2 = {};
    D3DXVECTOR3 vecDiff = {};
    D3DXVECTOR3 posHit = {};
    D3DXVECTOR3 posManipulater = pManipulater->GetPosition();
    D3DXVECTOR3 rotManipulater = pManipulater->GetRotation();
    float fLengthAxis = 0.0f;
    float fLengthDiff = 0.0f;
    float fDot = 0.0f;
    float fLength = 0.0f;
    float fAngle = 0.0f;

    switch (m_mode)
    {
    case E_MODE::MODE_X:
        CDebugProc::GetInstance()->Print("\nX軸モード");

        break;
    case E_MODE::MODE_Y:
        CDebugProc::GetInstance()->Print("\nY軸モード");
        // XY平面でマウスのレイキャストを行う
        // Y軸ベクトルの作成
        vecAxis = universal::PolarCoordinates(rotManipulater) * LENGTH_TRANSLATE_MANIPULATER;

        // 真横のベクトルを作成する
        //rotManipulater.x += D3DX_PI * 0.5f; // 極座標のためにロールさせる
        //rotManipulater.y += D3DX_PI * 0.5f; // 極座標のためにロールさせる
        vecAxis2 = universal::PolarCoordinates(rotManipulater) * LENGTH_TRANSLATE_MANIPULATER;
        //D3DXVec3Normalize(&vecAxis2, &vecAxis2);

        universal::CalcRayFlat(posManipulater, vecAxis2, posNear, posFar, &posHit);

        CEffect3D::Create(posHit, 10.0f, 3, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
        CEffect3D::Create(vecAxis2 * 50.0f, 10.0f, 3, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

        break;
    case E_MODE::MODE_Z:
        CDebugProc::GetInstance()->Print("\nZ軸モード");

        break;
    }

    CDebugProc::GetInstance()->Print("\nfLength[%f]", fLength);
}
