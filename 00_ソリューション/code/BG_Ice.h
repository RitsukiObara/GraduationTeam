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

//*****************************************************
// クラス定義
//*****************************************************
class CBgIce : public CObjectX
{
public:
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

private:
	TYPE m_type;	//モデルのタイプ分け変数

};

#endif
