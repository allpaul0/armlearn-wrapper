/**
 * File generated with GEGELATI v2.0.0
 * On the 2025-11-21 13:02:17
 * With the CodeGen::ProgramGenerationEngine.
 */

#include "TPG_program.h"

fixedpt P0(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

fixedpt P1(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in3[2];
		reg[1] = op0 + op1;
	}
	{
		fixedpt op0 = in2[0];
		fixedpt op1 = reg[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P2(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

fixedpt P3(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P4(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

fixedpt P5(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

fixedpt P6(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[4];
		fixedpt op1 = in3[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P7(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[2];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

fixedpt P8(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P9(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[2];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

fixedpt P10(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[0];
		fixedpt op1 = in3[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P11(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[4];
		fixedpt op1 = in4[1];
		reg[4] = op0 + op1;
	}
	{
		fixedpt op0 = reg[4];
		fixedpt op1 = in1[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P12(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in1[0];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P13(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		reg[0] = f_pow2(op0);
	}
	return reg[0];
}

fixedpt P14(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[0];
		fixedpt op1 = in3[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P15(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = reg[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P16(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P17(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[2];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

fixedpt P18(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[2];
		fixedpt op1 = reg[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P19(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

fixedpt P20(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = reg[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P21(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P22(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[2];
		fixedpt op1 = reg[5];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P23(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[1];
		fixedpt op1 = in4[4];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P24(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[6];
		fixedpt op1 = in4[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P25(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = in4[4];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P26(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = reg[6];
		reg[6] = op0 + op1;
	}
	{
		fixedpt op0 = in1[0];
		fixedpt op1 = reg[6];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P27(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[2];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

fixedpt P28(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in1[0];
		reg[7] = (op0 > op1) ? op0 : op1;
	}
	{
		fixedpt op0 = reg[7];
		reg[6] = f_log2(op0);
	}
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = reg[6];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P29(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[0];
		fixedpt op1 = in1[0];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P30(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[1];
		fixedpt op1 = in3[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P31(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in1[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P32(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[6];
		fixedpt op1 = in4[0];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P33(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = reg[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P34(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[0];
		fixedpt op1 = in4[3];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P35(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[0];
		fixedpt op1 = in4[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P36(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in1[2];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P37(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		reg[7] = f_log2(op0);
	}
	{
		fixedpt op0 = in1[2];
		fixedpt op1 = in4[1];
		reg[3] = op0 + op1;
	}
	{
		fixedpt op0 = reg[3];
		fixedpt op1 = reg[7];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P38(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in4[3];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P39(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[2];
		fixedpt op1 = in1[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P40(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = in4[4];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P41(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[0];
		fixedpt op1 = in4[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P42(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		reg[7] = f_log2(op0);
	}
	{
		fixedpt op0 = in1[2];
		fixedpt op1 = in4[1];
		reg[3] = op0 + op1;
	}
	{
		fixedpt op0 = reg[3];
		fixedpt op1 = reg[7];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

fixedpt P43(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[2];
		fixedpt op1 = in3[2];
		reg[6] = op0 - op1;
	}
	{
		fixedpt op0 = reg[6];
		fixedpt op1 = in1[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P44(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[1];
		fixedpt op1 = in4[2];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P45(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = in1[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P46(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P47(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = in1[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P48(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in3[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P49(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = in1[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P50(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[2];
		fixedpt op1 = in4[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P51(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = in1[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P52(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in4[3];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P53(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		reg[4] = f_log2(op0);
	}
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = reg[4];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P54(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[1];
		fixedpt op1 = in4[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P55(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = in3[0];
		reg[3] = op0 - op1;
	}
	{
		fixedpt op0 = reg[0];
		fixedpt op1 = reg[3];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P56(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = in3[2];
		reg[6] = op0 - op1;
	}
	{
		fixedpt op0 = reg[6];
		fixedpt op1 = in1[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

fixedpt P57(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[1];
		fixedpt op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P58(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in3[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P59(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in4[3];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

fixedpt P60(){
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[2];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}
