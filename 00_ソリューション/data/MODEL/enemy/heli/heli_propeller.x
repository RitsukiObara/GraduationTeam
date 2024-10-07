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
 296;
 2.25814;56.56234;0.02305;,
 1.46419;56.56234;1.93978;,
 1.46419;-1.57805;1.93975;,
 2.25814;-1.57805;0.02302;,
 -0.45254;56.56234;2.73370;,
 -0.45254;-1.57805;2.73370;,
 -2.36930;56.56234;1.93978;,
 -2.36930;-1.57805;1.93975;,
 -3.16322;56.56234;0.02305;,
 -3.16322;-1.57805;0.02302;,
 -2.36930;56.56234;-1.89374;,
 -2.36930;-1.57805;-1.89374;,
 -0.45254;56.56234;-2.68766;,
 -0.45254;-1.57805;-2.68769;,
 1.46419;56.56234;-1.89374;,
 1.46419;-1.57805;-1.89374;,
 2.25814;56.56234;0.02305;,
 2.25814;-1.57805;0.02302;,
 -0.45254;56.56234;0.02305;,
 -0.45254;56.56234;0.02305;,
 -0.45254;56.56234;0.02305;,
 -0.45254;56.56234;0.02305;,
 -0.45254;56.56234;0.02305;,
 -0.45254;56.56234;0.02305;,
 -0.45254;56.56234;0.02305;,
 -0.45254;56.56234;0.02305;,
 -0.45254;-1.57805;0.02302;,
 -0.45254;-1.57805;0.02302;,
 -0.45254;-1.57805;0.02302;,
 -0.45254;-1.57805;0.02302;,
 -0.45254;-1.57805;0.02302;,
 -0.45254;-1.57805;0.02302;,
 -0.45254;-1.57805;0.02302;,
 -0.45254;-1.57805;0.02302;,
 18.88975;20.84689;0.03181;,
 11.14891;20.84686;11.63326;,
 11.14891;19.49857;11.63326;,
 18.88975;19.49857;0.03181;,
 -0.45254;20.84686;16.43875;,
 -0.45254;19.49857;16.43875;,
 -12.05402;20.84686;11.63326;,
 -12.05402;19.49857;11.63326;,
 -19.79486;20.84689;0.03178;,
 -19.79486;19.49857;0.03178;,
 -12.05402;20.84689;-11.56964;,
 -12.05402;19.49857;-11.56964;,
 -0.45254;20.84689;-16.37516;,
 -0.45254;19.49857;-16.37516;,
 11.14891;20.84689;-11.56964;,
 11.14891;19.49857;-11.56964;,
 18.88975;20.84689;0.03181;,
 18.88975;19.49857;0.03181;,
 -0.45254;20.84689;0.03181;,
 -0.45254;20.84689;0.03181;,
 -0.45254;20.84689;0.03181;,
 -0.45254;20.84689;0.03181;,
 -0.45254;20.84689;0.03181;,
 -0.45254;20.84689;0.03181;,
 -0.45254;20.84689;0.03181;,
 -0.45254;20.84689;0.03181;,
 -0.45254;19.49857;0.03181;,
 -0.45254;19.49857;0.03181;,
 -0.45254;19.49857;0.03181;,
 -0.45254;19.49857;0.03181;,
 -0.45254;19.49857;0.03181;,
 -0.45254;19.49857;0.03181;,
 -0.45254;19.49857;0.03181;,
 -0.45254;19.49857;0.03181;,
 9.56098;19.69324;0.01867;,
 6.62809;19.69324;7.09930;,
 3.17674;12.34408;3.64792;,
 4.68004;12.34411;0.01864;,
 -0.45254;19.69324;10.03222;,
 -0.45254;12.34408;5.15122;,
 -7.53320;19.69324;7.09930;,
 -4.08182;12.34408;3.64792;,
 -10.46609;19.69324;0.01867;,
 -5.58512;12.34411;0.01864;,
 -7.53317;19.69324;-7.06196;,
 -4.08182;12.34411;-3.61064;,
 -0.45254;19.69324;-9.99485;,
 -0.45254;12.34411;-5.11394;,
 6.62809;19.69324;-7.06196;,
 3.17674;12.34411;-3.61064;,
 9.56098;19.69324;0.01867;,
 4.68004;12.34411;0.01864;,
 -0.45254;19.69324;0.01867;,
 -0.45254;19.69324;0.01867;,
 -0.45254;19.69324;0.01867;,
 -0.45254;19.69324;0.01867;,
 -0.45254;19.69324;0.01867;,
 -0.45254;19.69324;0.01867;,
 -0.45254;19.69324;0.01867;,
 -0.45254;19.69324;0.01867;,
 -0.45254;12.34411;0.01864;,
 -0.45254;12.34411;0.01864;,
 -0.45254;12.34411;0.01864;,
 -0.45254;12.34411;0.01864;,
 -0.45254;12.34411;0.01864;,
 -0.45254;12.34411;0.01864;,
 -0.45254;12.34411;0.01864;,
 -0.45254;12.34411;0.01864;,
 15.55030;57.33056;0.01363;,
 10.86319;57.33056;11.32933;,
 10.86319;52.14715;11.32933;,
 15.55030;52.14715;0.01363;,
 -0.45254;57.33056;16.01644;,
 -0.45254;52.14715;16.01644;,
 -11.76827;57.33056;11.32933;,
 -11.76827;52.14715;11.32933;,
 -16.45541;57.33056;0.01363;,
 -16.45541;52.14715;0.01363;,
 -11.76830;57.33056;-11.30210;,
 -11.76830;52.14715;-11.30210;,
 -0.45254;57.33056;-15.98924;,
 -0.45254;52.14715;-15.98924;,
 10.86319;57.33056;-11.30210;,
 10.86319;52.14715;-11.30210;,
 -0.45254;57.33056;0.01363;,
 10.86319;57.33056;11.32933;,
 15.55030;57.33056;0.01363;,
 -0.45254;57.33056;16.01644;,
 -11.76827;57.33056;11.32933;,
 -16.45541;57.33056;0.01363;,
 -11.76830;57.33056;-11.30210;,
 -0.45254;57.33056;-15.98924;,
 10.86319;57.33056;-11.30210;,
 -0.45254;52.14715;0.01363;,
 15.55030;52.14715;0.01363;,
 10.86319;52.14715;11.32933;,
 -0.45254;52.14715;16.01644;,
 -11.76827;52.14715;11.32933;,
 -16.45541;52.14715;0.01363;,
 -11.76830;52.14715;-11.30210;,
 -0.45254;52.14715;-15.98924;,
 10.86319;52.14715;-11.30210;,
 8.65564;42.33265;4.22905;,
 5.02384;42.33265;11.15128;,
 5.02384;41.19461;11.15128;,
 8.65564;41.19466;1.97539;,
 5.02384;42.33265;11.15128;,
 -5.92895;42.33265;11.15128;,
 -5.92895;41.19461;11.15128;,
 5.02384;41.19461;11.15128;,
 -5.92895;42.33265;11.15128;,
 -9.56075;42.33265;4.22905;,
 -9.56075;41.19466;1.97539;,
 -5.92895;41.19461;11.15128;,
 -9.56075;42.33265;4.22905;,
 8.65564;42.33265;4.22905;,
 8.65564;41.19466;1.97539;,
 -9.56075;41.19466;1.97539;,
 5.02384;42.33265;11.15128;,
 8.65564;42.33265;4.22905;,
 8.65564;41.19466;1.97539;,
 5.02384;41.19461;11.15128;,
 4.52872;41.98145;9.72727;,
 4.52875;41.98145;171.30371;,
 4.52875;41.42371;171.30371;,
 4.52872;41.42374;9.72727;,
 4.52875;41.98145;171.30371;,
 -5.43377;41.98145;161.33417;,
 -5.43377;41.42371;161.33417;,
 4.52875;41.42371;171.30371;,
 -5.43377;41.98145;161.33417;,
 -5.43380;41.98145;9.72727;,
 -5.43380;41.42374;9.72727;,
 -5.43377;41.42371;161.33417;,
 -5.43380;41.98145;9.72727;,
 4.52872;41.98145;9.72727;,
 4.52872;41.42374;9.72727;,
 -5.43380;41.42374;9.72727;,
 4.52875;41.98145;171.30371;,
 4.52872;41.98145;9.72727;,
 4.52872;41.42374;9.72727;,
 4.52875;41.42371;171.30371;,
 -9.56075;42.33265;-4.18115;,
 -5.92892;42.33265;-11.10341;,
 -5.92892;41.19466;-11.10341;,
 -9.56075;41.19466;-1.92749;,
 -5.92892;42.33265;-11.10341;,
 5.02384;42.33265;-11.10341;,
 5.02384;41.19466;-11.10341;,
 -5.92892;41.19466;-11.10341;,
 5.02384;42.33265;-11.10341;,
 8.65567;42.33265;-4.18115;,
 8.65567;41.19466;-1.92749;,
 5.02384;41.19466;-11.10341;,
 8.65567;42.33265;-4.18115;,
 -9.56075;42.33265;-4.18115;,
 -9.56075;41.19466;-1.92749;,
 8.65567;41.19466;-1.92749;,
 -5.92892;42.33265;-11.10341;,
 -9.56075;42.33265;-4.18115;,
 -9.56075;41.19466;-1.92749;,
 -5.92892;41.19466;-11.10341;,
 -5.43380;41.98145;-9.67940;,
 -5.43386;41.98150;-171.25608;,
 -5.43386;41.42380;-171.25608;,
 -5.43380;41.42374;-9.67940;,
 -5.43386;41.98150;-171.25608;,
 4.52866;41.98150;-161.28646;,
 4.52866;41.42380;-161.28646;,
 -5.43386;41.42380;-171.25608;,
 4.52866;41.98150;-161.28646;,
 4.52872;41.98145;-9.67940;,
 4.52872;41.42374;-9.67940;,
 4.52866;41.42380;-161.28646;,
 4.52872;41.98145;-9.67940;,
 -5.43380;41.98145;-9.67940;,
 -5.43380;41.42374;-9.67940;,
 4.52872;41.42374;-9.67940;,
 -5.43386;41.98150;-171.25608;,
 -5.43380;41.98145;-9.67940;,
 -5.43380;41.42374;-9.67940;,
 -5.43386;41.42380;-171.25608;,
 -4.51397;42.33265;9.03481;,
 -11.43623;42.33265;5.40301;,
 -11.43623;41.19461;5.40301;,
 -2.26034;41.19461;9.03481;,
 -11.43623;42.33265;5.40301;,
 -11.43623;42.33265;-5.54978;,
 -11.43623;41.19466;-5.54978;,
 -11.43623;41.19461;5.40301;,
 -11.43623;42.33265;-5.54978;,
 -4.51397;42.33265;-9.18161;,
 -2.26034;41.19466;-9.18161;,
 -11.43623;41.19466;-5.54978;,
 -4.51397;42.33265;-9.18161;,
 -4.51397;42.33265;9.03481;,
 -2.26034;41.19461;9.03481;,
 -2.26034;41.19466;-9.18161;,
 -11.43623;42.33265;5.40301;,
 -4.51397;42.33265;9.03481;,
 -2.26034;41.19461;9.03481;,
 -11.43623;41.19461;5.40301;,
 -10.01222;41.98145;4.90786;,
 -171.58877;41.98145;4.90798;,
 -171.58877;41.42374;4.90798;,
 -10.01222;41.42374;4.90786;,
 -171.58877;41.98145;4.90798;,
 -161.61923;41.98145;-5.05454;,
 -161.61923;41.42374;-5.05454;,
 -171.58877;41.42374;4.90798;,
 -161.61923;41.98145;-5.05454;,
 -10.01222;41.98145;-5.05463;,
 -10.01222;41.42374;-5.05463;,
 -161.61923;41.42374;-5.05454;,
 -10.01222;41.98145;-5.05463;,
 -10.01222;41.98145;4.90786;,
 -10.01222;41.42374;4.90786;,
 -10.01222;41.42374;-5.05463;,
 -171.58877;41.98145;4.90798;,
 -10.01222;41.98145;4.90786;,
 -10.01222;41.42374;4.90786;,
 -171.58877;41.42374;4.90798;,
 3.69724;42.33265;-9.18161;,
 10.61950;42.33265;-5.54978;,
 10.61950;41.19466;-5.54978;,
 1.44361;41.19466;-9.18161;,
 10.61950;42.33265;-5.54978;,
 10.61950;42.33265;5.40298;,
 10.61950;41.19461;5.40298;,
 10.61950;41.19466;-5.54978;,
 10.61950;42.33265;5.40298;,
 3.69724;42.33265;9.03481;,
 1.44361;41.19461;9.03481;,
 10.61950;41.19461;5.40298;,
 3.69724;42.33265;9.03481;,
 3.69724;42.33265;-9.18161;,
 1.44361;41.19466;-9.18161;,
 1.44361;41.19461;9.03481;,
 10.61950;42.33265;-5.54978;,
 3.69724;42.33265;-9.18161;,
 1.44361;41.19466;-9.18161;,
 10.61950;41.19466;-5.54978;,
 9.19549;41.98145;-5.05466;,
 170.77195;41.98145;-5.05469;,
 170.77195;41.42374;-5.05469;,
 9.19549;41.42374;-5.05466;,
 170.77195;41.98145;-5.05469;,
 160.80241;41.98145;4.90780;,
 160.80241;41.42374;4.90780;,
 170.77195;41.42374;-5.05469;,
 160.80241;41.98145;4.90780;,
 9.19549;41.98145;4.90786;,
 9.19549;41.42374;4.90786;,
 160.80241;41.42374;4.90780;,
 9.19549;41.98145;4.90786;,
 9.19549;41.98145;-5.05466;,
 9.19549;41.42374;-5.05466;,
 9.19549;41.42374;4.90786;,
 170.77195;41.98145;-5.05469;,
 9.19549;41.98145;-5.05466;,
 9.19549;41.42374;-5.05466;,
 170.77195;41.42374;-5.05469;;
 
 144;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 3;52,35,34;,
 3;53,38,35;,
 3;54,40,38;,
 3;55,42,40;,
 3;56,44,42;,
 3;57,46,44;,
 3;58,48,46;,
 3;59,50,48;,
 3;60,37,36;,
 3;61,36,39;,
 3;62,39,41;,
 3;63,41,43;,
 3;64,43,45;,
 3;65,45,47;,
 3;66,47,49;,
 3;67,49,51;,
 4;68,69,70,71;,
 4;69,72,73,70;,
 4;72,74,75,73;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;82,84,85,83;,
 3;86,69,68;,
 3;87,72,69;,
 3;88,74,72;,
 3;89,76,74;,
 3;90,78,76;,
 3;91,80,78;,
 3;92,82,80;,
 3;93,84,82;,
 3;94,71,70;,
 3;95,70,73;,
 3;96,73,75;,
 3;97,75,77;,
 3;98,77,79;,
 3;99,79,81;,
 3;100,81,83;,
 3;101,83,85;,
 4;102,103,104,105;,
 4;103,106,107,104;,
 4;106,108,109,107;,
 4;108,110,111,109;,
 4;110,112,113,111;,
 4;112,114,115,113;,
 4;114,116,117,115;,
 4;116,102,105,117;,
 3;118,119,120;,
 3;118,121,119;,
 3;118,122,121;,
 3;118,123,122;,
 3;118,124,123;,
 3;118,125,124;,
 3;118,126,125;,
 3;118,120,126;,
 3;127,128,129;,
 3;127,129,130;,
 3;127,130,131;,
 3;127,131,132;,
 3;127,132,133;,
 3;127,133,134;,
 3;127,134,135;,
 3;127,135,128;,
 4;136,137,138,139;,
 4;140,141,142,143;,
 4;144,145,146,147;,
 4;148,149,150,151;,
 4;148,141,152,153;,
 4;154,155,142,151;,
 4;156,157,158,159;,
 4;160,161,162,163;,
 4;164,165,166,167;,
 4;168,169,170,171;,
 4;168,161,172,173;,
 4;174,175,162,171;,
 4;176,177,178,179;,
 4;180,181,182,183;,
 4;184,185,186,187;,
 4;188,189,190,191;,
 4;188,181,192,193;,
 4;194,195,182,191;,
 4;196,197,198,199;,
 4;200,201,202,203;,
 4;204,205,206,207;,
 4;208,209,210,211;,
 4;208,201,212,213;,
 4;214,215,202,211;,
 4;216,217,218,219;,
 4;220,221,222,223;,
 4;224,225,226,227;,
 4;228,229,230,231;,
 4;228,221,232,233;,
 4;234,235,222,231;,
 4;236,237,238,239;,
 4;240,241,242,243;,
 4;244,245,246,247;,
 4;248,249,250,251;,
 4;248,241,252,253;,
 4;254,255,242,251;,
 4;256,257,258,259;,
 4;260,261,262,263;,
 4;264,265,266,267;,
 4;268,269,270,271;,
 4;268,261,272,273;,
 4;274,275,262,271;,
 4;276,277,278,279;,
 4;280,281,282,283;,
 4;284,285,286,287;,
 4;288,289,290,291;,
 4;288,281,292,293;,
 4;294,295,282,291;;
 
 MeshMaterialList {
  5;
  144;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Documents\\Visual Studio 2015\\Projects\\3D004\\data\\TEXTURE\\Camouflage.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Glass.png";
   }
  }
  Material {
   0.385600;0.385600;0.385600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Documents\\Visual Studio 2015\\Projects\\3D004\\data\\TEXTURE\\Camouflage.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Documents\\Visual Studio 2015\\Projects\\3D004\\data\\TEXTURE\\Camouflage.jpg";
   }
  }
 }
 MeshNormals {
  93;
  0.000000;1.000000;0.000000;,
  1.000000;-0.000000;0.000004;,
  0.707104;-0.000000;0.707109;,
  0.000003;-0.000000;1.000000;,
  -0.707107;-0.000000;0.707106;,
  -1.000000;-0.000000;0.000004;,
  -0.707110;0.000000;-0.707103;,
  0.000003;0.000000;-1.000000;,
  0.707107;0.000000;-0.707106;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;1.000000;0.000001;,
  0.831834;0.000000;0.555025;,
  0.634647;0.000000;0.772802;,
  0.000000;0.000000;1.000000;,
  -0.634647;0.000000;0.772802;,
  -0.831834;0.000000;0.555024;,
  -0.634647;0.000000;-0.772802;,
  0.000000;0.000000;-1.000000;,
  0.634648;0.000000;-0.772801;,
  0.833016;-0.553249;-0.000001;,
  0.589032;-0.553250;0.589029;,
  0.000000;-0.553252;0.833014;,
  -0.589031;-0.553252;0.589029;,
  -0.833015;-0.553251;-0.000002;,
  -0.589031;-0.553248;-0.589032;,
  -0.000000;-0.553247;-0.833017;,
  0.589032;-0.553248;-0.589032;,
  0.459287;-0.888288;-0.000003;,
  0.324765;-0.888290;0.324760;,
  0.000000;-0.888290;0.459282;,
  -0.324764;-0.888290;0.324759;,
  -0.459286;-0.888289;-0.000004;,
  -0.324765;-0.888288;-0.324765;,
  -0.000000;-0.888288;-0.459287;,
  0.324765;-0.888288;-0.324765;,
  0.000000;-1.000000;-0.000004;,
  1.000000;0.000000;0.000000;,
  0.707106;0.000000;0.707107;,
  -0.000000;0.000000;1.000000;,
  -0.707106;0.000000;0.707108;,
  -1.000000;0.000000;0.000002;,
  -0.707107;0.000000;-0.707106;,
  0.000000;0.000000;-1.000000;,
  0.707108;0.000000;-0.707106;,
  0.861891;-0.310114;0.401213;,
  0.000000;0.000000;1.000000;,
  -0.861891;-0.310114;0.401213;,
  1.000000;0.000000;-0.000000;,
  -0.923947;0.000000;0.382521;,
  -1.000000;0.000000;0.000000;,
  -0.861889;-0.310118;-0.401216;,
  0.000000;0.000000;-1.000000;,
  0.861889;-0.310118;-0.401216;,
  -1.000000;0.000000;0.000000;,
  0.923948;0.000000;-0.382519;,
  1.000000;0.000000;-0.000000;,
  -0.401216;-0.310102;0.861895;,
  -1.000000;0.000000;0.000000;,
  -0.401217;-0.310115;-0.861889;,
  0.000001;0.000000;1.000000;,
  -0.382521;0.000000;-0.923947;,
  -0.000001;0.000000;-1.000000;,
  0.401217;-0.310115;-0.861889;,
  1.000000;0.000000;0.000000;,
  0.401218;-0.310104;0.861893;,
  -0.000000;0.000000;-1.000000;,
  0.382520;0.000000;0.923947;,
  0.000000;0.000000;1.000000;,
  -0.831833;0.000000;-0.555026;,
  0.831834;0.000000;-0.555025;,
  0.000000;1.000000;0.000002;,
  0.000000;1.000000;0.000001;,
  0.000000;1.000000;0.000002;,
  -0.000000;1.000000;0.000002;,
  -0.000000;1.000000;0.000001;,
  0.000000;0.972793;-0.231677;,
  0.000000;0.892652;-0.450747;,
  0.000000;-1.000000;-0.000006;,
  -0.707356;0.000000;0.706858;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.972793;0.231677;,
  0.000000;0.892652;0.450747;,
  0.707358;0.000000;-0.706855;,
  -0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.231684;0.972791;0.000001;,
  0.450761;0.892645;0.000001;,
  -0.000000;-1.000000;-0.000004;,
  -0.706858;0.000000;-0.707356;,
  -0.231684;0.972791;0.000001;,
  -0.450761;0.892645;0.000001;,
  0.000000;-1.000000;-0.000004;,
  0.706856;0.000000;0.707357;;
  144;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;68,16,16,68;,
  4;16,17,17,16;,
  4;17,18,18,17;,
  4;18,69,69,18;,
  3;10,70,71;,
  3;10,72,70;,
  3;10,73,72;,
  3;10,74,73;,
  3;10,0,74;,
  3;10,0,0;,
  3;10,0,0;,
  3;10,71,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,19,27,34;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;35,27,28;,
  3;35,28,29;,
  3;35,29,30;,
  3;35,30,31;,
  3;35,31,32;,
  3;35,32,33;,
  3;35,33,34;,
  3;35,34,27;,
  4;36,37,37,36;,
  4;37,38,38,37;,
  4;38,39,39,38;,
  4;39,40,40,39;,
  4;40,41,41,40;,
  4;41,42,42,41;,
  4;42,43,43,42;,
  4;43,36,36,43;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;44,44,44,44;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;75,75,76,76;,
  4;75,0,0,75;,
  4;77,77,77,77;,
  4;47,47,47,47;,
  4;78,48,48,78;,
  4;48,49,49,48;,
  4;51,51,51,51;,
  4;0,0,0,0;,
  4;79,79,79,79;,
  4;50,50,50,50;,
  4;51,51,51,51;,
  4;52,52,52,52;,
  4;80,80,81,81;,
  4;80,0,0,80;,
  4;9,9,9,9;,
  4;53,53,53,53;,
  4;82,54,54,82;,
  4;54,55,55,54;,
  4;45,45,45,45;,
  4;83,83,83,83;,
  4;84,84,84,84;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;85,85,86,86;,
  4;85,0,0,85;,
  4;87,87,87,87;,
  4;59,59,59,59;,
  4;88,60,60,88;,
  4;60,61,61,60;,
  4;63,63,63,63;,
  4;0,0,0,0;,
  4;9,9,9,9;,
  4;62,62,62,62;,
  4;63,63,63,63;,
  4;64,64,64,64;,
  4;89,89,90,90;,
  4;89,0,0,89;,
  4;91,91,91,91;,
  4;65,65,65,65;,
  4;92,66,66,92;,
  4;66,67,67,66;,
  4;57,57,57,57;,
  4;0,0,0,0;,
  4;9,9,9,9;;
 }
 MeshTextureCoords {
  296;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.506103;-2.366528;,
  -0.061705;-2.366528;,
  -0.061705;-2.107358;,
  0.506103;-2.107358;,
  -0.300989;-2.366528;,
  -0.300989;-2.107358;,
  -0.071580;-2.366528;,
  -0.071580;-2.107358;,
  0.492138;-2.366528;,
  0.492138;-2.107358;,
  1.059948;-2.366528;,
  1.059948;-2.107358;,
  1.299234;-2.366528;,
  1.299234;-2.107358;,
  1.069823;-2.366528;,
  1.069823;-2.107358;,
  0.475089;0.508066;,
  1.039965;-0.058624;,
  1.275229;0.506782;,
  0.473803;-0.292070;,
  -0.091606;-0.056808;,
  -0.325053;0.509350;,
  -0.089789;1.074757;,
  0.476375;1.308205;,
  1.041784;1.072941;,
  0.475295;0.507278;,
  1.275436;0.505994;,
  1.040172;-0.059411;,
  0.474009;-0.292858;,
  -0.091400;-0.057595;,
  -0.324847;0.508562;,
  -0.089583;1.073969;,
  0.476581;1.307417;,
  1.041990;1.072153;,
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
  1.000000;0.000000;,
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
  1.000000;0.000000;,
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
  1.000000;0.000000;,
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
  1.000000;0.000000;,
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
  1.000000;0.000000;,
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
  1.000000;0.000000;,
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
  1.000000;0.000000;,
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
