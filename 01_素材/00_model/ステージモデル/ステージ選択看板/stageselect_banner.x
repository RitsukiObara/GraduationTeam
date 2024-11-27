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
 84;
 0.00000;7.00000;-20.00000;,
 17.32050;7.00000;-10.00000;,
 17.32050;-3.80900;-10.00000;,
 0.00000;-3.80900;-20.00000;,
 17.32050;7.00000;10.00000;,
 17.32050;-3.80900;10.00000;,
 0.00000;7.00000;20.00000;,
 -0.00000;-3.80900;20.00000;,
 -17.32050;7.00000;10.00000;,
 -17.32050;-3.80900;10.00000;,
 -17.32050;7.00000;-10.00000;,
 -17.32050;-3.80900;-10.00000;,
 0.00000;7.00000;0.00000;,
 0.00000;-3.80900;-0.00000;,
 0.00000;34.30746;9.06112;,
 1.13138;34.11314;9.48756;,
 1.13138;8.63422;-2.12384;,
 0.00000;8.82858;-2.55028;,
 1.60000;33.64396;10.51706;,
 1.60000;8.16506;-1.09434;,
 1.13138;33.17478;11.54656;,
 1.13138;7.69590;-0.06484;,
 0.00000;32.98044;11.97300;,
 0.00000;7.50156;0.36160;,
 -1.13138;33.17478;11.54656;,
 -1.13138;7.69590;-0.06484;,
 -1.60000;33.64396;10.51706;,
 -1.60000;8.16506;-1.09434;,
 -1.13138;34.11314;9.48756;,
 -1.13138;8.63422;-2.12384;,
 0.00000;33.64396;10.51706;,
 0.00000;8.16506;-1.09434;,
 -45.16000;52.94348;20.21350;,
 45.16000;52.94348;20.21350;,
 45.16000;53.62356;18.72116;,
 -45.16000;53.62356;18.72116;,
 -45.16000;52.63304;16.07184;,
 45.16000;52.63304;16.07184;,
 45.16000;35.00016;8.03610;,
 -45.16000;35.00016;8.03610;,
 -45.16000;32.35086;9.02662;,
 45.16000;32.35086;9.02662;,
 45.16000;31.67076;10.51896;,
 -45.16000;31.67076;10.51896;,
 -45.16000;32.66132;13.16828;,
 45.16000;32.66132;13.16828;,
 45.16000;50.29412;21.20402;,
 -45.16000;50.29412;21.20402;,
 47.16000;51.12352;19.38410;,
 47.16000;33.49070;11.34836;,
 47.16000;34.17078;9.85602;,
 47.16000;51.80364;17.89176;,
 -47.16000;33.49070;11.34836;,
 -47.16000;51.12352;19.38410;,
 -47.16000;51.80364;17.89176;,
 -47.16000;34.17078;9.85602;,
 45.16000;53.67698;17.19134;,
 -45.16000;53.67698;17.19134;,
 45.16000;52.63304;16.07184;,
 -45.16000;52.63304;16.07184;,
 -45.16000;33.47036;7.98266;,
 45.16000;33.47036;7.98266;,
 -45.16000;35.00016;8.03610;,
 45.16000;35.00016;8.03610;,
 -45.16000;51.82394;21.25746;,
 45.16000;51.82394;21.25746;,
 45.16000;31.61736;12.04876;,
 -45.16000;31.61736;12.04876;,
 46.57422;52.41042;19.97058;,
 46.57422;53.09052;18.47822;,
 46.57422;32.88392;9.26954;,
 46.57422;32.20380;10.76188;,
 -46.57422;53.09052;18.47822;,
 -46.57422;52.41042;19.97058;,
 -46.57422;32.20380;10.76188;,
 -46.57422;32.88392;9.26954;,
 46.57422;52.39012;16.60488;,
 46.57422;34.75726;8.56914;,
 46.57422;32.90422;12.63522;,
 46.57422;50.53706;20.67098;,
 -46.57422;34.75726;8.56914;,
 -46.57422;52.39012;16.60488;,
 -46.57422;50.53706;20.67098;,
 -46.57422;32.90422;12.63522;;
 
 104;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,0,3,11;,
 3;12,1,0;,
 3;12,4,1;,
 3;12,6,4;,
 3;12,8,6;,
 3;12,10,8;,
 3;12,0,10;,
 3;13,3,2;,
 3;13,2,5;,
 3;13,5,7;,
 3;13,7,9;,
 3;13,9,11;,
 3;13,11,3;,
 4;14,15,16,17;,
 4;15,18,19,16;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,14,17,29;,
 3;30,15,14;,
 3;30,18,15;,
 3;30,20,18;,
 3;30,22,20;,
 3;30,24,22;,
 3;30,26,24;,
 3;30,28,26;,
 3;30,14,28;,
 3;31,17,16;,
 3;31,16,19;,
 3;31,19,21;,
 3;31,21,23;,
 3;31,23,25;,
 3;31,25,27;,
 3;31,27,29;,
 3;31,29,17;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;35,34,56,57;,
 4;57,56,58,59;,
 4;60,61,41,40;,
 4;62,63,61,60;,
 4;64,65,33,32;,
 4;47,46,65,64;,
 4;43,42,66,67;,
 4;67,66,45,44;,
 4;34,33,68,69;,
 4;69,68,48,51;,
 4;42,41,70,71;,
 4;71,70,50,49;,
 4;32,35,72,73;,
 4;73,72,54,53;,
 4;40,43,74,75;,
 4;75,74,52,55;,
 4;63,58,76,77;,
 4;77,76,51,50;,
 4;46,45,78,79;,
 4;79,78,49,48;,
 4;59,62,80,81;,
 4;81,80,55,54;,
 4;44,47,82,83;,
 4;83,82,53,52;,
 3;34,69,56;,
 3;69,51,76;,
 3;76,56,69;,
 3;56,76,58;,
 3;57,72,35;,
 3;81,54,72;,
 3;72,57,81;,
 3;59,81,57;,
 3;61,70,41;,
 3;77,50,70;,
 3;70,61,77;,
 3;63,77,61;,
 3;65,68,33;,
 3;79,48,68;,
 3;68,65,79;,
 3;46,79,65;,
 3;40,75,60;,
 3;75,55,80;,
 3;80,60,75;,
 3;60,80,62;,
 3;42,71,66;,
 3;71,49,78;,
 3;78,66,71;,
 3;66,78,45;,
 3;32,73,64;,
 3;73,53,82;,
 3;82,64,73;,
 3;64,82,47;,
 3;67,74,43;,
 3;83,52,74;,
 3;74,67,83;,
 3;44,83,67;;
 
 MeshMaterialList {
  3;
  104;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  2,
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
    "data\\TEXTURE\\MATERIAL\\ice001.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.270000;0.270000;0.270000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\selectstage_bannner_wood.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.270000;0.270000;0.270000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\selectstage_banner_text.png";
   }
  }
 }
 MeshNormals {
  80;
  0.000000;1.000000;0.000000;,
  0.500000;0.000000;-0.866025;,
  1.000000;0.000000;0.000000;,
  0.500000;0.000000;0.866025;,
  -0.500000;0.000000;0.866025;,
  -1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.500000;0.000000;-0.866025;,
  0.000000;0.909960;0.414697;,
  0.000000;0.414693;-0.909961;,
  0.707109;0.293231;-0.643438;,
  1.000000;-0.000000;-0.000000;,
  0.707109;-0.293231;0.643437;,
  0.000000;-0.414693;0.909961;,
  -0.707109;-0.293231;0.643437;,
  -1.000000;-0.000000;0.000000;,
  -0.707109;0.293231;-0.643438;,
  0.000000;-0.909963;-0.414689;,
  -0.000008;0.909961;0.414694;,
  0.000000;0.909964;0.414687;,
  0.000000;0.909958;0.414701;,
  0.000002;0.909959;0.414699;,
  0.000000;0.909959;0.414698;,
  -0.000002;0.909959;0.414699;,
  0.000008;0.909961;0.414694;,
  0.000000;-0.909959;-0.414698;,
  -0.000007;-0.909962;-0.414692;,
  0.000000;-0.909965;-0.414686;,
  0.000001;-0.909964;-0.414687;,
  0.000000;-0.909964;-0.414688;,
  -0.000001;-0.909964;-0.414687;,
  0.000007;-0.909962;-0.414692;,
  -0.191965;0.796181;0.573799;,
  0.191965;0.796181;0.573799;,
  -0.191962;0.955395;0.224436;,
  0.191962;0.955395;0.224436;,
  -0.191959;0.573800;-0.796182;,
  0.191959;0.573800;-0.796182;,
  -0.191960;0.224447;-0.955393;,
  0.191960;0.224447;-0.955393;,
  -0.191964;-0.796183;-0.573797;,
  0.191964;-0.796183;-0.573797;,
  -0.191961;-0.955395;-0.224436;,
  0.191961;-0.955395;-0.224436;,
  -0.191962;-0.573800;0.796181;,
  0.191962;-0.573800;0.796181;,
  -0.191962;-0.224447;0.955392;,
  0.191962;-0.224447;0.955392;,
  0.962446;0.095069;0.254281;,
  0.962446;-0.254280;0.095072;,
  0.962447;0.254279;-0.095070;,
  0.962446;-0.095071;-0.254281;,
  -0.962446;-0.254280;0.095072;,
  -0.962446;0.095069;0.254281;,
  -0.962446;-0.095071;-0.254281;,
  -0.962447;0.254279;-0.095070;,
  -0.282363;0.898557;-0.335955;,
  0.282363;0.898557;-0.335955;,
  -0.282369;-0.335952;-0.898557;,
  0.282369;-0.335952;-0.898557;,
  -0.282368;0.335945;0.898560;,
  0.282368;0.335945;0.898560;,
  -0.282362;-0.898556;0.335961;,
  0.282362;-0.898556;0.335961;,
  0.678336;0.500154;0.538243;,
  0.678333;0.734351;0.024356;,
  0.678332;-0.734352;-0.024354;,
  0.678335;-0.500157;-0.538242;,
  -0.678336;0.500154;0.538243;,
  -0.678333;0.734351;0.024356;,
  -0.678332;-0.734352;-0.024354;,
  -0.678335;-0.500157;-0.538242;,
  0.678333;0.024361;-0.734351;,
  0.678332;0.538244;-0.500159;,
  0.678331;-0.538243;0.500161;,
  0.678333;-0.024362;0.734350;,
  -0.678333;0.024361;-0.734351;,
  -0.678332;0.538244;-0.500159;,
  -0.678331;-0.538243;0.500161;,
  -0.678333;-0.024362;0.734350;;
  104;
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;7,7,7,7;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,9,9,16;,
  3;8,18,19;,
  3;8,20,18;,
  3;8,21,20;,
  3;8,22,21;,
  3;8,23,22;,
  3;8,20,23;,
  3;8,24,20;,
  3;8,19,24;,
  3;17,25,26;,
  3;17,26,27;,
  3;17,27,28;,
  3;17,28,29;,
  3;17,29,30;,
  3;17,30,27;,
  3;17,27,31;,
  3;17,31,25;,
  4;32,33,35,34;,
  4;36,37,39,38;,
  4;40,41,43,42;,
  4;44,45,47,46;,
  4;48,49,51,50;,
  4;52,53,55,54;,
  4;34,35,57,56;,
  4;56,57,37,36;,
  4;58,59,41,40;,
  4;38,39,59,58;,
  4;60,61,33,32;,
  4;46,47,61,60;,
  4;42,43,63,62;,
  4;62,63,45,44;,
  4;35,33,64,65;,
  4;65,64,48,50;,
  4;43,41,67,66;,
  4;66,67,51,49;,
  4;32,34,69,68;,
  4;68,69,55,53;,
  4;40,42,70,71;,
  4;71,70,52,54;,
  4;39,37,73,72;,
  4;72,73,50,51;,
  4;47,45,74,75;,
  4;75,74,49,48;,
  4;36,38,76,77;,
  4;77,76,54,55;,
  4;44,46,79,78;,
  4;78,79,53,52;,
  3;35,65,57;,
  3;65,50,73;,
  3;73,57,65;,
  3;57,73,37;,
  3;56,69,34;,
  3;77,55,69;,
  3;69,56,77;,
  3;36,77,56;,
  3;59,67,41;,
  3;72,51,67;,
  3;67,59,72;,
  3;39,72,59;,
  3;61,64,33;,
  3;75,48,64;,
  3;64,61,75;,
  3;47,75,61;,
  3;40,71,58;,
  3;71,54,76;,
  3;76,58,71;,
  3;58,76,38;,
  3;43,66,63;,
  3;66,49,74;,
  3;74,63,66;,
  3;63,74,45;,
  3;32,68,60;,
  3;68,53,79;,
  3;79,60,68;,
  3;60,79,46;,
  3;62,70,42;,
  3;78,52,70;,
  3;70,62,78;,
  3;44,78,62;;
 }
 MeshTextureCoords {
  84;
  0.050701;0.743628;,
  0.481457;0.990041;,
  0.511310;1.004319;,
  0.080553;0.757906;,
  0.911424;0.737166;,
  0.941277;0.751444;,
  0.910634;0.237879;,
  0.940486;0.252157;,
  0.479877;-0.008534;,
  0.509730;0.005744;,
  0.049910;0.244341;,
  0.079763;0.258619;,
  0.480667;0.490753;,
  0.510520;0.505031;,
  0.500000;-0.379345;,
  0.528284;-0.374364;,
  0.528284;0.278694;,
  0.500000;0.273712;,
  0.540000;-0.362338;,
  0.540000;0.290719;,
  0.528284;-0.350313;,
  0.528284;0.302744;,
  0.500000;-0.345332;,
  0.500000;0.307725;,
  0.471716;-0.350313;,
  0.471716;0.302744;,
  0.460000;-0.362338;,
  0.460000;0.290719;,
  0.471716;-0.374364;,
  0.471716;0.278694;,
  0.500000;-0.362338;,
  0.500000;0.290719;,
  -0.629000;-0.857010;,
  1.629000;-0.857010;,
  1.629000;-0.874442;,
  -0.629000;-0.874442;,
  0.001734;-1.963027;,
  0.998266;-1.963027;,
  0.998266;-1.063500;,
  0.001734;-1.063500;,
  -0.629000;-0.329195;,
  1.629000;-0.329195;,
  1.629000;-0.311763;,
  -0.629000;-0.311763;,
  -0.629000;-0.337152;,
  1.629000;-0.337152;,
  1.629000;-0.789104;,
  -0.629000;-0.789104;,
  1.679000;-0.810362;,
  1.679000;-0.358410;,
  1.679000;-0.375841;,
  1.679000;-0.827795;,
  -0.679000;-0.358410;,
  -0.679000;-0.810362;,
  -0.679000;-0.827795;,
  -0.679000;-0.375841;,
  1.629000;-0.875811;,
  -0.629000;-0.875811;,
  1.629000;-0.849053;,
  -0.629000;-0.849053;,
  -0.629000;-0.357889;,
  1.629000;-0.357889;,
  -0.629000;-0.397099;,
  1.629000;-0.397099;,
  -0.629000;-0.828315;,
  1.629000;-0.828315;,
  1.629000;-0.310394;,
  -0.629000;-0.310394;,
  1.664355;-0.843347;,
  1.664355;-0.860779;,
  1.664355;-0.342858;,
  1.664355;-0.325425;,
  -0.664355;-0.860779;,
  -0.664355;-0.843347;,
  -0.664355;-0.325425;,
  -0.664355;-0.342858;,
  1.664355;-0.842827;,
  1.664355;-0.390874;,
  1.664355;-0.343378;,
  1.664355;-0.795331;,
  -0.664355;-0.390874;,
  -0.664355;-0.842827;,
  -0.664355;-0.795331;,
  -0.664355;-0.343378;;
 }
}
