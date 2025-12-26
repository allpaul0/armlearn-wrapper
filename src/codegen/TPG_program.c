/**
 * File generated with GEGELATI v2.0.0
 * On the 2025-12-26 19:52:57
 * With the CodeGen::ProgramGenerationEngine.
 */

#include "TPG_program.h"

extern double* in1;
extern double* in2;
extern double* in3;
extern double* in4;

double P0(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[2];
		double op1 = in3[1];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P1(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

double P2(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[2];
		double op1 = in3[1];
		reg[5] = protected_fixedpt_div(op0,op1);
	}
	{
		double op0 = reg[5];
		double op1 = in4[2];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

double P3(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[3];
		double op1 = in1[1];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P4(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[3];
		double op1 = in2[0];
		reg[4] = op0 - op1;
	}
	{
		double op0 = reg[4];
		double op1 = in4[1];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P5(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[1];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P6(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[1];
		double op1 = in3[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

double P7(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

double P8(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[1];
		double op1 = in3[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

double P9(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

double P10(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[1];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P11(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[1];
		double op1 = in1[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

double P12(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in1[0];
		double op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P13(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[2];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P14(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in1[0];
		double op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P15(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[1];
		double op1 = in1[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

double P16(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in1[0];
		double op1 = in3[0];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

double P17(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[0];
		double op1 = in1[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

double P18(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in1[0];
		double op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P19(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[2];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P20(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[2];
		double op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P21(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[2];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P22(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[2];
		double op1 = reg[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

double P23(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[2];
		double op1 = in1[2];
		reg[3] = fixedpt_mul(op0,op1);
	}
	{
		double op0 = in1[1];
		double op1 = reg[3];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P24(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[1];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P25(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in1[0];
		double op1 = in3[0];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

double P26(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in1[2];
		double op1 = in4[1];
		reg[3] = op0 + op1;
	}
	{
		double op0 = reg[3];
		double op1 = reg[7];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

double P27(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in1[0];
		double op1 = in3[0];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

double P28(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[2];
		double op1 = in1[2];
		reg[3] = fixedpt_mul(op0,op1);
	}
	{
		double op0 = in1[0];
		double op1 = reg[3];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P29(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[2];
		double op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P30(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in1[2];
		double op1 = in4[1];
		reg[3] = op0 + op1;
	}
	{
		double op0 = reg[3];
		double op1 = reg[7];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P31(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[4];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P32(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[1];
		double op1 = in3[1];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

double P33(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[0];
		double op1 = in3[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

double P34(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[0];
		double op1 = in3[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

double P35(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[1];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P36(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[0];
		double op1 = in3[1];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P37(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[0];
		double op1 = in3[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

double P38(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[0];
		double op1 = in2[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P39(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[1];
		double op1 = in3[2];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P40(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[1];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P41(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[0];
		double op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P42(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[0];
		double op1 = in3[2];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

double P43(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in3[0];
		double op1 = in3[2];
		reg[0] = protected_fixedpt_div(op0,op1);
	}
	return reg[0];
}

double P44(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[0];
		double op1 = in4[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P45(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in4[3];
		double op1 = reg[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

double P46(){
	double reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		double op0 = in2[0];
		double op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}
