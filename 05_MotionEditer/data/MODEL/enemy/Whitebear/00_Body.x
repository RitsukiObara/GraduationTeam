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
 216;
 0.00000;24.82220;-35.35918;,
 -8.82205;28.43982;-41.59271;,
 -8.82205;44.17254;-24.01357;,
 0.76376;38.19704;-22.78631;,
 -7.10615;47.29428;0.00000;,
 3.23498;41.73723;0.00000;,
 -8.82205;44.17254;24.01355;,
 0.76376;38.19704;22.78631;,
 -8.82205;28.43982;41.59271;,
 0.00000;24.82220;35.35916;,
 -8.82205;6.94854;48.80745;,
 0.00000;6.55184;41.79994;,
 -8.82205;-14.54274;41.59271;,
 -1.19358;-11.71852;35.35916;,
 -8.82205;-28.47067;24.01355;,
 -1.19358;-23.28858;20.41461;,
 -8.82205;-30.49209;0.00000;,
 -1.19358;-24.94595;0.00000;,
 -8.82205;-28.47067;-24.01357;,
 -1.19358;-23.28858;-20.41463;,
 -8.82205;-14.54274;-41.59271;,
 -1.19358;-11.71852;-35.35918;,
 -8.82205;6.94854;-48.80745;,
 0.00000;6.55184;-41.79994;,
 -21.27427;30.53873;-43.91865;,
 -21.27427;50.33419;-25.35644;,
 -18.15824;54.14039;0.00000;,
 -21.27427;50.33419;25.35644;,
 -21.27427;30.53873;43.91865;,
 -21.27427;6.94854;51.81914;,
 -21.27427;-16.64165;43.91865;,
 -21.27427;-31.20509;25.35644;,
 -21.27427;-33.44240;0.00000;,
 -21.27427;-31.20509;-25.35644;,
 -21.27427;-16.64165;-43.91865;,
 -21.27427;6.94854;-51.81917;,
 -38.92221;35.50709;-43.96156;,
 -38.92221;54.94136;-25.38122;,
 -38.92221;58.86736;0.00000;,
 -38.92221;54.94136;25.38122;,
 -38.92221;35.50709;43.96156;,
 -38.92221;6.94854;50.42633;,
 -38.92221;-17.45156;43.96156;,
 -38.92221;-32.70470;25.38122;,
 -38.92221;-35.02532;0.00000;,
 -38.92221;-32.70470;-25.38122;,
 -38.92221;-17.45156;-43.96156;,
 -38.92221;6.94854;-50.42635;,
 -63.87286;34.99848;-42.99721;,
 -63.87286;55.55835;-24.82445;,
 -63.87286;59.12154;0.00000;,
 -63.87286;55.55835;24.82445;,
 -63.87286;34.99848;42.99721;,
 -63.87286;6.94854;49.32014;,
 -63.87286;-18.09488;42.99721;,
 -63.87286;-32.91003;24.82445;,
 -63.87286;-35.29681;0.00000;,
 -63.87286;-32.91003;-24.82445;,
 -63.87286;-18.09488;-42.99721;,
 -63.87286;6.94854;-49.32016;,
 -86.06939;34.45346;-38.87435;,
 -86.06939;53.17213;-22.44411;,
 -86.06939;55.66621;0.00000;,
 -86.06939;53.17213;22.44411;,
 -86.06939;34.45346;38.87435;,
 -86.06939;9.77600;44.59104;,
 -86.06939;-14.90141;38.87435;,
 -86.06939;-28.29515;22.44411;,
 -86.06939;-30.87770;0.00000;,
 -86.06939;-28.29515;-22.44411;,
 -86.06939;-14.90141;-38.87435;,
 -86.06939;9.77600;-44.59104;,
 -99.73773;33.16246;-33.61325;,
 -99.73773;49.43516;-19.40663;,
 -99.73773;51.41666;0.00000;,
 -99.73773;49.43516;19.40661;,
 -99.73773;33.16246;33.61323;,
 -99.73773;11.82474;37.34863;,
 -99.73773;-7.16661;33.61323;,
 -99.73773;-23.31182;19.40661;,
 -99.73773;-25.72000;0.00000;,
 -99.73773;-23.31182;-19.40663;,
 -99.73773;-7.16661;-33.61325;,
 -99.73773;11.82474;-37.34863;,
 9.96005;29.35307;-17.13159;,
 14.32019;18.37652;-8.56580;,
 14.14640;14.32739;-13.65424;,
 9.61249;21.25483;-27.30848;,
 11.33006;32.15270;0.00000;,
 15.00520;19.77631;0.00000;,
 9.96005;29.35307;17.13159;,
 14.32019;18.37652;8.56580;,
 9.61249;21.25483;27.30848;,
 14.14640;14.32739;13.65424;,
 7.43463;7.24655;30.32071;,
 13.05749;7.32325;15.16037;,
 5.63879;-6.84241;27.26678;,
 12.15956;0.27875;13.63340;,
 4.43713;-17.15625;15.74249;,
 11.55875;-4.87816;7.87124;,
 3.72658;-18.44086;-0.00000;,
 11.20347;-5.52047;-0.00000;,
 4.43713;-17.15625;-15.74249;,
 11.55875;-4.87816;-7.87124;,
 5.63879;-6.84241;-27.26678;,
 12.15956;0.27875;-13.63340;,
 7.43463;7.24655;-30.32071;,
 13.05749;7.32325;-15.16037;,
 -109.09397;28.66793;-26.53304;,
 -115.67598;24.32941;-19.69859;,
 -115.67597;33.48344;-11.37299;,
 -109.09395;40.99795;-15.31888;,
 -115.67598;34.55633;0.00000;,
 -109.09397;42.51715;0.00000;,
 -115.67597;33.48344;11.37299;,
 -109.09395;40.99795;15.31886;,
 -115.67598;24.32941;19.69857;,
 -109.09397;28.66793;26.53304;,
 -115.67597;11.82474;21.88766;,
 -109.09395;11.82474;29.48162;,
 -115.67597;1.66644;19.69857;,
 -109.09395;-2.67208;26.53304;,
 -115.01539;-6.42436;10.49508;,
 -110.01214;-15.00165;15.31886;,
 -115.01539;-7.92396;0.00000;,
 -110.01214;-16.94752;0.00000;,
 -115.01539;-6.42436;-10.49508;,
 -110.01214;-15.00165;-15.31888;,
 -115.67597;1.66644;-19.69859;,
 -109.09395;-2.67208;-26.53304;,
 -115.67597;11.82474;-21.88766;,
 -109.09395;11.82474;-29.48164;,
 -104.14674;31.76435;-31.41081;,
 -104.14674;46.36109;-18.13505;,
 -104.14674;48.19878;0.00000;,
 -104.14674;46.36109;18.13505;,
 -104.14674;31.76435;31.41079;,
 -104.14674;11.82474;34.90146;,
 -104.14674;-5.76848;31.41079;,
 -104.14674;-20.36524;18.13505;,
 -104.14674;-22.62960;0.00000;,
 -104.14674;-20.36524;-18.13505;,
 -104.14674;-5.76848;-31.41081;,
 -104.14674;11.82474;-34.90146;,
 -118.52283;22.83849;-6.42575;,
 -118.52283;24.57533;0.00000;,
 -118.52283;18.88990;-11.12974;,
 -118.52283;11.82474;-12.36658;,
 -118.52283;7.10595;-11.12974;,
 -118.52283;1.93390;-6.42575;,
 -118.52283;0.99382;0.00000;,
 -118.52283;1.93390;6.42575;,
 -118.52283;7.10595;11.12974;,
 -118.52283;11.82474;12.36656;,
 -118.52283;18.88990;11.12974;,
 -118.52283;24.06195;6.42575;,
 -121.96341;11.61140;0.00000;,
 14.47243;7.39996;0.00000;,
 26.18937;27.48386;1.05955;,
 22.80359;29.51671;4.03904;,
 24.81007;23.34042;6.72896;,
 27.27527;24.14128;2.51532;,
 26.81656;17.16410;4.03904;,
 28.36116;20.79869;1.05955;,
 27.64769;14.60581;-2.45502;,
 28.81096;19.41415;-2.45502;,
 26.81656;17.16410;-8.94906;,
 28.36116;20.79869;-5.96956;,
 24.81007;23.34042;-11.63898;,
 27.27527;24.14128;-7.42534;,
 22.80359;29.51671;-8.94906;,
 26.18937;27.48386;-5.96956;,
 21.97247;32.07501;-2.45502;,
 25.73957;28.86842;-2.45502;,
 18.49903;30.21156;6.02987;,
 21.12064;22.14185;9.54443;,
 23.74225;14.07211;6.02987;,
 24.82816;10.72951;-2.45502;,
 23.74225;14.07211;-10.93990;,
 21.12064;22.14185;-14.45445;,
 18.49903;30.21156;-10.93990;,
 17.41313;33.55416;-2.45502;,
 13.93105;29.46260;6.72896;,
 16.76866;20.72801;10.53308;,
 19.60627;11.99340;6.72896;,
 20.78165;8.37540;-2.45502;,
 19.60627;11.99340;-11.63898;,
 16.76866;20.72801;-15.44312;,
 13.93105;29.46263;-11.63898;,
 12.75568;33.08060;-2.45502;,
 9.79508;27.38392;6.02987;,
 12.41669;19.31418;9.54443;,
 15.03828;11.24446;6.02987;,
 16.12420;7.90186;-2.45502;,
 15.03828;11.24446;-10.93990;,
 12.41669;19.31418;-14.45445;,
 9.79508;27.38392;-10.93990;,
 8.70917;30.72650;-2.45502;,
 6.72075;24.29190;4.03904;,
 8.72726;18.11561;6.72896;,
 10.73376;11.93929;4.03904;,
 11.56487;9.38099;-2.45502;,
 10.73376;11.93929;-8.94906;,
 8.72726;18.11561;-11.63898;,
 6.72075;24.29190;-8.94906;,
 5.88965;26.85020;-2.45502;,
 5.17615;20.65732;1.05955;,
 6.26207;17.31472;2.51532;,
 7.34796;13.97214;1.05955;,
 7.79776;12.58758;-2.45502;,
 7.34796;13.97214;-5.96956;,
 6.26207;17.31472;-7.42534;,
 5.17615;20.65732;-5.96956;,
 4.72635;22.04186;-2.45502;,
 28.14093;24.42251;-2.45502;,
 5.39638;17.03349;-2.45502;;
 
 232;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,1,0;,
 4;1,24,25,2;,
 4;2,25,26,4;,
 4;4,26,27,6;,
 4;6,27,28,8;,
 4;8,28,29,10;,
 4;10,29,30,12;,
 4;12,30,31,14;,
 4;14,31,32,16;,
 4;16,32,33,18;,
 4;18,33,34,20;,
 4;20,34,35,22;,
 4;22,35,24,1;,
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
 4;43,55,56,44;,
 4;44,56,57,45;,
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
 4;84,85,86,87;,
 4;88,89,85,84;,
 4;90,91,89,88;,
 4;92,93,91,90;,
 4;94,95,93,92;,
 4;96,97,95,94;,
 4;98,99,97,96;,
 4;100,101,99,98;,
 4;102,103,101,100;,
 4;104,105,103,102;,
 4;106,107,105,104;,
 4;87,86,107,106;,
 4;108,109,110,111;,
 4;111,110,112,113;,
 4;113,112,114,115;,
 4;115,114,116,117;,
 4;117,116,118,119;,
 4;119,118,120,121;,
 4;121,120,122,123;,
 4;123,122,124,125;,
 4;125,124,126,127;,
 4;127,126,128,129;,
 4;129,128,130,131;,
 4;131,130,109,108;,
 4;72,132,133,73;,
 4;73,133,134,74;,
 4;74,134,135,75;,
 4;75,135,136,76;,
 4;76,136,137,77;,
 4;77,137,138,78;,
 4;78,138,139,79;,
 4;79,139,140,80;,
 4;80,140,141,81;,
 4;81,141,142,82;,
 4;82,142,143,83;,
 4;83,143,132,72;,
 4;132,108,111,133;,
 4;133,111,113,134;,
 4;134,113,115,135;,
 4;135,115,117,136;,
 4;136,117,119,137;,
 4;137,119,121,138;,
 4;138,121,123,139;,
 4;139,123,125,140;,
 4;140,125,127,141;,
 4;141,127,129,142;,
 4;142,129,131,143;,
 4;143,131,108,132;,
 4;3,84,87,0;,
 4;5,88,84,3;,
 4;7,90,88,5;,
 4;9,92,90,7;,
 4;11,94,92,9;,
 4;13,96,94,11;,
 4;15,98,96,13;,
 4;17,100,98,15;,
 4;19,102,100,17;,
 4;21,104,102,19;,
 4;23,106,104,21;,
 4;0,87,106,23;,
 4;110,144,145,112;,
 4;109,146,144,110;,
 4;130,147,146,109;,
 4;128,148,147,130;,
 4;126,149,148,128;,
 4;124,150,149,126;,
 4;122,151,150,124;,
 4;120,152,151,122;,
 4;118,153,152,120;,
 4;116,154,153,118;,
 4;114,155,154,116;,
 4;112,145,155,114;,
 3;155,156,154;,
 3;145,156,155;,
 3;144,156,145;,
 3;146,156,144;,
 3;147,156,146;,
 3;148,156,147;,
 3;149,156,148;,
 3;150,156,149;,
 3;151,156,150;,
 3;152,156,151;,
 3;153,156,152;,
 3;154,156,153;,
 3;91,157,89;,
 3;93,157,91;,
 3;95,157,93;,
 3;97,157,95;,
 3;99,157,97;,
 3;101,157,99;,
 3;103,157,101;,
 3;105,157,103;,
 3;107,157,105;,
 3;86,157,107;,
 3;85,157,86;,
 3;89,157,85;,
 4;158,159,160,161;,
 4;161,160,162,163;,
 4;163,162,164,165;,
 4;165,164,166,167;,
 4;167,166,168,169;,
 4;169,168,170,171;,
 4;171,170,172,173;,
 4;173,172,159,158;,
 4;159,174,175,160;,
 4;160,175,176,162;,
 4;162,176,177,164;,
 4;164,177,178,166;,
 4;166,178,179,168;,
 4;168,179,180,170;,
 4;170,180,181,172;,
 4;172,181,174,159;,
 4;174,182,183,175;,
 4;175,183,184,176;,
 4;176,184,185,177;,
 4;177,185,186,178;,
 4;178,186,187,179;,
 4;179,187,188,180;,
 4;180,188,189,181;,
 4;181,189,182,174;,
 4;182,190,191,183;,
 4;183,191,192,184;,
 4;184,192,193,185;,
 4;185,193,194,186;,
 4;186,194,195,187;,
 4;187,195,196,188;,
 4;188,196,197,189;,
 4;189,197,190,182;,
 4;190,198,199,191;,
 4;191,199,200,192;,
 4;192,200,201,193;,
 4;193,201,202,194;,
 4;194,202,203,195;,
 4;195,203,204,196;,
 4;196,204,205,197;,
 4;197,205,198,190;,
 4;198,206,207,199;,
 4;199,207,208,200;,
 4;200,208,209,201;,
 4;201,209,210,202;,
 4;202,210,211,203;,
 4;203,211,212,204;,
 4;204,212,213,205;,
 4;205,213,206,198;,
 3;161,214,158;,
 3;163,214,161;,
 3;165,214,163;,
 3;167,214,165;,
 3;169,214,167;,
 3;171,214,169;,
 3;173,214,171;,
 3;158,214,173;,
 3;206,215,207;,
 3;207,215,208;,
 3;208,215,209;,
 3;209,215,210;,
 3;210,215,211;,
 3;211,215,212;,
 3;212,215,213;,
 3;213,215,206;;
 
 MeshMaterialList {
  2;
  232;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\enemy\\whitebear_UV.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  216;
  0.672392;0.377124;-0.636920;,
  0.648039;0.666720;-0.368145;,
  0.641061;0.767490;0.000000;,
  0.648039;0.666719;0.368145;,
  0.672392;0.377124;0.636920;,
  0.726728;-0.030620;0.686243;,
  0.737953;-0.408887;0.536877;,
  0.718856;-0.651759;0.241778;,
  0.687756;-0.725942;0.000000;,
  0.718856;-0.651759;-0.241778;,
  0.737953;-0.408887;-0.536877;,
  0.726728;-0.030620;-0.686243;,
  0.445112;0.476137;-0.758398;,
  0.483313;0.772729;-0.411459;,
  0.501119;0.865378;0.000000;,
  0.483314;0.772729;0.411459;,
  0.445126;0.476135;0.758392;,
  0.455489;-0.004151;0.890232;,
  0.456485;-0.518046;0.723360;,
  0.444096;-0.834649;0.325791;,
  0.414687;-0.909964;0.000000;,
  0.444095;-0.834649;-0.325791;,
  0.456471;-0.518049;-0.723367;,
  0.455461;-0.004151;-0.890246;,
  0.186122;0.501066;-0.845158;,
  0.321179;0.834418;-0.447874;,
  0.369023;0.929420;0.000000;,
  0.321179;0.834418;0.447873;,
  0.186137;0.501065;0.845155;,
  0.108916;-0.004568;0.994040;,
  0.126903;-0.562234;0.817183;,
  0.157450;-0.917113;0.366216;,
  0.157412;-0.987533;0.000000;,
  0.157450;-0.917113;-0.366216;,
  0.126887;-0.562235;-0.817185;,
  0.108885;-0.004568;-0.994044;,
  0.029984;0.477524;-0.878107;,
  0.109910;0.877386;-0.467027;,
  0.127275;0.991867;0.000000;,
  0.109910;0.877386;0.467027;,
  0.029982;0.477524;0.878107;,
  -0.029787;-0.012282;0.999481;,
  -0.005630;-0.548817;0.835923;,
  0.036878;-0.925620;0.376654;,
  0.047843;-0.998855;-0.000000;,
  0.036878;-0.925620;-0.376654;,
  -0.005630;-0.548818;-0.835923;,
  -0.029785;-0.012282;-0.999481;,
  -0.102435;0.457992;-0.883035;,
  -0.078482;0.880792;-0.466953;,
  -0.061413;0.998112;0.000000;,
  -0.078482;0.880792;0.466953;,
  -0.102437;0.457992;0.883035;,
  -0.123764;-0.010440;0.992257;,
  -0.128705;-0.531552;0.837190;,
  -0.111293;-0.918506;0.379421;,
  -0.098371;-0.995150;0.000000;,
  -0.111293;-0.918506;-0.379421;,
  -0.128705;-0.531553;-0.837190;,
  -0.123762;-0.010440;-0.992257;,
  -0.275557;0.434451;-0.857509;,
  -0.243216;0.859508;-0.449545;,
  -0.215387;0.976529;0.000000;,
  -0.243217;0.859508;0.449545;,
  -0.275557;0.434451;0.857509;,
  -0.326359;-0.003560;0.945239;,
  -0.364372;-0.476140;0.800328;,
  -0.330065;-0.865127;0.377640;,
  -0.282135;-0.959375;0.000000;,
  -0.330065;-0.865127;-0.377640;,
  -0.364371;-0.476140;-0.800328;,
  -0.326359;-0.003560;-0.945239;,
  -0.450363;0.392336;-0.802026;,
  -0.464466;0.786190;-0.407648;,
  -0.446329;0.894869;0.000000;,
  -0.464467;0.786190;0.407648;,
  -0.450364;0.392336;0.802025;,
  -0.464003;-0.005622;0.885816;,
  -0.502259;-0.402154;0.765511;,
  -0.506433;-0.773225;0.381640;,
  -0.470163;-0.882580;0.000000;,
  -0.506433;-0.773225;-0.381639;,
  -0.502259;-0.402154;-0.765512;,
  -0.464003;-0.005622;-0.885816;,
  -0.647667;0.329873;-0.686812;,
  -0.687573;0.646106;-0.331348;,
  -0.675214;0.737622;0.000000;,
  -0.687572;0.646106;0.331348;,
  -0.647667;0.329874;0.686812;,
  -0.621531;-0.009012;0.783338;,
  -0.637278;-0.339654;0.691746;,
  -0.659232;-0.664492;0.351943;,
  -0.637876;-0.770139;0.000000;,
  -0.659232;-0.664492;-0.351943;,
  -0.637278;-0.339653;-0.691746;,
  -0.621531;-0.009012;-0.783338;,
  -0.765704;0.280004;-0.579047;,
  -0.782999;0.556148;-0.278590;,
  -0.999883;-0.015055;-0.002509;,
  -0.761472;0.648197;0.000000;,
  -0.782999;0.556148;0.278591;,
  -0.765704;0.280004;0.579047;,
  -0.748351;-0.009560;0.663234;,
  -0.772208;-0.285359;0.567684;,
  -0.803271;-0.527394;0.276787;,
  -0.795499;-0.605955;0.000000;,
  -0.803271;-0.527394;-0.276787;,
  -0.772208;-0.285359;-0.567684;,
  -0.748351;-0.009560;-0.663234;,
  0.871980;0.202689;-0.445610;,
  0.867351;0.425370;-0.258386;,
  0.991757;-0.128130;-0.000000;,
  0.877752;0.479115;0.000000;,
  0.867351;0.425370;0.258386;,
  0.871980;0.202689;0.445610;,
  0.873292;-0.093272;0.478186;,
  0.870578;-0.322497;0.371603;,
  0.848971;-0.503456;0.160563;,
  0.823977;-0.566623;0.000000;,
  0.848971;-0.503455;-0.160563;,
  0.870578;-0.322497;-0.371603;,
  0.873292;-0.093272;-0.478186;,
  -0.886044;0.463557;-0.006320;,
  -0.897625;0.393603;-0.198359;,
  -0.888694;0.202921;-0.411152;,
  -0.877579;-0.010000;-0.479327;,
  -0.893632;-0.218585;-0.391973;,
  -0.907389;-0.379088;-0.181488;,
  -0.905460;-0.424431;0.000000;,
  -0.907389;-0.379088;0.181488;,
  -0.893632;-0.218585;0.391973;,
  -0.877579;-0.010000;0.479327;,
  -0.887365;0.200269;0.415302;,
  -0.897249;0.395271;0.196735;,
  -0.965010;0.113239;0.236501;,
  -0.968750;0.222725;0.109166;,
  -0.964783;0.262406;-0.018359;,
  -0.966471;0.228115;-0.117888;,
  -0.965999;0.123100;-0.227359;,
  -0.960929;-0.010308;-0.276602;,
  -0.959349;-0.144472;-0.242440;,
  -0.951543;-0.279204;-0.128884;,
  -0.941470;-0.337097;0.000000;,
  -0.951543;-0.279204;0.128884;,
  -0.959349;-0.144471;0.242441;,
  -0.960929;-0.010308;0.276602;,
  0.992518;0.122103;0.000000;,
  0.987546;0.113863;0.108569;,
  0.985300;0.001409;0.170830;,
  0.966014;-0.136563;0.219470;,
  0.956169;-0.226201;0.185942;,
  0.941025;-0.330688;0.071531;,
  0.926338;-0.376693;-0.000000;,
  0.941025;-0.330688;-0.071531;,
  0.956169;-0.226201;-0.185942;,
  0.966014;-0.136563;-0.219470;,
  0.985300;0.001409;-0.170830;,
  0.987546;0.113863;-0.108569;,
  0.810755;0.528363;0.252010;,
  0.888619;0.288684;0.356395;,
  0.966483;0.049006;0.252009;,
  0.998736;-0.050271;-0.000000;,
  0.966483;0.049006;-0.252010;,
  0.888619;0.288684;-0.356395;,
  0.810755;0.528363;-0.252010;,
  0.778502;0.627642;-0.000000;,
  0.551616;0.682846;0.479000;,
  0.699614;0.227283;0.677409;,
  0.847613;-0.228280;0.479000;,
  0.908916;-0.416979;-0.000000;,
  0.847613;-0.228279;-0.479000;,
  0.699615;0.227283;-0.677408;,
  0.551617;0.682846;-0.479000;,
  0.490313;0.871546;-0.000000;,
  0.190891;0.740066;0.644874;,
  0.390140;0.126745;0.911990;,
  0.589390;-0.486576;0.644875;,
  0.671922;-0.740622;-0.000000;,
  0.589390;-0.486575;-0.644875;,
  0.390141;0.126745;-0.911990;,
  0.190893;0.740066;-0.644874;,
  0.108360;0.994112;0.000000;,
  -0.218478;0.672508;0.707107;,
  -0.000000;0.000000;1.000000;,
  0.218477;-0.672508;0.707107;,
  0.308973;-0.951071;-0.000000;,
  0.218477;-0.672508;-0.707107;,
  0.000000;-0.000000;-1.000000;,
  -0.218478;0.672508;-0.707107;,
  -0.308974;0.951070;0.000000;,
  -0.589390;0.486577;0.644874;,
  -0.390141;-0.126745;0.911990;,
  -0.190892;-0.740066;0.644874;,
  -0.108361;-0.994112;-0.000000;,
  -0.190892;-0.740066;-0.644874;,
  -0.390141;-0.126745;-0.911990;,
  -0.589390;0.486576;-0.644874;,
  -0.671922;0.740622;0.000000;,
  -0.847613;0.228280;0.479000;,
  -0.699613;-0.227284;0.677409;,
  -0.551615;-0.682846;0.479001;,
  -0.490312;-0.871547;-0.000000;,
  -0.551615;-0.682846;-0.479001;,
  -0.699614;-0.227284;-0.677409;,
  -0.847613;0.228279;-0.479000;,
  -0.908916;0.416980;-0.000000;,
  -0.966483;-0.049007;0.252010;,
  -0.888618;-0.288686;0.356397;,
  -0.810753;-0.528364;0.252012;,
  -0.778501;-0.627643;-0.000000;,
  -0.810753;-0.528364;-0.252012;,
  -0.888618;-0.288686;-0.356398;,
  -0.966483;-0.049007;-0.252011;,
  -0.998736;0.050273;-0.000000;,
  0.951071;0.308972;-0.000000;,
  -0.951070;-0.308974;-0.000000;;
  232;
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
  4;43,55,56,44;,
  4;44,56,57,45;,
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
  4;110,157,156,109;,
  4;112,146,157,110;,
  4;113,147,146,112;,
  4;114,148,147,113;,
  4;115,149,148,114;,
  4;116,150,149,115;,
  4;117,151,150,116;,
  4;118,152,151,117;,
  4;119,153,152,118;,
  4;120,154,153,119;,
  4;121,155,154,120;,
  4;109,156,155,121;,
  4;96,124,123,97;,
  4;97,123,122,99;,
  4;99,122,133,100;,
  4;100,133,132,101;,
  4;101,132,131,102;,
  4;102,131,130,103;,
  4;103,130,129,104;,
  4;104,129,128,105;,
  4;105,128,127,106;,
  4;106,127,126,107;,
  4;107,126,125,108;,
  4;108,125,124,96;,
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
  4;85,97,99,86;,
  4;86,99,100,87;,
  4;87,100,101,88;,
  4;88,101,102,89;,
  4;89,102,103,90;,
  4;90,103,104,91;,
  4;91,104,105,92;,
  4;92,105,106,93;,
  4;93,106,107,94;,
  4;94,107,108,95;,
  4;95,108,96,84;,
  4;1,110,109,0;,
  4;2,112,110,1;,
  4;3,113,112,2;,
  4;4,114,113,3;,
  4;5,115,114,4;,
  4;6,116,115,5;,
  4;7,117,116,6;,
  4;8,118,117,7;,
  4;9,119,118,8;,
  4;10,120,119,9;,
  4;11,121,120,10;,
  4;0,109,121,11;,
  4;123,137,136,122;,
  4;124,138,137,123;,
  4;125,139,138,124;,
  4;126,140,139,125;,
  4;127,141,140,126;,
  4;128,142,141,127;,
  4;129,143,142,128;,
  4;130,144,143,129;,
  4;131,145,144,130;,
  4;132,134,145,131;,
  4;133,135,134,132;,
  4;122,136,135,133;,
  3;135,98,134;,
  3;136,98,135;,
  3;137,98,136;,
  3;138,98,137;,
  3;139,98,138;,
  3;140,98,139;,
  3;141,98,140;,
  3;142,98,141;,
  3;143,98,142;,
  3;144,98,143;,
  3;145,98,144;,
  3;134,98,145;,
  3;147,111,146;,
  3;148,111,147;,
  3;149,111,148;,
  3;150,111,149;,
  3;151,111,150;,
  3;152,111,151;,
  3;153,111,152;,
  3;154,111,153;,
  3;155,111,154;,
  3;156,111,155;,
  3;157,111,156;,
  3;146,111,157;,
  4;158,166,167,159;,
  4;159,167,168,160;,
  4;160,168,169,161;,
  4;161,169,170,162;,
  4;162,170,171,163;,
  4;163,171,172,164;,
  4;164,172,173,165;,
  4;165,173,166,158;,
  4;166,174,175,167;,
  4;167,175,176,168;,
  4;168,176,177,169;,
  4;169,177,178,170;,
  4;170,178,179,171;,
  4;171,179,180,172;,
  4;172,180,181,173;,
  4;173,181,174,166;,
  4;174,182,183,175;,
  4;175,183,184,176;,
  4;176,184,185,177;,
  4;177,185,186,178;,
  4;178,186,187,179;,
  4;179,187,188,180;,
  4;180,188,189,181;,
  4;181,189,182,174;,
  4;182,190,191,183;,
  4;183,191,192,184;,
  4;184,192,193,185;,
  4;185,193,194,186;,
  4;186,194,195,187;,
  4;187,195,196,188;,
  4;188,196,197,189;,
  4;189,197,190,182;,
  4;190,198,199,191;,
  4;191,199,200,192;,
  4;192,200,201,193;,
  4;193,201,202,194;,
  4;194,202,203,195;,
  4;195,203,204,196;,
  4;196,204,205,197;,
  4;197,205,198,190;,
  4;198,206,207,199;,
  4;199,207,208,200;,
  4;200,208,209,201;,
  4;201,209,210,202;,
  4;202,210,211,203;,
  4;203,211,212,204;,
  4;204,212,213,205;,
  4;205,213,206,198;,
  3;159,214,158;,
  3;160,214,159;,
  3;161,214,160;,
  3;162,214,161;,
  3;163,214,162;,
  3;164,214,163;,
  3;165,214,164;,
  3;158,214,165;,
  3;206,215,207;,
  3;207,215,208;,
  3;208,215,209;,
  3;209,215,210;,
  3;210,215,211;,
  3;211,215,212;,
  3;212,215,213;,
  3;213,215,206;;
 }
 MeshTextureCoords {
  216;
  0.361610;0.467280;,
  0.345720;0.458450;,
  0.345720;0.420060;,
  0.362990;0.434640;,
  0.348810;0.404090;,
  0.367440;0.418870;,
  0.345720;0.420060;,
  0.362990;0.434640;,
  0.345720;0.458450;,
  0.361610;0.467280;,
  0.345720;0.510890;,
  0.361610;0.511860;,
  0.345720;0.563330;,
  0.359460;0.556440;,
  0.345720;0.601720;,
  0.359460;0.589080;,
  0.345720;0.615770;,
  0.359460;0.601020;,
  0.345720;0.601720;,
  0.359460;0.589080;,
  0.345720;0.563330;,
  0.359460;0.556440;,
  0.345720;0.510890;,
  0.361610;0.511860;,
  0.323280;0.453330;,
  0.323280;0.411190;,
  0.328890;0.385880;,
  0.323280;0.411190;,
  0.323280;0.453330;,
  0.323280;0.510890;,
  0.323280;0.568450;,
  0.323280;0.610590;,
  0.323280;0.626020;,
  0.323280;0.610590;,
  0.323280;0.568450;,
  0.323280;0.510890;,
  0.291470;0.451350;,
  0.291470;0.407770;,
  0.291470;0.387640;,
  0.291470;0.407770;,
  0.291470;0.451350;,
  0.291470;0.510890;,
  0.291470;0.570430;,
  0.291470;0.614020;,
  0.291470;0.629970;,
  0.291470;0.614020;,
  0.291470;0.570430;,
  0.291470;0.510890;,
  0.240160;0.456970;,
  0.240160;0.417500;,
  0.240160;0.403060;,
  0.240160;0.417500;,
  0.240160;0.456970;,
  0.240160;0.510890;,
  0.240160;0.564810;,
  0.240160;0.604280;,
  0.240160;0.618730;,
  0.240160;0.604280;,
  0.240160;0.564810;,
  0.240160;0.510890;,
  0.192710;0.469400;,
  0.192710;0.439020;,
  0.192710;0.427910;,
  0.192710;0.439020;,
  0.192710;0.469400;,
  0.192710;0.510890;,
  0.192710;0.552390;,
  0.192710;0.582760;,
  0.192710;0.593880;,
  0.192710;0.582760;,
  0.192710;0.552390;,
  0.192710;0.510890;,
  0.151390;0.480540;,
  0.151390;0.458330;,
  0.151390;0.450190;,
  0.151390;0.458330;,
  0.151390;0.480540;,
  0.151390;0.510890;,
  0.151390;0.541240;,
  0.151390;0.563460;,
  0.151390;0.571590;,
  0.151390;0.563460;,
  0.151390;0.541240;,
  0.151390;0.510890;,
  0.381720;0.450720;,
  0.380590;0.480460;,
  0.379190;0.493090;,
  0.378940;0.475980;,
  0.384240;0.432400;,
  0.381840;0.471300;,
  0.381720;0.450720;,
  0.380590;0.480460;,
  0.378940;0.475980;,
  0.379190;0.493090;,
  0.375010;0.510170;,
  0.377230;0.510180;,
  0.371780;0.544540;,
  0.375610;0.527370;,
  0.369610;0.569710;,
  0.374530;0.539950;,
  0.368330;0.578920;,
  0.373890;0.544560;,
  0.369610;0.569710;,
  0.374530;0.539950;,
  0.371780;0.544540;,
  0.375610;0.527370;,
  0.375010;0.510170;,
  0.377230;0.510180;,
  0.140150;0.486940;,
  0.140150;0.498920;,
  0.140150;0.490150;,
  0.140150;0.469400;,
  0.140150;0.486940;,
  0.140150;0.462980;,
  0.140150;0.490150;,
  0.140150;0.469400;,
  0.140150;0.498920;,
  0.140150;0.486940;,
  0.140150;0.510890;,
  0.140150;0.510890;,
  0.140150;0.522870;,
  0.140150;0.534850;,
  0.140150;0.531640;,
  0.140150;0.552390;,
  0.140150;0.534850;,
  0.140150;0.558810;,
  0.140150;0.531640;,
  0.140150;0.552390;,
  0.140150;0.522870;,
  0.140150;0.534850;,
  0.140150;0.510890;,
  0.140150;0.510890;,
  0.146100;0.482530;,
  0.146100;0.461770;,
  0.146100;0.454170;,
  0.146100;0.461770;,
  0.146100;0.482530;,
  0.146100;0.510890;,
  0.146100;0.539260;,
  0.146100;0.560020;,
  0.146100;0.567620;,
  0.146100;0.560020;,
  0.146100;0.539260;,
  0.146100;0.510890;,
  0.140150;0.500520;,
  0.140150;0.498920;,
  0.140150;0.504900;,
  0.140150;0.510890;,
  0.140150;0.516880;,
  0.140150;0.521270;,
  0.140150;0.522870;,
  0.140150;0.521270;,
  0.140150;0.516880;,
  0.140150;0.510890;,
  0.140150;0.504900;,
  0.140150;0.500520;,
  0.140150;0.510890;,
  0.379450;0.510200;,
  0.360220;0.449900;,
  0.327840;0.431650;,
  0.347030;0.487110;,
  0.370610;0.479920;,
  0.366220;0.542570;,
  0.381000;0.509930;,
  0.374170;0.565540;,
  0.385300;0.522370;,
  0.366220;0.542570;,
  0.381000;0.509930;,
  0.347030;0.487110;,
  0.370610;0.479920;,
  0.327840;0.431650;,
  0.360220;0.449900;,
  0.319890;0.408680;,
  0.355920;0.437470;,
  0.286670;0.425410;,
  0.311740;0.497870;,
  0.336820;0.570340;,
  0.347210;0.600350;,
  0.336820;0.570340;,
  0.311740;0.497870;,
  0.286670;0.425410;,
  0.276290;0.395390;,
  0.242980;0.432130;,
  0.270120;0.510570;,
  0.297260;0.589000;,
  0.308500;0.621490;,
  0.297260;0.589000;,
  0.270120;0.510570;,
  0.242980;0.432130;,
  0.231740;0.399640;,
  0.203430;0.450800;,
  0.228500;0.523270;,
  0.253570;0.595730;,
  0.263960;0.625740;,
  0.253570;0.595730;,
  0.228500;0.523270;,
  0.203430;0.450800;,
  0.193040;0.420790;,
  0.174020;0.478570;,
  0.193210;0.534030;,
  0.212400;0.589490;,
  0.220350;0.612460;,
  0.212400;0.589490;,
  0.193210;0.534030;,
  0.174020;0.478570;,
  0.166070;0.455590;,
  0.159250;0.511200;,
  0.169630;0.541220;,
  0.180020;0.571240;,
  0.184320;0.583670;,
  0.180020;0.571240;,
  0.169630;0.541220;,
  0.159250;0.511200;,
  0.154950;0.498770;,
  0.378890;0.477390;,
  0.161350;0.543750;;
 }
}
