//*****************************************************
//
// Xファイルの処理[objectX.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"
#include "model.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_TEX	(5)	// テクスチャの数

//*****************************************************
// クラス定義
//*****************************************************
class CObjectX : public CObject
{
public:
	CObjectX();	// コンストラクタ
	~CObjectX();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObjectX *Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	float GetHeight(void) { return 0.0f; }	// サイズ取得
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// 設定処理
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }// 向き設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }	// 向き取得処理
	void BindModel(int nIdx);
	int GetIdxModel(void) { return m_IdxModel; }
	void SetIdxModel(int nIdx) { m_IdxModel = nIdx; }
	void SetRadius(void);
	float GetRadius(void) { return m_fRadius; }
	D3DXMATRIX *GetMatrix(void) { return &m_mtxWorld; }
	void SetMatrix(void);
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }	// 取得処理
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }	// 取得処理

private:
	D3DXVECTOR3 m_pos;								//位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;								//向き
	D3DXMATRIX m_mtxWorld;							//ワールドマトリックス
	CModel::Model *m_pModel;
	int m_IdxModel;	// モデルの番号
	float m_fRadius;	// モデルの半径
	D3DXVECTOR3 m_vtxMax;	// 最大頂点
	D3DXVECTOR3 m_vtxMin;	// 最小頂点
};

#endif