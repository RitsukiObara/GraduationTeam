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
#include "renderer.h"
#include "blur.h"
#include "MyEffekseer.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CObject *CObject::s_apTop[NUM_PRIORITY] = {};		// 先頭のオブジェクトのポインタ
CObject *CObject::m_apCur[NUM_PRIORITY] = {};		// 最後尾のオブジェクトのポインタ
list<CObject*> CObject::s_aDraw3D[NUM_PRIORITY];	// 3D描画する配列
list<CObject*> CObject::s_aDraw2D[NUM_PRIORITY];	// 2D描画する配列
int CObject::m_nNumAll = 0;	// 総数

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CObject::CObject(int nPriority)
{
	m_nNumAll++;

	// 値のクリア
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_bDeath = false;
	m_bWire = false;
	m_bZtest = false;
	m_bNotStop = false;
	m_bLighting = true;
	m_bAdd = false;
	m_bFog = true;
	m_bCull = true;
	m_bBlur = true;
	m_dAlpha = 0;

	m_nPriority = nPriority;

	if (s_apTop[nPriority] == nullptr)
	{// 先頭と最後尾アドレスの代入
		s_apTop[nPriority] = this;
		m_apCur[nPriority] = this;

		return;
	}

	// 前のアドレスに最後尾のアドレスを代入する
	m_pPrev = m_apCur[nPriority];

	// 最後尾のアドレスを自分にする
	m_apCur[nPriority] = this;

	if (m_pPrev != nullptr)
	{
		// 前のオブジェクトの次のアドレスを自分にする
		m_pPrev->m_pNext = this;
	}
}

//=====================================================
// デストラクタ
//=====================================================
CObject::~CObject()
{
	if (m_apCur[m_nPriority] != this && s_apTop[m_nPriority] != this)
	{// 真ん中のアドレスの破棄
		if (m_pPrev != nullptr)
		{
			// 前のアドレスから次のアドレスをつなぐ
			m_pPrev->m_pNext = m_pNext;
		}

		if (m_pNext != nullptr)
		{
			// 次のアドレスから前のアドレスをつなぐ
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	if (s_apTop[m_nPriority] == this)
	{// 先頭アドレスの破棄
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = nullptr;
		}

		// 先頭アドレスを次のアドレスに引き継ぐ
		s_apTop[m_nPriority] = m_pNext;
	}

	if (m_apCur[m_nPriority] == this)
	{// 最後尾アドレスの破棄
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = nullptr;
		}

		// 最後尾アドレスを前のアドレスに引き継ぐ
		m_apCur[m_nPriority] = m_pPrev;
	}

	m_nNumAll--;
}

//=====================================================
// 3Dに追加
//=====================================================
void CObject::Add3D(void)
{
	s_aDraw3D[m_nPriority].push_back(this);
}

//=====================================================
// 2Dに追加
//=====================================================
void CObject::Add2D(void)
{
	s_aDraw2D[m_nPriority].push_back(this);
}

//=====================================================
// 3D除外
//=====================================================
void CObject::Remove3D(void)
{
	s_aDraw3D[m_nPriority].remove(this);
}

//=====================================================
// 2D除外
//=====================================================
void CObject::Remove2D(void)
{
	s_aDraw2D[m_nPriority].remove(this);
}

//=====================================================
// 個別リリース処理
//=====================================================
void CObject::Release(void)
{
	// 死亡フラグを立てる
	m_bDeath = true;
}

//=====================================================
// 破棄処理
//=====================================================
void CObject::Delete(void)
{
	// 自身の破棄
	delete this;
}

// 静的メンバ関数======================================

//=====================================================
// 全リリース処理
//=====================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = s_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			// 終了処理
			pObject->Uninit();

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}

	// オブジェクトの削除
	DeleteAll();
}

//=====================================================
// 全更新処理
//=====================================================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = s_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			// 更新処理
			pObject->Update();

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}

	// オブジェクトの削除
	DeleteAll();
}

//=====================================================
// オブジェクトの削除
//=====================================================
void CObject::DeleteAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject *pObject = s_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject *pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath)
			{
				// 終了処理
				pObject->Delete();
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}
}

//=====================================================
// 全描画処理
//=====================================================
void CObject::DrawAll(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{// カメラの設定
		pCamera->SetCamera();
	}

	// オブジェクトの描画
	DrawObject(s_aDraw3D);

	if (CMyEffekseer::GetInstance() != nullptr)
	{// エフェクシアの更新
		CMyEffekseer::GetInstance()->Update();
		CMyEffekseer::GetInstance()->Draw();
	}

	// オブジェクトの描画
	DrawObject(s_aDraw2D);

	// 死亡フラグのたったオブジェクトの破棄
	DeleteAll();
}

//=====================================================
// オブジェクトの描画
//=====================================================
void CObject::DrawObject(list<CObject*> *pListDraw)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (CObject *pObject : pListDraw[nCntPri])
		{
			if (pObject->m_bWire)
			{// ワイヤーフレームの設定
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}

			if (pObject->m_bZtest)
			{// Zテストの設定
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}

			if (pObject->m_bLighting == false)
			{
				// ライティングを無効化
				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			}

			if (pObject->m_bAdd)
			{
				// 加算合成かどうか
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}

			if (pObject->m_bCull == false)
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}

			// アルファテストの有効化
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, pObject->m_dAlpha);

			// フォグの設定
			pDevice->SetRenderState(D3DRS_FOGENABLE, pObject->m_bFog && CRenderer::GetInstance()->IsFog());

			// 描画処理
			pObject->Draw();

			if (pObject->m_bWire)
			{
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}

			if (pObject->m_bZtest)
			{// Zテストの設定
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}

			if (pObject->m_bLighting == false)
			{
				// ライティングを有効化
				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			}

			if (pObject->m_bAdd)
			{
				// 加算合成を戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}

			if (pObject->m_bCull == false)
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}

			// アルファテストの無効化
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		}
	}
}

namespace Object
{
void DeleteObject(CObject **ppObject,int nSize)
{// オブジェクトの削除
	for (int i = 0; i < nSize; i++)
	{
		if (ppObject[i] != nullptr)
		{
			ppObject[i]->Uninit();
			ppObject[i] = nullptr;
		}
	}
}
}