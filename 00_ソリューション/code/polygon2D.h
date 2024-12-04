//*****************************************************
//
// 2Dポリゴンの処理[polygon2D.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _POLYGON2D_H_
#define _POLYGON2D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object2D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CPolygon2D	: public CObject2D
{
public:
	CPolygon2D(int nPriority = 5);	// コンストラク
	~CPolygon2D();	//	デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
	void AddMove(D3DXVECTOR3 move);	// 移動量加算処理
	void DicMove(float fDicrease);	// 移動量減衰処理

	// 変数取得・設定関数
	virtual void SetSize(float width, float height);	// サイズ
	virtual D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_width, m_heigth); }
	virtual void SetWidth(float width);
	virtual void SetHeight(float height);
	virtual float GetWidth(void) { return m_width; }	// 幅
	virtual float GetHeight(void) { return m_heigth; }	// 高さ
	void SetAnim(int nAnim, int nNumAnim, int nNumV);	// アニメーション
	void SetTex(D3DXVECTOR2 texLeftUp, D3DXVECTOR2 texRightDown);	// テクスチャ座標
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }	// 頂点バッファ
	void SetMove(D3DXVECTOR3 move);	// 移動量設定処理
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }	// 前回の位置
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 移動量取得処理
	D3DXCOLOR GetCol(void) { return m_col; }	// 色
	float GetAlpha(void) { return m_col.a; }	// 透明度
	void SetCol(D3DXCOLOR col);
	void SetAlpha(float fAlpha);
	void SetVtx(void);	// 頂点情報設定
	void SetCenterHeight(float fHeight);	// 中心の高さ
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }	// テクスチャ番号
	int GetIdxTexture(void) { return m_nIdxTexture; }

	// 静的メンバ関数
	static CPolygon2D *Create(int nPriority = 3, float fCenterHeight = 0.5f);	// 生成処理

private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXCOLOR m_col;	// 色
	float m_width;	// 幅
	float m_heigth;	// 高さ
	float m_fCenterHeight;	// 中心の高さ
	int m_nIdxTexture;	// テクスチャの番号
};

#endif