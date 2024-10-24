////*****************************************************
////
//// 流氷の処理[IceSpeedManager.cpp]
//// Author:髙山桃也
////
////*****************************************************
//
////*****************************************************
//// インクルード
////*****************************************************
//#include "iceSpeedManager.h"
//#include "ice.h"
//#include "iceManager.h"
//#include "effect3D.h"
//
////*****************************************************
//// 定数定義
////*****************************************************
//namespace
//{
//	const int NUM_ICE = 4;	// 氷の数
//	const std::string ICE_FLOW = "data\\MOTION\\motionPenguin.txt";	// 流れる氷のパス
//}
//
////*****************************************************
//// 静的メンバ変数宣言
////*****************************************************
//
//
////=====================================================
//// コンストラクタ
////=====================================================
//CIceSpeedManager::CIceSpeedManager(int nPriority) : CObject(nPriority)
//{
//
//}
//
////=====================================================
//// デストラクタ
////=====================================================
//CIceSpeedManager::~CIceSpeedManager()
//{
//
//}
//
////=====================================================
//// 生成処理
////=====================================================
//CIceSpeedManager* CIceSpeedManager::Create(void)
//{
//	CIceSpeedManager* pIceSpeedManager = nullptr;
//
//	pIceSpeedManager = new CIceSpeedManager;
//
//	if (pIceSpeedManager == nullptr)
//		return nullptr;
//
//	pIceSpeedManager->Init();
//
//	return pIceSpeedManager;
//}
//
////=====================================================
//// 初期化処理
////=====================================================
//HRESULT CIceSpeedManager::Init(void)
//{
//	// 氷の生成
//	//CreateIce();
//
//	// 読込
//	Load((char*)&ICE_FLOW[0]);
//
//	return S_OK;
//}
//
////=====================================================
//// 読込処理
////=====================================================
//void CIceSpeedManager::Load(char* pPath)
//{
//	for (int nCntMotion = 0; nCntMotion < MAX_ICEFLOW; nCntMotion++)
//	{// パーティクル情報の破棄
//		if (m_aMotionInfo[nCntMotion].pEvent != nullptr)
//		{
//			delete m_aMotionInfo[nCntMotion].pEvent;
//			m_aMotionInfo[nCntMotion].pEvent = nullptr;
//		}
//	}
//
//	//ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
//	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
//	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
//	//ZeroMemory(&m_abMotion[0], sizeof(m_abMotion));
//	m_nKey = 0;
//	m_nNumMotion = 0;
//
//	//変数宣言
//	char cTemp[MAX_STRING];
//	int nCntMotion = 0;
//	int nCntModel = 0;
//
//	//ファイルから読み込む
//	FILE* pFile = fopen(pPath, "r");
//
//	if (pFile != nullptr)
//	{//ファイルが開けた場合
//		while (true)
//		{
//			//文字読み込み
//			(void)fscanf(pFile, "%s", &cTemp[0]);
//
//			//ファイル名読み込み=========================================
//			if (strcmp(cTemp, "NUM_MODEL") == 0)
//			{
//				//"="読み込み
//				(void)fscanf(pFile, "%s", &cTemp[0]);
//
//				//モデル数読み込み
//				(void)fscanf(pFile, "%d", &m_nNumParts);
//
//				for (int nCntFile = 0; nCntFile < m_nNumParts;)
//				{//ファイル名読み込み
//
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "MODEL_FILENAME") == 0)
//					{//ファイル名読み込み
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						char aPath[MAX_STRING];
//
//						// モデルパス読込
//						(void)fscanf(pFile, "%s", &aPath[0]);
//
//						m_apParts[nCntFile] = new Parts;
//
//						m_apParts[nCntFile]->pParts = CParts::Create();
//
//						int nIdx = CModel::Load(&aPath[0]);
//
//						// モデル読込
//						m_apParts[nCntFile]->pParts->SetIdxModel(nIdx);
//						m_apParts[nCntFile]->pParts->BindModel(m_apParts[nCntFile]->pParts->GetIdxModel());
//
//						nCntFile++;
//					}
//				}
//			}
//			//===========================================================
//
//			//キャラクター設定===========================================
//			if (strcmp(cTemp, "CHARACTERSET") == 0)
//			{//キャラクタースタート
//				nCntModel = 0;
//
//				while (strcmp(cTemp, "END_CHARACTERSET") != 0)
//				{//終わりまでキャラクター設定
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "PARTSSET") == 0)
//					{//パーツスタート
//						while (strcmp(cTemp, "END_PARTSSET") != 0)
//						{//終わりまでパーツ設定
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "INDEX") == 0)
//							{//番号読み込み
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdx);
//							}
//
//							if (strcmp(cTemp, "PARENT") == 0)
//							{//親番号読み込み
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdxParent);
//							}
//
//							if (strcmp(cTemp, "POS") == 0)
//							{//位置読み込み
//								D3DXVECTOR3 pos;
//
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
//								{
//									(void)fscanf(pFile, "%f", &pos[nCntPos]);
//								}
//
//								m_apParts[nCntModel]->pParts->SetPosition(pos);
//
//								m_apParts[nCntModel]->pParts->SetPosOrg(pos);
//							}
//
//							if (strcmp(cTemp, "ROT") == 0)
//							{//向き読み込み
//								D3DXVECTOR3 rot;
//
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								for (int nCntRot = 0; nCntRot < 3; nCntRot++)
//								{
//									(void)fscanf(pFile, "%f", &rot[nCntRot]);
//								}
//
//								m_apParts[nCntModel]->pParts->SetRotation(rot);
//							}
//
//						}//END_PART
//
//						nCntModel++;
//					}
//				}//END_CHAR
//			}
//			//===========================================================
//
//			//変数宣言
//			int nCntKey = 0;
//			int nCntPart = 0;
//			int nCntEvent = 0;
//
//			//流れる氷の設定===========================================
//			if (strcmp(cTemp, "MOTIONSET") == 0)
//			{//流れる氷のスタート
//				while (strcmp(cTemp, "END_MOTIONSET") != 0)
//				{//終わりまでキャラクター設定
//
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "LOOP") == 0)
//					{//ループ判断
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion].bLoop);
//					}
//
//					if (strcmp(cTemp, "NUM_KEY") == 0)
//					{//キー数判断
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
//					}
//
//					if (strcmp(cTemp, "NUM_EVENT") == 0)
//					{// イベント数判断
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumEvent);
//
//						if (m_aMotionInfo[m_nNumMotion].nNumEvent != 0)
//						{// イベント情報を生成
//							m_aMotionInfo[m_nNumMotion].pEvent = new EVENT_INFO[m_aMotionInfo[m_nNumMotion].nNumEvent];
//
//							ZeroMemory(m_aMotionInfo[m_nNumMotion].pEvent, sizeof(EVENT_INFO) * m_aMotionInfo[m_nNumMotion].nNumEvent);
//
//							for (int i = 0; i < m_aMotionInfo[m_nNumMotion].nNumEvent; i++)
//							{
//								m_aMotionInfo[m_nNumMotion].pEvent->fTimer = FLT_MAX;
//							}
//						}
//					}
//
//					if (strcmp(cTemp, "EVENTSET") == 0 && m_aMotionInfo[m_nNumMotion].pEvent != 0)
//					{// イベント情報設定
//						while (strcmp(cTemp, "END_EVENTSET") != 0)
//						{// 終わりまでイベント設定
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "KEY") == 0)
//							{// 再生キー取得
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nKey);
//							}
//
//							if (strcmp(cTemp, "FRAME") == 0)
//							{// 再生フレーム取得
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nFrame);
//							}
//
//							if (strcmp(cTemp, "NUM_FRAME") == 0)
//							{// 再生フレーム数取得
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fNumFrame);
//
//								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fTimer = FLT_MAX;
//							}
//
//							if (strcmp(cTemp, "RADIUS") == 0)
//							{// 半径
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fRadius);
//							}
//
//							if (strcmp(cTemp, "POS") == 0)
//							{//位置読み込み
//								D3DXVECTOR3 pos;
//
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
//								{
//									(void)fscanf(pFile, "%f", &pos[nCntPos]);
//								}
//
//								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].offset = pos;
//							}
//
//							if (strcmp(cTemp, "PARENT") == 0)
//							{// 親パーツ番号取得
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nIdxParent);
//							}
//						}
//
//						nCntEvent++;
//					}
//
//					if (strcmp(cTemp, "KEYSET") == 0)
//					{//キースタート
//						while (strcmp(cTemp, "END_KEYSET") != 0)
//						{//終わりまでキー設定
//
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "FRAME") == 0)
//							{//フレーム数取得
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
//							}
//
//							if (strcmp(cTemp, "KEY") == 0)
//							{//フレーム数取得
//								while (strcmp(cTemp, "END_KEY") != 0)
//								{//終わりまでキー設定
//									(void)fscanf(pFile, "%s", &cTemp[0]);
//
//									if (strcmp(cTemp, "POS") == 0)
//									{//位置取得
//										(void)fscanf(pFile, "%s", &cTemp[0]);
//
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
//									}
//
//									if (strcmp(cTemp, "ROT") == 0)
//									{//向き取得
//										(void)fscanf(pFile, "%s", &cTemp[0]);
//
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
//									}
//								}
//
//								nCntPart++;
//
//							}//KEYif
//						}//ENDKEYwhile
//
//						nCntKey++;
//						nCntPart = 0;
//					}
//				}//MOTIONwhile
//
//				nCntMotion++;
//				m_nNumMotion++;
//			}
//			//===========================================================
//
//			if (strcmp(cTemp, "END_SCRIPT") == 0)
//			{
//				break;
//			}
//		}//while閉じ
//
//		fclose(pFile);
//	}
//	else
//	{
//		assert(("流れる氷のデータ読み込みに失敗", false));
//	}
//}
//
////=====================================================
//// 氷の生成
////=====================================================
//void CIceSpeedManager::CreateIce(void)
//{
//	CIceManager* pIceManager = CIceManager::GetInstance();
//
//	if (pIceManager == nullptr)
//		return;
//
//	int aV[NUM_ICE] =
//	{// 縦のグリッド番号の配列
//		3,4,5,6
//	};
//	int aH[NUM_ICE] =
//	{// 横のグリッド番号の配列
//		9,9,9,9
//	};
//
//	for (int i = 0; i < NUM_ICE; i++)
//	{
//		CIce* pIce = pIceManager->CreateIce(aV[i], aH[i]);
//		pIce->ChangeState(new CIceStateFlow);
//		m_apIce.push_back(pIce);
//	}
//}
//
////=====================================================
//// 氷を配列に追加
////=====================================================
//void CIceSpeedManager::AddIceToArray(CIce* pIce)
//{
//	m_apIce.push_back(pIce);
//}
//
////=====================================================
//// 終了処理
////=====================================================
//void CIceSpeedManager::Uninit(void)
//{
//	Release();
//}
//
////=====================================================
//// 更新処理
////=====================================================
//void CIceSpeedManager::Update(void)
//{
//	// どれかの氷が止まっていないかのチェック
//	CheckSomeIceStop();
//
//#ifdef _DEBUG
//	Debug();
//#endif
//}
//
////=====================================================
//// どれかの氷が止まっていないかのチェック
////=====================================================
//void CIceSpeedManager::CheckSomeIceStop(void)
//{
//	for (auto it : m_apIce)
//	{
//		if (it->IsStop())
//		{// 止まっているなら全ての氷を止める
//			StopAllIce();
//			Uninit();	// 自身の破棄
//			break;
//		}
//	}
//}
//
////=====================================================
//// 全ての氷を止める
////=====================================================
//void CIceSpeedManager::StopAllIce(void)
//{
//	CIceManager* pIceManager = CIceManager::GetInstance();
//
//	if (pIceManager == nullptr)
//		return;
//
//	for (auto it : m_apIce)
//	{
//		it->ChangeState(new CIceStaeteNormal);
//
//		pIceManager->AddIce(it, it->GetPosition());
//	}
//}
//
////=====================================================
//// デバッグ処理
////=====================================================
//void CIceSpeedManager::Debug(void)
//{
//	for (auto it : m_apIce)
//	{
//		D3DXVECTOR3 posIce = it->GetPosition();
//
//		CEffect3D::Create(posIce, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//	}
//}
//
////=====================================================
//// 描画処理
////=====================================================
//void CIceSpeedManager::Draw(void)
//{
//
//}