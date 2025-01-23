//*****************************************************
//
// フェード処理（氷が落ちてくる）[fade_fallice.h]
// Author:石原颯馬
//
//*****************************************************

#ifndef _FADE_FALLICE_H_
#define _FADE_FALLICE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "fade.h"
#include <vector>

class CPolygon2D;

//*****************************************************
// クラスの定義
//*****************************************************
class CFade_FallIce : public CFade
{
public:
	CFade_FallIce();	// コンストラクタ
	~CFade_FallIce();	// デストラクタ

	static CFade_FallIce* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetFade(CScene::MODE modeNext, bool bTrans = true) override;
	static CFade_FallIce* GetInstance(void) { return m_pFade; }
	void ChangeOBRTexture(void);
private:
	int m_nCounterFrame;								// 次の落下カウンタ
	int m_nCounterPattern;								// 次落とすパターン
	std::vector<LPDIRECT3DVERTEXBUFFER9> m_icePolygon;	// 氷のポリゴン
	LPDIRECT3DTEXTURE9 m_pTexture;						// 氷のテクスチャ
	static CFade_FallIce* m_pFade;						// 自身のポインタ
};

#endif
