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
 30;
 16.81946;-26.54283;-71.05360;,
 -16.81946;-26.54283;-71.05360;,
 -16.81946;-14.58258;-61.16679;,
 16.81946;-14.58258;-61.16679;,
 16.81946;1.94545;-17.16168;,
 -16.81946;1.94545;-17.16168;,
 -16.81946;2.00722;24.43736;,
 16.81946;2.00722;24.43736;,
 16.81946;2.00722;24.43736;,
 -16.81946;2.00722;24.43736;,
 -16.81946;-26.54283;24.43736;,
 16.81946;-26.54283;24.43736;,
 16.81946;-26.54283;24.43736;,
 -16.81946;-26.54283;24.43736;,
 -16.81946;-26.54283;-17.16167;,
 16.81946;-26.54283;-17.16167;,
 -16.81946;1.94545;-17.16168;,
 -16.81946;-26.54283;-17.16167;,
 -16.81946;-26.54283;24.43736;,
 -16.81946;2.00722;24.43736;,
 16.81946;-26.54283;24.43736;,
 16.81946;-26.54283;-17.16167;,
 16.81946;1.94545;-17.16168;,
 16.81946;2.00722;24.43736;,
 -16.81946;-26.54283;-71.05360;,
 -16.81946;-14.58258;-61.16679;,
 -16.81946;-26.54283;-71.05360;,
 16.81946;-26.54283;-71.05360;,
 16.81946;-26.54283;-71.05360;,
 16.81946;-14.58258;-61.16679;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;3,2,5,4;,
 4;24,17,16,25;,
 4;15,14,26,27;,
 4;22,21,28,29;;
 
 MeshMaterialList {
  1;
  10;
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
    "data\\TEXTURE\\MATERIAL\\arms_02.png";
   }
  }
 }
 MeshNormals {
  8;
  0.000000;0.637135;-0.770753;,
  0.000000;0.814078;-0.580755;,
  0.000000;0.999999;-0.001485;,
  0.000000;0.000000;1.000000;,
  0.000000;0.983774;-0.179411;,
  0.000000;-1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;;
  10;
  4;0,0,1,1;,
  4;4,4,2,2;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;1,1,4,4;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;7,7,7,7;;
 }
 MeshTextureCoords {
  30;
  0.722905;0.524217;,
  0.723578;0.587803;,
  0.742289;0.587605;,
  0.741616;0.524019;,
  0.824896;0.523137;,
  0.825569;0.586723;,
  0.904295;0.585889;,
  0.903622;0.522303;,
  0.910360;0.603205;,
  0.980714;0.603205;,
  0.980714;0.673559;,
  0.910360;0.673559;,
  0.749065;0.734423;,
  0.747101;0.677319;,
  0.814268;0.677002;,
  0.816231;0.734106;,
  0.830352;0.601937;,
  0.830352;0.661067;,
  0.896733;0.661067;,
  0.896733;0.601809;,
  0.896733;0.661067;,
  0.830352;0.661067;,
  0.830352;0.601937;,
  0.896733;0.601809;,
  0.744355;0.661067;,
  0.760132;0.636242;,
  0.901282;0.676592;,
  0.903246;0.733696;,
  0.744355;0.661067;,
  0.760132;0.636242;;
 }
}
