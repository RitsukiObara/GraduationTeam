//*****************************************************
//
// プレイヤー選択処理[playerselect.cpp]
// Author:堀川萩大
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "playerselect.h"
#include "inputkeyboard.h"
#include "inputManager.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "debugproc.h"
#include "UIManager.h"
#include "texture.h"
#include "meshfield.h"
#include "CameraState.h"
#include "player.h"
#include "ocean.h"
#include "gameManager.h"
#include "meshcylinder.h"
#include "fan3D.h"
#include "particle.h"
#include "inputjoypad.h"
#include "collision.h"
#include "shadow.h"
#include "polygon3D.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SIZE_UI_PLAYERNUMBER = 0.05f;	// プレイヤーナンバーUIのサイズ
const float HEIGHT_UI_PLAYERNUMBER = 0.8f;	// プレイヤーナンバーUIの位置の高さ

const string PATH_UI_STANDBY = "data\\TEXTURE\\UI\\standby.png";	// スタンドバイテクスチャのパス
const string PATH_UI_READY = "data\\TEXTURE\\UI\\ready.png";	// 準備完了テクスチャのパス
const string PATH_BANNER = "data\\MODEL\\other\\entry_banner.x";	// 看板のモデルパス
const string PATH_PLAYERNUM = "data\\TEXTURE\\UI\\player_Count.png";	// プレイヤー番号テクスチャパス
const D3DXVECTOR3 BANNER_POS = D3DXVECTOR3(0.0f, 0.0f, 500.0f);	// 看板の初期の位置
const D3DXVECTOR3 BANNER_ROT = D3DXVECTOR3(0.1f, 0.0f, 0.07f);		// 看板の初期の向き
const float BANNER_SCALE = 6.5f;	// 看板のサイズ
const float BANNER_COLLISION_SIZE = 20.0f;

const D3DXVECTOR3 INIT_MOVE_PLAYER = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// プレイヤーの初期の移動量
const D3DXVECTOR3 POS_PLAYER_INIT = D3DXVECTOR3(0.0f, -200.0f, 0.0f);		// プレイヤーの初期の位置

const float PLAYERNUM_SIZE = 80.0f;
const float PLAYERNUM_POS_Y = 250.0f;

const float GRAVITY = 0.6f;	// 重力

const float POW_VIB_APPER = 0.6f;	// 出現時の振動強さ
const int TIME_VIB_APPER = 15;		// 出現時の振動長さ

//----------------------------------
// ステージの定数
//----------------------------------
namespace stage
{
const float RADIUS = 1000.0f;				// 半径
const float RADIUS_COLLIDE = RADIUS * 0.9f;	// 判定の半径
const int PRIOLITY = 4;						// 優先順位
const int NUM_VTX = 32;						// 頂点の数
const float HEIGHT = 100.0f;				// 高さ
const string PATH_TEX_FAN = "data\\TEXTURE\\MATERIAL\\field.jpg";			// 円のテクスチャパス
const string PATH_TEX_CYLINDER = "data\\TEXTURE\\MATERIAL\\small_ice.png";	// 円筒のテクスチャパス
}
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

//=====================================================
// コンストラクタ
//=====================================================
CPlayerSelect::CPlayerSelect()
{
	m_nNumPlayer = 0;
	ZeroMemory(&m_StandbyState[0], sizeof(m_StandbyState));
	ZeroMemory(&m_apPlayerUI[0], sizeof(m_apPlayerUI));
	ZeroMemory(&m_apPlayer[0], sizeof(m_apPlayer));
	ZeroMemory(&m_apInputMgr[0], sizeof(m_apInputMgr));
	ZeroMemory(&m_apBillboard[0], sizeof(m_apBillboard));
	m_pCylinder = nullptr;
	m_pFan = nullptr;
	m_pCollisionSphere = nullptr;
	m_pShadow = nullptr;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayerSelect::Init(void)
{
	// 親クラスの初期化
	CScene::Init();

	// UIマネージャーの追加
	CUIManager::Create();

	Camera::ChangeState(new CFollowPlayer);

	// テクスチャ番号取得
	int nIdx[2]; 
	nIdx[0] = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\player_Count.png");
	nIdx[1]= CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Abutton.png");

	float fRateOnePlayer = 1.0f / MAX_PLAYER;

	// UI生成
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		// プレイヤーナンバーUIの生成
		m_apPlayerUI[nCount] = CUI::Create();
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->SetSize(SIZE_UI_PLAYERNUMBER, SIZE_UI_PLAYERNUMBER);
			m_apPlayerUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + fRateOnePlayer * nCount, HEIGHT_UI_PLAYERNUMBER, 0.0f));
			m_apPlayerUI[nCount]->SetTex(D3DXVECTOR2(0.0f + fRateOnePlayer * nCount, 0.0f), D3DXVECTOR2(fRateOnePlayer + fRateOnePlayer * nCount, 1.0f));
			m_apPlayerUI[nCount]->SetIdxTexture(nIdx[0]);
			m_apPlayerUI[nCount]->SetVtx();
		}

		// エントリーUIの生成
		m_apStateUI[nCount] = CUI::Create();
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->SetSize(0.07f, 0.05f);
			m_apStateUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + fRateOnePlayer * nCount, 0.9f, 0.0f));
			m_apStateUI[nCount]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_apStateUI[nCount]->SetIdxTexture(nIdx[1]);
			m_apStateUI[nCount]->SetVtx();
		}

		// 入力マネージャーの生成
		CInputManager *pInputMgr = CInputManager::Create();
		m_apInputMgr[nCount] = pInputMgr;
	}

	// メッシュの生成
	CreateMesh();

	// 海の生成
	COcean::Create();

	// 看板配置
	CObjectX* pBanner = CObjectX::Create(BANNER_POS, BANNER_ROT, 5);
	if (pBanner != nullptr)
	{
		pBanner->BindModel(CModel::Load(&PATH_BANNER[0]));	// モデル読み込んで設定
		pBanner->SetScale(BANNER_SCALE);	// サイズ設定

		//// 球の判定生成
		//m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG::TAG_PLAYER, CCollision::TYPE::TYPE_SPHERE, pBanner);

		//if (m_pCollisionSphere != nullptr)
		//{
		//	m_pCollisionSphere->SetRadius(BANNER_COLLISION_SIZE);
		//	m_pCollisionSphere->SetPosition(D3DXVECTOR3(BANNER_POS.x, 50.0f, BANNER_POS.z + 141.0f));
		//}

		// 影の生成
		m_pShadow = CShadow::Create(4);
		if (m_pShadow != nullptr)
		{
			m_pShadow->SetPosition(D3DXVECTOR3(BANNER_POS.x, 0.0f, BANNER_POS.z + 141.0f));
		}
	}

	return S_OK;
}

//=====================================================
// メッシュの生成
//=====================================================
void CPlayerSelect::CreateMesh(void)
{
	m_pFan = CFan3D::Create(stage::PRIOLITY, stage::NUM_VTX);

	if (m_pFan == nullptr)
		return;

	m_pFan->SetPosition(D3DXVECTOR3(0.0f, stage::HEIGHT, 0.0f));
	m_pFan->SetRadius(stage::RADIUS);
	m_pFan->SetVtx();

	int nIdxTexture = Texture::GetIdx(&stage::PATH_TEX_FAN[0]);
	m_pFan->SetIdxTexture(nIdxTexture);

	m_pCylinder = CMeshCylinder::Create(stage::NUM_VTX);

	if (m_pCylinder == nullptr)
		return;

	m_pCylinder->SetHeight(stage::HEIGHT);
	m_pCylinder->SetRadius(stage::RADIUS);
	m_pCylinder->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pCylinder->SetVtx();

	nIdxTexture = Texture::GetIdx(&stage::PATH_TEX_CYLINDER[0]);
	m_pCylinder->SetIdxTexture(nIdxTexture);
}

//=====================================================
// 終了処理
//=====================================================
void CPlayerSelect::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pCylinder);
	Object::DeleteObject((CObject**)&m_pFan);
	Object::DeleteObject((CObject**)&m_pShadow);
	//Object::DeleteObject((CObject**)&m_pCollisionSphere);

	// 各種オブジェクトの破棄
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->Uninit();
			m_apStateUI[nCount] = nullptr;
		}
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->Uninit();
			m_apPlayerUI[nCount] = nullptr;
		}
		if (m_apPlayer[nCount] != nullptr)
		{
			m_apPlayer[nCount]->Uninit();
			m_apPlayer[nCount] = nullptr;
		}
		if (m_apBillboard[nCount] != nullptr)
		{
			m_apBillboard[nCount]->Uninit();
			m_apBillboard[nCount] = nullptr;
		}
	}

	// オブジェクト全棄
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayerSelect::Update(void)
{
	CFade* pFade = CFade::GetInstance();
	
	// シーンの更新
	CScene::Update();

	// 操作
	Input();

	// プレイヤーの位置制限
	LimitPlayerPos();

	// プレイヤーの重力処理
	GravityPlayer();

	// 開始するかの確認
	CheckStart();

	// プレイヤー番号ビルボード位置更新
	UpdateBillboard();
	
#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 操作
//=====================================================
void CPlayerSelect::Input(void)
{
	// フェードの開始
	CFade* pFade = CFade::GetInstance();

	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (m_apInputMgr[i] == nullptr)
			continue;

		if (m_apInputMgr[i]->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
			CreatePlayer(i);	// プレイヤーのエントリー

		if (m_apPlayer[i] != nullptr &&
			m_apInputMgr[i]->GetTrigger(CInputManager::E_Button::BUTTON_READY))
			Ready(i);	// 準備

		if (m_apInputMgr[i]->GetTrigger(CInputManager::BUTTON_BACK))	// BACK押したとき
		{
			pFade->SetFade(CScene::MODE::MODE_SELECTMODE);
		}
	}
}

//=====================================================
// プレイヤーの位置制限
//=====================================================
void CPlayerSelect::LimitPlayerPos(void)
{
	for (CPlayer *pPlayer : m_apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		universal::LimitDistCylinderInSide(stage::RADIUS_COLLIDE, &posPlayer, D3DXVECTOR3(0.0f, posPlayer.y, 0.0f));

		pPlayer->SetPosition(posPlayer);

		posPlayer.y = stage::HEIGHT;	// 影の位置をステージの高さに固定
		pPlayer->SetShadowPos(posPlayer);
	}
}

//=====================================================
// プレイヤーの重力処理
//=====================================================
void CPlayerSelect::GravityPlayer(void)
{
	for (CPlayer *pPlayer : m_apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		// 移動量の加算
		D3DXVECTOR3 move = pPlayer->GetMove();
		move.y -= GRAVITY;
		pPlayer->SetMove(move);

		// 床との判定
		D3DXVECTOR3 pos = pPlayer->GetPosition();

		if (pos.y <= stage::HEIGHT && move.y < 0)
		{
			pos.y = stage::HEIGHT;
			pPlayer->SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
		}

		pPlayer->SetPosition(pos);
	}
}

//=====================================================
// プレイヤーの生成
//=====================================================
void CPlayerSelect::CreatePlayer(int nIdx)
{
	if (m_apPlayer[nIdx] != nullptr)
		return;	// 枠が埋まってたら処理を通らない

	CSound* pSound = CSound::GetInstance();

	if (pSound == nullptr)
		return;

	// ペンギンエントリー音
	pSound->Play(CSound::LABEL_SE_PENGUIN_VOICE00);

	// テクスチャ変更
	int nIdxTexture = Texture::GetIdx(&PATH_UI_STANDBY[0]);
	if(m_apStateUI[nIdx] != nullptr)
		m_apStateUI[nIdx]->SetIdxTexture(nIdxTexture);

	// エントリー状態を設定
	m_StandbyState[nIdx] = STANDBY_PLAY;
	
	// プレイヤーの生成
	m_apPlayer[nIdx] = CPlayer::Create();
	
	if (m_apPlayer[nIdx] != nullptr)
	{
		m_apPlayer[nIdx]->ReLoadModel(&player::PATH_BODY[nIdx][0]);

		// プレイヤー初期設定
		m_apPlayer[nIdx]->SetMove(INIT_MOVE_PLAYER);
		m_apPlayer[nIdx]->SetState(CPlayer::STATE_NORMAL);
		m_apPlayer[nIdx]->SetPosition(POS_PLAYER_INIT);

		// 入力マネージャーの割り当て
		m_apPlayer[nIdx]->BindInputMgr(m_apInputMgr[nIdx]);	

		// プレイヤーIDの割り当て
		m_apPlayer[nIdx]->SetID(nIdx);

		// 頭にビルボード配置
		m_apBillboard[nIdx] = CPolygon3D::Create(D3DXVECTOR3(POS_PLAYER_INIT.x,POS_PLAYER_INIT.y + PLAYERNUM_POS_Y, POS_PLAYER_INIT.z));
		if (m_apBillboard[nIdx] != nullptr)
		{
			m_apBillboard[nIdx]->SetMode(CPolygon3D::MODE_BILLBOARD);
			m_apBillboard[nIdx]->SetSize(PLAYERNUM_SIZE, PLAYERNUM_SIZE);
			m_apBillboard[nIdx]->SetTex(D3DXVECTOR2((float)(nIdx + 1) / MAX_PLAYER, 1.0f), D3DXVECTOR2((float)(nIdx) / MAX_PLAYER, 0.0f));

			CTexture* pTexture = CTexture::GetInstance();
			if (pTexture != nullptr)
			{
				m_apBillboard[nIdx]->SetIdxTexture(pTexture->Regist(&PATH_PLAYERNUM[0]));
			}
			
			m_apBillboard[nIdx]->SetVtx();
		}

		// モーション設定
		m_apPlayer[nIdx]->SetMotion(CPlayer::MOTION::MOTION_MULTIAPPEAR);

		// joypad振動させる
		m_apPlayer[nIdx]->VibJoypad(POW_VIB_APPER, TIME_VIB_APPER);

		// パーティクルの発生
		CParticle::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CParticle::TYPE::TYPE_ENTERPLAYER);
	}

	m_nNumPlayer++;
}

//=====================================================
// 準備
//=====================================================
void CPlayerSelect::Ready(int nIdx)
{
	// エントリー状態を設定
	m_StandbyState[nIdx] = STANDBY_OK;

	// テクスチャ変更
	int nIdxTexture = Texture::GetIdx(&PATH_UI_READY[0]);
	if (m_apStateUI[nIdx] != nullptr)
		m_apStateUI[nIdx]->SetIdxTexture(nIdxTexture);
}

//=====================================================
// 開始するかの確認
//=====================================================
void CPlayerSelect::CheckStart(void)
{
	if (m_nNumPlayer <= 1)
		return;	// プレイヤーが複数人いない場合、通らない

	bool bStart = true;

	for (int i = 0; i < MAX_PLAYER; i++)
		if (m_StandbyState[i] != E_StandyrState::STANDBY_OK && m_apPlayer[i] != nullptr)
			bStart = false;

	if (bStart)
		StartFade();
}

//=====================================================
// フェードの開始
//=====================================================
void CPlayerSelect::StartFade(void)
{
	CFade* pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pFade->GetState() != CFade::FADE::FADE_NONE)
		return;

	// プレイヤーエンターフラグの設定
	vector<bool> abEnter(MAX_PLAYER);

	for (int i = 0; i < MAX_PLAYER; i++)
		abEnter[i] = m_apPlayer[i] != nullptr;

	// モードの保存
	gameManager::SaveMode(CGame::E_GameMode::MODE_MULTI, abEnter);

	// ゲームに遷移
	pFade->SetFade(CScene::MODE_SELECTSTAGE);
}

//=====================================================
// プレイヤー番号ビルボード位置更新
//=====================================================
void CPlayerSelect::UpdateBillboard(void)
{
	for (int cnt = 0; cnt < MAX_PLAYER; cnt++)
	{
		if (m_apBillboard[cnt] != nullptr)
		{
			D3DXVECTOR3 pos = m_apPlayer[cnt]->GetPosition();
			m_apBillboard[cnt]->SetPosition(D3DXVECTOR3(pos.x, pos.y + PLAYERNUM_POS_Y, pos.z));
			m_apBillboard[cnt]->SetVtx();
		}
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CPlayerSelect::Debug(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
		return;
}

//=====================================================
// 描画処理
//=====================================================
void CPlayerSelect::Draw(void)
{

}