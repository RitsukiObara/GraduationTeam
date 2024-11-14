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
 235;
 -6.36554;-54.07443;4.89854;,
 -7.70051;-54.04803;6.31798;,
 -9.88819;-54.17665;-0.59706;,
 -7.31747;-54.17665;-0.59706;,
 -8.00604;-54.31478;-8.02332;,
 -6.36554;-54.27886;-6.09266;,
 -4.34741;-54.35798;-10.34604;,
 -3.45658;-54.31462;-8.01551;,
 0.00000;-54.36556;-10.75433;,
 3.47792;-54.36223;-10.57441;,
 0.00000;-54.32119;-8.36899;,
 4.34741;-54.35798;-10.34604;,
 3.45658;-54.31462;-8.01551;,
 6.36554;-54.27886;-6.09266;,
 8.00604;-54.31478;-8.02332;,
 9.88819;-54.17665;-0.59706;,
 7.31750;-54.17665;-0.59706;,
 7.70051;-54.04803;6.31798;,
 6.36554;-54.07443;4.89854;,
 -0.00000;-54.00430;8.66884;,
 -0.00000;-54.03209;7.17489;,
 -8.80243;-52.38279;7.11281;,
 -11.16236;-52.52677;-0.62775;,
 -9.45596;-52.68314;-9.03580;,
 -5.13472;-52.73710;-11.93618;,
 3.70602;-52.74348;-12.28009;,
 5.13472;-52.73710;-11.93618;,
 9.45596;-52.68314;-9.03580;,
 11.16236;-52.52677;-0.62775;,
 8.80245;-52.38279;7.11281;,
 -0.00000;-52.33547;9.65738;,
 -9.24930;-51.32439;7.54704;,
 -11.72905;-51.47681;-0.64728;,
 -9.93602;-51.64161;-9.50904;,
 -5.39541;-51.69733;-12.50341;,
 3.77678;-51.70743;-13.04750;,
 5.39541;-51.69733;-12.50341;,
 9.93602;-51.64161;-9.50904;,
 11.72905;-51.47681;-0.64728;,
 9.24932;-51.32439;7.54704;,
 -0.00000;-51.27357;10.27953;,
 -8.54525;-49.13879;6.79127;,
 -10.83623;-49.27790;-0.68819;,
 -9.17968;-49.42942;-8.83507;,
 -4.98472;-49.48236;-11.68137;,
 3.48930;-49.49166;-12.18105;,
 4.98472;-49.48236;-11.68137;,
 5.39541;-51.69733;-12.50341;,
 3.77678;-51.70743;-13.04750;,
 9.17968;-49.42942;-8.83507;,
 10.83623;-49.27790;-0.68819;,
 8.54525;-49.13879;6.79127;,
 -0.00000;-49.09347;9.22766;,
 -7.85963;-46.41838;5.90150;,
 -9.03501;-46.54190;-0.73906;,
 -7.85963;-46.66541;-7.37962;,
 -4.26791;-46.64482;-8.84680;,
 2.98755;-46.62990;-8.81615;,
 4.26791;-46.64482;-8.84680;,
 7.85965;-46.66541;-7.37962;,
 9.03501;-46.54190;-0.73906;,
 7.85965;-46.41838;5.90150;,
 -0.00000;-46.87076;7.64577;,
 -4.25360;-44.64691;2.82078;,
 -4.88970;-44.71376;-0.77308;,
 -4.25360;-44.78060;-4.36689;,
 -2.12681;-44.79444;-5.11121;,
 4.26791;-46.64482;-8.84680;,
 2.98755;-46.62990;-8.81615;,
 2.12681;-44.79444;-5.11121;,
 0.00000;-46.59504;-8.74463;,
 0.00000;-44.80828;-5.85554;,
 4.25360;-44.78060;-4.36689;,
 4.88970;-44.71376;-0.77308;,
 4.25360;-44.64691;2.82078;,
 -0.00000;-44.61923;4.30940;,
 0.00000;-54.17665;-0.59706;,
 0.00000;-44.07181;-0.78500;,
 4.98472;-49.48236;-11.68137;,
 -3.47792;-54.36223;-10.57441;,
 -3.71619;-52.74348;-12.28009;,
 0.00000;-52.75543;-12.92216;,
 -3.77678;-51.70743;-13.04750;,
 0.00000;-51.71740;-13.58334;,
 -3.48930;-49.49166;-12.18105;,
 0.00000;-49.49968;-12.61330;,
 -2.98753;-46.62990;-8.81615;,
 3.48930;-49.49166;-12.18105;,
 -4.26791;-46.64482;-8.84680;,
 -2.98753;-46.62990;-8.81615;,
 -3.48930;-49.49166;-12.18105;,
 -4.98472;-49.48236;-11.68137;,
 -3.77678;-51.70743;-13.04750;,
 -5.39541;-51.69733;-12.50341;,
 8.21561;-35.66486;-7.42637;,
 9.18151;-25.98086;-8.43469;,
 10.19984;-25.90891;-4.56642;,
 9.12683;-35.60805;-4.37137;,
 11.14934;-25.82064;0.17941;,
 9.97641;-35.53175;-0.26989;,
 11.31624;-25.73237;4.92523;,
 10.12576;-35.45547;3.83157;,
 9.93067;-16.94338;-9.55076;,
 11.03209;-16.86337;-5.24878;,
 12.05903;-16.76790;-0.11572;,
 12.23955;-16.67241;5.01732;,
 10.33137;-0.21654;-11.64251;,
 11.47725;-0.12967;-6.97207;,
 12.54564;-0.02630;-1.41358;,
 12.73345;0.07708;4.14495;,
 0.00000;7.76356;-11.08357;,
 -2.54374;7.76356;-11.08357;,
 0.00000;7.96696;-0.14818;,
 -7.63123;7.77575;-10.42763;,
 -8.47764;7.83992;-6.97781;,
 -9.26680;7.92974;-2.14877;,
 -9.33617;7.97924;0.51190;,
 -10.33137;-0.21654;-11.64251;,
 -9.93067;-16.94338;-9.55076;,
 -11.03209;-16.86337;-5.24878;,
 -11.47723;-0.12967;-6.97207;,
 -12.05903;-16.76790;-0.11572;,
 -12.54564;-0.02630;-1.41358;,
 -12.23957;-16.67241;5.01732;,
 -12.73345;0.07708;4.14495;,
 -9.18151;-25.98086;-8.43469;,
 -10.19984;-25.90891;-4.56642;,
 -11.14934;-25.82064;0.17941;,
 -11.31624;-25.73237;4.92523;,
 -8.21561;-35.66486;-7.42637;,
 -9.12681;-35.61271;-4.62265;,
 -9.97641;-35.53643;-0.52117;,
 -10.12576;-35.46014;3.58029;,
 -7.85963;-46.78076;-7.37748;,
 -2.98440;-47.96354;-9.39190;,
 -2.33800;-46.91063;-4.70779;,
 -8.39763;-46.91063;-4.70779;,
 -2.55564;-46.84608;-1.23752;,
 -9.11693;-46.93957;-0.92794;,
 -2.59389;-46.78155;2.23272;,
 -8.60947;-46.78155;2.23272;,
 2.98440;-47.96354;-9.39190;,
 2.33800;-46.91063;-4.70779;,
 2.55564;-46.84608;-1.23752;,
 2.59389;-46.78155;2.23272;,
 7.85965;-46.96022;-7.37413;,
 8.47332;-46.90096;-4.18795;,
 9.03501;-46.83672;-0.73357;,
 8.46913;-46.77266;2.70958;,
 -2.86463;-35.33369;10.37979;,
 -3.20141;-25.60626;11.70508;,
 -9.60424;-25.60626;11.70508;,
 -8.59385;-35.33834;10.12851;,
 2.86461;-35.33369;10.37979;,
 3.20141;-25.60626;11.70508;,
 8.59388;-35.33369;10.37979;,
 9.60424;-25.60626;11.70508;,
 -3.46261;-16.54749;11.73427;,
 -10.38787;-16.54749;11.73427;,
 3.46261;-16.54749;11.73427;,
 10.38787;-16.54749;11.73427;,
 -3.60234;0.21238;11.41861;,
 -10.80704;0.21238;11.41861;,
 3.60234;0.21238;11.41861;,
 10.80704;0.21238;11.41861;,
 -3.06049;-25.99717;-9.31225;,
 -3.03086;-35.68433;-8.47283;,
 3.06049;-25.99717;-9.31225;,
 3.03086;-35.68433;-8.47283;,
 -3.31022;-16.95883;-10.38179;,
 3.31022;-16.95883;-10.38179;,
 -3.44378;-0.23305;-12.53056;,
 3.44378;-0.23305;-12.53056;,
 -2.49336;-46.73038;4.98355;,
 -7.85963;-46.71320;5.90699;,
 -9.73336;-35.39968;6.83143;,
 -10.87772;-25.66975;8.29138;,
 2.49338;-46.73038;4.98355;,
 7.85965;-46.71320;5.90699;,
 10.87772;-25.66975;8.29138;,
 9.73339;-35.39500;7.08271;,
 -11.76525;-16.61040;8.35225;,
 11.76525;-16.61040;8.35225;,
 -12.24000;0.14427;7.75627;,
 12.24000;0.14427;7.75627;,
 -2.20147;-48.52734;7.30591;,
 -5.87799;-48.38936;7.08532;,
 2.20147;-48.35167;7.30262;,
 5.46575;-47.85008;6.84119;,
 -7.69268;-43.45553;-7.50278;,
 -8.54861;-43.40270;-4.66231;,
 -2.96325;-43.47816;-8.72037;,
 -9.36374;-43.33342;-0.93839;,
 -8.92649;-43.26519;2.73037;,
 2.96325;-43.47816;-8.72037;,
 8.35155;-43.39327;-4.15622;,
 7.75064;-43.45981;-7.73345;,
 8.95606;-43.32600;-0.53951;,
 8.87029;-43.25893;3.06675;,
 -8.40377;-43.20198;6.12896;,
 7.92760;-43.20574;5.92715;,
 -6.99950;-43.16135;8.31319;,
 -2.33316;-43.15984;8.39471;,
 2.33318;-43.15984;8.39471;,
 6.99950;-43.16553;8.08909;,
 9.55314;3.37406;-11.62593;,
 10.61271;3.45438;-7.30730;,
 3.18438;3.35878;-12.44711;,
 11.60062;3.54996;-2.16751;,
 11.77427;3.64556;2.97227;,
 -3.18438;3.35878;-12.44711;,
 -10.61269;3.45438;-7.30730;,
 -9.55314;3.37406;-11.62593;,
 -11.60062;3.54996;-2.16751;,
 -11.77427;3.64556;2.97227;,
 -11.31799;3.70769;6.31161;,
 11.31799;3.70769;6.31161;,
 -3.33099;3.77066;9.69806;,
 -9.99297;3.77066;9.69806;,
 3.33099;3.77066;9.69806;,
 9.99299;3.77066;9.69806;,
 7.63126;7.77575;-10.42763;,
 8.47764;7.83992;-6.97781;,
 2.54374;7.76356;-11.08357;,
 9.26680;7.91629;-2.87204;,
 9.33617;7.95447;-0.81913;,
 9.40554;7.99265;1.23372;,
 -9.40554;8.02505;2.97556;,
 -9.04104;8.07467;5.64307;,
 9.04104;8.04227;3.90125;,
 -2.66087;8.09258;6.60640;,
 -7.98260;8.09258;6.60640;,
 -0.00000;8.09258;6.60640;,
 2.66087;8.09258;6.60640;,
 7.98260;8.09258;6.60640;;
 
 247;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 3;8,9,10;,
 3;9,11,10;,
 3;10,11,12;,
 4;13,14,15,16;,
 4;16,15,17,18;,
 4;18,17,19,20;,
 4;20,19,1,0;,
 4;1,21,22,2;,
 4;2,22,23,4;,
 4;4,23,24,6;,
 4;25,26,11,9;,
 4;14,27,28,15;,
 4;15,28,29,17;,
 4;17,29,30,19;,
 4;19,30,21,1;,
 4;21,31,32,22;,
 4;22,32,33,23;,
 4;23,33,34,24;,
 4;35,36,26,25;,
 4;27,37,38,28;,
 4;28,38,39,29;,
 4;29,39,40,30;,
 4;30,40,31,21;,
 4;31,41,42,32;,
 4;32,42,43,33;,
 4;33,43,44,34;,
 4;45,46,47,48;,
 4;37,49,50,38;,
 4;38,50,51,39;,
 4;39,51,52,40;,
 4;40,52,41,31;,
 4;41,53,54,42;,
 4;42,54,55,43;,
 4;43,55,56,44;,
 4;57,58,46,45;,
 4;49,59,60,50;,
 4;50,60,61,51;,
 4;51,61,62,52;,
 4;52,62,53,41;,
 4;53,63,64,54;,
 4;54,64,65,55;,
 4;55,65,66,56;,
 3;67,68,69;,
 3;68,70,69;,
 3;69,70,71;,
 4;59,72,73,60;,
 4;60,73,74,61;,
 4;61,74,75,62;,
 4;62,75,63,53;,
 3;3,76,0;,
 3;5,76,3;,
 4;10,76,5,7;,
 4;13,76,10,12;,
 3;16,76,13;,
 3;18,76,16;,
 3;20,76,18;,
 3;0,76,20;,
 3;63,77,64;,
 3;64,77,65;,
 4;65,77,71,66;,
 4;71,77,72,69;,
 3;72,77,73;,
 3;73,77,74;,
 3;74,77,75;,
 3;75,77,63;,
 4;11,14,13,12;,
 4;26,27,14,11;,
 4;36,37,27,26;,
 4;78,49,37,36;,
 4;67,59,49,78;,
 4;69,72,59,67;,
 3;8,10,79;,
 3;79,10,6;,
 3;6,10,7;,
 4;80,81,8,79;,
 4;82,83,81,80;,
 4;84,85,83,82;,
 4;86,70,85,84;,
 3;70,86,71;,
 3;86,56,71;,
 3;71,56,66;,
 4;85,70,68,87;,
 4;83,85,87,35;,
 4;81,83,35,25;,
 4;8,81,25,9;,
 4;88,89,90,91;,
 4;91,90,92,93;,
 4;34,82,80,24;,
 4;24,80,79,6;,
 4;94,95,96,97;,
 4;97,96,98,99;,
 4;99,98,100,101;,
 4;95,102,103,96;,
 4;96,103,104,98;,
 4;98,104,105,100;,
 4;102,106,107,103;,
 4;103,107,108,104;,
 4;104,108,109,105;,
 3;110,111,112;,
 3;111,113,112;,
 3;113,114,112;,
 3;114,115,112;,
 3;112,115,116;,
 4;117,118,119,120;,
 4;120,119,121,122;,
 4;122,121,123,124;,
 4;118,125,126,119;,
 4;119,126,127,121;,
 4;121,127,128,123;,
 4;125,129,130,126;,
 4;126,130,131,127;,
 4;127,131,132,128;,
 4;133,134,135,136;,
 4;136,135,137,138;,
 4;138,137,139,140;,
 4;134,141,142,135;,
 4;135,142,143,137;,
 4;137,143,144,139;,
 4;141,145,146,142;,
 4;142,146,147,143;,
 4;143,147,148,144;,
 4;149,150,151,152;,
 4;153,154,150,149;,
 4;155,156,154,153;,
 4;150,157,158,151;,
 4;154,159,157,150;,
 4;156,160,159,154;,
 4;157,161,162,158;,
 4;159,163,161,157;,
 4;160,164,163,159;,
 4;129,125,165,166;,
 4;166,165,167,168;,
 4;168,167,95,94;,
 4;125,118,169,165;,
 4;165,169,170,167;,
 4;167,170,102,95;,
 4;118,117,171,169;,
 4;169,171,172,170;,
 4;170,172,106,102;,
 4;139,173,174,140;,
 4;132,175,176,128;,
 4;144,177,173,139;,
 4;148,178,177,144;,
 4;100,179,180,101;,
 4;128,176,181,123;,
 4;105,182,179,100;,
 4;123,181,183,124;,
 4;109,184,182,105;,
 4;173,185,186,174;,
 4;175,152,151,176;,
 4;177,187,185,173;,
 4;178,188,187,177;,
 4;179,156,155,180;,
 4;176,151,158,181;,
 4;182,160,156,179;,
 4;181,158,162,183;,
 4;184,164,160,182;,
 4;129,189,190,130;,
 4;166,191,189,129;,
 4;130,190,192,131;,
 4;131,192,193,132;,
 4;168,194,191,166;,
 4;97,195,196,94;,
 4;94,196,194,168;,
 4;99,197,195,97;,
 4;101,198,197,99;,
 4;132,193,199,175;,
 4;180,200,198,101;,
 4;175,199,201,152;,
 4;152,201,202,149;,
 4;149,202,203,153;,
 4;153,203,204,155;,
 4;155,204,200,180;,
 4;189,133,136,190;,
 4;191,134,133,189;,
 4;190,136,138,192;,
 4;192,138,140,193;,
 4;194,141,134,191;,
 4;195,146,145,196;,
 4;196,145,141,194;,
 4;197,147,146,195;,
 4;198,148,147,197;,
 4;193,140,174,199;,
 4;200,178,148,198;,
 4;199,174,186,201;,
 4;201,186,185,202;,
 4;202,185,187,203;,
 4;203,187,188,204;,
 4;204,188,178,200;,
 4;106,205,206,107;,
 4;172,207,205,106;,
 4;107,206,208,108;,
 4;108,208,209,109;,
 4;171,210,207,172;,
 4;120,211,212,117;,
 4;117,212,210,171;,
 4;122,213,211,120;,
 4;124,214,213,122;,
 4;183,215,214,124;,
 4;109,209,216,184;,
 4;161,217,218,162;,
 4;162,218,215,183;,
 4;163,219,217,161;,
 4;184,216,220,164;,
 4;164,220,219,163;,
 4;205,221,222,206;,
 4;207,223,221,205;,
 4;206,222,224,208;,
 3;224,225,208;,
 3;208,225,209;,
 3;209,225,226;,
 3;207,210,223;,
 3;223,210,110;,
 3;110,210,111;,
 4;211,114,113,212;,
 4;212,113,111,210;,
 4;213,115,114,211;,
 3;115,213,116;,
 3;213,214,116;,
 3;116,214,227;,
 4;215,228,227,214;,
 4;209,226,229,216;,
 4;217,230,231,218;,
 4;218,231,228,215;,
 3;230,217,232;,
 3;232,217,233;,
 3;233,217,219;,
 4;216,229,234,220;,
 4;220,234,233,219;,
 3;223,110,221;,
 3;110,112,221;,
 3;221,112,222;,
 3;222,112,224;,
 3;224,112,225;,
 3;225,112,226;,
 3;112,232,226;,
 3;226,232,229;,
 3;229,233,234;,
 3;233,229,232;,
 3;112,116,232;,
 3;116,227,232;,
 3;227,228,232;,
 3;228,231,232;,
 3;232,231,230;;
 
 MeshMaterialList {
  2;
  247;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  282;
  -0.000000;-0.999827;0.018597;,
  0.000001;-0.999827;0.018596;,
  0.000002;-0.999827;0.018597;,
  -0.000000;-0.999827;0.018601;,
  -0.000002;-0.999827;0.018597;,
  -0.000001;-0.999827;0.018596;,
  0.000000;-0.999827;0.018597;,
  -0.000000;-0.999827;0.018597;,
  -0.289860;-0.901958;0.320082;,
  -0.418358;-0.907592;0.035402;,
  -0.309667;-0.931409;-0.191267;,
  0.000039;-0.943111;-0.332477;,
  0.309668;-0.931409;-0.191267;,
  0.418358;-0.907592;0.035402;,
  0.289859;-0.901959;0.320081;,
  -0.000000;-0.874722;0.484626;,
  -0.573190;-0.561235;0.597050;,
  -0.825750;-0.561939;0.048598;,
  -0.652320;-0.627920;-0.424494;,
  0.000071;-0.674312;-0.738447;,
  0.652320;-0.627920;-0.424494;,
  0.825750;-0.561939;0.048598;,
  0.573190;-0.561236;0.597049;,
  -0.000000;-0.510482;0.859888;,
  -0.601225;-0.513573;0.612186;,
  -0.997313;-0.054615;0.048821;,
  -0.831169;-0.070700;-0.551507;,
  0.000039;-0.102220;-0.994762;,
  0.831169;-0.070700;-0.551507;,
  0.997313;-0.054614;0.048820;,
  0.601225;-0.513573;0.612186;,
  -0.000000;-0.039509;0.999219;,
  -0.626879;0.470158;0.621268;,
  -0.900104;0.434766;0.028146;,
  -0.704820;0.519390;-0.483180;,
  0.000000;0.606373;-0.795180;,
  0.704820;0.519390;-0.483180;,
  0.900104;0.434765;0.028146;,
  0.626879;0.470159;0.621268;,
  -0.000000;0.444954;0.895553;,
  -0.458407;0.758524;0.463146;,
  -0.670994;0.741462;0.000874;,
  -0.471552;0.808404;-0.352309;,
  0.035865;0.835043;-0.549015;,
  0.471552;0.808404;-0.352309;,
  0.670995;0.741461;0.000874;,
  0.458408;0.758524;0.463146;,
  -0.000000;0.693246;0.720701;,
  -0.180681;0.968286;0.172560;,
  -0.269456;0.962846;-0.017909;,
  -0.183412;0.968864;-0.166324;,
  0.009164;0.924566;-0.380912;,
  0.183412;0.968864;-0.166323;,
  0.269456;0.962846;-0.017909;,
  0.180680;0.968286;0.172560;,
  -0.000000;0.949610;0.313434;,
  -0.000000;-0.999827;0.018596;,
  -0.000000;0.999827;-0.018596;,
  -0.000002;-0.999827;0.018599;,
  0.119383;-0.959619;-0.254715;,
  0.320864;-0.640504;-0.697711;,
  0.430108;-0.084756;-0.898790;,
  0.326290;0.556667;-0.763974;,
  0.156933;0.819776;-0.550762;,
  0.073476;0.917619;-0.390612;,
  0.000002;-0.999827;0.018599;,
  -0.119477;-0.959638;-0.254599;,
  -0.321200;-0.640378;-0.697672;,
  -0.430280;-0.084506;-0.898731;,
  -0.326290;0.556667;-0.763974;,
  -0.150718;0.831931;-0.534018;,
  -0.105743;0.946865;-0.303753;,
  0.035724;0.836669;-0.546542;,
  0.143090;0.592257;-0.792942;,
  0.220550;-0.098268;-0.970413;,
  0.152502;-0.657546;-0.737819;,
  0.064859;-0.938008;-0.340490;,
  -0.034313;0.810095;-0.585294;,
  -0.143090;0.592257;-0.792942;,
  -0.220714;-0.097888;-0.970414;,
  -0.152777;-0.657378;-0.737911;,
  -0.064941;-0.938061;-0.340330;,
  0.964564;-0.097145;-0.245314;,
  0.970611;-0.104028;-0.217010;,
  0.987181;-0.121184;-0.103869;,
  0.985328;-0.152355;0.076921;,
  0.959778;-0.112639;-0.257173;,
  0.967871;-0.110031;-0.226095;,
  0.987464;-0.108783;-0.114376;,
  0.992348;-0.113310;0.049061;,
  0.966177;-0.080424;-0.245018;,
  0.972686;-0.077473;-0.218814;,
  0.991046;-0.070586;-0.113341;,
  0.996590;-0.063942;0.052144;,
  0.967285;0.078073;-0.241379;,
  0.972860;0.081699;-0.216493;,
  0.988661;0.097602;-0.114121;,
  0.991069;0.124198;0.048543;,
  0.063627;-0.049922;-0.996724;,
  -0.063626;-0.049922;-0.996724;,
  -0.967286;0.078073;-0.241376;,
  -0.972860;0.081699;-0.216493;,
  -0.988661;0.097602;-0.114123;,
  -0.991069;0.124199;0.048543;,
  -0.966177;-0.080423;-0.245017;,
  -0.972686;-0.077472;-0.218814;,
  -0.991045;-0.070586;-0.113343;,
  -0.996590;-0.063942;0.052144;,
  -0.958253;-0.112126;-0.263019;,
  -0.967337;-0.108514;-0.229092;,
  -0.987621;-0.107281;-0.114433;,
  -0.992283;-0.113872;0.049059;,
  -0.953461;-0.090325;-0.287669;,
  -0.965486;-0.088719;-0.244878;,
  -0.989804;-0.102509;-0.098896;,
  -0.988375;-0.135785;0.068389;,
  -0.098925;-0.035998;-0.994444;,
  0.092866;-0.032052;-0.995163;,
  -0.075714;-0.108006;-0.991263;,
  0.075714;-0.108006;-0.991263;,
  -0.064631;-0.124673;-0.990091;,
  0.064631;-0.124673;-0.990091;,
  -0.949812;-0.174056;0.259925;,
  -0.962734;-0.120452;0.242146;,
  0.946558;-0.191544;0.259496;,
  0.963082;-0.117527;0.242198;,
  -0.965489;-0.056518;0.254236;,
  0.965490;-0.056518;0.254235;,
  -0.955879;0.158287;0.247469;,
  0.955879;0.158286;0.247468;,
  -0.025672;-0.194549;0.980557;,
  -0.012840;-0.192658;0.981182;,
  -0.010857;-0.075902;0.997056;,
  -0.005427;-0.072582;0.997348;,
  0.007894;-0.195662;0.980640;,
  -0.000000;-0.069260;0.997599;,
  0.015793;-0.200578;0.979550;,
  -0.000000;-0.069260;0.997599;,
  -0.000000;0.007805;0.999970;,
  -0.000000;0.007805;0.999970;,
  -0.000000;0.007805;0.999970;,
  -0.000000;0.007805;0.999970;,
  -0.000000;0.232464;0.972605;,
  -0.000000;0.232464;0.972605;,
  -0.000000;0.232464;0.972605;,
  -0.000000;0.232464;0.972605;,
  -0.830021;-0.002930;-0.557724;,
  -0.970301;-0.047198;-0.237251;,
  -0.132423;0.063358;-0.989166;,
  -0.995018;-0.082296;-0.056273;,
  -0.983831;-0.128579;0.124674;,
  0.121144;0.058351;-0.990918;,
  0.981841;-0.032777;-0.186852;,
  0.979186;-0.026526;-0.201226;,
  0.995587;-0.068767;-0.063858;,
  0.982570;-0.121361;0.140812;,
  -0.916363;-0.197517;0.348233;,
  0.908211;-0.194956;0.370329;,
  -0.616428;-0.250021;0.746664;,
  -0.016720;-0.226244;0.973927;,
  0.037886;-0.239141;0.970245;,
  0.318888;-0.254963;0.912855;,
  -0.069033;-0.993132;0.094466;,
  -0.031066;-0.998230;0.050721;,
  -0.015571;-0.996247;0.085146;,
  -0.034606;-0.986945;0.157295;,
  0.007620;-0.999774;0.019877;,
  0.003810;-0.999808;0.019236;,
  0.004116;-0.999659;0.025773;,
  0.002058;-0.999752;0.022184;,
  0.013041;-0.995296;0.096003;,
  0.026020;-0.984591;0.172926;,
  -0.000000;-0.999827;0.018596;,
  0.000001;-0.999827;0.018596;,
  0.026028;-0.997033;0.072447;,
  0.051965;-0.990664;0.126038;,
  -0.000000;-0.999827;0.018597;,
  0.000002;-0.999827;0.018596;,
  -0.032887;-0.934297;-0.354975;,
  -0.012144;-0.944155;-0.329279;,
  0.025058;-0.951340;-0.307121;,
  0.041975;-0.949668;-0.310433;,
  -0.061008;-0.733356;-0.677102;,
  -0.022800;-0.770629;-0.636876;,
  0.047392;-0.798978;-0.599490;,
  0.079223;-0.792636;-0.604527;,
  0.916705;0.326040;-0.230974;,
  0.921548;0.328110;-0.207590;,
  0.062889;0.167149;-0.983924;,
  0.923693;0.370674;-0.096912;,
  0.915975;0.400542;0.023586;,
  -0.041860;0.210845;-0.976623;,
  -0.921194;0.331102;-0.204383;,
  -0.916705;0.326040;-0.230973;,
  -0.922238;0.375289;-0.092922;,
  -0.917728;0.396406;0.025250;,
  -0.862623;0.434393;0.259198;,
  0.872157;0.438520;0.216890;,
  -0.000000;0.534704;0.845039;,
  -0.248129;0.574219;0.780196;,
  -0.000000;0.510417;0.859927;,
  0.618626;0.568760;0.542046;,
  0.870087;0.440246;-0.221659;,
  0.875050;0.441000;-0.199516;,
  0.040356;0.300824;-0.952825;,
  0.883900;0.454994;-0.108172;,
  0.871720;0.488790;0.034476;,
  -0.000001;0.999827;-0.018597;,
  -0.000001;0.999827;-0.018597;,
  -0.000001;0.999827;-0.018597;,
  -0.000001;0.999827;-0.018599;,
  -0.878941;0.475364;0.038633;,
  -0.451039;0.879757;0.150306;,
  0.369341;0.926028;0.077847;,
  -0.000000;0.581808;0.813326;,
  -0.266327;0.835146;0.481249;,
  -0.000000;0.841665;0.540000;,
  0.304953;0.849981;0.429576;,
  0.000000;0.999827;-0.018592;,
  -0.000000;0.983606;0.180334;,
  0.883842;0.466196;-0.038535;,
  0.000000;0.999827;-0.018597;,
  -0.000000;0.999827;-0.018596;,
  -0.638914;0.432958;0.635875;,
  0.638913;0.432961;0.635874;,
  -0.196896;-0.044302;-0.979423;,
  -0.150999;-0.113454;-0.982002;,
  0.150999;-0.113454;-0.982002;,
  0.184969;-0.036488;-0.982067;,
  -0.128991;-0.126353;-0.983563;,
  0.128991;-0.126353;-0.983563;,
  -0.127015;-0.047394;-0.990768;,
  0.127015;-0.047394;-0.990768;,
  -0.903359;-0.191048;0.383984;,
  -0.928757;-0.121472;0.350221;,
  0.929264;-0.117231;0.350323;,
  0.915053;-0.208816;0.345071;,
  -0.928644;-0.051075;0.367439;,
  0.928644;-0.051075;0.367439;,
  -0.916509;0.176238;0.359097;,
  0.916510;0.176236;0.359096;,
  -0.223580;0.012465;-0.974606;,
  0.240287;0.025755;-0.970360;,
  -0.029632;-0.240492;0.970199;,
  0.896333;-0.269785;0.351858;,
  -0.727507;0.032807;-0.685315;,
  -0.974608;-0.027417;-0.222232;,
  -0.152179;0.104589;-0.982803;,
  -0.997021;-0.071070;-0.029977;,
  -0.980870;-0.112545;0.158831;,
  0.141928;0.086763;-0.986067;,
  0.984928;0.029118;-0.170499;,
  0.983757;0.024732;-0.177794;,
  0.280463;0.023386;-0.959580;,
  0.999353;-0.009133;-0.034786;,
  0.982775;-0.063754;0.173463;,
  -0.891860;-0.192738;0.409192;,
  0.879225;-0.145357;0.453691;,
  -0.432878;-0.245743;0.867310;,
  -0.018619;-0.209210;0.977693;,
  0.060018;-0.222485;0.973087;,
  0.461599;-0.240585;0.853841;,
  0.125551;0.172620;-0.976954;,
  -0.125551;0.172620;-0.976954;,
  -0.870809;0.360731;0.334013;,
  -0.000000;0.435315;0.900278;,
  0.120866;0.309543;-0.943173;,
  0.000000;0.295715;-0.955276;,
  -0.060546;0.303196;-0.951003;,
  -0.873621;0.446700;-0.192988;,
  -0.870086;0.440247;-0.221661;,
  -0.120866;0.309544;-0.943172;,
  -0.881178;0.462482;-0.098164;,
  -0.880402;0.472962;-0.034625;,
  0.000000;0.999827;-0.018581;,
  0.000001;0.999827;-0.018592;,
  0.000000;0.999827;-0.018598;,
  0.000001;0.999827;-0.018596;,
  0.000001;0.999827;-0.018595;,
  0.000000;0.999827;-0.018596;,
  -0.000000;0.999827;-0.018595;,
  0.000000;0.000000;0.000000;;
  247;
  4;0,8,9,1;,
  4;1,9,10,2;,
  4;2,10,66,65;,
  3;11,76,3;,
  3;76,59,3;,
  3;3,59,58;,
  4;4,12,13,5;,
  4;5,13,14,6;,
  4;6,14,15,7;,
  4;7,15,8,0;,
  4;8,16,17,9;,
  4;9,17,18,10;,
  4;10,18,67,66;,
  4;75,60,59,76;,
  4;12,20,21,13;,
  4;13,21,22,14;,
  4;14,22,23,15;,
  4;15,23,16,8;,
  4;16,24,25,17;,
  4;17,25,26,18;,
  4;18,26,68,67;,
  4;74,61,60,75;,
  4;20,28,29,21;,
  4;21,29,30,22;,
  4;22,30,31,23;,
  4;23,31,24,16;,
  4;223,32,33,25;,
  4;25,33,34,26;,
  4;26,34,69,68;,
  4;73,62,61,74;,
  4;28,36,37,29;,
  4;29,37,38,224;,
  4;224,38,39,31;,
  4;31,39,32,223;,
  4;32,40,41,33;,
  4;33,41,42,34;,
  4;34,42,70,69;,
  4;72,63,62,73;,
  4;36,44,45,37;,
  4;37,45,46,38;,
  4;38,46,47,39;,
  4;39,47,40,32;,
  4;40,48,49,41;,
  4;41,49,50,42;,
  4;42,50,71,70;,
  3;63,72,64;,
  3;72,43,64;,
  3;64,43,51;,
  4;44,52,53,45;,
  4;45,53,54,46;,
  4;46,54,55,47;,
  4;47,55,48,40;,
  3;1,56,0;,
  3;2,56,1;,
  4;3,56,2,65;,
  4;4,56,3,58;,
  3;5,56,4;,
  3;6,56,5;,
  3;7,56,6;,
  3;0,56,7;,
  3;48,57,49;,
  3;49,57,50;,
  4;50,57,51,71;,
  4;51,57,52,64;,
  3;52,57,53;,
  3;53,57,54;,
  3;54,57,55;,
  3;55,57,48;,
  4;59,12,4,58;,
  4;60,20,12,59;,
  4;61,28,20,60;,
  4;62,36,28,61;,
  4;63,44,36,62;,
  4;64,52,44,63;,
  3;11,3,81;,
  3;81,3,66;,
  3;66,3,65;,
  4;80,19,11,81;,
  4;79,27,19,80;,
  4;78,35,27,79;,
  4;77,43,35,78;,
  3;43,77,51;,
  3;77,70,51;,
  3;51,70,71;,
  4;35,43,72,73;,
  4;27,35,73,74;,
  4;19,27,74,75;,
  4;11,19,75,76;,
  4;70,77,78,69;,
  4;69,78,79,68;,
  4;68,79,80,67;,
  4;67,80,81,66;,
  4;82,86,87,83;,
  4;83,87,88,84;,
  4;84,88,89,85;,
  4;86,90,91,87;,
  4;87,91,92,88;,
  4;88,92,93,89;,
  4;90,94,95,91;,
  4;91,95,96,92;,
  4;92,96,97,93;,
  3;218,207,221;,
  3;207,209,221;,
  3;209,208,221;,
  3;208,210,221;,
  3;221,210,222;,
  4;100,104,105,101;,
  4;101,105,106,102;,
  4;102,106,107,103;,
  4;104,108,109,105;,
  4;105,109,110,106;,
  4;106,110,111,107;,
  4;108,112,113,109;,
  4;109,113,114,110;,
  4;110,114,115,111;,
  4;162,165,164,163;,
  4;163,164,167,166;,
  4;166,167,169,168;,
  4;165,171,170,164;,
  4;164,170,172,167;,
  4;167,172,173,169;,
  4;171,175,174,170;,
  4;170,174,176,172;,
  4;172,176,177,173;,
  4;131,133,132,130;,
  4;134,135,133,131;,
  4;136,137,135,134;,
  4;133,139,138,132;,
  4;135,140,139,133;,
  4;137,141,140,135;,
  4;139,143,142,138;,
  4;140,144,143,139;,
  4;141,145,144,140;,
  4;225,226,118,116;,
  4;116,118,119,117;,
  4;117,119,227,228;,
  4;226,229,120,118;,
  4;118,120,121,119;,
  4;119,121,230,227;,
  4;229,231,99,120;,
  4;120,99,98,121;,
  4;121,98,232,230;,
  4;169,179,178,168;,
  4;115,122,123,111;,
  4;173,180,179,169;,
  4;177,181,180,173;,
  4;89,125,124,85;,
  4;111,123,126,107;,
  4;93,127,125,89;,
  4;107,126,128,103;,
  4;97,129,127,93;,
  4;179,183,182,178;,
  4;122,233,234,123;,
  4;180,184,183,179;,
  4;181,185,184,180;,
  4;125,235,236,124;,
  4;123,234,237,126;,
  4;127,238,235,125;,
  4;126,237,239,128;,
  4;129,240,238,127;,
  4;112,146,147,113;,
  4;116,148,241,225;,
  4;113,147,149,114;,
  4;114,149,150,115;,
  4;117,151,148,116;,
  4;83,152,153,82;,
  4;228,242,151,117;,
  4;84,154,152,83;,
  4;85,155,154,84;,
  4;115,150,156,122;,
  4;124,157,155,85;,
  4;122,156,158,233;,
  4;130,243,159,131;,
  4;131,159,160,134;,
  4;134,160,161,136;,
  4;236,244,157,124;,
  4;146,245,246,147;,
  4;148,247,245,146;,
  4;147,246,248,149;,
  4;149,248,249,150;,
  4;151,250,247,148;,
  4;152,251,252,153;,
  4;242,253,250,151;,
  4;154,254,251,152;,
  4;155,255,254,154;,
  4;150,249,256,156;,
  4;157,257,255,155;,
  4;156,256,258,158;,
  4;158,258,259,159;,
  4;159,259,260,160;,
  4;160,260,261,161;,
  4;161,261,257,157;,
  4;94,186,187,95;,
  4;98,188,262,232;,
  4;95,187,189,96;,
  4;96,189,190,97;,
  4;99,191,188,98;,
  4;101,192,193,100;,
  4;231,263,191,99;,
  4;102,194,192,101;,
  4;103,195,194,102;,
  4;128,196,195,103;,
  4;97,190,197,129;,
  4;143,198,199,142;,
  4;239,264,196,128;,
  4;144,200,198,143;,
  4;129,197,201,240;,
  4;145,265,200,144;,
  4;186,202,203,187;,
  4;188,204,266,262;,
  4;187,203,205,189;,
  3;205,220,189;,
  3;189,220,190;,
  3;190,220,206;,
  3;188,191,204;,
  3;204,191,267;,
  3;267,191,268;,
  4;192,269,270,193;,
  4;263,271,268,191;,
  4;194,272,269,192;,
  3;272,194,273;,
  3;194,195,273;,
  3;273,195,211;,
  4;196,212,211,195;,
  4;190,206,213,197;,
  4;198,214,215,199;,
  4;199,215,212,196;,
  3;214,198,219;,
  3;219,198,216;,
  3;216,198,200;,
  4;197,213,217,201;,
  4;201,217,216,200;,
  3;274,218,275;,
  3;218,221,275;,
  3;275,221,276;,
  3;276,221,277;,
  3;277,221,278;,
  3;278,221,279;,
  3;221,219,279;,
  3;279,219,213;,
  3;213,216,217;,
  3;216,213,219;,
  3;221,222,219;,
  3;222,280,219;,
  3;280,212,219;,
  3;212,215,219;,
  3;281,281,281;;
 }
 MeshTextureCoords {
  235;
  0.324170;0.628730;,
  0.338240;0.628730;,
  0.269670;0.628730;,
  0.269670;0.628730;,
  0.196030;0.628730;,
  0.215180;0.628730;,
  0.173000;0.628730;,
  0.196110;0.628730;,
  0.168950;0.628730;,
  0.170740;0.628730;,
  0.192600;0.628730;,
  0.173000;0.628730;,
  0.196110;0.628730;,
  0.215180;0.628730;,
  0.196030;0.628730;,
  0.269670;0.628730;,
  0.269670;0.628730;,
  0.338240;0.628730;,
  0.324170;0.628730;,
  0.361560;0.628730;,
  0.346740;0.628730;,
  0.346430;0.622140;,
  0.269670;0.622140;,
  0.186300;0.622140;,
  0.157540;0.622140;,
  0.154130;0.622140;,
  0.157540;0.622140;,
  0.186300;0.622140;,
  0.269670;0.622140;,
  0.346430;0.622140;,
  0.371660;0.622140;,
  0.350930;0.617960;,
  0.269670;0.617960;,
  0.181800;0.617960;,
  0.152100;0.617960;,
  0.146710;0.617960;,
  0.152100;0.617960;,
  0.181800;0.617960;,
  0.269670;0.617960;,
  0.350930;0.617960;,
  0.378020;0.617960;,
  0.343840;0.609180;,
  0.269670;0.609180;,
  0.188890;0.609180;,
  0.160660;0.609180;,
  0.710190;0.193180;,
  0.740130;0.193180;,
  0.688430;0.307180;,
  0.655830;0.307180;,
  0.188890;0.609180;,
  0.269670;0.609180;,
  0.343840;0.609180;,
  0.368000;0.609180;,
  0.335520;0.598270;,
  0.269670;0.598270;,
  0.203820;0.598270;,
  0.189280;0.598080;,
  0.914900;0.048110;,
  0.913050;0.048850;,
  0.203820;0.598270;,
  0.269670;0.598270;,
  0.335520;0.598270;,
  0.352730;0.600200;,
  0.305310;0.590980;,
  0.269670;0.590980;,
  0.234040;0.590980;,
  0.226650;0.590980;,
  0.189280;0.598080;,
  0.189590;0.598020;,
  0.226650;0.590980;,
  0.190300;0.597890;,
  0.219270;0.590980;,
  0.234040;0.590980;,
  0.269670;0.590980;,
  0.305310;0.590980;,
  0.320070;0.590980;,
  0.269670;0.628730;,
  0.269670;0.588420;,
  0.160660;0.609180;,
  0.170740;0.628730;,
  0.154130;0.622140;,
  0.147760;0.622140;,
  0.146710;0.617960;,
  0.141400;0.617960;,
  0.155710;0.609180;,
  0.151420;0.609180;,
  0.189590;0.598020;,
  0.155710;0.609180;,
  0.913050;0.048850;,
  0.914900;0.048110;,
  0.710190;0.193180;,
  0.740130;0.193180;,
  0.655830;0.307180;,
  0.688430;0.307180;,
  0.205390;0.554400;,
  0.197180;0.515700;,
  0.235540;0.515700;,
  0.235680;0.554400;,
  0.282600;0.515700;,
  0.276350;0.554400;,
  0.329660;0.515700;,
  0.317020;0.554400;,
  0.187780;0.479580;,
  0.230440;0.479580;,
  0.281340;0.479580;,
  0.332240;0.479580;,
  0.170130;0.412720;,
  0.216440;0.412720;,
  0.271560;0.412720;,
  0.326680;0.412720;,
  0.199400;0.371270;,
  0.199400;0.371270;,
  0.282760;0.371270;,
  0.204310;0.371270;,
  0.230130;0.371270;,
  0.273910;0.371270;,
  0.289280;0.371270;,
  0.170130;0.412720;,
  0.187780;0.479580;,
  0.230440;0.479580;,
  0.216440;0.412720;,
  0.281340;0.479580;,
  0.271560;0.412720;,
  0.332240;0.479580;,
  0.326680;0.412720;,
  0.197180;0.515700;,
  0.235540;0.515700;,
  0.282600;0.515700;,
  0.329660;0.515700;,
  0.205390;0.554400;,
  0.233190;0.554400;,
  0.273860;0.554400;,
  0.314530;0.554400;,
  0.203820;0.598730;,
  0.183640;0.603300;,
  0.230260;0.599440;,
  0.230260;0.599440;,
  0.264670;0.599440;,
  0.267730;0.599840;,
  0.299090;0.599440;,
  0.299090;0.599440;,
  0.183640;0.603300;,
  0.230260;0.599440;,
  0.264670;0.599440;,
  0.299090;0.599440;,
  0.203820;0.599440;,
  0.235420;0.599440;,
  0.269670;0.599440;,
  0.303820;0.599440;,
  0.381960;0.554400;,
  0.396890;0.515700;,
  0.396890;0.515700;,
  0.379470;0.554400;,
  0.381960;0.554400;,
  0.396890;0.515700;,
  0.381960;0.554400;,
  0.396890;0.515700;,
  0.398850;0.479580;,
  0.398850;0.479580;,
  0.398850;0.479580;,
  0.398850;0.479580;,
  0.398810;0.412720;,
  0.398810;0.412720;,
  0.398810;0.412720;,
  0.398810;0.412720;,
  0.188480;0.515700;,
  0.195010;0.554400;,
  0.188480;0.515700;,
  0.195010;0.554400;,
  0.179540;0.479580;,
  0.179540;0.479580;,
  0.161320;0.412720;,
  0.161320;0.412720;,
  0.326360;0.599440;,
  0.335520;0.599440;,
  0.346770;0.554400;,
  0.363040;0.515700;,
  0.326360;0.599440;,
  0.335520;0.599440;,
  0.363040;0.515700;,
  0.349260;0.554400;,
  0.365310;0.479580;,
  0.365310;0.479580;,
  0.362490;0.412720;,
  0.362490;0.412720;,
  0.349050;0.606780;,
  0.346890;0.606220;,
  0.349050;0.606080;,
  0.344570;0.604050;,
  0.203190;0.585460;,
  0.231360;0.585460;,
  0.191120;0.585460;,
  0.268290;0.585460;,
  0.304670;0.585460;,
  0.191120;0.585460;,
  0.236380;0.585460;,
  0.200910;0.585460;,
  0.272240;0.585460;,
  0.308000;0.585460;,
  0.338370;0.585460;,
  0.336370;0.585460;,
  0.360030;0.585460;,
  0.360840;0.585460;,
  0.360840;0.585460;,
  0.357810;0.585460;,
  0.180930;0.393110;,
  0.219550;0.393110;,
  0.173580;0.393110;,
  0.265520;0.393110;,
  0.311490;0.393110;,
  0.173580;0.393110;,
  0.219550;0.393110;,
  0.180930;0.393110;,
  0.265520;0.393110;,
  0.311490;0.393110;,
  0.341350;0.393110;,
  0.341350;0.393110;,
  0.371640;0.393110;,
  0.371640;0.393110;,
  0.371640;0.393110;,
  0.371640;0.393110;,
  0.204310;0.371270;,
  0.230130;0.371270;,
  0.199400;0.371270;,
  0.260870;0.371270;,
  0.276240;0.371270;,
  0.291600;0.371270;,
  0.304640;0.371270;,
  0.324610;0.371270;,
  0.311570;0.371270;,
  0.331820;0.371270;,
  0.331820;0.371270;,
  0.331820;0.371270;,
  0.331820;0.371270;,
  0.331820;0.371270;;
 }
}
