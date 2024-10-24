//*****************************************************
//
// ３Dアニメーション処理[anim3D.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************
// インクルード
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CAnim3D : public CPolygon3D
{
public:
	CAnim3D(int nPriority = 6);	// コンストラクタ
	~CAnim3D();	// デストラクタ

	static CAnim3D *Create(D3DXVECTOR3 pos, int nNumAnim, int nTimeAnim, bool bLoop = false);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RandStart(void);	// アニメーションのスタートをランダムにする処理

private:
	void SetAnim(int nAnim, int nNumAnim);

	int m_nCounterAnim;	// アニメーションカウンター
	int m_nPatternAnim;	// アニメパターンNO.
	int m_nSpeedAnim;	// アニメーション速度
	int m_nNumAnim;	// アニメーション枚数
	bool m_bLoop;	// ループするかどうか
	bool m_bFinish;	// 終了したかどうか
};

#endif