//*****************************************************
//
// メッシュフィ－ルドの処理[meshfield.h]
// Author:髙山桃也
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
	CMeshField(int nPriority = 3);	// コンストラクタ
	~CMeshField();	// デストラクタ

	typedef struct
	{
		int nNumIdx;							//インデックス数
		int nNumVtx;							//頂点数
	}MeshField;

	// 列挙型定義
	enum E_Ocean_Speed
	{// 海流の状態
		OCEAN_STATE_NONE = 0,	// 何でもない状態
		OCEAN_STATE_UP,	// 速度が上がってる状態
		OCEAN_STATE_DOWN,	// 速度が下がってる状態
		OCEAN_STATE_MAX
	};

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
	void ResetNormal(void);
	void Dent(D3DXVECTOR3 pos, float fRadius, float fDepth);
	bool RayCheck(D3DXVECTOR3 pos, D3DXVECTOR3 vecDir, D3DXVECTOR3 posOwn);
	void SetCol(D3DXCOLOR col);
	void SetIdxTexture(int nIdxTexture) { m_nIdxTexture = nIdxTexture; }
	void SetDivTex(int nDivTex);
	void Reset(void);
	void Wave(float fRot);

	// 変数取得・設定関数
	E_Ocean_Speed GetOceanSpeedState(void) { return m_eOcean_Speed_State; }	// 海流の速度状態の取得
	void SetOceanSpeedState(E_Ocean_Speed fOceanSpeedState) { m_eOcean_Speed_State = fOceanSpeedState; }	// 海流の速度状態の設定

private:
	int m_nIdxTexture;	// テクスチャ番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ
	MeshField m_MeshField;	//構造体の情報
	D3DXCOLOR m_col;	// 色
	E_Ocean_Speed m_eOcean_Speed_State;	// 海流の速度状態
	float m_fLengthMesh;	// メッシュ一辺の長さ
	int m_nDivNumU;
	int m_nDivNumV;
	int m_nDivTex;
	float m_fOceanSpeed;	// 海流の速度
	static CMeshField *m_pMeshField;	// 自身のポインタ
};
#endif