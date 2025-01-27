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
 92;
 -83.31047;59.70566;7.80766;,
 -83.31047;0.79682;7.80766;,
 -37.76259;-0.70954;-30.36805;,
 -37.76259;61.21202;-30.36805;,
 -37.76259;61.21202;45.98333;,
 -37.76259;-0.70954;45.98333;,
 -83.31047;59.70566;7.80766;,
 7.62139;65.16111;7.80766;,
 7.62139;65.16111;34.20123;,
 -37.76259;61.21202;45.98333;,
 -37.76259;61.21202;-30.36805;,
 7.62139;65.16111;-18.58593;,
 -83.31047;0.79682;7.80766;,
 7.62139;-0.09795;7.80766;,
 7.62139;-0.09795;-18.58593;,
 -37.76259;-0.70954;-30.36805;,
 -37.76259;-0.70954;45.98333;,
 7.62139;-0.09795;34.20123;,
 7.62139;-2.94861;-76.29449;,
 7.62139;63.45114;-76.29449;,
 -12.50900;-1.96982;-54.54329;,
 -37.76259;-0.70954;-30.36805;,
 -12.50900;62.47234;-54.54329;,
 -37.76259;61.21202;-30.36805;,
 -12.50900;-1.96982;70.15868;,
 -37.76259;-0.70954;45.98333;,
 -37.76259;61.21202;45.98333;,
 -12.50900;62.47234;70.15868;,
 7.62139;-2.94861;91.90980;,
 7.62139;63.45114;91.90980;,
 40.07626;60.17190;36.11010;,
 7.62139;65.16111;34.20123;,
 40.07626;60.17190;7.80766;,
 40.07626;1.66514;36.11010;,
 7.62139;-0.09795;34.20123;,
 40.07626;1.66514;7.80766;,
 40.07626;1.66514;-20.49474;,
 7.62139;-0.09795;-18.58593;,
 40.07626;60.17190;-20.49474;,
 7.62139;65.16111;-18.58593;,
 73.74633;56.64477;40.67810;,
 73.74633;56.64477;7.80766;,
 73.74633;3.85773;40.67810;,
 73.74633;3.85773;7.80766;,
 73.74633;3.85773;-25.06279;,
 73.74633;56.64477;-25.06279;,
 -75.85517;56.99052;7.80766;,
 -34.63151;58.35386;-26.74375;,
 -34.63151;2.31097;-26.74375;,
 -75.85517;3.67432;7.80766;,
 -34.63151;2.31097;42.35904;,
 -34.63151;58.35386;42.35904;,
 -75.85517;56.99052;7.80766;,
 -34.63151;58.35386;42.35904;,
 6.44384;61.92803;31.69550;,
 6.44384;61.92803;7.80766;,
 6.44384;61.92803;-16.08019;,
 -34.63151;58.35386;-26.74375;,
 -75.85517;3.67432;7.80766;,
 -34.63151;2.31097;-26.74375;,
 6.44384;2.86450;-16.08019;,
 6.44384;2.86450;7.80766;,
 6.44384;2.86450;31.69550;,
 -34.63151;2.31097;42.35904;,
 6.44384;0.28447;-68.31004;,
 6.44384;60.38040;-68.31004;,
 -11.77542;1.17034;-48.62385;,
 -34.63151;2.31097;-26.74375;,
 -11.77542;59.49453;-48.62385;,
 -34.63151;58.35386;-26.74375;,
 -11.77542;1.17034;64.23923;,
 -11.77542;59.49453;64.23923;,
 -34.63151;58.35386;42.35904;,
 -34.63151;2.31097;42.35904;,
 6.44384;0.28447;83.92535;,
 6.44384;60.38040;83.92535;,
 35.81752;57.41248;33.42314;,
 35.81752;57.41248;7.80766;,
 6.44384;61.92803;31.69550;,
 35.81752;4.46020;33.42314;,
 6.44384;2.86450;31.69550;,
 35.81752;4.46020;7.80766;,
 35.81752;4.46020;-17.80778;,
 6.44384;2.86450;-16.08019;,
 35.81752;57.41248;-17.80778;,
 6.44384;61.92803;-16.08019;,
 66.29103;54.22021;37.55747;,
 66.29103;54.22021;7.80766;,
 66.29103;6.44463;37.55747;,
 66.29103;6.44463;7.80766;,
 66.29103;6.44463;-21.94216;,
 66.29103;54.22021;-21.94216;;
 
 64;
 4;0,1,2,3;,
 4;0,4,5,1;,
 4;6,7,8,9;,
 4;6,10,11,7;,
 4;12,13,14,15;,
 4;12,16,17,13;,
 4;18,14,11,19;,
 4;20,21,14,18;,
 4;22,23,21,20;,
 4;22,20,18,19;,
 4;24,25,26,27;,
 4;28,17,25,24;,
 4;29,8,17,28;,
 4;29,28,24,27;,
 4;30,31,7,32;,
 4;33,34,31,30;,
 4;35,13,34,33;,
 4;36,37,13,35;,
 4;38,39,37,36;,
 4;32,7,39,38;,
 4;40,30,32,41;,
 4;42,33,30,40;,
 4;43,35,33,42;,
 4;41,43,42,40;,
 4;44,36,35,43;,
 4;45,38,36,44;,
 4;41,32,38,45;,
 4;41,45,44,43;,
 3;8,27,9;,
 3;27,31,29;,
 3;11,10,22;,
 3;19,39,22;,
 4;46,47,48,49;,
 4;46,49,50,51;,
 4;52,53,54,55;,
 4;52,55,56,57;,
 4;58,59,60,61;,
 4;58,61,62,63;,
 4;64,65,56,60;,
 4;66,64,60,67;,
 4;68,66,67,69;,
 4;68,65,64,66;,
 4;70,71,72,73;,
 4;74,70,73,62;,
 4;75,74,62,54;,
 4;75,71,70,74;,
 4;76,77,55,78;,
 4;79,76,78,80;,
 4;81,79,80,61;,
 4;82,81,61,83;,
 4;84,82,83,85;,
 4;77,84,85,55;,
 4;86,87,77,76;,
 4;88,86,76,79;,
 4;89,88,79,81;,
 4;87,86,88,89;,
 4;90,89,81,82;,
 4;91,90,82,84;,
 4;87,91,84,77;,
 4;87,89,90,91;,
 3;54,53,71;,
 3;71,75,78;,
 3;56,68,57;,
 3;65,68,85;;
 
 MeshMaterialList {
  2;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.724800;0.724800;0.724800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  81;
  0.667301;0.000000;-0.744788;,
  0.667300;0.000000;0.744789;,
  0.020228;-0.999613;0.019080;,
  -0.019482;0.999578;-0.021522;,
  0.020228;-0.999613;-0.019080;,
  -0.019482;0.999578;0.021522;,
  0.642358;0.000000;0.766405;,
  -0.039582;-0.999216;-0.000000;,
  -0.028043;0.999607;0.000000;,
  0.713045;0.000000;-0.701118;,
  0.713045;0.000000;0.701118;,
  -0.002351;0.998712;-0.050675;,
  -1.000000;0.000000;0.000000;,
  -0.002350;0.998712;0.050675;,
  0.096646;0.000000;0.995319;,
  -0.059614;0.998222;0.000000;,
  -0.128101;-0.991761;0.000000;,
  0.096646;0.000000;-0.995319;,
  -0.059614;0.998222;0.000000;,
  0.134439;0.000000;0.990922;,
  -0.064982;0.997886;0.000000;,
  -0.104186;-0.994558;0.000000;,
  0.134438;0.000000;-0.990922;,
  -0.104186;-0.994558;0.000000;,
  0.642358;0.000000;-0.766405;,
  0.073297;-0.997310;-0.000000;,
  0.076054;-0.996834;-0.023207;,
  0.076054;-0.996834;0.023207;,
  -0.001820;0.999998;0.000000;,
  -0.002085;0.999477;-0.032271;,
  -0.002085;0.999477;0.032271;,
  0.733922;0.000000;0.679233;,
  0.733921;0.000000;-0.679235;,
  0.058715;0.000000;-0.998275;,
  -0.059614;0.998222;0.000000;,
  0.058713;0.000000;0.998275;,
  0.079580;-0.996378;-0.029954;,
  0.080344;-0.996330;-0.029522;,
  0.079580;-0.996378;0.029954;,
  0.080344;-0.996330;0.029522;,
  -0.667301;0.000000;0.744788;,
  -0.667300;0.000000;-0.744789;,
  -0.020228;0.999613;-0.019080;,
  0.019482;-0.999578;0.021522;,
  -0.020228;0.999613;0.019080;,
  0.019482;-0.999578;-0.021522;,
  -0.642358;0.000000;-0.766405;,
  0.039582;0.999216;0.000000;,
  0.028043;-0.999607;-0.000000;,
  -0.713045;0.000000;0.701118;,
  -0.713045;0.000000;-0.701118;,
  0.002350;-0.998712;0.050675;,
  1.000000;0.000000;0.000000;,
  0.002350;-0.998712;-0.050675;,
  -0.096646;0.000000;-0.995319;,
  0.059614;-0.998222;-0.000000;,
  0.128101;0.991761;0.000000;,
  -0.096646;0.000000;0.995319;,
  0.059614;-0.998222;-0.000000;,
  -0.134440;0.000000;-0.990922;,
  0.064982;-0.997886;-0.000000;,
  0.104186;0.994558;0.000000;,
  -0.134438;0.000000;0.990922;,
  0.104186;0.994558;0.000000;,
  -0.642358;0.000000;0.766405;,
  -0.073297;0.997310;0.000000;,
  -0.076054;0.996834;0.023207;,
  -0.076054;0.996834;-0.023207;,
  0.001820;-0.999998;-0.000000;,
  0.002086;-0.999477;0.032271;,
  0.002085;-0.999477;-0.032271;,
  -0.733922;0.000000;-0.679233;,
  -0.733921;0.000000;0.679235;,
  -0.058715;0.000000;0.998275;,
  0.059614;-0.998222;-0.000000;,
  -0.058713;0.000000;-0.998275;,
  0.064982;-0.997886;-0.000000;,
  -0.079580;0.996378;0.029954;,
  -0.080343;0.996330;0.029522;,
  -0.079580;0.996378;-0.029954;,
  -0.080344;0.996330;-0.029522;;
  64;
  4;6,6,1,1;,
  4;24,0,0,24;,
  4;25,7,4,26;,
  4;25,27,2,7;,
  4;28,8,3,29;,
  4;28,30,5,8;,
  4;12,12,12,12;,
  4;11,29,3,11;,
  4;10,1,1,10;,
  4;10,10,31,31;,
  4;9,0,0,9;,
  4;13,5,30,13;,
  4;12,12,12,12;,
  4;32,32,9,9;,
  4;16,4,7,16;,
  4;17,33,33,17;,
  4;18,8,5,34;,
  4;15,3,8,18;,
  4;14,35,35,14;,
  4;16,7,2,16;,
  4;21,16,16,23;,
  4;22,17,17,22;,
  4;20,18,34,20;,
  4;12,12,12,12;,
  4;20,15,18,20;,
  4;19,14,14,19;,
  4;23,16,16,21;,
  4;12,12,12,12;,
  3;4,36,26;,
  3;36,4,37;,
  3;2,27,38;,
  3;39,2,38;,
  4;46,41,41,46;,
  4;64,64,40,40;,
  4;65,66,44,47;,
  4;65,47,42,67;,
  4;68,69,43,48;,
  4;68,48,45,70;,
  4;52,52,52,52;,
  4;51,51,43,69;,
  4;50,50,41,41;,
  4;50,71,71,50;,
  4;49,49,40,40;,
  4;53,53,70,45;,
  4;52,52,52,52;,
  4;72,49,49,72;,
  4;56,56,47,44;,
  4;57,57,73,73;,
  4;58,74,45,48;,
  4;55,58,48,43;,
  4;54,54,75,75;,
  4;56,56,42,47;,
  4;61,63,56,56;,
  4;62,62,57,57;,
  4;60,76,74,58;,
  4;52,52,52,52;,
  4;60,60,58,55;,
  4;59,59,54,54;,
  4;63,61,56,56;,
  4;52,52,52,52;,
  3;44,66,77;,
  3;77,78,44;,
  3;42,79,67;,
  3;80,79,42;;
 }
 MeshTextureCoords {
  92;
  0.500000;0.000000;,
  0.500000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;;
 }
}
