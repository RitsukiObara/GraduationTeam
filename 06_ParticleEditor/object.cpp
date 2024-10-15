//*****************************************************
//
// オブジェクトの処理[object.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "camera.h"
#include "manager.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CObject *CObject::m_apObject[NUM_PRIORITY][NUM_OBJECT] = {};	// オブジェクトのポインタ
int CObject::m_nNumAll = 0;	// 総数

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CObject::CObject(int nPriority)
{
	// 値のクリア
	m_type = TYPE_NONE;
	m_nID = -1;

	m_nPriority = nPriority;

	for (int nCntObject = 0; nCntObject < NUM_OBJECT; nCntObject++)
	{
		if (m_apObject[nPriority][nCntObject] == NULL)
		{
			// 自分のポインタを代入
			m_apObject[nPriority][nCntObject] = this;

			// IDを記憶
			m_nID = nCntObject;

			// 総数カウントアップ
			m_nNumAll++;

			break;
		}
	}
}

//=====================================================
// デストラクタ
//=====================================================
CObject::~CObject()
{

}

//=====================================================
// 個別リリース処理
//=====================================================
void CObject::Release(void)
{
	int nId = m_nID;
	int nPriority = m_nPriority;

	if (m_apObject[nPriority][nId] != NULL)
	{
		delete m_apObject[nPriority][nId];
		m_apObject[nPriority][nId] = NULL;

		m_nNumAll--;
	}
}

// 静的メンバ関数======================================

//=====================================================
// 全リリース処理
//=====================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < NUM_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] != NULL)
			{
				m_apObject[nCntPriority][nCntObject]->Uninit();
			}
		}
	}
}

//=====================================================
// 全更新処理
//=====================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < NUM_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] != NULL)
			{
				// 更新処理
				m_apObject[nCntPriority][nCntObject]->Update();
			}
		}
	}
}

//=====================================================
// 全描画処理
//=====================================================
void CObject::DrawAll(void)
{
	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{// カメラの設定
		pCamera->SetCamera();
	}

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntObject = 0; nCntObject < NUM_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPriority][nCntObject] != NULL)
			{
				// 描画処理
				m_apObject[nCntPriority][nCntObject]->Draw();
			}
		}
	}
}