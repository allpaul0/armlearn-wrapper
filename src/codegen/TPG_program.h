
#ifndef C_TPG_program_H
#define C_TPG_program_H

#include "externHeader.h"


inline __attribute__((always_inline)) fixedpt P0(const fixedpt * in1, const fixedpt * in2, const fixedpt * in3, const fixedpt * in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 + op1;
	}
	// replace by empty program that returns 0
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P1(const fixedpt * in1, const fixedpt * in2, const fixedpt * in3, const fixedpt * in4) {
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

inline __attribute__((always_inline)) fixedpt P2(const fixedpt * in1, const fixedpt * in2, const fixedpt * in3, const fixedpt * in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		fixedpt op1 = in3[2];
		reg[0] = op0 - op1;
	}
	return reg[0];
}

static inline __attribute__((always_inline)) fixedpt P3(const fixedpt * in1, const fixedpt * in2, const fixedpt * in3, const fixedpt * in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[1];
		reg[0] = f_log2(op0);
	}
	return reg[0];
}


inline __attribute__((always_inline)) fixedpt P4(const fixedpt * in1, const fixedpt * in2, const fixedpt * in3, const fixedpt * in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = reg[4];
		fixedpt op1 = in3[1];
		reg[0] = (op0 > op1) ? op0 : op1;
	}
	return reg[0];
}

inline __attribute__((always_inline)) fixedpt P5(const fixedpt * in1, const fixedpt * in2, const fixedpt * in3, const fixedpt * in4) {
	fixedpt reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	{
		fixedpt op0 = in3[2];
		fixedpt op1 = in3[1];
		reg[0] = op0 + op1;
	}
	return reg[0];
}

#endif