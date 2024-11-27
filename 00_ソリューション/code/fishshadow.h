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

	void ResultState(void); // リザルトUIの状態
	void ResultClear(void); // リザルトがクリアの時
	void ResultFail(void); // リザルトが失敗の時

	FISHSHADOW m_FishShadow;	// 選択項目
	D3DXVECTOR3 m_aPosDest[FISHSHADOW_MAX];	// 目標の位置
	static CFishShadow* m_pFishShadow;	// 自身のポインタ
	bool m_bSound;
	int nCountMove;
};

#endif