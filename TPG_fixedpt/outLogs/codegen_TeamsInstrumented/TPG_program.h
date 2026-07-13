/**
 * File generated with GEGELATI v2.0.0
 * On the 2026-05-28 21:28:33
 * With the CodeGen::GotoProgramGenerationEngine.
 */
#ifndef C_TPG_program_H
#define C_TPG_program_H

#include "externHeader.h"


inline __attribute__((always_inline)) fixedpt P0(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P1(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[2];
		fixedpt op1 = in4[0];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P2(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P3(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P4(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = reg[7];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P5(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P6(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P7(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in4[3];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P8(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P9(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P10(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P11(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P12(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P13(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in2[1];
		reg[5] = fixedpt_mul(op0,op1);
	}
	{
		fixedpt op0 = reg[5];
		fixedpt op1 = reg[3];
		reg[4] = (op0 > op1) ? op0 : op1;
	}
	{
		fixedpt op0 = reg[4];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P14(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[6];
		reg[0] = f_pow2(op0);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P15(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[0];
		fixedpt op1 = in3[0];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P16(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in3[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P17(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P18(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = reg[0];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P19(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in2[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P20(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P21(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[0];
		fixedpt op1 = in1[0];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P22(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in2[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P23(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in3[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P24(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in2[2];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P25(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[0];
		fixedpt op1 = in3[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P26(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[3];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P27(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = reg[0];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P28(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P29(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		reg[7] = f_pow2(op0);
	}
	{
		fixedpt op0 = reg[7];
		reg[6] = f_pow2(op0);
	}
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = reg[6];
		reg[6] = op0 + op1;
	}
	{
		fixedpt op0 = reg[6];
		fixedpt op1 = in4[1];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P30(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[0];
		fixedpt op1 = in1[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P31(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[2];
		fixedpt op1 = in4[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P32(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[2];
		fixedpt op1 = in4[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P33(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[7];
		reg[6] = f_pow2(op0);
	}
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = reg[6];
		reg[6] = op0 + op1;
	}
	{
		fixedpt op0 = reg[6];
		fixedpt op1 = in4[1];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P34(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[1];
		fixedpt op1 = in4[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P35(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[7];
		reg[6] = f_pow2(op0);
	}
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = reg[6];
		reg[6] = op0 + op1;
	}
	{
		fixedpt op0 = reg[6];
		fixedpt op1 = in4[1];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P36(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = in3[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P37(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = reg[0];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P38(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[2];
		fixedpt op1 = in3[2];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P39(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in3[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P40(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[1];
		fixedpt op1 = in3[0];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P41(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in4[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P42(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[2];
		fixedpt op1 = in4[2];
		reg[3] = (op0 > op1) ? op0 : op1;
	}
	{
		fixedpt op0 = reg[3];
		fixedpt op1 = in1[0];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P43(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[1];
		fixedpt op1 = in4[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P44(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in4[0];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P45(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[1];
		fixedpt op1 = in3[0];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P46(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = reg[6];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P47(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P48(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[1];
		fixedpt op1 = in3[0];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P49(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[0];
		fixedpt op1 = in3[1];
		reg[2] = (op0 > op1) ? op0 : op1;
	}
	{
		fixedpt op0 = in1[1];
		fixedpt op1 = reg[2];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P50(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[7];
		reg[6] = f_pow2(op0);
	}
	{
		fixedpt op0 = in4[3];
		fixedpt op1 = reg[6];
		reg[6] = op0 + op1;
	}
	{
		fixedpt op0 = in2[2];
		fixedpt op1 = reg[6];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P51(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in1[1];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P52(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = in4[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P53(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[0];
		fixedpt op1 = in1[1];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P54(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[1];
		fixedpt op1 = in3[2];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P55(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[7];
		reg[6] = f_pow2(op0);
	}
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = reg[6];
		reg[6] = op0 + op1;
	}
	{
		fixedpt op0 = in2[2];
		fixedpt op1 = reg[6];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P56(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[2];
		fixedpt op1 = in1[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P57(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in1[1];
		fixedpt op1 = in3[2];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P58(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[0];
		fixedpt op1 = in1[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P59(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in2[0];
		fixedpt op1 = in3[2];
		reg[1] = fixedpt_mul(op0,op1);
	}
	{
		fixedpt op0 = reg[1];
		fixedpt op1 = in2[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P60(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = reg[6];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P61(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[0];
		fixedpt op1 = in1[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P62(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[1];
		fixedpt op1 = reg[6];
		reg[6] = op0 + op1;
	}
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = reg[6];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P63(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in1[1];
		reg[0] = fixedpt_mul(op0,op1);
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P64(const fixedpt * __restrict__ in1, const fixedpt * __restrict__ in2, const fixedpt * __restrict__ in3, const fixedpt * __restrict__ in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in4[2];
		fixedpt op1 = in1[1];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

#endif
