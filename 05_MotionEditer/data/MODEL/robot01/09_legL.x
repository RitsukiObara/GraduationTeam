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
 256;
 5.23491;2.67362;-5.35746;,
 6.66989;1.92294;-5.35746;,
 6.66989;-2.66656;-5.35746;,
 5.23491;-3.44492;-5.35746;,
 5.80547;3.61422;-4.06115;,
 7.72425;2.63274;-4.06115;,
 7.72425;-3.36784;-4.06115;,
 5.78375;-4.38552;-4.06115;,
 0.37147;-3.36784;-4.06115;,
 0.37147;2.63274;-4.06115;,
 1.42581;1.92294;-5.35746;,
 1.42581;-2.66656;-5.35746;,
 2.29025;3.61422;-4.06115;,
 2.86081;2.67362;-5.35746;,
 2.86081;-3.44492;-5.35746;,
 2.29025;-4.38552;-4.06115;,
 8.16373;-3.95244;-4.29567;,
 8.16373;-9.38759;-4.29567;,
 5.96139;-9.38759;-4.29567;,
 5.96139;-1.46404;-4.29567;,
 5.96139;4.82946;1.88391;,
 8.16373;3.72186;1.88391;,
 8.16373;3.86322;-0.82766;,
 5.96139;4.97084;-0.82766;,
 5.96139;-9.38759;3.78878;,
 8.16373;-9.38759;3.78878;,
 8.16373;-3.95244;3.78878;,
 5.96139;-1.46404;3.78878;,
 5.15125;-15.54207;-0.57602;,
 6.40481;-15.54207;-0.57602;,
 6.40481;-15.64171;0.77542;,
 5.15125;-15.64171;0.77542;,
 8.16373;-3.99124;1.88391;,
 8.16373;-9.38759;1.88391;,
 8.16373;-9.38759;-0.82766;,
 8.16373;-3.90740;-0.82766;,
 -0.00195;-2.07596;-0.82766;,
 -0.00195;-9.38759;-0.82766;,
 -0.00195;-9.38759;1.88391;,
 -0.00195;-2.15982;1.88391;,
 -0.00195;-9.38759;-4.29567;,
 2.20041;-9.38759;-4.29567;,
 1.68739;-11.33055;-2.56690;,
 6.47441;-11.33055;-2.56690;,
 6.47441;-11.38529;0.95036;,
 6.47441;-11.36127;-0.59298;,
 6.47441;-11.40215;2.03459;,
 5.18331;-11.40215;2.03459;,
 1.68739;-11.36127;-0.59298;,
 1.68739;-11.38529;0.95036;,
 8.16373;3.83180;-2.60835;,
 5.96139;4.93940;-2.60835;,
 8.16373;-9.38759;-2.60835;,
 8.16373;-3.92604;-2.60835;,
 6.47441;-11.34549;-1.60652;,
 5.15125;-15.47663;-1.46354;,
 6.40481;-15.47663;-1.46354;,
 -0.00195;-9.38759;-2.60835;,
 1.68739;-11.34549;-1.60652;,
 -0.00195;-2.09462;-2.60835;,
 8.16373;2.86374;-4.29567;,
 5.96139;3.97138;-4.29567;,
 8.16373;-9.38759;-4.29567;,
 8.16373;-3.95244;-4.29567;,
 6.47441;-11.33055;-2.56690;,
 5.15125;-15.41463;-2.30450;,
 6.40481;-15.41463;-2.30450;,
 -0.00195;-9.38759;-4.29567;,
 1.68739;-11.33055;-2.56690;,
 -0.00195;-2.12100;-4.29567;,
 6.40481;-15.71171;1.72482;,
 5.15125;-15.71171;1.72482;,
 8.16373;-9.38759;3.78878;,
 6.47441;-11.40215;2.03459;,
 8.16373;-3.95244;3.78878;,
 5.96139;3.97138;3.78878;,
 8.16373;2.86374;3.78878;,
 -0.00195;-9.38759;3.78878;,
 -0.00195;-2.12100;3.78878;,
 1.68739;-11.40215;2.03459;,
 1.75697;-15.47663;-1.46354;,
 1.75697;-15.41463;-2.30450;,
 3.01053;-15.47663;-1.46354;,
 3.01053;-15.41463;-2.30450;,
 1.75697;-15.54207;-0.57602;,
 3.01053;-15.54207;-0.57602;,
 1.75697;-15.64171;0.77542;,
 3.01053;-15.64171;0.77542;,
 1.75697;-15.71171;1.72482;,
 3.01053;-15.71171;1.72482;,
 -0.00195;-9.38759;3.78878;,
 1.68739;-11.40215;2.03459;,
 2.97847;-11.40215;2.03459;,
 2.20041;-9.38759;3.78878;,
 -0.00195;-2.12100;3.78878;,
 2.20041;-1.46404;3.78878;,
 -0.00195;3.72186;1.88391;,
 -0.00195;2.86374;3.78878;,
 2.20041;3.97138;3.78878;,
 2.20041;4.82946;1.88391;,
 -0.00195;3.86322;-0.82766;,
 2.20041;4.97084;-0.82766;,
 -0.00195;3.83180;-2.60835;,
 2.20041;4.93940;-2.60835;,
 -0.00195;2.86374;-4.29567;,
 2.20041;3.97138;-4.29567;,
 -0.00195;-2.12100;-4.29567;,
 2.20041;-1.46404;-4.29567;,
 -0.00195;2.86374;3.78878;,
 2.20041;3.97138;3.78878;,
 5.96139;3.97138;3.78878;,
 8.16373;2.86374;3.78878;,
 9.91161;2.62626;1.88391;,
 9.91161;1.76816;3.78878;,
 9.96087;-2.28876;3.78878;,
 9.96087;-2.32756;1.88391;,
 9.91161;2.76762;-0.82766;,
 9.96087;-2.24372;-0.82766;,
 9.91161;2.73620;-2.60835;,
 9.96087;-2.26236;-2.60835;,
 9.96087;-2.28876;-4.29567;,
 9.91161;1.76816;-4.29567;,
 5.96139;3.97138;-4.29567;,
 8.16373;2.86374;-4.29567;,
 2.20041;3.97138;-4.29567;,
 -0.00195;2.86374;-4.29567;,
 -0.00195;3.83180;-2.60835;,
 -0.00195;2.86374;-4.29567;,
 -0.00195;3.86322;-0.82766;,
 -0.00195;3.72186;1.88391;,
 -0.00195;2.86374;3.78878;,
 9.96087;-2.28876;3.78878;,
 9.91161;1.76816;3.78878;,
 9.91161;1.76816;3.78878;,
 9.91161;2.62626;1.88391;,
 8.16373;3.72186;1.88391;,
 8.16373;3.86322;-0.82766;,
 8.16373;3.83180;-2.60835;,
 9.91161;2.73620;-2.60835;,
 9.91161;1.76816;-4.29567;,
 9.91161;1.76816;-4.29567;,
 9.96087;-2.28876;-4.29567;,
 1.75697;-15.41463;-2.30450;,
 3.01053;-15.41463;-2.30450;,
 5.15125;-15.41463;-2.30450;,
 6.40481;-15.41463;-2.30450;,
 6.40481;-15.41463;-2.30450;,
 6.40481;-15.47663;-1.46354;,
 6.40481;-15.54207;-0.57602;,
 6.40481;-15.64171;0.77542;,
 6.40481;-15.71171;1.72482;,
 5.15125;-15.71171;1.72482;,
 6.40481;-15.71171;1.72482;,
 3.01053;-15.71171;1.72482;,
 1.75697;-15.71171;1.72482;,
 1.75697;-15.71171;1.72482;,
 1.75697;-15.64171;0.77542;,
 1.75697;-15.54207;-0.57602;,
 1.75697;-15.47663;-1.46354;,
 1.75697;-15.41463;-2.30450;,
 5.16475;5.30548;2.46120;,
 6.38529;5.31854;1.82283;,
 6.38529;5.39834;-2.08004;,
 5.16475;5.41188;-2.74194;,
 5.94737;4.05284;3.58745;,
 7.72425;3.92616;1.89140;,
 7.72425;4.05092;-2.60732;,
 5.94155;4.09692;-4.06184;,
 0.37147;4.05092;-2.60732;,
 0.37147;3.92616;1.89140;,
 1.71043;5.31854;1.82283;,
 1.71043;5.39834;-2.08004;,
 2.14835;4.05284;3.58745;,
 2.93097;5.30548;2.46120;,
 2.93097;5.41188;-2.74194;,
 2.13243;4.09692;-4.06184;,
 9.91163;1.89046;-4.57188;,
 11.10669;1.58242;-4.57188;,
 11.09275;0.46850;-4.57188;,
 9.89405;0.48348;-4.57188;,
 9.91163;1.89046;3.92250;,
 11.10669;1.58242;3.92250;,
 11.10669;1.58242;-4.57188;,
 9.91163;1.89046;-4.57188;,
 9.89405;0.48348;3.92250;,
 11.09275;0.46850;3.92250;,
 11.10669;1.58242;3.92250;,
 9.91163;1.89046;3.92250;,
 9.89405;0.48348;-4.57188;,
 11.09275;0.46850;-4.57188;,
 11.09275;0.46850;3.92250;,
 9.89405;0.48348;3.92250;,
 11.09275;0.46850;-4.57188;,
 11.10669;1.58242;-4.57188;,
 11.10669;1.58242;3.92250;,
 11.09275;0.46850;3.92250;,
 8.79335;0.49724;3.92250;,
 8.80727;1.61118;3.92250;,
 8.80727;1.61118;-4.57188;,
 8.79335;0.49724;-4.57188;,
 8.79335;0.49724;3.92250;,
 8.79335;0.49724;-4.57188;,
 8.80727;1.61118;3.92250;,
 8.79335;0.49724;3.92250;,
 8.80727;1.61118;-4.57188;,
 8.80727;1.61118;3.92250;,
 8.79335;0.49724;-4.57188;,
 8.80727;1.61118;-4.57188;,
 9.91163;-0.70496;-4.57188;,
 11.10669;-1.01300;-4.57188;,
 11.09275;-2.12694;-4.57188;,
 9.89405;-2.11194;-4.57188;,
 9.91163;-0.70496;3.92250;,
 11.10669;-1.01300;3.92250;,
 11.10669;-1.01300;-4.57188;,
 9.91163;-0.70496;-4.57188;,
 9.89405;-2.11194;3.92250;,
 11.09275;-2.12694;3.92250;,
 11.10669;-1.01300;3.92250;,
 9.91163;-0.70496;3.92250;,
 9.89405;-2.11194;-4.57188;,
 11.09275;-2.12694;-4.57188;,
 11.09275;-2.12694;3.92250;,
 9.89405;-2.11194;3.92250;,
 11.09275;-2.12694;-4.57188;,
 11.10669;-1.01300;-4.57188;,
 11.10669;-1.01300;3.92250;,
 11.09275;-2.12694;3.92250;,
 8.79335;-2.09818;3.92250;,
 8.80727;-0.98426;3.92250;,
 8.80727;-0.98426;-4.57188;,
 8.79335;-2.09818;-4.57188;,
 8.79335;-2.09818;3.92250;,
 8.79335;-2.09818;-4.57188;,
 8.80727;-0.98426;3.92250;,
 8.79335;-2.09818;3.92250;,
 8.80727;-0.98426;-4.57188;,
 8.80727;-0.98426;3.92250;,
 8.79335;-2.09818;-4.57188;,
 8.80727;-0.98426;-4.57188;,
 3.12233;-0.41278;5.35745;,
 2.48661;-0.74534;5.35745;,
 2.48661;-2.77860;5.35745;,
 3.12233;-3.12342;5.35745;,
 2.29025;3.61422;3.74114;,
 0.37147;2.63274;3.74114;,
 0.37147;-3.36784;3.74114;,
 2.31195;-4.38552;3.74114;,
 7.72425;-3.36784;3.74114;,
 7.72425;2.63274;3.74114;,
 5.60909;-0.74534;5.35745;,
 5.60909;-2.77860;5.35745;,
 5.80547;3.61422;3.74114;,
 4.97337;-0.41278;5.35745;,
 4.97337;-3.12342;5.35745;,
 5.80547;-4.38552;3.74114;;
 
 150;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;3,2,6,7;,
 4;2,1,5,6;,
 4;8,9,10,11;,
 4;10,9,12,13;,
 4;11,10,13,14;,
 4;8,11,14,15;,
 3;14,3,15;,
 4;13,0,3,14;,
 4;12,4,0,13;,
 3;3,7,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 3;40,41,42;,
 3;42,41,43;,
 3;41,18,43;,
 3;43,18,17;,
 4;34,33,44,45;,
 4;46,25,24,47;,
 4;38,37,48,49;,
 4;23,22,50,51;,
 4;35,34,52,53;,
 4;52,34,45,54;,
 4;55,56,29,28;,
 4;37,57,58,48;,
 4;59,57,37,36;,
 4;51,50,60,61;,
 4;62,63,53,52;,
 4;64,62,52,54;,
 4;65,66,56,55;,
 4;57,67,68,58;,
 4;69,67,57,59;,
 4;31,30,70,71;,
 4;33,72,73,44;,
 4;74,72,33,32;,
 4;75,76,21,20;,
 4;77,78,39,38;,
 4;79,77,38,49;,
 3;80,81,82;,
 3;81,83,82;,
 3;82,83,65;,
 4;84,80,82,85;,
 4;86,84,85,87;,
 4;88,86,87,89;,
 4;90,91,92,93;,
 4;94,90,93,95;,
 4;96,97,98,99;,
 4;100,96,99,101;,
 4;102,100,101,103;,
 4;104,102,103,105;,
 4;40,106,107,41;,
 4;19,18,41,107;,
 4;103,51,61,105;,
 4;101,23,51,103;,
 4;99,20,23,101;,
 4;98,75,20,99;,
 4;93,24,27,95;,
 4;47,24,93,92;,
 4;87,31,71,89;,
 4;85,28,31,87;,
 4;82,55,28,85;,
 3;65,55,82;,
 4;108,94,95,109;,
 4;27,110,109,95;,
 4;26,111,110,27;,
 4;112,113,114,115;,
 4;116,112,115,117;,
 4;118,116,117,119;,
 4;120,121,118,119;,
 4;122,123,16,19;,
 4;124,122,19,107;,
 4;106,125,124,107;,
 4;126,127,69,59;,
 4;128,126,59,36;,
 4;129,128,36,39;,
 4;78,130,129,39;,
 4;32,115,114,74;,
 4;26,131,132,111;,
 4;76,133,134,21;,
 4;35,117,115,32;,
 4;135,112,116,136;,
 4;53,119,117,35;,
 4;136,116,118,137;,
 4;63,120,119,53;,
 4;50,138,139,60;,
 4;123,140,141,16;,
 3;42,43,142;,
 3;142,43,143;,
 3;143,43,144;,
 3;144,43,145;,
 4;146,64,54,147;,
 4;45,148,147,54;,
 4;44,149,148,45;,
 4;73,150,149,44;,
 4;151,152,46,47;,
 4;153,151,47,92;,
 4;91,154,153,92;,
 4;155,79,49,156;,
 4;48,157,156,49;,
 4;58,158,157,48;,
 4;68,159,158,58;,
 4;160,161,162,163;,
 4;164,165,161,160;,
 4;163,162,166,167;,
 4;162,161,165,166;,
 4;168,169,170,171;,
 4;170,169,172,173;,
 4;171,170,173,174;,
 4;168,171,174,175;,
 3;174,163,175;,
 4;173,160,163,174;,
 4;172,164,160,173;,
 3;163,167,175;,
 4;176,177,178,179;,
 4;180,181,182,183;,
 4;184,185,186,187;,
 4;188,189,190,191;,
 4;192,193,194,195;,
 4;196,197,198,199;,
 4;200,201,188,191;,
 4;202,203,184,187;,
 4;204,205,180,183;,
 4;206,207,176,179;,
 4;208,209,210,211;,
 4;212,213,214,215;,
 4;216,217,218,219;,
 4;220,221,222,223;,
 4;224,225,226,227;,
 4;228,229,230,231;,
 4;232,233,220,223;,
 4;234,235,216,219;,
 4;236,237,212,215;,
 4;238,239,208,211;,
 4;240,241,242,243;,
 4;244,245,241,240;,
 4;243,242,246,247;,
 4;242,241,245,246;,
 4;248,249,250,251;,
 4;250,249,252,253;,
 4;251,250,253,254;,
 4;248,251,254,255;,
 3;254,243,255;,
 4;253,240,243,254;,
 4;252,244,240,253;,
 3;243,247,255;;
 
 MeshMaterialList {
  1;
  150;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "ac2_02.png";
   }
  }
 }
 MeshNormals {
  164;
  -0.431060;-0.254506;-0.865687;,
  0.431072;-0.255613;-0.865355;,
  -0.427820;0.256942;-0.866574;,
  0.427817;0.256941;-0.866576;,
  -0.613883;0.368689;-0.698009;,
  0.613880;0.368687;-0.698013;,
  -0.617821;-0.364773;-0.696590;,
  0.617818;-0.366348;-0.695766;,
  -0.178070;0.756385;-0.629423;,
  0.178070;0.756384;-0.629424;,
  -0.097737;0.415156;-0.904485;,
  0.097737;0.415156;-0.904485;,
  -0.100529;-0.413703;-0.904845;,
  0.080290;-0.512627;-0.854849;,
  -0.121939;-0.776293;-0.618465;,
  0.182947;-0.755513;-0.629071;,
  0.000000;-0.997293;-0.073525;,
  0.000000;-0.997293;-0.073525;,
  -0.399842;0.795034;-0.456123;,
  0.439162;0.779267;-0.447079;,
  -0.416815;0.828782;0.373345;,
  0.456957;0.810999;0.365337;,
  0.000000;-0.997293;-0.073529;,
  0.000000;-0.355610;-0.934635;,
  0.000000;-0.350600;0.936525;,
  -0.939695;-0.342004;-0.002660;,
  -0.449247;0.893275;0.015388;,
  0.490691;0.871204;0.015009;,
  0.938012;-0.346592;-0.002701;,
  0.000000;-0.997293;-0.073533;,
  0.000000;-0.997293;-0.073533;,
  -0.938011;-0.346595;-0.002701;,
  -0.435916;0.866766;-0.242265;,
  0.476842;0.846387;-0.237173;,
  0.937110;-0.349023;-0.002714;,
  0.000000;-0.997293;-0.073531;,
  0.000000;-0.997293;-0.073531;,
  -0.937109;-0.349025;-0.002714;,
  0.000000;-0.997293;-0.073529;,
  0.000000;-0.997293;-0.073529;,
  0.939071;-0.343714;-0.002679;,
  0.480063;0.852176;0.208169;,
  -0.438997;0.872892;0.212935;,
  -0.939070;-0.343717;-0.002679;,
  0.000000;-0.997293;-0.073525;,
  0.000000;-0.997293;-0.073531;,
  0.000000;-0.997293;-0.073529;,
  0.000000;-0.350600;0.936525;,
  0.000000;-0.350600;0.936525;,
  -0.213317;0.890775;0.401267;,
  0.213319;0.890775;0.401267;,
  0.225645;0.946650;0.230083;,
  -0.225643;0.946650;0.230083;,
  -0.230874;0.972839;0.016756;,
  0.230875;0.972839;0.016756;,
  -0.224242;0.939013;-0.260710;,
  0.224244;0.939013;-0.260710;,
  -0.204225;0.849108;-0.487142;,
  0.204227;0.849108;-0.487141;,
  0.000000;-0.425501;-0.904958;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.916347;-0.400378;-0.002111;,
  0.916338;-0.400384;-0.004093;,
  0.916325;-0.400401;0.005228;,
  0.000000;0.000000;-1.000000;,
  0.918431;-0.395579;-0.001425;,
  0.945823;0.324214;0.017439;,
  0.918500;-0.395311;0.009332;,
  0.999937;0.011042;0.002116;,
  0.872602;0.488360;0.008395;,
  0.918302;-0.395860;-0.004028;,
  0.871560;0.472222;-0.131869;,
  0.918402;-0.395624;0.004403;,
  0.918517;-0.395354;0.004682;,
  0.999935;0.010998;-0.002922;,
  0.000000;-0.488069;-0.872805;,
  0.000000;-0.339624;-0.940561;,
  0.934133;-0.356911;-0.003104;,
  0.935099;-0.354372;-0.003084;,
  0.936230;-0.351373;-0.003056;,
  0.000000;-0.383967;0.923347;,
  0.000000;-0.383967;0.923347;,
  -0.936893;-0.349603;-0.003034;,
  -0.936230;-0.351374;-0.003056;,
  -0.935099;-0.354372;-0.003084;,
  -0.934132;-0.356915;-0.003106;,
  -0.409504;0.891688;-0.192872;,
  0.409414;0.891471;-0.194059;,
  -0.430183;0.872457;0.231866;,
  0.430183;0.872457;0.231866;,
  -0.615059;0.720300;0.320734;,
  0.615058;0.720301;0.320734;,
  -0.589693;0.754314;-0.288571;,
  0.589577;0.753744;-0.290292;,
  -0.231587;0.688519;0.687247;,
  0.231587;0.688519;0.687247;,
  -0.123683;0.917393;0.378276;,
  0.123683;0.917393;0.378276;,
  -0.108022;0.936254;-0.334304;,
  0.086191;0.902596;-0.421771;,
  -0.134782;0.729023;-0.671088;,
  0.202236;0.732893;-0.649592;,
  -0.999922;0.012495;0.000000;,
  0.249603;0.968348;0.000000;,
  0.002287;0.999997;0.000000;,
  -0.999922;0.012495;0.000000;,
  0.249602;0.968349;0.000000;,
  0.002279;0.999997;0.000000;,
  0.348852;-0.235876;0.907009;,
  -0.348849;-0.236949;0.906730;,
  0.273827;0.117533;0.954570;,
  -0.273829;0.117533;0.954570;,
  0.406722;0.174575;0.896717;,
  -0.406724;0.174575;0.896716;,
  0.508942;-0.344121;0.789024;,
  -0.508947;-0.345694;0.788333;,
  0.086988;0.355137;0.930758;,
  -0.086988;0.355137;0.930758;,
  0.044180;0.180368;0.982607;,
  -0.044180;0.180368;0.982607;,
  0.095657;-0.396545;0.913018;,
  -0.076456;-0.492702;0.866833;,
  0.117069;-0.752357;0.648270;,
  -0.175657;-0.730649;0.659770;,
  0.936638;-0.350288;-0.002725;,
  0.916321;-0.400395;0.006265;,
  0.933631;-0.358224;-0.003118;,
  -0.936637;-0.350290;-0.002725;,
  -0.933628;-0.358231;-0.003122;,
  0.939696;-0.342001;-0.002660;,
  0.936893;-0.349602;-0.003034;,
  0.916313;-0.400380;0.008155;,
  0.000000;-0.350600;0.936525;,
  0.000000;-0.383967;0.923347;,
  0.496185;0.791603;0.356601;,
  0.477655;0.762040;-0.437198;,
  0.000000;-0.064117;-0.997942;,
  0.000000;-0.064117;-0.997942;,
  0.000000;-0.064117;-0.997942;,
  0.000000;-0.064117;-0.997942;,
  0.999856;-0.016972;-0.000755;,
  0.999858;-0.016862;-0.000750;,
  0.999862;-0.016613;-0.000738;,
  0.999866;-0.016337;-0.000725;,
  0.999869;-0.016201;-0.000719;,
  0.000000;-0.071692;0.997427;,
  0.000000;-0.071692;0.997427;,
  0.000000;-0.071692;0.997427;,
  -0.999869;-0.016196;-0.000719;,
  -0.999866;-0.016332;-0.000725;,
  -0.999862;-0.016608;-0.000738;,
  -0.999857;-0.016865;-0.000753;,
  -0.999856;-0.016982;-0.000762;,
  -0.012498;-0.999922;0.000000;,
  -0.012496;-0.999922;0.000000;,
  0.999922;-0.012514;-0.000000;,
  -0.012500;-0.999922;0.000000;,
  -0.245170;0.969480;0.000000;,
  -0.012506;-0.999922;0.000000;,
  -0.012513;-0.999922;0.000000;,
  0.999922;-0.012514;-0.000000;,
  -0.012500;-0.999922;0.000000;,
  -0.245187;0.969476;0.000000;;
  150;
  4;11,3,1,13;,
  4;9,5,3,11;,
  4;13,1,7,15;,
  4;1,3,5,7;,
  4;6,4,2,0;,
  4;2,4,8,10;,
  4;0,2,10,12;,
  4;6,0,12,14;,
  3;12,13,14;,
  4;10,11,13,12;,
  4;8,9,11,10;,
  3;13,15,14;,
  4;65,23,23,65;,
  4;51,41,27,54;,
  4;47,24,61,61;,
  4;29,29,39,39;,
  4;62,40,28,63;,
  4;60,31,43,60;,
  3;23,59,76;,
  3;76,59,77;,
  3;59,23,77;,
  3;77,23,23;,
  4;28,40,80,79;,
  4;81,24,47,82;,
  4;43,31,85,84;,
  4;54,27,33,56;,
  4;63,28,34,64;,
  4;34,28,79,78;,
  4;45,35,29,29;,
  4;31,37,86,85;,
  4;60,37,31,60;,
  4;56,33,19,58;,
  4;125,126,64,34;,
  4;127,125,34,78;,
  4;44,17,35,45;,
  4;37,128,129,86;,
  4;60,128,37,60;,
  4;39,39,22,22;,
  4;40,130,131,80;,
  4;132,130,40,62;,
  4;50,21,41,51;,
  4;25,60,60,43;,
  4;83,25,43,84;,
  3;36,16,46;,
  3;16,16,46;,
  3;46,16,44;,
  4;30,36,46,30;,
  4;38,30,30,38;,
  4;22,38,38,22;,
  4;133,134,81,48;,
  4;61,133,48,61;,
  4;42,20,49,52;,
  4;26,42,52,53;,
  4;32,26,53,55;,
  4;18,32,55,57;,
  4;23,65,65,59;,
  4;65,23,59,65;,
  4;55,56,58,57;,
  4;53,54,56,55;,
  4;52,51,54,53;,
  4;49,50,51,52;,
  4;48,47,61,61;,
  4;82,47,48,81;,
  4;38,39,22,22;,
  4;30,29,39,38;,
  4;46,45,29,30;,
  3;44,45,46;,
  4;61,61,61,61;,
  4;61,61,61,61;,
  4;61,61,61,61;,
  4;67,69,68,66;,
  4;70,67,66,71;,
  4;72,70,71,73;,
  4;74,75,72,73;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;60,60,60,60;,
  4;60,60,60,60;,
  4;60,60,60,60;,
  4;60,60,60,60;,
  4;62,66,68,132;,
  4;61,61,61,61;,
  4;21,135,135,41;,
  4;63,71,66,62;,
  4;41,67,70,27;,
  4;64,73,71,63;,
  4;27,70,72,33;,
  4;126,74,73,64;,
  4;33,72,136,19;,
  4;65,65,65,65;,
  3;76,77,137;,
  3;137,77,138;,
  3;138,77,139;,
  3;139,77,140;,
  4;141,127,78,142;,
  4;79,143,142,78;,
  4;80,144,143,79;,
  4;131,145,144,80;,
  4;146,147,81,82;,
  4;146,146,82,81;,
  4;134,148,146,81;,
  4;149,83,84,150;,
  4;85,151,150,84;,
  4;86,152,151,85;,
  4;129,153,152,86;,
  4;98,90,88,100;,
  4;96,92,90,98;,
  4;100,88,94,102;,
  4;88,90,92,94;,
  4;93,91,89,87;,
  4;89,91,95,97;,
  4;87,89,97,99;,
  4;93,87,99,101;,
  3;99,100,101;,
  4;97,98,100,99;,
  4;95,96,98,97;,
  3;100,102,101;,
  4;65,65,65,65;,
  4;105,104,104,105;,
  4;61,61,61,61;,
  4;154,155,155,154;,
  4;156,156,156,156;,
  4;103,103,103,103;,
  4;157,157,154,154;,
  4;61,61,61,61;,
  4;158,158,105,105;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;108,107,107,108;,
  4;61,61,61,61;,
  4;159,160,160,159;,
  4;161,161,161,161;,
  4;106,106,106,106;,
  4;162,162,159,159;,
  4;61,61,61,61;,
  4;163,163,108,108;,
  4;65,65,65,65;,
  4;120,112,110,122;,
  4;118,114,112,120;,
  4;122,110,116,124;,
  4;110,112,114,116;,
  4;115,113,111,109;,
  4;111,113,117,119;,
  4;109,111,119,121;,
  4;115,109,121,123;,
  3;121,122,123;,
  4;119,120,122,121;,
  4;117,118,120,119;,
  3;122,124,123;;
 }
 MeshTextureCoords {
  256;
  0.892130;0.141960;,
  0.908910;0.150740;,
  0.908910;0.204390;,
  0.892130;0.213490;,
  0.898800;0.130970;,
  0.921230;0.142440;,
  0.921230;0.212590;,
  0.898550;0.224480;,
  0.835280;0.212590;,
  0.835280;0.142440;,
  0.847600;0.150740;,
  0.847600;0.204390;,
  0.857710;0.130970;,
  0.864380;0.141960;,
  0.864380;0.213490;,
  0.857710;0.224480;,
  0.801500;0.332250;,
  0.801500;0.386860;,
  0.779370;0.386860;,
  0.779370;0.307250;,
  0.673960;0.193150;,
  0.690710;0.193220;,
  0.690630;0.213840;,
  0.673880;0.213780;,
  0.782720;0.110330;,
  0.804850;0.110330;,
  0.804850;0.164940;,
  0.782720;0.189940;,
  0.273050;-1.049190;,
  0.273050;-1.040370;,
  0.263550;-1.040370;,
  0.263550;-1.049190;,
  0.900890;0.323930;,
  0.900890;0.378140;,
  0.873650;0.378140;,
  0.873650;0.323090;,
  0.663160;0.311180;,
  0.663160;0.384640;,
  0.635920;0.384640;,
  0.635920;0.312030;,
  0.719460;0.386860;,
  0.741580;0.386860;,
  0.736430;0.406380;,
  0.784520;0.406380;,
  0.891510;0.398220;,
  0.876010;0.397980;,
  0.787880;0.090090;,
  0.774910;0.090090;,
  0.660810;0.404470;,
  0.645300;0.404710;,
  0.690580;0.227390;,
  0.673820;0.227320;,
  0.855760;0.378140;,
  0.855760;0.323270;,
  0.865820;0.397820;,
  0.279300;-1.049190;,
  0.279300;-1.040370;,
  0.681060;0.384640;,
  0.670990;0.404310;,
  0.681060;0.311370;,
  0.690520;0.240220;,
  0.673770;0.240150;,
  0.838810;0.378140;,
  0.838810;0.323540;,
  0.856180;0.397670;,
  0.285210;-1.049190;,
  0.285210;-1.040370;,
  0.698010;0.384640;,
  0.680640;0.404160;,
  0.698010;0.311640;,
  0.256870;-1.040370;,
  0.256870;-1.049190;,
  0.920030;0.378140;,
  0.902410;0.398380;,
  0.920030;0.323540;,
  0.674020;0.178660;,
  0.690770;0.178730;,
  0.616780;0.384640;,
  0.616780;0.311640;,
  0.634410;0.404880;,
  0.279300;-1.073080;,
  0.285210;-1.073080;,
  0.279300;-1.064250;,
  0.285210;-1.064250;,
  0.273050;-1.073080;,
  0.273050;-1.064250;,
  0.263550;-1.073080;,
  0.263550;-1.064250;,
  0.256870;-1.073080;,
  0.256870;-1.064250;,
  0.722810;0.110330;,
  0.739780;0.090090;,
  0.752750;0.090090;,
  0.744940;0.110330;,
  0.722810;0.183340;,
  0.744940;0.189940;,
  0.628600;0.192970;,
  0.628660;0.178480;,
  0.645410;0.178550;,
  0.645360;0.193040;,
  0.628520;0.213590;,
  0.645270;0.213660;,
  0.628470;0.227140;,
  0.645220;0.227200;,
  0.628410;0.239970;,
  0.645170;0.240040;,
  0.719460;0.313850;,
  0.741580;0.307250;,
  0.722810;0.233420;,
  0.744940;0.244550;,
  0.782720;0.244550;,
  0.804850;0.233420;,
  0.900890;0.257440;,
  0.920030;0.266060;,
  0.920030;0.306820;,
  0.900890;0.307210;,
  0.873650;0.256020;,
  0.873650;0.306370;,
  0.855760;0.256340;,
  0.855760;0.306560;,
  0.838810;0.306820;,
  0.838810;0.266060;,
  0.779370;0.252640;,
  0.801500;0.263770;,
  0.741580;0.252640;,
  0.719460;0.263770;,
  0.681060;0.251830;,
  0.698010;0.261550;,
  0.663160;0.251510;,
  0.635920;0.252930;,
  0.616780;0.261550;,
  0.822910;0.181650;,
  0.822410;0.222420;,
  0.704070;0.178780;,
  0.704010;0.193270;,
  0.900890;0.246430;,
  0.873650;0.245010;,
  0.855760;0.245330;,
  0.703870;0.227440;,
  0.703820;0.240280;,
  0.819060;0.274780;,
  0.819550;0.315540;,
  0.737130;0.447410;,
  0.749720;0.447410;,
  0.771230;0.447410;,
  0.783830;0.447410;,
  0.858810;0.438700;,
  0.867260;0.439320;,
  0.876180;0.439980;,
  0.889760;0.440980;,
  0.899290;0.441680;,
  0.774580;0.046790;,
  0.787180;0.046790;,
  0.753080;0.046790;,
  0.740480;0.046790;,
  0.637520;0.448180;,
  0.647060;0.447480;,
  0.660640;0.446480;,
  0.669550;0.445820;,
  0.678000;0.445200;,
  0.673220;0.115340;,
  0.683300;0.120610;,
  0.683300;0.152850;,
  0.673220;0.158320;,
  0.679680;0.106030;,
  0.694360;0.120040;,
  0.694360;0.157210;,
  0.679640;0.169220;,
  0.633620;0.157210;,
  0.633620;0.120040;,
  0.644680;0.120610;,
  0.644680;0.152850;,
  0.648300;0.106030;,
  0.654770;0.115340;,
  0.654770;0.158320;,
  0.648170;0.169220;,
  0.694620;0.018680;,
  0.706680;0.021560;,
  0.706740;0.032750;,
  0.694700;0.032820;,
  0.679660;0.039950;,
  0.687780;0.039950;,
  0.687780;0.097700;,
  0.679660;0.097700;,
  0.695300;0.033230;,
  0.683260;0.032930;,
  0.683540;0.021740;,
  0.695650;0.019100;,
  0.655430;0.041220;,
  0.663800;0.041220;,
  0.663800;0.100480;,
  0.655430;0.100480;,
  0.694030;0.038110;,
  0.702190;0.038110;,
  0.702190;0.100300;,
  0.694030;0.100300;,
  0.694120;0.037040;,
  0.702520;0.037040;,
  0.702520;0.101050;,
  0.694120;0.101050;,
  0.647760;0.100480;,
  0.647760;0.041220;,
  0.706640;0.022320;,
  0.706360;0.033510;,
  0.672150;0.097700;,
  0.672150;0.039950;,
  0.683640;0.032890;,
  0.683580;0.021690;,
  0.694830;0.018940;,
  0.706800;0.022180;,
  0.706530;0.033370;,
  0.694480;0.033080;,
  0.680120;0.038960;,
  0.688250;0.038960;,
  0.688250;0.096720;,
  0.680120;0.096720;,
  0.695530;0.033330;,
  0.683490;0.032940;,
  0.683850;0.021760;,
  0.695980;0.019200;,
  0.655110;0.039540;,
  0.663470;0.039540;,
  0.663470;0.098800;,
  0.655110;0.098800;,
  0.694500;0.037420;,
  0.702650;0.037420;,
  0.702650;0.099610;,
  0.694500;0.099610;,
  0.694230;0.037140;,
  0.702630;0.037140;,
  0.702630;0.101150;,
  0.694230;0.101150;,
  0.647430;0.098800;,
  0.647430;0.039540;,
  0.706940;0.022500;,
  0.706580;0.033690;,
  0.672610;0.096720;,
  0.672610;0.038960;,
  0.683430;0.032810;,
  0.683700;0.021620;,
  0.865270;0.080680;,
  0.858880;0.077330;,
  0.858880;0.056900;,
  0.865270;0.053440;,
  0.856900;0.121130;,
  0.837630;0.111270;,
  0.837630;0.050980;,
  0.857120;0.040760;,
  0.911500;0.050980;,
  0.911500;0.111270;,
  0.890250;0.077330;,
  0.890250;0.056900;,
  0.892220;0.121130;,
  0.883860;0.080680;,
  0.883860;0.053440;,
  0.892220;0.040760;;
 }
}
