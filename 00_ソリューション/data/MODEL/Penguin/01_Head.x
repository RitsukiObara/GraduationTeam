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
 266;
 16.04814;22.73374;-24.36167;,
 12.31917;25.75421;-26.02414;,
 16.08677;28.20456;-23.67059;,
 20.13211;25.12067;-20.09086;,
 15.46727;28.03208;-22.45717;,
 19.43616;24.89781;-18.87139;,
 11.62325;25.53139;-24.80463;,
 15.42861;22.56127;-23.14824;,
 -16.04816;22.73374;-24.36167;,
 -20.13215;25.12067;-20.09086;,
 -16.08680;28.20456;-23.67059;,
 -12.31920;25.75421;-26.02414;,
 -19.43619;24.89781;-18.87139;,
 -15.46730;28.03208;-22.45717;,
 -15.42862;22.56127;-23.14822;,
 -11.62328;25.53139;-24.80463;,
 -5.66174;15.12535;-29.05714;,
 -8.33598;17.17512;-27.92197;,
 -4.74453;19.23387;-38.29127;,
 -2.83088;13.87586;-30.15936;,
 -0.00002;19.05246;-47.65176;,
 -0.00002;12.92178;-31.51780;,
 4.74450;19.23387;-38.29127;,
 2.83085;13.87586;-30.15936;,
 8.33595;17.17512;-27.92197;,
 5.66171;15.12535;-29.05714;,
 -7.55285;20.39579;-28.90154;,
 -3.77643;22.64567;-36.68181;,
 -0.00002;22.92224;-41.57411;,
 3.77640;22.64567;-36.68181;,
 7.55282;20.39579;-28.90154;,
 -7.55285;20.39579;-28.90154;,
 -5.66174;26.00695;-29.16458;,
 -2.83088;27.69285;-30.31336;,
 -3.77643;22.64567;-36.68181;,
 -0.00002;29.47152;-31.43352;,
 -0.00002;22.92224;-41.57411;,
 2.83085;27.69285;-30.31336;,
 3.77640;22.64567;-36.68181;,
 5.66171;26.00695;-29.16458;,
 7.55282;20.39579;-28.90154;,
 -6.36658;26.61219;-26.47285;,
 -3.18330;28.50037;-26.54955;,
 -0.00002;30.44435;-27.71139;,
 3.18326;28.50037;-26.54955;,
 6.36654;26.61219;-26.47285;,
 -8.49310;20.29491;-25.56162;,
 -4.24656;20.29491;-26.54955;,
 -0.00002;20.29491;-27.71139;,
 4.24653;20.29491;-26.54955;,
 8.49307;20.29491;-25.56162;,
 -9.37373;16.67327;-22.27968;,
 -4.68686;16.67327;-26.47285;,
 -0.00002;16.67327;-27.71139;,
 4.68683;16.67327;-26.54955;,
 9.37370;16.67327;-22.27968;,
 -6.36658;14.36837;-25.73366;,
 -3.18330;12.96330;-26.54955;,
 -0.00002;12.41885;-27.71139;,
 3.18326;12.96330;-26.54955;,
 6.36654;14.36837;-25.73366;,
 -6.36658;14.36837;-25.73366;,
 -3.18330;12.96330;-26.54955;,
 -0.00002;12.41885;-27.71139;,
 3.18326;12.96330;-26.54955;,
 6.36654;14.36837;-25.73366;,
 9.37370;16.67327;-22.27968;,
 8.49307;20.29491;-25.56162;,
 -9.37373;16.67327;-22.27968;,
 -8.49310;20.29491;-25.56162;,
 17.47965;1.45347;9.85315;,
 22.94519;6.64957;12.91794;,
 13.24739;6.64957;22.37669;,
 10.09184;1.45347;17.05885;,
 -0.00002;6.64957;25.81330;,
 -0.00002;1.45347;20.76336;,
 -13.24744;6.64957;22.37669;,
 -10.09194;1.45347;17.05885;,
 -22.94523;6.64957;12.91794;,
 -17.47970;1.45347;9.85315;,
 -26.36360;6.64957;-0.00142;,
 -20.08474;1.45347;0.01224;,
 -18.41914;9.52642;-18.45283;,
 -17.47970;1.45347;-9.83315;,
 -12.20461;5.24557;-20.04363;,
 -6.88792;1.67216;-19.76872;,
 -6.88792;1.67216;-19.76872;,
 -12.20461;5.24557;-20.04363;,
 -0.00002;6.37970;-24.65312;,
 -0.00002;1.16373;-20.26122;,
 12.20456;5.24557;-20.04363;,
 6.88782;1.67216;-19.76872;,
 6.88782;1.67216;-19.76872;,
 12.20456;5.24557;-20.04363;,
 18.41909;9.52642;-18.45283;,
 17.47965;1.45347;-9.83315;,
 26.36362;6.64957;-0.00142;,
 20.08478;1.45347;0.01224;,
 25.32874;12.86608;14.22253;,
 14.62355;12.86608;24.66384;,
 -0.00002;12.86608;28.67149;,
 -14.62358;12.86608;24.66384;,
 -25.32877;12.86608;14.22253;,
 -29.10224;12.86608;-0.03914;,
 -22.68731;14.98405;-16.83678;,
 -18.41914;9.52642;-18.45283;,
 -22.68731;14.98405;-16.83678;,
 -14.62358;13.92483;-22.95757;,
 -0.00002;13.43882;-28.53000;,
 14.62355;13.92483;-22.95757;,
 22.68728;14.98405;-16.83678;,
 18.41909;9.52642;-18.45283;,
 22.68728;14.98405;-16.83678;,
 29.10226;12.86608;-0.03914;,
 26.02010;19.96043;14.61184;,
 15.02270;19.96043;25.33816;,
 -0.00002;19.96043;29.52544;,
 -15.02274;19.96043;25.33816;,
 -26.02015;19.96043;14.61184;,
 -29.89381;19.96043;-0.03914;,
 -23.92506;19.96043;-15.67026;,
 -23.92506;19.96043;-15.67026;,
 -15.26552;19.96043;-22.81011;,
 -0.00002;19.96043;-31.06885;,
 15.26550;19.96043;-22.81011;,
 23.92502;19.96043;-15.67026;,
 23.92502;19.96043;-15.67026;,
 29.89386;19.96043;-0.03914;,
 25.14717;27.43963;14.13717;,
 14.94789;27.76327;25.21178;,
 -0.00002;27.76327;29.45085;,
 -14.94792;27.76327;25.21178;,
 -25.14722;27.43963;14.13717;,
 -28.88414;27.43963;-0.02230;,
 -23.74093;25.82622;-15.23586;,
 -23.74093;25.82622;-15.23586;,
 -15.59328;27.43963;-22.57658;,
 -0.00002;27.65755;-29.97688;,
 -3.87864;26.63917;-27.06447;,
 15.59325;27.43963;-22.57658;,
 3.87861;26.63917;-27.06447;,
 23.74088;25.82622;-15.23586;,
 23.74088;25.82622;-15.23586;,
 28.88418;27.43963;-0.02230;,
 23.03862;34.02931;12.94982;,
 13.30134;34.02931;22.44709;,
 -0.00002;34.02931;27.59578;,
 -13.30138;34.02931;22.44709;,
 -23.03867;34.02931;12.94982;,
 -26.45095;34.02931;-0.02230;,
 -22.12568;31.45954;-15.27979;,
 -22.12568;31.45954;-15.27979;,
 -14.18746;34.02931;-21.18200;,
 -5.88941;34.02931;-25.09467;,
 -0.00002;34.02931;-27.69486;,
 -0.00002;27.65755;-29.97688;,
 -3.87864;26.63917;-27.06447;,
 5.88938;34.02931;-25.09467;,
 14.18743;34.02931;-21.18200;,
 22.12563;31.45954;-15.27979;,
 22.12563;31.45954;-15.27979;,
 26.45100;34.02931;-0.02230;,
 20.16802;40.17947;11.33331;,
 11.64402;40.17947;19.64725;,
 -0.00002;40.17947;24.33133;,
 -11.64405;40.17947;19.64725;,
 -20.16807;40.17947;11.33331;,
 -23.13904;40.17947;-0.02230;,
 -17.84202;36.88752;-16.96181;,
 -17.84202;36.88752;-16.96181;,
 -12.11293;38.29215;-19.06803;,
 -7.82125;37.46602;-22.76610;,
 -0.00002;40.17947;-23.90282;,
 7.82122;37.46602;-22.76610;,
 12.11291;38.29215;-19.06803;,
 17.84198;36.88752;-16.96181;,
 17.84198;36.88752;-16.96181;,
 23.13910;40.17947;-0.02230;,
 15.90810;44.35475;8.93445;,
 9.18453;45.70575;15.49229;,
 -0.00002;45.70575;19.42549;,
 -9.18456;45.70575;15.49229;,
 -15.90811;44.35475;8.93445;,
 -18.22978;44.35475;-0.02230;,
 -15.90811;44.35475;-8.98186;,
 -17.84202;36.88752;-16.96181;,
 -15.90811;44.35475;-8.98186;,
 -9.18456;44.35475;-15.53967;,
 -12.11293;38.29215;-19.06803;,
 -0.00002;44.35475;-19.52454;,
 9.18453;44.35475;-15.53967;,
 7.82122;37.46602;-22.76610;,
 12.11291;38.29215;-19.06803;,
 15.90810;44.35475;-8.98186;,
 17.84198;36.88752;-16.96181;,
 15.90810;44.35475;-8.98186;,
 18.22983;44.35475;-0.02230;,
 11.47203;48.07560;6.44003;,
 6.62338;48.07560;11.16918;,
 -0.00002;48.07560;14.39394;,
 -6.62346;48.07560;11.16918;,
 -11.47206;48.07560;6.44003;,
 -13.11064;48.07560;-0.01853;,
 -11.47206;48.07560;-6.48026;,
 -6.62346;48.07560;-11.20941;,
 -9.18456;44.35475;-15.53967;,
 -0.00002;48.07560;-14.49302;,
 -0.00002;44.35475;-19.52454;,
 6.62338;48.07560;-11.20941;,
 9.18453;44.35475;-15.53967;,
 11.47203;48.07560;-6.48026;,
 13.11069;48.07560;-0.01853;,
 5.99077;-1.32328;10.11386;,
 0.00000;-1.32328;-0.00000;,
 10.37637;-1.32328;5.83638;,
 0.00000;-1.32328;12.31294;,
 -5.99082;-1.32328;10.11386;,
 -10.37637;-1.32328;5.83638;,
 -11.92278;-1.32328;-0.00544;,
 -10.37637;-1.32328;-5.84990;,
 -5.99082;-1.32328;-10.12738;,
 0.00000;-1.32328;-12.37544;,
 5.99077;-1.32328;-10.12738;,
 10.37637;-1.32328;-5.84990;,
 11.92283;-1.32328;-0.00544;,
 5.73600;51.08589;3.21202;,
 -0.00002;51.83608;-0.01600;,
 3.31168;51.08589;5.57659;,
 -0.00002;51.08589;7.18898;,
 -3.31173;51.08589;5.57659;,
 -5.73605;51.08589;3.21202;,
 -6.55533;51.08589;-0.01726;,
 -5.73605;51.08589;-3.24813;,
 -3.31173;51.08589;-5.61270;,
 -0.00002;51.08589;-7.25451;,
 3.31168;51.08589;-5.61270;,
 5.73600;51.08589;-3.24813;,
 6.55534;51.08589;-0.01726;,
 10.09184;1.45347;17.05885;,
 8.33496;0.00000;14.08367;,
 14.43666;0.00000;8.13238;,
 17.47965;1.45347;9.85315;,
 -0.00002;1.45347;20.76336;,
 -0.00002;0.00000;17.14323;,
 -10.09194;1.45347;17.05885;,
 -8.33504;0.00000;14.08367;,
 -17.47970;1.45347;9.85315;,
 -14.43667;0.00000;8.13238;,
 -20.08474;1.45347;0.01224;,
 -16.58819;0.00000;0.00466;,
 -17.47970;1.45347;-9.83315;,
 -14.43667;0.00000;-8.12675;,
 -6.88792;1.67216;-19.76872;,
 -8.33504;0.00000;-14.07800;,
 -0.00002;1.16373;-20.26122;,
 -0.00002;0.00000;-17.20574;,
 6.88782;1.67216;-19.76872;,
 8.33496;0.00000;-14.07802;,
 17.47965;1.45347;-9.83315;,
 14.43666;0.00000;-8.12675;,
 20.08478;1.45347;0.01224;,
 16.58824;0.00000;0.00466;,
 -7.82125;37.46602;-22.76610;,
 -5.88941;34.02931;-25.09467;,
 5.88938;34.02931;-25.09467;,
 3.87861;26.63917;-27.06447;;
 
 218;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,1,0;,
 4;1,6,4,2;,
 4;7,0,3,5;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;12,14,15,13;,
 4;14,8,11,15;,
 4;11,10,13,15;,
 4;14,12,9,8;,
 4;16,17,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;17,26,27,18;,
 4;18,27,28,20;,
 4;20,28,29,22;,
 4;22,29,30,24;,
 4;31,32,33,34;,
 4;34,33,35,36;,
 4;36,35,37,38;,
 4;38,37,39,40;,
 4;32,41,42,33;,
 4;33,42,43,35;,
 4;35,43,44,37;,
 4;37,44,45,39;,
 4;41,46,47,42;,
 4;42,47,48,43;,
 4;43,48,49,44;,
 4;44,49,50,45;,
 4;46,51,52,47;,
 4;47,52,53,48;,
 4;48,53,54,49;,
 4;49,54,55,50;,
 4;51,56,57,52;,
 4;52,57,58,53;,
 4;53,58,59,54;,
 4;54,59,60,55;,
 4;61,16,19,62;,
 4;62,19,21,63;,
 4;63,21,23,64;,
 4;64,23,25,65;,
 4;25,24,66,65;,
 4;24,30,67,66;,
 4;40,39,45,50;,
 4;61,68,17,16;,
 4;68,69,26,17;,
 4;46,41,32,31;,
 4;70,71,72,73;,
 4;73,72,74,75;,
 4;75,74,76,77;,
 4;77,76,78,79;,
 4;79,78,80,81;,
 4;81,80,82,83;,
 4;83,82,84,85;,
 4;86,87,88,89;,
 4;89,88,90,91;,
 4;92,93,94,95;,
 4;95,94,96,97;,
 4;97,96,71,70;,
 4;71,98,99,72;,
 4;72,99,100,74;,
 4;74,100,101,76;,
 4;76,101,102,78;,
 4;78,102,103,80;,
 4;80,103,104,82;,
 4;105,106,107,87;,
 4;87,107,108,88;,
 4;88,108,109,90;,
 4;90,109,110,111;,
 4;94,112,113,96;,
 4;96,113,98,71;,
 4;98,114,115,99;,
 4;99,115,116,100;,
 4;100,116,117,101;,
 4;101,117,118,102;,
 4;102,118,119,103;,
 4;103,119,120,104;,
 4;106,121,122,107;,
 4;107,122,123,108;,
 4;108,123,124,109;,
 4;109,124,125,110;,
 4;112,126,127,113;,
 4;113,127,114,98;,
 4;114,128,129,115;,
 4;115,129,130,116;,
 4;116,130,131,117;,
 4;117,131,132,118;,
 4;118,132,133,119;,
 4;119,133,134,120;,
 4;121,135,136,122;,
 3;123,122,137;,
 3;137,122,138;,
 3;138,122,136;,
 3;139,124,140;,
 3;124,123,140;,
 3;140,123,137;,
 4;124,139,141,125;,
 4;126,142,143,127;,
 4;127,143,128,114;,
 4;128,144,145,129;,
 4;129,145,146,130;,
 4;130,146,147,131;,
 4;131,147,148,132;,
 4;132,148,149,133;,
 4;133,149,150,134;,
 4;135,151,152,136;,
 4;153,154,155,156;,
 4;157,158,139,140;,
 4;139,158,159,141;,
 4;142,160,161,143;,
 4;143,161,144,128;,
 4;144,162,163,145;,
 4;145,163,164,146;,
 4;146,164,165,147;,
 4;147,165,166,148;,
 4;148,166,167,149;,
 4;149,167,168,150;,
 4;151,169,170,152;,
 4;171,172,154,153;,
 4;173,174,158,157;,
 4;158,174,175,159;,
 4;160,176,177,161;,
 4;161,177,162,144;,
 4;162,178,179,163;,
 4;163,179,180,164;,
 4;164,180,181,165;,
 4;165,181,182,166;,
 4;166,182,183,167;,
 4;167,183,184,168;,
 4;185,186,187,188;,
 3;188,187,171;,
 3;171,187,172;,
 3;172,187,189;,
 3;189,190,172;,
 3;172,190,191;,
 3;191,190,192;,
 4;192,190,193,194;,
 4;176,195,196,177;,
 4;177,196,178,162;,
 4;178,197,198,179;,
 4;179,198,199,180;,
 4;180,199,200,181;,
 4;181,200,201,182;,
 4;182,201,202,183;,
 4;183,202,203,184;,
 4;184,203,204,205;,
 4;205,204,206,207;,
 4;207,206,208,209;,
 4;209,208,210,195;,
 4;195,210,211,196;,
 4;196,211,197,178;,
 3;212,213,214;,
 3;215,213,212;,
 3;216,213,215;,
 3;217,213,216;,
 3;218,213,217;,
 3;219,213,218;,
 3;220,213,219;,
 3;221,213,220;,
 3;222,213,221;,
 3;223,213,222;,
 3;224,213,223;,
 3;214,213,224;,
 3;225,226,227;,
 3;227,226,228;,
 3;228,226,229;,
 3;229,226,230;,
 3;230,226,231;,
 3;231,226,232;,
 3;232,226,233;,
 3;233,226,234;,
 3;234,226,235;,
 3;235,226,236;,
 3;236,226,237;,
 3;237,226,225;,
 4;206,234,235,208;,
 4;208,235,236,210;,
 4;210,236,237,211;,
 4;211,237,225,197;,
 4;197,225,227,198;,
 4;198,227,228,199;,
 4;199,228,229,200;,
 4;200,229,230,201;,
 4;201,230,231,202;,
 4;202,231,232,203;,
 4;203,232,233,204;,
 4;204,233,234,206;,
 4;238,239,240,241;,
 4;242,243,239,238;,
 4;244,245,243,242;,
 4;246,247,245,244;,
 4;248,249,247,246;,
 4;250,251,249,248;,
 4;252,253,251,250;,
 4;254,255,253,252;,
 4;256,257,255,254;,
 4;258,259,257,256;,
 4;260,261,259,258;,
 4;241,240,261,260;,
 4;239,212,214,240;,
 4;243,215,212,239;,
 4;245,216,215,243;,
 4;247,217,216,245;,
 4;249,218,217,247;,
 4;251,219,218,249;,
 4;253,220,219,251;,
 4;255,221,220,253;,
 4;257,222,221,255;,
 4;259,223,222,257;,
 4;261,224,223,259;,
 4;240,214,224,261;,
 4;152,170,262,263;,
 4;154,172,191,264;,
 4;136,152,263,138;,
 4;155,154,264,265;;
 
 MeshMaterialList {
  2;
  218;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\player\\Ping-_UV.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\player\\Ping-_UV.jpg";
   }
  }
 }
 MeshNormals {
  231;
  0.591398;0.096948;-0.800531;,
  0.368188;0.865654;0.339234;,
  -0.067826;0.989979;0.123860;,
  -0.591449;-0.096939;0.800494;,
  -0.591398;0.096946;-0.800531;,
  -0.368188;0.865654;0.339234;,
  0.067826;0.989979;0.123860;,
  0.591449;-0.096935;0.800495;,
  -0.582908;-0.754446;-0.301711;,
  -0.475837;-0.826226;-0.301547;,
  -0.190219;-0.945793;-0.263234;,
  0.475837;-0.826226;-0.301547;,
  0.582908;-0.754446;-0.301711;,
  -0.825273;-0.471949;-0.310144;,
  -0.555260;-0.747689;-0.364208;,
  -0.529629;-0.772958;-0.349327;,
  0.555260;-0.747689;-0.364208;,
  0.825273;-0.471949;-0.310144;,
  -0.904153;0.295343;-0.308674;,
  -0.763997;0.498078;-0.410155;,
  -0.664336;0.598761;-0.447375;,
  0.763997;0.498078;-0.410155;,
  0.904153;0.295343;-0.308674;,
  -0.766096;0.549944;-0.332653;,
  -0.626462;0.702013;-0.338707;,
  -0.591682;0.736208;-0.328497;,
  0.626462;0.702013;-0.338707;,
  0.766096;0.549944;-0.332653;,
  -0.764602;0.576227;-0.288698;,
  -0.550377;0.799229;-0.241491;,
  -0.571980;0.791635;-0.214833;,
  0.550377;0.799229;-0.241491;,
  0.764602;0.576227;-0.288698;,
  0.286382;0.137184;0.948244;,
  0.284823;0.062450;0.956544;,
  0.001939;-0.014099;0.999899;,
  -0.284269;0.056816;0.957060;,
  -0.289070;0.130578;0.948361;,
  0.466014;0.083134;0.880863;,
  0.374376;0.050766;0.925886;,
  0.003857;0.018335;0.999824;,
  -0.374343;0.052099;0.925826;,
  -0.472458;0.086821;0.877066;,
  0.485399;-0.074368;0.871124;,
  0.395069;-0.016441;0.918504;,
  0.003861;0.047297;0.998873;,
  -0.396433;-0.002579;0.918060;,
  -0.495021;-0.053772;0.867216;,
  0.445691;-0.856414;0.260606;,
  0.274154;-0.847359;0.454777;,
  0.000000;-0.847831;0.530267;,
  -0.274155;-0.847359;0.454777;,
  -0.445688;-0.856416;0.260602;,
  -0.541724;-0.839945;-0.032069;,
  -0.487601;-0.814027;-0.315603;,
  -0.287744;-0.807215;-0.515371;,
  0.000000;-0.796538;-0.604588;,
  0.287743;-0.807215;-0.515371;,
  0.487600;-0.814027;-0.315605;,
  0.541727;-0.839943;-0.032068;,
  0.688911;-0.602813;0.402515;,
  0.402826;-0.590726;0.699124;,
  0.000000;-0.585979;0.810326;,
  -0.402827;-0.590725;0.699124;,
  -0.688910;-0.602816;0.402513;,
  -0.803299;-0.590961;-0.074002;,
  -0.667328;-0.540826;-0.512036;,
  -0.402154;-0.510043;-0.760347;,
  0.000000;-0.507102;-0.861886;,
  0.402154;-0.510044;-0.760347;,
  0.667327;-0.540826;-0.512037;,
  0.803301;-0.590958;-0.074001;,
  0.834141;-0.259011;0.486952;,
  0.479332;-0.259773;0.838307;,
  0.000000;-0.263838;0.964567;,
  -0.479332;-0.259773;0.838307;,
  -0.834142;-0.259011;0.486950;,
  -0.957236;-0.280659;-0.070204;,
  -0.760821;-0.233362;-0.605552;,
  -0.483402;-0.239788;-0.841917;,
  -0.000000;-0.327301;-0.944920;,
  0.483402;-0.239789;-0.841917;,
  0.760820;-0.233363;-0.605553;,
  0.957236;-0.280660;-0.070204;,
  0.866948;-0.003548;0.498386;,
  0.504406;-0.034094;0.862793;,
  -0.000000;-0.050374;0.998730;,
  -0.504406;-0.034098;0.862793;,
  -0.866949;-0.003548;0.498384;,
  -0.998633;-0.002772;-0.052205;,
  -0.812898;-0.001913;-0.582403;,
  -0.540049;0.104771;-0.835087;,
  0.139629;0.023781;-0.989918;,
  0.511254;0.019065;-0.859218;,
  0.812897;-0.001912;-0.582404;,
  0.998633;-0.002772;-0.052204;,
  0.846019;0.240389;0.475883;,
  0.508834;0.219306;0.832462;,
  -0.000000;0.200863;0.979619;,
  -0.508834;0.219301;0.832463;,
  -0.846021;0.240390;0.475880;,
  -0.973745;0.223145;-0.045024;,
  -0.814453;0.170779;-0.554528;,
  -0.520854;0.127602;-0.844055;,
  -0.538834;0.328617;-0.775673;,
  0.520854;0.127603;-0.844054;,
  0.814452;0.170779;-0.554529;,
  0.973745;0.223146;-0.045024;,
  0.787210;0.422913;0.448827;,
  0.486804;0.424682;0.763326;,
  0.000000;0.431175;0.902268;,
  -0.486803;0.424682;0.763327;,
  -0.787196;0.422946;0.448820;,
  -0.910274;0.411150;-0.048554;,
  -0.750050;0.372157;-0.546739;,
  -0.506869;0.338290;-0.792870;,
  0.000000;0.369937;-0.929057;,
  0.506870;0.338290;-0.792870;,
  0.750050;0.372156;-0.546741;,
  0.910289;0.411116;-0.048549;,
  0.687484;0.611622;0.391515;,
  0.454678;0.575780;0.679518;,
  0.000000;0.582209;0.813039;,
  -0.454677;0.575780;0.679518;,
  -0.687464;0.611651;0.391505;,
  -0.786522;0.615454;-0.050977;,
  -0.650957;0.577252;-0.492985;,
  -0.477289;0.541473;-0.692099;,
  0.000000;0.639042;-0.769172;,
  0.477289;0.541473;-0.692100;,
  0.650956;0.577251;-0.492987;,
  0.786545;0.615426;-0.050973;,
  0.547331;0.786748;0.285405;,
  0.361356;0.792684;0.490993;,
  0.000000;0.812300;0.583240;,
  -0.361357;0.792683;0.490993;,
  -0.547332;0.786748;0.285403;,
  -0.639697;0.768278;-0.023173;,
  -0.557912;0.746592;-0.362401;,
  -0.373700;0.706608;-0.600877;,
  0.000000;0.773572;-0.633708;,
  0.373699;0.706609;-0.600877;,
  0.557910;0.746593;-0.362403;,
  0.639696;0.768279;-0.023173;,
  0.428808;0.875080;0.224407;,
  0.268066;0.895496;0.355285;,
  0.000000;0.917718;0.397233;,
  -0.268067;0.895496;0.355285;,
  -0.428810;0.875080;0.224404;,
  -0.505577;0.862781;0.000038;,
  -0.436790;0.863302;-0.252831;,
  -0.294634;0.857207;-0.422358;,
  0.000000;0.876628;-0.481169;,
  0.294634;0.857208;-0.422358;,
  0.436788;0.863302;-0.252834;,
  0.505576;0.862782;0.000038;,
  -0.000000;1.000000;0.000090;,
  0.000000;0.969532;-0.244965;,
  0.157356;0.962452;-0.221191;,
  0.231518;0.963595;-0.133727;,
  0.268469;0.963288;0.000019;,
  0.231569;0.963575;0.133787;,
  0.156588;0.962439;0.221791;,
  0.000000;0.969264;0.246023;,
  -0.156588;0.962439;0.221790;,
  -0.231570;0.963575;0.133785;,
  -0.268471;0.963288;0.000019;,
  -0.231520;0.963596;-0.133725;,
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
  -0.417676;0.338354;-0.843246;,
  0.386100;0.548800;-0.741448;,
  0.417676;0.338353;-0.843246;,
  -0.475016;0.274628;-0.836026;,
  0.470380;0.157790;-0.868242;,
  -0.466424;-0.798108;-0.381410;,
  -0.486751;0.864877;-0.122720;,
  0.431609;-0.897799;0.087577;,
  0.466425;-0.798107;-0.381408;,
  0.486751;0.864878;-0.122720;,
  -0.431609;-0.897800;0.087576;,
  0.529629;-0.772958;-0.349327;,
  -0.913651;0.267219;-0.306325;,
  -0.830379;0.401638;-0.386210;,
  -0.721045;0.523452;-0.453973;,
  0.721045;0.523452;-0.453973;,
  0.664336;0.598761;-0.447375;,
  0.830379;0.401638;-0.386210;,
  0.913652;0.267218;-0.306325;,
  0.591682;0.736208;-0.328497;,
  0.571980;0.791635;-0.214833;,
  0.126832;0.030525;0.991454;,
  0.216167;0.006739;0.976333;,
  0.000000;-0.017953;0.999839;,
  -0.216167;0.006739;0.976333;,
  -0.126832;0.030525;0.991454;,
  -0.582800;-0.769190;-0.262090;,
  -0.388269;-0.891426;-0.233679;,
  0.000000;-0.979398;-0.201941;,
  0.388269;-0.891426;-0.233679;,
  0.582800;-0.769190;-0.262090;,
  0.915988;-0.309122;-0.255753;,
  0.947261;0.201116;-0.249496;,
  -0.915988;-0.309123;-0.255753;,
  -0.947261;0.201116;-0.249497;,
  0.567849;0.156125;-0.808191;;
  218;
  4;0,0,0,0;,
  4;1,2,2,1;,
  4;3,3,3,3;,
  4;200,200,200,200;,
  4;201,201,2,2;,
  4;202,202,202,202;,
  4;4,4,4,4;,
  4;5,5,6,6;,
  4;7,7,7,7;,
  4;203,203,203,203;,
  4;204,6,6,204;,
  4;205,205,205,205;,
  4;8,13,14,9;,
  4;9,14,15,10;,
  4;206,206,16,11;,
  4;11,16,17,12;,
  4;207,18,19,208;,
  4;208,19,20,209;,
  4;210,211,21,212;,
  4;212,21,22,213;,
  4;18,23,24,19;,
  4;19,24,25,20;,
  4;211,214,26,21;,
  4;21,26,27,22;,
  4;23,28,29,24;,
  4;24,29,30,25;,
  4;214,215,31,26;,
  4;26,31,32,27;,
  4;216,33,34,217;,
  4;217,34,35,218;,
  4;218,35,36,219;,
  4;219,36,37,220;,
  4;33,38,39,34;,
  4;34,39,40,35;,
  4;35,40,41,36;,
  4;36,41,42,37;,
  4;38,43,44,39;,
  4;39,44,45,40;,
  4;40,45,46,41;,
  4;41,46,47,42;,
  4;221,8,9,222;,
  4;222,9,10,223;,
  4;223,10,11,224;,
  4;224,11,12,225;,
  4;12,17,226,225;,
  4;17,22,227,226;,
  4;22,27,32,227;,
  4;221,228,13,8;,
  4;228,229,18,13;,
  4;229,28,23,18;,
  4;48,60,61,49;,
  4;49,61,62,50;,
  4;50,62,63,51;,
  4;51,63,64,52;,
  4;52,64,65,53;,
  4;53,65,66,54;,
  4;54,66,67,55;,
  4;55,67,68,56;,
  4;56,68,69,57;,
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
  4;67,79,80,68;,
  4;68,80,81,69;,
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
  4;79,91,92,80;,
  4;80,92,93,81;,
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
  3;92,91,104;,
  3;104,91,198;,
  3;198,91,103;,
  3;105,93,199;,
  3;93,92,199;,
  3;199,92,230;,
  4;93,105,106,94;,
  4;94,106,107,95;,
  4;95,107,96,84;,
  4;96,108,109,97;,
  4;97,109,110,98;,
  4;98,110,111,99;,
  4;99,111,112,100;,
  4;100,112,113,101;,
  4;101,113,114,102;,
  4;102,114,115,103;,
  4;195,116,104,198;,
  4;197,117,105,199;,
  4;105,117,118,106;,
  4;106,118,119,107;,
  4;107,119,108,96;,
  4;108,120,121,109;,
  4;109,121,122,110;,
  4;110,122,123,111;,
  4;111,123,124,112;,
  4;112,124,125,113;,
  4;113,125,126,114;,
  4;114,126,127,115;,
  4;194,128,116,195;,
  4;196,129,117,197;,
  4;117,129,130,118;,
  4;118,130,131,119;,
  4;119,131,120,108;,
  4;120,132,133,121;,
  4;121,133,134,122;,
  4;122,134,135,123;,
  4;123,135,136,124;,
  4;124,136,137,125;,
  4;125,137,138,126;,
  4;126,138,139,127;,
  3;127,139,194;,
  3;194,139,128;,
  3;128,139,140;,
  3;140,141,128;,
  3;128,141,196;,
  3;196,141,129;,
  4;129,141,142,130;,
  4;130,142,143,131;,
  4;131,143,132,120;,
  4;132,144,145,133;,
  4;133,145,146,134;,
  4;134,146,147,135;,
  4;135,147,148,136;,
  4;136,148,149,137;,
  4;137,149,150,138;,
  4;138,150,151,139;,
  4;139,151,152,140;,
  4;140,152,153,141;,
  4;141,153,154,142;,
  4;142,154,155,143;,
  4;143,155,144,132;,
  3;182,183,181;,
  3;184,183,182;,
  3;185,183,184;,
  3;186,183,185;,
  3;187,183,186;,
  3;188,183,187;,
  3;189,183,188;,
  3;190,183,189;,
  3;191,183,190;,
  3;192,183,191;,
  3;193,183,192;,
  3;181,183,193;,
  3;161,156,162;,
  3;162,156,163;,
  3;163,156,164;,
  3;164,156,165;,
  3;165,156,166;,
  3;166,156,167;,
  3;167,156,168;,
  3;168,156,157;,
  3;157,156,158;,
  3;158,156,159;,
  3;159,156,160;,
  3;160,156,161;,
  4;152,157,158,153;,
  4;153,158,159,154;,
  4;154,159,160,155;,
  4;155,160,161,144;,
  4;144,161,162,145;,
  4;145,162,163,146;,
  4;146,163,164,147;,
  4;147,164,165,148;,
  4;148,165,166,149;,
  4;149,166,167,150;,
  4;150,167,168,151;,
  4;151,168,157,152;,
  4;49,170,169,48;,
  4;50,171,170,49;,
  4;51,172,171,50;,
  4;52,173,172,51;,
  4;53,174,173,52;,
  4;54,175,174,53;,
  4;55,176,175,54;,
  4;56,177,176,55;,
  4;57,178,177,56;,
  4;58,179,178,57;,
  4;59,180,179,58;,
  4;48,169,180,59;,
  4;170,182,181,169;,
  4;171,184,182,170;,
  4;172,185,184,171;,
  4;173,186,185,172;,
  4;174,187,186,173;,
  4;175,188,187,174;,
  4;176,189,188,175;,
  4;177,190,189,176;,
  4;178,191,190,177;,
  4;179,192,191,178;,
  4;180,193,192,179;,
  4;169,181,193,180;,
  4;115,127,194,195;,
  4;116,128,196,197;,
  4;103,115,195,198;,
  4;230,116,197,199;;
 }
 MeshTextureCoords {
  266;
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
  0.702250;0.177501;,
  0.604668;0.890762;,
  0.577076;0.837801;,
  0.614132;0.784609;,
  0.633876;0.923045;,
  0.663084;0.789297;,
  0.663084;0.947695;,
  0.712037;0.784609;,
  0.692292;0.923045;,
  0.749092;0.837801;,
  0.721500;0.890762;,
  0.585156;0.754588;,
  0.624120;0.696458;,
  0.663084;0.689312;,
  0.702048;0.696458;,
  0.741012;0.754588;,
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
  0.597396;0.910320;,
  0.630240;0.946622;,
  0.663084;0.960690;,
  0.695928;0.946622;,
  0.728773;0.910320;,
  0.759800;0.850768;,
  0.750714;0.757195;,
  0.566369;0.850768;,
  0.575455;0.757195;,
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
  0.756465;0.254066;;
 }
}
