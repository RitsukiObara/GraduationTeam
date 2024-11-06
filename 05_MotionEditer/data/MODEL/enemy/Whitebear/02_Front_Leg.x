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
 251;
 -16.65232;-34.66879;-9.80543;,
 -15.88910;-27.08120;-10.79836;,
 -12.28831;-26.99777;-11.84519;,
 -13.80854;-34.60291;-10.74215;,
 -7.87066;-26.89542;-12.82126;,
 -9.99068;-34.51444;-11.61551;,
 -3.45301;-26.79307;-12.99283;,
 -6.17281;-34.42597;-11.76905;,
 -15.06715;-19.99510;-11.56849;,
 -11.06264;-19.90231;-12.70073;,
 -6.28452;-19.79161;-13.75643;,
 -1.50642;-19.68089;-13.94199;,
 -12.88028;-6.82931;-11.77508;,
 -8.43797;-6.72637;-13.04638;,
 -3.15097;-6.60388;-14.23175;,
 2.13606;-6.48137;-14.44012;,
 -0.19980;8.62339;-7.71660;,
 -0.19980;8.62339;0.00000;,
 1.33664;8.65901;-7.77393;,
 5.35770;8.75219;0.00000;,
 3.33306;8.70527;-7.47268;,
 5.35770;8.75219;-6.59783;,
 5.35770;8.75219;-2.19928;,
 -12.88028;-6.82931;11.14983;,
 -15.06715;-19.99510;8.84869;,
 -11.06264;-19.90231;9.98093;,
 -8.43797;-6.72637;12.42115;,
 -6.28452;-19.79161;11.03663;,
 -3.15097;-6.60388;13.60649;,
 -1.50642;-19.68089;11.22221;,
 2.13606;-6.48137;13.81487;,
 -15.88910;-27.08120;8.07856;,
 -12.28831;-26.99777;9.12539;,
 -7.87066;-26.89542;10.10146;,
 -3.45301;-26.79307;10.27303;,
 -16.65232;-34.66879;7.08563;,
 -14.04245;-34.60833;8.02235;,
 -10.22459;-34.51986;8.89571;,
 -6.40672;-34.43139;9.04925;,
 -16.59836;-43.34067;6.71972;,
 -18.47264;-44.27764;1.70802;,
 -14.11312;-43.42313;1.04353;,
 -14.11312;-43.42313;7.27274;,
 -10.88282;-43.34827;1.26724;,
 -10.59458;-43.41904;8.01218;,
 -7.65252;-43.27344;1.30658;,
 -7.65252;-43.27344;7.49052;,
 -18.47264;-44.27764;-4.42782;,
 -14.11312;-43.42313;-3.76333;,
 -10.88282;-43.34827;-3.98704;,
 -7.65252;-43.27344;-4.02638;,
 -16.59510;-43.48064;-9.43952;,
 -13.62922;-43.41193;-10.07037;,
 -10.41372;-43.33741;-10.64774;,
 -7.20864;-43.26316;-10.06605;,
 -0.07740;-34.28474;1.58490;,
 2.85804;-26.64684;1.93111;,
 2.85804;-26.64684;8.51312;,
 -0.31131;-34.29016;7.47448;,
 -0.07740;-34.28474;-4.30468;,
 2.85804;-26.64684;-4.65091;,
 -0.07740;-34.28474;-10.19428;,
 2.85804;-26.64684;-11.23292;,
 4.74604;-19.53603;2.19962;,
 4.74604;-19.53603;9.31869;,
 4.74604;-19.53603;-4.91942;,
 4.74604;-19.53603;-12.03849;,
 9.05445;-6.32108;3.68410;,
 9.05445;-6.32108;11.67757;,
 9.05445;-6.32108;-4.30936;,
 9.05445;-6.32108;-12.30280;,
 -16.70598;-27.10015;1.78625;,
 -17.62641;-34.69136;1.75579;,
 -16.70598;-27.10015;-4.50605;,
 -17.62641;-34.69136;-4.47559;,
 -15.84076;-20.01303;2.04296;,
 -15.84076;-20.01303;-4.76276;,
 -13.72496;-6.84889;3.50820;,
 -13.72496;-6.84889;-4.13345;,
 -5.09193;-43.21411;1.20326;,
 -4.23234;-43.19420;6.71972;,
 -3.38038;-34.36128;8.64587;,
 -0.31959;-26.72046;9.82224;,
 -5.09193;-43.21411;-3.92306;,
 -4.23234;-43.19420;-9.43952;,
 -0.31959;-26.72046;-12.54204;,
 -3.14649;-34.35586;-11.36567;,
 1.59791;-19.60898;10.73462;,
 1.59791;-19.60898;-13.45444;,
 5.57102;-6.40179;13.26740;,
 5.57102;-6.40179;-13.89265;,
 -2.92876;-44.59963;0.90317;,
 -3.13420;-44.49355;4.68259;,
 -2.93195;-44.46261;-3.62297;,
 -3.36184;-44.07453;-6.97860;,
 -16.71754;-40.74732;6.54806;,
 -14.07346;-40.68605;7.42795;,
 -17.85092;-40.77358;1.68629;,
 -10.60704;-40.60573;8.26587;,
 -7.19199;-40.52660;7.81641;,
 -17.85092;-40.77358;-4.40608;,
 -13.60238;-40.67514;-9.94518;,
 -16.93224;-40.75229;-9.32746;,
 -10.23574;-40.59713;-10.56661;,
 -6.87883;-40.51935;-10.47843;,
 -4.02838;-40.45329;7.27907;,
 -4.21623;-40.45766;-9.50938;,
 -1.99519;-40.40620;5.83551;,
 -1.91929;-40.40444;1.03855;,
 -1.91929;-40.40444;-3.75838;,
 -2.20376;-40.41102;-8.55529;,
 -11.04667;-0.25596;-10.03327;,
 -7.00812;-0.16238;-11.12751;,
 -11.81459;-0.27374;-3.45605;,
 -2.20163;-0.05101;-12.14775;,
 2.60487;0.06035;-12.32712;,
 -11.81459;-0.27374;3.12122;,
 -7.00812;-0.16238;10.79271;,
 -11.04667;-0.25596;9.69847;,
 -2.20163;-0.05101;11.81295;,
 2.60487;0.06035;11.99232;,
 5.72765;0.13270;11.52108;,
 5.72765;0.13270;-11.85588;,
 8.89450;0.20610;3.27263;,
 8.89450;0.20610;10.15270;,
 8.89450;0.20610;-3.60743;,
 8.89450;0.20610;-10.48752;,
 -9.22786;4.71976;-8.28580;,
 -5.83609;4.79835;-9.20477;,
 -9.87282;4.70482;-2.76192;,
 -1.79939;4.89188;-10.06164;,
 2.23733;4.98541;-10.21225;,
 -9.87282;4.70482;2.76192;,
 -5.83609;4.79835;9.20479;,
 -9.22786;4.71976;8.28578;,
 -1.79939;4.89188;10.06164;,
 2.23733;4.98541;10.21225;,
 4.85995;5.04617;9.81650;,
 4.85995;5.04617;-9.81653;,
 7.51964;5.10779;2.88909;,
 7.51964;5.10779;8.66727;,
 7.51964;5.10779;-2.88911;,
 7.51964;5.10779;-8.66729;,
 -5.16546;-48.94425;5.18380;,
 -3.84417;-48.91365;6.55612;,
 -10.28108;-49.06278;8.80501;,
 -10.28108;-49.06278;6.16237;,
 -17.19383;-49.22296;6.87020;,
 -15.39666;-49.18131;5.18380;,
 -19.35594;-49.27307;3.10916;,
 -17.18656;-49.22280;2.19343;,
 -19.73603;-49.28186;-1.35990;,
 -19.56854;-49.27799;-4.93517;,
 -17.51562;-49.23041;-1.35990;,
 -19.35594;-49.27307;-5.82896;,
 -17.18656;-49.22280;-4.91323;,
 -15.39666;-49.18131;-7.90360;,
 -17.19383;-49.22296;-9.58999;,
 -10.28108;-49.06278;-11.52482;,
 -10.28108;-49.06278;-8.88217;,
 -3.84417;-48.91365;-9.27594;,
 -5.16546;-48.94425;-7.90360;,
 -1.65586;-48.86293;-1.35990;,
 -3.04652;-48.89516;-1.35990;,
 -3.10554;-47.60888;7.68888;,
 -10.31087;-47.77582;10.11485;,
 -18.13752;-47.95718;8.36071;,
 -20.83736;-48.01973;3.91853;,
 -21.15752;-48.02715;-5.16965;,
 -20.83736;-48.01973;-6.63833;,
 -18.13752;-47.95718;-11.08051;,
 -10.31087;-47.77582;-12.83465;,
 -3.10554;-47.60888;-10.40868;,
 -0.73694;-47.55400;-1.35990;,
 -2.70216;-46.78009;8.14826;,
 -10.32986;-46.95682;10.69740;,
 -18.57886;-47.14796;8.85420;,
 -21.36616;-47.21254;4.18651;,
 -21.87263;-47.22428;-5.24419;,
 -21.36616;-47.21254;-6.90811;,
 -18.57886;-47.14796;-11.57398;,
 -10.32986;-46.95682;-13.41720;,
 -2.70216;-46.78009;-10.86806;,
 -0.15860;-46.72116;-1.35990;,
 -3.40733;-45.08030;7.42450;,
 -10.36960;-45.24161;9.77960;,
 -17.95318;-45.41733;8.07667;,
 -20.60264;-45.47871;3.76252;,
 -21.06779;-45.48949;-4.94865;,
 -20.60264;-45.47871;-6.48592;,
 -21.36616;-47.21254;-6.90811;,
 -21.87263;-47.22428;-5.24419;,
 -17.95318;-45.41733;-10.79649;,
 -10.36960;-45.24161;-12.49940;,
 -3.40733;-45.08030;-10.14431;,
 -1.13942;-45.02775;-1.35990;,
 -4.23765;-42.96422;6.71972;,
 -10.41907;-43.10743;7.92797;,
 -16.60045;-43.25067;6.71972;,
 -17.96618;-43.24498;3.02564;,
 -17.93770;-43.23310;-4.43284;,
 -17.96618;-43.24498;-5.74904;,
 -16.60045;-43.25067;-9.43952;,
 -10.41907;-43.10743;-10.64774;,
 -4.23765;-42.96422;-9.43952;,
 -2.61365;-43.30485;-1.35990;,
 -7.10676;-41.60392;3.01275;,
 -10.45211;-41.68145;3.66665;,
 -13.79743;-41.75897;3.01275;,
 -14.49027;-41.77501;0.82642;,
 -17.96618;-43.24498;-5.74904;,
 -17.93770;-43.23310;-4.43284;,
 -14.49027;-41.77501;-3.54623;,
 -17.87114;-43.20542;-1.35990;,
 -15.18311;-41.79107;-1.35990;,
 -13.79743;-41.75897;-5.73257;,
 -10.45211;-41.68145;-6.38644;,
 -7.10676;-41.60392;-5.73257;,
 -5.72107;-41.57181;-1.35990;,
 -10.28108;-49.06278;-1.35990;,
 -10.46372;-41.18070;-1.35990;,
 -20.60264;-45.47871;-6.48592;,
 -19.56854;-49.27799;2.21535;,
 -21.15752;-48.02715;2.46028;,
 -21.75516;-48.04099;-1.35990;,
 -21.87263;-47.22428;2.52259;,
 -22.37146;-47.23585;-1.35990;,
 -21.06779;-45.48949;2.22525;,
 -21.47013;-45.49883;-1.35990;,
 -17.93770;-43.23310;1.70946;,
 -21.06779;-45.48949;-4.94865;,
 -17.96618;-43.24498;3.02564;,
 -17.93770;-43.23310;1.70946;,
 -21.06779;-45.48949;2.22525;,
 -20.60264;-45.47871;3.76252;,
 -21.87263;-47.22428;2.52259;,
 -21.36616;-47.21254;4.18651;,
 3.33306;8.70527;7.47265;,
 1.33664;8.65901;7.77391;,
 -1.73624;8.58780;7.65925;,
 -0.19980;8.62339;7.71658;,
 -4.80913;8.51661;7.00700;,
 -7.39105;8.45678;6.30743;,
 -7.88200;8.44540;2.10247;,
 -7.88200;8.44540;-2.10247;,
 -7.88200;8.44540;0.00000;,
 -7.39105;8.45678;-6.30745;,
 -4.80913;8.51661;-7.00700;,
 -1.73624;8.58780;-7.65927;,
 5.35770;8.75219;2.19928;,
 5.35770;8.75219;6.59783;;
 
 263;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;1,8,9,2;,
 4;2,9,10,4;,
 4;4,10,11,6;,
 4;8,12,13,9;,
 4;9,13,14,10;,
 4;10,14,15,11;,
 3;16,17,18;,
 3;17,19,18;,
 3;18,19,20;,
 3;20,19,21;,
 3;21,19,22;,
 4;23,24,25,26;,
 4;26,25,27,28;,
 4;28,27,29,30;,
 4;24,31,32,25;,
 4;25,32,33,27;,
 4;27,33,34,29;,
 4;31,35,36,32;,
 4;32,36,37,33;,
 4;33,37,38,34;,
 4;39,40,41,42;,
 4;42,41,43,44;,
 4;44,43,45,46;,
 4;40,47,48,41;,
 4;41,48,49,43;,
 4;43,49,50,45;,
 4;47,51,52,48;,
 4;48,52,53,49;,
 4;49,53,54,50;,
 4;55,56,57,58;,
 4;59,60,56,55;,
 4;61,62,60,59;,
 4;56,63,64,57;,
 4;60,65,63,56;,
 4;62,66,65,60;,
 4;63,67,68,64;,
 4;65,69,67,63;,
 4;66,70,69,65;,
 4;35,31,71,72;,
 4;72,71,73,74;,
 4;74,73,1,0;,
 4;31,24,75,71;,
 4;71,75,76,73;,
 4;73,76,8,1;,
 4;24,23,77,75;,
 4;75,77,78,76;,
 4;76,78,12,8;,
 4;45,79,80,46;,
 4;38,81,82,34;,
 4;50,83,79,45;,
 4;54,84,83,50;,
 4;6,85,86,7;,
 4;34,82,87,29;,
 4;11,88,85,6;,
 4;29,87,89,30;,
 4;15,90,88,11;,
 4;79,91,92,80;,
 4;81,58,57,82;,
 4;83,93,91,79;,
 4;84,94,93,83;,
 4;85,62,61,86;,
 4;82,57,64,87;,
 4;88,66,62,85;,
 4;87,64,68,89;,
 4;90,70,66,88;,
 4;35,95,96,36;,
 4;72,97,95,35;,
 4;36,96,98,37;,
 4;37,98,99,38;,
 4;74,100,97,72;,
 4;3,101,102,0;,
 4;0,102,100,74;,
 4;5,103,101,3;,
 4;7,104,103,5;,
 4;38,99,105,81;,
 4;86,106,104,7;,
 4;81,105,107,58;,
 4;58,107,108,55;,
 4;55,108,109,59;,
 4;59,109,110,61;,
 4;61,110,106,86;,
 4;95,39,42,96;,
 4;97,40,39,95;,
 4;96,42,44,98;,
 4;98,44,46,99;,
 4;100,47,40,97;,
 4;101,52,51,102;,
 4;102,51,47,100;,
 4;103,53,52,101;,
 4;104,54,53,103;,
 4;99,46,80,105;,
 4;106,84,54,104;,
 4;105,80,92,107;,
 4;107,92,91,108;,
 4;108,91,93,109;,
 4;109,93,94,110;,
 4;110,94,84,106;,
 4;12,111,112,13;,
 4;78,113,111,12;,
 4;13,112,114,14;,
 4;14,114,115,15;,
 4;77,116,113,78;,
 4;26,117,118,23;,
 4;23,118,116,77;,
 4;28,119,117,26;,
 4;30,120,119,28;,
 4;89,121,120,30;,
 4;15,115,122,90;,
 4;67,123,124,68;,
 4;68,124,121,89;,
 4;69,125,123,67;,
 4;90,122,126,70;,
 4;70,126,125,69;,
 4;111,127,128,112;,
 4;113,129,127,111;,
 4;112,128,130,114;,
 4;114,130,131,115;,
 4;116,132,129,113;,
 4;117,133,134,118;,
 4;118,134,132,116;,
 4;119,135,133,117;,
 4;120,136,135,119;,
 4;121,137,136,120;,
 4;115,131,138,122;,
 4;123,139,140,124;,
 4;124,140,137,121;,
 4;125,141,139,123;,
 4;122,138,142,126;,
 4;126,142,141,125;,
 4;143,144,145,146;,
 4;146,145,147,148;,
 4;148,147,149,150;,
 3;151,152,153;,
 3;152,154,153;,
 3;153,154,155;,
 4;156,157,158,159;,
 4;159,158,160,161;,
 4;161,160,162,163;,
 4;163,162,144,143;,
 4;144,164,165,145;,
 4;145,165,166,147;,
 4;147,166,167,149;,
 4;168,169,154,152;,
 4;157,170,171,158;,
 4;158,171,172,160;,
 4;160,172,173,162;,
 4;162,173,164,144;,
 4;164,174,175,165;,
 4;165,175,176,166;,
 4;166,176,177,167;,
 4;178,179,169,168;,
 4;170,180,181,171;,
 4;171,181,182,172;,
 4;172,182,183,173;,
 4;173,183,174,164;,
 4;174,184,185,175;,
 4;175,185,186,176;,
 4;176,186,187,177;,
 4;188,189,190,191;,
 4;180,192,193,181;,
 4;181,193,194,182;,
 4;182,194,195,183;,
 4;183,195,184,174;,
 4;184,196,197,185;,
 4;185,197,198,186;,
 4;186,198,199,187;,
 4;200,201,189,188;,
 4;192,202,203,193;,
 4;193,203,204,194;,
 4;194,204,205,195;,
 4;195,205,196,184;,
 4;196,206,207,197;,
 4;197,207,208,198;,
 4;198,208,209,199;,
 3;210,211,212;,
 3;211,213,212;,
 3;212,213,214;,
 4;202,215,216,203;,
 4;203,216,217,204;,
 4;204,217,218,205;,
 4;205,218,206,196;,
 3;146,219,143;,
 3;148,219,146;,
 4;153,219,148,150;,
 4;156,219,153,155;,
 3;159,219,156;,
 3;161,219,159;,
 3;163,219,161;,
 3;143,219,163;,
 3;206,220,207;,
 3;207,220,208;,
 4;208,220,214,209;,
 4;214,220,215,212;,
 3;215,220,216;,
 3;216,220,217;,
 3;217,220,218;,
 3;218,220,206;,
 4;154,157,156,155;,
 4;169,170,157,154;,
 4;179,180,170,169;,
 4;221,192,180,179;,
 4;210,202,192,221;,
 4;212,215,202,210;,
 3;151,153,222;,
 3;222,153,149;,
 3;149,153,150;,
 4;223,224,151,222;,
 4;225,226,224,223;,
 4;227,228,226,225;,
 4;229,213,228,227;,
 3;213,229,214;,
 3;229,199,214;,
 3;214,199,209;,
 4;228,213,211,230;,
 4;226,228,230,178;,
 4;224,226,178,168;,
 4;151,224,168,152;,
 4;231,232,233,234;,
 4;234,233,235,236;,
 4;177,225,223,167;,
 4;167,223,222,149;,
 4;137,237,238,136;,
 3;135,136,239;,
 3;239,136,240;,
 3;240,136,238;,
 4;135,239,241,133;,
 4;133,241,242,134;,
 4;134,242,243,132;,
 3;129,132,244;,
 3;244,132,245;,
 3;245,132,243;,
 4;129,244,246,127;,
 4;127,246,247,128;,
 4;128,247,248,130;,
 3;130,248,131;,
 3;248,16,131;,
 3;131,16,18;,
 4;131,18,20,138;,
 4;138,20,21,142;,
 4;142,21,22,141;,
 3;249,139,19;,
 3;139,141,19;,
 3;19,141,22;,
 4;139,249,250,140;,
 4;140,250,237,137;,
 3;17,240,19;,
 3;240,238,19;,
 3;238,237,19;,
 3;237,249,19;,
 3;249,237,250;,
 3;245,243,17;,
 3;243,242,17;,
 3;242,241,17;,
 3;241,239,17;,
 3;17,239,240;,
 3;244,245,246;,
 3;245,17,246;,
 3;246,17,247;,
 3;247,17,248;,
 3;248,17,16;;
 
 MeshMaterialList {
  2;
  263;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\enemy\\whitebear_UV.jpg";
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
  292;
  -0.268855;-0.094974;-0.958487;,
  -0.237884;-0.107954;-0.965276;,
  -0.113377;-0.144471;-0.982992;,
  0.084972;-0.203744;-0.975330;,
  -0.283255;-0.079390;-0.955753;,
  -0.249165;-0.083837;-0.964825;,
  -0.125769;-0.111772;-0.985743;,
  0.054781;-0.160748;-0.985474;,
  -0.274121;-0.020350;-0.961480;,
  -0.245020;-0.026054;-0.969168;,
  -0.126941;-0.053532;-0.990465;,
  0.058833;-0.100173;-0.993229;,
  -0.270582;0.176366;-0.946404;,
  -0.243356;0.173589;-0.954277;,
  -0.129405;0.157591;-0.978989;,
  0.055012;0.126460;-0.990445;,
  -0.973232;0.223294;-0.054397;,
  -0.974430;0.217975;0.054534;,
  -0.271872;0.081266;0.958896;,
  -0.244218;0.077770;0.966597;,
  -0.128709;0.059680;0.989885;,
  0.057493;0.028267;0.997946;,
  -0.271257;-0.096733;0.957634;,
  -0.242141;-0.102951;0.964764;,
  -0.124403;-0.131636;0.983461;,
  0.059785;-0.177463;0.982310;,
  -0.289688;-0.076370;0.954069;,
  -0.252481;-0.080660;0.964234;,
  -0.125853;-0.109796;0.985954;,
  0.054770;-0.161458;0.985359;,
  -0.314869;-0.083659;0.945441;,
  -0.268403;-0.086734;0.959394;,
  -0.108176;-0.120120;0.986848;,
  0.076073;-0.182628;0.980235;,
  -0.993640;0.068226;0.089583;,
  -0.993799;0.072853;-0.083997;,
  -0.991239;0.112991;0.068403;,
  -0.991239;0.112991;-0.068403;,
  -0.989346;0.133736;0.057524;,
  -0.988763;0.138015;-0.057444;,
  0.280322;-0.250951;0.926522;,
  0.262207;-0.217797;0.940113;,
  0.279969;-0.272528;-0.920514;,
  0.262390;-0.214217;-0.940884;,
  0.277385;-0.226982;0.933561;,
  0.280099;-0.155037;-0.947369;,
  0.284504;-0.010660;0.958616;,
  0.281911;0.082618;-0.955877;,
  0.875965;-0.350943;0.330945;,
  0.945486;-0.325469;0.011217;,
  0.949149;-0.314692;0.009193;,
  0.950263;-0.311414;0.004596;,
  0.944235;-0.329197;-0.007035;,
  0.951346;-0.308126;-0.000000;,
  0.942232;-0.334665;-0.014067;,
  0.951346;-0.308126;-0.000000;,
  0.959001;-0.283403;-0.000000;,
  0.989458;-0.144818;-0.000000;,
  0.989458;-0.144818;-0.000000;,
  -0.579810;-0.006655;0.814725;,
  -0.259620;-0.062022;0.963717;,
  -0.989918;0.074440;0.120500;,
  -0.061046;-0.108170;0.992256;,
  0.137821;-0.167792;0.976141;,
  -0.991544;0.068483;-0.110230;,
  -0.205052;-0.043002;-0.977806;,
  -0.220740;-0.034815;-0.974711;,
  -0.069509;-0.090282;-0.993488;,
  0.155119;-0.157996;-0.975180;,
  0.371330;-0.251479;0.893796;,
  0.393619;-0.245240;-0.885958;,
  0.839105;-0.308227;0.448218;,
  0.963957;-0.265638;0.014966;,
  0.959276;-0.280427;-0.033910;,
  0.905840;-0.298335;-0.300749;,
  0.087491;-0.994767;0.052758;,
  0.050444;-0.998445;0.023746;,
  0.079300;-0.996780;0.011894;,
  0.140165;-0.989776;0.026425;,
  0.024239;-0.999690;-0.005784;,
  0.023702;-0.999715;-0.002892;,
  0.029178;-0.999569;-0.003125;,
  0.026170;-0.999656;-0.001562;,
  0.088360;-0.996039;-0.009953;,
  0.153230;-0.987991;-0.019873;,
  0.023164;-0.999732;0.000000;,
  0.023162;-0.999732;0.000000;,
  0.068573;-0.997448;-0.019877;,
  0.113808;-0.992709;-0.039706;,
  0.023164;-0.999732;0.000001;,
  0.023162;-0.999732;0.000001;,
  -0.310273;-0.950264;0.027019;,
  -0.284838;-0.958522;0.010102;,
  -0.263007;-0.964586;-0.020025;,
  -0.266406;-0.963277;-0.033551;,
  -0.608227;-0.792126;0.050948;,
  -0.565817;-0.824306;0.019245;,
  -0.527301;-0.848811;-0.038383;,
  -0.532984;-0.843685;-0.064207;,
  -0.254962;0.360084;-0.897404;,
  -0.229787;0.359023;-0.904600;,
  -0.943329;0.327503;-0.053594;,
  -0.124415;0.355384;-0.926403;,
  0.045408;0.350185;-0.935579;,
  -0.944239;0.324852;0.053708;,
  -0.232618;0.313978;0.920492;,
  -0.258233;0.315472;0.913123;,
  -0.125386;0.309057;0.942742;,
  0.047394;0.303227;0.951739;,
  0.259699;0.299188;0.918174;,
  0.254547;0.344571;-0.903591;,
  0.988923;0.148430;0.000000;,
  0.988923;0.148430;0.000000;,
  0.371383;0.337003;-0.865161;,
  -0.240655;0.472577;-0.847795;,
  -0.217337;0.470291;-0.855331;,
  -0.904432;0.423450;-0.051892;,
  -0.119196;0.470212;-0.874467;,
  0.010509;0.502253;-0.864657;,
  -0.898209;0.438202;0.034633;,
  -0.219828;0.445144;0.868057;,
  -0.512261;0.518458;0.684682;,
  -0.120235;0.444400;0.887723;,
  0.011365;0.485405;0.874215;,
  0.231018;0.476339;0.848370;,
  0.226775;0.500720;-0.835376;,
  0.908640;0.417581;0.000000;,
  0.805412;0.522614;0.279619;,
  0.569346;0.570527;-0.591898;,
  0.023163;-0.999732;-0.000002;,
  0.023164;-0.999732;-0.000002;,
  0.023168;-0.999732;-0.000001;,
  0.023170;-0.999732;-0.000000;,
  0.023168;-0.999732;0.000001;,
  0.023164;-0.999732;0.000002;,
  0.023163;-0.999732;0.000002;,
  0.023162;-0.999732;-0.000000;,
  0.302038;-0.920947;0.246231;,
  0.039519;-0.933628;0.356058;,
  -0.166476;-0.951485;0.258772;,
  -0.287674;-0.957728;-0.000031;,
  -0.166476;-0.951485;-0.258772;,
  0.039519;-0.933628;-0.356059;,
  0.302038;-0.920947;-0.246232;,
  0.454248;-0.890875;0.000000;,
  0.573258;-0.644269;0.506254;,
  0.053807;-0.664561;0.745294;,
  -0.397462;-0.716064;0.573826;,
  -0.675490;-0.737369;-0.000048;,
  -0.397390;-0.716126;-0.573798;,
  0.053807;-0.664558;-0.745296;,
  0.573258;-0.644268;-0.506255;,
  0.817983;-0.575242;0.000000;,
  0.592400;-0.597286;0.540659;,
  0.056176;-0.581631;0.811511;,
  -0.446004;-0.622593;0.643008;,
  -0.768548;-0.639792;-0.000035;,
  -0.445867;-0.622738;-0.642962;,
  0.056175;-0.581628;-0.811513;,
  0.592400;-0.597286;-0.540659;,
  0.833376;-0.552707;-0.000000;,
  0.599548;0.557634;0.574095;,
  0.026836;0.534709;0.844610;,
  -0.473045;0.605999;0.639527;,
  -0.749895;0.661556;0.000026;,
  -0.472987;0.605933;-0.639632;,
  0.026837;0.534709;-0.844610;,
  0.599548;0.557635;-0.574095;,
  0.856353;0.516391;0.000000;,
  0.422488;0.815476;0.395606;,
  -0.002056;0.809191;0.587543;,
  -0.324284;0.857808;0.398756;,
  -0.488440;0.872133;-0.028450;,
  -0.324223;0.857805;-0.398812;,
  -0.002056;0.809191;-0.587542;,
  0.422487;0.815476;-0.395606;,
  0.660040;0.751230;0.000000;,
  0.141526;0.979958;0.140190;,
  -0.022653;0.977546;0.209502;,
  -0.148778;0.978622;0.141999;,
  -0.335531;0.941999;-0.007611;,
  -0.148755;0.978625;-0.142006;,
  -0.022653;0.977546;-0.209503;,
  0.141526;0.979958;-0.140190;,
  0.263911;0.964547;0.000000;,
  0.023167;-0.999732;0.000000;,
  -0.023167;0.999732;0.000000;,
  0.023168;-0.999732;0.000003;,
  -0.221840;-0.970234;-0.097119;,
  -0.645892;-0.712926;-0.273058;,
  -0.720569;-0.619477;-0.311492;,
  -0.730701;0.620688;-0.284294;,
  -0.496160;0.858605;-0.128926;,
  -0.343379;0.937441;-0.057410;,
  0.023168;-0.999732;-0.000003;,
  -0.221729;-0.970252;0.097193;,
  -0.646005;-0.712709;0.273358;,
  -0.720964;-0.618827;0.311873;,
  -0.730689;0.620765;0.284156;,
  -0.479574;0.868893;0.122610;,
  -0.266635;0.960321;0.081783;,
  -0.487119;0.872849;-0.029144;,
  -0.750247;0.649455;-0.123844;,
  -0.758959;-0.632138;-0.156148;,
  -0.677007;-0.724654;-0.128599;,
  -0.296191;-0.953698;-0.052261;,
  -0.523476;0.851578;0.028055;,
  -0.750196;0.649511;0.123859;,
  -0.759489;-0.631421;0.156474;,
  -0.677193;-0.724439;0.128837;,
  -0.296035;-0.953743;0.052325;,
  0.005583;0.883199;0.468965;,
  0.077486;0.934152;0.348363;,
  -0.072714;0.821528;0.565513;,
  -0.132253;0.875976;0.463869;,
  -0.342036;0.899907;0.270514;,
  -0.877944;0.476119;0.050253;,
  -0.879803;0.474156;-0.033497;,
  -0.273380;0.938267;-0.211939;,
  -0.132252;0.875975;-0.463871;,
  -0.072715;0.821526;-0.565515;,
  -0.000789;0.927866;-0.372913;,
  0.103965;0.892917;-0.438053;,
  0.454882;0.840537;-0.294246;,
  0.000000;0.000000;-0.000000;,
  0.860054;0.510203;0.000000;,
  0.454881;0.840537;0.294247;,
  -0.023167;0.999732;-0.000001;,
  -0.882760;0.469825;0.000000;,
  -0.035564;0.924825;0.378728;,
  -0.023165;0.999732;0.000000;,
  -0.038128;0.878517;-0.476186;,
  -0.982282;0.057196;0.178469;,
  -0.985096;0.104669;0.136492;,
  -0.985096;0.104669;-0.136492;,
  -0.983647;0.066494;-0.167381;,
  -0.985222;0.127063;0.114856;,
  -0.984099;0.135612;-0.114709;,
  -0.970289;0.216035;0.108943;,
  -0.967896;0.226659;-0.108646;,
  0.359982;-0.276013;0.891196;,
  0.375582;-0.244526;0.893949;,
  0.375949;-0.239429;-0.895174;,
  0.368741;-0.303862;-0.878464;,
  0.396100;-0.249946;0.883534;,
  0.400957;-0.182807;-0.897672;,
  0.409693;-0.032780;0.911634;,
  0.407665;0.055184;-0.911463;,
  -0.979003;0.014003;0.203366;,
  -0.975310;0.030055;-0.218784;,
  0.376009;-0.341295;-0.861473;,
  -0.703686;0.038634;0.709460;,
  -0.243684;-0.036229;0.969178;,
  -0.982642;0.123372;0.138545;,
  -0.032291;-0.093476;0.995098;,
  0.175164;-0.146882;0.973521;,
  -0.986322;0.102180;-0.129341;,
  -0.187903;0.038043;-0.981451;,
  -0.195850;0.032265;-0.980103;,
  -0.966331;0.027413;-0.255838;,
  -0.038177;-0.012066;-0.999198;,
  0.190979;-0.083323;-0.978051;,
  0.433133;-0.243239;0.867888;,
  0.477289;-0.177843;-0.860562;,
  0.866914;-0.292824;0.403379;,
  0.969127;-0.245997;0.016687;,
  0.963758;-0.261307;-0.053758;,
  0.854134;-0.282717;-0.436493;,
  -0.936994;0.332544;-0.107032;,
  -0.938829;0.327256;0.107258;,
  0.378015;0.293450;0.878062;,
  -0.897615;0.428423;-0.103644;,
  -0.923677;0.367973;0.106851;,
  0.362673;0.361964;0.858749;,
  0.962842;0.270065;0.000000;,
  0.620351;0.516681;0.590090;,
  0.035200;0.475759;0.878871;,
  -0.514516;0.491328;0.702759;,
  -0.815244;0.454948;0.358328;,
  -0.815287;0.454754;-0.358479;,
  -0.876600;0.445387;-0.182218;,
  -0.514476;0.491207;-0.702873;,
  0.035199;0.475755;-0.878873;,
  0.620351;0.516681;-0.590090;,
  0.874564;0.484911;0.000000;,
  -0.893650;0.448766;0.000036;,
  -0.876530;0.445517;0.182237;,
  0.860054;0.510203;0.000000;,
  -0.023165;0.999732;0.000000;,
  -0.023170;0.999732;0.000000;,
  -0.023164;0.999732;-0.000001;,
  -0.023183;0.999731;0.000000;;
  263;
  4;0,4,5,1;,
  4;1,5,6,2;,
  4;2,6,7,3;,
  4;4,8,9,5;,
  4;5,9,10,6;,
  4;6,10,11,7;,
  4;8,12,13,9;,
  4;9,13,14,10;,
  4;10,14,15,11;,
  3;231,230,221;,
  3;230,227,221;,
  3;221,227,222;,
  3;222,227,223;,
  3;224,224,224;,
  4;18,22,23,19;,
  4;19,23,24,20;,
  4;20,24,25,21;,
  4;22,26,27,23;,
  4;23,27,28,24;,
  4;24,28,29,25;,
  4;26,30,31,27;,
  4;27,31,32,28;,
  4;28,32,33,29;,
  4;75,78,77,76;,
  4;76,77,80,79;,
  4;79,80,82,81;,
  4;78,84,83,77;,
  4;77,83,85,80;,
  4;80,85,86,82;,
  4;84,88,87,83;,
  4;83,87,89,85;,
  4;85,89,90,86;,
  4;49,51,50,48;,
  4;52,53,51,49;,
  4;54,55,53,52;,
  4;51,56,56,50;,
  4;53,56,56,51;,
  4;55,56,56,53;,
  4;56,57,57,56;,
  4;56,58,57,56;,
  4;56,58,58,56;,
  4;232,233,36,34;,
  4;34,36,37,35;,
  4;35,37,234,235;,
  4;233,236,38,36;,
  4;36,38,39,37;,
  4;37,39,237,234;,
  4;236,238,17,38;,
  4;38,17,16,39;,
  4;39,16,239,237;,
  4;82,92,91,81;,
  4;33,40,41,29;,
  4;86,93,92,82;,
  4;90,94,93,86;,
  4;7,43,42,3;,
  4;29,41,44,25;,
  4;11,45,43,7;,
  4;25,44,46,21;,
  4;15,47,45,11;,
  4;92,96,95,91;,
  4;40,240,241,41;,
  4;93,97,96,92;,
  4;94,98,97,93;,
  4;43,242,243,42;,
  4;41,241,244,44;,
  4;45,245,242,43;,
  4;44,244,246,46;,
  4;47,247,245,45;,
  4;30,59,60,31;,
  4;34,61,248,232;,
  4;31,60,62,32;,
  4;32,62,63,33;,
  4;35,64,61,34;,
  4;1,65,66,0;,
  4;235,249,64,35;,
  4;2,67,65,1;,
  4;3,68,67,2;,
  4;33,63,69,40;,
  4;42,70,68,3;,
  4;40,69,71,48;,
  4;48,71,72,49;,
  4;49,72,73,52;,
  4;52,73,74,54;,
  4;243,250,70,42;,
  4;59,251,252,60;,
  4;61,253,251,59;,
  4;60,252,254,62;,
  4;62,254,255,63;,
  4;64,256,253,61;,
  4;65,257,258,66;,
  4;249,259,256,64;,
  4;67,260,257,65;,
  4;68,261,260,67;,
  4;63,255,262,69;,
  4;70,263,261,68;,
  4;69,262,264,71;,
  4;71,264,265,72;,
  4;72,265,266,73;,
  4;73,266,267,74;,
  4;74,267,263,70;,
  4;12,99,100,13;,
  4;16,101,268,239;,
  4;13,100,102,14;,
  4;14,102,103,15;,
  4;17,104,101,16;,
  4;19,105,106,18;,
  4;238,269,104,17;,
  4;20,107,105,19;,
  4;21,108,107,20;,
  4;46,109,108,21;,
  4;15,103,110,47;,
  4;57,111,111,57;,
  4;246,270,109,46;,
  4;58,112,111,57;,
  4;47,110,113,247;,
  4;58,112,112,58;,
  4;99,114,115,100;,
  4;101,116,271,268;,
  4;100,115,117,102;,
  4;102,117,118,103;,
  4;104,119,116,101;,
  4;105,120,121,106;,
  4;269,272,119,104;,
  4;107,122,120,105;,
  4;108,123,122,107;,
  4;109,124,123,108;,
  4;103,118,125,110;,
  4;111,126,127,111;,
  4;270,273,124,109;,
  4;112,126,126,111;,
  4;110,125,128,113;,
  4;112,274,126,112;,
  4;129,137,138,130;,
  4;130,138,139,131;,
  4;131,139,195,194;,
  3;140,205,132;,
  3;205,188,132;,
  3;132,188,187;,
  4;133,141,142,134;,
  4;134,142,143,135;,
  4;135,143,144,136;,
  4;136,144,137,129;,
  4;137,145,146,138;,
  4;138,146,147,139;,
  4;139,147,196,195;,
  4;204,189,188,205;,
  4;141,149,150,142;,
  4;142,150,151,143;,
  4;143,151,152,144;,
  4;144,152,145,137;,
  4;145,153,154,146;,
  4;146,154,155,147;,
  4;147,155,197,196;,
  4;203,190,189,204;,
  4;149,157,158,150;,
  4;150,158,159,151;,
  4;151,159,160,152;,
  4;152,160,153,145;,
  4;275,161,162,276;,
  4;276,162,163,277;,
  4;277,163,198,278;,
  4;202,191,279,280;,
  4;281,165,166,282;,
  4;282,166,167,283;,
  4;283,167,168,284;,
  4;284,168,161,275;,
  4;161,169,170,162;,
  4;162,170,171,163;,
  4;163,171,199,198;,
  4;201,192,191,202;,
  4;165,173,174,166;,
  4;166,174,175,167;,
  4;167,175,176,168;,
  4;168,176,169,161;,
  4;169,177,178,170;,
  4;170,178,179,171;,
  4;171,179,200,199;,
  3;192,201,193;,
  3;201,172,193;,
  3;193,172,180;,
  4;173,181,182,174;,
  4;174,182,183,175;,
  4;175,183,184,176;,
  4;176,184,177,169;,
  3;130,185,129;,
  3;131,185,130;,
  4;132,185,131,194;,
  4;133,185,132,187;,
  3;134,185,133;,
  3;135,185,134;,
  3;136,185,135;,
  3;129,185,136;,
  3;177,186,178;,
  3;178,186,179;,
  4;179,186,180,200;,
  4;180,186,181,193;,
  3;181,186,182;,
  3;182,186,183;,
  3;183,186,184;,
  3;184,186,177;,
  4;188,141,133,187;,
  4;189,149,141,188;,
  4;190,157,149,189;,
  4;191,165,281,279;,
  4;192,173,165,191;,
  4;193,181,173,192;,
  3;140,132,210;,
  3;210,132,195;,
  3;195,132,194;,
  4;209,148,140,210;,
  4;208,156,148,209;,
  4;207,164,285,286;,
  4;206,172,164,207;,
  3;172,206,180;,
  3;206,199,180;,
  3;180,199,200;,
  4;164,172,201,202;,
  4;285,164,202,280;,
  4;148,156,203,204;,
  4;140,148,204,205;,
  4;199,206,207,198;,
  4;198,207,286,278;,
  4;197,208,209,196;,
  4;196,209,210,195;,
  4;124,212,211,123;,
  3;122,123,213;,
  3;213,123,229;,
  3;229,123,211;,
  4;122,213,214,120;,
  4;120,214,215,121;,
  4;121,215,216,119;,
  3;116,119,217;,
  3;217,119,228;,
  3;228,119,216;,
  4;116,217,218,271;,
  4;114,218,219,115;,
  4;115,219,220,117;,
  3;117,220,118;,
  3;220,231,118;,
  3;118,231,221;,
  4;118,221,222,125;,
  4;125,222,223,128;,
  4;128,223,225,126;,
  3;225,126,287;,
  3;126,126,287;,
  3;287,126,225;,
  4;126,225,226,127;,
  4;127,226,212,124;,
  3;230,229,227;,
  3;229,211,227;,
  3;211,212,227;,
  3;212,288,227;,
  3;288,212,226;,
  3;289,290,230;,
  3;290,215,230;,
  3;215,214,230;,
  3;214,213,230;,
  3;230,213,229;,
  3;291,289,218;,
  3;289,230,218;,
  3;218,230,219;,
  3;219,230,220;,
  3;220,230,231;;
 }
 MeshTextureCoords {
  251;
  0.195340;0.557820;,
  0.202940;0.520910;,
  0.232110;0.520910;,
  0.218370;0.557820;,
  0.267890;0.520910;,
  0.249300;0.557820;,
  0.303680;0.520910;,
  0.280230;0.557820;,
  0.210920;0.486450;,
  0.243360;0.486450;,
  0.282070;0.486450;,
  0.320770;0.486450;,
  0.230250;0.422660;,
  0.265460;0.422660;,
  0.307380;0.422660;,
  0.349290;0.422660;,
  0.342910;0.371270;,
  0.342910;0.371270;,
  0.354590;0.371270;,
  0.385170;0.371270;,
  0.369770;0.371270;,
  0.385170;0.371270;,
  0.385170;0.371270;,
  0.230250;0.422660;,
  0.210920;0.486450;,
  0.243360;0.486450;,
  0.265460;0.422660;,
  0.282070;0.486450;,
  0.307380;0.422660;,
  0.320770;0.486450;,
  0.349290;0.422660;,
  0.202940;0.520910;,
  0.232110;0.520910;,
  0.267890;0.520910;,
  0.303680;0.520910;,
  0.195340;0.557820;,
  0.216480;0.557820;,
  0.247410;0.557820;,
  0.278330;0.557820;,
  0.194150;0.600110;,
  0.178800;0.604470;,
  0.214250;0.600790;,
  0.214250;0.600790;,
  0.240420;0.600790;,
  0.242740;0.601170;,
  0.266590;0.600790;,
  0.266590;0.600790;,
  0.178800;0.604470;,
  0.214250;0.600790;,
  0.240420;0.600790;,
  0.266590;0.600790;,
  0.194150;0.600790;,
  0.218170;0.600790;,
  0.244220;0.600790;,
  0.270180;0.600790;,
  0.329600;0.557820;,
  0.354800;0.520910;,
  0.354800;0.520910;,
  0.327710;0.557820;,
  0.329600;0.557820;,
  0.354800;0.520910;,
  0.329600;0.557820;,
  0.354800;0.520910;,
  0.371420;0.486450;,
  0.371420;0.486450;,
  0.371420;0.486450;,
  0.371420;0.486450;,
  0.404130;0.422660;,
  0.404130;0.422660;,
  0.404130;0.422660;,
  0.404130;0.422660;,
  0.196330;0.520910;,
  0.187450;0.557820;,
  0.196330;0.520910;,
  0.187450;0.557820;,
  0.204660;0.486450;,
  0.204660;0.486450;,
  0.223550;0.422660;,
  0.223550;0.422660;,
  0.287330;0.600790;,
  0.294290;0.600790;,
  0.302850;0.557820;,
  0.329060;0.520910;,
  0.287330;0.600790;,
  0.294290;0.600790;,
  0.329060;0.520910;,
  0.304740;0.557820;,
  0.345920;0.486450;,
  0.345920;0.486450;,
  0.376520;0.422660;,
  0.376520;0.422660;,
  0.304580;0.607790;,
  0.302940;0.607250;,
  0.304580;0.607130;,
  0.301170;0.605180;,
  0.193670;0.587450;,
  0.215090;0.587450;,
  0.184490;0.587450;,
  0.243170;0.587450;,
  0.270830;0.587450;,
  0.184490;0.587450;,
  0.218910;0.587450;,
  0.191930;0.587450;,
  0.246180;0.587450;,
  0.273370;0.587450;,
  0.296460;0.587450;,
  0.294930;0.587450;,
  0.312930;0.587450;,
  0.313540;0.587450;,
  0.313540;0.587450;,
  0.311240;0.587450;,
  0.254270;0.397330;,
  0.283640;0.397330;,
  0.248680;0.397330;,
  0.318590;0.397330;,
  0.353540;0.397330;,
  0.248680;0.397330;,
  0.283640;0.397330;,
  0.254270;0.397330;,
  0.318590;0.397330;,
  0.353540;0.397330;,
  0.376250;0.397330;,
  0.376250;0.397330;,
  0.399280;0.397330;,
  0.399280;0.397330;,
  0.399280;0.397330;,
  0.399280;0.397330;,
  0.288210;0.371270;,
  0.307850;0.371270;,
  0.284480;0.371270;,
  0.331220;0.371270;,
  0.354590;0.371270;,
  0.284480;0.371270;,
  0.307850;0.371270;,
  0.288210;0.371270;,
  0.331220;0.371270;,
  0.354590;0.371270;,
  0.369770;0.371270;,
  0.369770;0.371270;,
  0.385170;0.371270;,
  0.385170;0.371270;,
  0.385170;0.371270;,
  0.385170;0.371270;,
  0.285660;0.628730;,
  0.296360;0.628730;,
  0.244220;0.628730;,
  0.244220;0.628730;,
  0.188230;0.628730;,
  0.202780;0.628730;,
  0.170710;0.628730;,
  0.188280;0.628730;,
  0.167630;0.628730;,
  0.168990;0.628730;,
  0.185620;0.628730;,
  0.170710;0.628730;,
  0.188280;0.628730;,
  0.202780;0.628730;,
  0.188230;0.628730;,
  0.244220;0.628730;,
  0.244220;0.628730;,
  0.296360;0.628730;,
  0.285660;0.628730;,
  0.314090;0.628730;,
  0.302820;0.628730;,
  0.302590;0.622450;,
  0.244220;0.622450;,
  0.180820;0.622450;,
  0.158950;0.622450;,
  0.156360;0.622450;,
  0.158950;0.622450;,
  0.180820;0.622450;,
  0.244220;0.622450;,
  0.302590;0.622450;,
  0.321770;0.622450;,
  0.306010;0.618450;,
  0.244220;0.618450;,
  0.177400;0.618450;,
  0.154820;0.618450;,
  0.150720;0.618450;,
  0.154820;0.618450;,
  0.177400;0.618450;,
  0.244220;0.618450;,
  0.306010;0.618450;,
  0.326610;0.618450;,
  0.300620;0.610080;,
  0.244220;0.610080;,
  0.182790;0.610080;,
  0.161330;0.610080;,
  0.708150;0.205170;,
  0.735740;0.205170;,
  0.688090;0.310240;,
  0.658050;0.310240;,
  0.182790;0.610080;,
  0.244220;0.610080;,
  0.300620;0.610080;,
  0.318990;0.610080;,
  0.294290;0.599670;,
  0.244220;0.599670;,
  0.194150;0.599670;,
  0.183090;0.599490;,
  0.896830;0.071460;,
  0.895120;0.072150;,
  0.194150;0.599670;,
  0.244220;0.599670;,
  0.294290;0.599670;,
  0.307380;0.601510;,
  0.271320;0.592710;,
  0.244220;0.592710;,
  0.217120;0.592710;,
  0.211510;0.592710;,
  0.183090;0.599490;,
  0.183330;0.599430;,
  0.211510;0.592710;,
  0.183870;0.599310;,
  0.205900;0.592710;,
  0.217120;0.592710;,
  0.244220;0.592710;,
  0.271320;0.592710;,
  0.282540;0.592710;,
  0.244220;0.628730;,
  0.244220;0.590270;,
  0.161330;0.610080;,
  0.168990;0.628730;,
  0.156360;0.622450;,
  0.151520;0.622450;,
  0.150720;0.618450;,
  0.146680;0.618450;,
  0.157560;0.610080;,
  0.154300;0.610080;,
  0.183330;0.599430;,
  0.157560;0.610080;,
  0.895120;0.072150;,
  0.896830;0.071460;,
  0.708150;0.205170;,
  0.735740;0.205170;,
  0.658050;0.310240;,
  0.688090;0.310240;,
  0.369770;0.371270;,
  0.354590;0.371270;,
  0.331220;0.371270;,
  0.342910;0.371270;,
  0.307850;0.371270;,
  0.288210;0.371270;,
  0.284480;0.371270;,
  0.284480;0.371270;,
  0.284480;0.371270;,
  0.288210;0.371270;,
  0.307850;0.371270;,
  0.331220;0.371270;,
  0.385170;0.371270;,
  0.385170;0.371270;;
 }
}
