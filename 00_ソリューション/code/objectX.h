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
#include "object3D.h"
#include "model.h"

//*****************************************************
// クラス定義
//*****************************************************
class CObjectX : public CObject3D
{
public:
	CObjectX(int nPriority = 5);	// コンストラクタ
	~CObjectX();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void JustDraw(void);
	void DrawShadow(void);
	static CObjectX *Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f }, int nPriority = 3);
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }	// 設定処理
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// 設定処理
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void BindModel(int nIdx);
	int GetIdxModel(void) { return m_nIdxModel; }
	void SetIdxModel(int nIdx) { m_nIdxModel = nIdx; }
	void SetRadius(void);
	float GetRadius(void) { return m_fRadius; }
	void CalcMatrix(void);
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }	// 取得処理
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }	// 取得処理
	void SetVtxMax(D3DXVECTOR3 vtx) { m_vtxMax = vtx; }
	void SetVtxMin(D3DXVECTOR3 vtx) { m_vtxMin = vtx; }
	void EnableShadow(bool bEnable) { m_bShadow = bEnable; }
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }
	D3DXMATERIAL GetMaterial(int nIdx);					// マテリアル
	void SetMaterial(D3DXMATERIAL mat, int nIdx);
	D3DXCOLOR GetDeffuseeCol(int nIdx = 0);
	void SetDeffuseCol(D3DXCOLOR col, int nIdx = 0);
	void ResetMat(void);

private:
	D3DXVECTOR3 m_posOld;			// 前回の位置
	D3DXVECTOR3 m_move;				// 移動量
	D3DXCOLOR m_col;				// 色
	CModel::Model *m_pModel;		// モデル情報
	int m_nIdxModel;				// モデルの番号
	float m_fRadius;				// モデルの半径
	D3DXVECTOR3 m_vtxMax;			// 最大頂点
	D3DXVECTOR3 m_vtxMin;			// 最小頂点
	vector<D3DXMATERIAL> m_aMat;	// 変化用のマテリアル
	bool m_bChangeMat;				// 色変更フラグ
	bool m_bShadow;					// 影描画フラグ
	bool m_bDisp;					// 描画フラグ
};

#endif