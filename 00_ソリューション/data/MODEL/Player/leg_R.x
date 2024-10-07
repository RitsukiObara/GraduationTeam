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
 90;
 0.00000;5.91051;-2.71580;,
 10.17548;5.91051;-12.89128;,
 0.00000;5.91051;-17.10608;,
 14.39030;5.91051;-2.71580;,
 10.17548;5.91051;7.45967;,
 0.00000;5.91051;11.67450;,
 -10.17545;5.91051;7.45967;,
 -14.39027;5.91051;-2.71580;,
 -10.17545;5.91051;-12.89125;,
 11.14898;5.51631;-13.86475;,
 0.00000;5.51631;-18.48282;,
 15.76700;5.51631;-2.71580;,
 11.14898;5.51631;8.43318;,
 0.00000;5.51631;13.05123;,
 -11.14898;5.51631;8.43318;,
 -15.76700;5.51631;-2.71580;,
 -11.14898;5.51631;-13.86475;,
 11.97427;4.39398;-14.69005;,
 0.00000;4.39398;-19.64995;,
 16.93415;4.39398;-2.71580;,
 11.97427;4.39398;9.25848;,
 0.00000;4.39398;14.21837;,
 -11.97422;4.39398;9.25848;,
 -16.93415;4.39398;-2.71580;,
 -11.97422;4.39398;-14.69005;,
 12.52570;2.71428;-15.24148;,
 0.00000;2.71428;-20.42980;,
 17.71403;2.71428;-2.71580;,
 12.52570;2.71428;9.80990;,
 0.00000;2.71428;14.99820;,
 -12.52568;2.71428;9.80990;,
 -17.71402;2.71428;-2.71580;,
 -12.52568;2.71428;-15.24148;,
 12.71935;0.73293;-15.43513;,
 0.00000;0.73293;-20.70365;,
 17.98785;0.73293;-2.71580;,
 12.71935;0.73293;10.00352;,
 0.00000;0.73293;15.27210;,
 -12.71933;0.73293;10.00352;,
 -17.98785;0.73293;-2.71580;,
 -12.71933;0.73293;-15.43513;,
 0.00000;-41.11370;-17.74725;,
 10.62885;-41.11370;-13.34463;,
 10.46705;-42.76948;-13.18280;,
 0.00000;-42.76948;-17.51840;,
 15.03145;-41.11370;-2.71580;,
 14.80263;-42.76948;-2.71580;,
 10.62885;-41.11370;7.91305;,
 10.46705;-42.76948;7.75125;,
 0.00000;-41.11370;12.31567;,
 0.00000;-42.76948;12.08683;,
 -10.62883;-41.11370;7.91305;,
 -10.46703;-42.76948;7.75125;,
 -15.03145;-41.11370;-2.71580;,
 -14.80260;-42.76948;-2.71580;,
 -10.62883;-41.11370;-13.34463;,
 -10.46703;-42.76948;-13.18280;,
 10.00625;-44.17317;-12.72200;,
 0.00000;-44.17317;-16.86673;,
 14.15095;-44.17317;-2.71580;,
 10.00625;-44.17317;7.29043;,
 0.00000;-44.17317;11.43515;,
 -10.00620;-44.17317;7.29043;,
 -14.15090;-44.17317;-2.71580;,
 -10.00620;-44.17317;-12.72200;,
 9.31657;-45.11110;-12.03235;,
 0.00000;-45.11110;-15.89140;,
 13.17560;-45.11110;-2.71580;,
 9.31657;-45.11110;6.60078;,
 0.00000;-45.11110;10.45982;,
 -9.31657;-45.11110;6.60078;,
 -13.17563;-45.11110;-2.71580;,
 -9.31657;-45.11110;-12.03235;,
 8.50307;-45.44038;-11.21888;,
 0.00000;-45.44038;-14.74095;,
 12.02518;-45.44038;-2.71580;,
 8.50307;-45.44038;5.78728;,
 0.00000;-45.44038;9.30938;,
 -8.50307;-45.44038;5.78728;,
 -12.02515;-45.44038;-2.71580;,
 -8.50307;-45.44038;-11.21885;,
 0.00000;-45.44038;-2.71580;,
 11.68182;-20.03578;-14.39760;,
 0.00000;-19.83890;-19.25030;,
 16.49103;-20.45400;-2.71580;,
 11.64128;-20.84765;12.23098;,
 0.00000;-20.98728;17.04310;,
 -11.64402;-20.79193;12.23378;,
 -16.49663;-20.37493;-2.71580;,
 -11.68462;-19.97973;-14.40040;;
 
 96;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,7,6;,
 3;0,8,7;,
 3;0,2,8;,
 4;2,1,9,10;,
 4;1,3,11,9;,
 4;3,4,12,11;,
 4;4,5,13,12;,
 4;5,6,14,13;,
 4;6,7,15,14;,
 4;7,8,16,15;,
 4;8,2,10,16;,
 4;10,9,17,18;,
 4;9,11,19,17;,
 4;11,12,20,19;,
 4;12,13,21,20;,
 4;13,14,22,21;,
 4;14,15,23,22;,
 4;15,16,24,23;,
 4;16,10,18,24;,
 4;18,17,25,26;,
 4;17,19,27,25;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;22,23,31,30;,
 4;23,24,32,31;,
 4;24,18,26,32;,
 4;26,25,33,34;,
 4;25,27,35,33;,
 4;27,28,36,35;,
 4;28,29,37,36;,
 4;29,30,38,37;,
 4;30,31,39,38;,
 4;31,32,40,39;,
 4;32,26,34,40;,
 4;41,42,43,44;,
 4;42,45,46,43;,
 4;45,47,48,46;,
 4;47,49,50,48;,
 4;49,51,52,50;,
 4;51,53,54,52;,
 4;53,55,56,54;,
 4;55,41,44,56;,
 4;44,43,57,58;,
 4;43,46,59,57;,
 4;46,48,60,59;,
 4;48,50,61,60;,
 4;50,52,62,61;,
 4;52,54,63,62;,
 4;54,56,64,63;,
 4;56,44,58,64;,
 4;58,57,65,66;,
 4;57,59,67,65;,
 4;59,60,68,67;,
 4;60,61,69,68;,
 4;61,62,70,69;,
 4;62,63,71,70;,
 4;63,64,72,71;,
 4;64,58,66,72;,
 4;66,65,73,74;,
 4;65,67,75,73;,
 4;67,68,76,75;,
 4;68,69,77,76;,
 4;69,70,78,77;,
 4;70,71,79,78;,
 4;71,72,80,79;,
 4;72,66,74,80;,
 3;74,73,81;,
 3;73,75,81;,
 3;75,76,81;,
 3;76,77,81;,
 3;77,78,81;,
 3;78,79,81;,
 3;79,80,81;,
 3;80,74,81;,
 4;82,42,41,83;,
 4;82,83,34,33;,
 4;84,45,42,82;,
 4;84,82,33,35;,
 4;85,47,45,84;,
 4;85,84,35,36;,
 4;86,49,47,85;,
 4;86,85,36,37;,
 4;87,51,49,86;,
 4;87,86,37,38;,
 4;88,53,51,87;,
 4;88,87,38,39;,
 4;89,55,53,88;,
 4;89,88,39,40;,
 4;83,41,55,89;,
 4;83,89,40,34;;
 
 MeshMaterialList {
  4;
  96;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.800000;0.684000;0.542400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  90;
  0.000000;1.000000;0.000000;,
  0.000000;0.990358;-0.138533;,
  0.097959;0.990358;-0.097959;,
  0.138536;0.990357;-0.000000;,
  0.097959;0.990358;0.097959;,
  0.000000;0.990358;0.138534;,
  -0.097958;0.990358;0.097958;,
  -0.138533;0.990358;-0.000000;,
  -0.097958;0.990358;-0.097957;,
  -0.000000;0.868696;-0.495345;,
  0.350262;0.868697;-0.350261;,
  0.495344;0.868697;-0.000000;,
  0.350261;0.868697;0.350260;,
  -0.000000;0.868697;0.495343;,
  -0.350262;0.868696;0.350261;,
  -0.495346;0.868696;-0.000000;,
  -0.350263;0.868696;-0.350262;,
  -0.000001;0.582896;-0.812547;,
  0.574556;0.582898;-0.574556;,
  0.812544;0.582901;-0.000000;,
  0.574556;0.582898;0.574556;,
  -0.000001;0.582896;0.812547;,
  -0.574557;0.582897;0.574557;,
  -0.812545;0.582899;-0.000000;,
  -0.574557;0.582897;-0.574557;,
  -0.000000;0.282968;-0.959130;,
  0.678207;0.282967;-0.678207;,
  0.959130;0.282966;-0.000000;,
  0.678207;0.282966;0.678207;,
  -0.000000;0.282968;0.959130;,
  -0.678206;0.282968;0.678207;,
  -0.959129;0.282970;-0.000000;,
  -0.678206;0.282969;-0.678207;,
  -0.000000;0.033454;-0.999440;,
  0.706711;0.033453;-0.706711;,
  0.999152;0.039974;-0.009863;,
  0.711899;0.078545;0.697876;,
  0.000079;0.109436;0.993994;,
  -0.711833;0.078618;0.697935;,
  -0.999151;0.039993;-0.009851;,
  -0.706711;0.033453;-0.706711;,
  -0.000000;-0.103767;-0.994602;,
  0.703290;-0.103763;-0.703289;,
  0.993915;-0.109663;-0.010384;,
  0.707685;-0.150317;0.690352;,
  -0.000087;-0.182713;0.983166;,
  -0.707752;-0.150234;0.690302;,
  -0.993916;-0.109648;-0.010389;,
  -0.703290;-0.103765;-0.703289;,
  -0.000000;-0.282957;-0.959132;,
  0.678210;-0.282954;-0.678210;,
  0.959134;-0.282951;-0.000001;,
  0.678210;-0.282954;0.678209;,
  -0.000000;-0.282957;0.959132;,
  -0.678209;-0.282960;0.678208;,
  -0.959131;-0.282962;-0.000001;,
  -0.678209;-0.282960;-0.678208;,
  -0.000001;-0.582878;-0.812560;,
  0.574565;-0.582881;-0.574566;,
  0.812556;-0.582883;-0.000001;,
  0.574564;-0.582883;0.574564;,
  -0.000001;-0.582881;0.812558;,
  -0.574567;-0.582878;0.574565;,
  -0.812561;-0.582876;-0.000001;,
  -0.574568;-0.582876;-0.574566;,
  -0.000000;-0.868717;-0.495309;,
  0.350235;-0.868718;-0.350235;,
  0.495305;-0.868719;-0.000000;,
  0.350234;-0.868719;0.350234;,
  -0.000001;-0.868717;0.495309;,
  -0.350240;-0.868715;0.350238;,
  -0.495316;-0.868713;-0.000000;,
  -0.350240;-0.868715;-0.350238;,
  0.000000;-0.990365;-0.138482;,
  0.097922;-0.990365;-0.097922;,
  0.138483;-0.990365;-0.000000;,
  0.097922;-0.990365;0.097921;,
  -0.000000;-0.990365;0.138481;,
  -0.097920;-0.990365;0.097921;,
  -0.138480;-0.990365;-0.000000;,
  -0.097920;-0.990365;-0.097921;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-0.070473;-0.997514;,
  0.705349;-0.070473;-0.705348;,
  0.997371;-0.069617;-0.020127;,
  0.720873;-0.072701;0.689244;,
  -0.000008;-0.074191;0.997244;,
  -0.720877;-0.072550;0.689255;,
  -0.997373;-0.069580;-0.020120;,
  -0.705349;-0.070473;-0.705348;;
  96;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,49,57,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,57,65,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,65,73,80;,
  3;73,74,81;,
  3;74,75,81;,
  3;75,76,81;,
  3;76,77,81;,
  3;77,78,81;,
  3;78,79,81;,
  3;79,80,81;,
  3;80,73,81;,
  4;83,42,41,82;,
  4;83,82,33,34;,
  4;84,43,42,83;,
  4;84,83,34,35;,
  4;85,44,43,84;,
  4;85,84,35,36;,
  4;86,45,44,85;,
  4;86,85,36,37;,
  4;87,46,45,86;,
  4;87,86,37,38;,
  4;88,47,46,87;,
  4;88,87,38,39;,
  4;89,48,47,88;,
  4;89,88,39,40;,
  4;82,41,48,89;,
  4;82,89,40,33;;
 }
 MeshTextureCoords {
  90;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
