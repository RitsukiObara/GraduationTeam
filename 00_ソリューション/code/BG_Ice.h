//*****************************************************
//
// 背景氷の処理[BG_Ice.h]
// Author:早川友貴
//
//*****************************************************
#ifndef _BG_ICE_H_
#define _BG_ICE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"
#include "iceManager.h"

//*****************************************************
// クラス定義
//*****************************************************
class CBgIce : public CObjectX
{
public:
	CBgIce();	// コンストラクタ
	~CBgIce();	// デストラクタ

	static CBgIce* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void Load(char* pPath);

	void Move(void);
	void Flow(void);

private:

	//背景氷の状態
	typedef enum
	{
		STATE_FLOW = 0,
		STATE_SINK,
		STATE_MAX
	}STATE;

	STATE m_state;			// 状態分け変数
	float fgravity_speed;	// 沈んでいく速度;
	float m_fspeed;			// 沈み始める速度
	bool m_binscrean;		// 画面内に入ったかチェックする変数
};

//*****************************************************
// 定数定義
//*****************************************************
namespace bgice
{
	const int BGICE_MODEL = 3;	// 背景の氷総数
}

#endif
