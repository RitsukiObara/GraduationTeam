//*****************************************************
//
// カメラの処理[camera.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CAMERA_H_
#define _CAMERA_H_

// マクロ定義
#define MIN_DRAW		(10.0f)					// 描画を開始する距離
#define MAX_DRAW		(15000.0f)				// 描画を終了する距離

//*****************************************************
// クラスの定義
//****************************************************
class CCamera
{
public:
	typedef struct
	{
		D3DXVECTOR3 posV;							//視点
		D3DXVECTOR3 posVOld;						//視点の前回の位置
		D3DXVECTOR3 posR;							//注視点
		D3DXVECTOR3 posVDest;						//目的の視点
		D3DXVECTOR3 posRDest;						//目的の注視点
		D3DXVECTOR3 vecU;							//上方向ベクトル
		D3DXVECTOR3 rot;							//向き
		float fLength;								//視点から注視点までの距離
		D3DXMATRIX mtxProjection;					//プロジェクションマトリックス
		D3DXMATRIX mtxView;							//ビューマトリックス
	}Camera;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetPosV(void);
	void SetPosR(void);
	void Control(void);
	Camera *GetCamera(void);

private:
	Camera m_camera;												//構造体の情報
};

#endif
