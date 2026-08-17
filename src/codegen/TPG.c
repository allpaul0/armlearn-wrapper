/**
 * File generated with GEGELATI v2.0.0
 * On the 2026-07-16 10:06:50
 * With the CodeGen::TPGGenerationEngine.
 */

#include "TPG.h"

static inline int bestProgram(const float *results, int nb) {

	// char buf[256];
	// int pos = 0;
	// pos += snprintf(buf, sizeof(buf), "Results: ");
	// for(int i=0; i<nb; i++){
	// 	pos += snprintf(buf+pos, sizeof(buf) - pos, "%f ", results[i]);
	// }

	int bestProgram = 0;
	float bestScore = (isnan(results[0]))? -INFINITY : results[0];
	for (int i = 1; i < nb; i++) {
		float challengerScore = (isnan(results[i]))? -INFINITY : results[i];
		if (challengerScore >= bestScore) {
			bestProgram = i;
			bestScore = challengerScore;
		}
	}

	// snprintf(buf + pos, sizeof(buf) - pos," | bestProgram=%d score=%f\n", bestProgram, bestScore);
    // printf("%s", buf);

	return bestProgram;
}

/* ------------------------------------------------------------ */
/* Inference — computed goto dispatch                            */
/* ------------------------------------------------------------ */

void inferenceTPG(int *actions,
					const float * __restrict__ in1,
					const float * __restrict__ in2,
					const float * __restrict__ in3,
					const float * __restrict__ in4)
{
	/* Jump table — static const lets GCC keep it in .rodata and
	   potentially cache it in a register across iterations.       */
	static const void * const jump_table[] = {
		&&L_T0, &&L_T1, &&L_T2, &&L_T3, &&L_T4, &&L_T5, &&L_T6, &&L_T7, &&L_T8, &&L_T9, &&L_T10, &&L_T11, &&L_T12, &&L_T13, &&L_T14, &&L_T15, &&L_T16, &&L_T17, &&L_T18, &&L_T19, &&L_A4, &&L_A0, &&L_A7, &&L_A8, &&L_A5, &&L_A6, &&L_A1, &&L_A2
    };

	/* Initial dispatch — always start at T19 */
	goto *jump_table[19];	/* == &&L_T19 */

	/* ---- Team nodes ----------------------------------------- */

L_T0: {
		//printf("Team %d\n", 0);
		static const int next[3] = { 20, 21, 23 };
		float  scores[3];

        scores[0] = P0(in1, in2, in3, in4);
        scores[1] = P1(in1, in2, in3, in4);
        scores[2] = P2(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 3)]];
	}

L_T1: {
		//printf("Team %d\n", 1);
		static const int next[3] = { 0, 24, 22 };
		float  scores[3];

        scores[0] = P3(in1, in2, in3, in4);
        scores[1] = P4(in1, in2, in3, in4);
        scores[2] = P5(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 3)]];
	}

L_T2: {
		//printf("Team %d\n", 2);
		static const int next[2] = { 0, 24 };
		float  scores[2];

        scores[0] = P6(in1, in2, in3, in4);
        scores[1] = P7(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 2)]];
	}

L_T3: {
		//printf("Team %d\n", 3);
		static const int next[5] = { 0, 25, 24, 0, 22 };
		float  scores[5];

        scores[0] = P3(in1, in2, in3, in4);
        scores[1] = P8(in1, in2, in3, in4);
        scores[2] = P9(in1, in2, in3, in4);
        scores[3] = P10(in1, in2, in3, in4);
        scores[4] = P11(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 5)]];
	}

L_T4: {
		//printf("Team %d\n", 4);
		static const int next[3] = { 1, 0, 0 };
		float  scores[3];

        scores[0] = P12(in1, in2, in3, in4);
        scores[1] = P13(in1, in2, in3, in4);
        scores[2] = P14(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 3)]];
	}

L_T5: {
		//printf("Team %d\n", 5);
		static const int next[3] = { 1, 0, 0 };
		float  scores[3];

        scores[0] = P15(in1, in2, in3, in4);
        scores[1] = P14(in1, in2, in3, in4);
        scores[2] = P16(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 3)]];
	}

L_T6: {
		//printf("Team %d\n", 6);
		static const int next[4] = { 5, 22, 25, 25 };
		float  scores[4];

        scores[0] = P17(in1, in2, in3, in4);
        scores[1] = P18(in1, in2, in3, in4);
        scores[2] = P19(in1, in2, in3, in4);
        scores[3] = P20(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 4)]];
	}

L_T7: {
		//printf("Team %d\n", 7);
		static const int next[4] = { 3, 0, 0, 6 };
		float  scores[4];

        scores[0] = P21(in1, in2, in3, in4);
        scores[1] = P22(in1, in2, in3, in4);
        scores[2] = P23(in1, in2, in3, in4);
        scores[3] = P24(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 4)]];
	}

L_T8: {
		//printf("Team %d\n", 8);
		static const int next[5] = { 2, 0, 0, 6, 24 };
		float  scores[5];

        scores[0] = P25(in1, in2, in3, in4);
        scores[1] = P14(in1, in2, in3, in4);
        scores[2] = P26(in1, in2, in3, in4);
        scores[3] = P27(in1, in2, in3, in4);
        scores[4] = P28(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 5)]];
	}

L_T9: {
		//printf("Team %d\n", 9);
		static const int next[4] = { 3, 24, 0, 6 };
		float  scores[4];

        scores[0] = P29(in1, in2, in3, in4);
        scores[1] = P30(in1, in2, in3, in4);
        scores[2] = P14(in1, in2, in3, in4);
        scores[3] = P31(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 4)]];
	}

L_T10: {
		//printf("Team %d\n", 10);
		static const int next[6] = { 3, 0, 6, 24, 8, 27 };
		float  scores[6];

        scores[0] = P21(in1, in2, in3, in4);
        scores[1] = P14(in1, in2, in3, in4);
        scores[2] = P32(in1, in2, in3, in4);
        scores[3] = P33(in1, in2, in3, in4);
        scores[4] = P34(in1, in2, in3, in4);
        scores[5] = P35(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 6)]];
	}

L_T11: {
		//printf("Team %d\n", 11);
		static const int next[3] = { 25, 6, 7 };
		float  scores[3];

        scores[0] = P36(in1, in2, in3, in4);
        scores[1] = P37(in1, in2, in3, in4);
        scores[2] = P38(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 3)]];
	}

L_T12: {
		//printf("Team %d\n", 12);
		static const int next[2] = { 8, 10 };
		float  scores[2];

        scores[0] = P39(in1, in2, in3, in4);
        scores[1] = P40(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 2)]];
	}

L_T13: {
		//printf("Team %d\n", 13);
		static const int next[5] = { 11, 26, 7, 0, 0 };
		float  scores[5];

        scores[0] = P41(in1, in2, in3, in4);
        scores[1] = P42(in1, in2, in3, in4);
        scores[2] = P38(in1, in2, in3, in4);
        scores[3] = P14(in1, in2, in3, in4);
        scores[4] = P43(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 5)]];
	}

L_T14: {
		//printf("Team %d\n", 14);
		static const int next[5] = { 13, 10, 25, 26, 0 };
		float  scores[5];

        scores[0] = P44(in1, in2, in3, in4);
        scores[1] = P45(in1, in2, in3, in4);
        scores[2] = P46(in1, in2, in3, in4);
        scores[3] = P42(in1, in2, in3, in4);
        scores[4] = P47(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 5)]];
	}

L_T15: {
		//printf("Team %d\n", 15);
		static const int next[4] = { 10, 25, 12, 7 };
		float  scores[4];

        scores[0] = P45(in1, in2, in3, in4);
        scores[1] = P46(in1, in2, in3, in4);
        scores[2] = P48(in1, in2, in3, in4);
        scores[3] = P49(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 4)]];
	}

L_T16: {
		//printf("Team %d\n", 16);
		static const int next[2] = { 14, 27 };
		float  scores[2];

        scores[0] = P50(in1, in2, in3, in4);
        scores[1] = P51(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 2)]];
	}

L_T17: {
		//printf("Team %d\n", 17);
		static const int next[2] = { 16, 8 };
		float  scores[2];

        scores[0] = P52(in1, in2, in3, in4);
        scores[1] = P53(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 2)]];
	}

L_T18: {
		//printf("Team %d\n", 18);
		static const int next[3] = { 15, 17, 16 };
		float  scores[3];

        scores[0] = P54(in1, in2, in3, in4);
        scores[1] = P55(in1, in2, in3, in4);
        scores[2] = P56(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 3)]];
	}

L_T19: {
		//printf("Team %d\n", 19);
		static const int next[7] = { 26, 9, 18, 22, 27, 4, 0 };
		float  scores[7];

        scores[0] = P57(in1, in2, in3, in4);
        scores[1] = P58(in1, in2, in3, in4);
        scores[2] = P59(in1, in2, in3, in4);
        scores[3] = P60(in1, in2, in3, in4);
        scores[4] = P61(in1, in2, in3, in4);
        scores[5] = P62(in1, in2, in3, in4);
        scores[6] = P63(in1, in2, in3, in4);

		goto *jump_table[next[bestProgram(scores, 7)]];
	}

L_A4: actions[0] = 4; return;
L_A0: actions[0] = 0; return;
L_A7: actions[0] = 7; return;
L_A8: actions[0] = 8; return;
L_A5: actions[0] = 5; return;
L_A6: actions[0] = 6; return;
L_A1: actions[0] = 1; return;
L_A2: actions[0] = 2; return;
}
