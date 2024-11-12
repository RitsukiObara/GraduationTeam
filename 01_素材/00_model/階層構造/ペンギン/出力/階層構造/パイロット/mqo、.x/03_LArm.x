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
 39;
 23.22002;-20.83520;-4.69114;,
 11.38581;-13.84850;-4.14496;,
 1.31899;10.31296;-4.14496;,
 28.81109;1.22563;-8.29685;,
 30.82701;1.22461;-4.98944;,
 0.00000;11.17003;0.00000;,
 1.31899;10.31296;2.64726;,
 32.68343;0.41192;-2.72694;,
 11.35672;-15.36275;2.64726;,
 27.07616;-22.49154;0.87877;,
 10.53008;-14.93295;-0.00000;,
 24.96173;-22.32614;-1.21235;,
 -7.31446;0.38702;0.56568;,
 -2.84248;-10.33165;0.56568;,
 -0.50317;-10.23024;3.31122;,
 -4.66251;0.00000;3.31122;,
 63.11837;-26.35931;-31.60240;,
 62.09363;-26.68418;-33.00761;,
 52.15468;-15.86854;-20.87762;,
 53.17034;-16.22176;-18.94240;,
 38.78637;-21.90267;-5.96520;,
 40.64653;-20.67707;-3.64032;,
 40.39122;-10.32318;-12.40195;,
 36.04442;-21.84114;-9.24645;,
 43.32723;-10.49298;-10.20122;,
 44.96931;-10.41109;-8.57243;,
 49.71061;-24.95655;-18.16440;,
 50.84485;-24.49672;-16.35229;,
 49.04407;-14.77582;-22.41594;,
 46.70318;-23.85211;-19.82583;,
 58.07200;-25.28800;-32.91513;,
 4.19933;-12.91666;0.18339;,
 5.64886;-12.68699;2.85149;,
 -2.13190;5.57398;2.85149;,
 -3.95125;6.21227;0.18339;,
 5.64886;-12.68699;-4.34918;,
 -2.13189;5.57398;-4.34918;,
 -0.50317;-10.23024;-4.80890;,
 -4.66251;0.00000;-4.80890;;
 
 38;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;9,11,20,21;,
 4;0,3,22,23;,
 4;24,4,7,25;,
 4;7,9,21,25;,
 4;21,20,26,27;,
 4;23,22,28,29;,
 4;18,24,25,19;,
 4;25,21,27,19;,
 4;27,26,17,16;,
 3;29,28,30;,
 3;19,27,16;,
 4;31,10,8,32;,
 4;8,6,33,32;,
 4;6,5,34,33;,
 4;2,1,35,36;,
 4;13,31,32,14;,
 4;32,33,15,14;,
 4;33,34,12,15;,
 4;36,35,37,38;,
 4;3,2,5,4;,
 4;34,5,2,36;,
 4;12,34,36,38;,
 4;37,13,12,38;,
 4;35,31,13,37;,
 4;1,10,31,35;,
 4;11,10,1,0;,
 4;20,11,0,23;,
 4;26,20,23,29;,
 4;17,26,29,30;,
 4;18,17,30,28;,
 4;22,24,18,28;,
 4;3,4,24,22;;
 
 MeshMaterialList {
  2;
  38;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\player\\Ping-pilot_UV.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\player\\Ping-pilot_UV.jpg";
   }
  }
 }
 MeshNormals {
  59;
  -0.255843;-0.096266;-0.961913;,
  -0.054891;-0.042670;-0.997580;,
  0.442354;0.887522;-0.128949;,
  0.318889;0.941134;0.112144;,
  0.304908;0.109662;0.946047;,
  0.109017;0.055782;0.992474;,
  0.092211;-0.972088;0.215735;,
  -0.558753;-0.027262;-0.828886;,
  0.450312;0.847977;-0.279561;,
  0.073775;-0.965732;0.248836;,
  -0.740849;0.022383;-0.671298;,
  0.421602;0.870543;-0.253783;,
  0.479705;0.820140;-0.311854;,
  -0.754562;0.007354;-0.656187;,
  -0.345335;-0.917373;0.197916;,
  0.064570;0.026988;-0.997548;,
  -0.343242;0.276361;0.897669;,
  -0.569016;-0.652870;0.499981;,
  0.091014;0.037914;-0.995128;,
  -0.499074;0.051859;0.865006;,
  0.430640;0.875854;-0.217782;,
  0.305831;0.951989;-0.013592;,
  -0.830134;0.546243;0.111780;,
  -0.740652;0.051739;0.669894;,
  -0.669107;-0.735172;0.108710;,
  -0.328041;-0.944656;0.003706;,
  -0.382593;-0.922745;-0.046527;,
  -0.225921;-0.973475;-0.036149;,
  -0.030153;-0.997185;0.068656;,
  -0.095832;-0.991067;0.092754;,
  0.428473;0.818530;-0.382649;,
  0.383891;0.877507;-0.287418;,
  0.437219;0.847105;-0.302082;,
  -0.155266;-0.978116;0.138498;,
  -0.388552;-0.911963;0.131725;,
  0.619209;0.051136;0.783559;,
  0.777352;-0.022489;0.628664;,
  -0.113827;-0.993349;0.017335;,
  0.081010;-0.979751;0.183102;,
  0.777548;-0.021805;0.628445;,
  0.065800;0.027218;0.997462;,
  -0.661417;0.524941;0.535691;,
  -0.785778;0.610984;0.096186;,
  0.091017;0.037915;0.995127;,
  0.415625;0.857318;-0.303746;,
  0.287851;0.947518;-0.139108;,
  -0.740385;0.565033;-0.364099;,
  -0.778673;0.494087;-0.386714;,
  -0.902134;0.037090;-0.429858;,
  -0.655720;-0.692865;-0.299949;,
  -0.298103;-0.935379;-0.190267;,
  -0.364233;-0.903811;-0.224632;,
  -0.287657;-0.935124;-0.206872;,
  -0.151544;-0.985112;-0.081167;,
  -0.260617;-0.962998;-0.068649;,
  -0.301194;-0.941760;-0.149569;,
  0.373968;0.810666;-0.450520;,
  0.344912;0.882212;-0.320527;,
  0.423681;0.845638;-0.324638;;
  38;
  4;0,1,1,0;,
  4;20,21,3,2;,
  4;4,5,5,4;,
  4;33,34,26,27;,
  4;23,24,17,19;,
  4;12,30,31,11;,
  4;33,27,28,6;,
  4;0,0,7,7;,
  4;32,20,2,8;,
  4;4,4,35,35;,
  4;6,28,29,9;,
  4;7,7,10,10;,
  4;31,32,8,11;,
  4;35,35,36,36;,
  4;9,29,37,38;,
  3;10,10,13;,
  3;36,36,39;,
  4;25,26,34,14;,
  4;5,5,16,40;,
  4;41,42,22,16;,
  4;1,1,15,15;,
  4;24,25,14,17;,
  4;40,16,19,43;,
  4;16,22,23,19;,
  4;15,15,18,18;,
  4;44,45,21,20;,
  4;22,42,46,47;,
  4;48,22,47,48;,
  4;49,24,48,48;,
  4;50,25,24,49;,
  4;51,26,25,50;,
  4;27,26,51,52;,
  4;28,27,52,53;,
  4;29,28,53,54;,
  4;37,29,54,55;,
  4;31,30,56,57;,
  4;58,32,31,57;,
  4;44,20,32,58;;
 }
 MeshTextureCoords {
  39;
  0.274336;0.593334;,
  0.230258;0.544915;,
  0.192763;0.377472;,
  0.295160;0.440449;,
  0.302669;0.440456;,
  0.187850;0.371533;,
  0.192763;0.377472;,
  0.309583;0.446088;,
  0.230149;0.555409;,
  0.288698;0.604813;,
  0.227070;0.552431;,
  0.280823;0.603667;,
  0.160606;0.446261;,
  0.177263;0.520543;,
  0.185976;0.519840;,
  0.170484;0.448943;,
  0.422942;0.631617;,
  0.419126;0.633869;,
  0.382107;0.558915;,
  0.385890;0.561362;,
  0.332315;0.600732;,
  0.339243;0.592238;,
  0.338292;0.520484;,
  0.322102;0.600306;,
  0.349228;0.521661;,
  0.355344;0.521093;,
  0.373003;0.621896;,
  0.377228;0.618709;,
  0.370521;0.551342;,
  0.361802;0.614242;,
  0.404146;0.624193;,
  0.203491;0.538457;,
  0.208890;0.536866;,
  0.179909;0.410314;,
  0.173133;0.405891;,
  0.208890;0.536866;,
  0.179909;0.410314;,
  0.185976;0.519840;,
  0.170484;0.448943;;
 }
}
