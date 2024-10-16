//*****************************************************
//
// メッシュフィ−ルドの処理[meshfield.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"

//*****************************************************
// クラス定義
//*****************************************************
class CMeshField : public CObject3D
{
public:
	CMeshField();	// コンストラクタ
	~CMeshField();	// デストラクタ

	typedef struct
	{
		int nNumIdx;							//インデックス数
		int nNumVtx;							//頂点数
	}MeshField;

	static CMeshField *Create(void);
	static CMeshField *GetInstance(void) { return m_pMeshField; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor = nullptr);
	void Edit(void);
	void Load(std::string path);
	void Save(std::string path);
	void SetNormal(void);
	void Dent(D3DXVECTOR3 pos, float fRadius, float fDepth);
	bool RayCheck(D3DXVECTOR3 pos, D3DXVECTOR3 vecDir, D3DXVECTOR3 posOwn);
	void SetCol(D3DXCOLOR col);
	void SetIdxTexture(int nIdxTexture) { m_nIdxTexture = nIdxTexture; }
	void Reset(void);
	void Wave(float fRot);

private:
	int m_nIdxTexture;	// テクスチャ番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ
	MeshField m_MeshField;	//構造体の情報
	D3DXCOLOR m_col;	// 色
	float m_fLengthMesh;	// メッシュ一辺の長さ
	int m_nDivNumU;
	int m_nDivNumV;
	int m_nDivTex;
	static CMeshField *m_pMeshField;	// 自身のポインタ
};
#endif