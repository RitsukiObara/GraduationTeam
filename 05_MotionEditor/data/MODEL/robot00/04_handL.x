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
 344;
 -3.43926;-0.20788;-0.10608;,
 -3.52841;-0.13286;-0.10698;,
 -3.28482;0.15664;-0.10451;,
 -3.19567;0.08162;-0.10360;,
 -3.19567;0.08162;-0.10360;,
 -3.28482;0.15664;-0.10451;,
 -3.28965;0.15664;0.37083;,
 -3.20050;0.08162;0.37174;,
 -3.20050;0.08162;0.37174;,
 -3.28965;0.15664;0.37083;,
 -3.53324;-0.13286;0.36836;,
 -3.44409;-0.20788;0.36926;,
 -3.44409;-0.20788;0.36926;,
 -3.53324;-0.13286;0.36836;,
 -3.52841;-0.13286;-0.10698;,
 -3.43926;-0.20788;-0.10608;,
 -3.28965;0.15664;0.37083;,
 -3.28482;0.15664;-0.10451;,
 -3.44409;-0.20788;0.36926;,
 -3.43926;-0.20788;-0.10608;,
 -3.20080;-0.39530;-0.10365;,
 -3.28666;-0.47406;-0.10453;,
 -3.54240;-0.19524;-0.10712;,
 -3.45653;-0.11647;-0.10625;,
 -3.45653;-0.11647;-0.10625;,
 -3.54240;-0.19524;-0.10712;,
 -3.54723;-0.19524;0.36822;,
 -3.46136;-0.11647;0.36909;,
 -3.46136;-0.11647;0.36909;,
 -3.54723;-0.19524;0.36822;,
 -3.29149;-0.47406;0.37081;,
 -3.20563;-0.39530;0.37168;,
 -3.20563;-0.39530;0.37168;,
 -3.29149;-0.47406;0.37081;,
 -3.28666;-0.47406;-0.10453;,
 -3.20080;-0.39530;-0.10365;,
 -3.28666;-0.47406;-0.10453;,
 -3.29149;-0.47406;0.37081;,
 -3.45653;-0.11647;-0.10625;,
 -3.46136;-0.11647;0.36909;,
 -2.81038;-0.11248;-0.09969;,
 -2.73810;-0.20387;-0.09895;,
 -3.19603;-0.53712;-0.10361;,
 -3.26831;-0.44573;-0.10434;,
 -3.26831;-0.44573;-0.10434;,
 -3.19603;-0.53712;-0.10361;,
 -3.20086;-0.53712;0.37173;,
 -3.27314;-0.44573;0.37100;,
 -3.27314;-0.44573;0.37100;,
 -3.20086;-0.53712;0.37173;,
 -2.74293;-0.20387;0.37638;,
 -2.81521;-0.11248;0.37565;,
 -2.81521;-0.11248;0.37565;,
 -2.74293;-0.20387;0.37638;,
 -2.73810;-0.20387;-0.09895;,
 -2.81038;-0.11248;-0.09969;,
 -3.20086;-0.53712;0.37173;,
 -3.19603;-0.53712;-0.10361;,
 -2.81521;-0.11248;0.37565;,
 -2.81038;-0.11248;-0.09969;,
 -2.77472;-0.50801;-0.39157;,
 -2.88249;-0.42982;-0.39354;,
 -2.52314;0.06527;-0.40186;,
 -2.41536;-0.01292;-0.39989;,
 -2.41536;-0.01292;-0.39989;,
 -2.52314;0.06527;-0.40186;,
 -2.52411;0.07010;-0.15636;,
 -2.41634;-0.00809;-0.15439;,
 -2.41634;-0.00809;-0.15439;,
 -2.52411;0.07010;-0.15636;,
 -2.88347;-0.42499;-0.14804;,
 -2.77570;-0.50318;-0.14607;,
 -2.77570;-0.50318;-0.14607;,
 -2.88347;-0.42499;-0.14804;,
 -2.88249;-0.42982;-0.39354;,
 -2.77472;-0.50801;-0.39157;,
 -2.52411;0.07010;-0.15636;,
 -2.52314;0.06527;-0.40186;,
 -2.77570;-0.50318;-0.14607;,
 -2.77472;-0.50801;-0.39157;,
 -3.33553;-0.26566;-0.39726;,
 -3.27451;-0.14730;-0.39752;,
 -2.73074;-0.42765;-0.40396;,
 -2.79176;-0.54601;-0.40371;,
 -2.79176;-0.54601;-0.40371;,
 -2.73074;-0.42765;-0.40396;,
 -2.72823;-0.42842;-0.15843;,
 -2.78925;-0.54678;-0.15818;,
 -2.78925;-0.54678;-0.15818;,
 -2.72823;-0.42842;-0.15843;,
 -3.27200;-0.14807;-0.15199;,
 -3.33302;-0.26643;-0.15173;,
 -3.33302;-0.26643;-0.15173;,
 -3.27200;-0.14807;-0.15199;,
 -3.27451;-0.14730;-0.39752;,
 -3.33553;-0.26566;-0.39726;,
 -3.27451;-0.14730;-0.39752;,
 -3.27200;-0.14807;-0.15199;,
 -2.72823;-0.42842;-0.15843;,
 -2.73074;-0.42765;-0.40396;,
 -3.33302;-0.26643;-0.15173;,
 -3.33553;-0.26566;-0.39726;,
 -2.79176;-0.54601;-0.40371;,
 -2.78925;-0.54678;-0.15818;,
 -2.35376;0.40092;0.40018;,
 -2.35638;0.03116;0.54850;,
 -2.06248;0.02908;0.54850;,
 -2.05986;0.39884;0.40018;,
 -2.35900;-0.33860;0.40018;,
 -2.06510;-0.34069;0.40018;,
 -2.35900;-0.33860;0.40018;,
 -2.36009;-0.49176;0.04212;,
 -2.06619;-0.49385;0.04212;,
 -2.06510;-0.34069;0.40018;,
 -2.35900;-0.33860;-0.31594;,
 -2.06510;-0.34069;-0.31594;,
 -2.35900;-0.33860;-0.31594;,
 -2.35638;0.03116;-0.46425;,
 -2.06248;0.02908;-0.46425;,
 -2.06510;-0.34069;-0.31594;,
 -2.35376;0.40092;-0.31594;,
 -2.05986;0.39884;-0.31594;,
 -2.35376;0.40092;-0.31594;,
 -2.35267;0.55409;0.04212;,
 -2.05877;0.55200;0.04212;,
 -2.05986;0.39884;-0.31594;,
 -2.35376;0.40092;0.40018;,
 -2.05986;0.39884;0.40018;,
 -2.35638;0.03116;0.54850;,
 -2.35376;0.40092;0.40018;,
 -2.35638;0.03116;0.04212;,
 -2.35900;-0.33860;0.40018;,
 -2.36009;-0.49176;0.04212;,
 -2.35900;-0.33860;-0.31594;,
 -2.35638;0.03116;-0.46425;,
 -2.35376;0.40092;-0.31594;,
 -2.35267;0.55409;0.04212;,
 -2.05986;0.39884;0.40018;,
 -2.06248;0.02908;0.54850;,
 -2.06248;0.02908;0.04212;,
 -2.06510;-0.34069;0.40018;,
 -2.06619;-0.49385;0.04212;,
 -2.06510;-0.34069;-0.31594;,
 -2.06248;0.02908;-0.46425;,
 -2.05986;0.39884;-0.31594;,
 -2.05877;0.55200;0.04212;,
 -3.32265;-0.03237;-0.43001;,
 -3.32065;0.24981;-0.42999;,
 -2.35657;0.24297;-0.42019;,
 -2.35857;-0.03921;-0.42021;,
 -2.35857;-0.03921;-0.42021;,
 -2.35657;0.24297;-0.42019;,
 -2.36523;0.24297;0.43201;,
 -2.36723;-0.03921;0.43199;,
 -2.36723;-0.03921;0.43199;,
 -2.36523;0.24297;0.43201;,
 -3.32931;0.24981;0.42222;,
 -3.33131;-0.03237;0.42220;,
 -3.33131;-0.03237;0.42220;,
 -3.32931;0.24981;0.42222;,
 -3.32065;0.24981;-0.42999;,
 -3.32265;-0.03237;-0.43001;,
 -3.32065;0.24981;-0.42999;,
 -3.32931;0.24981;0.42222;,
 -2.36523;0.24297;0.43201;,
 -2.35657;0.24297;-0.42019;,
 -3.33131;-0.03237;0.42220;,
 -3.32265;-0.03237;-0.43001;,
 -2.35857;-0.03921;-0.42021;,
 -2.36723;-0.03921;0.43199;,
 0.19966;0.41526;0.19658;,
 -0.00089;0.41928;0.27966;,
 -0.01651;-0.36061;0.27966;,
 0.18403;-0.36463;0.19658;,
 -0.20143;0.42330;0.19658;,
 -0.21705;-0.35659;0.19658;,
 -0.20143;0.42330;0.19658;,
 -0.28450;0.42496;-0.00401;,
 -0.30012;-0.35493;-0.00401;,
 -0.21705;-0.35659;0.19658;,
 -0.20143;0.42330;-0.20459;,
 -0.21705;-0.35659;-0.20459;,
 -0.20143;0.42330;-0.20459;,
 -0.00089;0.41928;-0.28767;,
 -0.01651;-0.36061;-0.28767;,
 -0.21705;-0.35659;-0.20459;,
 0.19966;0.41526;-0.20459;,
 0.18403;-0.36463;-0.20459;,
 0.19966;0.41526;-0.20459;,
 0.28273;0.41360;-0.00401;,
 0.26710;-0.36629;-0.00401;,
 0.18403;-0.36463;-0.20459;,
 0.19966;0.41526;0.19658;,
 0.18403;-0.36463;0.19658;,
 -0.00089;0.41928;0.27966;,
 0.19966;0.41526;0.19658;,
 -0.00089;0.41928;-0.00401;,
 -0.20143;0.42330;0.19658;,
 -0.28450;0.42496;-0.00401;,
 -0.20143;0.42330;-0.20459;,
 -0.00089;0.41928;-0.28767;,
 0.19966;0.41526;-0.20459;,
 0.28273;0.41360;-0.00401;,
 0.18403;-0.36463;0.19658;,
 -0.01651;-0.36061;0.27966;,
 -0.01651;-0.36061;-0.00401;,
 -0.21705;-0.35659;0.19658;,
 -0.30012;-0.35493;-0.00401;,
 -0.21705;-0.35659;-0.20459;,
 -0.01651;-0.36061;-0.28767;,
 0.18403;-0.36463;-0.20459;,
 0.26710;-0.36629;-0.00401;,
 -0.76218;0.54011;0.26113;,
 -0.76746;0.27681;0.52706;,
 -0.18897;0.26521;0.53147;,
 -0.18365;0.53069;0.26333;,
 -0.76746;0.27681;0.52706;,
 -0.77299;0.00083;0.52634;,
 -0.19450;-0.01076;0.53074;,
 -0.18897;0.26521;0.53147;,
 -0.77852;-0.27515;0.52706;,
 -0.20003;-0.28674;0.53147;,
 -0.77852;-0.27515;0.52706;,
 -0.78380;-0.53845;0.26113;,
 -0.20535;-0.55222;0.26333;,
 -0.20003;-0.28674;0.53147;,
 -0.78383;-0.54031;-0.00408;,
 -0.20539;-0.55410;-0.00408;,
 -0.78380;-0.53863;-0.26928;,
 -0.20536;-0.55240;-0.27149;,
 -0.77852;-0.27515;-0.53414;,
 -0.20003;-0.28674;-0.53855;,
 -0.77852;-0.27515;-0.53414;,
 -0.77299;0.00083;-0.53449;,
 -0.19450;-0.01076;-0.53890;,
 -0.20003;-0.28674;-0.53855;,
 -0.76746;0.27681;-0.53414;,
 -0.18897;0.26521;-0.53855;,
 -0.76746;0.27681;-0.53414;,
 -0.76218;0.54029;-0.26928;,
 -0.18365;0.53088;-0.27149;,
 -0.18897;0.26521;-0.53855;,
 -0.76215;0.54196;-0.00408;,
 -0.18362;0.53257;-0.00408;,
 -2.18544;0.29255;0.24439;,
 -2.18297;0.41563;0.12009;,
 -2.19071;0.02923;-0.00388;,
 -2.19071;0.02923;0.24406;,
 -2.19599;-0.23408;0.24439;,
 -2.19845;-0.35716;0.12009;,
 -2.19847;-0.35803;-0.00388;,
 -2.19846;-0.35725;-0.12786;,
 -2.19599;-0.23408;-0.25166;,
 -2.19071;0.02923;-0.25183;,
 -2.18544;0.29255;-0.25166;,
 -2.18297;0.41571;-0.12786;,
 -2.18295;0.41650;-0.00388;,
 -0.18365;0.53069;0.26333;,
 -0.18897;0.26521;0.53147;,
 -0.19450;-0.01076;-0.00409;,
 -0.19450;-0.01076;0.53074;,
 -0.20003;-0.28674;0.53147;,
 -0.20535;-0.55222;0.26333;,
 -0.20539;-0.55410;-0.00408;,
 -0.20536;-0.55240;-0.27149;,
 -0.20003;-0.28674;-0.53855;,
 -0.19450;-0.01076;-0.53890;,
 -0.18897;0.26521;-0.53855;,
 -0.18365;0.53088;-0.27149;,
 -0.18362;0.53257;-0.00408;,
 -1.10516;0.44308;0.16803;,
 -1.10858;0.27234;0.34047;,
 -1.10858;0.27234;0.34047;,
 -1.11388;0.00766;0.34000;,
 -1.11919;-0.25702;0.34047;,
 -1.11919;-0.25702;0.34047;,
 -1.12261;-0.42776;0.16803;,
 -1.12263;-0.42896;-0.00395;,
 -1.12261;-0.42788;-0.17592;,
 -1.11919;-0.25702;-0.34767;,
 -1.11919;-0.25702;-0.34767;,
 -1.11388;0.00766;-0.34790;,
 -1.10858;0.27234;-0.34767;,
 -1.10858;0.27234;-0.34767;,
 -1.10515;0.44319;-0.17592;,
 -1.10513;0.44428;-0.00395;,
 -2.18297;0.41563;0.12009;,
 -2.18544;0.29255;0.24439;,
 -2.18544;0.29255;0.24439;,
 -2.19071;0.02923;0.24406;,
 -2.19599;-0.23408;0.24439;,
 -2.19599;-0.23408;0.24439;,
 -2.19845;-0.35716;0.12009;,
 -2.19847;-0.35803;-0.00388;,
 -2.19846;-0.35725;-0.12786;,
 -2.19599;-0.23408;-0.25166;,
 -2.19599;-0.23408;-0.25166;,
 -2.19071;0.02923;-0.25183;,
 -2.18544;0.29255;-0.25166;,
 -2.18544;0.29255;-0.25166;,
 -2.18297;0.41571;-0.12786;,
 -2.18295;0.41650;-0.00388;,
 0.28212;0.10244;0.28896;,
 -0.01079;0.10831;0.41031;,
 -0.01480;-0.09164;0.41031;,
 0.27811;-0.09751;0.28896;,
 -0.30369;0.11418;0.28896;,
 -0.30770;-0.08577;0.28896;,
 -0.30369;0.11418;0.28896;,
 -0.42502;0.11661;-0.00401;,
 -0.42903;-0.08334;-0.00401;,
 -0.30770;-0.08577;0.28896;,
 -0.30369;0.11418;-0.29697;,
 -0.30770;-0.08577;-0.29697;,
 -0.30369;0.11418;-0.29697;,
 -0.01079;0.10831;-0.41832;,
 -0.01480;-0.09164;-0.41832;,
 -0.30770;-0.08577;-0.29697;,
 0.28212;0.10244;-0.29697;,
 0.27811;-0.09751;-0.29697;,
 0.28212;0.10244;-0.29697;,
 0.40344;0.10001;-0.00401;,
 0.39944;-0.09994;-0.00401;,
 0.27811;-0.09751;-0.29697;,
 0.28212;0.10244;0.28896;,
 0.27811;-0.09751;0.28896;,
 -0.01079;0.10831;0.41031;,
 0.28212;0.10244;0.28896;,
 -0.01079;0.10831;-0.00401;,
 -0.30369;0.11418;0.28896;,
 -0.42502;0.11661;-0.00401;,
 -0.30369;0.11418;-0.29697;,
 -0.01079;0.10831;-0.41832;,
 0.28212;0.10244;-0.29697;,
 0.40344;0.10001;-0.00401;,
 0.27811;-0.09751;0.28896;,
 -0.01480;-0.09164;0.41031;,
 -0.01480;-0.09164;-0.00401;,
 -0.30770;-0.08577;0.28896;,
 -0.42903;-0.08334;-0.00401;,
 -0.30770;-0.08577;-0.29697;,
 -0.01480;-0.09164;-0.41832;,
 0.27811;-0.09751;-0.29697;,
 0.39944;-0.09994;-0.00401;;
 
 168;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;14,13,16,17;,
 4;18,19,4,7;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,26,25;,
 4;32,35,38,39;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;54,53,56,57;,
 4;58,59,44,47;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;74,73,76,77;,
 4;78,79,64,67;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;96,97,98,99;,
 4;100,101,102,103;,
 4;104,105,106,107;,
 4;105,108,109,106;,
 4;110,111,112,113;,
 4;111,114,115,112;,
 4;116,117,118,119;,
 4;117,120,121,118;,
 4;122,123,124,125;,
 4;123,126,127,124;,
 3;128,129,130;,
 3;131,128,130;,
 3;132,131,130;,
 3;133,132,130;,
 3;134,133,130;,
 3;135,134,130;,
 3;136,135,130;,
 3;129,136,130;,
 3;137,138,139;,
 3;138,140,139;,
 3;140,141,139;,
 3;141,142,139;,
 3;142,143,139;,
 3;143,144,139;,
 3;144,145,139;,
 3;145,137,139;,
 4;146,147,148,149;,
 4;150,151,152,153;,
 4;154,155,156,157;,
 4;158,159,160,161;,
 4;162,163,164,165;,
 4;166,167,168,169;,
 4;170,171,172,173;,
 4;171,174,175,172;,
 4;176,177,178,179;,
 4;177,180,181,178;,
 4;182,183,184,185;,
 4;183,186,187,184;,
 4;188,189,190,191;,
 4;189,192,193,190;,
 3;194,195,196;,
 3;197,194,196;,
 3;198,197,196;,
 3;199,198,196;,
 3;200,199,196;,
 3;201,200,196;,
 3;202,201,196;,
 3;195,202,196;,
 3;203,204,205;,
 3;204,206,205;,
 3;206,207,205;,
 3;207,208,205;,
 3;208,209,205;,
 3;209,210,205;,
 3;210,211,205;,
 3;211,203,205;,
 4;212,213,214,215;,
 4;216,217,218,219;,
 4;217,220,221,218;,
 4;222,223,224,225;,
 4;223,226,227,224;,
 4;226,228,229,227;,
 4;228,230,231,229;,
 4;232,233,234,235;,
 4;233,236,237,234;,
 4;238,239,240,241;,
 4;239,242,243,240;,
 4;242,212,215,243;,
 3;244,245,246;,
 3;247,244,246;,
 3;248,247,246;,
 3;249,248,246;,
 3;250,249,246;,
 3;251,250,246;,
 3;252,251,246;,
 3;253,252,246;,
 3;254,253,246;,
 3;255,254,246;,
 3;256,255,246;,
 3;245,256,246;,
 3;257,258,259;,
 3;258,260,259;,
 3;260,261,259;,
 3;261,262,259;,
 3;262,263,259;,
 3;263,264,259;,
 3;264,265,259;,
 3;265,266,259;,
 3;266,267,259;,
 3;267,268,259;,
 3;268,269,259;,
 3;269,257,259;,
 4;270,271,213,212;,
 4;272,273,217,216;,
 4;273,274,220,217;,
 4;275,276,223,222;,
 4;276,277,226,223;,
 4;277,278,228,226;,
 4;278,279,230,228;,
 4;280,281,233,232;,
 4;281,282,236,233;,
 4;283,284,239,238;,
 4;284,285,242,239;,
 4;285,270,212,242;,
 4;286,287,271,270;,
 4;288,289,273,272;,
 4;289,290,274,273;,
 4;291,292,276,275;,
 4;292,293,277,276;,
 4;293,294,278,277;,
 4;294,295,279,278;,
 4;296,297,281,280;,
 4;297,298,282,281;,
 4;299,300,284,283;,
 4;300,301,285,284;,
 4;301,286,270,285;,
 4;302,303,304,305;,
 4;303,306,307,304;,
 4;308,309,310,311;,
 4;309,312,313,310;,
 4;314,315,316,317;,
 4;315,318,319,316;,
 4;320,321,322,323;,
 4;321,324,325,322;,
 3;326,327,328;,
 3;329,326,328;,
 3;330,329,328;,
 3;331,330,328;,
 3;332,331,328;,
 3;333,332,328;,
 3;334,333,328;,
 3;327,334,328;,
 3;335,336,337;,
 3;336,338,337;,
 3;338,339,337;,
 3;339,340,337;,
 3;340,341,337;,
 3;341,342,337;,
 3;342,343,337;,
 3;343,335,337;;
 
 MeshMaterialList {
  1;
  168;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "acTex001.png";
   }
  }
 }
 MeshNormals {
  182;
  0.010158;-0.000000;-0.999948;,
  0.643822;0.765148;0.006539;,
  -0.010158;0.000000;0.999948;,
  0.010158;0.000002;-0.999948;,
  -0.675917;0.736946;-0.006865;,
  -0.010155;-0.000001;0.999948;,
  0.010158;-0.000002;-0.999948;,
  -0.784278;-0.620358;-0.007966;,
  -0.010155;-0.000001;0.999949;,
  0.003981;-0.019692;-0.999798;,
  0.587362;0.809210;-0.013594;,
  -0.003979;0.019690;0.999798;,
  -0.010242;0.003131;-0.999943;,
  0.888773;-0.458226;-0.010538;,
  0.010240;-0.003133;0.999943;,
  0.004957;0.698996;0.715109;,
  -0.000001;0.000000;1.000000;,
  -0.004962;-0.698996;0.715109;,
  -0.007099;-0.999975;-0.000000;,
  -0.004962;-0.698996;-0.715108;,
  -0.000001;0.000000;-1.000000;,
  0.004957;0.698996;-0.715108;,
  0.007091;0.999975;-0.000000;,
  -0.999975;0.007093;-0.000000;,
  0.999975;-0.007093;0.000000;,
  0.010156;0.000002;-0.999948;,
  0.999923;-0.007095;0.010155;,
  -0.010156;-0.000000;0.999948;,
  0.706965;-0.014165;0.707106;,
  -0.000002;-0.000000;1.000000;,
  -0.706964;0.014164;0.707108;,
  -0.999799;0.020031;0.000000;,
  -0.706964;0.014164;-0.707108;,
  -0.000002;-0.000000;-1.000000;,
  0.706965;-0.014165;-0.707106;,
  0.999799;-0.020033;-0.000000;,
  0.020033;0.999799;0.000000;,
  -0.020034;-0.999799;0.000000;,
  -0.999799;0.020034;-0.000003;,
  -0.999799;0.020033;0.000001;,
  -0.999799;0.020034;0.000000;,
  -0.999799;0.020033;-0.000001;,
  -0.999799;0.020034;0.000003;,
  -0.999799;0.020036;0.000006;,
  -0.999799;0.020037;-0.000000;,
  -0.999799;0.020036;-0.000002;,
  -0.999799;0.020030;-0.000005;,
  -0.999799;0.020031;0.000006;,
  -0.999799;0.020036;0.000007;,
  0.013562;0.923373;0.383665;,
  0.000658;0.383781;0.923424;,
  -0.007615;0.000153;0.999971;,
  -0.014716;-0.383499;0.923424;,
  -0.023438;-0.923175;0.383664;,
  -0.023828;-0.999716;0.000339;,
  -0.023440;-0.922853;-0.384438;,
  -0.014727;-0.384207;-0.923129;,
  -0.007617;0.000153;-0.999971;,
  0.000675;0.384489;-0.923129;,
  0.013547;0.923051;-0.384439;,
  0.016239;0.999868;0.000339;,
  -0.999799;0.020034;0.000001;,
  0.999799;-0.020034;-0.000001;,
  -0.167756;0.912097;0.374081;,
  -0.233355;0.382020;0.894207;,
  -0.251243;0.005034;0.967911;,
  -0.248473;-0.372365;0.894207;,
  -0.204159;-0.904645;0.374081;,
  -0.169851;-0.985470;0.000336;,
  -0.204402;-0.904296;-0.374792;,
  -0.248389;-0.372877;-0.894017;,
  -0.251336;0.005036;-0.967887;,
  -0.233254;0.382529;-0.894016;,
  -0.168014;0.911757;-0.374793;,
  -0.130238;0.991483;0.000335;,
  -0.194230;0.907706;0.371946;,
  -0.269569;0.379422;0.885083;,
  -0.290356;0.005818;0.956901;,
  -0.284555;-0.368319;0.885082;,
  -0.230435;-0.899198;0.371944;,
  -0.190510;-0.981685;0.000335;,
  -0.230717;-0.898836;-0.372645;,
  -0.284503;-0.368442;-0.885047;,
  -0.290462;0.005820;-0.956869;,
  -0.269516;0.379544;-0.885047;,
  -0.194526;0.907355;-0.372647;,
  -0.151033;0.988529;0.000333;,
  0.706963;-0.014164;0.707108;,
  -0.000000;0.000000;1.000000;,
  -0.706964;0.014167;0.707108;,
  -0.999799;0.020036;0.000000;,
  -0.706964;0.014167;-0.707108;,
  -0.000000;0.000000;-1.000000;,
  0.706963;-0.014164;-0.707108;,
  0.999799;-0.020028;-0.000000;,
  0.020034;0.999799;0.000000;,
  -0.020033;-0.999799;0.000000;,
  -0.643817;-0.765151;-0.006539;,
  -0.765107;0.643856;-0.007771;,
  0.765113;-0.643849;0.007771;,
  0.675913;-0.736950;0.006865;,
  -0.736906;-0.675954;-0.007485;,
  0.736907;0.675953;0.007485;,
  0.784279;0.620357;0.007966;,
  0.588365;-0.808573;0.005976;,
  -0.588365;0.808573;-0.005976;,
  -0.587355;-0.809215;0.013599;,
  -0.809316;0.587187;-0.014782;,
  0.809317;-0.587185;0.014788;,
  -0.888770;0.458231;0.010538;,
  0.458232;0.888830;-0.001907;,
  -0.458235;-0.888829;0.001910;,
  -0.999975;0.007094;0.000000;,
  -0.999975;0.007097;0.000001;,
  -0.999975;0.007090;0.000001;,
  -0.999975;0.007090;-0.000000;,
  -0.999975;0.007090;-0.000001;,
  -0.999975;0.007094;0.000000;,
  -0.999975;0.007097;-0.000001;,
  -0.999975;0.007096;-0.000000;,
  0.999975;-0.007097;-0.000001;,
  0.999975;-0.007093;-0.000000;,
  0.999975;-0.007090;-0.000001;,
  0.999975;-0.007090;0.000000;,
  0.999975;-0.007090;0.000001;,
  0.999975;-0.007093;-0.000000;,
  0.999975;-0.007097;0.000001;,
  0.999975;-0.007096;0.000000;,
  -0.999923;0.007095;-0.010155;,
  0.007095;0.999975;0.000072;,
  -0.007095;-0.999975;-0.000072;,
  0.020033;0.999799;0.000000;,
  0.020035;0.999799;0.000003;,
  0.020030;0.999799;0.000001;,
  0.020032;0.999799;0.000000;,
  0.020030;0.999799;-0.000001;,
  0.020035;0.999799;-0.000003;,
  0.020032;0.999799;0.000000;,
  -0.020037;-0.999799;-0.000000;,
  -0.020034;-0.999799;0.000000;,
  -0.020031;-0.999799;-0.000001;,
  -0.020032;-0.999799;0.000000;,
  -0.020031;-0.999799;0.000001;,
  -0.020037;-0.999799;0.000000;,
  -0.020037;-0.999799;0.000000;,
  0.999799;-0.020032;-0.000002;,
  0.999799;-0.020033;-0.000000;,
  0.999799;-0.020033;-0.000000;,
  0.999799;-0.020033;0.000000;,
  0.999799;-0.020034;-0.000004;,
  0.999799;-0.020037;-0.000005;,
  0.999799;-0.020037;0.000001;,
  0.999799;-0.020036;0.000001;,
  0.999799;-0.020032;0.000001;,
  0.999799;-0.020031;0.000001;,
  0.999799;-0.020031;-0.000001;,
  -0.040528;0.922934;0.382820;,
  -0.074817;0.384867;0.919935;,
  -0.088770;0.001779;0.996051;,
  -0.090173;-0.381561;0.919935;,
  -0.077466;-0.920570;0.382818;,
  -0.065790;-0.997833;0.000339;,
  -0.077553;-0.920243;-0.383586;,
  -0.090168;-0.381513;-0.919955;,
  -0.088801;0.001779;-0.996048;,
  -0.074812;0.384819;-0.919955;,
  -0.040627;0.922610;-0.383589;,
  -0.025765;0.999668;0.000338;,
  0.020034;0.999799;0.000000;,
  0.020035;0.999799;0.000000;,
  0.020032;0.999799;-0.000001;,
  0.020031;0.999799;0.000000;,
  0.020032;0.999799;0.000001;,
  0.020035;0.999799;-0.000000;,
  0.020035;0.999799;0.000000;,
  -0.020035;-0.999799;-0.000000;,
  -0.020034;-0.999799;0.000000;,
  -0.020032;-0.999799;0.000001;,
  -0.020031;-0.999799;0.000000;,
  -0.020032;-0.999799;-0.000001;,
  -0.020035;-0.999799;0.000000;,
  -0.020035;-0.999799;0.000000;;
  168;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;97,97,97,97;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;100,100,100,100;,
  4;101,101,101,101;,
  4;102,102,102,102;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;103,103,103,103;,
  4;104,104,104,104;,
  4;105,105,105,105;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;106,106,106,106;,
  4;107,107,107,107;,
  4;108,108,108,108;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;109,109,109,109;,
  4;110,110,110,110;,
  4;111,111,111,111;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,18,18,17;,
  4;18,19,19,18;,
  4;19,20,20,19;,
  4;20,21,21,20;,
  4;21,22,22,21;,
  4;22,15,15,22;,
  3;112,113,23;,
  3;114,112,23;,
  3;115,114,23;,
  3;116,115,23;,
  3;117,116,23;,
  3;118,117,23;,
  3;119,118,23;,
  3;113,119,23;,
  3;120,121,24;,
  3;121,122,24;,
  3;122,123,24;,
  3;123,124,24;,
  3;124,125,24;,
  3;125,126,24;,
  3;126,127,24;,
  3;127,120,24;,
  4;25,25,25,25;,
  4;26,26,26,26;,
  4;27,27,27,27;,
  4;128,128,128,128;,
  4;129,129,129,129;,
  4;130,130,130,130;,
  4;28,29,29,28;,
  4;29,30,30,29;,
  4;30,31,31,30;,
  4;31,32,32,31;,
  4;32,33,33,32;,
  4;33,34,34,33;,
  4;34,35,35,34;,
  4;35,28,28,35;,
  3;131,132,36;,
  3;133,131,36;,
  3;134,133,36;,
  3;135,134,36;,
  3;131,135,36;,
  3;136,131,36;,
  3;137,136,36;,
  3;132,137,36;,
  3;138,139,37;,
  3;139,140,37;,
  3;140,141,37;,
  3;141,142,37;,
  3;142,139,37;,
  3;139,143,37;,
  3;143,144,37;,
  3;144,138,37;,
  4;63,64,50,49;,
  4;64,65,51,50;,
  4;65,66,52,51;,
  4;66,67,53,52;,
  4;67,68,54,53;,
  4;68,69,55,54;,
  4;69,70,56,55;,
  4;70,71,57,56;,
  4;71,72,58,57;,
  4;72,73,59,58;,
  4;73,74,60,59;,
  4;74,63,49,60;,
  3;39,38,61;,
  3;40,39,61;,
  3;41,40,61;,
  3;42,41,61;,
  3;43,42,61;,
  3;44,43,61;,
  3;45,44,61;,
  3;40,45,61;,
  3;46,40,61;,
  3;47,46,61;,
  3;48,47,61;,
  3;38,48,61;,
  3;145,146,62;,
  3;146,147,62;,
  3;147,148,62;,
  3;148,149,62;,
  3;149,150,62;,
  3;150,151,62;,
  3;151,152,62;,
  3;152,147,62;,
  3;147,153,62;,
  3;153,154,62;,
  3;154,155,62;,
  3;155,145,62;,
  4;75,76,64,63;,
  4;76,77,65,64;,
  4;77,78,66,65;,
  4;78,79,67,66;,
  4;79,80,68,67;,
  4;80,81,69,68;,
  4;81,82,70,69;,
  4;82,83,71,70;,
  4;83,84,72,71;,
  4;84,85,73,72;,
  4;85,86,74,73;,
  4;86,75,63,74;,
  4;156,157,76,75;,
  4;157,158,77,76;,
  4;158,159,78,77;,
  4;159,160,79,78;,
  4;160,161,80,79;,
  4;161,162,81,80;,
  4;162,163,82,81;,
  4;163,164,83,82;,
  4;164,165,84,83;,
  4;165,166,85,84;,
  4;166,167,86,85;,
  4;167,156,75,86;,
  4;87,88,88,87;,
  4;88,89,89,88;,
  4;89,90,90,89;,
  4;90,91,91,90;,
  4;91,92,92,91;,
  4;92,93,93,92;,
  4;93,94,94,93;,
  4;94,87,87,94;,
  3;168,169,95;,
  3;170,168,95;,
  3;171,170,95;,
  3;172,171,95;,
  3;168,172,95;,
  3;173,168,95;,
  3;174,173,95;,
  3;169,174,95;,
  3;175,176,96;,
  3;176,177,96;,
  3;177,178,96;,
  3;178,179,96;,
  3;179,176,96;,
  3;176,180,96;,
  3;180,181,96;,
  3;181,175,96;;
 }
 MeshTextureCoords {
  344;
  0.935220;0.939620;,
  0.944450;0.931860;,
  0.919230;0.901880;,
  0.910000;0.909650;,
  0.947680;0.950370;,
  0.955450;0.950470;,
  0.955450;0.901250;,
  0.947680;0.901150;,
  0.909880;0.933060;,
  0.919110;0.940820;,
  0.944330;0.910850;,
  0.935100;0.903080;,
  0.914290;0.946510;,
  0.922050;0.946420;,
  0.922050;0.897200;,
  0.914290;0.897290;,
  0.952030;0.946680;,
  0.952030;0.897460;,
  0.917700;0.901410;,
  0.917700;0.950630;,
  0.921780;0.933850;,
  0.930670;0.942010;,
  0.957150;0.913140;,
  0.948260;0.904980;,
  0.955300;0.900660;,
  0.947140;0.900570;,
  0.947140;0.949780;,
  0.955300;0.949880;,
  0.945170;0.948540;,
  0.954060;0.940380;,
  0.927580;0.911510;,
  0.918690;0.919670;,
  0.930320;0.895280;,
  0.922160;0.895370;,
  0.922160;0.944590;,
  0.930320;0.944500;,
  0.918270;0.900830;,
  0.918270;0.950050;,
  0.959190;0.944770;,
  0.959190;0.895550;,
  0.947920;0.901630;,
  0.938450;0.894140;,
  0.903950;0.941560;,
  0.913410;0.949040;,
  0.920050;0.896640;,
  0.910580;0.896720;,
  0.910580;0.945940;,
  0.920050;0.945860;,
  0.929770;0.952600;,
  0.939230;0.945120;,
  0.904730;0.897700;,
  0.895260;0.905180;,
  0.955270;0.900930;,
  0.945800;0.900850;,
  0.945800;0.950070;,
  0.955270;0.950150;,
  0.911300;0.901340;,
  0.911300;0.950560;,
  0.954550;0.946340;,
  0.954550;0.897120;,
  0.945380;0.957320;,
  0.956540;0.949220;,
  0.919330;0.897950;,
  0.908170;0.906050;,
  0.946770;0.897170;,
  0.946970;0.889070;,
  0.921550;0.888570;,
  0.921340;0.896670;,
  0.911830;0.945000;,
  0.922990;0.953100;,
  0.960200;0.901840;,
  0.949040;0.893740;,
  0.957440;0.947200;,
  0.957230;0.939100;,
  0.931810;0.939600;,
  0.932020;0.947700;,
  0.956370;0.887840;,
  0.930950;0.888340;,
  0.920480;0.947930;,
  0.945900;0.948430;,
  0.942130;0.955020;,
  0.954380;0.948700;,
  0.925350;0.892400;,
  0.913100;0.898710;,
  0.933220;0.924970;,
  0.945480;0.925000;,
  0.945400;0.899570;,
  0.933140;0.899550;,
  0.949030;0.898780;,
  0.936780;0.892460;,
  0.907750;0.948770;,
  0.920000;0.955090;,
  0.917550;0.925000;,
  0.929800;0.924970;,
  0.929880;0.899550;,
  0.917630;0.899570;,
  0.908060;0.951280;,
  0.933480;0.951020;,
  0.932820;0.894720;,
  0.907390;0.894980;,
  0.904150;0.951020;,
  0.929580;0.951280;,
  0.930250;0.894980;,
  0.904820;0.894720;,
  0.817030;0.504700;,
  0.817220;0.476940;,
  0.795160;0.476790;,
  0.794970;0.504540;,
  0.817420;0.449190;,
  0.795360;0.449030;,
  0.816400;0.502950;,
  0.816480;0.476070;,
  0.794420;0.476070;,
  0.794340;0.502950;,
  0.816400;0.449190;,
  0.794340;0.449190;,
  0.817280;0.504500;,
  0.817090;0.476750;,
  0.795030;0.476900;,
  0.795220;0.504660;,
  0.816890;0.448990;,
  0.794830;0.449150;,
  0.817640;0.503980;,
  0.817560;0.477110;,
  0.795500;0.477110;,
  0.795580;0.503980;,
  0.817640;0.450230;,
  0.795580;0.450230;,
  0.780740;0.477080;,
  0.772520;0.456600;,
  0.752690;0.477080;,
  0.772520;0.497570;,
  0.752690;0.506060;,
  0.732850;0.497570;,
  0.724630;0.477080;,
  0.732850;0.456600;,
  0.752690;0.448110;,
  0.733620;0.456890;,
  0.725400;0.477380;,
  0.753460;0.477380;,
  0.733620;0.497870;,
  0.753460;0.506350;,
  0.773300;0.497870;,
  0.781510;0.477380;,
  0.773300;0.456890;,
  0.753460;0.448410;,
  0.718950;0.949480;,
  0.733900;0.949370;,
  0.733540;0.898300;,
  0.718590;0.898410;,
  0.734470;0.883840;,
  0.734410;0.898790;,
  0.779550;0.898970;,
  0.779610;0.884020;,
  0.795400;0.898940;,
  0.780450;0.898840;,
  0.780090;0.949910;,
  0.795040;0.950010;,
  0.779610;0.964690;,
  0.779530;0.949740;,
  0.734390;0.949990;,
  0.734470;0.964940;,
  0.733790;0.948760;,
  0.778930;0.949220;,
  0.779450;0.898150;,
  0.734310;0.897690;,
  0.673090;0.949760;,
  0.718240;0.949300;,
  0.717720;0.898230;,
  0.672570;0.898680;,
  0.946440;0.616880;,
  0.946260;0.603360;,
  0.870210;0.603870;,
  0.870400;0.617390;,
  0.946060;0.589840;,
  0.870020;0.590350;,
  0.869560;0.579560;,
  0.869980;0.593080;,
  0.945970;0.591510;,
  0.945550;0.577990;,
  0.870720;0.606590;,
  0.946720;0.605020;,
  0.870210;0.585470;,
  0.871180;0.598980;,
  0.947130;0.596360;,
  0.946160;0.582850;,
  0.872150;0.612480;,
  0.948100;0.609870;,
  0.870820;0.579170;,
  0.871570;0.592680;,
  0.947570;0.591120;,
  0.946820;0.577600;,
  0.871990;0.606200;,
  0.947990;0.604640;,
  0.752210;0.449290;,
  0.731440;0.457900;,
  0.752210;0.478670;,
  0.772970;0.457900;,
  0.781580;0.478670;,
  0.772970;0.499440;,
  0.752210;0.508040;,
  0.731440;0.499440;,
  0.722840;0.478670;,
  0.731020;0.497500;,
  0.751780;0.506100;,
  0.751780;0.476730;,
  0.772550;0.497500;,
  0.781150;0.476730;,
  0.772550;0.455960;,
  0.751780;0.447360;,
  0.731020;0.455960;,
  0.722410;0.476730;,
  0.794240;0.715050;,
  0.821770;0.715600;,
  0.822230;0.655700;,
  0.794460;0.655150;,
  0.823580;0.716270;,
  0.852160;0.716840;,
  0.853360;0.656940;,
  0.824780;0.656370;,
  0.880740;0.717410;,
  0.881940;0.657510;,
  0.882020;0.717000;,
  0.909560;0.717540;,
  0.909330;0.657650;,
  0.881570;0.657100;,
  0.937020;0.717550;,
  0.937020;0.657650;,
  0.964480;0.717540;,
  0.964710;0.657650;,
  0.991910;0.717000;,
  0.992360;0.657100;,
  0.651910;0.717410;,
  0.680480;0.716840;,
  0.679280;0.656940;,
  0.650710;0.657510;,
  0.709060;0.716270;,
  0.707860;0.656370;,
  0.711890;0.715600;,
  0.739310;0.715050;,
  0.739090;0.655150;,
  0.711430;0.655700;,
  0.766780;0.715050;,
  0.766770;0.655140;,
  0.873410;0.895340;,
  0.860540;0.882590;,
  0.847710;0.922600;,
  0.873380;0.922600;,
  0.873410;0.949870;,
  0.860540;0.962610;,
  0.847710;0.962700;,
  0.834870;0.962620;,
  0.822050;0.949870;,
  0.822030;0.922600;,
  0.822050;0.895340;,
  0.834870;0.882590;,
  0.847710;0.882500;,
  0.741700;0.529170;,
  0.713930;0.556650;,
  0.769390;0.585230;,
  0.714010;0.585230;,
  0.713930;0.613810;,
  0.741700;0.641300;,
  0.769390;0.641490;,
  0.797080;0.641320;,
  0.824730;0.613810;,
  0.824770;0.585230;,
  0.824730;0.556650;,
  0.797080;0.529150;,
  0.769390;0.528970;,
  0.784600;0.750570;,
  0.802450;0.750920;,
  0.824050;0.751590;,
  0.851450;0.752140;,
  0.878860;0.752690;,
  0.901340;0.752270;,
  0.919200;0.752630;,
  0.937010;0.752630;,
  0.954810;0.752630;,
  0.972600;0.752270;,
  0.653780;0.752690;,
  0.681190;0.752140;,
  0.708600;0.751590;,
  0.731200;0.750920;,
  0.748980;0.750570;,
  0.766790;0.750560;,
  0.779630;0.862170;,
  0.792500;0.862420;,
  0.821950;0.863100;,
  0.849220;0.863640;,
  0.876480;0.864190;,
  0.911290;0.863770;,
  0.924160;0.864030;,
  0.937000;0.864030;,
  0.949840;0.864030;,
  0.962660;0.863770;,
  0.656160;0.864190;,
  0.683430;0.863640;,
  0.710690;0.863100;,
  0.741140;0.862420;,
  0.753960;0.862170;,
  0.766790;0.862170;,
  0.796130;0.444900;,
  0.795520;0.475230;,
  0.816230;0.475650;,
  0.816830;0.445320;,
  0.794920;0.505560;,
  0.815620;0.505980;,
  0.815860;0.505770;,
  0.816120;0.475440;,
  0.795410;0.475440;,
  0.795160;0.505770;,
  0.815860;0.445100;,
  0.795160;0.445100;,
  0.817260;0.505360;,
  0.816650;0.475030;,
  0.795950;0.475440;,
  0.796550;0.505770;,
  0.816040;0.444700;,
  0.795340;0.445110;,
  0.815940;0.505770;,
  0.815680;0.475440;,
  0.794980;0.475440;,
  0.795230;0.505770;,
  0.815940;0.445100;,
  0.795230;0.445100;,
  0.753020;0.447920;,
  0.732260;0.456520;,
  0.753020;0.477290;,
  0.773780;0.456520;,
  0.782380;0.477290;,
  0.773780;0.498050;,
  0.753020;0.506650;,
  0.732260;0.498050;,
  0.723660;0.477290;,
  0.731910;0.497950;,
  0.752670;0.506550;,
  0.752670;0.477180;,
  0.773430;0.497950;,
  0.782030;0.477180;,
  0.773430;0.456420;,
  0.752670;0.447820;,
  0.731910;0.456420;,
  0.723310;0.477180;;
 }
}