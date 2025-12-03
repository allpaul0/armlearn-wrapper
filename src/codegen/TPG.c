/**
 * File generated with GEGELATI v2.0.0
 * On the 2025-11-21 13:02:17
 * With the CodeGen::TPGGenerationEngine.
 */

#include "TPG.h"

int bestProgram(fixedpt *results, int nb) {
	int bestProgram = 0;
	fixedpt bestScore = results[0];
	for (int i = 1; i < nb; i++) {
		fixedpt challengerScore = results[i];
		if (challengerScore >= bestScore) {
			bestProgram = i;
			bestScore = challengerScore;
		}
	}
	return bestProgram;
}

enum vertices {T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, A25, A26, A27, A28, A29, A30, A31, };

void inferenceTPG(fixedpt* actions) {

	enum vertices currentVertex = T24;
	while(1) {
		switch (currentVertex) {
		case T0: {
				const enum vertices next[2] = { A27, A28,  };

				fixedpt T0Scores[2];

				T0Scores[0] = P0();
				T0Scores[1] = P1();

				int best = bestProgram(T0Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T1: {
				const enum vertices next[2] = { A27, A28,  };

				fixedpt T1Scores[2];

				T1Scores[0] = P2();
				T1Scores[1] = P3();

				int best = bestProgram(T1Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T2: {
				const enum vertices next[2] = { T1, A26,  };

				fixedpt T2Scores[2];

				T2Scores[0] = P4();
				T2Scores[1] = P5();

				int best = bestProgram(T2Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T3: {
				const enum vertices next[3] = { A28, A27, A30,  };

				fixedpt T3Scores[3];

				T3Scores[0] = P3();
				T3Scores[1] = P6();
				T3Scores[2] = P7();

				int best = bestProgram(T3Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T4: {
				const enum vertices next[3] = { A28, A30, T0,  };

				fixedpt T4Scores[3];

				T4Scores[0] = P8();
				T4Scores[1] = P9();
				T4Scores[2] = P10();

				int best = bestProgram(T4Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T5: {
				const enum vertices next[1] = { T2,  };

				fixedpt T5Scores[1];

				T5Scores[0] = P11();

				int best = bestProgram(T5Scores, 1);
				currentVertex = next[best];
				break;
			}
		case T6: {
				const enum vertices next[5] = { A26, A30, A28, A25, A28,  };

				fixedpt T6Scores[5];

				T6Scores[0] = P12();
				T6Scores[1] = P13();
				T6Scores[2] = P14();
				T6Scores[3] = P15();
				T6Scores[4] = P16();

				int best = bestProgram(T6Scores, 5);
				currentVertex = next[best];
				break;
			}
		case T7: {
				const enum vertices next[1] = { T4,  };

				fixedpt T7Scores[1];

				T7Scores[0] = P17();

				int best = bestProgram(T7Scores, 1);
				currentVertex = next[best];
				break;
			}
		case T8: {
				const enum vertices next[1] = { T3,  };

				fixedpt T8Scores[1];

				T8Scores[0] = P18();

				int best = bestProgram(T8Scores, 1);
				currentVertex = next[best];
				break;
			}
		case T9: {
				const enum vertices next[3] = { T6, A25, A28,  };

				fixedpt T9Scores[3];

				T9Scores[0] = P19();
				T9Scores[1] = P20();
				T9Scores[2] = P21();

				int best = bestProgram(T9Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T10: {
				const enum vertices next[2] = { A27, T9,  };

				fixedpt T10Scores[2];

				T10Scores[0] = P22();
				T10Scores[1] = P23();

				int best = bestProgram(T10Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T11: {
				const enum vertices next[2] = { T6, T7,  };

				fixedpt T11Scores[2];

				T11Scores[0] = P24();
				T11Scores[1] = P25();

				int best = bestProgram(T11Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T12: {
				const enum vertices next[4] = { A29, T4, A29, T5,  };

				fixedpt T12Scores[4];

				T12Scores[0] = P26();
				T12Scores[1] = P27();
				T12Scores[2] = P28();
				T12Scores[3] = P29();

				int best = bestProgram(T12Scores, 4);
				currentVertex = next[best];
				break;
			}
		case T13: {
				const enum vertices next[2] = { T11, A26,  };

				fixedpt T13Scores[2];

				T13Scores[0] = P30();
				T13Scores[1] = P12();

				int best = bestProgram(T13Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T14: {
				const enum vertices next[1] = { T12,  };

				fixedpt T14Scores[1];

				T14Scores[0] = P31();

				int best = bestProgram(T14Scores, 1);
				currentVertex = next[best];
				break;
			}
		case T15: {
				const enum vertices next[1] = { T13,  };

				fixedpt T15Scores[1];

				T15Scores[0] = P32();

				int best = bestProgram(T15Scores, 1);
				currentVertex = next[best];
				break;
			}
		case T16: {
				const enum vertices next[2] = { T10, T14,  };

				fixedpt T16Scores[2];

				T16Scores[0] = P33();
				T16Scores[1] = P34();

				int best = bestProgram(T16Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T17: {
				const enum vertices next[2] = { A31, T12,  };

				fixedpt T17Scores[2];

				T17Scores[0] = P35();
				T17Scores[1] = P36();

				int best = bestProgram(T17Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T18: {
				const enum vertices next[6] = { T16, T15, A30, T13, A31, T13,  };

				fixedpt T18Scores[6];

				T18Scores[0] = P37();
				T18Scores[1] = P38();
				T18Scores[2] = P39();
				T18Scores[3] = P40();
				T18Scores[4] = P41();
				T18Scores[5] = P32();

				int best = bestProgram(T18Scores, 6);
				currentVertex = next[best];
				break;
			}
		case T19: {
				const enum vertices next[7] = { T16, A31, A31, T13, T10, T18, T17,  };

				fixedpt T19Scores[7];

				T19Scores[0] = P42();
				T19Scores[1] = P43();
				T19Scores[2] = P44();
				T19Scores[3] = P45();
				T19Scores[4] = P46();
				T19Scores[5] = P47();
				T19Scores[6] = P48();

				int best = bestProgram(T19Scores, 7);
				currentVertex = next[best];
				break;
			}
		case T20: {
				const enum vertices next[2] = { T13, T8,  };

				fixedpt T20Scores[2];

				T20Scores[0] = P45();
				T20Scores[1] = P49();

				int best = bestProgram(T20Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T21: {
				const enum vertices next[2] = { T13, T18,  };

				fixedpt T21Scores[2];

				T21Scores[0] = P50();
				T21Scores[1] = P51();

				int best = bestProgram(T21Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T22: {
				const enum vertices next[4] = { T15, T20, T19, T21,  };

				fixedpt T22Scores[4];

				T22Scores[0] = P52();
				T22Scores[1] = P53();
				T22Scores[2] = P54();
				T22Scores[3] = P55();

				int best = bestProgram(T22Scores, 4);
				currentVertex = next[best];
				break;
			}
		case T23: {
				const enum vertices next[4] = { T16, T22, T19, T17,  };

				fixedpt T23Scores[4];

				T23Scores[0] = P42();
				T23Scores[1] = P56();
				T23Scores[2] = P57();
				T23Scores[3] = P58();

				int best = bestProgram(T23Scores, 4);
				currentVertex = next[best];
				break;
			}
		case T24: {
				const enum vertices next[4] = { A31, T23, T19, T17,  };

				fixedpt T24Scores[4];

				T24Scores[0] = P43();
				T24Scores[1] = P59();
				T24Scores[2] = P57();
				T24Scores[3] = P60();

				int best = bestProgram(T24Scores, 4);
				currentVertex = next[best];
				break;
			}
		case A25: {
				actions[0] = 3;
				return;
			}
		case A26: {
				actions[0] = 4;
				return;
			}
		case A27: {
				actions[0] = 5;
				return;
			}
		case A28: {
				actions[0] = 6;
				return;
			}
		case A29: {
				actions[0] = 8;
				return;
			}
		case A30: {
				actions[0] = 0;
				return;
			}
		case A31: {
				actions[0] = 1;
				return;
			}
		}
	}
}
