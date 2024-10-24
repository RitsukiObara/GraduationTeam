////*****************************************************
////
//// ¬XΜ[IceSpeedManager.cpp]
//// Author:ϋόRη
////
////*****************************************************
//
////*****************************************************
//// CN[h
////*****************************************************
//#include "iceSpeedManager.h"
//#include "ice.h"
//#include "iceManager.h"
//#include "effect3D.h"
//
////*****************************************************
//// θθ`
////*****************************************************
//namespace
//{
//	const int NUM_ICE = 4;	// XΜ
//	const std::string ICE_FLOW = "data\\MOTION\\motionPenguin.txt";	// ¬κιXΜpX
//}
//
////*****************************************************
//// ΓIoΟιΎ
////*****************************************************
//
//
////=====================================================
//// RXgN^
////=====================================================
//CIceSpeedManager::CIceSpeedManager(int nPriority) : CObject(nPriority)
//{
//
//}
//
////=====================================================
//// fXgN^
////=====================================================
//CIceSpeedManager::~CIceSpeedManager()
//{
//
//}
//
////=====================================================
//// Ά¬
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
//// ϊ»
////=====================================================
//HRESULT CIceSpeedManager::Init(void)
//{
//	// XΜΆ¬
//	//CreateIce();
//
//	// Η
//	Load((char*)&ICE_FLOW[0]);
//
//	return S_OK;
//}
//
////=====================================================
//// Η
////=====================================================
//void CIceSpeedManager::Load(char* pPath)
//{
//	for (int nCntMotion = 0; nCntMotion < MAX_ICEFLOW; nCntMotion++)
//	{// p[eBNξρΜjό
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
//	//ΟιΎ
//	char cTemp[MAX_STRING];
//	int nCntMotion = 0;
//	int nCntModel = 0;
//
//	//t@C©ηΗέή
//	FILE* pFile = fopen(pPath, "r");
//
//	if (pFile != nullptr)
//	{//t@CͺJ―½κ
//		while (true)
//		{
//			//ΆΗέέ
//			(void)fscanf(pFile, "%s", &cTemp[0]);
//
//			//t@CΌΗέέ=========================================
//			if (strcmp(cTemp, "NUM_MODEL") == 0)
//			{
//				//"="Ηέέ
//				(void)fscanf(pFile, "%s", &cTemp[0]);
//
//				//fΗέέ
//				(void)fscanf(pFile, "%d", &m_nNumParts);
//
//				for (int nCntFile = 0; nCntFile < m_nNumParts;)
//				{//t@CΌΗέέ
//
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "MODEL_FILENAME") == 0)
//					{//t@CΌΗέέ
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						char aPath[MAX_STRING];
//
//						// fpXΗ
//						(void)fscanf(pFile, "%s", &aPath[0]);
//
//						m_apParts[nCntFile] = new Parts;
//
//						m_apParts[nCntFile]->pParts = CParts::Create();
//
//						int nIdx = CModel::Load(&aPath[0]);
//
//						// fΗ
//						m_apParts[nCntFile]->pParts->SetIdxModel(nIdx);
//						m_apParts[nCntFile]->pParts->BindModel(m_apParts[nCntFile]->pParts->GetIdxModel());
//
//						nCntFile++;
//					}
//				}
//			}
//			//===========================================================
//
//			//LN^[έθ===========================================
//			if (strcmp(cTemp, "CHARACTERSET") == 0)
//			{//LN^[X^[g
//				nCntModel = 0;
//
//				while (strcmp(cTemp, "END_CHARACTERSET") != 0)
//				{//IνθάΕLN^[έθ
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "PARTSSET") == 0)
//					{//p[cX^[g
//						while (strcmp(cTemp, "END_PARTSSET") != 0)
//						{//IνθάΕp[cέθ
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "INDEX") == 0)
//							{//ΤΗέέ
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdx);
//							}
//
//							if (strcmp(cTemp, "PARENT") == 0)
//							{//eΤΗέέ
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdxParent);
//							}
//
//							if (strcmp(cTemp, "POS") == 0)
//							{//ΚuΗέέ
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
//							{//ό«Ηέέ
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
//			//ΟιΎ
//			int nCntKey = 0;
//			int nCntPart = 0;
//			int nCntEvent = 0;
//
//			//¬κιXΜέθ===========================================
//			if (strcmp(cTemp, "MOTIONSET") == 0)
//			{//¬κιXΜX^[g
//				while (strcmp(cTemp, "END_MOTIONSET") != 0)
//				{//IνθάΕLN^[έθ
//
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "LOOP") == 0)
//					{//[v»f
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion].bLoop);
//					}
//
//					if (strcmp(cTemp, "NUM_KEY") == 0)
//					{//L[»f
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
//					}
//
//					if (strcmp(cTemp, "NUM_EVENT") == 0)
//					{// Cxg»f
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumEvent);
//
//						if (m_aMotionInfo[m_nNumMotion].nNumEvent != 0)
//						{// CxgξρπΆ¬
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
//					{// Cxgξρέθ
//						while (strcmp(cTemp, "END_EVENTSET") != 0)
//						{// IνθάΕCxgέθ
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "KEY") == 0)
//							{// ΔΆL[ζΎ
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nKey);
//							}
//
//							if (strcmp(cTemp, "FRAME") == 0)
//							{// ΔΆt[ζΎ
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nFrame);
//							}
//
//							if (strcmp(cTemp, "NUM_FRAME") == 0)
//							{// ΔΆt[ζΎ
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fNumFrame);
//
//								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fTimer = FLT_MAX;
//							}
//
//							if (strcmp(cTemp, "RADIUS") == 0)
//							{// Όa
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fRadius);
//							}
//
//							if (strcmp(cTemp, "POS") == 0)
//							{//ΚuΗέέ
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
//							{// ep[cΤζΎ
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
//					{//L[X^[g
//						while (strcmp(cTemp, "END_KEYSET") != 0)
//						{//IνθάΕL[έθ
//
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "FRAME") == 0)
//							{//t[ζΎ
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
//							}
//
//							if (strcmp(cTemp, "KEY") == 0)
//							{//t[ζΎ
//								while (strcmp(cTemp, "END_KEY") != 0)
//								{//IνθάΕL[έθ
//									(void)fscanf(pFile, "%s", &cTemp[0]);
//
//									if (strcmp(cTemp, "POS") == 0)
//									{//ΚuζΎ
//										(void)fscanf(pFile, "%s", &cTemp[0]);
//
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
//									}
//
//									if (strcmp(cTemp, "ROT") == 0)
//									{//ό«ζΎ
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
//		}//whileΒΆ
//
//		fclose(pFile);
//	}
//	else
//	{
//		assert(("¬κιXΜf[^ΗέέΙΈs", false));
//	}
//}
//
////=====================================================
//// XΜΆ¬
////=====================================================
//void CIceSpeedManager::CreateIce(void)
//{
//	CIceManager* pIceManager = CIceManager::GetInstance();
//
//	if (pIceManager == nullptr)
//		return;
//
//	int aV[NUM_ICE] =
//	{// cΜObhΤΜzρ
//		3,4,5,6
//	};
//	int aH[NUM_ICE] =
//	{// ‘ΜObhΤΜzρ
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
//// XπzρΙΗΑ
////=====================================================
//void CIceSpeedManager::AddIceToArray(CIce* pIce)
//{
//	m_apIce.push_back(pIce);
//}
//
////=====================================================
//// IΉ
////=====================================================
//void CIceSpeedManager::Uninit(void)
//{
//	Release();
//}
//
////=====================================================
//// XV
////=====================================================
//void CIceSpeedManager::Update(void)
//{
//	// Ηκ©ΜXͺ~άΑΔ’Θ’©Μ`FbN
//	CheckSomeIceStop();
//
//#ifdef _DEBUG
//	Debug();
//#endif
//}
//
////=====================================================
//// Ηκ©ΜXͺ~άΑΔ’Θ’©Μ`FbN
////=====================================================
//void CIceSpeedManager::CheckSomeIceStop(void)
//{
//	for (auto it : m_apIce)
//	{
//		if (it->IsStop())
//		{// ~άΑΔ’ιΘηSΔΜXπ~ίι
//			StopAllIce();
//			Uninit();	// ©gΜjό
//			break;
//		}
//	}
//}
//
////=====================================================
//// SΔΜXπ~ίι
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
//// fobO
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
//// `ζ
////=====================================================
//void CIceSpeedManager::Draw(void)
//{
//
//}