/*
*  Fonts
*
*	 Rafael Roman Otero
*
*/

#include <stdint.h>
#include <stdbool.h>
#include "fonts.h"


static VideoFont A, B, C, D, E, F, G, H, I , J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
static VideoFont DOLLAR_SIGN;
static VideoFont* char_font_map[127];
static bool char_is_implemented[127];

bool fonts_is_implemented(uint8_t c){
		return char_is_implemented[c];
}

VideoFont* fonts_char_to_font( uint8_t c ){
		return char_font_map[c];
}

void fonts_init(void){

	// Values are relative to origin
	//   x=0           x= BOX_WIDTH
	//   -------------    <-- y= -BOX_HEIGHT
	//   |            |
	//   |            |
	//   |            |
	//   -------------   <--y=0
	//   ^
	//   |----- (0,0)

	#define BOX_WIDTH		VIDEO_CHARACTER_WIDTH
	#define BOX_HEIGHT	VIDEO_CHARACTER_HEIGHT

	// -- A --
	A.num_lines = 3;
	A.lines[0] = (VideoLine){
	      .init = (VideoXY){ .x=0, .y=0 },
	      .end = (VideoXY){ .x=BOX_WIDTH/2, .y= -(BOX_HEIGHT) }
	   };
	A.lines[1] = (VideoLine){
	      .init = (VideoXY){ .x=BOX_WIDTH/2, .y=-(BOX_HEIGHT) },
	      .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	    };
	A.lines[2] = (VideoLine){
	          .init = (VideoXY){ .x=BOX_WIDTH/3, .y=-BOX_HEIGHT/3 },
	          .end = (VideoXY){ .x=BOX_WIDTH-(BOX_WIDTH/3), .y=-BOX_HEIGHT/3 }
	    };

	//Add entry to character to font map + mark it as implemented
	char_font_map['A'] = &A;
	char_is_implemented['A'] = true;

	// -- E --
	C.num_lines  = 3;
	C.lines[0] = (VideoLine){
			 .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
		 };
	C.lines[1] = (VideoLine){
				.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
				.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
			};
	C.lines[2] = (VideoLine){
							.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
							.end = (VideoXY){ .x=0, .y=0 }
				};

	//Add entry to character to font map + mark it as implemented
	char_font_map['C'] = &C;
	char_is_implemented['C'] = true;

	// -- E --
	E.num_lines  = 4;
	E.lines[0] = (VideoLine){
			 .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
		 };
	E.lines[1] = (VideoLine){
				.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
				.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
			};
	E.lines[2] = (VideoLine){
						.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },
						.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 }
			};
	E.lines[3] = (VideoLine){
							.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
							.end = (VideoXY){ .x=0, .y=0 }
				};

	//Add entry to character to font map + mark it as implemented
	char_font_map['E'] = &E;
	char_is_implemented['E'] = true;

	// -- H --
	H.num_lines  = 3;
	H.lines[0] = (VideoLine){
			 .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=0, .y=-BOX_HEIGHT }
		 };
	H.lines[1] = (VideoLine){
				.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
				.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
			};
	H.lines[2] = (VideoLine){
						.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },
						.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 }
			};

		//Add entry to character to font map + mark it as implemented
		char_font_map['H'] = &H;
		char_is_implemented['H'] = true;

	// -- I --
	I.num_lines  = 3;
	I.lines[0] = (VideoLine){
       .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	   };
	I.lines[1] = (VideoLine){
	      .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
	      .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
	    };
  I.lines[2] = (VideoLine){
	          .init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT },
	          .end = (VideoXY){ .x=BOX_WIDTH/2, .y=0 }
	    };

	//Add entry to character to font map + mark it as implemented
	char_font_map['I'] = &I;
	char_is_implemented['I'] = true;

	// -- L --
	L.num_lines  = 2;
	L.lines[0] = (VideoLine){
       .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	   };
	L.lines[1] = (VideoLine){
	      .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
	      .end = (VideoXY){ .x=0, .y=0 }
	    };


	//Add entry to character to font map + mark it as implemented
	char_font_map['L'] = &L;
	char_is_implemented['L'] = true;

	// -- M` --
  M.num_lines  = 4;
  M.lines[0] = (VideoLine){
 			.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
 			.end = (VideoXY){ .x=0, .y=0 }
 		};
  M.lines[1] = (VideoLine){
 					 .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
 					 .end = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/4 }
 		 };
  M.lines[2] = (VideoLine){
 					.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/4 },
 					.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
 				};
 	M.lines[3] = (VideoLine){
 							 .init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
 							 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
 				 };

  //Add entry to character to font map + mark it as implemented
	char_font_map['M'] = &M;
	char_is_implemented['M'] = true;

	// -- N` --
	N.num_lines  =3;
	N.lines[0] = (VideoLine){
				 .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
				 .end = (VideoXY){ .x=0, .y=0 }
	};
	N.lines[1] = (VideoLine){
				 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
				 	.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	};
	N.lines[2] = (VideoLine){
				 	.init = (VideoXY){ .x=BOX_WIDTH, .y=0 },
				 	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
	};

 //Add entry to character to font map + mark it as implemented
 char_font_map['N'] = &N;
 char_is_implemented['N'] = true;

	// -- O --
	O.num_lines  = 4;
	O.lines[0] = (VideoLine){
       .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	   };
	O.lines[1] = (VideoLine){
	      .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
	      .end = (VideoXY){ .x=0, .y=0 }
	    };
	O.lines[2] = (VideoLine){
		       .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
					 .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
			   };
	O.lines[3] = (VideoLine){
			      .init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
			      .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
			    };

			//Add entry to character to font map + mark it as implemented
			char_font_map['O'] = &O;
			char_is_implemented['O'] = true;

	// -- R --
	P.num_lines  = 3;
	P.lines[0] = (VideoLine){
					.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
					 .end = (VideoXY){ .x=0, .y=0 }
		};
	P.lines[1] = (VideoLine){
				.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
				.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT*(2.0/3.0) }
	 };
	P.lines[2] = (VideoLine){
				.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT*(2.0/3.0) },
				.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(1.0/3.0) }
	};

			//Add entry to character to font map + mark it as implemented
			char_font_map['P'] = &P;
			char_is_implemented['P'] = true;

		// -- R --
		R.num_lines  = 4;
		R.lines[0] = (VideoLine){
			    .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
					 .end = (VideoXY){ .x=0, .y=0 }
	   };
		R.lines[1] = (VideoLine){
		      .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
		      .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT*(2.0/3.0) }
		 };
		R.lines[2] = (VideoLine){
					.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT*(2.0/3.0) },
					.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(1.0/3.0) }
		};
		R.lines[3] = (VideoLine){
					.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(1.0/3.0) },
					.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
		};

	//Add entry to character to font map + mark it as implemented
	char_font_map['R'] = &R;
	char_is_implemented['R'] = true;


	// -- S --
	S.num_lines  = 3;
	S.lines[0] = (VideoLine){
				.init = (VideoXY){ .x=0, .y=0 },
				 .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 }
	 };
	S.lines[1] = (VideoLine){
				.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 },
				.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(2.0/3.0) }
	 };
	S.lines[2] = (VideoLine){
				.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(2.0/3.0) },
				.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
	};


//Add entry to character to font map + mark it as implemented
char_font_map['S'] = &S;
char_is_implemented['S'] = true;

	// -- T --
	T.num_lines  = 2;
	T.lines[0] = (VideoLine){
			 .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
		 };
	T.lines[1] = (VideoLine){
						.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT },
						.end = (VideoXY){ .x=BOX_WIDTH/2, .y=0 }
			};


 //Add entry to character to font map + mark it as implemented
 char_font_map['T'] = &T;
 char_is_implemented['T'] = true;

 // -- W` --
 W.num_lines  = 4;
 W.lines[0] = (VideoLine){
			.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
			.end = (VideoXY){ .x=BOX_WIDTH/4, .y=0 }
		};
 W.lines[1] = (VideoLine){
					 .init = (VideoXY){ .x=BOX_WIDTH/4, .y=0 },
					 .end = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/2 }
		 };
 W.lines[2] = (VideoLine){
					.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/2 },
					.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
				};
W.lines[3] = (VideoLine){
							 .init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
							 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
				 };

		char_font_map['W'] = &W;
		char_is_implemented['W'] = true;
	// -- Z --
	Z.num_lines  = 3;
	Z.lines[0] = (VideoLine){
	 		 .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
	 		 .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
		 	 };
 Z.lines[1] = (VideoLine){
					.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
					.end = (VideoXY){ .x=0, .y=0 }
 		};
  Z.lines[2] = (VideoLine){
				 .init = (VideoXY){ .x=0, .y=0 },
				 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
		};


char_font_map['Z'] = &Z;
char_is_implemented['Z'] = true;


//////////////////////////////////////////
/////////////////////////////////////////

// -- DOLLAR_SIGN --
DOLLAR_SIGN.num_lines  = 4;
DOLLAR_SIGN.lines[0] = (VideoLine){
			.init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 }
 };
DOLLAR_SIGN.lines[1] = (VideoLine){
			.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 },
			.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(2.0/3.0) }
 };
DOLLAR_SIGN.lines[2] = (VideoLine){
			.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(2.0/3.0) },
			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
};
DOLLAR_SIGN.lines[3] = (VideoLine){
			.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT },
			.end = (VideoXY){ .x=BOX_WIDTH/2, .y=0 }
};


//Add entry to character to font map + mark it as implemented
char_font_map['$'] = &DOLLAR_SIGN;
char_is_implemented['$'] = true;


}
