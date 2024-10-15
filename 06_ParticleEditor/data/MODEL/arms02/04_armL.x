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
 110;
 8.59425;-27.64401;-13.05914;,
 8.59425;-46.51413;-8.76933;,
 -7.84365;-46.51413;-8.76933;,
 -7.84365;-27.64401;-13.05914;,
 -7.84365;-46.51413;11.81062;,
 8.59425;-46.51413;11.81062;,
 8.59425;-27.64401;11.81062;,
 -7.84365;-27.64401;11.81062;,
 12.11616;-27.64401;8.28871;,
 12.11616;-46.51413;8.28871;,
 12.11616;-46.51413;-5.24743;,
 12.11616;-27.64401;-9.53723;,
 -11.36556;-27.64401;-9.53723;,
 -11.36556;-46.51413;-5.24743;,
 -11.36556;-46.51413;8.28871;,
 -11.36556;-27.64401;8.28871;,
 -11.36556;-3.13824;-8.14919;,
 -11.36556;-3.13824;8.28871;,
 -7.84365;-3.13824;-11.67109;,
 -7.84365;-3.13824;11.81062;,
 8.59425;-3.13824;-11.67109;,
 8.59425;-3.13824;11.81062;,
 12.11616;-3.13824;-8.14919;,
 12.11616;-3.13824;8.28871;,
 -11.36556;-46.51413;8.28871;,
 -11.36556;-46.51413;-5.24743;,
 -7.84365;-46.51413;11.81062;,
 -7.84365;-46.51413;-8.76933;,
 8.59425;-46.51413;11.81062;,
 8.59425;-46.51413;-8.76933;,
 12.11616;-46.51413;8.28871;,
 12.11616;-46.51413;-5.24743;,
 -11.36556;-46.51413;-5.24743;,
 -11.36556;-27.64401;-9.53723;,
 12.11616;-27.64401;-9.53723;,
 12.11616;-46.51413;-5.24743;,
 -11.36556;-27.64401;8.28871;,
 -11.36556;-46.51413;8.28871;,
 12.11616;-46.51413;8.28871;,
 12.11616;-27.64401;8.28871;,
 -11.36556;-3.13824;8.28871;,
 -11.36556;-3.13824;-8.14919;,
 -11.36556;-3.13824;8.28871;,
 -7.84365;-3.13824;11.81062;,
 8.59425;-3.13824;11.81062;,
 12.11616;-3.13824;8.28871;,
 12.11616;-3.13824;-8.14919;,
 12.11616;-3.13824;8.28871;,
 8.59425;-3.13824;-11.67109;,
 12.11616;-3.13824;-8.14919;,
 -7.84365;-3.13824;-11.67109;,
 -11.36556;-3.13824;-8.14919;,
 3.32959;-11.29099;4.08416;,
 6.15228;-8.62370;6.90685;,
 0.00000;-8.62370;9.45521;,
 0.00000;-11.29099;5.46332;,
 -6.15229;-8.62370;6.90685;,
 -3.32960;-11.29099;4.08416;,
 -8.70065;-8.62370;0.75457;,
 -4.70876;-11.29099;0.75457;,
 -6.15229;-8.62370;-5.39771;,
 -3.32960;-11.29099;-2.57502;,
 0.00000;-8.62370;-7.94607;,
 0.00000;-11.29099;-3.95418;,
 6.15228;-8.62370;-5.39771;,
 3.32959;-11.29099;-2.57502;,
 8.70064;-8.62370;0.75457;,
 4.70875;-11.29099;0.75457;,
 8.03835;-4.63181;8.79292;,
 0.00000;-4.63181;12.12251;,
 -8.03835;-4.63181;8.79292;,
 -11.36794;-4.63181;0.75457;,
 -8.03835;-4.63181;-7.28377;,
 0.00000;-4.63181;-10.61336;,
 8.03835;-4.63181;-7.28377;,
 11.36794;-4.63181;0.75457;,
 8.70063;0.07694;9.45521;,
 0.00000;0.07694;13.05914;,
 -8.70065;0.07694;9.45521;,
 -12.30457;0.07694;0.75457;,
 -8.70065;0.07694;-7.94607;,
 0.00000;0.07694;-11.55000;,
 8.70064;0.07694;-7.94607;,
 12.30457;0.07694;0.75457;,
 8.03835;4.78569;8.79292;,
 0.00000;4.78569;12.12251;,
 -8.03835;4.78569;8.79292;,
 -11.36794;4.78569;0.75457;,
 -8.03835;4.78569;-7.28377;,
 0.00000;4.78569;-10.61336;,
 8.03835;4.78569;-7.28377;,
 11.36794;4.78569;0.75457;,
 6.15228;8.77758;6.90685;,
 0.00000;8.77758;9.45521;,
 -6.15229;8.77758;6.90685;,
 -8.70065;8.77758;0.75457;,
 -6.15229;8.77758;-5.39771;,
 0.00000;8.77758;-7.94607;,
 6.15228;8.77758;-5.39771;,
 8.70064;8.77758;0.75457;,
 3.32959;11.44487;4.08416;,
 0.00000;11.44487;5.46332;,
 -3.32960;11.44487;4.08416;,
 -4.70876;11.44487;0.75457;,
 -3.32960;11.44487;-2.57502;,
 0.00000;11.44487;-3.95418;,
 3.32959;11.44487;-2.57502;,
 4.70875;11.44487;0.75457;,
 0.00000;-12.22763;0.75457;,
 0.00000;12.38151;0.75457;;
 
 92;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 3;16,17,18;,
 3;17,19,18;,
 3;18,19,20;,
 3;19,21,20;,
 3;20,21,22;,
 3;22,21,23;,
 3;24,25,26;,
 3;25,27,26;,
 3;26,27,28;,
 3;27,29,28;,
 3;28,29,30;,
 3;30,29,31;,
 4;32,33,3,2;,
 4;1,0,34,35;,
 4;36,37,4,7;,
 4;6,5,38,39;,
 4;40,41,12,15;,
 4;42,36,7,43;,
 4;6,44,43,7;,
 4;44,6,39,45;,
 4;46,47,8,11;,
 4;0,48,49,34;,
 4;50,48,0,3;,
 4;33,51,50,3;,
 4;52,53,54,55;,
 4;55,54,56,57;,
 4;57,56,58,59;,
 4;59,58,60,61;,
 4;61,60,62,63;,
 4;63,62,64,65;,
 4;65,64,66,67;,
 4;67,66,53,52;,
 4;53,68,69,54;,
 4;54,69,70,56;,
 4;56,70,71,58;,
 4;58,71,72,60;,
 4;60,72,73,62;,
 4;62,73,74,64;,
 4;64,74,75,66;,
 4;66,75,68,53;,
 4;68,76,77,69;,
 4;69,77,78,70;,
 4;70,78,79,71;,
 4;71,79,80,72;,
 4;72,80,81,73;,
 4;73,81,82,74;,
 4;74,82,83,75;,
 4;75,83,76,68;,
 4;76,84,85,77;,
 4;77,85,86,78;,
 4;78,86,87,79;,
 4;79,87,88,80;,
 4;80,88,89,81;,
 4;81,89,90,82;,
 4;82,90,91,83;,
 4;83,91,84,76;,
 4;84,92,93,85;,
 4;85,93,94,86;,
 4;86,94,95,87;,
 4;87,95,96,88;,
 4;88,96,97,89;,
 4;89,97,98,90;,
 4;90,98,99,91;,
 4;91,99,92,84;,
 4;92,100,101,93;,
 4;93,101,102,94;,
 4;94,102,103,95;,
 4;95,103,104,96;,
 4;96,104,105,97;,
 4;97,105,106,98;,
 4;98,106,107,99;,
 4;99,107,100,92;,
 3;55,108,52;,
 3;57,108,55;,
 3;59,108,57;,
 3;61,108,59;,
 3;63,108,61;,
 3;65,108,63;,
 3;67,108,65;,
 3;52,108,67;,
 3;100,109,101;,
 3;101,109,102;,
 3;102,109,103;,
 3;103,109,104;,
 3;104,109,105;,
 3;105,109,106;,
 3;106,109,107;,
 3;107,109,100;;
 
 MeshMaterialList {
  1;
  92;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  77;
  -0.921451;-0.086120;-0.378829;,
  -0.376857;-0.205333;-0.903226;,
  0.000000;1.000000;0.000000;,
  0.921451;-0.086120;-0.378829;,
  0.376857;-0.205333;-0.903226;,
  -0.923880;0.000000;0.382683;,
  -0.382683;0.000000;0.923880;,
  0.923880;0.000000;0.382683;,
  0.382683;0.000000;0.923880;,
  -0.923935;-0.032210;-0.381192;,
  0.923935;-0.032210;-0.381192;,
  0.382789;-0.077343;-0.920592;,
  -0.382789;-0.077343;-0.920592;,
  0.269133;-0.924735;0.269133;,
  0.000000;-0.924735;0.380611;,
  -0.269133;-0.924735;0.269133;,
  -0.380611;-0.924735;0.000000;,
  -0.269133;-0.924735;-0.269133;,
  0.000000;-0.924735;-0.380612;,
  0.269133;-0.924735;-0.269133;,
  0.380611;-0.924735;-0.000000;,
  0.498493;-0.709231;0.498493;,
  0.000000;-0.709231;0.704976;,
  -0.498494;-0.709231;0.498494;,
  -0.704977;-0.709230;-0.000000;,
  -0.498494;-0.709230;-0.498494;,
  0.000000;-0.709230;-0.704977;,
  0.498494;-0.709231;-0.498494;,
  0.704976;-0.709231;-0.000000;,
  0.652733;-0.384551;0.652733;,
  0.000000;-0.384551;0.923104;,
  -0.652733;-0.384551;0.652733;,
  -0.923104;-0.384551;-0.000000;,
  -0.652733;-0.384551;-0.652733;,
  0.000000;-0.384551;-0.923104;,
  0.652733;-0.384551;-0.652733;,
  0.923104;-0.384551;0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707107;,
  -1.000000;0.000000;-0.000000;,
  -0.707107;-0.000000;-0.707107;,
  0.000000;-0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.652733;0.384551;0.652733;,
  0.000000;0.384551;0.923104;,
  -0.652733;0.384551;0.652733;,
  -0.923104;0.384551;-0.000000;,
  -0.652733;0.384551;-0.652733;,
  0.000000;0.384551;-0.923104;,
  0.652733;0.384551;-0.652733;,
  0.923104;0.384551;0.000000;,
  0.498493;0.709231;0.498493;,
  0.000000;0.709231;0.704976;,
  -0.498494;0.709231;0.498494;,
  -0.704977;0.709230;-0.000000;,
  -0.498494;0.709230;-0.498494;,
  0.000000;0.709230;-0.704977;,
  0.498494;0.709231;-0.498494;,
  0.704976;0.709231;-0.000000;,
  0.269133;0.924735;0.269133;,
  0.000000;0.924735;0.380612;,
  -0.269133;0.924735;0.269133;,
  -0.380611;0.924735;0.000000;,
  -0.269133;0.924735;-0.269133;,
  0.000000;0.924735;-0.380612;,
  0.269133;0.924735;-0.269133;,
  0.380612;0.924735;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;-0.000000;,
  -0.000000;-1.000000;0.000000;,
  -0.923726;0.021662;-0.382441;,
  0.923880;0.000000;0.382683;,
  0.923726;0.021662;-0.382441;,
  0.382313;0.052255;-0.922554;,
  -0.382313;0.052255;-0.922554;;
  92;
  4;11,4,1,12;,
  4;6,8,8,6;,
  4;7,7,3,10;,
  4;9,0,5,5;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  4;0,9,12,1;,
  4;4,11,10,3;,
  4;5,5,6,6;,
  4;8,8,7,7;,
  4;5,72,9,5;,
  4;5,5,6,6;,
  4;8,8,6,6;,
  4;8,8,7,73;,
  4;74,73,7,10;,
  4;11,75,74,10;,
  4;76,75,11,12;,
  4;9,72,76,12;,
  4;13,21,22,14;,
  4;14,22,23,15;,
  4;15,23,24,16;,
  4;16,24,25,17;,
  4;17,25,26,18;,
  4;18,26,27,19;,
  4;19,27,28,20;,
  4;20,28,21,13;,
  4;21,29,30,22;,
  4;22,30,31,23;,
  4;23,31,32,24;,
  4;24,32,33,25;,
  4;25,33,34,26;,
  4;26,34,35,27;,
  4;27,35,36,28;,
  4;28,36,29,21;,
  4;29,37,38,30;,
  4;30,38,39,31;,
  4;31,39,40,32;,
  4;32,40,41,33;,
  4;33,41,42,34;,
  4;34,42,43,35;,
  4;35,43,44,36;,
  4;36,44,37,29;,
  4;37,45,46,38;,
  4;38,46,47,39;,
  4;39,47,48,40;,
  4;40,48,49,41;,
  4;41,49,50,42;,
  4;42,50,51,43;,
  4;43,51,52,44;,
  4;44,52,45,37;,
  4;45,53,54,46;,
  4;46,54,55,47;,
  4;47,55,56,48;,
  4;48,56,57,49;,
  4;49,57,58,50;,
  4;50,58,59,51;,
  4;51,59,60,52;,
  4;52,60,53,45;,
  4;53,61,62,54;,
  4;54,62,63,55;,
  4;55,63,64,56;,
  4;56,64,65,57;,
  4;57,65,66,58;,
  4;58,66,67,59;,
  4;59,67,68,60;,
  4;60,68,61,53;,
  3;14,69,13;,
  3;15,69,14;,
  3;16,69,15;,
  3;17,69,16;,
  3;18,69,17;,
  3;19,69,18;,
  3;20,69,19;,
  3;13,69,20;,
  3;61,70,62;,
  3;62,70,63;,
  3;63,70,64;,
  3;64,70,65;,
  3;65,70,66;,
  3;66,70,67;,
  3;67,70,68;,
  3;68,70,61;;
 }
 MeshTextureCoords {
  110;
  0.611267;0.392767;,
  0.611267;0.437076;,
  0.563074;0.437076;,
  0.563074;0.392767;,
  0.563074;0.437076;,
  0.611267;0.437076;,
  0.611267;0.392767;,
  0.563074;0.392767;,
  0.692255;0.390851;,
  0.692255;0.431132;,
  0.649288;0.431132;,
  0.635670;0.390851;,
  0.635670;0.390851;,
  0.649288;0.431132;,
  0.692255;0.431132;,
  0.692255;0.390851;,
  0.650693;0.324514;,
  0.650693;0.295346;,
  0.656942;0.330764;,
  0.656942;0.289097;,
  0.686110;0.330764;,
  0.686110;0.289097;,
  0.692359;0.324514;,
  0.692359;0.295346;,
  0.649321;0.295325;,
  0.649321;0.324535;,
  0.655982;0.287725;,
  0.655982;0.332135;,
  0.687070;0.287725;,
  0.687070;0.332135;,
  0.693731;0.295325;,
  0.693731;0.324535;,
  0.552749;0.437076;,
  0.552749;0.392767;,
  0.621593;0.392767;,
  0.621593;0.437076;,
  0.552749;0.392767;,
  0.552749;0.437076;,
  0.621593;0.437076;,
  0.621593;0.392767;,
  0.692255;0.338539;,
  0.640077;0.338539;,
  0.552749;0.335225;,
  0.563074;0.335225;,
  0.611267;0.335225;,
  0.621593;0.335225;,
  0.640077;0.338539;,
  0.692255;0.338539;,
  0.611267;0.335225;,
  0.621593;0.335225;,
  0.563074;0.335225;,
  0.552749;0.335225;,
  0.977852;0.052848;,
  0.983163;0.047830;,
  0.971589;0.047830;,
  0.971589;0.052848;,
  0.960015;0.047830;,
  0.965325;0.052848;,
  0.955220;0.047830;,
  0.962730;0.052848;,
  0.960015;0.047830;,
  0.965325;0.052848;,
  0.971589;0.047830;,
  0.971589;0.052848;,
  0.983163;0.047830;,
  0.977852;0.052848;,
  0.987957;0.047830;,
  0.980447;0.052848;,
  0.986711;0.040320;,
  0.971589;0.040320;,
  0.956466;0.040320;,
  0.950203;0.040320;,
  0.956466;0.040320;,
  0.971589;0.040320;,
  0.986711;0.040320;,
  0.992975;0.040320;,
  0.987957;0.031462;,
  0.971589;0.031462;,
  0.955220;0.031462;,
  0.948440;0.031462;,
  0.955220;0.031462;,
  0.971589;0.031462;,
  0.987957;0.031462;,
  0.994737;0.031462;,
  0.986711;0.022603;,
  0.971589;0.022603;,
  0.956466;0.022603;,
  0.950203;0.022603;,
  0.956466;0.022603;,
  0.971589;0.022603;,
  0.986711;0.022603;,
  0.992975;0.022603;,
  0.983163;0.015093;,
  0.971589;0.015093;,
  0.960015;0.015093;,
  0.955220;0.015093;,
  0.960015;0.015093;,
  0.971589;0.015093;,
  0.983163;0.015093;,
  0.987957;0.015093;,
  0.977852;0.010076;,
  0.971589;0.010076;,
  0.965325;0.010076;,
  0.962730;0.010076;,
  0.965325;0.010076;,
  0.971589;0.010076;,
  0.977852;0.010076;,
  0.980447;0.010076;,
  0.971589;0.054610;,
  0.971589;0.008314;;
 }
}
