//*****************************************************
//
// パーツの処理[parts.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PARTS_H_
#define _PARTS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "model.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_TEX	(5)	// テクスチャの数

//*****************************************************
// クラス定義
//*****************************************************
class CParts
{
public:
	CParts();	// コンストラクタ
	~CParts();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CParts *Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	float GetHeight(void) { return 0.0f; }	// サイズ取得
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// 設定処理
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	D3DXVECTOR3 GetPosOrg(void) { return m_posOrg; }	// 取得処理
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }// 向き設定処理
	void SetPosOrg(D3DXVECTOR3 pos) { m_posOrg = pos; }// 向き設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }	// 向き取得処理
	void BindModel(int nIdx);
	int GetIdxModel(void) { return m_IdxModel; }
	void SetIdxModel(int nIdx) { m_IdxModel = nIdx; }
	void SetRadius(void);
	float GetRadius(void) { return m_fRadius; }
	D3DXMATRIX *GetMatrix(void) { return &m_mtxWorld; }
	void SetMatrix(void);
	void SetCurrent(bool bCurrent) { m_bCurrent = bCurrent; }

private:
	D3DXVECTOR3 m_pos;								//位置
	D3DXVECTOR3 m_posOrg;								//最初の位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;								//向き
	D3DXMATRIX m_mtxWorld;							//ワールドマトリックス
	CModel::Model *m_pModel;
	int m_IdxModel;	// モデルの番号
	float m_fRadius;	// モデルの半径
	float m_fAlpha;	// パーツの透明度
	bool m_bCurrent;
};

#endif