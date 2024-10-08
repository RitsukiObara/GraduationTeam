//*****************************************************
//
// スカイボックスの処理[IceManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "iceManager.h"
#include "effect3D.h"
#include "ice.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CIceManager *CIceManager::s_pIceManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CIceManager::CIceManager(int nPriority) : CObject(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CIceManager::~CIceManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CIceManager *CIceManager::Create(int nNumV, int nNumH)
{
	if (s_pIceManager == nullptr)
	{
		s_pIceManager = new CIceManager;

		if (s_pIceManager != nullptr)
		{
			s_pIceManager->m_nNumGridVirtical = nNumV;
			s_pIceManager->m_nNumGridHorizontal = nNumH;
			s_pIceManager->Init();
		}
	}

	return s_pIceManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIceManager::Init(void)
{
	// 指定された数分だけグリッド配列をリサイズ
	// 縦のグリッド用意
	m_aGrid.resize(m_nNumGridVirtical);

	for (int i = 0; i < m_nNumGridVirtical; i++)
		m_aGrid[i].resize(m_nNumGridHorizontal);

	// グリッドの位置設定
	SetGridPos();

	// 仮マップ生成
	CreateIce(3, 6);
	CreateIce(3, 5);
	CreateIce(3, 4);
	CreateIce(3, 3);
	CreateIce(4, 3);
	CreateIce(5, 3);
	CreateIce(5, 8);
	CreateIce(5, 7);
	CreateIce(5, 6);
	CreateIce(5, 5);
	CreateIce(5, 4);
	CreateIce(4, 6);
	CreateIce(6, 6);

	return S_OK;
}

//=====================================================
// グリッドの位置を設定
//=====================================================
void CIceManager::SetGridPos(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			D3DXVECTOR3 pos;
			pos = { j * Grid::SIZE - Grid::SIZE * m_nNumGridHorizontal * 0.5f ,10.0f,i * Grid::SIZE * 0.67f - Grid::SIZE * m_nNumGridVirtical * 0.5f };

			// 縦で偶数列だったらずらす
			if (i % 2 == 0)
			{
				pos.x += Grid::SIZE * 0.5f;
			}

			m_aGrid[i][j].pos = pos;
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CIceManager::Uninit(void)
{
	s_pIceManager = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CIceManager::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif

	// 氷の状態管理
	ManageStateIce();
}

//=====================================================
// 氷の状態管理
//=====================================================
void CIceManager::ManageStateIce(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{
				m_aGrid[i][j].pIce->EnableBreak(false);
				m_aGrid[i][j].pIce->EnableCanFind(true);
			}
		}
	}
}

//=====================================================
// 氷の生成
//=====================================================
CIce *CIceManager::CreateIce(int nGridV, int nGridH)
{
	CIce *pIce = nullptr;

	pIce = CIce::Create();

	if (pIce == nullptr)
		return nullptr;

	// 氷のトランスフォーム設定
	pIce->SetPosition(m_aGrid[nGridV][nGridH].pos);
	pIce->SetSize(Grid::SIZE * 0.5f, Grid::SIZE * 0.5f);

	// 氷を配列にセット
	m_aGrid[nGridV][nGridH].pIce = pIce;

	// 氷の停止
	StopIce(pIce);

	return pIce;
}

//=====================================================
// 氷の停止
//=====================================================
void CIceManager::StopIce(CIce *pIce)
{
	if (pIce == nullptr)
		return;

	pIce->SetState(CIce::E_State::STATE_STOP);

	// グリッドに属性を割り振る
}

//=====================================================
// 氷をつつく
//=====================================================
void CIceManager::PeckIce(int nNumV, int nNumH, E_Direction direction)
{
	if (m_aGrid[nNumV][nNumH].pIce != nullptr)
	{
		// 今いる氷を見つけられないようにする
		m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);

		// 氷破壊フラグをたてる
		m_bBreakIce = true;
	}

	CIce *pIceStand = m_aGrid[nNumV][nNumH].pIce;

	switch (direction)
	{
	/*case CIceManager::DIRECTION_UP:
		nNumV++;
		break;*/
	case CIceManager::DIRECTION_RIGHT:
		nNumH++;
		break;
	/*case CIceManager::DIRECTION_DOWN:
		nNumV--;
		break;*/
	case CIceManager::DIRECTION_LEFT:
		nNumH--;
		break;
	default:
		break;
	}

	// 氷を突っついた判定にする
	if (m_aGrid[nNumV][nNumH].pIce)
	{
		m_aGrid[nNumV][nNumH].pIce->EnablePeck(true);
		m_aGrid[nNumV][nNumH].pIce->EnableBreak(true);
	}

	// 氷探索の再帰関数
	FindIce(nNumV, nNumH, 0, pIceStand);

	// 氷が壊れるフラグが立っていたら氷を壊す
	if (m_bBreakIce)
		BreakIce();
}

//=====================================================
// 氷の探索
//=====================================================
void CIceManager::FindIce(int nNumV, int nNumH, int nIdx, CIce *pIceStand)
{
	if (m_aGrid[nNumV][nNumH].pIce != nullptr)
	{
		// 探索済みフラグを立てる
		m_aGrid[nNumV][nNumH].pIce->EnableCanFind(false);
		m_aGrid[nNumV][nNumH].pIce->EnableBreak(true);

		CEffect3D::Create(m_aGrid[nNumV][nNumH].pIce->GetPosition(), 50.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		nIdx++;
	}

	vector<CIce*> apIce(DIRECTION_MAX);

	int aV[DIRECTION_MAX] = {};
	int aH[DIRECTION_MAX] = {};

	// 右上
	aV[DIRECTION_RIGHTUP] = nNumV + 1;
	aV[DIRECTION_RIGHTDOWN] = nNumV - 1;

	if (nNumV % 2 == 0)
	{// 偶数の時
		// グリッド番号を保存
		aH[DIRECTION_RIGHTUP] = nNumH + 1;
		aH[DIRECTION_RIGHTDOWN] = nNumH + 1;

		// 左ななめ
		aH[DIRECTION_LEFTUP] = nNumH;
		aH[DIRECTION_LEFTDOWN] = nNumH;
	}
	else
	{// 奇数の時

		// グリッド番号を保存
		aH[DIRECTION_RIGHTUP] = nNumH;
		aH[DIRECTION_RIGHTDOWN] = nNumH;

		// 左ななめ
		aH[DIRECTION_LEFTUP] = nNumH - 1;
		aH[DIRECTION_LEFTDOWN] = nNumH - 1;
	}

	// 左側
	aV[DIRECTION_LEFTUP] = nNumV + 1;
	aV[DIRECTION_LEFTDOWN] = nNumV - 1;

	// 左右
	aV[DIRECTION_RIGHT] = nNumV;
	aV[DIRECTION_LEFT] = nNumV;
	aH[DIRECTION_RIGHT] = nNumH + 1;
	aH[DIRECTION_LEFT] = nNumH - 1;

	// 四方向氷がないか探索できない状態なら終了
	bool bNothing = true;

	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		// 氷のポインタの保存
		apIce[i] = m_aGrid[aV[i]][aH[i]].pIce;

		if (apIce[i] == nullptr)
			continue;

		if(apIce[i] == pIceStand && nIdx != 1)
		{// 立っている氷に辿り着いたら氷を壊さない
			m_bBreakIce = false;
		}

		if (apIce[i]->IsPeck())
			continue;

		if (apIce[i]->IsCanFind() == false)
			continue;
		
		FindIce(aV[i], aH[i], nIdx, pIceStand);

		bNothing = false;
	}

	if (bNothing)
	{
		return;
	}
}

//=====================================================
// 氷の追加
//=====================================================
void CIceManager::AddIce(CIce *pIce, D3DXVECTOR3 pos)
{

}

//=====================================================
// 氷の破壊
//=====================================================
void CIceManager::BreakIce(void)
{
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			if (m_aGrid[i][j].pIce != nullptr)
			{
				// 壊れる判定と突っついた氷を壊す
				if (m_aGrid[i][j].pIce->IsBreak() || 
					m_aGrid[i][j].pIce->IsPeck())
				{
					m_aGrid[i][j].pIce->Uninit();
					m_aGrid[i][j].pIce = nullptr;
				}
			}
		}
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CIceManager::Debug(void)
{
	// グリッドの場所をデバッグ表示
	for (int i = 0; i < m_nNumGridVirtical; i++)
	{
		for (int j = 0; j < m_nNumGridHorizontal; j++)
		{
			D3DXCOLOR col = { (float)i / m_nNumGridHorizontal,(float)i / m_nNumGridHorizontal,(float)i / m_nNumGridHorizontal,1.0f };

			if (m_aGrid[i][j].state == E_StateGrid::STATE_MID)
				col = { 0.0f,1.0f,0.0f,1.0f };
			else if(m_aGrid[i][j].state == E_StateGrid::STATE_CORNER)
				col = { 0.0f,0.0f,1.0f,1.0f };

			if (m_aGrid[i][j].pIce != nullptr)
			{
				if (m_aGrid[i][j].pIce->IsBreak())
				{
					col = { 0.0f,0.0f,1.0f,1.0f };
				}

				if (m_aGrid[i][j].pIce->IsPeck())
				{
					col = { 0.0f,1.0f,0.0f,1.0f };
				}
			}

			CEffect3D::Create(m_aGrid[i][j].pos, 50.0f, 5, col);
		}
	}

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n氷の総数[%d]", CIce::GetNumAll());
}

//=====================================================
// グリッド位置の取得
//=====================================================
D3DXVECTOR3 CIceManager::GetGridPosition(int nNumV, int nNumH)
{
	if (m_aGrid.empty())
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return m_aGrid[nNumV][nNumH].pos;
}

//=====================================================
// 描画処理
//=====================================================
void CIceManager::Draw(void)
{

}