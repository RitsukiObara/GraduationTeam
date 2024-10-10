xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 347;
 -13.27805;-93.57098;16.79222;,
 18.11727;-92.97508;16.36784;,
 18.08051;-86.24619;23.09568;,
 -13.31481;-86.84208;23.52006;,
 -20.13235;-86.97144;16.88314;,
 -20.58040;-86.97994;-16.27694;,
 -13.72615;-93.57947;-16.36784;,
 -13.27805;-93.57098;16.79222;,
 -14.73449;-12.04019;23.52006;,
 -21.55201;-12.16959;16.88314;,
 -20.13235;-86.97144;16.88314;,
 -13.94473;-86.85399;-23.09568;,
 17.45060;-86.25812;-23.52006;,
 17.66917;-92.98364;-16.79224;,
 -13.72615;-93.57947;-16.36784;,
 -20.58040;-86.97994;-16.27694;,
 -22.00009;-12.17805;-16.27694;,
 -15.35877;-12.33075;-23.07006;,
 16.66083;-11.44429;23.09568;,
 16.44223;-4.71879;16.36784;,
 -14.95307;-5.31469;16.79222;,
 -14.95307;-5.31469;16.79222;,
 -15.40117;-5.32319;-16.36784;,
 -22.00009;-12.17805;-16.27694;,
 -21.55201;-12.16959;16.88314;,
 -15.40117;-5.32319;-16.36784;,
 15.99415;-4.72735;-16.79224;,
 16.03655;-11.73493;-23.49448;,
 -14.95307;-5.31469;16.79222;,
 16.44223;-4.71879;16.36784;,
 15.99415;-4.72735;-16.79224;,
 -15.40117;-5.32319;-16.36784;,
 18.11727;-92.97508;16.36784;,
 -13.27805;-93.57098;16.79222;,
 -13.72615;-93.57947;-16.36784;,
 17.66917;-92.98364;-16.79224;,
 52.17998;-14.29297;6.32410;,
 51.99380;-14.43547;-7.65176;,
 48.14677;-4.74523;-11.77842;,
 48.46105;-4.72319;10.50172;,
 48.46125;-4.72319;10.50172;,
 48.79684;-74.17159;10.51498;,
 52.20542;-68.53625;6.33764;,
 52.18014;-14.29299;6.32410;,
 48.49555;-74.17731;-11.78014;,
 52.01573;-68.53985;-7.69938;,
 52.01592;-68.53985;-7.69938;,
 48.49576;-74.17731;-11.78014;,
 48.79684;-74.17159;10.51498;,
 48.46125;-4.72319;10.50172;,
 48.46125;-4.72319;10.50172;,
 48.14695;-4.74519;-11.77842;,
 48.14695;-4.74519;-11.77842;,
 48.49555;-74.17731;-11.78014;,
 18.26736;-80.25474;14.87860;,
 17.85938;-80.26249;-15.31408;,
 45.92723;-63.13907;-10.75058;,
 46.20151;-63.13388;9.54950;,
 51.99393;-14.43547;-7.65176;,
 18.11727;-92.97508;16.36784;,
 18.26736;-80.25474;14.87860;,
 46.20151;-63.13388;9.54950;,
 48.79684;-74.17159;10.51498;,
 45.92723;-63.13907;-10.75058;,
 48.49555;-74.17731;-11.78014;,
 17.85938;-80.26249;-15.31408;,
 17.66917;-92.98364;-16.79224;,
 52.18014;-14.29299;6.32410;,
 52.17998;-14.29297;6.32410;,
 48.46105;-4.72319;10.50172;,
 48.46125;-4.72319;10.50172;,
 48.14677;-4.74523;-11.77842;,
 48.14695;-4.74519;-11.77842;,
 51.99393;-14.43547;-7.65176;,
 51.99380;-14.43547;-7.65176;,
 48.49576;-74.17731;-11.78014;,
 48.49555;-74.17731;-11.78014;,
 52.01592;-68.53985;-7.69938;,
 52.01573;-68.53985;-7.69938;,
 37.94057;-106.60852;2.86174;,
 37.18350;-66.71749;2.86174;,
 32.63712;-66.80373;4.74524;,
 33.39421;-106.69480;4.74524;,
 28.09081;-66.89002;2.86174;,
 28.84792;-106.78103;2.86174;,
 28.84792;-106.78103;2.86174;,
 28.09081;-66.89002;2.86174;,
 26.20763;-66.92576;-1.68540;,
 26.96473;-106.81677;-1.68540;,
 28.09081;-66.89002;-6.23256;,
 28.84792;-106.78103;-6.23256;,
 28.84792;-106.78103;-6.23256;,
 28.09081;-66.89002;-6.23256;,
 32.63712;-66.80373;-8.11606;,
 33.39421;-106.69480;-8.11606;,
 37.18350;-66.71749;-6.23256;,
 37.94057;-106.60852;-6.23256;,
 37.94057;-106.60852;-6.23256;,
 37.18350;-66.71749;-6.23256;,
 39.06661;-66.68166;-1.68540;,
 39.82372;-106.57276;-1.68540;,
 37.18350;-66.71749;2.86174;,
 37.94057;-106.60852;2.86174;,
 33.39421;-106.69480;4.74524;,
 33.39421;-106.69480;-1.68540;,
 37.94057;-106.60852;2.86174;,
 28.84792;-106.78103;2.86174;,
 26.96473;-106.81677;-1.68540;,
 28.84792;-106.78103;-6.23256;,
 33.39421;-106.69480;-8.11606;,
 37.94057;-106.60852;-6.23256;,
 39.82372;-106.57276;-1.68540;,
 37.18350;-66.71749;2.86174;,
 32.63712;-66.80373;-1.68540;,
 32.63712;-66.80373;4.74524;,
 28.09081;-66.89002;2.86174;,
 26.20763;-66.92576;-1.68540;,
 28.09081;-66.89002;-6.23256;,
 32.63712;-66.80373;-8.11606;,
 37.18350;-66.71749;-6.23256;,
 39.06661;-66.68166;-1.68540;,
 -8.29071;-124.90332;-6.11322;,
 2.27841;-134.36887;-6.11322;,
 -0.63665;-137.62381;-6.11322;,
 -11.20581;-128.15830;-6.11322;,
 2.27841;-134.36887;-6.11322;,
 2.27841;-134.36887;11.71292;,
 -0.63665;-137.62381;11.71292;,
 -0.63665;-137.62381;-6.11322;,
 2.27841;-134.36887;11.71292;,
 -8.29071;-124.90332;11.71292;,
 -11.20581;-128.15830;11.71292;,
 -0.63665;-137.62381;11.71292;,
 -8.29071;-124.90332;11.71292;,
 -8.29071;-124.90332;-6.11322;,
 -11.20581;-128.15830;-6.11322;,
 -11.20581;-128.15830;11.71292;,
 -0.63665;-137.62381;-6.11322;,
 -0.63665;-137.62381;11.71292;,
 -8.29071;-124.90332;11.71292;,
 -8.29071;-124.90332;-6.11322;,
 -3.36819;-130.00911;-17.07162;,
 -14.12287;-109.74778;-17.52048;,
 -9.71193;-107.40718;-17.55326;,
 1.04277;-127.66848;-17.10442;,
 -14.12287;-109.74778;-17.52048;,
 -14.15393;-109.56035;-8.31450;,
 -9.74299;-107.21973;-8.34726;,
 -9.71193;-107.40718;-17.55326;,
 -14.15393;-109.56035;-8.31450;,
 -3.39929;-129.82163;-7.86566;,
 1.01171;-127.48106;-7.89840;,
 -9.74299;-107.21973;-8.34726;,
 -3.39929;-129.82163;-7.86566;,
 -3.36819;-130.00911;-17.07162;,
 1.04277;-127.66848;-17.10442;,
 1.01171;-127.48106;-7.89840;,
 -9.71193;-107.40718;-17.55326;,
 -9.74299;-107.21973;-8.34726;,
 -3.39929;-129.82163;-7.86566;,
 -3.36819;-130.00911;-17.07162;,
 -12.70575;-109.08726;-17.07162;,
 5.69877;-95.39566;-17.52048;,
 8.67871;-99.40247;-17.55326;,
 -9.72575;-113.09404;-17.10442;,
 5.69877;-95.39566;-17.52048;,
 5.87933;-95.33666;-8.31450;,
 8.85937;-99.34346;-8.34726;,
 8.67871;-99.40247;-17.55326;,
 5.87933;-95.33666;-8.31450;,
 -12.52517;-109.02827;-7.86566;,
 -9.54515;-113.03504;-7.89840;,
 8.85937;-99.34346;-8.34726;,
 -12.52517;-109.02827;-7.86566;,
 -12.70575;-109.08726;-17.07162;,
 -9.72575;-113.09404;-17.10442;,
 -9.54515;-113.03504;-7.89840;,
 -9.72575;-113.09404;-17.10442;,
 -9.54515;-113.03504;-7.89840;,
 5.87933;-95.33666;-8.31450;,
 5.69877;-95.39566;-17.52048;,
 5.37397;-129.43484;-18.30438;,
 4.68789;-93.28568;-18.30438;,
 15.26809;-93.08488;-18.30438;,
 15.95417;-129.23406;-18.30438;,
 4.68789;-93.28568;-18.30438;,
 4.68789;-93.28568;13.65508;,
 15.26809;-93.08488;13.65508;,
 15.26809;-93.08488;-18.30438;,
 4.68789;-93.28568;13.65508;,
 5.37397;-129.43484;13.65508;,
 15.95417;-129.23406;13.65508;,
 15.26809;-93.08488;13.65508;,
 5.37397;-129.43484;13.65508;,
 5.37397;-129.43484;-18.30438;,
 15.95417;-129.23406;-18.30438;,
 15.95417;-129.23406;13.65508;,
 15.95417;-129.23406;-18.30438;,
 15.26809;-93.08488;-18.30438;,
 15.26809;-93.08488;13.65508;,
 15.95417;-129.23406;13.65508;,
 5.37397;-129.43484;13.65508;,
 4.68789;-93.28568;13.65508;,
 4.68789;-93.28568;-18.30438;,
 5.37397;-129.43484;-18.30438;,
 -1.15671;-133.76196;-6.11322;,
 9.59003;-124.49856;-6.11322;,
 12.44287;-127.80818;-6.11322;,
 1.69611;-137.07158;-6.11322;,
 9.59003;-124.49856;-6.11322;,
 9.59003;-124.49856;11.71292;,
 12.44287;-127.80818;11.71292;,
 12.44287;-127.80818;-6.11322;,
 9.59003;-124.49856;11.71292;,
 -1.15671;-133.76196;11.71292;,
 1.69611;-137.07158;11.71292;,
 12.44287;-127.80818;11.71292;,
 -1.15671;-133.76196;11.71292;,
 -1.15671;-133.76196;-6.11322;,
 1.69611;-137.07158;-6.11322;,
 1.69611;-137.07158;11.71292;,
 1.69611;-137.07158;-6.11322;,
 1.69611;-137.07158;11.71292;,
 9.59003;-124.49856;11.71292;,
 9.59003;-124.49856;-6.11322;,
 2.14039;-110.13687;-6.11322;,
 -10.15167;-127.45745;-6.11322;,
 -13.61073;-124.78779;-6.11322;,
 -1.31865;-107.46714;-6.11322;,
 -10.15167;-127.45745;-6.11322;,
 -10.15167;-127.45745;11.71292;,
 -13.61073;-124.78779;11.71292;,
 -13.61073;-124.78779;-6.11322;,
 -10.15167;-127.45745;11.71292;,
 2.14039;-110.13687;11.71292;,
 -1.31865;-107.46714;11.71292;,
 -13.61073;-124.78779;11.71292;,
 2.14039;-110.13687;11.71292;,
 2.14039;-110.13687;-6.11322;,
 -1.31865;-107.46714;-6.11322;,
 -1.31865;-107.46714;11.71292;,
 -1.31865;-107.46714;-6.11322;,
 -1.31865;-107.46714;11.71292;,
 -10.15167;-127.45745;11.71292;,
 -10.15167;-127.45745;-6.11322;,
 5.69005;-14.91587;4.13510;,
 9.46303;-11.21255;7.97770;,
 1.08927;-11.37147;11.44686;,
 1.15817;-15.00189;6.01258;,
 -7.28453;-11.53039;7.97770;,
 -3.37369;-15.08787;4.13510;,
 -10.75307;-11.59623;-0.39758;,
 -5.25081;-15.12349;-0.39758;,
 -7.28453;-11.53039;-8.77288;,
 -3.37369;-15.08787;-4.93026;,
 1.08927;-11.37147;-12.24206;,
 1.15817;-15.00189;-6.80776;,
 9.46303;-11.21255;-8.77288;,
 5.69005;-14.91587;-4.93026;,
 5.69005;-14.91587;-4.93026;,
 9.46303;-11.21255;-8.77288;,
 12.93159;-11.14675;-0.39758;,
 7.56717;-14.88027;-0.39758;,
 9.46303;-11.21255;7.97770;,
 5.69005;-14.91587;4.13510;,
 9.46303;-11.21255;7.97770;,
 11.92699;-5.73053;10.54526;,
 0.98615;-5.93817;15.07794;,
 1.08927;-11.37147;11.44686;,
 0.98615;-5.93817;15.07794;,
 -9.95473;-6.14575;10.54526;,
 -7.28453;-11.53039;7.97770;,
 -9.95473;-6.14575;10.54526;,
 -14.48659;-6.23181;-0.39758;,
 -10.75307;-11.59623;-0.39758;,
 -9.95473;-6.14575;-11.34042;,
 -7.28453;-11.53039;-8.77288;,
 -9.95473;-6.14575;-11.34042;,
 0.98615;-5.93817;-15.87310;,
 1.08927;-11.37147;-12.24206;,
 0.98615;-5.93817;-15.87310;,
 11.92699;-5.73053;-11.34042;,
 9.46303;-11.21255;-8.77288;,
 11.92699;-5.73053;-11.34042;,
 16.45887;-5.64447;-0.39758;,
 11.92699;-5.73053;10.54526;,
 12.70687;0.69559;11.44686;,
 0.86453;0.47083;16.35298;,
 -10.97781;0.24607;11.44686;,
 -9.95473;-6.14575;10.54526;,
 -10.97781;0.24607;11.44686;,
 -15.88303;0.15295;-0.39758;,
 -10.97781;0.24607;-12.24206;,
 -9.95473;-6.14575;-11.34042;,
 -10.97781;0.24607;-12.24206;,
 0.86453;0.47083;-17.14818;,
 12.70687;0.69559;-12.24206;,
 12.70687;0.69559;-12.24206;,
 17.61209;0.78867;-0.39758;,
 12.70687;0.69559;11.44686;,
 11.68371;7.08750;10.54526;,
 0.74285;6.87987;15.07794;,
 -10.19799;6.67223;10.54526;,
 -10.19799;6.67223;10.54526;,
 -14.72987;6.58617;-0.39758;,
 -10.19799;6.67223;-11.34042;,
 -10.19799;6.67223;-11.34042;,
 0.74285;6.87987;-15.87310;,
 11.68371;7.08750;-11.34042;,
 11.68371;7.08750;-11.34042;,
 16.21561;7.17353;-0.39758;,
 11.68371;7.08750;10.54526;,
 11.68371;7.08750;10.54526;,
 9.01351;12.47209;7.97770;,
 0.63975;12.31317;11.44686;,
 0.74285;6.87987;15.07794;,
 0.63975;12.31317;11.44686;,
 -7.73403;12.15425;7.97770;,
 -7.73403;12.15425;7.97770;,
 -11.20257;12.08845;-0.39758;,
 -7.73403;12.15425;-8.77288;,
 -7.73403;12.15425;-8.77288;,
 0.63975;12.31317;-12.24206;,
 0.74285;6.87987;-15.87310;,
 0.63975;12.31317;-12.24206;,
 9.01351;12.47209;-8.77288;,
 11.68371;7.08750;-11.34042;,
 9.01351;12.47209;-8.77288;,
 12.48209;12.53797;-0.39758;,
 9.01351;12.47209;7.97770;,
 5.10271;16.02957;4.13510;,
 0.57083;15.94355;6.01258;,
 -3.96103;15.85757;4.13510;,
 -7.73403;12.15425;7.97770;,
 -3.96103;15.85757;4.13510;,
 -5.83815;15.82197;-0.39758;,
 -3.96103;15.85757;-4.93026;,
 -7.73403;12.15425;-8.77288;,
 -3.96103;15.85757;-4.93026;,
 0.57083;15.94355;-6.80776;,
 5.10271;16.02957;-4.93026;,
 6.97987;16.06521;-0.39758;,
 12.48209;12.53797;-0.39758;,
 1.18235;-16.27667;-0.39758;,
 7.56717;-14.88027;-0.39758;,
 0.54667;17.21847;-0.39758;,
 -5.83815;15.82197;-0.39758;;
 
 164;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;3,8,9,10;,
 4;11,12,13,14;,
 4;15,16,17,11;,
 4;8,18,19,20;,
 4;21,22,23,24;,
 4;25,26,27,17;,
 4;4,24,23,5;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;11,17,27,12;,
 4;2,18,8,3;,
 3;0,3,10;,
 3;11,14,15;,
 3;8,20,9;,
 3;25,17,16;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;41,44,45,42;,
 4;37,46,47,38;,
 4;18,2,48,49;,
 4;50,51,30,29;,
 4;52,53,12,27;,
 4;54,55,56,57;,
 4;43,42,45,58;,
 3;18,49,19;,
 3;52,27,26;,
 3;2,1,48;,
 3;13,12,53;,
 4;59,60,61,62;,
 4;62,61,63,64;,
 4;64,63,65,66;,
 4;66,65,60,59;,
 4;67,68,69,70;,
 4;70,69,71,72;,
 4;73,74,68,67;,
 4;72,71,75,76;,
 4;76,75,77,78;,
 4;78,77,74,73;,
 4;79,80,81,82;,
 4;82,81,83,84;,
 4;85,86,87,88;,
 4;88,87,89,90;,
 4;91,92,93,94;,
 4;94,93,95,96;,
 4;97,98,99,100;,
 4;100,99,101,102;,
 3;103,104,105;,
 3;106,104,103;,
 3;107,104,106;,
 3;108,104,107;,
 3;109,104,108;,
 3;110,104,109;,
 3;111,104,110;,
 3;105,104,111;,
 3;112,113,114;,
 3;114,113,115;,
 3;115,113,116;,
 3;116,113,117;,
 3;117,113,118;,
 3;118,113,119;,
 3;119,113,120;,
 3;120,113,112;,
 4;121,122,123,124;,
 4;125,126,127,128;,
 4;129,130,131,132;,
 4;133,134,135,136;,
 4;135,137,138,136;,
 4;139,126,125,140;,
 4;141,142,143,144;,
 4;145,146,147,148;,
 4;149,150,151,152;,
 4;153,154,155,156;,
 4;155,157,158,156;,
 4;159,146,145,160;,
 4;161,162,163,164;,
 4;165,166,167,168;,
 4;169,170,171,172;,
 4;173,174,175,176;,
 4;177,168,167,178;,
 4;173,179,180,174;,
 4;181,182,183,184;,
 4;185,186,187,188;,
 4;189,190,191,192;,
 4;193,194,195,196;,
 4;197,198,199,200;,
 4;201,202,203,204;,
 4;205,206,207,208;,
 4;209,210,211,212;,
 4;213,214,215,216;,
 4;217,218,219,220;,
 4;221,212,211,222;,
 4;217,223,224,218;,
 4;225,226,227,228;,
 4;229,230,231,232;,
 4;233,234,235,236;,
 4;237,238,239,240;,
 4;241,232,231,242;,
 4;237,243,244,238;,
 4;245,246,247,248;,
 4;248,247,249,250;,
 4;250,249,251,252;,
 4;252,251,253,254;,
 4;254,253,255,256;,
 4;256,255,257,258;,
 4;259,260,261,262;,
 4;262,261,263,264;,
 4;265,266,267,268;,
 4;247,269,270,249;,
 4;271,272,273,274;,
 4;274,273,275,276;,
 4;253,277,278,255;,
 4;279,280,281,282;,
 4;260,283,284,261;,
 4;261,284,285,263;,
 4;266,286,287,267;,
 4;267,287,288,289;,
 4;272,290,291,273;,
 4;273,291,292,275;,
 4;293,294,295,280;,
 4;280,295,296,281;,
 4;283,297,298,284;,
 4;284,298,299,285;,
 4;286,300,301,287;,
 4;287,301,302,288;,
 4;290,303,304,291;,
 4;291,304,305,292;,
 4;294,306,307,295;,
 4;295,307,308,296;,
 4;297,309,310,298;,
 4;298,310,311,299;,
 4;312,313,314,315;,
 4;301,316,317,302;,
 4;303,318,319,304;,
 4;304,319,320,305;,
 4;306,321,322,307;,
 4;323,324,325,326;,
 4;309,327,328,310;,
 4;310,328,329,311;,
 4;313,330,331,314;,
 4;314,331,332,333;,
 4;318,334,335,319;,
 4;319,335,336,320;,
 4;337,338,339,324;,
 4;324,339,340,325;,
 4;325,340,341,342;,
 4;342,341,330,313;,
 3;248,343,245;,
 3;250,343,248;,
 3;252,343,250;,
 3;254,343,252;,
 3;256,343,254;,
 3;258,343,256;,
 3;344,343,258;,
 3;245,343,344;,
 3;330,345,331;,
 3;331,345,332;,
 3;332,345,346;,
 3;346,345,338;,
 3;338,345,339;,
 3;339,345,340;,
 3;340,345,341;,
 3;341,345,330;;
 
 MeshMaterialList {
  1;
  164;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "arms_00.png";
   }
  }
 }
 MeshNormals {
  170;
  -0.192458;-0.855062;0.481486;,
  -0.839034;-0.494637;0.226620;,
  -0.194495;-0.482406;0.854082;,
  -0.217514;-0.482742;-0.848321;,
  -0.844847;-0.494747;-0.203614;,
  -0.465249;-0.860238;-0.208649;,
  -0.471077;0.206236;0.857644;,
  -0.848639;0.199070;0.490084;,
  -0.224766;0.847142;0.481488;,
  -0.495779;0.841238;-0.215690;,
  -0.858504;0.195774;-0.473966;,
  -0.380946;0.351267;-0.855273;,
  0.139438;0.386957;0.911494;,
  0.028651;0.924187;0.380865;,
  0.019289;0.919636;-0.392297;,
  0.115061;0.376256;-0.919343;,
  0.249150;-0.363241;0.897764;,
  0.196598;-0.851683;0.485784;,
  0.183406;-0.851931;-0.490485;,
  0.224777;-0.363547;-0.904051;,
  0.941155;0.001376;0.337972;,
  0.468551;0.263387;0.843260;,
  0.710724;-0.323909;0.624463;,
  0.955092;-0.190347;0.227083;,
  0.999910;0.000439;-0.013420;,
  0.061347;0.918334;-0.391024;,
  0.962241;-0.271894;-0.012886;,
  0.873813;-0.424987;-0.236298;,
  0.868883;-0.494882;-0.011614;,
  0.521601;-0.853162;-0.006829;,
  0.930831;0.365151;-0.014797;,
  0.961396;0.133765;-0.240468;,
  0.908114;0.198966;-0.368431;,
  0.936328;0.003927;-0.351104;,
  0.746284;0.002300;-0.665624;,
  0.706980;0.013418;0.707106;,
  -0.000003;-0.000000;1.000000;,
  -0.706978;-0.013418;0.707108;,
  -0.999820;-0.018976;0.000001;,
  -0.706979;-0.013418;-0.707107;,
  -0.000003;-0.000000;-1.000000;,
  0.706981;0.013418;-0.707106;,
  0.999820;0.018976;0.000001;,
  0.018971;-0.999820;-0.000000;,
  -0.018975;0.999820;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.744928;-0.667145;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.003371;-0.020359;-0.999787;,
  -0.468757;0.883110;-0.019562;,
  -0.003378;0.020355;0.999787;,
  -0.019616;-0.006407;-0.999787;,
  0.802177;0.596765;-0.019563;,
  0.019611;0.006414;0.999787;,
  -0.018975;0.999820;0.000000;,
  0.757442;0.652903;0.000000;,
  -0.610981;-0.791646;0.000000;,
  0.286628;-0.919463;0.269131;,
  0.017546;-0.924570;0.380607;,
  -0.251537;-0.929677;0.269128;,
  -0.362992;-0.931792;0.000000;,
  -0.251536;-0.929677;-0.269127;,
  0.017546;-0.924571;-0.380606;,
  0.286629;-0.919463;-0.269130;,
  0.398084;-0.917349;-0.000000;,
  0.511862;-0.699643;0.498495;,
  0.013457;-0.709103;0.704977;,
  -0.484947;-0.718562;0.498494;,
  -0.691391;-0.722481;0.000000;,
  -0.484947;-0.718562;-0.498493;,
  0.013457;-0.709102;-0.704977;,
  0.511863;-0.699642;-0.498495;,
  0.718309;-0.695724;0.000000;,
  0.659912;-0.372097;0.652733;,
  0.007297;-0.384482;0.923104;,
  -0.645319;-0.396866;0.652734;,
  -0.915641;-0.401996;0.000000;,
  -0.645319;-0.396866;-0.652733;,
  0.007297;-0.384483;-0.923103;,
  0.659912;-0.372098;-0.652732;,
  0.930234;-0.366967;0.000000;,
  0.706979;0.013417;0.707107;,
  -0.000000;-0.000001;1.000000;,
  -0.706980;-0.013419;0.707107;,
  -0.999820;-0.018977;0.000000;,
  -0.706980;-0.013419;-0.707106;,
  -0.000000;-0.000001;-1.000000;,
  0.706979;0.013417;-0.707107;,
  0.999820;0.018975;0.000000;,
  0.645317;0.396868;0.652734;,
  -0.007297;0.384482;0.923104;,
  -0.659913;0.372094;0.652733;,
  -0.930236;0.366963;0.000000;,
  -0.659913;0.372094;-0.652733;,
  -0.007297;0.384483;-0.923103;,
  0.645317;0.396869;-0.652733;,
  0.915640;0.402000;0.000000;,
  0.484944;0.718562;0.498496;,
  -0.013457;0.709103;0.704976;,
  -0.511862;0.699643;0.498495;,
  -0.718309;0.695725;0.000000;,
  -0.511862;0.699643;-0.498494;,
  -0.013457;0.709103;-0.704977;,
  0.484945;0.718561;-0.498496;,
  0.691391;0.722481;0.000000;,
  0.251538;0.929674;0.269138;,
  -0.017546;0.924566;0.380617;,
  -0.286631;0.919461;0.269138;,
  -0.398091;0.917346;0.000000;,
  -0.286631;0.919461;-0.269137;,
  -0.017546;0.924567;-0.380616;,
  0.251538;0.929674;-0.269137;,
  0.362997;0.931790;0.000000;,
  0.018972;-0.999820;0.000000;,
  -0.018973;0.999820;0.000000;,
  -0.693496;-0.720397;0.009555;,
  -0.697362;-0.013235;0.716597;,
  0.003864;-0.707161;-0.707042;,
  -0.715806;-0.013464;-0.698169;,
  -0.003865;0.707162;0.707041;,
  -0.720333;0.693563;0.009556;,
  -0.022880;0.690934;-0.722556;,
  0.000400;1.000000;-0.000628;,
  0.346882;0.004365;-0.937899;,
  -0.118256;-0.114088;-0.986407;,
  -0.973625;-0.227773;0.013214;,
  -0.047369;-0.362129;0.930924;,
  -0.091557;-0.113581;0.989301;,
  0.999819;-0.013379;-0.013507;,
  0.026039;0.408428;-0.912419;,
  0.109228;-0.994017;-0.000558;,
  0.062133;0.998007;-0.011003;,
  0.999929;0.005023;0.010829;,
  0.000000;-0.586365;0.810047;,
  0.000000;-0.308647;0.951177;,
  0.000000;-0.000977;1.000000;,
  0.018969;-0.999820;-0.000000;,
  0.018976;-0.999820;-0.000002;,
  0.018966;-0.999820;0.000002;,
  0.018967;-0.999820;0.000000;,
  0.018966;-0.999820;-0.000002;,
  0.018969;-0.999820;-0.000000;,
  0.018976;-0.999820;0.000002;,
  0.018979;-0.999820;-0.000000;,
  -0.018974;0.999820;0.000009;,
  -0.018971;0.999820;0.000000;,
  -0.018977;0.999820;0.000000;,
  -0.018977;0.999820;0.000000;,
  -0.018977;0.999820;-0.000000;,
  -0.018971;0.999820;0.000000;,
  -0.018974;0.999820;-0.000009;,
  -0.018983;0.999820;0.000000;,
  -0.744929;0.667144;0.000000;,
  -0.667143;-0.744930;0.000000;,
  0.667146;0.744927;0.000000;,
  0.468752;-0.883113;0.019564;,
  0.883322;0.468721;-0.006563;,
  -0.883322;-0.468720;0.006562;,
  -0.802173;-0.596771;0.019560;,
  0.596760;-0.802393;-0.006565;,
  -0.596761;0.802392;0.006563;,
  0.018974;-0.999820;-0.000000;,
  0.999820;0.018976;0.000000;,
  -0.999820;-0.018976;0.000000;,
  -0.757444;-0.652900;0.000000;,
  0.652897;-0.757446;-0.000000;,
  -0.652898;0.757446;0.000000;,
  0.610993;0.791636;0.000000;,
  -0.815507;0.578747;0.000000;,
  0.815506;-0.578748;-0.000000;;
  164;
  4;0,17,16,2;,
  4;1,4,5,115;,
  4;116,6,7,116;,
  4;3,19,18,117;,
  4;118,10,11,118;,
  4;119,12,13,8;,
  4;120,9,120,120;,
  4;121,14,15,11;,
  4;1,7,10,4;,
  4;8,13,14,9;,
  4;17,0,5,18;,
  4;3,11,15,19;,
  4;16,12,6,2;,
  3;0,2,1;,
  3;3,5,4;,
  3;6,8,7;,
  3;9,11,10;,
  4;30,32,31,30;,
  4;21,22,23,20;,
  4;22,27,26,23;,
  4;32,34,33,31;,
  4;12,16,22,21;,
  4;122,25,14,13;,
  4;123,123,19,15;,
  4;28,28,29,29;,
  4;20,23,26,24;,
  3;12,21,13;,
  3;25,15,14;,
  3;16,17,22;,
  3;18,19,27;,
  4;124,124,124,124;,
  4;125,125,125,125;,
  4;126,127,127,127;,
  4;128,28,28,128;,
  4;129,129,129,129;,
  4;130,130,130,130;,
  4;131,131,131,131;,
  4;132,31,33,27;,
  4;126,133,134,134;,
  4;134,134,135,135;,
  4;35,35,36,36;,
  4;36,36,37,37;,
  4;37,37,38,38;,
  4;38,38,39,39;,
  4;39,39,40,40;,
  4;40,40,41,41;,
  4;41,41,42,42;,
  4;42,42,35,35;,
  3;136,43,137;,
  3;138,43,136;,
  3;139,43,138;,
  3;140,43,139;,
  3;141,43,140;,
  3;142,43,141;,
  3;143,43,142;,
  3;137,43,143;,
  3;144,44,145;,
  3;145,44,146;,
  3;146,44,147;,
  3;147,44,148;,
  3;148,44,149;,
  3;149,44,150;,
  3;150,44,151;,
  3;151,44,144;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;152,152,152,152;,
  4;153,153,153,153;,
  4;154,154,154,154;,
  4;48,48,48,48;,
  4;49,49,49,49;,
  4;50,50,50,50;,
  4;155,155,155,155;,
  4;156,156,156,156;,
  4;157,157,157,157;,
  4;51,51,51,51;,
  4;52,52,52,52;,
  4;53,53,53,53;,
  4;158,158,158,158;,
  4;159,159,159,159;,
  4;160,160,160,160;,
  4;45,45,45,45;,
  4;54,54,54,54;,
  4;47,47,47,47;,
  4;161,161,161,161;,
  4;162,162,162,162;,
  4;163,163,163,163;,
  4;45,45,45,45;,
  4;55,55,55,55;,
  4;47,47,47,47;,
  4;164,164,164,164;,
  4;165,165,165,165;,
  4;166,166,166,166;,
  4;45,45,45,45;,
  4;56,56,56,56;,
  4;47,47,47,47;,
  4;167,167,167,167;,
  4;168,168,168,168;,
  4;169,169,169,169;,
  4;57,65,66,58;,
  4;58,66,67,59;,
  4;59,67,68,60;,
  4;60,68,69,61;,
  4;61,69,70,62;,
  4;62,70,71,63;,
  4;63,71,72,64;,
  4;64,72,65,57;,
  4;65,73,74,66;,
  4;66,74,75,67;,
  4;67,75,76,68;,
  4;68,76,77,69;,
  4;69,77,78,70;,
  4;70,78,79,71;,
  4;71,79,80,72;,
  4;72,80,73,65;,
  4;73,81,82,74;,
  4;74,82,83,75;,
  4;75,83,84,76;,
  4;76,84,85,77;,
  4;77,85,86,78;,
  4;78,86,87,79;,
  4;79,87,88,80;,
  4;80,88,81,73;,
  4;81,89,90,82;,
  4;82,90,91,83;,
  4;83,91,92,84;,
  4;84,92,93,85;,
  4;85,93,94,86;,
  4;86,94,95,87;,
  4;87,95,96,88;,
  4;88,96,89,81;,
  4;89,97,98,90;,
  4;90,98,99,91;,
  4;91,99,100,92;,
  4;92,100,101,93;,
  4;93,101,102,94;,
  4;94,102,103,95;,
  4;95,103,104,96;,
  4;96,104,97,89;,
  4;97,105,106,98;,
  4;98,106,107,99;,
  4;99,107,108,100;,
  4;100,108,109,101;,
  4;101,109,110,102;,
  4;102,110,111,103;,
  4;103,111,112,104;,
  4;104,112,105,97;,
  3;58,113,57;,
  3;59,113,58;,
  3;60,113,59;,
  3;61,113,60;,
  3;62,113,61;,
  3;63,113,62;,
  3;64,113,63;,
  3;57,113,64;,
  3;105,114,106;,
  3;106,114,107;,
  3;107,114,108;,
  3;108,114,109;,
  3;109,114,110;,
  3;110,114,111;,
  3;111,114,112;,
  3;112,114,105;;
 }
 MeshTextureCoords {
  347;
  0.072960;0.436180;,
  0.105760;0.436610;,
  0.105730;0.430940;,
  0.072940;0.430510;,
  0.020570;0.442720;,
  0.043220;0.442740;,
  0.043280;0.446960;,
  0.020630;0.446940;,
  0.071620;0.367480;,
  0.064500;0.367390;,
  0.065820;0.430420;,
  0.072280;0.430510;,
  0.105080;0.430930;,
  0.105290;0.436600;,
  0.072500;0.436180;,
  0.065350;0.430420;,
  0.064030;0.367380;,
  0.070970;0.367710;,
  0.104420;0.367910;,
  0.104210;0.362240;,
  0.071410;0.361810;,
  0.020630;0.386830;,
  0.043280;0.386850;,
  0.043220;0.391790;,
  0.020570;0.391770;,
  0.070940;0.361810;,
  0.103740;0.362230;,
  0.103770;0.368130;,
  0.043250;0.447090;,
  0.042960;0.468480;,
  0.020310;0.468170;,
  0.020600;0.446790;,
  0.042670;0.386650;,
  0.021290;0.386940;,
  0.020980;0.364290;,
  0.042360;0.364000;,
  0.167060;0.375580;,
  0.152390;0.375700;,
  0.148060;0.368120;,
  0.171440;0.368090;,
  0.171440;0.368090;,
  0.171450;0.423670;,
  0.167070;0.419000;,
  0.167060;0.375580;,
  0.148060;0.423690;,
  0.152340;0.419010;,
  0.152340;0.419010;,
  0.148060;0.423690;,
  0.137850;0.421680;,
  0.137650;0.363190;,
  0.038950;0.490310;,
  0.023740;0.490100;,
  0.137320;0.363200;,
  0.137530;0.421680;,
  0.125440;0.486560;,
  0.079880;0.486420;,
  0.087480;0.443260;,
  0.118110;0.443360;,
  0.152390;0.375700;,
  0.128170;0.513140;,
  0.119820;0.497850;,
  0.072850;0.496840;,
  0.076580;0.512030;,
  0.073970;0.497700;,
  0.077820;0.512980;,
  0.121500;0.499140;,
  0.130010;0.514560;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.021960;0.497940;,
  0.021820;0.549810;,
  0.015910;0.549800;,
  0.016050;0.497920;,
  0.010000;0.549780;,
  0.010140;0.497910;,
  0.034240;0.550210;,
  0.034240;0.498490;,
  0.040160;0.498680;,
  0.040160;0.550400;,
  0.046070;0.498490;,
  0.046070;0.550210;,
  0.021710;0.550190;,
  0.022170;0.498320;,
  0.028080;0.498370;,
  0.027620;0.550240;,
  0.033990;0.498420;,
  0.033530;0.550290;,
  0.046560;0.550400;,
  0.046560;0.498680;,
  0.052470;0.498490;,
  0.052470;0.550210;,
  0.058380;0.498680;,
  0.058380;0.550400;,
  0.078950;0.550970;,
  0.078950;0.535670;,
  0.089740;0.546490;,
  0.068170;0.546490;,
  0.063700;0.535670;,
  0.068170;0.524850;,
  0.078950;0.520370;,
  0.089740;0.524850;,
  0.094200;0.535670;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.398550;0.415630;,
  0.386180;0.431190;,
  0.381390;0.427380;,
  0.393760;0.411830;,
  0.366650;0.412050;,
  0.366650;0.437020;,
  0.361860;0.437020;,
  0.361860;0.412050;,
  0.376350;0.431190;,
  0.363980;0.415630;,
  0.368770;0.411830;,
  0.381140;0.427380;,
  0.396460;0.412050;,
  0.396460;0.437020;,
  0.391670;0.437020;,
  0.391670;0.412050;,
  0.379310;0.437020;,
  0.379310;0.412050;,
  0.379010;0.437020;,
  0.379010;0.412050;,
  0.393650;0.437020;,
  0.381740;0.417450;,
  0.386000;0.414860;,
  0.397910;0.434420;,
  0.371500;0.417450;,
  0.362310;0.417260;,
  0.362340;0.414670;,
  0.371530;0.414860;,
  0.382360;0.431390;,
  0.394270;0.411830;,
  0.398530;0.414420;,
  0.386620;0.433990;,
  0.381480;0.436830;,
  0.372290;0.437020;,
  0.372250;0.434420;,
  0.381440;0.434240;,
  0.371800;0.414860;,
  0.381000;0.414670;,
  0.361860;0.436830;,
  0.371050;0.437020;,
  0.385960;0.414970;,
  0.401610;0.433620;,
  0.397550;0.437020;,
  0.381900;0.418380;,
  0.391190;0.414380;,
  0.400960;0.414310;,
  0.400920;0.418370;,
  0.391150;0.418440;,
  0.361870;0.433620;,
  0.377510;0.414970;,
  0.381570;0.418380;,
  0.365930;0.437020;,
  0.380630;0.429960;,
  0.390400;0.430030;,
  0.395390;0.414790;,
  0.385620;0.414720;,
  0.391630;0.434090;,
  0.401400;0.434010;,
  0.381110;0.414310;,
  0.390880;0.414380;,
  0.100920;0.579560;,
  0.101370;0.541180;,
  0.135640;0.541230;,
  0.135180;0.579600;,
  0.101280;0.578730;,
  0.101280;0.540990;,
  0.134740;0.540990;,
  0.134740;0.578730;,
  0.100040;0.579550;,
  0.100750;0.541170;,
  0.136010;0.541240;,
  0.135300;0.579610;,
  0.101260;0.578400;,
  0.101260;0.540930;,
  0.135140;0.540930;,
  0.135140;0.578400;,
  0.101200;0.579170;,
  0.101200;0.540910;,
  0.135120;0.540910;,
  0.135120;0.579170;,
  0.101180;0.579070;,
  0.101180;0.540810;,
  0.135100;0.540810;,
  0.135100;0.579070;,
  0.383310;0.411830;,
  0.397410;0.426370;,
  0.392930;0.430710;,
  0.378830;0.416170;,
  0.380430;0.411570;,
  0.380430;0.437020;,
  0.375960;0.437020;,
  0.375960;0.411570;,
  0.360010;0.426370;,
  0.374110;0.411830;,
  0.378580;0.416170;,
  0.364480;0.430710;,
  0.385210;0.411570;,
  0.385210;0.437020;,
  0.380730;0.437020;,
  0.380730;0.411570;,
  0.361860;0.411570;,
  0.361860;0.437020;,
  0.399310;0.411570;,
  0.399310;0.437020;,
  0.377950;0.417520;,
  0.365770;0.437020;,
  0.361860;0.434350;,
  0.374040;0.414860;,
  0.397500;0.437020;,
  0.378210;0.437020;,
  0.378210;0.434350;,
  0.397500;0.434350;,
  0.386690;0.411830;,
  0.398870;0.431320;,
  0.394960;0.433990;,
  0.382780;0.414490;,
  0.382520;0.414490;,
  0.363230;0.414490;,
  0.363230;0.411830;,
  0.382520;0.411830;,
  0.397500;0.414860;,
  0.378210;0.414860;,
  0.382520;0.433990;,
  0.363230;0.433990;,
  0.389180;0.429740;,
  0.393460;0.434370;,
  0.383390;0.438560;,
  0.383730;0.432000;,
  0.373320;0.434370;,
  0.378280;0.429740;,
  0.369150;0.424270;,
  0.376020;0.424270;,
  0.373320;0.414170;,
  0.378280;0.418800;,
  0.383390;0.409980;,
  0.383730;0.416540;,
  0.393460;0.414170;,
  0.389180;0.418800;,
  0.375680;0.441780;,
  0.371040;0.437050;,
  0.381150;0.436730;,
  0.381150;0.441600;,
  0.391250;0.437050;,
  0.386610;0.441780;,
  0.395210;0.435530;,
  0.388430;0.438110;,
  0.389460;0.424960;,
  0.395990;0.425460;,
  0.382880;0.442930;,
  0.369720;0.437470;,
  0.370170;0.440250;,
  0.367070;0.433960;,
  0.380270;0.434380;,
  0.380270;0.440580;,
  0.393470;0.433960;,
  0.390370;0.440250;,
  0.369720;0.411070;,
  0.382880;0.405600;,
  0.368160;0.425510;,
  0.374690;0.425010;,
  0.375710;0.438170;,
  0.368940;0.435590;,
  0.367950;0.430280;,
  0.381150;0.429860;,
  0.394340;0.430280;,
  0.380640;0.438600;,
  0.381750;0.424360;,
  0.382850;0.410110;,
  0.390480;0.411800;,
  0.365990;0.426330;,
  0.380270;0.426790;,
  0.394560;0.426330;,
  0.373670;0.411850;,
  0.381290;0.410170;,
  0.382400;0.424410;,
  0.383510;0.438650;,
  0.366860;0.422490;,
  0.381150;0.422030;,
  0.395430;0.422490;,
  0.373020;0.436910;,
  0.374040;0.423760;,
  0.375060;0.410600;,
  0.367070;0.418540;,
  0.380270;0.418970;,
  0.393470;0.418540;,
  0.389090;0.410650;,
  0.390110;0.423810;,
  0.391130;0.436970;,
  0.367950;0.414860;,
  0.381150;0.414440;,
  0.394340;0.414860;,
  0.392740;0.410910;,
  0.389140;0.414000;,
  0.379070;0.409820;,
  0.379580;0.405440;,
  0.367510;0.423250;,
  0.368290;0.413180;,
  0.370170;0.411770;,
  0.380270;0.412090;,
  0.390370;0.411770;,
  0.395860;0.413230;,
  0.396640;0.423300;,
  0.379580;0.442770;,
  0.379070;0.438390;,
  0.389140;0.434210;,
  0.392740;0.437300;,
  0.371040;0.408570;,
  0.381150;0.408240;,
  0.391250;0.408570;,
  0.384180;0.418640;,
  0.378730;0.416370;,
  0.373280;0.418640;,
  0.369000;0.414000;,
  0.374810;0.407040;,
  0.380270;0.407220;,
  0.385740;0.407040;,
  0.369000;0.434210;,
  0.373280;0.429570;,
  0.378730;0.431840;,
  0.384180;0.429570;,
  0.386440;0.424100;,
  0.393320;0.424100;,
  0.383850;0.424270;,
  0.391440;0.424270;,
  0.378610;0.424100;,
  0.371030;0.424100;;
 }
}
