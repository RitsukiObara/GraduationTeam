//*****************************************
//
// 汎用処理セット
// Author:髙山桃也
//
//*****************************************
#ifndef _UNIVERSAL_H_
#define _UNIVERSAL_H_

//*****************************************
// マクロの定義
//*****************************************
// メモリ開放マクロ
#define SAFE_UNINIT(p)		if ((p) != nullptr) { (p)->Uninit();		(p) = nullptr; }	// Uninit関数の破棄マクロ
#define SAFE_FREE(p)		if ((p) != nullptr) { free((p));			(p) = nullptr; }	// free関数の破棄マクロ
#define SAFE_RELEASE(p)		if ((p) != nullptr) { (p)->Release();		(p) = nullptr; }	// Release関数の破棄マクロ
#define SAFE_DEL_OBJECT(p)	if ((p) != nullptr) { DeleteObject((p));	(p) = nullptr; }	// DeleteObject関数の破棄マクロ
#define SAFE_REF_RELEASE(p)	if ((p) != nullptr) { (p)->Release((p)); }						// 参照ポインタ付きRelease関数の破棄マクロ
#define SAFE_DELETE(p)		if ((p) != nullptr) { delete	(p);		(p) = nullptr; }	// 配列を使用しないdeleteを使用する破棄マクロ
#define SAFE_DEL_ARRAY(p)	if ((p) != nullptr) { delete[]	(p);		(p) = nullptr; }	// 配列を使用したdelete[]を使用する破棄マクロ

//*****************************************
// クラスの定義
//*****************************************
namespace universal
{
//======================================
// 制限系
//======================================
void LimitValuefloat(float *pValue, float fMax, float fMin);
bool LimitValueInt(int *pValue, int nMax, int nMin);
void LimitRot(float *fRot);	//回転制御処理
void LimitSpeed(D3DXVECTOR3 *pVec, float fSpeedMax);
void LimitPosInSq(float fWidth, float fHeight, D3DXVECTOR3 *pPos);
float LimitDistCylinder(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget);
float LimitDistSphereInSide(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget);
float LimitDistCylinderInSide(float fLength, D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget);

//======================================
// 計算系
//======================================
void Horming(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, float fSpeedChase, D3DXVECTOR3 *pMove = nullptr);
D3DXVECTOR3 RelativeInversPos(D3DXVECTOR3 pos, D3DXVECTOR3 posCenter, float fRate = 1.0f);
D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget);
void FactingRot(float *pfRot, float fRotDest, float rotateFact);
void FactingRotTarget(D3DXVECTOR3 *pRot, D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fFact);
void MoveToDest(D3DXVECTOR3 *pPos, D3DXVECTOR3 posTarget, float fFact);
void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });
D3DXVECTOR3 VecToOffset(D3DXMATRIX mtx, D3DXVECTOR3 posOffset);
D3DXVECTOR3 PolarCoordinates(D3DXVECTOR3 rot);
float Vec3Dot(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);
bool CalcRaySphere(D3DXVECTOR3 posStartRay, D3DXVECTOR3 vecRay, D3DXVECTOR3 posSphere, float fRadius, D3DXVECTOR3 *pPosStartHit = nullptr, D3DXVECTOR3 *pPosEndHit = nullptr);
bool CalcRayFlat(D3DXVECTOR3 posPlane, D3DXVECTOR3 nor, D3DXVECTOR3 srcRay, D3DXVECTOR3 endRay, D3DXVECTOR3 *pPosHit = nullptr);
D3DXVECTOR3 CalcScreenToWorld(D3DXVECTOR3 posScreen);
void ConvertScreenPosTo3D(D3DXVECTOR3 *pPosNear, D3DXVECTOR3 *pPosFar, D3DXVECTOR3 *pVecDiff);
D3DXVECTOR3 Vec3Cross(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);
void ParentMtx(D3DXMATRIX *pMtx, D3DXMATRIX mtxParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
float ParabolaY(float x, float a);

//======================================
// 変換系
//======================================
D3DXCOLOR ConvertRGB(BYTE r, BYTE g, BYTE b, BYTE a);
D3DXVECTOR3 VecToRot(D3DXVECTOR3 vec);
D3DXVECTOR3 GetMtxPos(D3DXMATRIX mtx);
int RandRange(int nMax, int nMin);
void VecConvertLength(D3DXVECTOR3 *pVec, float fLength);
float lengthAxis(D3DXVECTOR3 separationAxis, D3DXVECTOR3 e1, D3DXVECTOR3 e2, D3DXVECTOR3 e3);
D3DXVECTOR3 PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset);
D3DXVECTOR3 NorRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offsetMax, D3DXVECTOR3 offsetMin);
void ConvertScreenRate(D3DXVECTOR3 &pos);

//======================================
// 判定系
//======================================
float PlusMin(float fData1, float fData2);
float CrossProduct(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
bool IsInTriangle(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 posTarget);
bool IsOnTrianglePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, float& rHeight);
bool IsOnSquarePolygon(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor1, D3DXVECTOR3 vtxNor2, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight);
bool IsOnSquare(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 vtxNor, D3DXVECTOR3 posTarget, D3DXVECTOR3 posOldTarget, float& rHeight);
bool IsCross(D3DXVECTOR3 posTarget, D3DXVECTOR3 vecSorce, D3DXVECTOR3 vecDest, float *pRate, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });
bool LineCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3* pos, D3DXVECTOR3 posOld);
bool CubeCrossProduct(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3, D3DXVECTOR3 vtx4, D3DXVECTOR3 pos);
bool IsInScreen(const D3DXVECTOR3 pos, D3DXVECTOR3 *pPos = nullptr);
bool DistCmp(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff);
bool DistCmpFlat(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fLengthMax, float *fDiff);
D3DXVECTOR3 CollideOBBToPlane(D3DXVECTOR3* posOBB, D3DXVECTOR3 vecAxial, D3DXVECTOR3 posPlane, D3DXVECTOR3 vecNorPlane);
bool IsInFanTargetYFlat(D3DXVECTOR3 posOwn, D3DXVECTOR3 posTarget, float fRot, float fRange);
template <typename T>
bool FindFromVector(const std::vector<T>& vec, T pValue){ return std::find(vec.begin(), vec.end(), pValue) != vec.end(); }
template <typename T, typename F>
void RemoveIfFromVector(std::vector<T>& vec, F function) { vec.erase(std::remove_if(vec.begin(), vec.end(), function), vec.end()); }

//======================================
// 補完系
//======================================
D3DXVECTOR3 Lerp(D3DXVECTOR3 start, D3DXVECTOR3 end, float fTime);
float EaseOutBack(float fTime);
}

//======================================
// イージング
//======================================
namespace easing
{
inline float EaseInSine(float fTime) { return 1 - cosf((fTime * D3DX_PI) / 2); }
inline float EaseOutSine(float fTime) { return sinf((fTime * D3DX_PI) / 2); }
inline float EaseInOutSine(float fTime) { return -(cosf(D3DX_PI * fTime) - 1) / 2; }

inline float EaseInCubic(float fTime) { return powf(fTime, 3.0f); };
inline float EaseOutCubic(float fTime) { return 1 - powf(1.0f - fTime, 3.0f); };
inline float EaseInOutCubic(float fTime) { return fTime < 0.5 ? 4 * fTime * fTime * fTime : 1 - powf(-2 * fTime + 2, 3) / 2; }

inline float EaseInQuint(float fTime) { return fTime * fTime * fTime * fTime * fTime; }
inline float EaseOutQuint(float fTime) { return 1 - powf(1 - fTime, 5); }
inline float EaseInOutQuint(float fTime) { return fTime < 0.5 ? 16 * fTime * fTime * fTime * fTime * fTime : 1 - powf(-2 * fTime + 2, 5) / 2; }

inline float EaseInQric(float fTime) { return 1 - sqrtf(1 - powf(fTime, 2)); }
inline float EaseOutQric(float fTime) { return sqrtf(1 - powf(fTime - 1, 2)); }
inline float EaseInOutQric(float fTime)
{
	return fTime < 0.5
		? (1 - sqrtf(1 - powf(2 * fTime, 2))) / 2
		: (sqrtf(1 - powf(-2 * fTime + 2, 2)) + 1) / 2;
}

inline float EaseInElastic(float fTime)
{ 
	const float c4 = (2 * D3DX_PI) / 3;

	return fTime == 0
		? 0
		: fTime == 1
		? 1
		: -powf(2, 10 * fTime - 10) * sinf((fTime * 10 - 10.75f) * c4);
}
inline float EaseOutElastic(float fTime)
{
	const float c4 = (2 * D3DX_PI) / 3;

	return fTime == 0
		? 0
		: fTime == 1
		? 1
		: powf(2, -10 * fTime) * sinf((fTime * 10 - 0.75f) * c4) + 1;
}
inline float EaseInOutElastic(float fTime)
{
	const float c5 = (2 * D3DX_PI) / 4.5f;

	return fTime == 0
		? 0
		: fTime == 1
		? 1
		: fTime < 0.5
		? -(powf(2, 20 * fTime - 10) * sinf((20 * fTime - 11.125f) * c5)) / 2
		: (powf(2, -20 * fTime + 10) * sinf((20 * fTime - 11.125f) * c5)) / 2 + 1;
}

inline float EaseInQuad(float fTime) { return fTime * fTime; }
inline float EaseOutQuad(float fTime) { return 1 - (1 - fTime) * (1 - fTime); }
inline float EaseInOutQuad(float fTime) { return fTime < 0.5 ? 2 * fTime * fTime : 1 - powf(-2 * fTime + 2, 2) / 2; }

inline float EaseInQuart(float fTime) { return fTime * fTime * fTime * fTime; }
inline float EaseOutQuart(float fTime) { return 1 - powf(1 - fTime, 4); }
inline float EaseInOutQuart(float fTime) { return fTime < 0.5 ? 8 * fTime * fTime * fTime * fTime : 1 - powf(-2 * fTime + 2, 4) / 2; }

inline float EaseInExpo(float fTime) { return fTime == 0 ? 0 : powf(2, 10 * fTime - 10); }
inline float EaseOutExpo(float fTime) { return fTime == 1 ? 1 : 1 - powf(2, -10 * fTime); }
inline float EaseInOutExpo(float fTime) 
{ 
	return fTime == 0
		? 0
		: fTime == 1
		? 1
		: fTime < 0.5 ? powf(2, 20 * fTime - 10) / 2
		: (2 - powf(2, -20 * fTime + 10)) / 2;
}

inline float EaseInBack(float fTime)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return c3 * fTime * fTime * fTime - c1 * fTime * fTime;
}
inline float EaseOutBack(float fTime)
{ 
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return 1 + c3 * powf(fTime - 1, 3) + c1 * powf(fTime - 1, 2);
}
inline float EaseInOutBack(float fTime)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return fTime < 0.5f
		? (powf(2 * fTime, 2) * ((c2 + 1) * 2 * fTime - c2)) / 2
		: (powf(2 * fTime - 2, 2) * ((c2 + 1) * (fTime * 2 - 2) + c2) + 2) / 2;
}

inline float EaseOutBounce(float fTime)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (fTime < 1 / d1) 
	{
		return n1 * fTime * fTime;
	}
	else if (fTime < 2 / d1) 
	{
		return n1 * (fTime -= 1.5f / d1) * fTime + 0.75f;
	}
	else if (fTime < 2.5 / d1) 
	{
		return n1 * (fTime -= 2.25f / d1) * fTime + 0.9375f;
	}
	else 
	{
		return n1 * (fTime -= 2.625f / d1) * fTime + 0.984375f;
	}
}
inline float EaseInBounce(float fTime) { return 1 - EaseOutBounce(1 - fTime); }
inline float EaseInOutBounce(float fTime)
{
	return fTime < 0.5f
		? (1 - EaseOutBounce(1 - 2 * fTime)) / 2
		: (1 + EaseOutBounce(2 * fTime - 1)) / 2;
}
}

#endif