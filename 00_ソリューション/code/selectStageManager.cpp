//*****************************************************
//
// ステージ選択管理処理[SelectStageManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectStageManager.h"
#include "skybox.h"
#include "selectStagePenguin.h"
#include "camera.h"
#include "cameraState.h"
#include "inputManager.h"
#include "debugproc.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "collision.h"
#include "manager.h"
#include "particle.h"
#include "ocean.h"
#include "gameManager.h"
#include "tutorial.h"
#include "number.h"
#include "texture.h"
#include "sound.h"
#include "iceManager.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\selectStage.txt";			// テキストパス
const string PATH_TEXT_MULTI = "data\\TEXT\\stagemulti.txt";	// マルチのテキストパス

const float RADIUS_COLLISION_PUSHOUT_STAGE = 500.0f;	// 押し出し判定の半径（ステージ）
const float RADIUS_COLLISION_PUSHOUT_BANNER = 250.0f;	// 押し出し判定の半径（看板）
const float RATE_SELECT_COLLISION = 1.4f;				// 選択時の半径の割合

const float SCALE_STATE[CSelectStageManager::E_StateStage::STATE_MAX] = { 0.0f, 1.0f, 1.4f };	// 状態ごとのスケール

const float SPEED_SCALING_STAGE = 0.3f;	// スケーリングの速度
const float TIME_ENTER = 2.0f;			// エンターにかかる時間

const float SPEED_SCALING_PENGUIN = 0.05f;	// ペンギンのスケーリング速度
const float SPEED_MOVE_ENTER = 0.01f;		// エンター時の移動速度

const float SPEED_PARTICLE = 22.0f;								// パーティクルの速度

const string PATH_BANNER = "data\\MODEL\\other\\stageselect_banner.x";	// 看板モデルのパス
const D3DXVECTOR3 BANNER_POS = D3DXVECTOR3(0.0f, 0.0, -2900.0);	// 看板の位置
const float SCALE_BANNER = 15.0f;								// 看板のスケール

const D3DXVECTOR3 INIT_POS_PLAYER = D3DXVECTOR3(0.0f, 0.0, -1300.0);	// プレイヤーの初期位置

const float HEIGHT_NUMBER = 0.14f;				// 名前の高さ
const float ADULTWALL_LENGTH = 3850.0f;			// 大人の壁
const D3DXVECTOR2 SIZE_NAME = { 0.25f, 0.09f };	// 名前のサイズ

const float DEPTH_PENGUIN = -500.0f;				// ペンギンの奥行き

//----------------------------------
// 戻るボタンUIの定数
//----------------------------------
namespace backButtonUI
{
const string PATH = "data\\TEXTURE\\UI\\B_Back.png";
const float WIDTH = 0.09f;
const float HEIGHT = 0.049f;
const D3DXVECTOR3 POS = D3DXVECTOR3(0.88f, 0.92f, 0.0f);
}
//----------------------------------
// スタートボタンUIの定数
//----------------------------------
namespace startButtonUI
{
	const string PATH = "data\\TEXTURE\\UI\\A_Start.png";
	const float WIDTH = 0.11f;
	const float HEIGHT = 0.039f;
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.78f, 0.92f, 0.0f);
}
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
vector<CSelectStageManager::S_InfoStage*> CSelectStageManager::s_aInfoStage;		// ステージ情報の配列
vector<CSelectStageManager::S_InfoStage*> CSelectStageManager::s_aInfoStageMulti;	// マルチステージ情報の配列

//=====================================================
// コンストラクタ
//=====================================================
CSelectStageManager::CSelectStageManager() : m_pBanner(nullptr),m_pPenguin(nullptr), m_bEnter(false), m_fTimerFade(0.0f), m_nIdxSelect(0), m_pButtonUIBack(nullptr)
{

}

//=====================================================
// 生成処理
//=====================================================
CSelectStageManager *CSelectStageManager::Create(void)
{
	CSelectStageManager *pManager = nullptr;

	pManager = new CSelectStageManager;

	if (pManager != nullptr)
	{
		// 初期化処理
		pManager->Init();
	}

	return pManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectStageManager::Init(void)
{
	// スカイボックスの生成
	CSkybox::Create();

	// ステージ選択看板設置
	if (m_pBanner == nullptr)
		m_pBanner = CObjectX::Create(BANNER_POS);
	
	if (m_pBanner != nullptr)
	{
		// モデルの割り当て
		m_pBanner->BindModel(CModel::Load(&PATH_BANNER[0]));
		m_pBanner->SetScale(SCALE_BANNER);

		// 看板用の判定の生成
		CCollisionSphere *pCollision = CCollisionSphere::Create(CCollision::TAG::TAG_BLOCK, CCollision::TYPE::TYPE_SPHERE, this);
		
		if (pCollision != nullptr)
		{
			pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_BANNER);
			pCollision->SetPosition(BANNER_POS);
		}
	}

	// ステージの設置
	SetStage();

	// ペンギンの生成
	m_pPenguin = CSelectStagePenguin::Create();
	if (m_pPenguin != nullptr)
	{
		int nIdxMapLast = gameManager::LoadIdxMap();

		if (nIdxMapLast == -1)
		{// 前回のマップ番号が無ければ規定位置
			m_pPenguin->SetPosition(INIT_POS_PLAYER);
		}
		else
		{// 前回のマップの前に配置
			if (nIdxMapLast >= (int)s_aInfoStage.size() ||
				nIdxMapLast < 0)
				return E_FAIL;

			D3DXVECTOR3 posPenguin = s_aInfoStage[nIdxMapLast]->pos;
			posPenguin.z += DEPTH_PENGUIN;
			m_pPenguin->SetPosition(posPenguin);
		}
	}

	// カメラのステイト設定
	Camera::ChangeState(new CCameraStateSelectStage(m_pPenguin));

	// 海の生成
	COcean::Create();

	// 氷マネージャー生成、Ocean動かすのに必要
	CIceManager::Create(0, 0);

	// ボタンUI配置
	m_pButtonUIBack = CUI::Create();
	if (m_pButtonUIBack != nullptr)
	{
		// 設定
		m_pButtonUIBack->SetIdxTexture(CTexture::GetInstance()->Regist(&backButtonUI::PATH[0]));	// テクスチャ割当
		m_pButtonUIBack->SetPosition(backButtonUI::POS);					// 位置
		m_pButtonUIBack->SetSize(backButtonUI::WIDTH, backButtonUI::HEIGHT);	// 大きさ
		m_pButtonUIBack->SetVtx();	// 頂点反映
	}

	return S_OK;
}

//=====================================================
// 読み込み処理
//=====================================================
void CSelectStageManager::Load(void)
{
	if (!s_aInfoStage.empty())
		return;

	std::ifstream file(PATH_TEXT);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// 読み込むものがなくなるまで読込
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETSTAGE")
			{// ステージ情報読込開始
				// ステージ情報の生成
				S_InfoStage *pInfoStage = new S_InfoStage;

				if (pInfoStage == nullptr)
					continue;

				LoadStage(file, temp, pInfoStage);

				s_aInfoStage.push_back(pInfoStage);
			}

			if (file.eof())
			{// 読み込み終了
				break;
			}
		}

		file.close();
	}
	else
	{
		assert(("ファイルが開けませんでした", false));
	}

	// マルチの読込
	LoadMulti();
}

//=====================================================
// マルチの読み込み処理
//=====================================================
void CSelectStageManager::LoadMulti(void)
{
	if (!s_aInfoStageMulti.empty())
		return;

	std::ifstream file(PATH_TEXT_MULTI);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// 読み込むものがなくなるまで読込
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETSTAGE")
			{// ステージ情報読込開始
				// ステージ情報の生成
				S_InfoStage *pInfoStage = new S_InfoStage;

				if (pInfoStage == nullptr)
					continue;

				LoadStage(file, temp, pInfoStage);

				s_aInfoStageMulti.push_back(pInfoStage);
			}

			if (file.eof())
			{// 読み込み終了
				break;
			}
		}

		file.close();
	}
	else
	{
		assert(("ファイルが開けませんでした", false));
	}
}

//=====================================================
// ステージの読込
//=====================================================
void CSelectStageManager::LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage)
{
	if (pInfoStage == nullptr)
		return;

	while (std::getline(file, str))
	{// 読込開始
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (key == "END_SETSTAGE")
		{// 終了
			break;
		}

		if (key == "MODEL")
		{// パス
			iss >> str >> pInfoStage->pathModel;
		}

		if (key == "POS")
		{// 位置
			iss >> str >> pInfoStage->pos.x >> pInfoStage->pos.y >> pInfoStage->pos.z;
		}

		if (key == "PATHMAP")
		{// マップのパス
			iss >> str >> pInfoStage->pathMap;
		}

		if (key == "PATHENEMY")
		{// 敵のパス
			iss >> str >> pInfoStage->pathEnemy;
		}

		if (key == "PATHNAME")
		{// ステージ名のパス
			iss >> str >> pInfoStage->pathName;
		}

		if (key == "DIR_STREAM")
		{// 海流の向き
			iss >> str >> pInfoStage->nDirStream;
		}
	}
}

//=====================================================
// ステージの設置
//=====================================================
void CSelectStageManager::SetStage(void)
{
	if (s_aInfoStage.empty())
		return;

	for (S_InfoStage *pInfoStage : s_aInfoStage)
	{
		// ステージのXモデルの設置
		pInfoStage->pModel = CObjectX::Create();

		if (pInfoStage->pModel == nullptr)
			return;

		int nIdxModel = CModel::Load(&pInfoStage->pathModel[0]);
		pInfoStage->pModel->BindModel(nIdxModel);

		pInfoStage->pModel->SetPosition(pInfoStage->pos);

		// 当たり判定の生成
		pInfoStage->pCollision = CCollisionSphere::Create(CCollision::TAG::TAG_BLOCK, CCollision::TYPE::TYPE_SPHERE, this);

		if (pInfoStage->pCollision == nullptr)
			return;

		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_STAGE);
		pInfoStage->pCollision->SetPosition(GetPosition());

		// 状態の初期設定
		pInfoStage->state = E_StateStage::STATE_NORMAL;

		if (pInfoStage != *(s_aInfoStage.end() - 1))
		{// パーティクル位置の生成
			CGameObject *pObject = new CGameObject;

			m_aParticlePos.push_back(pObject);
			pObject->SetPosition(pInfoStage->pos);
		}

		// 名前の生成
		pInfoStage->pName = CUI::Create();

		if (pInfoStage->pName == nullptr)
			return;

		D3DXVECTOR3 posScreen;
		universal::IsInScreen(pInfoStage->pos,&posScreen);
		universal::ConvertScreenRate(posScreen);

		pInfoStage->pName->SetPosition(D3DXVECTOR3(posScreen.x, posScreen.y - HEIGHT_NUMBER, 0.0f));
		pInfoStage->pName->SetSize(SIZE_NAME.x, SIZE_NAME.y);
		pInfoStage->pName->SetVtx();

		// テクスチャ反映
		int nIdxTexture = Texture::GetIdx(&pInfoStage->pathName[0]);
		pInfoStage->pName->SetIdxTexture(nIdxTexture);

		// スタートボタンUI生成
		pInfoStage->pStartButtonUI = CUI::Create();

		if (pInfoStage->pStartButtonUI == nullptr)
			return;

		// 設定
		pInfoStage->pStartButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&startButtonUI::PATH[0]));	// テクスチャ割当
		pInfoStage->pStartButtonUI->SetPosition(posScreen);					// 位置
		pInfoStage->pStartButtonUI->SetSize(0.0f, 0.0f);	// 大きさ
		pInfoStage->pStartButtonUI->SetVtx();	// 頂点反映
	}
}

//=====================================================
// 終了処理
//=====================================================
void CSelectStageManager::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pButtonUIBack);
	if (m_pBanner != nullptr)
	{
		m_pBanner->Uninit();
		m_pBanner = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectStageManager::Update(void)
{
	// 選択処理
	Select();

	if (m_bEnter)
		StayEnter();	// エンター中の処理
	else
		// モードセレクトに戻る処理
		ModeSelectBack();

	// 大人の壁判定
	CollisionAdultWall();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 選択処理
//=====================================================
void CSelectStageManager::Select(void)
{
	CSound* pSound = CSound::GetInstance();

	//=======================================
	// 各オブジェクトが常に行う処理
	//=======================================
	for (int i = 0; i < (int)s_aInfoStage.size(); i++)
	{
		S_InfoStage *pInfoStage = s_aInfoStage[i];

		// スケーリング処理
		Scaling(pInfoStage);

		// 波に追従させる
		FollowOcean(pInfoStage);

		// パーティクルを出す
		SetParticle(i);

		if (pInfoStage->pCollision == nullptr)
			continue;

		// 判定の追従
		pInfoStage->pCollision->SetPosition(pInfoStage->pModel->GetPosition());
	}

	//=======================================
	// 看板の波追従処理
	//=======================================
	if (m_pBanner != nullptr)
	{
		FollowOcean(m_pBanner);
	}

	//=======================================
	// オブジェクトを選ぶ処理
	//=======================================
	bool bEnterOnce = false;
	for (int i = 0; i < (int)s_aInfoStage.size(); i++)
	{
		S_InfoStage *pInfoStage = s_aInfoStage[i];

		if (pInfoStage->pCollision == nullptr)
			continue;

		// 判定の一時拡大
		pInfoStage->pCollision->SetRadius(RATE_SELECT_COLLISION * RADIUS_COLLISION_PUSHOUT_STAGE);

		// プレイヤーが入っていたら選択状態にする
		bool bEnter = pInfoStage->pCollision->OnEnter(CCollision::TAG_PLAYER);

		// 判定の大きさを戻す
		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_STAGE);

		if (bEnter && !bEnterOnce)
		{
			pInfoStage->state = E_StateStage::STATE_SELECT;

			// エンター操作の検出
			if (m_pPenguin != nullptr)
			{
				if (m_pPenguin->IsEnter())
				{// エンターの開始

					if (pSound != nullptr)
					{
						// ステージに入っていく音
						pSound->Play(pSound->LABEL_SE_STAGE_ENTRY);
					}

					m_nIdxSelect = i;

					StartEnter();
				}
			}

			bEnterOnce = true;
		}
		else
			pInfoStage->state = E_StateStage::STATE_NORMAL;
	}
}

//=====================================================
// スケーリング処理
//=====================================================
void CSelectStageManager::Scaling(S_InfoStage *pInfoStage)
{
	CObjectX *pModel = pInfoStage->pModel;

	if (pModel == nullptr)
		return;

	float fScaleDest = SCALE_STATE[pInfoStage->state];
	float fScale = pModel->GetScale().x;

	fScale += (fScaleDest - fScale) * SPEED_SCALING_STAGE;

	pModel->SetScale(fScale);

	// 名前の位置設定
	if (pInfoStage->pName == nullptr)
		return;

	D3DXVECTOR3 posScreen;
	universal::IsInScreen(pInfoStage->pos, &posScreen);
	universal::ConvertScreenRate(posScreen);

	pInfoStage->pName->SetPosition(D3DXVECTOR3(posScreen.x, posScreen.y - HEIGHT_NUMBER, 0.0f));
	pInfoStage->pName->SetVtx();

	// スタートボタンUIの設定
	if (pInfoStage->pStartButtonUI == nullptr)
		return;

	float fScaleUIDest = (pInfoStage->state == E_StateStage::STATE_SELECT) ? 1.0f : 0.0f;
	float fScaleUI = (pInfoStage->pStartButtonUI->GetSize().x / startButtonUI::WIDTH);
	fScaleUI += (fScaleUIDest - fScaleUI) * SPEED_SCALING_STAGE;

	pInfoStage->pStartButtonUI->SetPosition(posScreen);					// 位置
	pInfoStage->pStartButtonUI->SetSize(startButtonUI::WIDTH * fScaleUI, startButtonUI::HEIGHT * fScaleUI);	// 大きさ
	pInfoStage->pStartButtonUI->SetVtx();	// 頂点反映
}

//=====================================================
// 波に追従させる
//=====================================================
void CSelectStageManager::FollowOcean(S_InfoStage *pInfoStage)
{
	COcean*pOcean = COcean::GetInstance();
	if (pOcean == nullptr)
		return;

	if (pInfoStage->pModel == nullptr)
		return;

	D3DXVECTOR3 pos = pInfoStage->pModel->GetPosition();
	float fHeight = pOcean->GetHeight(pos, nullptr);

	pos.y = fHeight;

	pInfoStage->pModel->SetPosition(pos);
}

//=====================================================
// 波に追従させる(ObjectX版)
//=====================================================
void CSelectStageManager::FollowOcean(CObjectX* pObject)
{
	COcean* pOcean = COcean::GetInstance();
	if (pOcean == nullptr)
		return;

	if (pObject == nullptr)
		return;

	D3DXVECTOR3 pos = pObject->GetPosition();
	float fHeight = pOcean->GetHeight(pos, nullptr);

	pos.y = fHeight;

	pObject->SetPosition(pos);
}

//=====================================================
// パーティクルの発生
//=====================================================
void CSelectStageManager::SetParticle(int nIdx)
{
	if (nIdx >= (int)s_aInfoStage.size() - 1)
		return;

	// 差分ベクトルを移動速度に正規化
	D3DXVECTOR3 pos = s_aInfoStage[nIdx]->pos;
	D3DXVECTOR3 posNext = s_aInfoStage[nIdx + 1]->pos;

	D3DXVECTOR3 vecDiff = posNext - pos;

	D3DXVECTOR3 move = vecDiff;
	universal::VecConvertLength(&move, SPEED_PARTICLE);

	// 移動量を反映
	m_aParticlePos[nIdx]->Translate(move);

	// パーティクルを発生
	D3DXVECTOR3 posParticle = m_aParticlePos[nIdx]->GetPosition();
	CParticle::Create(posParticle, CParticle::TYPE::TYPE_STAGESELECT_SNOW);

	// 次の位置に一定以上近づいたら元の位置に戻す
	if (universal::DistCmpFlat(posParticle, posNext, RADIUS_COLLISION_PUSHOUT_STAGE * RATE_SELECT_COLLISION, nullptr))
	{
		D3DXVECTOR3 posBack = pos;
		D3DXVECTOR3 vecLength = vecDiff;

		universal::VecConvertLength(&vecLength, RADIUS_COLLISION_PUSHOUT_STAGE * RATE_SELECT_COLLISION);

		posBack += vecLength;

		m_aParticlePos[nIdx]->SetPosition(posBack);
	}
}

//=====================================================
// エンター開始
//=====================================================
void CSelectStageManager::StartEnter(void)
{
	// タイマーのリセット
	m_fTimerFade = 0.0f;

	// エンターフラグを立てる
	m_bEnter = true;

	// ペンギンを操作不能にする
	if (m_pPenguin != nullptr)
		m_pPenguin->EnableInput(false);

	// 当たり判定の削除
	for (S_InfoStage *pInfoStage : s_aInfoStage)
	{
		if (pInfoStage->pCollision != nullptr)
		{
			pInfoStage->pCollision->Uninit();
			pInfoStage->pCollision = nullptr;
		}
	}

	// ステージ番号保存
	gameManager::SaveIdxMap(m_nIdxSelect);
}

//=====================================================
// エンター中の処理
//=====================================================
void CSelectStageManager::StayEnter(void)
{
	m_fTimerFade += CManager::GetDeltaTime();

	if (TIME_ENTER < m_fTimerFade)
		EndEnter();	// エンターの終了

	if (m_pPenguin == nullptr)
		return;	// 以下、ペンギンの処理

	// ステージに引っ張られる処理
	D3DXVECTOR3 pos = m_pPenguin->GetPosition();
	D3DXVECTOR3 posStage = s_aInfoStage[m_nIdxSelect]->pos;

	universal::MoveToDest(&pos, posStage, SPEED_MOVE_ENTER);

	m_pPenguin->SetPosition(pos);

	// スケーリングの処理
	float fScale = m_pPenguin->GetParts(0)->pParts->GetScale().x;

	fScale += (0 - fScale) * SPEED_SCALING_PENGUIN;

	m_pPenguin->GetParts(0)->pParts->SetScale(fScale);
}

//=====================================================
// エンター終了
//=====================================================
void CSelectStageManager::EndEnter(void)
{
	// フェードの開始
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	bool bEnterTutorial = tutorial::LoadFrag();

	if(bEnterTutorial)
		pFade->SetFade(CScene::MODE::MODE_TUTORIAL);
	else
		pFade->SetFade(CScene::MODE::MODE_GAME);
}

//=====================================================
// 大人の壁判定
//=====================================================
void CSelectStageManager::CollisionAdultWall(void)
{
	D3DXVECTOR3 pos = m_pPenguin->GetPosition();
	float length = D3DXVec3Length(&pos);	// 距離計算

	if (length >= ADULTWALL_LENGTH)
	{// 大人の壁範囲外
		D3DXVECTOR3 vec;
		D3DXVec3Normalize(&vec, &pos);	// 正規化
		pos = vec * ADULTWALL_LENGTH;	// 大人の壁範囲内に収める
		m_pPenguin->SetPosition(pos);	// 位置設定
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CSelectStageManager::Debug(void)
{
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();

	if (pDebugProc == nullptr || pKeyboard == nullptr)
		return;

	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pKeyboard->GetTrigger(DIK_1))
		pFade->SetFade(CScene::MODE::MODE_GAME);
}

//=====================================================
// 描画処理
//=====================================================
void CSelectStageManager::Draw(void)
{

}

//=====================================================
// モードセレクトに戻る処理
//=====================================================
void CSelectStageManager::ModeSelectBack(void)
{
	// フェードの開始
	CFade* pFade = CFade::GetInstance();

	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	if (pInput->GetTrigger(CInputManager::BUTTON_BACK))	// BACK押したとき
	{
		pFade->SetFade(CScene::MODE::MODE_SELECTMODE);
	}
}