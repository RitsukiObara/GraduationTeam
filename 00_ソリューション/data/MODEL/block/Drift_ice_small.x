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
 57;
 -4.58801;1.03771;-4.06867;,
 -4.04773;2.26974;-3.41906;,
 0.36396;2.26974;-3.99506;,
 0.27084;1.03771;-4.06867;,
 -4.27164;2.26974;0.00000;,
 -4.06892;2.54595;3.60745;,
 -1.78319;2.86048;-1.41649;,
 0.36396;2.26974;3.99506;,
 0.17089;2.94564;2.12221;,
 -4.58801;1.03771;4.06867;,
 0.27084;1.03771;4.06867;,
 -4.87774;-0.21869;4.12233;,
 -4.87774;-0.21869;-4.12233;,
 0.18556;-0.21869;-4.12233;,
 0.18556;-0.21869;4.12233;,
 8.86664;2.26974;0.91798;,
 9.18302;1.03771;0.93489;,
 8.82007;2.26974;0.00000;,
 9.08540;1.03771;-0.93489;,
 8.77351;2.26974;-0.91798;,
 3.79712;-0.21869;-3.47793;,
 3.89886;-0.21869;3.47793;,
 0.18556;-0.21869;4.12233;,
 0.27084;1.03771;4.06867;,
 3.83420;1.03771;3.43266;,
 0.36396;2.26974;3.99506;,
 2.06377;2.26974;3.68281;,
 3.76359;2.26974;3.37056;,
 3.35739;2.96708;-1.40758;,
 2.04980;3.02086;-0.96141;,
 2.01721;2.26974;-3.68281;,
 3.67045;2.26974;-3.37056;,
 3.73657;1.03771;-3.43266;,
 2.01721;2.26974;-3.68281;,
 3.67045;2.26974;-3.37056;,
 6.85091;-0.21869;-2.61626;,
 6.95265;-0.21869;2.61626;,
 5.16150;2.26974;2.95302;,
 6.76468;1.03771;2.58220;,
 6.55941;2.26974;2.53549;,
 6.51284;2.87304;0.87052;,
 5.59749;3.10107;-1.23161;,
 5.06837;2.26974;-2.95302;,
 6.46628;2.26974;-2.53549;,
 6.66705;1.03771;-2.58220;,
 8.18733;-0.21869;-1.93275;,
 8.28907;-0.21869;1.93275;,
 8.04714;1.03771;1.90759;,
 7.78295;2.26974;1.87308;,
 7.38407;2.66205;-0.12601;,
 7.68981;2.26974;-1.87308;,
 7.94951;1.03771;-1.90759;,
 9.37102;-0.21869;-0.94722;,
 9.47275;-0.21869;0.94722;,
 4.62472;2.90644;1.24173;,
 2.06377;2.26974;3.68281;,
 2.04980;3.02086;-0.96141;;
 
 64;
 4;0,1,2,3;,
 3;4,5,6;,
 3;5,7,6;,
 3;6,7,8;,
 4;5,9,10,7;,
 4;11,12,13,14;,
 3;15,16,17;,
 3;16,18,17;,
 3;17,18,19;,
 3;0,9,1;,
 3;1,9,4;,
 3;4,9,5;,
 4;13,20,21,22;,
 3;23,24,25;,
 3;25,24,26;,
 3;26,24,27;,
 3;26,28,29;,
 3;29,28,30;,
 3;30,28,31;,
 3;3,2,32;,
 3;32,33,34;,
 3;33,32,2;,
 4;20,35,36,21;,
 3;27,24,37;,
 3;24,38,37;,
 3;37,38,39;,
 4;37,39,40,41;,
 3;34,42,32;,
 3;42,43,32;,
 3;32,43,44;,
 4;35,45,46,36;,
 4;47,48,39,38;,
 3;39,48,40;,
 3;40,48,43;,
 3;43,49,50;,
 3;49,43,48;,
 4;43,50,51,44;,
 4;45,52,53,46;,
 4;16,15,48,47;,
 4;48,15,17,49;,
 4;50,19,18,51;,
 4;12,0,3,13;,
 4;32,20,13,3;,
 4;44,35,20,32;,
 4;51,45,35,44;,
 4;18,52,45,51;,
 4;52,18,16,53;,
 4;46,53,16,47;,
 4;36,46,47,38;,
 4;21,36,38,24;,
 4;22,21,24,23;,
 4;9,11,14,10;,
 4;11,9,0,12;,
 3;42,34,28;,
 4;27,37,41,54;,
 4;26,27,54,28;,
 4;7,55,56,8;,
 4;41,42,28,54;,
 4;40,43,42,41;,
 3;6,8,2;,
 3;2,8,33;,
 3;33,8,56;,
 4;1,4,6,2;,
 4;17,19,50,49;;
 
 MeshMaterialList {
  1;
  64;
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
    "C:\\Users\\student\\Documents\\モデル\\全部\\おばら\\ステージモデル\\流氷\\data\\TEXTURE\\small_ice.png";
   }
  }
 }
 MeshNormals {
  75;
  0.000000;-1.000000;-0.000000;,
  -0.066132;0.274069;-0.959434;,
  0.101081;0.127774;-0.986639;,
  -0.105567;0.994310;0.014220;,
  -0.151543;0.959968;0.235576;,
  0.233315;0.062285;0.970404;,
  0.078480;0.943284;-0.322579;,
  0.378296;0.101279;0.920127;,
  0.345344;0.087494;-0.934389;,
  0.560704;0.143697;0.815452;,
  0.162700;0.975057;-0.150973;,
  0.893811;0.230082;0.384921;,
  0.874930;0.222765;-0.429968;,
  -0.033315;0.159560;-0.986626;,
  0.073857;0.100851;-0.992156;,
  0.231882;0.058380;-0.970990;,
  0.371601;0.088509;-0.924164;,
  0.556408;0.131680;-0.820409;,
  0.907303;0.221700;-0.357282;,
  0.926903;0.227952;0.298141;,
  0.556260;0.133568;0.820204;,
  0.354491;0.086787;0.931023;,
  0.221413;0.056982;0.973514;,
  0.077272;0.077023;0.994030;,
  -0.020926;0.111829;0.993507;,
  0.282923;0.073901;0.956291;,
  0.285611;0.063528;-0.956238;,
  0.018552;0.991715;-0.127111;,
  0.180352;0.059825;0.981781;,
  -0.100215;0.965719;-0.239465;,
  0.049958;0.994036;0.096932;,
  0.137691;0.990464;0.004659;,
  0.273966;0.961256;0.030509;,
  -0.159660;0.986831;0.025960;,
  -0.063236;0.997671;-0.025573;,
  -0.506817;0.859789;-0.062447;,
  -0.173047;0.984623;0.023937;,
  0.177968;0.981104;-0.075903;,
  0.967807;0.246763;-0.049573;,
  -0.041742;0.180403;0.982707;,
  0.105474;0.095930;0.989784;,
  -0.949249;0.314526;0.000000;,
  -0.967547;0.252511;-0.009552;,
  -0.973788;0.225014;-0.033240;,
  -0.997048;0.040399;-0.065295;,
  -0.942592;0.332813;0.027485;,
  0.062008;0.977094;0.203572;,
  0.185265;0.059387;-0.980893;,
  0.235753;0.061540;-0.969863;,
  0.104121;0.957679;0.268347;,
  0.074372;0.951997;0.296937;,
  0.312555;0.945969;0.086331;,
  0.335780;0.913582;-0.229390;,
  -0.862463;0.435669;0.257585;,
  0.560875;0.141697;-0.815684;,
  0.205275;0.975813;0.075176;,
  0.211659;0.977255;-0.013154;,
  0.000000;0.042669;-0.999089;,
  0.089334;0.040046;-0.995196;,
  0.226616;0.047030;-0.972848;,
  0.368380;0.077618;-0.926430;,
  0.551863;0.121645;-0.825015;,
  0.876089;0.199483;-0.438948;,
  0.896803;0.205361;0.391883;,
  0.551735;0.123421;0.824837;,
  0.368340;0.078815;0.926344;,
  0.224269;0.047666;0.973361;,
  0.086941;0.040220;0.995401;,
  0.000000;0.042669;0.999089;,
  -0.974427;0.224706;0.000000;,
  0.139362;0.949048;-0.282640;,
  0.095375;0.965897;0.240719;,
  0.009617;0.993143;-0.116512;,
  -0.137617;0.981396;-0.133883;,
  0.216382;0.971031;-0.101382;;
  64;
  4;13,1,2,14;,
  3;35,3,36;,
  3;3,4,36;,
  3;36,4,33;,
  4;39,24,23,40;,
  4;0,0,0,0;,
  3;11,19,38;,
  3;19,18,38;,
  3;38,18,12;,
  3;41,42,43;,
  3;43,42,44;,
  3;35,42,45;,
  4;0,0,0,0;,
  3;23,22,40;,
  3;40,22,28;,
  3;28,22,5;,
  3;46,27,34;,
  3;34,27,29;,
  3;29,27,6;,
  3;14,2,15;,
  3;15,47,48;,
  3;47,15,2;,
  4;0,0,0,0;,
  3;5,22,25;,
  3;22,21,25;,
  3;25,21,7;,
  4;49,50,32,31;,
  3;48,26,15;,
  3;26,8,15;,
  3;15,8,16;,
  4;0,0,0,0;,
  4;20,9,7,21;,
  3;50,51,32;,
  3;32,51,52;,
  3;52,37,10;,
  3;53,53,53;,
  4;8,54,17,16;,
  4;0,0,0,0;,
  4;19,11,9,20;,
  4;51,55,56,37;,
  4;54,12,18,17;,
  4;57,13,14,58;,
  4;15,59,58,14;,
  4;16,60,59,15;,
  4;17,61,60,16;,
  4;18,62,61,17;,
  4;62,18,19,63;,
  4;64,63,19,20;,
  4;65,64,20,21;,
  4;66,65,21,22;,
  4;67,66,22,23;,
  4;24,68,67,23;,
  4;69,42,41,69;,
  3;70,6,27;,
  4;71,49,31,30;,
  4;46,71,30,27;,
  4;4,46,34,33;,
  4;31,70,27,30;,
  4;32,52,70,31;,
  3;36,33,72;,
  3;72,33,29;,
  3;29,33,34;,
  4;73,35,36,72;,
  4;56,74,10,37;;
 }
 MeshTextureCoords {
  57;
  -0.200160;0.597690;,
  -0.213680;0.491840;,
  0.044360;0.420900;,
  0.038620;0.633980;,
  -0.368710;0.489370;,
  -0.531000;0.468550;,
  -0.266200;0.365450;,
  -0.781770;0.420900;,
  -0.630340;0.234830;,
  -0.537250;0.597690;,
  -0.776040;0.633980;,
  -0.533040;0.693230;,
  -0.204380;0.693230;,
  0.032970;0.833360;,
  -0.770390;0.833360;,
  1.009280;0.489740;,
  1.008100;0.600260;,
  0.967330;0.488830;,
  0.925990;0.600760;,
  0.924790;0.488670;,
  0.531900;0.894370;,
  1.390190;0.888880;,
  1.901700;0.833360;,
  1.896050;0.633980;,
  1.395620;0.649370;,
  1.890320;0.420900;,
  1.673370;0.377650;,
  1.401110;0.389520;,
  0.646360;0.021810;,
  2.823010;-0.282080;,
  2.925870;0.377960;,
  3.193380;0.386570;,
  0.526620;0.650880;,
  0.253780;0.377960;,
  0.521300;0.386570;,
  0.802990;0.764780;,
  1.128420;0.761150;,
  1.235560;0.425670;,
  1.132570;0.615430;,
  1.136910;0.457800;,
  1.029090;0.366390;,
  0.857450;0.283040;,
  0.692380;0.422720;,
  0.794370;0.455800;,
  0.798780;0.616390;,
  0.870230;0.728450;,
  1.062850;0.725880;,
  1.065480;0.606260;,
  1.068240;0.477030;,
  0.959870;0.421960;,
  0.864760;0.475600;,
  0.867560;0.606930;,
  0.927120;0.704590;,
  1.006980;0.702670;,
  1.120980;0.229040;,
  -0.998720;0.377650;,
  0.150920;-0.282080;;
 }
}
