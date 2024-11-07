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
 114;
 5.73067;-7.29458;18.26151;,
 4.91034;-6.07392;9.50568;,
 5.28687;0.00000;9.50568;,
 6.17012;0.00000;18.26151;,
 4.46534;-3.72234;1.61509;,
 4.80774;0.00000;1.61509;,
 5.73067;7.29456;18.26151;,
 4.91034;6.07390;9.50568;,
 4.11167;12.14782;9.50568;,
 4.79857;14.58914;18.26151;,
 4.46534;3.72233;1.61509;,
 3.73904;7.44466;1.61509;,
 0.00000;15.69066;17.65089;,
 0.00000;13.06501;8.23400;,
 -4.11167;12.14782;9.50568;,
 -4.79856;14.58914;18.26151;,
 0.00000;8.00676;-0.25236;,
 -3.73904;7.44466;1.61509;,
 -4.91034;6.07390;9.50568;,
 -5.73067;7.29456;18.26151;,
 -4.46534;3.72233;1.61509;,
 -6.17010;0.00000;18.26151;,
 -5.28687;0.00000;9.50568;,
 -4.91034;-6.07392;9.50568;,
 -5.73067;-7.29458;18.26151;,
 -4.80774;0.00000;1.61509;,
 -4.46534;-3.72234;1.61509;,
 -4.79856;-14.58915;18.26151;,
 -4.11167;-12.14782;9.50568;,
 0.00000;-13.06503;8.23400;,
 0.00000;-15.69066;17.65089;,
 -3.73904;-7.44466;1.61509;,
 0.00000;-8.00676;-0.25236;,
 0.00000;0.00000;-3.88077;,
 0.00000;-2.17203;-3.87852;,
 -2.98862;-2.02187;-2.77940;,
 -3.11870;0.00000;-2.79975;,
 0.00000;4.24608;-3.70376;,
 0.00000;2.17203;-3.87852;,
 -2.98862;2.02187;-2.77940;,
 -2.66433;3.96173;-2.75907;,
 -5.65390;-11.54862;45.98366;,
 0.04740;-12.42057;44.04841;,
 0.00000;0.00000;41.83814;,
 -6.09108;0.00000;43.92853;,
 -5.65390;11.54862;45.98366;,
 0.04740;12.42057;44.04841;,
 0.04740;24.84114;49.53279;,
 -4.72658;23.09724;51.08297;,
 0.00000;-18.59268;28.27421;,
 -4.92232;-17.28742;27.74299;,
 5.87849;-8.64372;25.84286;,
 6.32925;0.00000;25.84286;,
 -5.87849;-8.64372;25.84286;,
 -6.32925;0.00000;25.84286;,
 5.87849;8.64371;25.84286;,
 4.92234;17.28742;27.74299;,
 0.00000;18.59267;28.27421;,
 -4.92232;17.28742;27.74299;,
 -5.87849;8.64371;25.84286;,
 0.00000;-21.75252;36.63471;,
 -4.63815;-20.22545;35.78288;,
 5.53913;-10.11273;33.02355;,
 5.96387;0.00000;33.02355;,
 -5.53911;-10.11273;33.02355;,
 -5.96385;0.00000;33.02355;,
 5.53913;10.11272;33.02355;,
 4.63817;20.22543;35.78288;,
 0.00000;21.75252;36.63471;,
 -4.63815;20.22543;35.78288;,
 -5.53911;10.11272;33.02355;,
 0.00000;-22.61604;41.50707;,
 -4.67456;-21.12819;40.92591;,
 5.58258;-10.56409;38.16654;,
 6.01065;0.00000;38.16654;,
 -5.58256;-10.56409;38.16654;,
 -6.01065;0.00000;38.16654;,
 5.58258;10.56409;38.16654;,
 4.67456;21.12818;40.92591;,
 0.00000;22.72341;42.02646;,
 -4.67456;21.12818;40.92591;,
 -5.58256;10.56409;38.16654;,
 0.04740;-24.84114;49.53279;,
 -4.72658;-23.09726;51.08297;,
 5.74869;-11.54862;45.98366;,
 4.80774;0.00000;43.92853;,
 5.74869;11.54862;45.98366;,
 4.82136;23.09724;51.08297;,
 4.79857;-14.58915;18.26151;,
 4.11167;-12.14782;9.50568;,
 3.73904;-7.44466;1.61509;,
 0.00000;-4.24608;-3.70376;,
 -2.66433;-3.96173;-2.75907;,
 4.67456;-21.12819;40.92591;,
 4.82136;-23.09726;51.08297;,
 4.63817;-20.22545;35.78288;,
 4.92234;-17.28742;27.74299;,
 2.98862;2.02187;-2.77940;,
 2.66433;3.96174;-2.75905;,
 3.11871;0.00000;-2.79975;,
 2.98862;-2.02187;-2.77940;,
 2.66433;-3.96174;-2.75905;,
 -3.64431;0.00000;0.00001;,
 -3.38476;-2.82156;0.00001;,
 -2.83422;5.64312;0.00001;,
 -3.38476;2.82156;0.00001;,
 0.00000;6.06919;-1.41557;,
 -2.83422;-5.64314;0.00001;,
 0.00000;-6.06919;-1.41557;,
 2.83424;5.64312;0.00001;,
 3.38476;2.82156;0.00001;,
 3.64432;0.00000;0.00001;,
 3.38476;-2.82156;0.00001;,
 2.83424;-5.64314;0.00001;;
 
 112;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;6,7,8,9;,
 4;7,10,11,8;,
 4;12,13,14,15;,
 4;13,16,17,14;,
 4;15,14,18,19;,
 4;14,17,20,18;,
 4;21,22,23,24;,
 4;22,25,26,23;,
 4;27,28,29,30;,
 4;28,31,32,29;,
 4;33,34,35,36;,
 4;37,38,39,40;,
 4;41,42,43,44;,
 4;45,46,47,48;,
 4;27,30,49,50;,
 4;51,0,3,52;,
 4;21,24,53,54;,
 4;55,6,9,56;,
 4;57,12,15,58;,
 4;15,19,59,58;,
 4;50,49,60,61;,
 4;62,51,52,63;,
 4;54,53,64,65;,
 4;66,55,56,67;,
 4;68,57,58,69;,
 4;58,59,70,69;,
 4;61,60,71,72;,
 4;73,62,63,74;,
 4;65,64,75,76;,
 4;77,66,67,78;,
 4;79,68,69,80;,
 4;69,70,81,80;,
 4;72,71,82,83;,
 4;84,73,74,85;,
 4;76,75,41,44;,
 4;86,77,78,87;,
 4;47,79,80,48;,
 4;80,81,45,48;,
 4;3,2,7,6;,
 4;2,5,10,7;,
 4;38,33,36,39;,
 4;18,20,25,22;,
 4;19,18,22,21;,
 4;59,19,21,54;,
 4;70,59,54,65;,
 4;81,70,65,76;,
 4;45,81,76,44;,
 4;44,43,46,45;,
 4;74,77,86,85;,
 4;63,66,77,74;,
 4;52,55,66,63;,
 4;3,6,55,52;,
 4;88,89,1,0;,
 4;89,90,4,1;,
 4;34,91,92,35;,
 4;23,26,31,28;,
 4;24,23,28,27;,
 4;53,24,27,50;,
 4;64,53,50,61;,
 4;75,64,61,72;,
 4;41,75,72,83;,
 4;83,82,42,41;,
 4;93,73,84,94;,
 4;95,62,73,93;,
 4;96,51,62,95;,
 4;88,0,51,96;,
 4;9,8,13,12;,
 4;8,11,16,13;,
 4;97,38,37,98;,
 4;99,33,38,97;,
 4;100,34,33,99;,
 4;101,91,34,100;,
 4;29,32,90,89;,
 4;30,29,89,88;,
 4;49,30,88,96;,
 4;60,49,96,95;,
 4;71,60,95,93;,
 4;82,71,93,94;,
 4;42,82,94,84;,
 4;43,42,84,85;,
 4;46,43,85,86;,
 4;47,46,86,87;,
 4;78,79,47,87;,
 4;67,68,79,78;,
 4;56,57,68,67;,
 4;9,12,57,56;,
 4;26,25,102,103;,
 4;20,17,104,105;,
 4;17,16,106,104;,
 4;25,20,105,102;,
 4;32,31,107,108;,
 4;31,26,103,107;,
 4;16,11,109,106;,
 4;11,10,110,109;,
 4;10,5,111,110;,
 4;5,4,112,111;,
 4;90,32,108,113;,
 4;4,90,113,112;,
 4;103,102,36,35;,
 4;105,104,40,39;,
 4;104,106,37,40;,
 4;102,105,39,36;,
 4;108,107,92,91;,
 4;107,103,35,92;,
 4;106,109,98,37;,
 4;109,110,97,98;,
 4;110,111,99,97;,
 4;111,112,100,99;,
 4;113,108,91,101;,
 4;112,113,101,100;;
 
 MeshMaterialList {
  1;
  112;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  155;
  0.989007;-0.125624;-0.077993;,
  0.754474;-0.589913;-0.287701;,
  0.711391;-0.437901;-0.549695;,
  0.998016;0.000000;-0.062960;,
  0.996438;0.000000;-0.084326;,
  0.940737;0.000000;-0.339137;,
  0.989007;0.125624;-0.077993;,
  0.870954;0.410829;-0.269553;,
  0.711391;0.437901;-0.549695;,
  -0.250944;0.930947;-0.265264;,
  -0.309033;0.871694;-0.380327;,
  -0.711391;0.437901;-0.549696;,
  -0.998016;0.000000;-0.062960;,
  -0.996438;0.000000;-0.084325;,
  -0.940737;0.000000;-0.339138;,
  -0.250944;-0.930947;-0.265264;,
  -0.309034;-0.871693;-0.380327;,
  -0.711391;-0.437901;-0.549696;,
  -0.228796;-0.926186;-0.299721;,
  0.993840;-0.109796;-0.015068;,
  0.999951;0.000000;0.009940;,
  -0.999951;0.000000;0.009941;,
  0.993840;0.109796;-0.015068;,
  -0.228796;0.926185;-0.299721;,
  -0.252356;-0.933951;-0.253085;,
  0.995541;-0.094287;-0.002747;,
  0.999854;0.000000;0.017092;,
  -0.999854;0.000000;0.017092;,
  0.995541;0.094287;-0.002747;,
  -0.252884;0.933641;-0.253700;,
  -0.303766;-0.933062;-0.192671;,
  0.995287;-0.093326;-0.026347;,
  0.999056;0.000000;0.043436;,
  -0.999924;0.000000;-0.012323;,
  0.995287;0.093326;-0.026347;,
  -0.311487;0.930538;-0.192550;,
  -0.326943;-0.919702;-0.217385;,
  0.995009;-0.094233;-0.032834;,
  0.995259;0.000000;0.097260;,
  0.334543;0.000000;0.942381;,
  0.995009;0.094233;-0.032834;,
  0.377762;-0.385690;0.841747;,
  0.993255;0.092086;-0.070466;,
  0.989345;0.106343;-0.099435;,
  0.926209;0.120357;-0.357283;,
  -0.926209;0.120357;-0.357284;,
  -0.989345;0.106343;-0.099435;,
  -0.993255;0.092087;-0.070466;,
  -0.996733;0.080722;-0.002572;,
  -0.997575;0.069230;0.007165;,
  -0.997674;0.066135;-0.016510;,
  0.356143;-0.280750;0.891258;,
  0.998782;0.037283;0.032325;,
  0.998605;0.052108;0.008523;,
  0.997575;0.069230;0.007165;,
  0.996733;0.080722;-0.002572;,
  0.993255;-0.092086;-0.070466;,
  0.989345;-0.106343;-0.099435;,
  0.926209;-0.120357;-0.357282;,
  -0.926209;-0.120357;-0.357284;,
  -0.989345;-0.106343;-0.099435;,
  -0.993255;-0.092087;-0.070466;,
  -0.996733;-0.080722;-0.002572;,
  -0.997575;-0.069230;0.007165;,
  -0.997674;-0.066135;-0.016510;,
  0.356143;0.280750;0.891258;,
  0.998782;-0.037283;0.032325;,
  0.998605;-0.052108;0.008523;,
  0.997575;-0.069230;0.007165;,
  0.996733;-0.080722;-0.002572;,
  -0.000000;0.961720;-0.274032;,
  -0.000000;0.916558;-0.399902;,
  -0.000000;0.710071;-0.704130;,
  -0.000000;-0.710071;-0.704130;,
  -0.000000;-0.916558;-0.399902;,
  -0.000000;-0.961721;-0.274032;,
  -0.000000;-0.951423;-0.307888;,
  -0.000000;-0.965190;-0.261550;,
  0.000026;-0.979163;-0.203075;,
  0.000053;-0.972781;-0.231725;,
  -0.009758;0.295539;0.955281;,
  -0.019521;0.000000;0.999809;,
  -0.009758;-0.295539;0.955281;,
  -0.000000;-0.416556;0.909110;,
  0.000033;0.979057;-0.203587;,
  -0.000000;0.965007;-0.262223;,
  -0.000000;0.951423;-0.307888;,
  -0.909470;-0.123513;-0.396999;,
  -0.920902;0.000000;-0.389794;,
  -0.000001;0.682343;-0.731033;,
  -0.909470;0.123513;-0.396999;,
  -0.695327;0.427606;-0.577646;,
  -0.000001;-0.682343;-0.731032;,
  -0.695326;-0.427606;-0.577646;,
  0.909471;0.123514;-0.396998;,
  0.695327;0.427605;-0.577645;,
  0.920902;0.000000;-0.389794;,
  0.909471;-0.123514;-0.396998;,
  0.695327;-0.427606;-0.577645;,
  -0.000000;0.000000;-1.000000;,
  0.000000;-0.047443;-0.998874;,
  -0.340313;-0.044610;-0.939253;,
  -0.336377;0.000000;-0.941727;,
  0.000000;0.492683;-0.870209;,
  0.000000;0.047443;-0.998874;,
  -0.340313;0.044610;-0.939253;,
  -0.643818;0.398883;-0.652986;,
  0.000000;-0.492682;-0.870209;,
  -0.643817;-0.398882;-0.652987;,
  0.340314;0.044612;-0.939253;,
  0.643818;0.398883;-0.652986;,
  0.336376;0.000000;-0.941728;,
  0.340314;-0.044612;-0.939253;,
  0.643817;-0.398883;-0.652987;,
  -0.989007;0.125625;-0.077993;,
  -0.982943;0.143859;-0.114576;,
  -0.993840;0.109798;-0.015067;,
  -0.995541;0.094287;-0.002748;,
  -0.995598;0.091418;-0.020697;,
  -0.997698;-0.065429;-0.017813;,
  -0.999901;0.000000;-0.014093;,
  0.000067;0.972827;-0.231531;,
  -0.341192;0.914879;-0.215832;,
  -0.997698;0.065429;-0.017813;,
  -0.995671;0.090421;-0.021534;,
  -0.982943;-0.143860;-0.114576;,
  -0.989007;-0.125625;-0.077993;,
  -0.993840;-0.109798;-0.015067;,
  -0.995541;-0.094287;-0.002748;,
  -0.995598;-0.091418;-0.020697;,
  -0.995671;-0.090421;-0.021534;,
  0.377762;0.385690;0.841748;,
  -0.000001;0.416556;0.909110;,
  0.250944;0.930947;-0.265264;,
  0.267026;0.925716;-0.267856;,
  0.250944;-0.930947;-0.265264;,
  0.228795;-0.926186;-0.299721;,
  0.252355;-0.933951;-0.253085;,
  0.303831;-0.932706;-0.194282;,
  0.327043;-0.918904;-0.220587;,
  -0.377763;0.385690;0.841747;,
  -0.374614;0.269444;0.887166;,
  -0.369755;0.000000;0.929129;,
  -0.374615;-0.269444;0.887166;,
  -0.377763;-0.385690;0.841747;,
  0.311566;0.930135;-0.194362;,
  0.341318;0.913976;-0.219432;,
  0.252884;0.933641;-0.253700;,
  0.228795;0.926186;-0.299721;,
  -0.984464;0.000000;-0.175589;,
  -0.975844;-0.127113;-0.177681;,
  -0.975844;0.127113;-0.177681;,
  0.975844;0.127115;-0.177681;,
  0.984463;0.000000;-0.175590;,
  0.975844;-0.127115;-0.177681;;
  112;
  4;56,57,4,3;,
  4;57,58,5,4;,
  4;42,43,7,6;,
  4;43,44,8,7;,
  4;70,71,10,9;,
  4;71,72,11,10;,
  4;114,115,46,47;,
  4;115,11,45,46;,
  4;12,13,60,61;,
  4;13,14,59,60;,
  4;15,16,74,75;,
  4;16,17,73,74;,
  4;99,100,101,102;,
  4;103,104,105,106;,
  4;65,80,81,39;,
  4;51,82,83,41;,
  4;15,75,76,18;,
  4;69,56,3,20;,
  4;12,61,62,21;,
  4;55,42,6,22;,
  4;86,70,9,23;,
  4;114,47,48,116;,
  4;18,76,77,24;,
  4;68,69,20,26;,
  4;21,62,63,27;,
  4;54,55,22,28;,
  4;85,86,23,29;,
  4;116,48,49,117;,
  4;24,77,78,30;,
  4;67,68,26,32;,
  4;27,63,64,33;,
  4;53,54,28,34;,
  4;84,85,29,35;,
  4;117,49,50,118;,
  4;30,78,79,36;,
  4;66,67,32,38;,
  4;33,64,119,120;,
  4;52,53,34,40;,
  4;121,84,35,122;,
  4;118,50,123,124;,
  4;3,4,43,42;,
  4;4,5,44,43;,
  4;104,99,102,105;,
  4;46,45,14,13;,
  4;47,46,13,12;,
  4;48,47,12,21;,
  4;49,48,21,27;,
  4;50,49,27,33;,
  4;123,50,33,120;,
  4;39,81,82,51;,
  4;32,53,52,38;,
  4;26,54,53,32;,
  4;20,55,54,26;,
  4;3,42,55,20;,
  4;0,1,57,56;,
  4;1,2,58,57;,
  4;100,107,108,101;,
  4;60,59,17,125;,
  4;61,60,125,126;,
  4;62,61,126,127;,
  4;63,62,127,128;,
  4;64,63,128,129;,
  4;119,64,129,130;,
  4;131,132,80,65;,
  4;31,67,66,37;,
  4;25,68,67,31;,
  4;19,69,68,25;,
  4;0,56,69,19;,
  4;133,134,71,70;,
  4;7,8,72,71;,
  4;109,104,103,110;,
  4;111,99,104,109;,
  4;112,100,99,111;,
  4;113,107,100,112;,
  4;74,73,2,1;,
  4;75,74,1,135;,
  4;76,75,135,136;,
  4;77,76,136,137;,
  4;78,77,137,138;,
  4;79,78,138,139;,
  4;80,132,140,141;,
  4;81,80,141,142;,
  4;82,81,142,143;,
  4;83,82,143,144;,
  4;145,84,121,146;,
  4;147,85,84,145;,
  4;148,86,85,147;,
  4;133,70,86,148;,
  4;59,14,88,87;,
  4;45,11,91,90;,
  4;11,72,89,91;,
  4;14,45,90,88;,
  4;73,17,93,92;,
  4;17,59,87,93;,
  4;72,8,95,89;,
  4;8,44,94,95;,
  4;44,5,96,94;,
  4;5,58,97,96;,
  4;2,73,92,98;,
  4;58,2,98,97;,
  4;87,88,149,150;,
  4;90,91,106,151;,
  4;91,89,103,106;,
  4;88,90,151,149;,
  4;92,93,108,107;,
  4;93,87,150,108;,
  4;89,95,110,103;,
  4;95,94,152,110;,
  4;94,96,153,152;,
  4;96,97,154,153;,
  4;98,92,107,113;,
  4;97,98,113,154;;
 }
 MeshTextureCoords {
  114;
  0.315920;0.879060;,
  0.358960;0.872420;,
  0.358960;0.839390;,
  0.315920;0.839390;,
  0.397750;0.859640;,
  0.397750;0.839390;,
  0.315920;0.799720;,
  0.358960;0.806360;,
  0.358960;0.773330;,
  0.315920;0.760060;,
  0.397750;0.819150;,
  0.397750;0.798910;,
  0.318920;0.754070;,
  0.365210;0.768340;,
  0.358960;0.773330;,
  0.315920;0.760060;,
  0.406930;0.795850;,
  0.397750;0.798910;,
  0.358960;0.806360;,
  0.315920;0.799720;,
  0.397750;0.819150;,
  0.315920;0.839390;,
  0.358960;0.839390;,
  0.358960;0.872420;,
  0.315920;0.879060;,
  0.397750;0.839390;,
  0.397750;0.859640;,
  0.315920;0.918730;,
  0.358960;0.905450;,
  0.365210;0.910440;,
  0.318920;0.924720;,
  0.397750;0.879880;,
  0.406930;0.882930;,
  0.424760;0.839390;,
  0.424750;0.851200;,
  0.419350;0.850390;,
  0.419450;0.839390;,
  0.423900;0.816300;,
  0.424750;0.827580;,
  0.419350;0.828400;,
  0.419250;0.817850;,
  0.179660;0.902200;,
  0.189170;0.906940;,
  0.200030;0.839390;,
  0.189760;0.839390;,
  0.179660;0.776590;,
  0.189170;0.771850;,
  0.162210;0.704310;,
  0.154590;0.713790;,
  0.266710;0.940500;,
  0.269320;0.933400;,
  0.278660;0.886400;,
  0.278660;0.839390;,
  0.278660;0.886400;,
  0.278660;0.839390;,
  0.278660;0.792390;,
  0.269320;0.745380;,
  0.266710;0.738290;,
  0.269320;0.745380;,
  0.278660;0.792390;,
  0.216520;0.957680;,
  0.220710;0.949380;,
  0.234270;0.894390;,
  0.234270;0.839390;,
  0.234270;0.894390;,
  0.234270;0.839390;,
  0.234270;0.784400;,
  0.220710;0.729410;,
  0.216520;0.721100;,
  0.220710;0.729410;,
  0.234270;0.784400;,
  0.215910;0.962970;,
  0.204520;0.954290;,
  0.218080;0.896840;,
  0.218080;0.839390;,
  0.218080;0.896840;,
  0.218080;0.839390;,
  0.218080;0.781940;,
  0.204520;0.724500;,
  0.199110;0.715820;,
  0.204520;0.724500;,
  0.218080;0.781940;,
  0.162210;0.974480;,
  0.154590;0.965000;,
  0.179660;0.902200;,
  0.189760;0.839390;,
  0.179660;0.776590;,
  0.154590;0.713790;,
  0.315920;0.918730;,
  0.358960;0.905450;,
  0.397750;0.879880;,
  0.423900;0.862480;,
  0.419250;0.860940;,
  0.204520;0.954290;,
  0.154590;0.965000;,
  0.220710;0.949380;,
  0.269320;0.933400;,
  0.419350;0.828400;,
  0.419250;0.817850;,
  0.419450;0.839390;,
  0.419350;0.850390;,
  0.419250;0.860940;,
  0.405690;0.839390;,
  0.405690;0.854740;,
  0.405690;0.808710;,
  0.405690;0.824050;,
  0.412650;0.806390;,
  0.405690;0.870080;,
  0.412650;0.872400;,
  0.405690;0.808710;,
  0.405690;0.824050;,
  0.405690;0.839390;,
  0.405690;0.854740;,
  0.405690;0.870080;;
 }
}