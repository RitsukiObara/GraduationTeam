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
 35;
 3.26883;-7.80554;-5.78616;,
 4.96726;-7.80554;-22.58711;,
 14.18616;-7.80554;-0.75760;,
 7.27587;-7.80554;-0.12032;,
 8.37966;-7.80554;11.84864;,
 4.37262;-7.80554;6.18280;,
 -5.44090;-7.80554;13.12318;,
 -2.53765;-7.80554;6.82006;,
 -22.95154;-7.80554;-2.84218;,
 -6.54469;-7.80554;1.15422;,
 -16.49918;-7.80554;-23.32941;,
 -8.13198;-7.80554;-11.49840;,
 4.90597;-5.99187;-22.67376;,
 14.13605;-5.07890;-0.82845;,
 8.32955;-5.07890;11.77778;,
 -5.44090;-5.07890;13.12318;,
 -22.95154;-5.99187;-2.84218;,
 -16.49918;-6.59360;-23.32941;,
 1.34254;0.00000;-5.58096;,
 0.94245;2.47893;-2.10546;,
 2.26606;2.47893;-0.23390;,
 5.99429;0.00000;0.04234;,
 1.30706;2.47893;1.84816;,
 3.61602;0.00000;5.11294;,
 -0.95901;2.47893;2.08206;,
 -1.91782;0.00000;5.72171;,
 -2.28262;2.47893;0.21051;,
 -5.70941;0.00000;-0.59363;,
 -3.29443;1.75907;-4.65824;,
 -7.10032;-1.06656;-10.03963;,
 0.36560;-7.80554;0.51696;,
 0.00000;3.53629;0.00000;,
 7.22578;-5.07890;-0.19117;,
 3.21874;-5.07890;-5.85701;,
 4.32253;-5.07890;6.11195;;
 
 44;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,1,0;,
 4;1,12,13,2;,
 4;2,13,14,4;,
 4;4,14,15,6;,
 4;6,15,16,8;,
 4;8,16,17,10;,
 4;10,17,12,1;,
 4;18,19,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;25,24,26,27;,
 4;27,26,28,29;,
 4;29,28,19,18;,
 3;3,30,0;,
 3;5,30,3;,
 3;7,30,5;,
 3;9,30,7;,
 3;11,30,9;,
 3;0,30,11;,
 3;19,31,20;,
 3;20,31,22;,
 3;22,31,24;,
 3;24,31,26;,
 3;26,31,28;,
 3;28,31,19;,
 4;12,18,21,13;,
 4;32,20,19,33;,
 4;33,19,18,12;,
 4;13,21,23,14;,
 4;14,23,22,34;,
 4;34,22,20,32;,
 4;14,23,25,15;,
 4;34,22,23,14;,
 4;15,25,27,16;,
 4;16,27,29,17;,
 4;17,29,18,12;,
 4;12,18,19,33;,
 4;33,19,20,32;,
 4;32,20,22,34;;
 
 MeshMaterialList {
  1;
  44;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\player\\Ping-hatimaki_UV.jpg";
   }
  }
 }
 MeshNormals {
  38;
  0.000000;-1.000000;-0.000000;,
  0.696596;0.641332;-0.321632;,
  0.935261;0.336750;0.109027;,
  0.586493;0.647052;0.487185;,
  -0.291523;0.512015;0.807995;,
  -0.580255;0.716653;0.386928;,
  -0.953814;0.000000;-0.300399;,
  -0.860317;-0.424554;0.282149;,
  -0.833337;-0.546392;0.083702;,
  -0.816449;0.000003;0.577418;,
  0.468100;0.805878;-0.362550;,
  0.535347;0.842614;-0.058357;,
  0.616213;0.784336;-0.071400;,
  0.521219;0.754278;-0.399244;,
  0.306813;0.849696;0.428815;,
  0.392773;0.831553;0.392745;,
  -0.230638;0.843815;0.484544;,
  -0.220517;0.869752;0.441479;,
  -0.495117;0.855452;0.151858;,
  -0.501891;0.851516;0.151742;,
  -0.192545;0.942940;-0.271644;,
  -0.198321;0.939650;-0.278795;,
  0.028259;0.998827;0.039323;,
  0.921049;0.009665;-0.389328;,
  0.999669;0.020343;0.015781;,
  0.907937;0.027554;0.418200;,
  0.094527;0.013780;0.995427;,
  -0.316756;0.007536;0.948477;,
  -0.673749;0.000000;0.738961;,
  0.032578;-0.023272;-0.999198;,
  0.446245;0.842993;-0.300380;,
  -0.936900;-0.347945;0.033949;,
  -0.779391;-0.513494;-0.358991;,
  0.816449;-0.000003;-0.577418;,
  -0.162894;0.959710;-0.228961;,
  0.860317;0.424554;-0.282149;,
  0.833337;0.546392;-0.083702;,
  0.779391;0.513494;0.358991;;
  44;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;23,1,2,24;,
  4;24,2,3,25;,
  4;26,26,4,27;,
  4;27,4,5,28;,
  4;6,6,6,6;,
  4;29,29,29,29;,
  4;10,13,12,11;,
  4;11,12,15,14;,
  4;14,15,17,16;,
  4;16,17,19,18;,
  4;18,19,21,20;,
  4;20,21,13,10;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;13,22,12;,
  3;12,22,15;,
  3;15,22,17;,
  3;17,22,19;,
  3;19,22,21;,
  3;21,22,13;,
  4;1,10,11,30;,
  4;8,8,7,7;,
  4;7,7,31,31;,
  4;2,11,14,3;,
  4;9,9,9,9;,
  4;32,32,8,8;,
  4;3,14,16,4;,
  4;33,33,33,33;,
  4;4,16,18,5;,
  4;5,18,20,34;,
  4;34,20,10,1;,
  4;1,10,13,35;,
  4;35,13,12,36;,
  4;36,12,15,37;;
 }
 MeshTextureCoords {
  35;
  0.794597;0.310085;,
  0.806902;0.310085;,
  0.873692;0.310085;,
  0.823628;0.310085;,
  0.831625;0.310085;,
  0.802594;0.310085;,
  0.731496;0.310085;,
  0.752530;0.310085;,
  0.604632;0.310085;,
  0.723499;0.310085;,
  0.651379;0.310085;,
  0.711999;0.310085;,
  0.806458;0.267060;,
  0.873329;0.245401;,
  0.831262;0.245401;,
  0.731496;0.245401;,
  0.604632;0.267060;,
  0.651379;0.281334;,
  0.780641;0.124916;,
  0.777743;0.066109;,
  0.787332;0.066109;,
  0.814343;0.124916;,
  0.780384;0.066109;,
  0.797112;0.124916;,
  0.763967;0.066109;,
  0.757020;0.124916;,
  0.754377;0.066109;,
  0.729550;0.124916;,
  0.747047;0.083186;,
  0.719473;0.150218;,
  0.773563;0.310085;,
  0.770915;0.041025;,
  0.823265;0.245401;,
  0.794234;0.245401;,
  0.802231;0.245401;;
 }
}
