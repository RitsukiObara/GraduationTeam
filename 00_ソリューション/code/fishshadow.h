//*****************************************************
//
// 魚影の処理[fishshadow.h]
// Author:若木一真
//
//*****************************************************
#ifndef _FISHSHADOW_H_
#define _FISHSHADOW_H_

//*****************************************************
// クラス定義
//*****************************************************
class CFishShadow : public CObject3D
{
public:
	CFishShadow();	// コンストラクタ
	~CFishShadow();	// デストラクタ

	static CFishShadow* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CFishShadow* GetInstance(void) { return m_pFishShadow; }

private:

	// 魚影の種類
	enum FISHSHADOW
	{
		FISHSHADOW_SEALS = 0,	// アザラシ
		FISHSHADOW_BEARS,	// しろくま
		FISHSHADOW_MAX
	};

	void ResultState(void); // 魚影状態管理
	void FishShadowSeals(void); // アザラシの魚影処理
	void FishShadowBears(void); // しろくまの魚影処理

	FISHSHADOW m_FishShadow;	// 選択項目
	CObject3D* m_apFishShadow[FISHSHADOW_MAX];	// 魚影の3Dオブジェクト
	D3DXVECTOR3 m_aPosDest[FISHSHADOW_MAX];	// 目標の位置
	static CFishShadow* m_pFishShadow;	// 自身のポインタ
};

#endif