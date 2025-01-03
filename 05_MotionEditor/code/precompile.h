//========================================
//
// プリコンパイル[precompile.h]
// Author：��山桃也
//
//========================================
#ifndef PRECOMPILE_H__
#define PRECOMPILE_H__

//========================================
// 事前インクルード
//========================================
#include <windows.h>
#include"d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)						//ビルド時の警告対処用マクロ
#include "dinput.h"
#include "Xinput.h"
#include "xaudio2.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <time.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <locale>
#include <codecvt>

//========================================
// Using宣言
//========================================
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::vector;
using std::list;
using std::map;

#endif // _PRECOMPILE_H_
