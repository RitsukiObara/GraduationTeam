////*****************************************************
////
//// ���X�̏���[IceSpeedManager.cpp]
//// Author:���R����
////
////*****************************************************
//
////*****************************************************
//// �C���N���[�h
////*****************************************************
//#include "iceSpeedManager.h"
//#include "ice.h"
//#include "iceManager.h"
//#include "effect3D.h"
//
////*****************************************************
//// �萔��`
////*****************************************************
//namespace
//{
//	const int NUM_ICE = 4;	// �X�̐�
//	const std::string ICE_FLOW = "data\\MOTION\\motionPenguin.txt";	// �����X�̃p�X
//}
//
////*****************************************************
//// �ÓI�����o�ϐ��錾
////*****************************************************
//
//
////=====================================================
//// �R���X�g���N�^
////=====================================================
//CIceSpeedManager::CIceSpeedManager(int nPriority) : CObject(nPriority)
//{
//
//}
//
////=====================================================
//// �f�X�g���N�^
////=====================================================
//CIceSpeedManager::~CIceSpeedManager()
//{
//
//}
//
////=====================================================
//// ��������
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
//// ����������
////=====================================================
//HRESULT CIceSpeedManager::Init(void)
//{
//	// �X�̐���
//	//CreateIce();
//
//	// �Ǎ�
//	Load((char*)&ICE_FLOW[0]);
//
//	return S_OK;
//}
//
////=====================================================
//// �Ǎ�����
////=====================================================
//void CIceSpeedManager::Load(char* pPath)
//{
//	for (int nCntMotion = 0; nCntMotion < MAX_ICEFLOW; nCntMotion++)
//	{// �p�[�e�B�N�����̔j��
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
//	//�ϐ��錾
//	char cTemp[MAX_STRING];
//	int nCntMotion = 0;
//	int nCntModel = 0;
//
//	//�t�@�C������ǂݍ���
//	FILE* pFile = fopen(pPath, "r");
//
//	if (pFile != nullptr)
//	{//�t�@�C�����J�����ꍇ
//		while (true)
//		{
//			//�����ǂݍ���
//			(void)fscanf(pFile, "%s", &cTemp[0]);
//
//			//�t�@�C�����ǂݍ���=========================================
//			if (strcmp(cTemp, "NUM_MODEL") == 0)
//			{
//				//"="�ǂݍ���
//				(void)fscanf(pFile, "%s", &cTemp[0]);
//
//				//���f�����ǂݍ���
//				(void)fscanf(pFile, "%d", &m_nNumParts);
//
//				for (int nCntFile = 0; nCntFile < m_nNumParts;)
//				{//�t�@�C�����ǂݍ���
//
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "MODEL_FILENAME") == 0)
//					{//�t�@�C�����ǂݍ���
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						char aPath[MAX_STRING];
//
//						// ���f���p�X�Ǎ�
//						(void)fscanf(pFile, "%s", &aPath[0]);
//
//						m_apParts[nCntFile] = new Parts;
//
//						m_apParts[nCntFile]->pParts = CParts::Create();
//
//						int nIdx = CModel::Load(&aPath[0]);
//
//						// ���f���Ǎ�
//						m_apParts[nCntFile]->pParts->SetIdxModel(nIdx);
//						m_apParts[nCntFile]->pParts->BindModel(m_apParts[nCntFile]->pParts->GetIdxModel());
//
//						nCntFile++;
//					}
//				}
//			}
//			//===========================================================
//
//			//�L�����N�^�[�ݒ�===========================================
//			if (strcmp(cTemp, "CHARACTERSET") == 0)
//			{//�L�����N�^�[�X�^�[�g
//				nCntModel = 0;
//
//				while (strcmp(cTemp, "END_CHARACTERSET") != 0)
//				{//�I���܂ŃL�����N�^�[�ݒ�
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "PARTSSET") == 0)
//					{//�p�[�c�X�^�[�g
//						while (strcmp(cTemp, "END_PARTSSET") != 0)
//						{//�I���܂Ńp�[�c�ݒ�
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "INDEX") == 0)
//							{//�ԍ��ǂݍ���
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdx);
//							}
//
//							if (strcmp(cTemp, "PARENT") == 0)
//							{//�e�ԍ��ǂݍ���
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdxParent);
//							}
//
//							if (strcmp(cTemp, "POS") == 0)
//							{//�ʒu�ǂݍ���
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
//							{//�����ǂݍ���
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
//			//�ϐ��錾
//			int nCntKey = 0;
//			int nCntPart = 0;
//			int nCntEvent = 0;
//
//			//�����X�̐ݒ�===========================================
//			if (strcmp(cTemp, "MOTIONSET") == 0)
//			{//�����X�̃X�^�[�g
//				while (strcmp(cTemp, "END_MOTIONSET") != 0)
//				{//�I���܂ŃL�����N�^�[�ݒ�
//
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "LOOP") == 0)
//					{//���[�v���f
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion].bLoop);
//					}
//
//					if (strcmp(cTemp, "NUM_KEY") == 0)
//					{//�L�[�����f
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
//					}
//
//					if (strcmp(cTemp, "NUM_EVENT") == 0)
//					{// �C�x���g�����f
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumEvent);
//
//						if (m_aMotionInfo[m_nNumMotion].nNumEvent != 0)
//						{// �C�x���g���𐶐�
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
//					{// �C�x���g���ݒ�
//						while (strcmp(cTemp, "END_EVENTSET") != 0)
//						{// �I���܂ŃC�x���g�ݒ�
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "KEY") == 0)
//							{// �Đ��L�[�擾
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nKey);
//							}
//
//							if (strcmp(cTemp, "FRAME") == 0)
//							{// �Đ��t���[���擾
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nFrame);
//							}
//
//							if (strcmp(cTemp, "NUM_FRAME") == 0)
//							{// �Đ��t���[�����擾
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fNumFrame);
//
//								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fTimer = FLT_MAX;
//							}
//
//							if (strcmp(cTemp, "RADIUS") == 0)
//							{// ���a
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fRadius);
//							}
//
//							if (strcmp(cTemp, "POS") == 0)
//							{//�ʒu�ǂݍ���
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
//							{// �e�p�[�c�ԍ��擾
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
//					{//�L�[�X�^�[�g
//						while (strcmp(cTemp, "END_KEYSET") != 0)
//						{//�I���܂ŃL�[�ݒ�
//
//							(void)fscanf(pFile, "%s", &cTemp[0]);
//
//							if (strcmp(cTemp, "FRAME") == 0)
//							{//�t���[�����擾
//								(void)fscanf(pFile, "%s", &cTemp[0]);
//
//								(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
//							}
//
//							if (strcmp(cTemp, "KEY") == 0)
//							{//�t���[�����擾
//								while (strcmp(cTemp, "END_KEY") != 0)
//								{//�I���܂ŃL�[�ݒ�
//									(void)fscanf(pFile, "%s", &cTemp[0]);
//
//									if (strcmp(cTemp, "POS") == 0)
//									{//�ʒu�擾
//										(void)fscanf(pFile, "%s", &cTemp[0]);
//
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
//										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
//									}
//
//									if (strcmp(cTemp, "ROT") == 0)
//									{//�����擾
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
//		}//while��
//
//		fclose(pFile);
//	}
//	else
//	{
//		assert(("�����X�̃f�[�^�ǂݍ��݂Ɏ��s", false));
//	}
//}
//
////=====================================================
//// �X�̐���
////=====================================================
//void CIceSpeedManager::CreateIce(void)
//{
//	CIceManager* pIceManager = CIceManager::GetInstance();
//
//	if (pIceManager == nullptr)
//		return;
//
//	int aV[NUM_ICE] =
//	{// �c�̃O���b�h�ԍ��̔z��
//		3,4,5,6
//	};
//	int aH[NUM_ICE] =
//	{// ���̃O���b�h�ԍ��̔z��
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
//// �X��z��ɒǉ�
////=====================================================
//void CIceSpeedManager::AddIceToArray(CIce* pIce)
//{
//	m_apIce.push_back(pIce);
//}
//
////=====================================================
//// �I������
////=====================================================
//void CIceSpeedManager::Uninit(void)
//{
//	Release();
//}
//
////=====================================================
//// �X�V����
////=====================================================
//void CIceSpeedManager::Update(void)
//{
//	// �ǂꂩ�̕X���~�܂��Ă��Ȃ����̃`�F�b�N
//	CheckSomeIceStop();
//
//#ifdef _DEBUG
//	Debug();
//#endif
//}
//
////=====================================================
//// �ǂꂩ�̕X���~�܂��Ă��Ȃ����̃`�F�b�N
////=====================================================
//void CIceSpeedManager::CheckSomeIceStop(void)
//{
//	for (auto it : m_apIce)
//	{
//		if (it->IsStop())
//		{// �~�܂��Ă���Ȃ�S�Ă̕X���~�߂�
//			StopAllIce();
//			Uninit();	// ���g�̔j��
//			break;
//		}
//	}
//}
//
////=====================================================
//// �S�Ă̕X���~�߂�
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
//// �f�o�b�O����
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
//// �`�揈��
////=====================================================
//void CIceSpeedManager::Draw(void)
//{
//
//}