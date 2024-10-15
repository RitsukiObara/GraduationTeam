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
 80;
 0.25219;0.07130;0.24559;,
 0.17228;0.16990;0.16568;,
 0.01499;0.16990;0.23084;,
 0.01499;0.07130;0.34384;,
 -0.14230;0.16990;0.16568;,
 -0.22221;0.07130;0.24559;,
 -0.20745;0.16990;0.00840;,
 -0.32046;0.07130;0.00840;,
 -0.14230;0.16990;-0.14889;,
 -0.22221;0.07130;-0.22880;,
 0.01499;0.16990;-0.21405;,
 0.01499;0.07130;-0.32705;,
 0.17228;0.16990;-0.14889;,
 0.25219;0.07130;-0.22880;,
 0.23743;0.16990;0.00840;,
 0.35044;0.07130;0.00840;,
 0.17228;0.16990;0.16568;,
 0.25219;0.07130;0.24559;,
 0.01499;-0.16716;0.23084;,
 0.01499;-0.18864;0.00840;,
 0.17228;-0.16716;0.16568;,
 -0.14230;-0.16716;0.16568;,
 -0.20745;-0.16716;0.00840;,
 -0.14230;-0.16716;-0.14889;,
 0.01499;-0.16716;-0.21405;,
 0.17228;-0.16716;-0.14889;,
 0.23743;-0.16716;0.00840;,
 0.17228;0.16990;0.16568;,
 0.01499;0.19138;0.00840;,
 0.01499;0.16990;0.23084;,
 -0.14230;0.16990;0.16568;,
 -0.20745;0.16990;0.00840;,
 -0.14230;0.16990;-0.14889;,
 0.01499;0.16990;-0.21405;,
 0.17228;0.16990;-0.14889;,
 0.23743;0.16990;0.00840;,
 0.25219;-0.07329;0.24559;,
 0.01499;-0.07329;0.34384;,
 -0.22221;-0.07329;0.24559;,
 -0.32046;-0.07329;0.00840;,
 -0.22221;-0.07329;-0.22880;,
 0.01499;-0.07329;-0.32705;,
 0.25219;-0.07329;-0.22880;,
 0.35044;-0.07329;0.00840;,
 0.25219;-0.07329;0.24559;,
 0.17228;-0.16716;0.16568;,
 0.01499;-0.16716;0.23084;,
 -0.14230;-0.16716;0.16568;,
 -0.20745;-0.16716;0.00840;,
 -0.14230;-0.16716;-0.14889;,
 0.01499;-0.16716;-0.21405;,
 0.17228;-0.16716;-0.14889;,
 0.23743;-0.16716;0.00840;,
 0.17228;-0.16716;0.16568;,
 0.12388;0.00000;0.42566;,
 0.12388;0.27511;0.32378;,
 0.12388;0.31612;-0.27894;,
 0.12388;0.00000;-0.42566;,
 0.12388;-0.31612;-0.27894;,
 0.12388;-0.27511;0.32378;,
 -0.20036;0.08711;1.55057;,
 0.20036;0.08711;1.55057;,
 0.20036;0.08711;-18.44942;,
 -0.20036;0.08711;-18.44942;,
 0.20036;0.08711;1.55057;,
 0.20036;-0.08711;1.55057;,
 0.20036;-0.08711;-18.44942;,
 0.20036;0.08711;-18.44942;,
 0.20036;-0.08711;1.55057;,
 -0.20036;-0.08711;1.55057;,
 -0.20036;-0.08711;-18.44942;,
 0.20036;-0.08711;-18.44942;,
 -0.20036;-0.08711;1.55057;,
 -0.20036;0.08711;1.55057;,
 -0.20036;0.08711;-18.44942;,
 -0.20036;-0.08711;-18.44942;,
 0.20036;0.08711;1.55057;,
 -0.20036;0.08711;1.55057;,
 -0.20036;0.08711;-18.44942;,
 0.20036;0.08711;-18.44942;;
 
 48;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 3;18,19,20;,
 3;21,19,18;,
 3;22,19,21;,
 3;23,19,22;,
 3;24,19,23;,
 3;25,19,24;,
 3;26,19,25;,
 3;20,19,26;,
 3;27,28,29;,
 3;29,28,30;,
 3;30,28,31;,
 3;31,28,32;,
 3;32,28,33;,
 3;33,28,34;,
 3;34,28,35;,
 3;35,28,27;,
 4;36,0,3,37;,
 4;37,3,5,38;,
 4;38,5,7,39;,
 4;39,7,9,40;,
 4;40,9,11,41;,
 4;41,11,13,42;,
 4;42,13,15,43;,
 4;43,15,17,44;,
 4;45,36,37,46;,
 4;46,37,38,47;,
 4;47,38,39,48;,
 4;48,39,40,49;,
 4;49,40,41,50;,
 4;50,41,42,51;,
 4;51,42,43,52;,
 4;52,43,44,53;,
 4;54,55,56,57;,
 4;58,59,54,57;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;72,65,76,77;,
 4;78,79,66,75;;
 
 MeshMaterialList {
  1;
  48;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  }
 }
 MeshNormals {
  40;
  0.268592;-0.925049;0.268592;,
  0.000000;-0.925049;0.379847;,
  -0.268592;-0.925049;0.268592;,
  -0.379837;-0.925053;0.000004;,
  -0.268593;-0.925051;-0.268587;,
  -0.000001;-0.925051;-0.379843;,
  0.268592;-0.925052;-0.268585;,
  0.379838;-0.925053;0.000004;,
  0.276164;0.920580;0.276164;,
  0.000001;0.920580;0.390553;,
  -0.276163;0.920581;0.276162;,
  -0.390546;0.920584;0.000004;,
  -0.276165;0.920581;-0.276159;,
  0.000000;0.920580;-0.390553;,
  0.276165;0.920581;-0.276159;,
  0.390546;0.920584;0.000004;,
  0.000000;-1.000000;0.000002;,
  0.000000;1.000000;0.000002;,
  0.640788;0.422817;0.640794;,
  0.000001;0.422815;0.906216;,
  -0.640787;0.422819;0.640793;,
  -0.906211;0.422825;0.000008;,
  -0.640792;0.422818;-0.640789;,
  0.000000;0.422812;-0.906217;,
  0.640792;0.422818;-0.640789;,
  0.906211;0.422825;0.000008;,
  0.636878;-0.434471;0.636884;,
  0.000000;-0.434467;0.900688;,
  -0.636877;-0.434471;0.636884;,
  -0.900682;-0.434480;0.000007;,
  -0.636882;-0.434473;-0.636879;,
  -0.000001;-0.434469;-0.900687;,
  0.636882;-0.434475;-0.636877;,
  0.900682;-0.434479;0.000007;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;-0.000009;1.000000;,
  0.000000;0.000018;-1.000000;;
  48;
  4;18,8,9,19;,
  4;19,9,10,20;,
  4;20,10,11,21;,
  4;21,11,12,22;,
  4;22,12,13,23;,
  4;23,13,14,24;,
  4;24,14,15,25;,
  4;25,15,8,18;,
  3;1,16,0;,
  3;2,16,1;,
  3;3,16,2;,
  3;4,16,3;,
  3;5,16,4;,
  3;6,16,5;,
  3;7,16,6;,
  3;0,16,7;,
  3;8,17,9;,
  3;9,17,10;,
  3;10,17,11;,
  3;11,17,12;,
  3;12,17,13;,
  3;13,17,14;,
  3;14,17,15;,
  3;15,17,8;,
  4;26,18,19,27;,
  4;27,19,20,28;,
  4;28,20,21,29;,
  4;29,21,22,30;,
  4;30,22,23,31;,
  4;31,23,24,32;,
  4;32,24,25,33;,
  4;33,25,18,26;,
  4;0,26,27,1;,
  4;1,27,28,2;,
  4;2,28,29,3;,
  4;3,29,30,4;,
  4;4,30,31,5;,
  4;5,31,32,6;,
  4;6,32,33,7;,
  4;7,33,26,0;,
  4;34,34,34,34;,
  4;34,34,34,34;,
  4;35,35,35,35;,
  4;36,36,36,36;,
  4;37,37,37,37;,
  4;34,34,34,34;,
  4;38,38,38,38;,
  4;39,39,39,39;;
 }
 MeshTextureCoords {
  80;
  0.375000;0.442230;,
  0.375000;0.311560;,
  0.406250;0.311560;,
  0.406250;0.442230;,
  0.437500;0.311560;,
  0.437500;0.442230;,
  0.468750;0.311560;,
  0.468750;0.442230;,
  0.500000;0.311560;,
  0.500000;0.442230;,
  0.531250;0.311560;,
  0.531250;0.442230;,
  0.562500;0.311560;,
  0.562500;0.442230;,
  0.593750;0.311560;,
  0.593750;0.442230;,
  0.625000;0.311560;,
  0.625000;0.442230;,
  0.500000;1.000000;,
  0.500000;0.850000;,
  0.610490;0.954240;,
  0.389520;0.954240;,
  0.343750;0.843750;,
  0.389520;0.733270;,
  0.500000;0.687500;,
  0.610490;0.733270;,
  0.656250;0.843750;,
  0.610490;0.045760;,
  0.500000;0.162500;,
  0.500000;0.000000;,
  0.389520;0.045760;,
  0.343750;0.156250;,
  0.389520;0.266740;,
  0.500000;0.312500;,
  0.610490;0.266740;,
  0.656250;0.156250;,
  0.375000;0.560700;,
  0.406250;0.560700;,
  0.437500;0.560700;,
  0.468750;0.560700;,
  0.500000;0.560700;,
  0.531250;0.560700;,
  0.562500;0.560700;,
  0.593750;0.560700;,
  0.625000;0.560700;,
  0.375000;0.687500;,
  0.406250;0.687500;,
  0.437500;0.687500;,
  0.468750;0.687500;,
  0.500000;0.687500;,
  0.531250;0.687500;,
  0.562500;0.687500;,
  0.593750;0.687500;,
  0.625000;0.687500;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.625000;0.750000;,
  0.500000;0.750000;,
  0.375000;0.750000;,
  0.375000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
