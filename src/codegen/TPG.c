/**
 * File generated with GEGELATI v2.0.0
 * On the 2025-12-26 19:52:57
 * With the CodeGen::TPGGenerationEngine.
 */

#include "TPG.h"

int bestProgram(double *results, int nb) {
	int bestProgram = 0;
	double bestScore = (isnan(results[0]))? -INFINITY : results[0];
	for (int i = 1; i < nb; i++) {
		double challengerScore = (isnan(results[i]))? -INFINITY : results[i];
		if (challengerScore >= bestScore) {
			bestProgram = i;
			bestScore = challengerScore;
		}
	}
	return bestProgram;
}

enum vertices {T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, A14, A15, A16, A17, A18, A19, A20, A21, };

void inferenceTPG(double* actions) {

	enum vertices currentVertex = T13;
	while(1) {
		switch (currentVertex) {
		case T0: {
				const enum vertices next[2] = { A14, A15,  };

				double T0Scores[2];

				T0Scores[0] = P0();
				T0Scores[1] = P1();

				int best = bestProgram(T0Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T1: {
				const enum vertices next[3] = { A18, A17, T0,  };

				double T1Scores[3];

				T1Scores[0] = P2();
				T1Scores[1] = P3();
				T1Scores[2] = P4();

				int best = bestProgram(T1Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T2: {
				const enum vertices next[3] = { A20, A18, A14,  };

				double T2Scores[3];

				T2Scores[0] = P5();
				T2Scores[1] = P6();
				T2Scores[2] = P7();

				int best = bestProgram(T2Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T3: {
				const enum vertices next[3] = { A18, A21, A15,  };

				double T3Scores[3];

				T3Scores[0] = P8();
				T3Scores[1] = P9();
				T3Scores[2] = P10();

				int best = bestProgram(T3Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T4: {
				const enum vertices next[3] = { T3, A20, T2,  };

				double T4Scores[3];

				T4Scores[0] = P11();
				T4Scores[1] = P5();
				T4Scores[2] = P12();

				int best = bestProgram(T4Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T5: {
				const enum vertices next[3] = { A20, T2, T3,  };

				double T5Scores[3];

				T5Scores[0] = P13();
				T5Scores[1] = P14();
				T5Scores[2] = P15();

				int best = bestProgram(T5Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T6: {
				const enum vertices next[5] = { T3, T4, T3, T2, A15,  };

				double T6Scores[5];

				T6Scores[0] = P11();
				T6Scores[1] = P16();
				T6Scores[2] = P17();
				T6Scores[3] = P18();
				T6Scores[4] = P19();

				int best = bestProgram(T6Scores, 5);
				currentVertex = next[best];
				break;
			}
		case T7: {
				const enum vertices next[1] = { T6,  };

				double T7Scores[1];

				T7Scores[0] = P20();

				int best = bestProgram(T7Scores, 1);
				currentVertex = next[best];
				break;
			}
		case T8: {
				const enum vertices next[6] = { T1, T7, A20, T6, A20, T4,  };

				double T8Scores[6];

				T8Scores[0] = P21();
				T8Scores[1] = P22();
				T8Scores[2] = P23();
				T8Scores[3] = P20();
				T8Scores[4] = P24();
				T8Scores[5] = P25();

				int best = bestProgram(T8Scores, 6);
				currentVertex = next[best];
				break;
			}
		case T9: {
				const enum vertices next[4] = { A15, T4, A16, T6,  };

				double T9Scores[4];

				T9Scores[0] = P26();
				T9Scores[1] = P27();
				T9Scores[2] = P28();
				T9Scores[3] = P29();

				int best = bestProgram(T9Scores, 4);
				currentVertex = next[best];
				break;
			}
		case T10: {
				const enum vertices next[6] = { T9, T4, A15, T5, T8, T6,  };

				double T10Scores[6];

				T10Scores[0] = P30();
				T10Scores[1] = P25();
				T10Scores[2] = P31();
				T10Scores[3] = P32();
				T10Scores[4] = P33();
				T10Scores[5] = P20();

				int best = bestProgram(T10Scores, 6);
				currentVertex = next[best];
				break;
			}
		case T11: {
				const enum vertices next[4] = { T8, A15, A17, T10,  };

				double T11Scores[4];

				T11Scores[0] = P34();
				T11Scores[1] = P35();
				T11Scores[2] = P36();
				T11Scores[3] = P37();

				int best = bestProgram(T11Scores, 4);
				currentVertex = next[best];
				break;
			}
		case T12: {
				const enum vertices next[7] = { T2, A17, T10, A20, A20, A15, T9,  };

				double T12Scores[7];

				T12Scores[0] = P18();
				T12Scores[1] = P36();
				T12Scores[2] = P38();
				T12Scores[3] = P23();
				T12Scores[4] = P39();
				T12Scores[5] = P40();
				T12Scores[6] = P41();

				int best = bestProgram(T12Scores, 7);
				currentVertex = next[best];
				break;
			}
		case T13: {
				const enum vertices next[5] = { A19, A17, T11, T12, T9,  };

				double T13Scores[5];

				T13Scores[0] = P42();
				T13Scores[1] = P43();
				T13Scores[2] = P44();
				T13Scores[3] = P45();
				T13Scores[4] = P46();

				int best = bestProgram(T13Scores, 5);
				currentVertex = next[best];
				break;
			}
		case A14: {
				actions[0] = 4;
				return;
			}
		case A15: {
				actions[0] = 5;
				return;
			}
		case A16: {
				actions[0] = 3;
				return;
			}
		case A17: {
				actions[0] = 7;
				return;
			}
		case A18: {
				actions[0] = 6;
				return;
			}
		case A19: {
				actions[0] = 1;
				return;
			}
		case A20: {
				actions[0] = 2;
				return;
			}
		case A21: {
				actions[0] = 0;
				return;
			}
		}
	}
}
