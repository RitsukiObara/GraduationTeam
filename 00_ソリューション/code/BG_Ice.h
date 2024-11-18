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

	//背景氷の種類
	typedef enum
	{
		TYPE_BIG = 0,
		TYPE_SMALL,
		TYPE_MAX
	}TYPE;

	CBgIce();	// コンストラクタ
	~CBgIce();	// デストラクタ

	static CBgIce* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void Load(char* pPath);

	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }
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

	TYPE m_type;	// タイプ分け変数
	STATE m_state;	// 状態分け変数
	COcean::E_Stream m_streamOld;	// 海流情報
	float fgravity_speed;	//	沈んでいく速度;
	int m_nNumAll;	// 総数
};

#endif
