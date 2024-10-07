//*****************************************************
//
// 力士の処理[rikishi.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "rikishi.h"
#include "effect3D.h"
#include "MyEffekseer.h"
#include "debugproc.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// ボディのパス
const float GUARD_DEFENCE = 0.2f;	// ガード時に受けるダメージ割合
const float RATE_PUNCHGUARD = 0.4f;	// 殴り時のガード割合
const float RATE_BOTHHANDDAMAGE = 2.0f;	// 両手攻撃の割合
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CRikishi::CRikishi(int nPriority) : m_fragMotion()
{

}

//=====================================================
// デストラクタ
//=====================================================
CRikishi::~CRikishi()
{

}

//=====================================================
// 生成処理
//=====================================================
CRikishi *CRikishi::Create(void)
{
	CRikishi *pRikishi = nullptr;

	return pRikishi;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRikishi::Init(void)
{
	// 読込
	Load((char*)&PATH_BODY[0]);

	// 継承クラスの初期化
	CMotion::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRikishi::Uninit(void)
{
	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CRikishi::Update(void)
{
	// 継承クラスの更新
	CMotion::Update();

	ManageMotion();

#ifdef  _DEBUG
	for (int i = 0; i < E_Attack::ATTACK_MAX; i++)
	{
		CDebugProc::GetInstance()->Print("\n攻撃モーションフラグ[%d]", m_fragMotion.bAttack[i]);
	}
	CDebugProc::GetInstance()->Print("\nガードモーションフラグ[%d]", m_fragMotion.bGuard);
	CDebugProc::GetInstance()->Print("\nモーション[%d]", GetMotion());
#endif //  _DEBUG
}

//=====================================================
// 描画処理
//=====================================================
void CRikishi::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}

//=====================================================
// 攻撃
//=====================================================
void CRikishi::Attack(E_Attack typeAttack)
{
	if (typeAttack == E_Attack::ATTACK_BOTHHAND)
	{
		m_fragMotion.bAttack[E_Attack::ATTACK_LEFT] = false;
		m_fragMotion.bAttack[E_Attack::ATTACK_RIGHT] = false;
	}

	m_fragMotion.bAttack[typeAttack] = true;
	m_fragMotion.bGuard = false;
}

//=====================================================
// ガード
//=====================================================
void CRikishi::Guard(void)
{
	m_fragMotion.bGuard = true;
}

//=====================================================
// モーション管理
//=====================================================
void CRikishi::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bGuard)
	{// ガードモーション
		if (nMotion != E_Motion::MOTION_GUARD)
			SetMotion(E_Motion::MOTION_GUARD);
	}
	else if (m_fragMotion.bAttack[E_Attack::ATTACK_RIGHT])
	{// 右手殴り
		if (nMotion != E_Motion::MOTION_ATTACK_RIGHT)
		{
			SetMotion(E_Motion::MOTION_ATTACK_RIGHT);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bAttack[E_Attack::ATTACK_RIGHT] = false;
				SetMotion(E_Motion::MOTION_GUARD);
			}
		}
	}
	else if (m_fragMotion.bAttack[E_Attack::ATTACK_LEFT])
	{// 左手殴り
		if (nMotion != E_Motion::MOTION_ATTACK_LEFT)
		{
			SetMotion(E_Motion::MOTION_ATTACK_LEFT);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bAttack[E_Attack::ATTACK_LEFT] = false;
				SetMotion(E_Motion::MOTION_GUARD);
			}
		}
	}
	else if (m_fragMotion.bAttack[E_Attack::ATTACK_BOTHHAND])
	{// 両手殴り
		if (nMotion != E_Motion::MOTION_ATTACK_BOTHHAND)
		{
			SetMotion(E_Motion::MOTION_ATTACK_BOTHHAND);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bAttack[E_Attack::ATTACK_BOTHHAND] = false;
				SetMotion(E_Motion::MOTION_GUARD);
			}
		}
	}
}

//=====================================================
// イベントの管理
//=====================================================
void CRikishi::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

	if (nMotion == E_Motion::MOTION_ATTACK_RIGHT)
	{
		HitAttack(E_Attack::ATTACK_RIGHT);
	}

	if (nMotion == E_Motion::MOTION_ATTACK_LEFT)
	{
		HitAttack(E_Attack::ATTACK_LEFT);
	}

	if (nMotion == E_Motion::MOTION_ATTACK_BOTHHAND)
	{
		HitAttack(E_Attack::ATTACK_BOTHHAND);
	}
}

//=====================================================
// ヒット処理
//=====================================================
float CRikishi::Hit(E_Attack typeAttack, float fDamage)
{
	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	float fKnockBack = 0.0f;

	int nMotion = GetMotion();

	fKnockBack = fDamage;

	// 両手攻撃の威力を増やす
	if (typeAttack == E_Attack::ATTACK_BOTHHAND)
	{
		fKnockBack *= RATE_BOTHHANDDAMAGE;

		if (nMotion != E_Motion::MOTION_GUARD)
		{// ガードしていないときに命中したら大エフェクト発生
			// ガードヒットエフェクトの発生
			D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);

			D3DXVECTOR3 rotEffect = GetRotation();
			MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT02, posEffect, rotEffect);

			// サウンドの再生
			pSound->Play(CSound::LABEL_SE_ATTACK02);
		}
	}

	// ノックバック量の判定
	if (typeAttack == CRikishi::E_Attack::ATTACK_RIGHT &&
		nMotion == E_Motion::MOTION_ATTACK_RIGHT)
	{// 右で殴られたけど右殴りしてたら微ガード
		fKnockBack *= RATE_PUNCHGUARD;

		// ガードヒットエフェクトの発生
		D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);

		D3DXVECTOR3 rotEffect = GetRotation();
		MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT01, posEffect, rotEffect);

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_ATTACK01);
	}
	else if (typeAttack == CRikishi::E_Attack::ATTACK_LEFT &&
		nMotion == E_Motion::MOTION_ATTACK_LEFT)
	{// 左で殴られたけど左殴りしてたら微ガード
		fKnockBack *= RATE_PUNCHGUARD;

		// ガードヒットエフェクトの発生
		D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);

		D3DXVECTOR3 rotEffect = GetRotation();
		MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT01, posEffect, rotEffect);

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_ATTACK01);
	}
	else if (nMotion == E_Motion::MOTION_GUARD)
	{// 防御時は割合で減らす
		fKnockBack *= GUARD_DEFENCE;

		// ガードヒットエフェクトの発生
		D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);

		D3DXVECTOR3 rotEffect = GetRotation();
		MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT01, posEffect, rotEffect);

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_Guard01);
	}
	else
	{
		// 通常ヒットエフェクトの発生
		D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);
		posEffect.x += universal::RandRange(50, -25);
		posEffect.y += universal::RandRange(50, -25);
		posEffect.z += universal::RandRange(50, -25);

		MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT00, posEffect);

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_ATTACK01);
	}

	return fKnockBack;	// 敵とプレイヤーでノックバック方向が違うので値を返す
}