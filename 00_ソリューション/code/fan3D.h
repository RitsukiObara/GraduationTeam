//*****************************************************
//
// 3D円の処理[fan3D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FAN3D_H_
#define _FAN3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "fan.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CFan3D : public CFan
{
public:
	CFan3D(int nPriority = 5);
	~CFan3D();	//	デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理
	void DrawNormal(void);			// 通常描画
	void DrawBillboard(void);		// ビルボード描画
	void SetVtx(void) override;		// 頂点設定
	void SetTex(float fScrollX, float fScrollY);      // テクスチャ座標設定処理
	void SetCol(D3DXCOLOR col) override;
	void SetIdxTextureOverRay(int nIdx) { m_nIdxTextureOverRay = nIdx; }	// オーバーレイテクスチャ番号設定
	void ResetMtxParent(void);		// 親マトリックスのリセット

	// 変数取得・設定関数
	D3DXMATRIX GetMatrixParent(void) { return m_mtxParent; }	// 親マトリックス
	void SetMatrixParent(D3DXMATRIX mtx) { m_mtxParent = mtx; }

	// 静的メンバ関数
	static CFan3D *Create(int nPriority = 3, int nNumVtx = 16);	// 生成処理

private:
	// メンバ変数
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX m_mtxParent;	// 親マトリックス
	bool m_bBillboard;				// ビルボードかどうか
	int m_nStartOffsetCount;		// 使用する波形の開始位置をずらすカウント
	int m_nIdxTextureOverRay;		// オーバーレイテクスチャの設定
};

#endif