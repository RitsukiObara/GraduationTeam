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
const float SIZE_WIDTH_PLAYERNUMBER = 0.05f;	// プレイヤーナンバーUIのサイズ(幅)
const float SIZE_HEIGHT_PLAYERNUMBER = 0.07f;	// プレイヤーナンバーUIのサイズ(高さ)
const float HEIGHT_UI_PLAYERNUMBER = 0.8f;	// プレイヤーナンバーUIの位置の高さ

const string PATH_UI_STANDBY = "data\\TEXTURE\\UI\\standby.png";		// スタンドバイテクスチャのパス
const string PATH_UI_READY = "data\\TEXTURE\\UI\\ready.png";			// 準備完了テクスチャのパス
const string PATH_BANNER = "data\\MODEL\\other\\entry_banner.x";		// 看板のモデルパス
const D3DXVECTOR3 BANNER_POS = D3DXVECTOR3(0.0f, 0.0f, 500.0f);			// 看板の初期の位置
const D3DXVECTOR3 BANNER_ROT = D3DXVECTOR3(0.1f, 0.0f, 0.07f);			// 看板の初期の向き
const float BANNER_SCALE = 6.5f;										// 看板のサイズ
const float BANNER_COLLISION_SIZE = 20.0f;								// 看板の判定のサイズ

const D3DXVECTOR3 INIT_MOVE_PLAYER = D3DXVECTOR3(0.0f, 37.0f, 0.0f);	// プレイヤーの初期の移動量
const D3DXVECTOR3 POS_PLAYER_INIT = D3DXVECTOR3(0.0f, -200.0f, 0.0f);	// プレイヤーの初期の位置

const float PLAYERNUM_SIZE = 80.0f;
const float PLAYERNUM_POS_Y = 250.0f;

const float GRAVITY = 0.6f;	// 重力

const float POW_VIB_APPER = 0.6f;	// 出現時の振動強さ
const int TIME_VIB_APPER = 15;		// 出現時の振動長さ

const float TIME_FADE = 1.2f;		// フェードにかかる時間
const float FACT_ROT_FADE = 0.4f;	// フェード時の回転係数

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
//----------------------------------
// ボタンUIの定数
//----------------------------------
namespace buttonUI
{
	const string PATH = "data\\TEXTURE\\UI\\B_Back.png";
	const float WIDTH = 0.09f;
	const float HEIGHT = 0.049f;
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.12f, 0.08f, 0.0f);
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
	ZeroMemory(&m_apInputMgr[0], sizeof(m_apInputMgr));
	m_pCylinder = nullptr;
	m_pFan = nullptr;
	m_pShadow = nullptr;
	m_pButtonUI = nullptr;
	m_bFade = false;
	m_fTimerFade = 0.0f;
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
			m_apPlayerUI[nCount]->SetSize(SIZE_WIDTH_PLAYERNUMBER, SIZE_HEIGHT_PLAYERNUMBER);
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

	// ボタンUI配置
	m_pButtonUI = CUI::Create();
	if (m_pButtonUI != nullptr)
	{
		// 設定
		m_pButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&buttonUI::PATH[0]));	// テクスチャ割当
		m_pButtonUI->SetPosition(buttonUI::POS);					// 位置
		m_pButtonUI->SetSize(buttonUI::WIDTH, buttonUI::HEIGHT);	// 大きさ
		m_pButtonUI->SetVtx();	// 頂点反映
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

		// 影の生成
		m_pShadow = CShadow::Create(4);
		if (m_pShadow != nullptr)
		{
			m_pShadow->SetPosition(D3DXVECTOR3(BANNER_POS.x, 0.0f, BANNER_POS.z + 141.0f));
		}
	}

	// BGM再生
	Sound::Play(CSound::LABEL::LABEL_BGM_SELECTPLAYER);

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
	Object::DeleteObject((CObject**)&m_pButtonUI);

	// プレイヤーの破棄
	for (const auto& pair : m_mapPlayer)
	{
		CPlayer *pPlayer = pair.second;

		if (pPlayer != nullptr)
		{
			pPlayer->Uninit();
			pPlayer = nullptr;
		}
	}
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

	// フェードの処理
	Fade();

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
		{
			CreatePlayer(m_nNumPlayer, i);	// プレイヤーのエントリー
		}

		if (m_mapPlayer[m_apInputMgr[i]] != nullptr &&	
			m_apInputMgr[i]->GetTrigger(CInputManager::E_Button::BUTTON_READY))
		{// プレイヤーがエントリーしていて準備ボタンで準備OK
			int nIDPlayer = m_mapPlayer[m_apInputMgr[i]]->GetID();
			Ready(nIDPlayer);	// 準備
		}

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
	for (const auto& pair : m_mapPlayer)
	{
		CPlayer *pPlayer = pair.second;

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
	for (const auto& pair : m_mapPlayer)
	{
		CPlayer *pPlayer = pair.second;

		if (pPlayer == nullptr)
			continue;

		// 移動量の加算
		D3DXVECTOR3 move = pPlayer->GetMove();

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
void CPlayerSelect::CreatePlayer(int nIdx, int nIdxInput)
{
	CInputManager *pInputMgr = m_apInputMgr[nIdxInput];

	if (m_mapPlayer[pInputMgr] != nullptr)
		return;	// 枠が埋まってたら処理を通らない

	// 入力番号保存
	m_aIdxInput.push_back(nIdxInput);

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
	m_mapPlayer[pInputMgr] = CPlayer::Create();
	
	if (m_mapPlayer[pInputMgr] != nullptr)
	{
		CPlayer *pPlayer = m_mapPlayer[pInputMgr];

		pPlayer->ReLoadModel(&player::PATH_BODY[nIdx][0]);

		// プレイヤー初期設定
		pPlayer->SetMove(INIT_MOVE_PLAYER);
		pPlayer->SetState(CPlayer::STATE_NORMAL);
		pPlayer->SetPosition(POS_PLAYER_INIT);

		// 入力マネージャーの割り当て
		pPlayer->BindInputMgr(pInputMgr);

		// プレイヤーIDの割り当て
		pPlayer->SetID(nIdx);

		// モーション設定
		pPlayer->SetMotion(CPlayer::MOTION::MOTION_MULTIAPPEAR);

		// joypad振動させる
		pPlayer->VibJoypad(POW_VIB_APPER, TIME_VIB_APPER);

		// プレイヤーナンバー表示
		pPlayer->ShowNumber();

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
	if (m_bFade)
		return;

	if (m_nNumPlayer <= 1)
		return;	// プレイヤーが複数人いない場合、通らない

	bool bStart = true;


	for (int i = 0; i < m_nNumPlayer; i++)
	{
		CPlayer *pPlayer = m_mapPlayer[m_apInputMgr[i]];

		// 準備完了していない時またはプレイヤーが存在しない時
		if (m_StandbyState[i] != E_StandyrState::STANDBY_OK || pPlayer == nullptr)
			bStart = false;
	}

	if (bStart)
	{
		// SE再生
		Sound::Play(CSound::LABEL::LABEL_SE_MULTISTART);

		StartFade();
	}
}

//=====================================================
// フェードの開始
//=====================================================
void CPlayerSelect::StartFade(void)
{
	// プレイヤーエンターフラグの設定
	vector<bool> abEnter(m_nNumPlayer);

	for (int i = 0; i < m_nNumPlayer; i++)
	{
		CPlayer *pPlayer = m_mapPlayer[m_apInputMgr[i]];

		if (pPlayer != nullptr)
		{
			abEnter[i] = true;
			int nMotion = pPlayer->GetMotion();
			if(nMotion != CPlayer::MOTION::MOTION_GUTS)
				pPlayer->SetMotion(CPlayer::MOTION::MOTION_GUTS);

			pPlayer->EnableInput(false);
		}
	}

	// モードの保存
	gameManager::SaveMode(CGame::E_GameMode::MODE_MULTI, abEnter);

	// 入力番号の保存
	gameManager::SaveIdxInput(m_aIdxInput);

	// フェードフラグを立てる
	m_bFade = true;
}

//=====================================================
// フェードの処理
//=====================================================
void CPlayerSelect::Fade(void)
{
	if (!m_bFade)
		return;

	// 一定時間になったらフェード
	m_fTimerFade += CManager::GetDeltaTime();

	if (m_fTimerFade > TIME_FADE)
	{
		CFade* pFade = CFade::GetInstance();

		if (pFade == nullptr)
			return;

		pFade->SetFade(CScene::MODE_GAME);
	}

	// プレイヤーをカメラの向きに向ける
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();
	CCamera *pCamera = CManager::GetCamera();
	CCamera::Camera *pInfoCamera = nullptr;

	if (pCamera != nullptr)
		pInfoCamera = pCamera->GetCamera();

	if (pInfoCamera == nullptr)
		return;

	D3DXVECTOR3 posCamera = pInfoCamera->posV;

	for (CPlayer *pPlayer : apPlayer)
	{
		// カメラとの差分距離をとる
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
		D3DXVECTOR3 vecDiff = posCamera - posPlayer;

		// 目標角度を設定して、向きを補正
		float fAngleDest = 0.0f;

		fAngleDest = atan2f(vecDiff.x, vecDiff.z);

		D3DXVECTOR3 rotPlayer = pPlayer->GetRotation();
		universal::FactingRot(&rotPlayer.y, fAngleDest + D3DX_PI, FACT_ROT_FADE);
		pPlayer->SetRotation(rotPlayer);
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CPlayerSelect::Debug(void)
{
	int nCount = 0;	// カウント用引数

	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	pDebugProc->Print("\n\n=====エントリー画面デバッグ=====", m_nNumPlayer);
	// エントリー人数の表示
	pDebugProc->Print("\nエントリー人数：%d", m_nNumPlayer);
	for (int i = 0; i < m_nNumPlayer; i++)
	{
		if (m_StandbyState[i] == E_StandyrState::STANDBY_OK)
			nCount++;
	}
	// 準備完了人数の表示
	pDebugProc->Print("\n準備完了人数：%d\n", nCount);

	if (pKeyboard == nullptr || pDebugProc == nullptr)
		return;
}

//=====================================================
// 描画処理
//=====================================================
void CPlayerSelect::Draw(void)
{

}