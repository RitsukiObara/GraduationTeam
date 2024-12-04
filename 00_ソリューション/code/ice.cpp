//*****************************************************
//
// 氷の処理[ice.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "ice.h"
#include "iceHard.h"
#include "iceHardMulti.h"
#include "iceManager.h"
#include "texture.h"
#include "gameObject.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "ocean.h"
#include "objectX.h"
#include "manager.h"
#include "particle.h"
#include "model.h"
#include "effect3D.h"
#include "enemy.h"
#include "player.h"
#include "MyEffekseer.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEXTURE\\MATERIAL\\ice001.jpg";				// テクスチャパス
const string PATH_TEX_OVERRAY = "data\\TEXTURE\\MATERIAL\\iceanimation.jpg";	// オーバレイテクスチャパス
const float SIZE_INIT = 100.0f;	// 初期サイズ
const float HEIGHT_ICE = 50.0f;	// 氷の高さ
const int NUM_CORNER = 6;	// 角の数
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// 上側の初期向き

const float TIME_REPAIR_ICE = 11.0f;	// 氷の修復にかかる時間
const int MAX_SCALE = 50; // スケールの最大値
const int MIN_SCALE = 20; // スケールの最小値
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_piece.x";	// 破片氷のモデルパス
const float SPEED_SINK = 5.0f;	// 沈む速度
const float HEIGHT_DELETE = -100.0f;	// 削除するまでの高さ

const float HEIGHT_DEFAULT_FROM_OCEAN = 50.0f;	// 海からのデフォルトの高さ
const float HEIGHT_NORMALSINK_FROM_OCEAN = 10.0f;	// 海からの通常沈む高さ

const float LINE_STOP_ICE = 1.0f;	// 氷が止まるしきい値

//------------------------------
// 傾きの定数
//------------------------------
namespace tilt
{
const float SPEED_ROT = 0.1f;			// 回転速度
const float MAX_TILT = D3DX_PI * 0.2f;	// 最大の傾き
const float RATE_COLLIDE = 1.6f;		// 判定の割合
}

//------------------------------
// さざ波の定数
//------------------------------
namespace ripple
{
const int MAX_TIME = 7;	// 生成にかかる最大時間
const int MIN_TIME = 2;	// 生成にかかる最小時間
}

//------------------------------
// 光の定数
//------------------------------
namespace flash
{
const int MAX_TIME = 14;			// 再生にかかる最大時間
const int MIN_TIME = 7;				// 再生にかかる最小時間
const int FRAME_ANIMATION = 2;		// アニメーションを切り替えるフレーム
const int PATERN_ANIM = 5;			// アニメーションのパターン
}
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CIce::s_nNumAll = 0;
std::vector<CIce*> CIce::s_Vector = {};	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CIce::CIce(int nPriority) : CObject3D(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false),
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr), m_bSink(false), m_bStop(nullptr), m_abRipleFrag(), m_nCntAnimFlash(0), m_rotDest()
{
	s_nNumAll++;
	s_Vector.push_back(this);
}

//=====================================================
// デストラクタ
//=====================================================
CIce::~CIce()
{
	s_nNumAll--;
}

//=====================================================
// 生成処理
//=====================================================
CIce *CIce::Create(E_Type type,E_State state)
{
	CIce *pIce = nullptr;

	if (pIce == nullptr)
	{
		switch (type)
		{
		case CIce::TYPE_NORMAL:
			pIce = new CIce;
			break;
		case CIce::TYPE_HARD:
			pIce = new CIceHard;
			break;
		case CIce::TYPE_HARDMULTI:
			pIce = new CIceHardMulti;
			break;
		default:
			assert(false);
			break;
		}

		if (pIce != nullptr)
		{
			pIce->m_state = state;
			pIce->Init();
		}
	}

	return pIce;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIce::Init(void)
{
	EnableCanFind(true);

	// メッシュの生成
	CreateMesh();

	// トランスフォームの初期設定
	SetTransform(SIZE_INIT);

	// ステイト初期化
	ChangeState(new CIceStaeteNormal);

	// 光る処理の初期化
	StartFlash();

	// 継承クラスの初期化
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// メッシュの生成
//=====================================================
void CIce::CreateMesh(void)
{
	if (m_pUp == nullptr)
	{
		m_pUp = CFan3D::Create(4, NUM_CORNER);

		if (m_pUp != nullptr)
		{
			int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
			m_pUp->SetIdxTexture(nIdxTexture);

			int nIdxTextureOverray = Texture::GetIdx(&PATH_TEX_OVERRAY[0]);
			m_pUp->SetIdxTextureOverRay(nIdxTextureOverray);
			m_pUp->SetVtx();
		}
	}

	if (m_pSide == nullptr)
	{
		m_pSide = CMeshCylinder::Create(NUM_CORNER);
		int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
		m_pSide->SetIdxTexture(nIdxTexture);
	}

	SetTransform(Grid::SIZE);
}

//=====================================================
// メッシュの破棄
//=====================================================
void CIce::DeleteMesh(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);
}

//=====================================================
// 終了処理
//=====================================================
void CIce::Uninit(void)
{
	// メッシュの削除
	DeleteMesh();

	for (auto itr = s_Vector.begin(); itr < s_Vector.end(); itr++)
	{
		//削除対象じゃない場合
		if (*itr != this)
		{
			continue;
		}
		//Vectorから削除
		s_Vector.erase(itr);

		break;
	}

	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	// グリッドから削除
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager != nullptr)
		pIceManager->DeleteIce(this);

	// 継承クラスの終了
	CObject3D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CIce::Update(void)
{
	if (!IsSink())	// 沈むフラグがたっていないときのみ行う
		FollowWave();	// 波に追従する処理

	// ステイトの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	// 上に乗ってる物の検出
	SearchOnThis();

	// 傾きの処理
	Tilt();

	// さざ波の処理
	Ripples();

	// メッシュの追従
	FollowMesh();

	// 光る処理
	Flash();

	// 継承クラスの更新
	CObject3D::Update();
}

//=====================================================
// 波に追従する処理
//=====================================================
void CIce::FollowWave(void)
{
	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
	{
		return;
	}

	// 海と一緒に氷を動かす処理
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = pOcean->GetHeight(pos, &move) + HEIGHT_ICE;

	if (m_pUp != nullptr)
	{
		m_pUp->SetPosition(pos);
	}
	if (m_pSide != nullptr)
	{
		m_pSide->SetPosition(pos + D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	}

	SetPosition(pos);
}

//=====================================================
// 自身に乗ってるものの検出
//=====================================================
void CIce::SearchOnThis(void)
{
	if (!IsCanPeck())
		return;

	vector<CGameObject*> apObject;

	GetOnTopObject(apObject);


}

//=====================================================
// 何かしらが乗ってる判定
//=====================================================
bool CIce::IsOnTopAnyObject(void)
{
	vector<CGameObject*> apObject;

	GetOnTopObject(apObject);

	// 上にどれかが乗ってたら真を返す
	for (CGameObject* object : apObject)
	{
		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT, nullptr))
		{// 何かが乗ってるので真を返す
			return true;
		}
	}

	return false;
}

//=====================================================
// 判定するオブジェクトの検出
//=====================================================
void CIce::GetOnTopObject(vector<CGameObject*> &rVector, float fRate)
{
	vector<CGameObject*> apObject;

	// 敵の追加
	vector<CEnemy*> aEnemy = CEnemy::GetInstance();

	for (CEnemy* enemy : aEnemy)
		apObject.push_back((CGameObject*)enemy);

	// プレイヤーの追加
	vector<CPlayer*> aPlayer = CPlayer::GetInstance();

	for (CPlayer* player : aPlayer)
		apObject.push_back((CGameObject*)player);

	// 上にどれかが乗ってたら真を返す
	for (CGameObject* object : apObject)
	{
		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT * fRate, nullptr))
		{// 何かが乗ってるので真を返す
			rVector.push_back(object);
		}
	}

}

//=====================================================
// 傾きの処理
//=====================================================
void CIce::Tilt(void)
{
	if (!IsCanPeck())
		return;	// 叩けない氷なら通らない

	//------------------------------
	// 傾きの計算
	//------------------------------
	// 乗っているオブジェクトの取得
	vector<CGameObject*> apObject;
	GetOnTopObject(apObject, tilt::RATE_COLLIDE);

	int nNumObject = (int)apObject.size();

	if (nNumObject != 0)
	{// 何かしら乗ってたら判定
		D3DXVECTOR3 vecDiff = { 0.0f,0.0f,0.0f };

		for (CGameObject* pObj : apObject)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posObj = pObj->GetPosition();

			vecDiff += posObj - pos;
		}

		// 差分ベクトルを平均化
		vecDiff /= (float)nNumObject;

		m_rotDest.x = vecDiff.z / Grid::SIZE * tilt::MAX_TILT;
		m_rotDest.z = -vecDiff.x / Grid::SIZE * tilt::MAX_TILT;
	}
	else	// 上に何も乗ってなかったら元に戻す
		m_rotDest = { 0.0f,0.0f,0.0f };

	// 向きの補正
	D3DXVECTOR3 rot = GetRotation();

	rot += (m_rotDest - rot) * tilt::SPEED_ROT;

	SetRotation(rot);
}

//=====================================================
// さざ波の処理
//=====================================================
void CIce::Ripples(void)
{
	// タイマーでのせき止め
	m_fTimerRipples += CManager::GetDeltaTime();

	if (m_fTimerRipples <= m_fSpawnTimeRipples)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	COcean::E_Stream stream = pIceMgr->GetDirStream();

	// 流れに合った方向のフラグが立ってたら処理を通る
	if (!m_abRipleFrag[stream])
		return;

	// エフェクトのトランスフォーム設定
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[stream];

	D3DXVECTOR3 posEffect = GetPosition() - vecStream * SIZE_INIT;
	D3DXVECTOR3 rotEffect = { 0.0f,0.0f,0.0f };
	rotEffect.y = atan2f(vecStream.x, vecStream.z);

	// エフェクトの生成
	MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLESTRINGS, posEffect, rotEffect);

	// タイマーの再設定
	m_fTimerRipples = 0.0f;

	int nRand = universal::RandRange(ripple::MAX_TIME, ripple::MIN_TIME);

	m_fSpawnTimeRipples = (float)nRand;
}

//=====================================================
// メッシュの追従
//=====================================================
void CIce::FollowMesh(void)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	// マトリックスを確定させる
	CulcMatrix();

	D3DXMATRIX mtx = GetMatrix();

	// 上の扇ポリゴンの設定
	m_pUp->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUp->SetMatrixParent(mtx);

	// サイドのシリンダーの設定
	m_pSide->SetPosition(D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	m_pSide->SetMatrixParent(mtx);
}

//=====================================================
// 光る処理の開始
//=====================================================
void CIce::StartFlash(void)
{
	if (m_pUp == nullptr)
		return;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *m_pUp->GetVtxBuff();
	VERTEX_3D* pVtx;

	if (pVtxBuff == nullptr)
		return;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// スクロールをリセット
	int nNumVtx = m_pUp->GetNumVtx();

	for (int i = 0; i < nNumVtx + 2; i++)
	{
		pVtx[i].tex2 = pVtx[i].tex;

		pVtx[i].tex2.x *= 1.0f / flash::PATERN_ANIM;	// アニメーション基準に縮める
		pVtx[i].tex2.x += 1.0f / flash::PATERN_ANIM * flash::PATERN_ANIM;
	}

	// 頂点バッファのアンロック
	pVtxBuff->Unlock();

	// タイマーの再設定
	m_fTimerFlash = 0.0f;
	int nRand = universal::RandRange(flash::MAX_TIME, flash::MIN_TIME);
	m_fTimeStartFlash = (float)nRand;
}

//=====================================================
// きらりと光る処理
//=====================================================
void CIce::Flash(void)
{
	m_fTimerFlash += CManager::GetDeltaTime();

	if (m_fTimerFlash < m_fTimeStartFlash)
		return;

	if (m_pUp == nullptr)
		return;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *m_pUp->GetVtxBuff();
	VERTEX_3D* pVtx;

	if (pVtxBuff == nullptr)
		return;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// スクロールさせる
	int nNumVtx = m_pUp->GetNumVtx();

	bool bPassAll = true;	// 全頂点が通過したフラグ

	for (int i = 0; i < nNumVtx + 2; i++)
	{
		pVtx[i].tex2 = pVtx[i].tex;

		pVtx[i].tex2.x *= 1.0f / flash::PATERN_ANIM;	// アニメーション基準に縮める
		pVtx[i].tex2.x += 1.0f / flash::PATERN_ANIM * m_nPaternAnim;
	}

	// 頂点バッファのアンロック
	pVtxBuff->Unlock();

	// カウンター加算
	m_nCntAnimFlash++;

	if (m_nCntAnimFlash % flash::FRAME_ANIMATION == 0)
	{
		m_nPaternAnim = (m_nPaternAnim + 1) % flash::PATERN_ANIM;

		if (m_nPaternAnim == 0)
			StartFlash();	// アニメーションが戻ったらリセット
	}
}

//=====================================================
// 色の設定
//=====================================================
void CIce::SetColor(D3DXCOLOR col)
{
	if (m_pSide == nullptr || m_pUp == nullptr)
		return;

	m_pSide->SetCol(col);
	m_pUp->SetCol(col);
}

//=====================================================
// 描画処理
//=====================================================
void CIce::Draw(void)
{
	CObject3D::Draw();
}

//=====================================================
// トランスフォーム設定
//=====================================================
void CIce::SetTransform(float fRadius)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	D3DXVECTOR3 posIce = GetPosition();

	// 上の扇ポリゴンの設定
	m_pUp->SetRadius(fRadius * 0.5f);
	m_pUp->SetPosition(posIce);
	m_pUp->SetVtx();

	// サイドのシリンダーの設定
	m_pSide->SetRadius(fRadius * 0.5f);
	m_pSide->SetHeight(HEIGHT_ICE);
	D3DXVECTOR3 posSide = posIce;
	posSide.y -= HEIGHT_ICE;
	m_pSide->SetPosition(posSide);

	m_pSide->SetVtx();
}

//=====================================================
// ステイトの変更
//=====================================================
void CIce::ChangeState(CIceState *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
		m_pState->Init(this);
}

//*******************************************************************************
// 通常ステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteNormal::Init(CIce *pIce)
{
	pIce->EnableStop(true);

	// 漂着している番号を取得
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);
}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteNormal::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteNormal::Update(CIce *pIce)
{
	// 番号取得に失敗しているかの確認
	bool bMove = CheckFailGetIndex(pIce);

	if(bMove)
		MoveToGrid(pIce);	// グリッドに向かって移動する処理
}

//=====================================================
// 番号取得に失敗しているかの確認
//=====================================================
bool CIceStaeteNormal::CheckFailGetIndex(CIce *pIce)
{
	if (m_nIdxDriftV == -1 ||
		m_nIdxDriftH == -1)
	{// 番号取得に失敗している場合、再取得
		CIceManager *pIceMgr = CIceManager::GetInstance();

		if (pIceMgr == nullptr)
			return false;

		pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);

		if (m_nIdxDriftV == 0 && m_nIdxDriftH == 0)
		{
			int n = 0;
		}

		if (m_nIdxDriftV == -1 ||
			m_nIdxDriftH == -1)
			return false;	// それでも失敗した場合、偽を返す
	}

	return true;
}

//=====================================================
// グリッドに向かって移動する処理
//=====================================================
void CIceStaeteNormal::MoveToGrid(CIce *pIce)
{
	// グリッドの位置を取得
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIceMgr->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	D3DXVECTOR3 posIce = pIce->GetPosition();

	// 差分ベクトルを漂流速度に正規化
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVECTOR3 vecDiff = posGrid - posIce;
	D3DXVec3Normalize(&vecDiff, &vecDiff);

	vecDiff *= fSpeedFlow;

	// 氷の位置に移動量を加算
	pIce->Translate(vecDiff);

	// 上のオブジェクトを動かす
	MoveObjectOnIce(vecDiff,pIce);
}

//=====================================================
// 上のオブジェクトを動かす
//=====================================================
void CIceStaeteNormal::MoveObjectOnIce(D3DXVECTOR3 vecMove,CIce *pIce)
{
	vector<CGameObject*> apObject;

	// 敵の追加
	vector<CEnemy*> aEnemy = CEnemy::GetInstance();

	for (CEnemy* enemy : aEnemy)
		apObject.push_back((CGameObject*)enemy);

	// プレイヤーの追加
	vector<CPlayer*> aPlayer = CPlayer::GetInstance();

	for (CPlayer* player : aPlayer)
		apObject.push_back((CGameObject*)player);

	// 上にどれかが乗ってたら動かす
	for (CGameObject* object : apObject)
	{
		if (object == nullptr)
			continue;

		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = pIce->GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT, nullptr))
			object->Translate(vecMove); // 何かが乗ってるので動かす
	}
}

//*******************************************************************************
// 崩壊ステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteBreak::Init(CIce *pIce)
{
	// 氷のメッシュを削除
	pIce->DeleteMesh();
	
	// ここで氷破壊時のエフェクトを発生
	D3DXVECTOR3 posIce = pIce->GetPosition();
	CParticle::Create(posIce, CParticle::TYPE::TYPE_ICEBREAK);

	// 氷破片を生成
	for (int i = 0; i < CIceStaeteBreak::NUM_ICE_BREAK; i++)
	{
		CObjectX *pPeace = CObjectX::Create();

		// モデルの割り当て
		int nIdxModel = CModel::Load((char*)&PATH_ICE_DEBRIS[0]);
		pPeace->BindModel(nIdxModel);

		// 位置をランダム設定
		D3DXVECTOR3 posPeace = posIce;
		posIce.x += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));
		posIce.z += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));

		pPeace->SetPosition(posIce);

		// スケールをランダム設定
		float fRand = universal::RandRange(MAX_SCALE, MIN_SCALE) * 0.1f;
		pPeace->SetScale(fRand);

		// 回転
		float fRotY = universal::RandRange(314, 0) * 0.01f;
		pPeace->SetRotation(D3DXVECTOR3(0.0f, fRotY, 0.0f));

		// 配列に入れる
		m_aPeaceIce.push_back(pPeace);
	}

	// カウンターを初期化
	m_fTimerRepair = TIME_REPAIR_ICE;
}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteBreak::Uninit(CIce *pIce)
{
	for (auto it : m_aPeaceIce)
		it->Uninit();

	m_aPeaceIce.clear();
}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteBreak::Update(CIce *pIce)
{
	// 氷の修復の更新
	UpdateRepair(pIce);

	// 氷の追従
	FollowIce(pIce);
}

//=====================================================
// 修復の更新
//=====================================================
void CIceStaeteBreak::UpdateRepair(CIce *pIce)
{
	// 沈んでいる状態なら通らない
	if (pIce->IsSink())
		return;

	// タイマーを減らす
	m_fTimerRepair -= CManager::GetDeltaTime();

	if (m_fTimerRepair <= 0.0f)
	{// 一定時間経過で、氷を修復する
		pIce->CreateMesh();

		pIce->ChangeState(new CIceStaeteNormal);
		pIce->EnablePeck(false);

		// パーティクル発生
		CParticle::Create(pIce->GetPosition(), CParticle::TYPE::TYPE_REPAIRICE);
	}
}

//=====================================================
// 氷の追従
//=====================================================
void CIceStaeteBreak::FollowIce(CIce *pIce)
{
	// 氷の追従
	for (auto it : m_aPeaceIce)
	{
		D3DXVECTOR3 pos = { it->GetPosition().x, pIce->GetPosition().y, it->GetPosition().z };

		it->SetPosition(pos);
	}

	// 沈む状態の時、本体の位置を下げる
	if (pIce->IsSink())
	{
		D3DXVECTOR3 posIce = pIce->GetPosition();
		posIce.y -= SPEED_SINK;
		pIce->SetPosition(posIce);

		if (posIce.y < HEIGHT_DELETE)
		{// 一定まで沈んだら削除する
			pIce->Uninit();

			return;
		}
	}
}

//*******************************************************************************
// 流れるステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStateFlow::Init(CIce *pIce)
{
	pIce->EnableStop(false);

	// 自身のポインタをグリッドから外す
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->DeleteIce(pIce);
}

//=====================================================
// 終了処理
//=====================================================
void CIceStateFlow::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStateFlow::Update(CIce *pIce)
{
	if (!m_bDrift)
		UpdateSearchIce(pIce);	// 氷を探している時の更新
	else
		UpdateDriftIce(pIce);	// 漂着してるときの更新
}

//=====================================================
// 氷を探しているときの更新
//=====================================================
void CIceStateFlow::UpdateSearchIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// 海流のベクトル取得
	COcean::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	pIce->Translate(vecStream);

	// 氷との判定
	CollideIce(pIce);
}

//=====================================================
// 漂着するときの更新
//=====================================================
void CIceStateFlow::UpdateDriftIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// グリッドの位置取得
	D3DXVECTOR3 posDrift = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);

	// 海流のベクトル取得
	D3DXVECTOR3 posIce = pIce->GetPosition();
	if (pIceManager == nullptr)
		return;

	// 海流のベクトル取得
	D3DXVECTOR3 vecDiff = posDrift - posIce;

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecDiff, &vecDiff);
	vecDiff *= fSpeedFlow;
	pIce->Translate(vecDiff);

	// グリッドの位置との距離がしきい値を下回ったら止める
	bool bStop = universal::DistCmpFlat(posIce, posDrift, LINE_STOP_ICE, nullptr);

#ifdef _DEBUG
	//CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif

	if (bStop)
	{
		return;
	}
}

//=====================================================
// 氷との判定
//=====================================================
void CIceStateFlow::CollideIce(CIce *pIce)
{
	// 今いるグリッドの取得
	int nIdxV = -1;
	int nIdxH = -1;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	D3DXVECTOR3 posIce = pIce->GetPosition();
	bool bOk = pIceManager->GetIdxGridFromPosition(posIce, &nIdxV, &nIdxH);

	// 番号を保存
	m_nIdxDriftV = nIdxV;
	m_nIdxDriftH = nIdxH;

	// 海流の方向に合わせた判定関数の呼び出し
	DirectionFunc directionFuncs[COcean::E_Stream::STREAM_MAX] = 
	{
		&CIceStateFlow::CheckUp,
		&CIceStateFlow::CheckRight,
		&CIceStateFlow::CheckDown,
		&CIceStateFlow::CheckLeft
	};

	COcean::E_Stream stream = pIceManager->GetDirStream();
	
	// 漂着する氷があったら、フラグを立てて漂着グリッド番号を保存
	if (m_nIdxDriftV == -1 || m_nIdxDriftH == -1)
		return;

	if (pIceManager->GetGridIce(&nIdxV, &nIdxH) != nullptr)
	{
		pIce->ChangeState(new CIceStaeteNormal);
		return;
	}

	vector<CIce*> apIceHit;
	m_bDrift = (this->*directionFuncs[stream])(pIce, m_nIdxDriftV, m_nIdxDriftH, apIceHit);

#ifdef _DEBUG
	D3DXVECTOR3 posEffect = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	debug::Effect3DShort(posEffect, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
#endif

	if (m_bDrift)
	{
		debug::Effect3DShort(pIce->GetPosition(), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 120);
		// グリッドに氷情報を保存
		if (pIceManager->SetIceInGrid(nIdxV, nIdxH, pIce))
		{
			for (CIce* pIce : apIceHit)
			{
				if (pIce == nullptr)
					continue;

				// パーティクルを発生
				D3DXVECTOR3 pos = pIce->GetPosition();
				D3DXVECTOR3 posHitIce = pIce->GetPosition();
				D3DXVECTOR3 vecDIff = posHitIce - pos;

				pos += vecDIff * 0.5f;

				CParticle::Create(pos, CParticle::TYPE::TYPE_STICK_ICE);
			}
		}

		pIce->ChangeState(new CIceStaeteNormal);
	}
}

//=====================================================
// 上方向の確認
//=====================================================
bool CIceStateFlow::CheckUp(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// 上側のグリッドどれかに氷があれば漂着
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTUP]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// 右方向の確認
//=====================================================
bool CIceStateFlow::CheckRight(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// 右側のグリッドどれかに氷があれば漂着
	if (apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHT] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHT]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTDOWN]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// 下方向の確認
//=====================================================
bool CIceStateFlow::CheckDown(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// 下側のグリッドどれかに氷があれば漂着
	if (apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTDOWN]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTDOWN]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// 左方向の確認
//=====================================================
bool CIceStateFlow::CheckLeft(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// 左側のグリッドどれかに氷があれば漂着
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFT] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFT]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTDOWN]);

		bDrift = true;
	}

	return bDrift;
}

//*******************************************************************************
// 沈むステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteSink::Init(CIce *pIce)
{

}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteSink::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteSink::Update(CIce *pIce)
{

}