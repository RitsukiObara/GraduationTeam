//*****************************************************
//
// Q[[SelectStage.cpp]
// Author:ϋόRη
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "selectStage.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "skybox.h"
#include "selectStageManager.h"
#include "ice.h"
#include "sound.h"

//*****************************************************
// }Nθ`
//*****************************************************
namespace
{
	const char* BG_ICE = { "data\\TEXT\\selectstageIce.txt" };		// XzueLXg
	const float RADIUS_ICE = 400.0f;	// XΜΌa
}

//*****************************************************
// ΓIoΟιΎ
//*****************************************************

//=====================================================
// RXgN^
//=====================================================
CSelectStage::CSelectStage()
{

}

//=====================================================
// ϊ»
//=====================================================
HRESULT CSelectStage::Init(void)
{
	// eNXΜϊ»
	CScene::Init();

	// Xe[WIπΗNXΜΆ¬
	CSelectStageManager::Create();

	// XΜΗέέ
	Load(BG_ICE);

	// BGMΔΆ
	Sound::Play(CSound::LABEL::LABEL_BGM_SELECTSTAGE);
	
	return S_OK;
}

//=====================================================
// IΉ
//=====================================================
void CSelectStage::Uninit(void)
{
	// IuWFNgSό
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// XV
//=====================================================
void CSelectStage::Update(void)
{
	// V[ΜXV
	CScene::Update();

	// JXV
	UpdateCamera();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// JΜXV
//=====================================================
void CSelectStage::UpdateCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return;

	pCamera->Update();
}

//=====================================================
// fobO
//=====================================================
void CSelectStage::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
	{
		return;
	}
}

//=====================================================
// `ζ
//=====================================================
void CSelectStage::Draw(void)
{

}

//=====================================================
// Η
//=====================================================
void CSelectStage::Load(const char* pPath)
{
	//ΟιΎ
	char cTemp[MAX_STRING];

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	//t@C©ηΗέή
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//t@CͺJ―½κ
		while (true)
		{
			//ΆΗέέ
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "SET") == 0)
			{//L[X^[g
				while (strcmp(cTemp, "END_SET") != 0)
				{//IνθάΕL[έθ

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//ΚuζΎ
						(void)fscanf(pFile, "%s", &cTemp[0]);

						for (int nCntPos = 0; nCntPos < 3; nCntPos++)
						{
							(void)fscanf(pFile, "%f", &pos[nCntPos]);
						}
					}

					if (strcmp(cTemp, "ROT") == 0)
					{//ό«ζΎ
						(void)fscanf(pFile, "%s", &cTemp[0]);

						for (int nCntRot = 0; nCntRot < 3; nCntRot++)
						{
							(void)fscanf(pFile, "%f", &rot[nCntRot]);
						}
					}
				}

				// XΜΆ¬
				CIce* pIce = CIce::Create(CIce::TYPE_NORMAL, CIce::STATE_NORMAL);
				if (pIce == nullptr)
					return;

				// ΚuZbg
				pIce->SetPosition(pos);

				// ό«Zbg
				pIce->SetRotation(rot);

				// TCYZbg
				pIce->SetTransform(RADIUS_ICE);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
				break;
		}//while
	}//file
	else
	{
		assert(("SelectStageIceΗέέΙΈs", false));
	}

	fclose(pFile);
}