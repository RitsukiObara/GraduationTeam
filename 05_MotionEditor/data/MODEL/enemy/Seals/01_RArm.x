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
 170;
 -9.70101;2.81488;-17.56864;,
 -18.96725;1.42675;-15.40295;,
 -18.96725;4.46213;-8.15750;,
 -9.70101;6.23374;-9.40788;,
 -18.96725;5.57317;1.73989;,
 -9.70101;7.48514;1.73989;,
 -18.96725;4.46213;11.63730;,
 -9.70101;6.23374;12.88770;,
 -18.96725;1.42675;18.88271;,
 -9.70101;2.81488;21.04848;,
 -18.96725;-2.71966;21.53469;,
 -9.70101;-1.85537;24.03551;,
 -18.96725;-6.86606;18.88271;,
 -9.70101;-6.52563;21.04848;,
 -18.96725;-9.90144;11.63730;,
 -9.70101;-9.94449;12.88770;,
 -18.96725;-11.01249;1.73989;,
 -9.70101;-11.19588;1.73989;,
 -18.96725;-9.90144;-8.15750;,
 -9.70101;-9.94449;-9.40788;,
 -18.96725;-6.86606;-15.40295;,
 -9.70101;-6.52563;-17.56864;,
 -18.96725;-2.71966;-18.05492;,
 -9.70101;-1.85537;-20.55572;,
 9.48895;4.71222;-8.49593;,
 9.48895;-1.85537;1.73989;,
 9.48895;1.93643;-15.98913;,
 9.48895;5.72825;1.73989;,
 9.48895;4.71222;11.97576;,
 9.48895;1.93643;19.46891;,
 9.48895;-1.85537;22.21162;,
 9.48895;-5.78111;19.46891;,
 9.48895;-8.42298;11.97576;,
 9.48895;-9.43900;1.73989;,
 9.48895;-8.42298;-8.49593;,
 9.48895;-5.64718;-15.98913;,
 9.48895;-1.85537;-18.73182;,
 -77.04204;-12.81778;51.24457;,
 -74.55433;-13.42289;50.23325;,
 -75.99050;-12.37478;50.81718;,
 -74.55433;-12.21265;50.23325;,
 -73.11801;-12.37477;49.64943;,
 -72.06651;-12.81776;49.22199;,
 -71.68161;-13.42289;49.06557;,
 -72.06651;-14.02802;49.22199;,
 -73.11801;-14.47099;49.64943;,
 -74.55435;-14.63315;50.23331;,
 -75.99054;-14.47101;50.81716;,
 -77.04204;-14.02802;51.24454;,
 -77.42695;-13.42290;51.40099;,
 -28.19300;-0.47362;-13.62414;,
 -28.19300;2.15399;-6.59159;,
 -28.19300;3.11578;3.01496;,
 -28.19300;2.15399;12.62151;,
 -28.19300;-0.47362;19.65398;,
 -28.19300;-4.06302;22.22806;,
 -28.19300;-7.65240;19.65398;,
 -28.19300;-10.28001;12.62151;,
 -28.19300;-11.24178;3.01496;,
 -28.19300;-10.28001;-6.59159;,
 -28.19300;-7.65240;-13.62414;,
 -28.19300;-4.06302;-16.19813;,
 -37.96288;-2.54888;-11.40443;,
 -37.96288;-0.40573;-4.48486;,
 -37.96288;0.37872;4.96743;,
 -37.96288;-0.40573;14.41975;,
 -37.96288;-2.54888;21.33931;,
 -37.96288;-5.47647;23.87201;,
 -37.96288;-8.40406;21.33931;,
 -37.96288;-10.54722;14.41975;,
 -37.96288;-11.33167;4.96743;,
 -37.96288;-10.54722;-4.48486;,
 -37.96288;-8.40406;-11.40443;,
 -37.96288;-5.47647;-13.93720;,
 -49.52370;-4.16166;-4.19298;,
 -48.87473;-2.34424;1.65469;,
 -47.98827;-1.67902;9.64272;,
 -47.10174;-2.34424;17.63077;,
 -46.45280;-4.16166;23.47843;,
 -46.21523;-6.64429;25.61884;,
 -46.45280;-9.12692;23.47843;,
 -47.10174;-10.94433;17.63077;,
 -47.98827;-11.60957;9.64272;,
 -48.87473;-10.94433;1.65469;,
 -49.52370;-9.12692;-4.19296;,
 -49.76125;-6.64429;-6.33338;,
 -61.18463;-6.11390;5.26285;,
 -60.13757;-4.73637;10.10325;,
 -58.70728;-4.23216;16.71535;,
 -57.27703;-4.73637;23.32747;,
 -56.23000;-6.11390;28.16786;,
 -55.84675;-7.99566;29.93955;,
 -56.23000;-9.87742;28.16786;,
 -57.27703;-11.25496;23.32745;,
 -58.70728;-11.75917;16.71535;,
 -60.13756;-11.25496;10.10325;,
 -61.18458;-9.87742;5.26287;,
 -61.56780;-7.99566;3.49115;,
 -71.83154;-7.70438;17.58979;,
 -69.86779;-6.45259;20.84097;,
 -67.18512;-5.99440;25.28214;,
 -64.50244;-6.45259;29.72330;,
 -62.53862;-7.70439;32.97443;,
 -61.81981;-9.41437;34.16444;,
 -62.53862;-11.12433;32.97443;,
 -64.50244;-12.37613;29.72330;,
 -67.18512;-12.83432;25.28210;,
 -69.86775;-12.37613;20.84097;,
 -71.83154;-11.12435;17.58983;,
 -72.55045;-9.41437;16.39981;,
 -79.91742;-8.87388;28.91933;,
 -76.86777;-7.68730;30.50768;,
 -72.70172;-7.25298;32.67745;,
 -68.53574;-7.68729;34.84717;,
 -65.48606;-9.04979;36.43551;,
 -64.36975;-10.49474;37.01692;,
 -65.48606;-11.93969;36.43552;,
 -68.53574;-13.30218;34.84714;,
 -72.70174;-13.73651;32.67742;,
 -76.86777;-13.30219;30.50768;,
 -79.91736;-12.11563;28.91936;,
 -81.03364;-10.49474;28.33796;,
 -81.48499;-10.29904;39.57629;,
 -78.56140;-9.25369;39.63522;,
 -74.56788;-8.87106;39.71567;,
 -70.57426;-9.25368;39.79611;,
 -67.65074;-10.29900;39.85502;,
 -66.58065;-11.72700;39.87654;,
 -67.65074;-13.15493;39.85502;,
 -70.57432;-14.20028;39.79611;,
 -74.56790;-14.58293;39.71567;,
 -78.56140;-14.20032;39.63519;,
 -81.48496;-13.15493;39.57634;,
 -82.55512;-11.72699;39.55474;,
 -80.35781;-11.18535;45.06847;,
 -78.11917;-10.31257;44.81205;,
 -75.06106;-9.99310;44.46182;,
 -72.00293;-10.31256;44.11159;,
 -69.76419;-11.18533;43.85519;,
 -68.94473;-12.37756;43.76133;,
 -69.76419;-13.56978;43.85519;,
 -72.00293;-14.44257;44.11162;,
 -75.06108;-14.76203;44.46184;,
 -78.11917;-14.44257;44.81203;,
 -80.35779;-13.56979;45.06847;,
 -81.17725;-12.37756;45.16233;,
 -78.47326;-12.16767;49.73682;,
 -77.00206;-11.54061;49.30943;,
 -74.99230;-11.31108;48.72557;,
 -72.98257;-11.54060;48.14169;,
 -71.51134;-12.16764;47.71432;,
 -70.97285;-13.02422;47.55785;,
 -71.51134;-13.88079;47.71432;,
 -72.98261;-14.50785;48.14169;,
 -74.99234;-14.73737;48.72559;,
 -77.00208;-14.50787;49.30943;,
 -78.47326;-13.88079;49.73682;,
 -79.01183;-13.02423;49.89331;,
 0.00000;5.69148;-9.08218;,
 0.00000;2.50180;-17.00448;,
 0.00000;6.85898;1.73989;,
 0.00000;5.69148;12.56197;,
 0.00000;2.50180;20.48430;,
 0.00000;-1.85537;23.38406;,
 0.00000;-6.34647;20.48430;,
 0.00000;-9.40223;12.56197;,
 0.00000;-10.56973;1.73989;,
 0.00000;-9.40223;-9.08218;,
 0.00000;-6.21255;-17.00448;,
 0.00000;-1.85537;-19.90428;;
 
 180;
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
 3;24,25,26;,
 3;27,25,24;,
 3;28,25,27;,
 3;29,25,28;,
 3;30,25,29;,
 3;31,25,30;,
 3;32,25,31;,
 3;33,25,32;,
 3;34,25,33;,
 3;35,25,34;,
 3;36,25,35;,
 3;26,25,36;,
 3;37,38,39;,
 3;39,38,40;,
 3;40,38,41;,
 3;41,38,42;,
 3;42,38,43;,
 3;43,38,44;,
 3;44,38,45;,
 3;45,38,46;,
 3;46,38,47;,
 3;47,38,48;,
 3;48,38,49;,
 3;49,38,37;,
 4;1,50,51,2;,
 4;2,51,52,4;,
 4;4,52,53,6;,
 4;6,53,54,8;,
 4;8,54,55,10;,
 4;10,55,56,12;,
 4;12,56,57,14;,
 4;14,57,58,16;,
 4;16,58,59,18;,
 4;18,59,60,20;,
 4;20,60,61,22;,
 4;22,61,50,1;,
 4;50,62,63,51;,
 4;51,63,64,52;,
 4;52,64,65,53;,
 4;53,65,66,54;,
 4;54,66,67,55;,
 4;55,67,68,56;,
 4;56,68,69,57;,
 4;57,69,70,58;,
 4;58,70,71,59;,
 4;59,71,72,60;,
 4;60,72,73,61;,
 4;61,73,62,50;,
 4;62,74,75,63;,
 4;63,75,76,64;,
 4;64,76,77,65;,
 4;65,77,78,66;,
 4;66,78,79,67;,
 4;67,79,80,68;,
 4;68,80,81,69;,
 4;69,81,82,70;,
 4;70,82,83,71;,
 4;71,83,84,72;,
 4;72,84,85,73;,
 4;73,85,74,62;,
 4;74,86,87,75;,
 4;75,87,88,76;,
 4;76,88,89,77;,
 4;77,89,90,78;,
 4;78,90,91,79;,
 4;79,91,92,80;,
 4;80,92,93,81;,
 4;81,93,94,82;,
 4;82,94,95,83;,
 4;83,95,96,84;,
 4;84,96,97,85;,
 4;85,97,86,74;,
 4;86,98,99,87;,
 4;87,99,100,88;,
 4;88,100,101,89;,
 4;89,101,102,90;,
 4;90,102,103,91;,
 4;91,103,104,92;,
 4;92,104,105,93;,
 4;93,105,106,94;,
 4;94,106,107,95;,
 4;95,107,108,96;,
 4;96,108,109,97;,
 4;97,109,98,86;,
 4;98,110,111,99;,
 4;99,111,112,100;,
 4;100,112,113,101;,
 4;101,113,114,102;,
 4;102,114,115,103;,
 4;103,115,116,104;,
 4;104,116,117,105;,
 4;105,117,118,106;,
 4;106,118,119,107;,
 4;107,119,120,108;,
 4;108,120,121,109;,
 4;109,121,110,98;,
 4;110,122,123,111;,
 4;111,123,124,112;,
 4;112,124,125,113;,
 4;113,125,126,114;,
 4;114,126,127,115;,
 4;115,127,128,116;,
 4;116,128,129,117;,
 4;117,129,130,118;,
 4;118,130,131,119;,
 4;119,131,132,120;,
 4;120,132,133,121;,
 4;121,133,122,110;,
 4;122,134,135,123;,
 4;123,135,136,124;,
 4;124,136,137,125;,
 4;125,137,138,126;,
 4;126,138,139,127;,
 4;127,139,140,128;,
 4;128,140,141,129;,
 4;129,141,142,130;,
 4;130,142,143,131;,
 4;131,143,144,132;,
 4;132,144,145,133;,
 4;133,145,134,122;,
 4;134,146,147,135;,
 4;135,147,148,136;,
 4;136,148,149,137;,
 4;137,149,150,138;,
 4;138,150,151,139;,
 4;139,151,152,140;,
 4;140,152,153,141;,
 4;141,153,154,142;,
 4;142,154,155,143;,
 4;143,155,156,144;,
 4;144,156,157,145;,
 4;145,157,146,134;,
 4;146,37,39,147;,
 4;147,39,40,148;,
 4;148,40,41,149;,
 4;149,41,42,150;,
 4;150,42,43,151;,
 4;151,43,44,152;,
 4;152,44,45,153;,
 4;153,45,46,154;,
 4;154,46,47,155;,
 4;155,47,48,156;,
 4;156,48,49,157;,
 4;157,49,37,146;,
 4;3,158,159,0;,
 4;5,160,158,3;,
 4;7,161,160,5;,
 4;9,162,161,7;,
 4;11,163,162,9;,
 4;13,164,163,11;,
 4;15,165,164,13;,
 4;17,166,165,15;,
 4;19,167,166,17;,
 4;21,168,167,19;,
 4;23,169,168,21;,
 4;0,159,169,23;,
 4;158,24,26,159;,
 4;160,27,24,158;,
 4;161,28,27,160;,
 4;162,29,28,161;,
 4;163,30,29,162;,
 4;164,31,30,163;,
 4;165,32,31,164;,
 4;166,33,32,165;,
 4;167,34,33,166;,
 4;168,35,34,167;,
 4;169,36,35,168;,
 4;159,26,36,169;;
 
 MeshMaterialList {
  1;
  180;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\enemy\\Seals_UV.jpg";
   }
  }
 }
 MeshNormals {
  205;
  -0.098199;0.764813;-0.636724;,
  -0.077429;0.965562;-0.248382;,
  -0.070668;0.997500;-0.000000;,
  -0.077429;0.965563;0.248382;,
  -0.098200;0.764812;0.636726;,
  -0.104529;0.539724;0.835328;,
  -0.031941;-0.768284;0.639311;,
  0.009456;-0.968841;0.247502;,
  0.021122;-0.999777;-0.000000;,
  0.011082;-0.968374;-0.249258;,
  -0.029252;-0.768284;-0.639440;,
  -0.056933;-0.541779;-0.838591;,
  -0.264033;0.745610;-0.611844;,
  -0.241300;0.940832;-0.237927;,
  -0.226887;0.973921;0.000179;,
  -0.210610;0.947630;0.240085;,
  -0.182736;0.759621;0.624167;,
  -0.150916;0.543849;0.825501;,
  -0.002220;-0.773585;0.633688;,
  0.016033;-0.969352;0.245151;,
  0.005738;-0.999983;-0.000024;,
  -0.017179;-0.969309;-0.245245;,
  -0.088547;-0.770600;-0.631138;,
  -0.136605;-0.544836;-0.827341;,
  -0.282680;0.765531;-0.577975;,
  -0.275631;0.936870;-0.215179;,
  -0.256872;0.966445;0.000376;,
  -0.206693;0.953366;0.219933;,
  -0.090640;0.793920;0.601228;,
  -0.003829;0.590453;0.807063;,
  0.138087;-0.791544;0.595307;,
  0.071076;-0.972208;0.223067;,
  0.024391;-0.999703;-0.000038;,
  -0.003883;-0.974612;-0.223865;,
  -0.063176;-0.797339;-0.600216;,
  -0.105378;-0.587117;-0.802614;,
  -0.351662;0.781207;-0.515799;,
  -0.273674;0.943987;-0.184368;,
  -0.229096;0.973386;0.005930;,
  -0.170666;0.964597;0.201064;,
  -0.045440;0.824641;0.563829;,
  0.045454;0.636799;0.769689;,
  0.181097;-0.816540;0.548148;,
  0.090072;-0.976542;0.195585;,
  0.023189;-0.999731;-0.000879;,
  -0.039729;-0.979488;-0.197546;,
  -0.169607;-0.820372;-0.546098;,
  -0.252369;-0.620729;-0.742297;,
  -0.437904;0.782134;-0.443290;,
  -0.280320;0.948423;-0.148036;,
  -0.201472;0.979226;0.022911;,
  -0.123556;0.972238;0.198715;,
  0.033201;0.842998;0.536891;,
  0.141644;0.664557;0.733690;,
  0.247178;-0.829380;0.501031;,
  0.116488;-0.978087;0.172558;,
  0.022587;-0.999742;-0.002235;,
  -0.081689;-0.980848;-0.176816;,
  -0.294834;-0.822639;-0.486146;,
  -0.423381;-0.622933;-0.657801;,
  -0.490116;0.787249;-0.374199;,
  -0.284246;0.952580;-0.108610;,
  -0.178452;0.982810;0.047319;,
  -0.076375;0.974649;0.210300;,
  0.134617;0.844206;0.518839;,
  0.276136;0.664944;0.693973;,
  0.370636;-0.822921;0.430616;,
  0.182829;-0.974122;0.132894;,
  0.053706;-0.998360;-0.019809;,
  -0.085670;-0.981646;-0.170386;,
  -0.352278;-0.826592;-0.438915;,
  -0.510616;-0.628085;-0.587180;,
  -0.518958;0.795331;-0.313260;,
  -0.257839;0.963753;-0.068551;,
  -0.121660;0.989070;0.083300;,
  0.002666;0.967551;0.252661;,
  0.242913;0.817406;0.522342;,
  0.396574;0.640297;0.657836;,
  0.495880;-0.804666;0.326521;,
  0.252663;-0.964851;0.072276;,
  0.079326;-0.995207;-0.057182;,
  -0.098267;-0.978865;-0.179353;,
  -0.411824;-0.817686;-0.402232;,
  -0.591458;-0.614746;-0.521790;,
  -0.575400;0.806561;-0.135551;,
  -0.250372;0.967412;0.037778;,
  -0.078070;0.983942;0.160509;,
  0.088006;0.945061;0.314824;,
  0.357853;0.767207;0.532292;,
  0.511895;0.586598;0.627587;,
  0.564767;-0.801884;0.194987;,
  0.262036;-0.964930;-0.015732;,
  0.048631;-0.994199;-0.095930;,
  -0.157169;-0.974438;-0.160523;,
  -0.511559;-0.812593;-0.279285;,
  -0.714546;-0.611310;-0.340183;,
  -0.600165;0.788552;0.134119;,
  -0.234278;0.957264;0.169583;,
  -0.026255;0.974185;0.224219;,
  0.178417;0.929178;0.323723;,
  0.482587;0.727001;0.488446;,
  0.641423;0.524445;0.559941;,
  0.587354;-0.790169;0.175064;,
  0.243180;-0.969689;-0.023815;,
  0.014099;-0.997137;-0.074288;,
  -0.210848;-0.974202;-0.080461;,
  -0.592938;-0.803539;-0.052430;,
  -0.803111;-0.595345;-0.024029;,
  -0.562970;0.764690;0.313551;,
  -0.194759;0.945841;0.259718;,
  0.033628;0.963794;0.264521;,
  0.258918;0.910653;0.321983;,
  0.579994;0.686516;0.438523;,
  0.856059;0.168702;0.488573;,
  0.610977;-0.758458;0.226821;,
  0.240419;-0.969975;0.036697;,
  0.000123;-0.999917;-0.012852;,
  -0.232444;-0.972556;0.010204;,
  -0.604012;-0.786997;0.125723;,
  -0.796755;-0.568863;0.203904;,
  -0.454715;0.724382;0.518174;,
  -0.112901;0.904366;0.411552;,
  0.110581;0.916074;0.385462;,
  0.331388;0.851263;0.406859;,
  0.632185;0.623315;0.460240;,
  0.900867;-0.034038;0.432760;,
  0.646477;-0.736700;0.198345;,
  0.261043;-0.964136;0.047935;,
  0.011449;-0.999334;0.034661;,
  -0.218669;-0.970554;0.101039;,
  -0.556271;-0.775046;0.299776;,
  -0.723109;-0.547485;0.421157;,
  0.376576;-0.000098;0.926386;,
  0.376609;-0.000061;0.926372;,
  0.376578;0.000002;0.926385;,
  0.378781;0.319239;0.868684;,
  0.420386;0.450190;0.787785;,
  0.556665;0.313514;0.769307;,
  0.677406;-0.021637;0.735291;,
  0.591671;-0.175530;0.786838;,
  0.376575;0.000040;0.926386;,
  0.376588;0.000043;0.926381;,
  0.376585;0.000068;0.926382;,
  0.376575;0.000065;0.926386;,
  0.376581;-0.000019;0.926384;,
  0.088237;0.775299;-0.625401;,
  0.085602;0.967085;-0.239621;,
  0.087918;0.996128;-0.000000;,
  0.085602;0.967085;0.239621;,
  0.088238;0.775297;0.625402;,
  0.086473;0.556235;0.826514;,
  0.085854;-0.775535;0.625439;,
  0.083739;-0.968570;0.234222;,
  0.087917;-0.996128;-0.000000;,
  0.085602;-0.967085;-0.239621;,
  0.088237;-0.775298;-0.625401;,
  0.086471;-0.556238;-0.826512;,
  1.000000;0.000000;-0.000000;,
  -0.058780;-0.538786;0.840390;,
  -0.023145;-0.549854;0.834940;,
  -0.104528;0.539727;-0.835325;,
  -0.260031;0.531079;-0.806436;,
  0.164286;-0.582213;0.796265;,
  -0.266495;0.568927;-0.778012;,
  0.220785;-0.624105;0.749497;,
  -0.384081;0.593669;-0.707134;,
  0.310656;-0.647428;0.695938;,
  -0.520916;0.593503;-0.613515;,
  0.463841;-0.641184;0.611339;,
  -0.601854;0.598619;-0.528609;,
  0.613198;-0.625683;0.482192;,
  -0.661279;0.597950;-0.452952;,
  0.708231;-0.619619;0.338350;,
  -0.758239;0.606795;-0.238482;,
  0.752187;-0.577758;0.316877;,
  -0.803502;0.585497;0.107603;,
  0.763490;-0.536405;0.359657;,
  -0.761352;0.555632;0.334091;,
  -0.641519;0.520791;0.563232;,
  -0.355729;0.679439;0.641731;,
  -0.046915;0.856902;0.513339;,
  0.067720;0.875880;0.477753;,
  0.406997;-0.911476;0.059717;,
  0.268580;-0.962097;0.047265;,
  0.021198;-0.997922;0.060853;,
  -0.199788;-0.967286;0.156339;,
  -0.503321;-0.762674;0.406198;,
  -0.644976;-0.529361;0.551165;,
  -0.525943;0.486510;0.697633;,
  0.084910;-0.547160;0.832710;,
  0.086471;0.556238;-0.826512;,
  0.110613;0.966304;-0.232425;,
  0.113784;0.780976;-0.614109;,
  0.113770;0.993507;-0.000000;,
  0.110613;0.966304;0.232425;,
  0.113784;0.780975;0.614110;,
  0.111553;0.566590;0.816414;,
  0.114233;-0.780977;0.614024;,
  0.112121;-0.554135;0.824842;,
  0.110133;-0.968096;0.225079;,
  0.113769;-0.993507;-0.000000;,
  0.110613;-0.966304;-0.232425;,
  0.113784;-0.780975;-0.614110;,
  0.111552;-0.566591;-0.816413;,
  0.111552;0.566591;-0.816413;;
  180;
  4;0,12,13,1;,
  4;1,13,14,2;,
  4;2,14,15,3;,
  4;3,15,16,4;,
  4;4,16,17,5;,
  4;158,159,18,6;,
  4;6,18,19,7;,
  4;7,19,20,8;,
  4;8,20,21,9;,
  4;9,21,22,10;,
  4;10,22,23,11;,
  4;160,161,12,0;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;157,157,157;,
  3;132,134,133;,
  3;133,134,135;,
  3;135,134,136;,
  3;136,134,137;,
  3;137,134,138;,
  3;138,134,139;,
  3;139,134,140;,
  3;140,134,141;,
  3;141,134,142;,
  3;142,134,143;,
  3;143,134,144;,
  3;144,134,132;,
  4;12,24,25,13;,
  4;13,25,26,14;,
  4;14,26,27,15;,
  4;15,27,28,16;,
  4;16,28,29,17;,
  4;159,162,30,18;,
  4;18,30,31,19;,
  4;19,31,32,20;,
  4;20,32,33,21;,
  4;21,33,34,22;,
  4;22,34,35,23;,
  4;161,163,24,12;,
  4;24,36,37,25;,
  4;25,37,38,26;,
  4;26,38,39,27;,
  4;27,39,40,28;,
  4;28,40,41,29;,
  4;162,164,42,30;,
  4;30,42,43,31;,
  4;31,43,44,32;,
  4;32,44,45,33;,
  4;33,45,46,34;,
  4;34,46,47,35;,
  4;163,165,36,24;,
  4;36,48,49,37;,
  4;37,49,50,38;,
  4;38,50,51,39;,
  4;39,51,52,40;,
  4;40,52,53,41;,
  4;164,166,54,42;,
  4;42,54,55,43;,
  4;43,55,56,44;,
  4;44,56,57,45;,
  4;45,57,58,46;,
  4;46,58,59,47;,
  4;165,167,48,36;,
  4;48,60,61,49;,
  4;49,61,62,50;,
  4;50,62,63,51;,
  4;51,63,64,52;,
  4;52,64,65,53;,
  4;166,168,66,54;,
  4;54,66,67,55;,
  4;55,67,68,56;,
  4;56,68,69,57;,
  4;57,69,70,58;,
  4;58,70,71,59;,
  4;167,169,60,48;,
  4;60,72,73,61;,
  4;61,73,74,62;,
  4;62,74,75,63;,
  4;63,75,76,64;,
  4;64,76,77,65;,
  4;168,170,78,66;,
  4;66,78,79,67;,
  4;67,79,80,68;,
  4;68,80,81,69;,
  4;69,81,82,70;,
  4;70,82,83,71;,
  4;169,171,72,60;,
  4;72,84,85,73;,
  4;73,85,86,74;,
  4;74,86,87,75;,
  4;75,87,88,76;,
  4;76,88,89,77;,
  4;170,172,90,78;,
  4;78,90,91,79;,
  4;79,91,92,80;,
  4;80,92,93,81;,
  4;81,93,94,82;,
  4;82,94,95,83;,
  4;171,173,84,72;,
  4;84,96,97,85;,
  4;85,97,98,86;,
  4;86,98,99,87;,
  4;87,99,100,88;,
  4;88,100,101,89;,
  4;172,174,102,90;,
  4;90,102,103,91;,
  4;91,103,104,92;,
  4;92,104,105,93;,
  4;93,105,106,94;,
  4;94,106,107,95;,
  4;173,175,96,84;,
  4;96,108,109,97;,
  4;97,109,110,98;,
  4;98,110,111,99;,
  4;99,111,112,100;,
  4;100,112,113,101;,
  4;174,176,114,102;,
  4;102,114,115,103;,
  4;103,115,116,104;,
  4;104,116,117,105;,
  4;105,117,118,106;,
  4;106,118,119,107;,
  4;175,177,108,96;,
  4;108,120,121,109;,
  4;109,121,122,110;,
  4;110,122,123,111;,
  4;111,123,124,112;,
  4;112,124,125,113;,
  4;113,125,126,114;,
  4;114,126,127,115;,
  4;115,127,128,116;,
  4;116,128,129,117;,
  4;117,129,130,118;,
  4;118,130,131,119;,
  4;177,178,120,108;,
  4;120,179,180,121;,
  4;121,180,181,122;,
  4;122,135,136,123;,
  4;123,136,137,124;,
  4;124,137,138,125;,
  4;125,138,139,126;,
  4;126,182,183,127;,
  4;127,183,184,128;,
  4;128,184,185,129;,
  4;129,185,186,130;,
  4;130,186,187,131;,
  4;178,188,179,120;,
  4;1,146,145,0;,
  4;2,147,146,1;,
  4;3,148,147,2;,
  4;4,149,148,3;,
  4;5,150,149,4;,
  4;6,151,189,158;,
  4;7,152,151,6;,
  4;8,153,152,7;,
  4;9,154,153,8;,
  4;10,155,154,9;,
  4;11,156,155,10;,
  4;0,145,190,160;,
  4;146,191,192,145;,
  4;147,193,191,146;,
  4;148,194,193,147;,
  4;149,195,194,148;,
  4;150,196,195,149;,
  4;151,197,198,189;,
  4;152,199,197,151;,
  4;153,200,199,152;,
  4;154,201,200,153;,
  4;155,202,201,154;,
  4;156,203,202,155;,
  4;145,192,204,190;;
 }
 MeshTextureCoords {
  170;
  0.398680;0.751550;,
  0.389550;0.770470;,
  0.359010;0.729100;,
  0.364280;0.704950;,
  0.317300;0.713950;,
  0.317300;0.687890;,
  0.275580;0.729100;,
  0.270310;0.704950;,
  0.245050;0.770470;,
  0.235920;0.751550;,
  0.233870;0.827000;,
  0.223330;0.815210;,
  0.245050;0.883520;,
  0.235920;0.878880;,
  0.275580;0.924900;,
  0.270310;0.925490;,
  0.317300;0.940040;,
  0.317300;0.942540;,
  0.359010;0.924900;,
  0.364280;0.925490;,
  0.389550;0.883520;,
  0.398680;0.878880;,
  0.400730;0.827000;,
  0.411260;0.815210;,
  0.360440;0.725690;,
  0.317300;0.815210;,
  0.392020;0.763530;,
  0.317300;0.711840;,
  0.274160;0.725690;,
  0.242580;0.763530;,
  0.231020;0.815210;,
  0.242580;0.868730;,
  0.274160;0.904740;,
  0.317300;0.918590;,
  0.360440;0.904740;,
  0.392020;0.866900;,
  0.403580;0.815210;,
  0.108660;0.964650;,
  0.112920;0.972900;,
  0.110460;0.958610;,
  0.112920;0.956400;,
  0.115380;0.958610;,
  0.117180;0.964650;,
  0.117840;0.972900;,
  0.117180;0.981150;,
  0.115380;0.987190;,
  0.112920;0.989400;,
  0.110460;0.987190;,
  0.108660;0.981150;,
  0.108000;0.972900;,
  0.382050;0.796380;,
  0.352410;0.760560;,
  0.311920;0.747450;,
  0.271440;0.760560;,
  0.241800;0.796380;,
  0.230950;0.845310;,
  0.241800;0.894240;,
  0.271440;0.930060;,
  0.311920;0.943170;,
  0.352410;0.930060;,
  0.382050;0.894240;,
  0.392900;0.845310;,
  0.372700;0.824670;,
  0.343530;0.795450;,
  0.303690;0.784760;,
  0.263860;0.795450;,
  0.234690;0.824670;,
  0.224020;0.864580;,
  0.234690;0.904490;,
  0.263860;0.933700;,
  0.303690;0.944390;,
  0.343530;0.933700;,
  0.372700;0.904490;,
  0.383370;0.864580;,
  0.342300;0.846650;,
  0.317660;0.821880;,
  0.283990;0.812810;,
  0.250320;0.821880;,
  0.225680;0.846650;,
  0.216660;0.880500;,
  0.225680;0.914340;,
  0.250320;0.939110;,
  0.283990;0.948180;,
  0.317660;0.939110;,
  0.342300;0.914340;,
  0.351320;0.880500;,
  0.302450;0.873270;,
  0.282050;0.854490;,
  0.254180;0.847620;,
  0.226320;0.854490;,
  0.205920;0.873270;,
  0.198450;0.898920;,
  0.205920;0.924570;,
  0.226320;0.943350;,
  0.254180;0.950220;,
  0.282050;0.943350;,
  0.302450;0.924570;,
  0.309920;0.898920;,
  0.250500;0.894950;,
  0.236790;0.877880;,
  0.218080;0.871640;,
  0.199360;0.877880;,
  0.185660;0.894950;,
  0.180640;0.918260;,
  0.185660;0.941570;,
  0.199360;0.958630;,
  0.218080;0.964880;,
  0.236790;0.958630;,
  0.250500;0.941570;,
  0.255510;0.918260;,
  0.202750;0.910890;,
  0.196050;0.894720;,
  0.186910;0.888790;,
  0.177760;0.894720;,
  0.171070;0.913290;,
  0.168620;0.932990;,
  0.171070;0.952680;,
  0.177760;0.971260;,
  0.186910;0.977180;,
  0.196050;0.971260;,
  0.202750;0.955080;,
  0.205200;0.932990;,
  0.157830;0.930320;,
  0.157580;0.916070;,
  0.157240;0.910850;,
  0.156910;0.916070;,
  0.156660;0.930320;,
  0.156570;0.949780;,
  0.156660;0.969250;,
  0.156910;0.983500;,
  0.157240;0.988710;,
  0.157580;0.983500;,
  0.157830;0.969250;,
  0.157920;0.949780;,
  0.134680;0.942400;,
  0.135770;0.930500;,
  0.137240;0.926150;,
  0.138720;0.930500;,
  0.139800;0.942400;,
  0.140190;0.958650;,
  0.139800;0.974900;,
  0.138720;0.986800;,
  0.137240;0.991160;,
  0.135770;0.986800;,
  0.134680;0.974900;,
  0.134290;0.958650;,
  0.115010;0.955790;,
  0.116810;0.947240;,
  0.119270;0.944110;,
  0.121730;0.947240;,
  0.123530;0.955790;,
  0.124190;0.967470;,
  0.123530;0.979140;,
  0.121730;0.987690;,
  0.119270;0.990820;,
  0.116810;0.987690;,
  0.115010;0.979140;,
  0.114350;0.967470;,
  0.362910;0.712340;,
  0.396300;0.755820;,
  0.317300;0.696420;,
  0.271690;0.712340;,
  0.238300;0.755820;,
  0.226080;0.815210;,
  0.238300;0.876440;,
  0.271690;0.918090;,
  0.317300;0.934010;,
  0.362910;0.918090;,
  0.396300;0.874610;,
  0.408520;0.815210;;
 }
}