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
 296;
 17.47965;2.77675;9.85315;,
 22.94519;7.97285;12.91794;,
 13.24739;7.97285;22.37669;,
 10.09184;2.77675;17.05885;,
 -0.00002;7.97285;25.81330;,
 -0.00002;2.77675;20.76336;,
 -13.24744;7.97285;22.37669;,
 -10.09194;2.77675;17.05885;,
 -22.94523;7.97285;12.91794;,
 -17.47970;2.77675;9.85315;,
 -26.36360;7.97285;-0.00142;,
 -20.08474;2.77675;0.01224;,
 -18.41914;10.84970;-18.45283;,
 -17.47970;2.77675;-9.83315;,
 -12.20461;6.56885;-20.04363;,
 -6.88792;2.99544;-19.76872;,
 -6.88792;2.99544;-19.76872;,
 -12.20461;6.56885;-20.04363;,
 -0.00002;7.70298;-24.65312;,
 -0.00002;2.48701;-20.26122;,
 12.20456;6.56885;-20.04363;,
 6.88782;2.99544;-19.76872;,
 6.88782;2.99544;-19.76872;,
 12.20456;6.56885;-20.04363;,
 18.41909;10.84970;-18.45283;,
 17.47965;2.77675;-9.83315;,
 26.36362;7.97285;-0.00142;,
 20.08478;2.77675;0.01224;,
 25.32874;14.18934;14.22253;,
 14.62355;14.18934;24.66384;,
 -0.00002;14.18934;28.67149;,
 -14.62358;14.18934;24.66384;,
 -25.32877;14.18934;14.22253;,
 -29.10224;14.18934;-0.03914;,
 -22.68731;16.30733;-16.83678;,
 -18.41914;10.84970;-18.45283;,
 -22.68731;16.30733;-16.83678;,
 -14.62358;15.24811;-22.95757;,
 -0.00002;14.76208;-28.53000;,
 14.62355;15.24811;-22.95757;,
 22.68728;16.30733;-16.83678;,
 18.41909;10.84970;-18.45283;,
 22.68728;16.30733;-16.83678;,
 29.10226;14.18934;-0.03914;,
 26.02010;21.28371;14.61184;,
 15.02270;21.28371;25.33816;,
 -0.00002;21.28371;29.52544;,
 -15.02274;21.28371;25.33816;,
 -26.02015;21.28371;14.61184;,
 -29.89381;21.28371;-0.03914;,
 -23.92506;21.28371;-15.67026;,
 -23.92506;21.28371;-15.67026;,
 -15.26552;21.28371;-22.81011;,
 -0.00002;21.28371;-31.06885;,
 15.26550;21.28371;-22.81011;,
 23.92502;21.28371;-15.67026;,
 23.92502;21.28371;-15.67026;,
 29.89386;21.28371;-0.03914;,
 25.14717;28.76291;14.13717;,
 14.94789;29.08653;25.21178;,
 -0.00002;29.08653;29.45085;,
 -14.94792;29.08653;25.21178;,
 -25.14722;28.76291;14.13717;,
 -28.88414;28.76291;-0.02230;,
 -23.74093;27.14949;-15.23586;,
 -23.74093;27.14949;-15.23586;,
 -15.59328;28.76291;-22.57658;,
 -0.00002;28.98082;-29.97688;,
 -3.87864;27.96243;-27.06447;,
 15.59325;28.76291;-22.57658;,
 3.87861;27.96243;-27.06447;,
 23.74088;27.14949;-15.23586;,
 23.74088;27.14949;-15.23586;,
 28.88418;28.76291;-0.02230;,
 23.03862;35.35259;12.94982;,
 13.30134;35.35259;22.44709;,
 -0.00002;35.35259;27.59578;,
 -13.30138;35.35259;22.44709;,
 -23.03867;35.35259;12.94982;,
 -26.45095;35.35259;-0.02230;,
 -22.12568;32.78282;-15.27979;,
 -22.12568;32.78282;-15.27979;,
 -14.18746;35.35259;-21.18200;,
 -5.88941;35.35259;-25.09467;,
 -0.00002;35.35259;-27.69486;,
 -0.00002;28.98082;-29.97688;,
 -3.87864;27.96243;-27.06447;,
 5.88938;35.35259;-25.09467;,
 14.18743;35.35259;-21.18200;,
 22.12563;32.78282;-15.27979;,
 22.12563;32.78282;-15.27979;,
 26.45100;35.35259;-0.02230;,
 20.16802;41.50275;11.33331;,
 11.64402;41.50275;19.64725;,
 -0.00002;41.50275;24.33133;,
 -11.64405;41.50275;19.64725;,
 -20.16807;41.50275;11.33331;,
 -23.13904;41.50275;-0.02230;,
 -17.84202;38.21080;-16.96181;,
 -17.84202;38.21080;-16.96181;,
 -12.11293;39.61543;-19.06803;,
 -7.82125;38.78930;-22.76610;,
 -0.00002;41.50275;-23.90282;,
 7.82122;38.78930;-22.76610;,
 12.11291;39.61543;-19.06803;,
 17.84198;38.21080;-16.96181;,
 17.84198;38.21080;-16.96181;,
 23.13910;41.50275;-0.02230;,
 15.90810;45.67803;8.93445;,
 9.18453;47.02903;15.49229;,
 -0.00002;47.02903;19.42549;,
 -9.18456;47.02903;15.49229;,
 -15.90811;45.67803;8.93445;,
 -18.22978;45.67803;-0.02230;,
 -15.90811;45.67803;-8.98186;,
 -17.84202;38.21080;-16.96181;,
 -15.90811;45.67803;-8.98186;,
 -9.18456;45.67803;-15.53967;,
 -12.11293;39.61543;-19.06803;,
 -0.00002;45.67803;-19.52454;,
 9.18453;45.67803;-15.53967;,
 7.82122;38.78930;-22.76610;,
 12.11291;39.61543;-19.06803;,
 15.90810;45.67803;-8.98186;,
 17.84198;38.21080;-16.96181;,
 15.90810;45.67803;-8.98186;,
 18.22983;45.67803;-0.02230;,
 11.47203;49.39887;6.44003;,
 6.62338;49.39887;11.16918;,
 -0.00002;49.39887;14.39394;,
 -6.62346;49.39887;11.16918;,
 -11.47206;49.39887;6.44003;,
 -13.11064;49.39887;-0.01853;,
 -11.47206;49.39887;-6.48026;,
 -6.62346;49.39887;-11.20941;,
 -9.18456;45.67803;-15.53967;,
 -0.00002;49.39887;-14.49302;,
 -0.00002;45.67803;-19.52454;,
 6.62338;49.39887;-11.20941;,
 9.18453;45.67803;-15.53967;,
 11.47203;49.39887;-6.48026;,
 13.11069;49.39887;-0.01853;,
 5.99077;0.00000;10.11386;,
 0.00000;0.00000;0.00000;,
 10.37637;0.00000;5.83638;,
 0.00000;0.00000;12.31294;,
 -5.99082;0.00000;10.11386;,
 -10.37637;0.00000;5.83638;,
 -11.92278;0.00000;-0.00544;,
 -10.37637;0.00000;-5.84990;,
 -5.99082;0.00000;-10.12738;,
 0.00000;0.00000;-12.37544;,
 5.99077;0.00000;-10.12738;,
 10.37637;0.00000;-5.84990;,
 11.92283;0.00000;-0.00544;,
 5.73600;52.40917;3.21202;,
 -0.00002;53.15935;-0.01600;,
 3.31168;52.40917;5.57659;,
 -0.00002;52.40917;7.18898;,
 -3.31173;52.40917;5.57659;,
 -5.73605;52.40917;3.21202;,
 -6.55533;52.40917;-0.01726;,
 -5.73605;52.40917;-3.24813;,
 -3.31173;52.40917;-5.61270;,
 -0.00002;52.40917;-7.25451;,
 3.31168;52.40917;-5.61270;,
 5.73600;52.40917;-3.24813;,
 6.55534;52.40917;-0.01726;,
 10.09184;2.77675;17.05885;,
 8.33496;1.32328;14.08367;,
 14.43666;1.32328;8.13238;,
 17.47965;2.77675;9.85315;,
 -0.00002;2.77675;20.76336;,
 -0.00002;1.32328;17.14323;,
 -10.09194;2.77675;17.05885;,
 -8.33504;1.32328;14.08367;,
 -17.47970;2.77675;9.85315;,
 -14.43667;1.32328;8.13238;,
 -20.08474;2.77675;0.01224;,
 -16.58819;1.32328;0.00466;,
 -17.47970;2.77675;-9.83315;,
 -14.43667;1.32328;-8.12675;,
 -6.88792;2.99544;-19.76872;,
 -8.33504;1.32328;-14.07800;,
 -0.00002;2.48701;-20.26122;,
 -0.00002;1.32328;-17.20574;,
 6.88782;2.99544;-19.76872;,
 8.33496;1.32328;-14.07802;,
 17.47965;2.77675;-9.83315;,
 14.43666;1.32328;-8.12675;,
 20.08478;2.77675;0.01224;,
 16.58824;1.32328;0.00466;,
 -7.82125;38.78930;-22.76610;,
 -5.88941;35.35259;-25.09467;,
 5.88938;35.35259;-25.09467;,
 3.87861;27.96243;-27.06447;,
 -5.66174;16.44862;-29.05714;,
 -8.33598;18.49840;-27.92197;,
 -4.74453;20.55715;-38.29127;,
 -2.83088;15.19914;-30.15936;,
 -0.00002;20.37573;-47.65176;,
 -0.00002;14.24506;-31.51780;,
 4.74450;20.55715;-38.29127;,
 2.83085;15.19914;-30.15936;,
 8.33595;18.49840;-27.92197;,
 5.66171;16.44862;-29.05714;,
 -7.55285;21.71907;-28.90154;,
 -3.77643;23.96893;-36.68181;,
 -0.00002;24.24552;-41.57411;,
 3.77640;23.96893;-36.68181;,
 7.55282;21.71907;-28.90154;,
 -7.55285;21.71907;-28.90154;,
 -5.66174;27.33022;-29.16458;,
 -2.83088;29.01611;-30.31336;,
 -3.77643;23.96893;-36.68181;,
 -0.00002;30.79480;-31.43352;,
 -0.00002;24.24552;-41.57411;,
 2.83085;29.01611;-30.31336;,
 3.77640;23.96893;-36.68181;,
 5.66171;27.33022;-29.16458;,
 7.55282;21.71907;-28.90154;,
 -6.36658;27.93547;-26.47285;,
 -3.18330;29.82365;-26.54955;,
 -0.00002;31.76763;-27.71139;,
 3.18326;29.82365;-26.54955;,
 6.36654;27.93547;-26.47285;,
 -8.49310;21.61818;-25.56162;,
 -4.24656;21.61818;-26.54955;,
 -0.00002;21.61818;-27.71139;,
 4.24653;21.61818;-26.54955;,
 8.49307;21.61818;-25.56162;,
 -9.37373;17.99654;-22.27968;,
 -4.68686;17.99654;-26.47285;,
 -0.00002;17.99654;-27.71139;,
 4.68683;17.99654;-26.54955;,
 9.37370;17.99654;-22.27968;,
 -6.36658;15.69163;-25.73366;,
 -3.18330;14.28658;-26.54955;,
 -0.00002;13.74213;-27.71139;,
 3.18326;14.28658;-26.54955;,
 6.36654;15.69163;-25.73366;,
 -6.36658;15.69163;-25.73366;,
 -3.18330;14.28658;-26.54955;,
 -0.00002;13.74213;-27.71139;,
 3.18326;14.28658;-26.54955;,
 6.36654;15.69163;-25.73366;,
 9.37370;17.99654;-22.27968;,
 8.49307;21.61818;-25.56162;,
 -9.37373;17.99654;-22.27968;,
 -8.49310;21.61818;-25.56162;,
 11.90744;44.10890;-20.50251;,
 12.21288;48.78691;-16.24040;,
 14.86246;46.67920;-15.39216;,
 14.63736;42.66924;-19.11802;,
 19.00101;41.72594;-15.22570;,
 19.08928;40.32147;-16.86021;,
 23.76629;41.98367;-10.87653;,
 23.54120;37.97370;-14.60240;,
 26.57658;41.21206;-8.95579;,
 26.27112;36.53408;-13.21789;,
 10.79824;47.65928;-14.62358;,
 13.44782;45.55157;-13.77533;,
 17.58635;40.59832;-13.60890;,
 22.35165;40.85603;-9.25973;,
 25.16192;40.08447;-7.33898;,
 9.92382;42.52773;-18.23541;,
 12.65376;41.08806;-16.85093;,
 17.10566;38.74031;-14.59312;,
 21.55759;36.39255;-12.33530;,
 24.28750;34.95290;-10.95080;,
 9.61837;37.84970;-22.49754;,
 11.03301;38.97733;-24.11435;,
 13.84330;38.20576;-22.19360;,
 12.42864;37.07811;-20.57679;,
 18.60859;38.46346;-17.84443;,
 17.19395;37.33584;-16.22762;,
 22.74714;33.51023;-17.67798;,
 21.33248;32.38259;-16.06117;,
 25.39669;31.40250;-16.82973;,
 23.98205;30.27486;-15.21291;,
 16.04814;24.05702;-24.36167;,
 12.31917;27.07749;-26.02414;,
 16.08677;29.52784;-23.67059;,
 20.13211;26.44394;-20.09086;,
 15.46727;29.35534;-22.45717;,
 19.43616;26.22109;-18.87139;,
 11.62325;26.85466;-24.80463;,
 15.42861;23.88454;-23.14824;,
 -16.04816;24.05702;-24.36167;,
 -20.13215;26.44394;-20.09086;,
 -16.08680;29.52784;-23.67059;,
 -12.31920;27.07749;-26.02414;,
 -19.43619;26.22109;-18.87139;,
 -15.46730;29.35534;-22.45717;,
 -15.42862;23.88454;-23.14822;,
 -11.62328;26.85466;-24.80463;;
 
 246;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;16,17,18,19;,
 4;19,18,20,21;,
 4;22,23,24,25;,
 4;25,24,26,27;,
 4;27,26,1,0;,
 4;1,28,29,2;,
 4;2,29,30,4;,
 4;4,30,31,6;,
 4;6,31,32,8;,
 4;8,32,33,10;,
 4;10,33,34,12;,
 4;35,36,37,17;,
 4;17,37,38,18;,
 4;18,38,39,20;,
 4;20,39,40,41;,
 4;24,42,43,26;,
 4;26,43,28,1;,
 4;28,44,45,29;,
 4;29,45,46,30;,
 4;30,46,47,31;,
 4;31,47,48,32;,
 4;32,48,49,33;,
 4;33,49,50,34;,
 4;36,51,52,37;,
 4;37,52,53,38;,
 4;38,53,54,39;,
 4;39,54,55,40;,
 4;42,56,57,43;,
 4;43,57,44,28;,
 4;44,58,59,45;,
 4;45,59,60,46;,
 4;46,60,61,47;,
 4;47,61,62,48;,
 4;48,62,63,49;,
 4;49,63,64,50;,
 4;51,65,66,52;,
 3;53,52,67;,
 3;67,52,68;,
 3;68,52,66;,
 3;69,54,70;,
 3;54,53,70;,
 3;70,53,67;,
 4;54,69,71,55;,
 4;56,72,73,57;,
 4;57,73,58,44;,
 4;58,74,75,59;,
 4;59,75,76,60;,
 4;60,76,77,61;,
 4;61,77,78,62;,
 4;62,78,79,63;,
 4;63,79,80,64;,
 4;65,81,82,66;,
 4;83,84,85,86;,
 4;87,88,69,70;,
 4;69,88,89,71;,
 4;72,90,91,73;,
 4;73,91,74,58;,
 4;74,92,93,75;,
 4;75,93,94,76;,
 4;76,94,95,77;,
 4;77,95,96,78;,
 4;78,96,97,79;,
 4;79,97,98,80;,
 4;81,99,100,82;,
 4;101,102,84,83;,
 4;103,104,88,87;,
 4;88,104,105,89;,
 4;90,106,107,91;,
 4;91,107,92,74;,
 4;92,108,109,93;,
 4;93,109,110,94;,
 4;94,110,111,95;,
 4;95,111,112,96;,
 4;96,112,113,97;,
 4;97,113,114,98;,
 4;115,116,117,118;,
 3;118,117,101;,
 3;101,117,102;,
 3;102,117,119;,
 3;119,120,102;,
 3;102,120,121;,
 3;121,120,122;,
 4;122,120,123,124;,
 4;106,125,126,107;,
 4;107,126,108,92;,
 4;108,127,128,109;,
 4;109,128,129,110;,
 4;110,129,130,111;,
 4;111,130,131,112;,
 4;112,131,132,113;,
 4;113,132,133,114;,
 4;114,133,134,135;,
 4;135,134,136,137;,
 4;137,136,138,139;,
 4;139,138,140,125;,
 4;125,140,141,126;,
 4;126,141,127,108;,
 3;142,143,144;,
 3;145,143,142;,
 3;146,143,145;,
 3;147,143,146;,
 3;148,143,147;,
 3;149,143,148;,
 3;150,143,149;,
 3;151,143,150;,
 3;152,143,151;,
 3;153,143,152;,
 3;154,143,153;,
 3;144,143,154;,
 3;155,156,157;,
 3;157,156,158;,
 3;158,156,159;,
 3;159,156,160;,
 3;160,156,161;,
 3;161,156,162;,
 3;162,156,163;,
 3;163,156,164;,
 3;164,156,165;,
 3;165,156,166;,
 3;166,156,167;,
 3;167,156,155;,
 4;136,164,165,138;,
 4;138,165,166,140;,
 4;140,166,167,141;,
 4;141,167,155,127;,
 4;127,155,157,128;,
 4;128,157,158,129;,
 4;129,158,159,130;,
 4;130,159,160,131;,
 4;131,160,161,132;,
 4;132,161,162,133;,
 4;133,162,163,134;,
 4;134,163,164,136;,
 4;168,169,170,171;,
 4;172,173,169,168;,
 4;174,175,173,172;,
 4;176,177,175,174;,
 4;178,179,177,176;,
 4;180,181,179,178;,
 4;182,183,181,180;,
 4;184,185,183,182;,
 4;186,187,185,184;,
 4;188,189,187,186;,
 4;190,191,189,188;,
 4;171,170,191,190;,
 4;169,142,144,170;,
 4;173,145,142,169;,
 4;175,146,145,173;,
 4;177,147,146,175;,
 4;179,148,147,177;,
 4;181,149,148,179;,
 4;183,150,149,181;,
 4;185,151,150,183;,
 4;187,152,151,185;,
 4;189,153,152,187;,
 4;191,154,153,189;,
 4;170,144,154,191;,
 4;82,100,192,193;,
 4;84,102,121,194;,
 4;66,82,193,68;,
 4;85,84,194,195;,
 4;196,197,198,199;,
 4;199,198,200,201;,
 4;201,200,202,203;,
 4;203,202,204,205;,
 4;197,206,207,198;,
 4;198,207,208,200;,
 4;200,208,209,202;,
 4;202,209,210,204;,
 4;211,212,213,214;,
 4;214,213,215,216;,
 4;216,215,217,218;,
 4;218,217,219,220;,
 4;212,221,222,213;,
 4;213,222,223,215;,
 4;215,223,224,217;,
 4;217,224,225,219;,
 4;221,226,227,222;,
 4;222,227,228,223;,
 4;223,228,229,224;,
 4;224,229,230,225;,
 4;226,231,232,227;,
 4;227,232,233,228;,
 4;228,233,234,229;,
 4;229,234,235,230;,
 4;231,236,237,232;,
 4;232,237,238,233;,
 4;233,238,239,234;,
 4;234,239,240,235;,
 4;241,196,199,242;,
 4;242,199,201,243;,
 4;243,201,203,244;,
 4;244,203,205,245;,
 4;205,204,246,245;,
 4;204,210,247,246;,
 4;220,219,225,230;,
 4;241,248,197,196;,
 4;248,249,206,197;,
 4;226,221,212,211;,
 4;250,251,252,253;,
 4;253,252,254,255;,
 4;255,254,256,257;,
 4;257,256,258,259;,
 4;251,260,261,252;,
 4;252,261,262,254;,
 4;254,262,263,256;,
 4;256,263,264,258;,
 4;260,265,266,261;,
 4;261,266,267,262;,
 4;262,267,268,263;,
 4;263,268,269,264;,
 4;270,271,272,273;,
 4;273,272,274,275;,
 4;275,274,276,277;,
 4;277,276,278,279;,
 4;259,258,264,269;,
 4;265,260,251,250;,
 4;271,250,253,272;,
 4;272,253,255,274;,
 4;274,255,257,276;,
 4;276,257,259,278;,
 4;278,259,269,279;,
 4;268,277,279,269;,
 4;267,275,277,268;,
 4;266,273,275,267;,
 4;265,270,273,266;,
 4;270,265,250,271;,
 4;280,281,282,283;,
 4;283,282,284,285;,
 4;285,284,286,287;,
 4;287,286,281,280;,
 4;281,286,284,282;,
 4;287,280,283,285;,
 4;288,289,290,291;,
 4;289,292,293,290;,
 4;292,294,295,293;,
 4;294,288,291,295;,
 4;291,290,293,295;,
 4;294,292,289,288;;
 
 MeshMaterialList {
  1;
  246;
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
    "data\\TEXTURE\\player\\Ping-Swimwear_UV.jpg";
   }
  }
 }
 MeshNormals {
  279;
  0.445687;-0.856416;0.260605;,
  0.274154;-0.847359;0.454777;,
  0.000000;-0.847831;0.530267;,
  -0.274155;-0.847359;0.454777;,
  -0.445691;-0.856414;0.260603;,
  -0.541727;-0.839943;-0.032068;,
  -0.487601;-0.814027;-0.315603;,
  -0.287744;-0.807215;-0.515371;,
  0.000000;-0.796538;-0.604588;,
  0.287743;-0.807215;-0.515372;,
  0.487600;-0.814027;-0.315605;,
  0.541724;-0.839945;-0.032069;,
  0.688908;-0.602816;0.402515;,
  0.402826;-0.590726;0.699124;,
  0.000000;-0.585979;0.810326;,
  -0.402826;-0.590725;0.699124;,
  -0.688912;-0.602813;0.402513;,
  -0.803301;-0.590958;-0.074002;,
  -0.667328;-0.540826;-0.512036;,
  -0.402154;-0.510044;-0.760347;,
  0.000000;-0.507103;-0.861886;,
  0.402154;-0.510045;-0.760347;,
  0.667327;-0.540826;-0.512037;,
  0.803298;-0.590962;-0.074002;,
  0.834141;-0.259012;0.486952;,
  0.479332;-0.259774;0.838307;,
  0.000000;-0.263838;0.964567;,
  -0.479332;-0.259774;0.838307;,
  -0.834142;-0.259011;0.486950;,
  -0.957236;-0.280660;-0.070204;,
  -0.760821;-0.233362;-0.605552;,
  -0.483402;-0.239788;-0.841917;,
  -0.000000;-0.327301;-0.944920;,
  0.483402;-0.239789;-0.841917;,
  0.760820;-0.233363;-0.605553;,
  0.957236;-0.280661;-0.070204;,
  0.866948;-0.003544;0.498386;,
  0.504406;-0.034098;0.862793;,
  0.000000;-0.050378;0.998730;,
  -0.504406;-0.034098;0.862793;,
  -0.866949;-0.003544;0.498384;,
  -0.998633;-0.002768;-0.052205;,
  -0.812898;-0.001912;-0.582403;,
  -0.540049;0.104772;-0.835087;,
  0.139629;0.023781;-0.989918;,
  0.511254;0.019065;-0.859218;,
  0.812897;-0.001912;-0.582404;,
  0.998633;-0.002768;-0.052204;,
  0.846018;0.240393;0.475882;,
  0.508835;0.219301;0.832463;,
  0.000000;0.200858;0.979620;,
  -0.508834;0.219301;0.832463;,
  -0.846015;0.240410;0.475879;,
  -0.973740;0.223166;-0.045026;,
  -0.814453;0.170778;-0.554528;,
  -0.520854;0.127602;-0.844055;,
  -0.538834;0.328617;-0.775673;,
  0.520854;0.127603;-0.844054;,
  0.814452;0.170779;-0.554529;,
  0.973744;0.223150;-0.045024;,
  0.787210;0.422912;0.448827;,
  0.486804;0.424682;0.763326;,
  0.000000;0.431175;0.902268;,
  -0.486803;0.424682;0.763327;,
  -0.787204;0.422929;0.448822;,
  -0.910282;0.411133;-0.048551;,
  -0.750050;0.372157;-0.546739;,
  -0.506869;0.338290;-0.792870;,
  -0.000000;0.369937;-0.929057;,
  0.506870;0.338290;-0.792870;,
  0.750050;0.372155;-0.546741;,
  0.910289;0.411116;-0.048549;,
  0.687437;0.611680;0.391506;,
  0.454678;0.575780;0.679519;,
  0.000000;0.582209;0.813039;,
  -0.454677;0.575780;0.679518;,
  -0.687438;0.611681;0.391503;,
  -0.786498;0.615484;-0.050988;,
  -0.650957;0.577252;-0.492985;,
  -0.477289;0.541474;-0.692099;,
  0.000000;0.639042;-0.769172;,
  0.477289;0.541473;-0.692100;,
  0.650957;0.577251;-0.492987;,
  0.786498;0.615484;-0.050987;,
  0.547273;0.786794;0.285390;,
  0.361356;0.792684;0.490993;,
  0.000000;0.812300;0.583239;,
  -0.361356;0.792684;0.490993;,
  -0.547274;0.786794;0.285387;,
  -0.639639;0.768325;-0.023187;,
  -0.557911;0.746593;-0.362400;,
  -0.373700;0.706608;-0.600877;,
  0.000000;0.773573;-0.633708;,
  0.373699;0.706609;-0.600877;,
  0.557909;0.746593;-0.362403;,
  0.639638;0.768326;-0.023187;,
  0.428808;0.875080;0.224408;,
  0.268066;0.895496;0.355285;,
  0.000000;0.917718;0.397233;,
  -0.268067;0.895496;0.355285;,
  -0.428811;0.875079;0.224405;,
  -0.505578;0.862781;0.000038;,
  -0.436790;0.863302;-0.252831;,
  -0.294634;0.857207;-0.422358;,
  0.000000;0.876628;-0.481169;,
  0.294634;0.857207;-0.422358;,
  0.436788;0.863302;-0.252834;,
  0.505575;0.862782;0.000038;,
  -0.000000;1.000000;0.000089;,
  0.000000;0.969532;-0.244965;,
  0.157356;0.962452;-0.221191;,
  0.231518;0.963595;-0.133727;,
  0.268469;0.963288;0.000019;,
  0.231569;0.963575;0.133787;,
  0.156588;0.962439;0.221790;,
  0.000000;0.969264;0.246023;,
  -0.156588;0.962439;0.221790;,
  -0.231570;0.963575;0.133785;,
  -0.268471;0.963288;0.000019;,
  -0.231519;0.963596;-0.133725;,
  -0.157355;0.962452;-0.221190;,
  0.283936;-0.944351;0.166076;,
  0.177606;-0.943106;0.281081;,
  0.000000;-0.947822;0.318801;,
  -0.177605;-0.943107;0.281080;,
  -0.283936;-0.944352;0.166073;,
  -0.328857;-0.944380;-0.000024;,
  -0.287335;-0.942022;-0.173298;,
  -0.173362;-0.944664;-0.278487;,
  0.000000;-0.954387;-0.298573;,
  0.173363;-0.944664;-0.278488;,
  0.287335;-0.942022;-0.173301;,
  0.328857;-0.944380;-0.000024;,
  0.118766;-0.990489;0.069474;,
  0.074385;-0.990258;0.117708;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.991118;0.132987;,
  -0.074385;-0.990259;0.117707;,
  -0.118766;-0.990489;0.069473;,
  -0.137570;-0.990492;-0.000011;,
  -0.118985;-0.990451;-0.069632;,
  -0.075068;-0.990191;-0.117843;,
  0.000000;-0.991120;-0.132973;,
  0.075068;-0.990191;-0.117843;,
  0.118985;-0.990451;-0.069633;,
  0.137570;-0.990492;-0.000011;,
  -0.386101;0.548800;-0.741448;,
  -0.417676;0.338353;-0.843246;,
  0.386100;0.548800;-0.741448;,
  0.417676;0.338353;-0.843246;,
  -0.475016;0.274629;-0.836025;,
  0.470380;0.157790;-0.868242;,
  -0.582908;-0.754445;-0.301712;,
  -0.475836;-0.826226;-0.301547;,
  -0.190219;-0.945793;-0.263234;,
  0.475836;-0.826226;-0.301547;,
  0.582908;-0.754445;-0.301712;,
  -0.825273;-0.471948;-0.310145;,
  -0.555260;-0.747689;-0.364208;,
  -0.529629;-0.772958;-0.349326;,
  0.555260;-0.747689;-0.364208;,
  0.825273;-0.471948;-0.310145;,
  -0.904153;0.295344;-0.308674;,
  -0.763997;0.498078;-0.410154;,
  -0.664337;0.598760;-0.447374;,
  0.763997;0.498078;-0.410154;,
  0.904153;0.295344;-0.308674;,
  -0.766096;0.549944;-0.332655;,
  -0.626463;0.702012;-0.338708;,
  -0.591685;0.736206;-0.328497;,
  0.626463;0.702012;-0.338708;,
  0.766096;0.549944;-0.332655;,
  -0.764601;0.576227;-0.288700;,
  -0.550378;0.799228;-0.241493;,
  -0.571982;0.791633;-0.214834;,
  0.550377;0.799228;-0.241493;,
  0.764601;0.576227;-0.288699;,
  0.286382;0.137184;0.948244;,
  0.284823;0.062450;0.956544;,
  0.001939;-0.014099;0.999899;,
  -0.284269;0.056816;0.957060;,
  -0.289070;0.130578;0.948361;,
  0.466014;0.083134;0.880863;,
  0.374376;0.050766;0.925886;,
  0.003857;0.018335;0.999824;,
  -0.374343;0.052099;0.925826;,
  -0.472458;0.086821;0.877067;,
  0.485399;-0.074367;0.871124;,
  0.395069;-0.016441;0.918504;,
  0.003861;0.047297;0.998873;,
  -0.396433;-0.002578;0.918060;,
  -0.495021;-0.053772;0.867216;,
  0.108451;-0.857398;-0.503098;,
  0.226551;-0.880689;-0.416008;,
  0.339649;-0.884539;-0.319734;,
  -0.398742;-0.550935;-0.733127;,
  -0.289459;-0.647557;-0.704899;,
  0.596665;0.523434;-0.608283;,
  0.604865;0.547073;-0.578662;,
  0.590481;0.581650;-0.559478;,
  0.575047;0.562799;-0.593783;,
  0.580858;0.531770;-0.616298;,
  0.289459;0.647562;0.704895;,
  0.398743;0.550937;0.733125;,
  0.499223;0.442151;0.745171;,
  -0.226548;0.880691;0.416005;,
  -0.108444;0.857401;0.503095;,
  0.590952;0.460552;-0.662320;,
  0.597966;0.456756;-0.658642;,
  0.583049;0.464757;-0.666374;,
  0.567930;0.472597;-0.673875;,
  0.575091;0.468917;-0.670364;,
  -0.590953;-0.460550;0.662320;,
  -0.597968;-0.456755;0.658642;,
  -0.583049;-0.464756;0.666374;,
  -0.567929;-0.472596;0.673876;,
  -0.575089;-0.468917;0.670365;,
  0.591398;0.096948;-0.800530;,
  0.368189;0.865655;0.339233;,
  -0.067828;0.989978;0.123865;,
  -0.591449;-0.096939;0.800494;,
  -0.591398;0.096946;-0.800531;,
  -0.368188;0.865655;0.339234;,
  0.067828;0.989978;0.123864;,
  0.591449;-0.096935;0.800495;,
  0.567849;0.156125;-0.808191;,
  0.529629;-0.772958;-0.349326;,
  -0.913651;0.267219;-0.306325;,
  -0.830379;0.401639;-0.386209;,
  -0.721045;0.523453;-0.453973;,
  0.721045;0.523453;-0.453973;,
  0.664337;0.598760;-0.447374;,
  0.830379;0.401638;-0.386210;,
  0.913651;0.267219;-0.306325;,
  0.591684;0.736206;-0.328497;,
  0.571982;0.791633;-0.214834;,
  0.126832;0.030525;0.991454;,
  0.216167;0.006739;0.976333;,
  0.000000;-0.017952;0.999839;,
  -0.216167;0.006739;0.976333;,
  -0.126832;0.030525;0.991454;,
  -0.582800;-0.769190;-0.262091;,
  -0.388268;-0.891427;-0.233679;,
  -0.000000;-0.979398;-0.201941;,
  0.388268;-0.891427;-0.233679;,
  0.582800;-0.769190;-0.262091;,
  0.915988;-0.309121;-0.255753;,
  0.947261;0.201116;-0.249496;,
  -0.915988;-0.309121;-0.255753;,
  -0.947261;0.201116;-0.249496;,
  -0.339650;0.884539;0.319732;,
  -0.565347;-0.402827;0.719800;,
  -0.552518;-0.375495;0.744129;,
  -0.560956;-0.336176;0.756514;,
  -0.582337;-0.359768;0.729006;,
  -0.581158;-0.394488;0.711783;,
  -0.499222;-0.442151;-0.745171;,
  0.807050;-0.425608;0.409302;,
  0.807049;-0.425609;0.409304;,
  -0.807052;0.425607;-0.409300;,
  -0.807052;0.425606;-0.409300;,
  0.581157;0.394489;-0.711783;,
  0.582337;0.359769;-0.729006;,
  0.560956;0.336177;-0.756514;,
  0.552520;0.375494;-0.744128;,
  0.565352;0.402824;-0.719799;,
  0.807052;-0.425607;0.409300;,
  -0.604866;-0.547073;0.578661;,
  -0.596667;-0.523432;0.608282;,
  -0.590481;-0.581650;0.559478;,
  -0.575048;-0.562797;0.593784;,
  -0.580859;-0.531768;0.616300;,
  -0.807052;0.425607;-0.409299;,
  -0.466421;-0.798108;-0.381413;,
  -0.486755;0.864876;-0.122712;,
  0.431605;-0.897801;0.087581;,
  0.466422;-0.798108;-0.381412;,
  0.486755;0.864876;-0.122712;,
  -0.431605;-0.897801;0.087580;;
  246;
  4;0,12,13,1;,
  4;1,13,14,2;,
  4;2,14,15,3;,
  4;3,15,16,4;,
  4;4,16,17,5;,
  4;5,17,18,6;,
  4;6,18,19,7;,
  4;7,19,20,8;,
  4;8,20,21,9;,
  4;9,21,22,10;,
  4;10,22,23,11;,
  4;11,23,12,0;,
  4;12,24,25,13;,
  4;13,25,26,14;,
  4;14,26,27,15;,
  4;15,27,28,16;,
  4;16,28,29,17;,
  4;17,29,30,18;,
  4;18,30,31,19;,
  4;19,31,32,20;,
  4;20,32,33,21;,
  4;21,33,34,22;,
  4;22,34,35,23;,
  4;23,35,24,12;,
  4;24,36,37,25;,
  4;25,37,38,26;,
  4;26,38,39,27;,
  4;27,39,40,28;,
  4;28,40,41,29;,
  4;29,41,42,30;,
  4;30,42,43,31;,
  4;31,43,44,32;,
  4;32,44,45,33;,
  4;33,45,46,34;,
  4;34,46,47,35;,
  4;35,47,36,24;,
  4;36,48,49,37;,
  4;37,49,50,38;,
  4;38,50,51,39;,
  4;39,51,52,40;,
  4;40,52,53,41;,
  4;41,53,54,42;,
  4;42,54,55,43;,
  3;44,43,56;,
  3;56,43,150;,
  3;150,43,55;,
  3;57,45,151;,
  3;45,44,151;,
  3;151,44,225;,
  4;45,57,58,46;,
  4;46,58,59,47;,
  4;47,59,48,36;,
  4;48,60,61,49;,
  4;49,61,62,50;,
  4;50,62,63,51;,
  4;51,63,64,52;,
  4;52,64,65,53;,
  4;53,65,66,54;,
  4;54,66,67,55;,
  4;147,68,56,150;,
  4;149,69,57,151;,
  4;57,69,70,58;,
  4;58,70,71,59;,
  4;59,71,60,48;,
  4;60,72,73,61;,
  4;61,73,74,62;,
  4;62,74,75,63;,
  4;63,75,76,64;,
  4;64,76,77,65;,
  4;65,77,78,66;,
  4;66,78,79,67;,
  4;146,80,68,147;,
  4;148,81,69,149;,
  4;69,81,82,70;,
  4;70,82,83,71;,
  4;71,83,72,60;,
  4;72,84,85,73;,
  4;73,85,86,74;,
  4;74,86,87,75;,
  4;75,87,88,76;,
  4;76,88,89,77;,
  4;77,89,90,78;,
  4;78,90,91,79;,
  3;79,91,146;,
  3;146,91,80;,
  3;80,91,92;,
  3;92,93,80;,
  3;80,93,148;,
  3;148,93,81;,
  4;81,93,94,82;,
  4;82,94,95,83;,
  4;83,95,84,72;,
  4;84,96,97,85;,
  4;85,97,98,86;,
  4;86,98,99,87;,
  4;87,99,100,88;,
  4;88,100,101,89;,
  4;89,101,102,90;,
  4;90,102,103,91;,
  4;91,103,104,92;,
  4;92,104,105,93;,
  4;93,105,106,94;,
  4;94,106,107,95;,
  4;95,107,96,84;,
  3;134,135,133;,
  3;136,135,134;,
  3;137,135,136;,
  3;138,135,137;,
  3;139,135,138;,
  3;140,135,139;,
  3;141,135,140;,
  3;142,135,141;,
  3;143,135,142;,
  3;144,135,143;,
  3;145,135,144;,
  3;133,135,145;,
  3;113,108,114;,
  3;114,108,115;,
  3;115,108,116;,
  3;116,108,117;,
  3;117,108,118;,
  3;118,108,119;,
  3;119,108,120;,
  3;120,108,109;,
  3;109,108,110;,
  3;110,108,111;,
  3;111,108,112;,
  3;112,108,113;,
  4;104,109,110,105;,
  4;105,110,111,106;,
  4;106,111,112,107;,
  4;107,112,113,96;,
  4;96,113,114,97;,
  4;97,114,115,98;,
  4;98,115,116,99;,
  4;99,116,117,100;,
  4;100,117,118,101;,
  4;101,118,119,102;,
  4;102,119,120,103;,
  4;103,120,109,104;,
  4;1,122,121,0;,
  4;2,123,122,1;,
  4;3,124,123,2;,
  4;4,125,124,3;,
  4;5,126,125,4;,
  4;6,127,126,5;,
  4;7,128,127,6;,
  4;8,129,128,7;,
  4;9,130,129,8;,
  4;10,131,130,9;,
  4;11,132,131,10;,
  4;0,121,132,11;,
  4;122,134,133,121;,
  4;123,136,134,122;,
  4;124,137,136,123;,
  4;125,138,137,124;,
  4;126,139,138,125;,
  4;127,140,139,126;,
  4;128,141,140,127;,
  4;129,142,141,128;,
  4;130,143,142,129;,
  4;131,144,143,130;,
  4;132,145,144,131;,
  4;121,133,145,132;,
  4;67,79,146,147;,
  4;68,80,148,149;,
  4;55,67,147,150;,
  4;225,68,149,151;,
  4;152,157,158,153;,
  4;153,158,159,154;,
  4;226,226,160,155;,
  4;155,160,161,156;,
  4;227,162,163,228;,
  4;228,163,164,229;,
  4;230,231,165,232;,
  4;232,165,166,233;,
  4;162,167,168,163;,
  4;163,168,169,164;,
  4;231,234,170,165;,
  4;165,170,171,166;,
  4;167,172,173,168;,
  4;168,173,174,169;,
  4;234,235,175,170;,
  4;170,175,176,171;,
  4;236,177,178,237;,
  4;237,178,179,238;,
  4;238,179,180,239;,
  4;239,180,181,240;,
  4;177,182,183,178;,
  4;178,183,184,179;,
  4;179,184,185,180;,
  4;180,185,186,181;,
  4;182,187,188,183;,
  4;183,188,189,184;,
  4;184,189,190,185;,
  4;185,190,191,186;,
  4;241,152,153,242;,
  4;242,153,154,243;,
  4;243,154,155,244;,
  4;244,155,156,245;,
  4;156,161,246,245;,
  4;161,166,247,246;,
  4;166,171,176,247;,
  4;241,248,157,152;,
  4;248,249,162,157;,
  4;249,172,167,162;,
  4;207,197,198,208;,
  4;208,198,199,209;,
  4;209,199,200,210;,
  4;210,200,201,211;,
  4;202,202,203,203;,
  4;203,203,204,204;,
  4;250,250,205,205;,
  4;205,205,206,206;,
  4;251,216,215,252;,
  4;252,215,214,253;,
  4;253,214,213,254;,
  4;254,213,212,255;,
  4;192,192,193,193;,
  4;193,193,194,194;,
  4;256,256,195,195;,
  4;195,195,196,196;,
  4;257,258,258,257;,
  4;259,260,260,259;,
  4;261,207,208,262;,
  4;262,208,209,263;,
  4;263,209,210,264;,
  4;264,210,211,265;,
  4;266,257,257,266;,
  4;213,267,268,212;,
  4;214,269,267,213;,
  4;215,270,269,214;,
  4;216,271,270,215;,
  4;272,259,259,272;,
  4;217,217,217,217;,
  4;218,219,219,218;,
  4;220,220,220,220;,
  4;273,273,273,273;,
  4;274,274,219,219;,
  4;275,275,275,275;,
  4;221,221,221,221;,
  4;222,222,223,223;,
  4;224,224,224,224;,
  4;276,276,276,276;,
  4;277,223,223,277;,
  4;278,278,278,278;;
 }
 MeshTextureCoords {
  296;
  0.783674;0.274093;,
  0.804549;0.251773;,
  0.767510;0.251773;,
  0.755458;0.274093;,
  0.716914;0.251773;,
  0.716914;0.274093;,
  0.666319;0.251773;,
  0.678370;0.274093;,
  0.629280;0.251773;,
  0.650155;0.274093;,
  0.616224;0.251773;,
  0.640205;0.274093;,
  0.629280;0.251773;,
  0.650155;0.274093;,
  0.666319;0.251773;,
  0.678370;0.274093;,
  0.234568;0.600409;,
  0.222321;0.575946;,
  0.273736;0.575946;,
  0.273736;0.600409;,
  0.325151;0.575946;,
  0.312904;0.600409;,
  0.755458;0.274093;,
  0.767510;0.251773;,
  0.804549;0.251773;,
  0.783674;0.274093;,
  0.817605;0.251773;,
  0.793624;0.274093;,
  0.813652;0.225070;,
  0.772766;0.225070;,
  0.716914;0.225070;,
  0.661063;0.225070;,
  0.620177;0.225070;,
  0.605765;0.225070;,
  0.620177;0.225070;,
  0.184682;0.575946;,
  0.175431;0.546679;,
  0.216980;0.546679;,
  0.273736;0.546679;,
  0.330492;0.546679;,
  0.372040;0.546679;,
  0.362790;0.575946;,
  0.813652;0.225070;,
  0.828065;0.225070;,
  0.816293;0.194596;,
  0.774291;0.194596;,
  0.716914;0.194596;,
  0.659538;0.194596;,
  0.617536;0.194596;,
  0.602741;0.194596;,
  0.617536;0.194596;,
  0.172748;0.513280;,
  0.215430;0.513280;,
  0.273736;0.513280;,
  0.332041;0.513280;,
  0.374724;0.513280;,
  0.816293;0.194596;,
  0.831088;0.194596;,
  0.812959;0.162469;,
  0.774005;0.161079;,
  0.716914;0.161079;,
  0.659824;0.161079;,
  0.620870;0.162469;,
  0.606598;0.162469;,
  0.620870;0.162469;,
  0.176136;0.478068;,
  0.217386;0.478068;,
  0.273736;0.478068;,
  0.245561;0.478068;,
  0.330085;0.478068;,
  0.301911;0.478068;,
  0.371336;0.478068;,
  0.812959;0.162469;,
  0.827232;0.162469;,
  0.804906;0.134163;,
  0.767716;0.134163;,
  0.716914;0.134163;,
  0.666113;0.134163;,
  0.628923;0.134163;,
  0.615891;0.134163;,
  0.628923;0.134163;,
  0.184319;0.447045;,
  0.222111;0.447045;,
  0.714495;0.184345;,
  0.739162;0.184345;,
  0.739162;0.235328;,
  0.721859;0.254066;,
  0.299548;0.447045;,
  0.325360;0.447045;,
  0.363152;0.447045;,
  0.804906;0.134163;,
  0.817939;0.134163;,
  0.793942;0.107745;,
  0.761386;0.107745;,
  0.716914;0.107745;,
  0.672443;0.107745;,
  0.639887;0.107745;,
  0.628540;0.107745;,
  0.639887;0.107745;,
  0.195461;0.418090;,
  0.228544;0.418090;,
  0.717568;0.136763;,
  0.739162;0.136763;,
  0.296332;0.418090;,
  0.318928;0.418090;,
  0.352011;0.418090;,
  0.793942;0.107745;,
  0.805290;0.107745;,
  0.777672;0.089810;,
  0.751993;0.084007;,
  0.716914;0.084007;,
  0.681836;0.084007;,
  0.656157;0.089810;,
  0.647290;0.089810;,
  0.656157;0.089810;,
  0.664359;0.136763;,
  0.680159;0.104460;,
  0.705097;0.104460;,
  0.695975;0.136763;,
  0.739162;0.104460;,
  0.773228;0.104460;,
  0.760756;0.136763;,
  0.782350;0.136763;,
  0.798165;0.104460;,
  0.813965;0.136763;,
  0.777672;0.089810;,
  0.786539;0.089810;,
  0.760729;0.073827;,
  0.742211;0.073827;,
  0.716914;0.073827;,
  0.691618;0.073827;,
  0.673099;0.073827;,
  0.666841;0.073827;,
  0.673099;0.073827;,
  0.691618;0.073827;,
  0.681836;0.089810;,
  0.716914;0.073827;,
  0.716914;0.089810;,
  0.742211;0.073827;,
  0.751993;0.089810;,
  0.760729;0.073827;,
  0.766988;0.073827;,
  0.290032;0.622161;,
  0.258525;0.622161;,
  0.313097;0.622161;,
  0.258525;0.622161;,
  0.227018;0.622161;,
  0.203953;0.622161;,
  0.195820;0.622161;,
  0.203953;0.622161;,
  0.227018;0.622161;,
  0.258525;0.622161;,
  0.290032;0.622161;,
  0.313097;0.622161;,
  0.321230;0.622161;,
  0.738822;0.060896;,
  0.716914;0.057674;,
  0.729563;0.060896;,
  0.716914;0.060896;,
  0.704266;0.060896;,
  0.695007;0.060896;,
  0.691878;0.060896;,
  0.695007;0.060896;,
  0.704266;0.060896;,
  0.716914;0.060896;,
  0.729563;0.060896;,
  0.738822;0.060896;,
  0.741951;0.060896;,
  0.311601;0.426322;,
  0.302361;0.528832;,
  0.334451;0.528832;,
  0.350455;0.426322;,
  0.258525;0.426322;,
  0.258525;0.528832;,
  0.205449;0.426322;,
  0.214689;0.528832;,
  0.166595;0.426322;,
  0.182599;0.528832;,
  0.152894;0.426322;,
  0.171283;0.528832;,
  0.166595;0.426322;,
  0.182599;0.528832;,
  0.222299;0.410899;,
  0.214689;0.528832;,
  0.258525;0.446757;,
  0.258525;0.528832;,
  0.294750;0.410899;,
  0.302361;0.528832;,
  0.350455;0.426322;,
  0.334451;0.528832;,
  0.364156;0.426322;,
  0.345767;0.528832;,
  0.251140;0.418090;,
  0.247924;0.447045;,
  0.763830;0.184345;,
  0.756465;0.254066;,
  0.602626;0.873268;,
  0.574213;0.834396;,
  0.612371;0.795354;,
  0.632703;0.896963;,
  0.662779;0.798795;,
  0.662779;0.915055;,
  0.713188;0.795354;,
  0.692856;0.896963;,
  0.751346;0.834396;,
  0.722933;0.873268;,
  0.582534;0.773320;,
  0.622656;0.730654;,
  0.662779;0.725409;,
  0.702902;0.730654;,
  0.743025;0.773320;,
  0.642183;0.201566;,
  0.671572;0.110876;,
  0.715564;0.083627;,
  0.700870;0.165202;,
  0.759556;0.054879;,
  0.759556;0.160732;,
  0.803548;0.083627;,
  0.818242;0.165202;,
  0.847540;0.110876;,
  0.876929;0.201566;,
  0.660618;0.101093;,
  0.710087;0.070575;,
  0.759556;0.039156;,
  0.809025;0.070575;,
  0.858494;0.101093;,
  0.627572;0.203196;,
  0.693564;0.203196;,
  0.759556;0.203196;,
  0.825548;0.203196;,
  0.891540;0.203196;,
  0.613887;0.261731;,
  0.686721;0.261731;,
  0.759556;0.261731;,
  0.832391;0.261731;,
  0.905225;0.261731;,
  0.660618;0.298985;,
  0.710087;0.321694;,
  0.759556;0.330494;,
  0.809025;0.321694;,
  0.858494;0.298985;,
  0.595137;0.887623;,
  0.628958;0.914268;,
  0.662779;0.924593;,
  0.696600;0.914268;,
  0.730422;0.887623;,
  0.762371;0.843913;,
  0.753015;0.775233;,
  0.563188;0.843913;,
  0.572544;0.775233;,
  0.812130;0.455288;,
  0.815386;0.409600;,
  0.843635;0.430185;,
  0.841235;0.469349;,
  0.887758;0.478562;,
  0.888699;0.492278;,
  0.938563;0.476044;,
  0.936163;0.515208;,
  0.968525;0.483580;,
  0.965268;0.529269;,
  0.800304;0.420613;,
  0.828553;0.441198;,
  0.872676;0.489575;,
  0.923481;0.487058;,
  0.953443;0.494593;,
  0.790982;0.470731;,
  0.820087;0.484791;,
  0.867551;0.507721;,
  0.915015;0.530651;,
  0.944120;0.544711;,
  0.787725;0.516419;,
  0.802807;0.505406;,
  0.832769;0.512942;,
  0.817687;0.523955;,
  0.883574;0.510425;,
  0.868492;0.521438;,
  0.927697;0.558801;,
  0.912615;0.569814;,
  0.955946;0.579386;,
  0.940863;0.590400;,
  0.674851;0.265944;,
  0.699001;0.164998;,
  0.682222;0.074505;,
  0.659959;0.164390;,
  0.685115;0.085167;,
  0.663208;0.176894;,
  0.702250;0.177501;,
  0.677744;0.276605;,
  0.674851;0.265944;,
  0.659959;0.164390;,
  0.682222;0.074505;,
  0.699001;0.164998;,
  0.663208;0.176894;,
  0.685115;0.085167;,
  0.677744;0.276605;,
  0.702250;0.177501;;
 }
}
