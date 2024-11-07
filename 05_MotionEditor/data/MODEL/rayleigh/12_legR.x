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
 197;
 0.09913;-1.21410;-0.64773;,
 -0.23163;-1.21410;-0.64773;,
 -0.21822;-1.92097;-0.49697;,
 0.08573;-1.92097;-0.49697;,
 0.08573;-0.41056;0.61036;,
 -0.21822;-0.41056;0.61036;,
 -0.24416;-0.43068;0.08589;,
 0.11166;-0.43068;0.08589;,
 0.08573;-1.92097;0.61036;,
 -0.21822;-1.92097;0.61036;,
 -0.23163;-1.21410;0.64773;,
 0.09913;-1.21410;0.64773;,
 0.07319;-3.58728;0.14717;,
 -0.20569;-3.58728;0.14717;,
 -0.21822;-3.83307;0.61036;,
 0.08573;-3.83307;0.61036;,
 -0.52631;-1.21410;0.09734;,
 -0.47866;-1.21410;0.51419;,
 -0.44523;-1.92097;0.47681;,
 -0.44523;-1.92097;0.13569;,
 0.31273;-1.92097;0.47681;,
 0.34616;-1.21410;0.51419;,
 0.39381;-1.21410;0.09734;,
 0.31273;-1.92097;0.13569;,
 0.03912;-2.53085;-0.41205;,
 -0.17162;-2.53085;-0.41205;,
 -0.16924;-2.92063;-0.40025;,
 0.03675;-2.92063;-0.40025;,
 -0.32902;-2.62884;0.16988;,
 -0.32902;-2.69486;0.38201;,
 -0.37615;-3.08463;0.38201;,
 -0.37615;-3.01862;0.16988;,
 0.05802;-3.08463;0.51556;,
 -0.19052;-3.08463;0.51556;,
 -0.17162;-2.69486;0.51556;,
 0.03912;-2.69486;0.51556;,
 0.24365;-3.08463;0.38201;,
 0.19652;-2.69486;0.38201;,
 0.19652;-2.62884;0.16988;,
 0.24365;-3.01862;0.16988;,
 -0.50992;-0.43068;0.08589;,
 -0.44523;-0.41056;0.47681;,
 0.08573;-0.41056;-0.62069;,
 -0.21822;-0.41056;-0.62069;,
 0.31273;-0.41056;0.47681;,
 0.37742;-0.43068;0.08589;,
 -0.15897;-3.41177;-0.45667;,
 0.02647;-3.41177;-0.45667;,
 -0.46546;-3.46441;0.47681;,
 -0.43419;-3.25682;0.14717;,
 0.09384;-3.46441;0.61036;,
 -0.22634;-3.46441;0.61036;,
 0.33296;-3.46441;0.47681;,
 0.30169;-3.25682;0.14717;,
 -0.29746;-3.41177;-0.33032;,
 -0.27723;-3.78044;-0.33032;,
 -0.15649;-3.78044;-0.45667;,
 -0.44523;-3.83307;0.47681;,
 -0.41396;-3.58728;0.14717;,
 0.31273;-3.83307;0.47681;,
 0.28146;-3.58728;0.14717;,
 -0.24416;-0.41410;-0.24017;,
 0.11166;-0.41410;-0.24017;,
 -0.50992;-0.41410;-0.24017;,
 -0.52631;-1.21410;-0.25035;,
 -0.44523;-1.92097;-0.14883;,
 -0.32902;-2.57378;-0.08613;,
 -0.37615;-2.96355;-0.08613;,
 -0.40811;-3.20171;-0.12779;,
 -0.38788;-3.53217;-0.12779;,
 0.06273;-3.53217;-0.12779;,
 -0.19523;-3.53217;-0.12779;,
 0.27561;-3.20171;-0.12779;,
 0.25538;-3.53217;-0.12779;,
 0.24365;-2.96355;-0.08613;,
 0.19652;-2.57378;-0.08613;,
 0.31273;-1.92097;-0.14883;,
 0.39381;-1.21410;-0.25035;,
 0.37742;-0.41410;-0.24017;,
 -0.47866;-1.21410;-0.52138;,
 -0.44523;-0.41056;-0.49434;,
 -0.44523;-1.92097;-0.37062;,
 -0.32902;-2.53085;-0.28570;,
 -0.32309;-2.92063;-0.27390;,
 0.02399;-3.78044;-0.45667;,
 -0.15649;-3.78044;-0.45667;,
 0.16496;-3.41177;-0.33032;,
 0.14473;-3.78044;-0.33032;,
 0.19059;-2.92063;-0.27390;,
 0.19652;-2.53085;-0.28570;,
 0.31273;-1.92097;-0.37062;,
 0.34616;-1.21410;-0.52138;,
 0.31273;-0.41056;-0.49434;,
 0.28146;-3.58728;0.14717;,
 0.25538;-3.53217;-0.12779;,
 0.31273;-3.83307;0.47681;,
 0.33296;-3.46441;0.47681;,
 0.24365;-3.08463;0.38201;,
 0.19652;-2.69486;0.38201;,
 0.31273;-1.92097;0.47681;,
 0.34616;-1.21410;0.51419;,
 0.31273;-0.41056;0.47681;,
 0.37742;-0.43068;0.08589;,
 0.37742;-0.41410;-0.24017;,
 -0.50992;-0.41410;-0.24017;,
 -0.50992;-0.43068;0.08589;,
 -0.44523;-0.41056;0.47681;,
 -0.47866;-1.21410;0.51419;,
 -0.44523;-1.92097;0.47681;,
 -0.32902;-2.69486;0.38201;,
 -0.37615;-3.08463;0.38201;,
 -0.46546;-3.46441;0.47681;,
 -0.44523;-3.83307;0.47681;,
 -0.41396;-3.58728;0.14717;,
 -0.38788;-3.53217;-0.12779;,
 -0.27723;-3.78044;-0.33032;,
 0.02399;-3.78044;-0.45667;,
 0.14473;-3.78044;-0.33032;,
 -0.16019;-0.58094;0.19773;,
 -0.29599;-0.45261;0.33353;,
 -0.00000;-0.45261;0.45614;,
 -0.00000;-0.58094;0.26408;,
 0.29599;-0.45261;0.33353;,
 0.16019;-0.58094;0.19773;,
 0.41860;-0.45261;0.03754;,
 0.22654;-0.58094;0.03754;,
 0.29599;-0.45261;-0.25846;,
 0.16019;-0.58094;-0.12265;,
 0.00000;-0.45261;-0.38106;,
 0.00000;-0.58094;-0.18901;,
 -0.29599;-0.45261;-0.25846;,
 -0.16019;-0.58094;-0.12265;,
 -0.41860;-0.45261;0.03754;,
 -0.22654;-0.58094;0.03754;,
 -0.29599;-0.45261;0.33353;,
 -0.16019;-0.58094;0.19773;,
 -0.38674;-0.26056;0.42428;,
 -0.00000;-0.26056;0.58447;,
 0.38674;-0.26056;0.42428;,
 0.54693;-0.26056;0.03754;,
 0.38674;-0.26056;-0.34920;,
 0.00000;-0.26056;-0.50939;,
 -0.38674;-0.26056;-0.34920;,
 -0.54693;-0.26056;0.03754;,
 -0.38674;-0.26056;0.42428;,
 -0.41860;-0.03401;0.45614;,
 -0.00000;-0.03401;0.62953;,
 0.41860;-0.03401;0.45614;,
 0.59199;-0.03401;0.03754;,
 0.41860;-0.03401;-0.38106;,
 0.00000;-0.03401;-0.55445;,
 -0.41860;-0.03401;-0.38106;,
 -0.59199;-0.03401;0.03754;,
 -0.41860;-0.03401;0.45614;,
 -0.38674;0.19253;0.42428;,
 0.00000;0.19253;0.58447;,
 0.38674;0.19253;0.42428;,
 0.54693;0.19253;0.03754;,
 0.38674;0.19253;-0.34920;,
 0.00000;0.19253;-0.50939;,
 -0.38674;0.19253;-0.34920;,
 -0.54693;0.19253;0.03754;,
 -0.38674;0.19253;0.42428;,
 -0.29599;0.38459;0.33353;,
 0.00000;0.38459;0.45614;,
 0.29599;0.38459;0.33353;,
 0.41860;0.38459;0.03754;,
 0.29599;0.38459;-0.25846;,
 0.00000;0.38459;-0.38106;,
 -0.29599;0.38459;-0.25846;,
 -0.41860;0.38459;0.03754;,
 -0.29599;0.38459;0.33353;,
 -0.16019;0.51292;0.19773;,
 0.00000;0.51292;0.26408;,
 0.16019;0.51292;0.19773;,
 0.22654;0.51292;0.03754;,
 0.16019;0.51292;-0.12265;,
 0.00000;0.51292;-0.18901;,
 -0.16019;0.51292;-0.12265;,
 -0.22654;0.51292;0.03754;,
 -0.16019;0.51292;0.19773;,
 -0.00000;-0.62600;0.03754;,
 -0.00000;-0.62600;0.03754;,
 -0.00000;-0.62600;0.03754;,
 -0.00000;-0.62600;0.03754;,
 -0.00000;-0.62600;0.03754;,
 -0.00000;-0.62600;0.03754;,
 -0.00000;-0.62600;0.03754;,
 -0.00000;-0.62600;0.03754;,
 0.00000;0.55798;0.03754;,
 0.00000;0.55798;0.03754;,
 0.00000;0.55798;0.03754;,
 0.00000;0.55798;0.03754;,
 0.00000;0.55798;0.03754;,
 0.00000;0.55798;0.03754;,
 0.00000;0.55798;0.03754;,
 0.00000;0.55798;0.03754;;
 
 154;
 4;3,2,1,0;,
 4;7,6,5,4;,
 4;11,10,9,8;,
 4;15,14,13,12;,
 4;19,18,17,16;,
 4;23,22,21,20;,
 4;27,26,25,24;,
 4;31,30,29,28;,
 4;35,34,33,32;,
 4;39,38,37,36;,
 4;4,5,10,11;,
 4;16,17,41,40;,
 4;0,1,43,42;,
 4;22,45,44,21;,
 4;47,46,26,27;,
 4;49,48,30,31;,
 4;32,33,51,50;,
 4;53,39,36,52;,
 4;56,55,54,46;,
 4;58,57,48,49;,
 4;50,51,14,15;,
 4;60,53,52,59;,
 4;28,29,18,19;,
 4;24,25,2,3;,
 4;38,23,20,37;,
 4;8,9,34,35;,
 4;62,61,6,7;,
 4;64,16,40,63;,
 4;65,19,16,64;,
 4;66,28,19,65;,
 4;67,31,28,66;,
 4;68,49,31,67;,
 4;69,58,49,68;,
 4;12,13,71,70;,
 4;60,73,72,53;,
 4;53,72,74,39;,
 4;39,74,75,38;,
 4;38,75,76,23;,
 4;23,76,77,22;,
 4;22,77,78,45;,
 4;42,43,61,62;,
 4;64,63,80,79;,
 4;65,64,79,81;,
 4;66,65,81,82;,
 4;67,66,82,83;,
 4;68,67,83,54;,
 4;69,68,54,55;,
 4;71,85,84,70;,
 4;73,87,86,72;,
 4;72,86,88,74;,
 4;74,88,89,75;,
 4;75,89,90,76;,
 4;76,90,91,77;,
 4;77,91,92,78;,
 4;12,70,94,93;,
 4;15,12,93,95;,
 4;50,15,95,96;,
 4;32,50,96,97;,
 4;35,32,97,98;,
 4;8,35,98,99;,
 4;11,8,99,100;,
 4;4,11,100,101;,
 4;7,4,101,102;,
 4;62,7,102,103;,
 4;42,62,103,92;,
 4;0,42,92,91;,
 4;3,0,91,90;,
 4;24,3,90,89;,
 4;27,24,89,88;,
 4;47,27,88,86;,
 4;46,54,83,26;,
 4;26,83,82,25;,
 4;25,82,81,2;,
 4;2,81,79,1;,
 4;1,79,80,43;,
 4;43,80,104,61;,
 4;61,104,105,6;,
 4;6,105,106,5;,
 4;5,106,107,10;,
 4;10,107,108,9;,
 4;9,108,109,34;,
 4;34,109,110,33;,
 4;33,110,111,51;,
 4;51,111,112,14;,
 4;14,112,113,13;,
 4;13,113,114,71;,
 4;71,114,115,85;,
 4;116,47,86,87;,
 4;84,117,94,70;,
 4;116,56,46,47;,
 4;121,120,119,118;,
 4;123,122,120,121;,
 4;125,124,122,123;,
 4;127,126,124,125;,
 4;129,128,126,127;,
 4;131,130,128,129;,
 4;133,132,130,131;,
 4;135,134,132,133;,
 4;120,137,136,119;,
 4;122,138,137,120;,
 4;124,139,138,122;,
 4;126,140,139,124;,
 4;128,141,140,126;,
 4;130,142,141,128;,
 4;132,143,142,130;,
 4;134,144,143,132;,
 4;137,146,145,136;,
 4;138,147,146,137;,
 4;139,148,147,138;,
 4;140,149,148,139;,
 4;141,150,149,140;,
 4;142,151,150,141;,
 4;143,152,151,142;,
 4;144,153,152,143;,
 4;146,155,154,145;,
 4;147,156,155,146;,
 4;148,157,156,147;,
 4;149,158,157,148;,
 4;150,159,158,149;,
 4;151,160,159,150;,
 4;152,161,160,151;,
 4;153,162,161,152;,
 4;155,164,163,154;,
 4;156,165,164,155;,
 4;157,166,165,156;,
 4;158,167,166,157;,
 4;159,168,167,158;,
 4;160,169,168,159;,
 4;161,170,169,160;,
 4;162,171,170,161;,
 4;164,173,172,163;,
 4;165,174,173,164;,
 4;166,175,174,165;,
 4;167,176,175,166;,
 4;168,177,176,167;,
 4;169,178,177,168;,
 4;170,179,178,169;,
 4;171,180,179,170;,
 3;118,181,121;,
 3;121,182,123;,
 3;123,183,125;,
 3;125,184,127;,
 3;127,185,129;,
 3;129,186,131;,
 3;131,187,133;,
 3;133,188,135;,
 3;173,189,172;,
 3;174,190,173;,
 3;175,191,174;,
 3;176,192,175;,
 3;177,193,176;,
 3;178,194,177;,
 3;179,195,178;,
 3;180,196,179;;
 
 MeshMaterialList {
  1;
  154;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  176;
  0.811734;-0.037147;-0.582844;,
  -0.811734;-0.037147;-0.582844;,
  0.797823;0.043026;-0.601354;,
  -0.797820;0.043028;-0.601358;,
  0.825887;0.050217;0.561595;,
  -0.825885;0.050219;0.561598;,
  0.989428;-0.057432;-0.133172;,
  -0.989428;-0.057432;-0.133172;,
  0.842893;-0.187669;-0.504293;,
  -0.842891;-0.187669;-0.504297;,
  -0.865094;-0.127090;0.485242;,
  0.865097;-0.127090;0.485238;,
  0.837790;-0.013930;0.545815;,
  -0.837787;-0.013931;0.545819;,
  -0.816073;-0.062113;-0.574601;,
  0.816076;-0.062112;-0.574598;,
  0.844732;0.046138;-0.533197;,
  -0.844728;0.046139;-0.533203;,
  -0.860051;0.192043;0.472685;,
  0.860051;0.192043;0.472685;,
  0.814011;0.020706;-0.580481;,
  -0.814009;0.020709;-0.580484;,
  -0.871827;0.130012;0.472244;,
  0.942871;0.176094;0.282816;,
  -0.891153;-0.038614;0.452057;,
  -0.871962;-0.095395;-0.480189;,
  0.871966;-0.095394;-0.480183;,
  0.891154;-0.038616;0.452055;,
  0.997219;0.028159;0.069001;,
  -0.997219;0.028159;0.069001;,
  -0.998236;-0.033907;0.048737;,
  -0.991704;-0.128474;0.004185;,
  -0.999768;-0.020721;0.005883;,
  -0.985738;0.165520;0.030382;,
  -0.995999;0.076680;-0.045898;,
  -0.991043;-0.059100;-0.119754;,
  0.991043;-0.059100;-0.119754;,
  0.995999;0.076680;-0.045898;,
  0.985738;0.165520;0.030382;,
  0.999768;-0.020721;0.005883;,
  0.991704;-0.128474;0.004185;,
  0.998236;-0.033907;0.048737;,
  0.994056;0.027959;-0.105220;,
  -0.994056;0.027959;-0.105220;,
  -0.996442;-0.039570;-0.074416;,
  -0.989456;-0.141503;-0.030894;,
  -0.998353;-0.045827;-0.034500;,
  -0.986343;0.113185;-0.119654;,
  -0.971594;0.041573;-0.232972;,
  -0.960149;-0.057328;-0.273547;,
  0.960149;-0.057328;-0.273547;,
  0.971594;0.041573;-0.232973;,
  0.986343;0.113185;-0.119654;,
  0.998353;-0.045827;-0.034500;,
  0.989456;-0.141503;-0.030894;,
  0.996442;-0.039570;-0.074416;,
  -0.150220;-0.739997;0.655620;,
  -0.377280;-0.011299;-0.926030;,
  0.083452;-0.963742;0.253452;,
  -0.083452;-0.963742;0.253452;,
  0.033763;-0.935019;-0.352986;,
  -0.033764;-0.935020;-0.352985;,
  -0.066688;-0.863402;-0.500090;,
  0.066686;-0.863401;-0.500091;,
  -0.265166;0.132626;0.955038;,
  0.265168;0.132628;0.955037;,
  -0.298660;0.152265;0.942135;,
  0.298660;0.152265;0.942134;,
  0.313945;-0.059813;0.947555;,
  -0.313942;-0.059811;0.947556;,
  -0.277018;-0.102169;0.955418;,
  0.277023;-0.102169;0.955416;,
  0.253971;-0.003564;0.967205;,
  -0.253967;-0.003564;0.967206;,
  -0.005806;0.999106;-0.041888;,
  0.005806;0.999106;-0.041888;,
  -0.002906;0.999986;0.004453;,
  0.002906;0.999986;0.004453;,
  -0.000673;0.999529;0.030673;,
  0.000673;0.999529;0.030673;,
  0.242491;0.039586;-0.969346;,
  -0.242486;0.039587;-0.969347;,
  -0.241903;-0.086085;-0.966474;,
  0.241908;-0.086085;-0.966473;,
  0.262112;-0.189117;-0.946325;,
  -0.262108;-0.189117;-0.946327;,
  -0.311136;-0.100572;-0.945029;,
  0.311141;-0.100572;-0.945027;,
  0.342057;0.031117;-0.939164;,
  -0.342049;0.031117;-0.939167;,
  0.363351;0.042005;-0.930705;,
  -0.363347;0.042007;-0.930706;,
  -0.269132;-0.924735;0.269133;,
  -0.000000;-0.924734;0.380614;,
  0.269132;-0.924735;0.269133;,
  0.380610;-0.924736;0.000001;,
  0.269135;-0.924736;-0.269127;,
  0.000000;-0.924738;-0.380605;,
  -0.269135;-0.924736;-0.269127;,
  -0.380610;-0.924736;0.000001;,
  -0.498486;-0.709241;0.498487;,
  -0.000000;-0.709241;0.704966;,
  0.498486;-0.709241;0.498487;,
  0.704965;-0.709242;0.000002;,
  0.498488;-0.709238;-0.498488;,
  0.000000;-0.709236;-0.704971;,
  -0.498488;-0.709238;-0.498488;,
  -0.704965;-0.709242;0.000002;,
  -0.652730;-0.384561;0.652730;,
  -0.000000;-0.384562;0.923099;,
  0.652730;-0.384561;0.652730;,
  0.923100;-0.384559;0.000001;,
  0.652730;-0.384555;-0.652734;,
  0.000000;-0.384551;-0.923104;,
  -0.652730;-0.384555;-0.652734;,
  -0.923100;-0.384559;0.000001;,
  -0.707107;0.000004;0.707107;,
  0.000000;0.000004;1.000000;,
  0.707107;0.000004;0.707107;,
  1.000000;0.000004;0.000000;,
  0.707107;0.000004;-0.707107;,
  0.000000;0.000005;-1.000000;,
  -0.707107;0.000004;-0.707107;,
  -1.000000;0.000004;0.000000;,
  -0.652732;0.384553;0.652732;,
  0.000000;0.384554;0.923103;,
  0.652732;0.384553;0.652732;,
  0.923104;0.384551;0.000001;,
  0.652733;0.384547;-0.652736;,
  0.000000;0.384543;-0.923107;,
  -0.652733;0.384547;-0.652736;,
  -0.923104;0.384551;0.000001;,
  -0.498492;0.709232;0.498493;,
  0.000000;0.709233;0.704975;,
  0.498492;0.709232;0.498493;,
  0.704974;0.709233;0.000002;,
  0.498494;0.709230;-0.498494;,
  0.000000;0.709228;-0.704979;,
  -0.498494;0.709230;-0.498494;,
  -0.704974;0.709233;0.000002;,
  -0.269132;0.924735;0.269133;,
  0.000000;0.924734;0.380614;,
  0.269132;0.924735;0.269133;,
  0.380610;0.924736;0.000001;,
  0.269135;0.924736;-0.269127;,
  0.000000;0.924738;-0.380605;,
  -0.269135;0.924736;-0.269127;,
  -0.380610;0.924736;0.000001;,
  -0.000000;-1.000000;0.000002;,
  0.000000;1.000000;0.000002;,
  -0.253629;0.052213;0.965891;,
  0.253634;0.052212;0.965890;,
  0.257322;-0.009807;0.966276;,
  -0.257317;-0.009811;0.966277;,
  0.001345;0.999943;0.010550;,
  -0.001346;0.999943;0.010550;,
  0.150220;-0.739997;0.655620;,
  -0.067760;-0.926786;-0.369426;,
  -0.170277;-0.943674;0.283699;,
  -0.132988;-0.838436;-0.528526;,
  0.497288;-0.018953;0.867379;,
  -0.005813;0.999979;0.002676;,
  -0.011612;0.998900;-0.045439;,
  -0.001346;0.999509;0.031297;,
  -0.002691;0.999927;0.011798;,
  0.001346;0.999509;0.031297;,
  0.002691;0.999927;0.011798;,
  0.005812;0.999979;0.002675;,
  0.011611;0.998900;-0.045439;,
  -0.497279;-0.018960;0.867383;,
  0.067758;-0.926786;-0.369428;,
  0.132983;-0.838435;-0.528529;,
  0.170277;-0.943674;0.283699;,
  0.296119;-0.660586;0.689883;,
  0.377280;-0.011299;-0.926030;,
  -0.296119;-0.660586;0.689883;;
  154;
  4;84,85,82,83;,
  4;76,77,75,74;,
  4;72,73,70,71;,
  4;62,63,60,61;,
  4;31,10,13,30;,
  4;40,41,12,11;,
  4;88,89,86,87;,
  4;33,18,24,32;,
  4;68,69,66,67;,
  4;38,39,27,19;,
  4;151,150,73,72;,
  4;30,13,5,29;,
  4;83,82,81,80;,
  4;41,28,4,12;,
  4;90,91,89,88;,
  4;34,22,18,33;,
  4;67,66,64,65;,
  4;37,38,19,23;,
  4;57,1,21,91;,
  4;35,7,22,34;,
  4;65,64,153,152;,
  4;36,37,23,6;,
  4;32,24,10,31;,
  4;87,86,85,84;,
  4;39,40,11,27;,
  4;71,70,69,68;,
  4;78,79,77,76;,
  4;44,30,29,43;,
  4;45,31,30,44;,
  4;46,32,31,45;,
  4;47,33,32,46;,
  4;48,34,33,47;,
  4;49,35,34,48;,
  4;61,60,58,59;,
  4;36,50,51,37;,
  4;37,51,52,38;,
  4;38,52,53,39;,
  4;39,53,54,40;,
  4;40,54,55,41;,
  4;41,55,42,28;,
  4;155,154,79,78;,
  4;44,43,3,14;,
  4;45,44,14,9;,
  4;46,45,9,25;,
  4;47,46,25,17;,
  4;48,47,17,21;,
  4;49,48,21,1;,
  4;58,156,56,59;,
  4;50,0,20,51;,
  4;51,20,16,52;,
  4;52,16,26,53;,
  4;53,26,8,54;,
  4;54,8,15,55;,
  4;55,15,2,42;,
  4;61,59,158,157;,
  4;62,61,157,159;,
  4;65,152,160,160;,
  4;67,65,23,19;,
  4;68,67,19,27;,
  4;71,68,27,11;,
  4;72,71,11,12;,
  4;151,72,12,4;,
  4;76,74,162,161;,
  4;78,76,161,163;,
  4;155,78,163,164;,
  4;83,80,2,15;,
  4;84,83,15,8;,
  4;87,84,8,26;,
  4;88,87,26,16;,
  4;90,88,16,20;,
  4;91,21,17,89;,
  4;89,17,25,86;,
  4;86,25,9,85;,
  4;85,9,14,82;,
  4;82,14,3,81;,
  4;154,166,165,79;,
  4;79,165,167,77;,
  4;77,167,168,75;,
  4;150,5,13,73;,
  4;73,13,10,70;,
  4;70,10,24,69;,
  4;69,24,18,66;,
  4;66,18,22,64;,
  4;64,22,169,153;,
  4;63,171,170,60;,
  4;60,170,172,58;,
  4;58,172,173,156;,
  4;174,90,20,0;,
  4;56,175,158,59;,
  4;174,57,91,90;,
  4;93,101,100,92;,
  4;94,102,101,93;,
  4;95,103,102,94;,
  4;96,104,103,95;,
  4;97,105,104,96;,
  4;98,106,105,97;,
  4;99,107,106,98;,
  4;92,100,107,99;,
  4;101,109,108,100;,
  4;102,110,109,101;,
  4;103,111,110,102;,
  4;104,112,111,103;,
  4;105,113,112,104;,
  4;106,114,113,105;,
  4;107,115,114,106;,
  4;100,108,115,107;,
  4;109,117,116,108;,
  4;110,118,117,109;,
  4;111,119,118,110;,
  4;112,120,119,111;,
  4;113,121,120,112;,
  4;114,122,121,113;,
  4;115,123,122,114;,
  4;108,116,123,115;,
  4;117,125,124,116;,
  4;118,126,125,117;,
  4;119,127,126,118;,
  4;120,128,127,119;,
  4;121,129,128,120;,
  4;122,130,129,121;,
  4;123,131,130,122;,
  4;116,124,131,123;,
  4;125,133,132,124;,
  4;126,134,133,125;,
  4;127,135,134,126;,
  4;128,136,135,127;,
  4;129,137,136,128;,
  4;130,138,137,129;,
  4;131,139,138,130;,
  4;124,132,139,131;,
  4;133,141,140,132;,
  4;134,142,141,133;,
  4;135,143,142,134;,
  4;136,144,143,135;,
  4;137,145,144,136;,
  4;138,146,145,137;,
  4;139,147,146,138;,
  4;132,140,147,139;,
  3;92,148,93;,
  3;93,148,94;,
  3;94,148,95;,
  3;95,148,96;,
  3;96,148,97;,
  3;97,148,98;,
  3;98,148,99;,
  3;99,148,92;,
  3;141,149,140;,
  3;142,149,141;,
  3;143,149,142;,
  3;144,149,143;,
  3;145,149,144;,
  3;146,149,145;,
  3;147,149,146;,
  3;140,149,147;;
 }
 MeshTextureCoords {
  197;
  0.449870;0.818730;,
  0.550130;0.818730;,
  0.550130;0.882460;,
  0.449870;0.882460;,
  0.449870;0.500000;,
  0.550130;0.500000;,
  0.550130;0.600630;,
  0.449870;0.600630;,
  0.449870;0.367540;,
  0.550130;0.367540;,
  0.550130;0.431270;,
  0.449870;0.431270;,
  0.449870;0.149370;,
  0.550130;0.149370;,
  0.550130;0.250000;,
  0.449870;0.250000;,
  0.774370;0.818730;,
  0.875000;0.818730;,
  0.875000;0.882460;,
  0.774370;0.882460;,
  0.125000;0.882460;,
  0.125000;0.818730;,
  0.225630;0.818730;,
  0.225630;0.882460;,
  0.449870;0.916630;,
  0.550130;0.916630;,
  0.550130;0.952040;,
  0.449870;0.952040;,
  0.774370;0.916630;,
  0.875000;0.916630;,
  0.875000;0.952040;,
  0.774370;0.952040;,
  0.449870;0.297960;,
  0.550130;0.297960;,
  0.550130;0.333370;,
  0.449870;0.333370;,
  0.125000;0.952040;,
  0.125000;0.916630;,
  0.225630;0.916630;,
  0.225630;0.952040;,
  0.774370;0.750000;,
  0.875000;0.750000;,
  0.449870;0.750000;,
  0.550130;0.750000;,
  0.125000;0.750000;,
  0.225630;0.750000;,
  0.550130;0.979440;,
  0.449870;0.979440;,
  0.875000;0.979440;,
  0.774370;0.979440;,
  0.449870;0.270560;,
  0.550130;0.270560;,
  0.125000;0.979440;,
  0.225630;0.979440;,
  0.625000;0.979440;,
  0.625000;1.000000;,
  0.580360;1.000000;,
  0.875000;1.000000;,
  0.774370;1.000000;,
  0.125000;1.000000;,
  0.225630;1.000000;,
  0.550130;0.684570;,
  0.449870;0.684570;,
  0.690430;0.750000;,
  0.690430;0.818730;,
  0.690430;0.882460;,
  0.690430;0.916630;,
  0.690430;0.952040;,
  0.690430;0.979440;,
  0.690430;1.000000;,
  0.449870;0.065430;,
  0.550130;0.065430;,
  0.309570;0.979440;,
  0.309570;1.000000;,
  0.309570;0.952040;,
  0.309570;0.916630;,
  0.309570;0.882460;,
  0.309570;0.818730;,
  0.309570;0.750000;,
  0.625000;0.818730;,
  0.625000;0.750000;,
  0.625000;0.882460;,
  0.625000;0.916630;,
  0.625000;0.952040;,
  0.419640;0.000000;,
  0.580360;0.000000;,
  0.375000;0.979440;,
  0.375000;1.000000;,
  0.375000;0.952040;,
  0.375000;0.916630;,
  0.375000;0.882460;,
  0.375000;0.818730;,
  0.375000;0.750000;,
  0.375000;0.149370;,
  0.375000;0.065430;,
  0.375000;0.250000;,
  0.375000;0.270560;,
  0.375000;0.297960;,
  0.375000;0.333370;,
  0.375000;0.367540;,
  0.375000;0.431270;,
  0.375000;0.500000;,
  0.375000;0.600630;,
  0.375000;0.684570;,
  0.625000;0.684570;,
  0.625000;0.600630;,
  0.625000;0.500000;,
  0.625000;0.431270;,
  0.625000;0.367540;,
  0.625000;0.333370;,
  0.625000;0.297960;,
  0.625000;0.270560;,
  0.625000;0.250000;,
  0.625000;0.149370;,
  0.625000;0.065430;,
  0.625000;0.000000;,
  0.419640;1.000000;,
  0.375000;0.000000;,
  0.000000;0.875000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.125000;0.875000;,
  0.250000;0.750000;,
  0.250000;0.875000;,
  0.375000;0.750000;,
  0.375000;0.875000;,
  0.500000;0.750000;,
  0.500000;0.875000;,
  0.625000;0.750000;,
  0.625000;0.875000;,
  0.750000;0.750000;,
  0.750000;0.875000;,
  0.875000;0.750000;,
  0.875000;0.875000;,
  1.000000;0.750000;,
  1.000000;0.875000;,
  0.000000;0.625000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.375000;,
  0.125000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.000000;0.250000;,
  0.125000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.000000;0.125000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.375000;0.125000;,
  0.500000;0.125000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  1.000000;0.125000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;;
 }
}