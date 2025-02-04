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
 -3.18713;-7.61040;-6.18200;,
 -7.09399;-7.61040;-0.65781;,
 -13.83151;-7.61040;-1.27915;,
 -4.84308;-7.61040;-22.56294;,
 -4.26332;-7.61040;5.48772;,
 -8.17017;-7.61040;11.01192;,
 2.47421;-7.61040;6.10907;,
 5.30486;-7.61040;12.25461;,
 6.38107;-7.61040;0.58488;,
 22.37773;-7.61040;-3.31161;,
 7.92868;-7.61040;-11.75145;,
 16.08670;-7.61040;-23.28667;,
 -13.78265;-4.95192;-1.34823;,
 -4.78333;-5.84207;-22.64741;,
 -8.12133;-4.95192;10.94284;,
 5.30486;-4.95192;12.25461;,
 22.37773;-5.84207;-3.31161;,
 16.08670;-6.42876;-23.28667;,
 -1.30898;0.00000;-5.98193;,
 -5.84445;0.00000;-0.49922;,
 -2.20941;2.41695;-0.76855;,
 -0.91889;2.41695;-2.59333;,
 -3.52562;0.00000;4.44463;,
 -1.27438;2.41695;1.26145;,
 1.86988;0.00000;5.03816;,
 0.93503;2.41695;1.48950;,
 5.56666;0.00000;-1.11928;,
 2.22556;2.41695;-0.33526;,
 6.92280;-1.03990;-10.32915;,
 3.21207;1.71510;-5.08228;,
 -0.35646;-7.61040;-0.03647;,
 0.00000;3.44788;-0.54049;,
 -7.04513;-4.95192;-0.72688;,
 -3.13827;-4.95192;-6.25108;,
 -4.21446;-4.95192;5.41864;;
 
 44;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,0,3,11;,
 4;3,2,12,13;,
 4;2,5,14,12;,
 4;5,7,15,14;,
 4;7,9,16,15;,
 4;9,11,17,16;,
 4;11,3,13,17;,
 4;18,19,20,21;,
 4;19,22,23,20;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,18,21,29;,
 3;1,0,30;,
 3;4,1,30;,
 3;6,4,30;,
 3;8,6,30;,
 3;10,8,30;,
 3;0,10,30;,
 3;21,20,31;,
 3;20,23,31;,
 3;23,25,31;,
 3;25,27,31;,
 3;27,29,31;,
 3;29,21,31;,
 4;13,12,19,18;,
 4;32,33,21,20;,
 4;33,13,18,21;,
 4;12,14,22,19;,
 4;14,34,23,22;,
 4;34,32,20,23;,
 4;14,15,24,22;,
 4;34,14,22,23;,
 4;15,16,26,24;,
 4;16,17,28,26;,
 4;17,13,18,28;,
 4;13,33,21,18;,
 4;33,32,20,21;,
 4;32,34,23,20;;
 
 MeshMaterialList {
  2;
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
    "data\\TEXTURE\\player\\Ping-_UV.jpg";
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
  38;
  0.000000;-1.000000;-0.000000;,
  -0.696596;0.641332;-0.321633;,
  -0.935261;0.336748;0.109027;,
  -0.586494;0.647051;0.487186;,
  0.291524;0.512014;0.807995;,
  0.580255;0.716653;0.386928;,
  0.953814;0.000000;-0.300398;,
  0.860317;-0.424555;0.282148;,
  0.833337;-0.546392;0.083700;,
  0.816449;0.000001;0.577417;,
  -0.468100;0.805878;-0.362550;,
  -0.535345;0.842615;-0.058356;,
  -0.616213;0.784337;-0.071399;,
  -0.521219;0.754278;-0.399243;,
  -0.306811;0.849697;0.428814;,
  -0.392772;0.831553;0.392747;,
  0.230638;0.843815;0.484543;,
  0.220518;0.869751;0.441481;,
  0.495117;0.855452;0.151858;,
  0.501892;0.851516;0.151743;,
  0.192546;0.942940;-0.271644;,
  0.198321;0.939650;-0.278794;,
  -0.028259;0.998827;0.039326;,
  -0.921049;0.009663;-0.389327;,
  -0.999668;0.020339;0.015781;,
  -0.907937;0.027550;0.418200;,
  -0.094528;0.013777;0.995427;,
  0.316756;0.007534;0.948477;,
  0.673749;0.000000;0.738961;,
  -0.032577;-0.023267;-0.999198;,
  -0.446244;0.842993;-0.300380;,
  0.936900;-0.347946;0.033949;,
  0.779390;-0.513494;-0.358992;,
  -0.816449;-0.000001;-0.577417;,
  0.162894;0.959710;-0.228961;,
  -0.860317;0.424555;-0.282148;,
  -0.833337;0.546392;-0.083700;,
  -0.779390;0.513494;0.358992;;
  44;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;23,24,2,1;,
  4;24,25,3,2;,
  4;26,27,4,26;,
  4;27,28,5,4;,
  4;6,6,6,6;,
  4;29,29,29,29;,
  4;10,11,12,13;,
  4;11,14,15,12;,
  4;14,16,17,15;,
  4;16,18,19,17;,
  4;18,20,21,19;,
  4;20,10,13,21;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;13,12,22;,
  3;12,15,22;,
  3;15,17,22;,
  3;17,19,22;,
  3;19,21,22;,
  3;21,13,22;,
  4;1,30,11,10;,
  4;8,7,7,8;,
  4;7,31,31,7;,
  4;2,3,14,11;,
  4;9,9,9,9;,
  4;32,8,8,32;,
  4;3,4,16,14;,
  4;33,33,33,33;,
  4;4,5,18,16;,
  4;5,34,20,18;,
  4;34,1,10,20;,
  4;1,35,13,10;,
  4;35,36,12,13;,
  4;36,37,15,12;;
 }
 MeshTextureCoords {
  35;
  0.698590;0.269240;,
  0.676370;0.269240;,
  0.638060;0.269240;,
  0.689170;0.269240;,
  0.692470;0.269240;,
  0.670250;0.269240;,
  0.730790;0.269240;,
  0.746880;0.269240;,
  0.753000;0.269240;,
  0.843970;0.269240;,
  0.761800;0.269240;,
  0.808200;0.269240;,
  0.638340;0.219740;,
  0.689510;0.236320;,
  0.670530;0.219740;,
  0.746880;0.219740;,
  0.843970;0.236320;,
  0.808200;0.247240;,
  0.709270;0.127530;,
  0.683480;0.127530;,
  0.704150;0.082520;,
  0.711490;0.082520;,
  0.696670;0.127530;,
  0.709470;0.082520;,
  0.727350;0.127530;,
  0.722030;0.082520;,
  0.748370;0.127530;,
  0.729370;0.082520;,
  0.756080;0.146890;,
  0.734980;0.095590;,
  0.714690;0.269240;,
  0.716720;0.063330;,
  0.676650;0.219740;,
  0.698870;0.219740;,
  0.692750;0.219740;;
 }
}
