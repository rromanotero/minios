/*
*  Fonts
*
*	 Rafael Roman Otero
*
*  TO DO:
*   edit font specs to use less space
*
*  E.g.:
*
*   font.lines[0] = (VideoLine){ 	.init = (VideoXY){ .x=0, .					y=0 }, 							.end = (VideoXY){ .x=BOX_WIDTH/2, 						.y= -(BOX_HEIGHT) } };
*		font.lines[1] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-(BOX_HEIGHT) }, 	.end = (VideoXY){ .x=BOX_WIDTH, 							.y=0 } };
*		font.lines[2] = (VideoLine){ 	.init = (VideoXY){ .x=BOX_WIDTH/3, .y=-BOX_HEIGHT/3 }, 	.end = (VideoXY){ .x=BOX_WIDTH-(BOX_WIDTH/3), .y=-BOX_HEIGHT/3 } };
*
*/

#include <stdint.h>
#include <stdbool.h>
#include "fonts.h"

#define NUM_OF_CHARS_IN_FONT	127

static VideoFont font;		// Used to create evey char in the font
static VideoFont char_font_map[NUM_OF_CHARS_IN_FONT];


VideoFont* fonts_char_to_font( uint8_t c ){
		if( c > 127 ) return &char_font_map[' '];

		//lowercase are mapped onto upppercase
		return &char_font_map[
								(c >= 'a' && c<= 'z') ?
										c-32 :
										c
									];
}

void fonts_init(void){

	//Fill whole map with spaces, so Unimplemented characters ARE
	//displayed as space
	font.num_lines = 0;

	for( int i=0; i<NUM_OF_CHARS_IN_FONT; i++ )
		char_font_map[i] = font;

	//Now we can begin with the characters...

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

	// -- SPACE --
	font.num_lines = 0;

	//Add entry to character to font map + mark it as implemented
	char_font_map[' '] = font;

	// -- A --
	font.num_lines = 3;
	font.lines[0] = (VideoLine){ 	.init = (VideoXY){ .x=0, .					y=0 }, 							.end = (VideoXY){ .x=BOX_WIDTH/2, 						.y= -(BOX_HEIGHT) } };
	font.lines[1] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-(BOX_HEIGHT) }, 	.end = (VideoXY){ .x=BOX_WIDTH, 							.y=0 } };
	font.lines[2] = (VideoLine){ 	.init = (VideoXY){ .x=BOX_WIDTH/3, .y=-BOX_HEIGHT/3 }, 	.end = (VideoXY){ .x=BOX_WIDTH-(BOX_WIDTH/3), .y=-BOX_HEIGHT/3 } };

	//Add entry to character to font map + mark it as implemented
	char_font_map['A'] = font;

	// -- B --
	font.num_lines  = 5;
	font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, 					.y=0 },									.end = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT }  };
	font.lines[1] = (VideoLine){  .init = (VideoXY){ .x=0, 					.y=-BOX_HEIGHT },				.end = (VideoXY){ .x=BOX_WIDTH, .y=-2*BOX_HEIGHT/3 }  };
	font.lines[2] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH, 	.y=-2*BOX_HEIGHT/3 },		.end = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT/2 }  };
 	font.lines[3] = (VideoLine){  .init = (VideoXY){ .x=0, 					.y=-BOX_HEIGHT/2 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 }  };
	font.lines[4] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH, 	.y=-BOX_HEIGHT/3 },			.end = (VideoXY){ .x=0, 				.y=0 }  };

	//Add entry to character to font map + mark it as implemented
	char_font_map['B'] = font;

	// -- C --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){ .init = (VideoXY){ .x=0, .y=0 }, 					.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
	font.lines[1] = (VideoLine){ .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }};
	font.lines[2] = (VideoLine){ .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, .end = (VideoXY){ .x=0, 				.y=0 } };

	//Add entry to character to font map + mark it as implemented
	char_font_map['C'] = font;

	// -- D --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, 					.y=0 },									.end = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT }  };
	font.lines[1] = (VideoLine){  .init = (VideoXY){ .x=0, 					.y=-BOX_HEIGHT },				.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 }  };
	font.lines[2] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH, 	.y=-BOX_HEIGHT/2 },				.end = (VideoXY){ .x=0, 				.y=0 }  };

	//Add entry to character to font map + mark it as implemented
	char_font_map['D'] = font;


	// -- E --
	font.num_lines  = 4;
	font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },							.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
	font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
	font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };
	font.lines[3] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },		.end = (VideoXY){ .x=0, 				.y=0 } };

	//Add entry to character to font map + mark it as implemented
	char_font_map['E'] = font;

	// -- F --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
	font.lines[1] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };
	font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },		.end = (VideoXY){ .x=0, 				.y=0 } };

	//Add entry to character to font map + mark it as implemented
	char_font_map['F'] = font;

	// -- G --
	font.num_lines  = 5;
	font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },							.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
	font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
	font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };
	font.lines[3] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },		.end = (VideoXY){ .x=0, 				.y=0 } };
  font.lines[4] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH, .y=0 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };

	//Add entry to character to font map + mark it as implemented
	char_font_map['G'] = font;


	// -- H --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){
			 .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=0, .y=-BOX_HEIGHT }
		 };
	font.lines[1] = (VideoLine){
				.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
				.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
			};
	font.lines[2] = (VideoLine){
						.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },
						.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 }
			};

		//Add entry to character to font map + mark it as implemented
		char_font_map['H'] = font;

	// -- I --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){
       .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	   };
	font.lines[1] = (VideoLine){
	      .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
	      .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
	    };
  font.lines[2] = (VideoLine){
	          .init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT },
	          .end = (VideoXY){ .x=BOX_WIDTH/2, .y=0 }
	    };

	//Add entry to character to font map + mark it as implemented
	char_font_map['I'] = font;

	// -- K --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){ .init = (VideoXY){ .x=0, .y=0 }, 						.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT } };
	font.lines[1] = (VideoLine){ .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 }, .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
	font.lines[2] = (VideoLine){ .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 }, .end = (VideoXY){ .x=BOX_WIDTH, .y=0 } };

	//Add entry to character to font map + mark it as implemented
	char_font_map['K'] = font;

	// -- L --
	font.num_lines  = 2;
	font.lines[0] = (VideoLine){
       .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	   };
	font.lines[1] = (VideoLine){
	      .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
	      .end = (VideoXY){ .x=0, .y=0 }
	    };


	//Add entry to character to font map + mark it as implemented
	char_font_map['L'] = font;

	// -- M` --
  font.num_lines  = 4;
  font.lines[0] = (VideoLine){
 			.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
 			.end = (VideoXY){ .x=0, .y=0 }
 		};
  font.lines[1] = (VideoLine){
 					 .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
 					 .end = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/4 }
 		 };
  font.lines[2] = (VideoLine){
 					.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/4 },
 					.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
 				};
 	font.lines[3] = (VideoLine){
 							 .init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
 							 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
 				 };

  //Add entry to character to font map + mark it as implemented
	char_font_map['M'] = font;

	// -- N` --
	font.num_lines  =3;
	font.lines[0] = (VideoLine){
				 .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
				 .end = (VideoXY){ .x=0, .y=0 }
	};
	font.lines[1] = (VideoLine){
				 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
				 	.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	};
	font.lines[2] = (VideoLine){
				 	.init = (VideoXY){ .x=BOX_WIDTH, .y=0 },
				 	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
	};

 //Add entry to character to font map + mark it as implemented
 char_font_map['N'] = font;

	// -- O --
	font.num_lines  = 4;
	font.lines[0] = (VideoLine){
       .init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
	   };
	font.lines[1] = (VideoLine){
	      .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
	      .end = (VideoXY){ .x=0, .y=0 }
	    };
	font.lines[2] = (VideoLine){
		       .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
					 .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
			   };
	font.lines[3] = (VideoLine){
			      .init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
			      .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
			    };

			//Add entry to character to font map + mark it as implemented
			char_font_map['O'] = font;

	// -- P --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){
					.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
					 .end = (VideoXY){ .x=0, .y=0 }
		};
	font.lines[1] = (VideoLine){
				.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
				.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT*(2.0/3.0) }
	 };
	font.lines[2] = (VideoLine){
				.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT*(2.0/3.0) },
				.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(1.0/3.0) }
	};

			//Add entry to character to font map + mark it as implemented
			char_font_map['P'] = font;

		// -- R --
		font.num_lines  = 4;
		font.lines[0] = (VideoLine){
			    .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
					 .end = (VideoXY){ .x=0, .y=0 }
	   };
		font.lines[1] = (VideoLine){
		      .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
		      .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT*(2.0/3.0) }
		 };
		font.lines[2] = (VideoLine){
					.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT*(2.0/3.0) },
					.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(1.0/3.0) }
		};
		font.lines[3] = (VideoLine){
					.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(1.0/3.0) },
					.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
		};

	//Add entry to character to font map + mark it as implemented
	char_font_map['R'] = font;


	// -- S --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){ .init = (VideoXY){ .x=0, 				.y=0 }, 										.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 } };
	font.lines[1] = (VideoLine){ .init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 }, 				.end = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT*(2.0/3.0) } };
	font.lines[2] = (VideoLine){ .init = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT*(2.0/3.0) }, .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }};


	//Add entry to character to font map + mark it as implemented
	char_font_map['S'] = font;


	// -- T --
	font.num_lines  = 2;
	font.lines[0] = (VideoLine){
			 .init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
		 };
	font.lines[1] = (VideoLine){
						.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT },
						.end = (VideoXY){ .x=BOX_WIDTH/2, .y=0 }
			};


 //Add entry to character to font map + mark it as implemented
 char_font_map['T'] = font;

 // -- U --
 font.num_lines  = 3;
 font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, 				.y=0 },		.end = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT }  };
 font.lines[1] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH, .y=0 },		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }  };
 font.lines[2] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },						.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };


 //Add entry to character to font map + mark it as implemented
 char_font_map['U'] = font;

 // -- V --
 font.num_lines  = 2;
 font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH/2, 	.y=0 },				.end = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT }  };
 font.lines[1] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH/2, 	.y=0 },				.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }  };


 //Add entry to character to font map + mark it as implemented
 char_font_map['V'] = font;

 // -- W` --
 font.num_lines  = 4;
 font.lines[0] = (VideoLine){
			.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },
			.end = (VideoXY){ .x=BOX_WIDTH/4, .y=0 }
		};
 font.lines[1] = (VideoLine){
					 .init = (VideoXY){ .x=BOX_WIDTH/4, .y=0 },
					 .end = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/2 }
		 };
 font.lines[2] = (VideoLine){
					.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/2 },
					.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
				};
font.lines[3] = (VideoLine){
							 .init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },
							 .end = (VideoXY){ .x=BOX_WIDTH, .y=0 }
				 };

		char_font_map['W'] = font;

		// -- X --
	  font.num_lines  = 2;
	  font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, 	.y=-BOX_HEIGHT },	.end = (VideoXY){ .x=BOX_WIDTH, 	.y=0 }  };
	  font.lines[1] = (VideoLine){  .init = (VideoXY){ .x=0, 	.y=0 },						.end = (VideoXY){ .x=BOX_WIDTH,	 	.y=-BOX_HEIGHT }  };

	  //Add entry to character to font map + mark it as implemented
	  char_font_map['X'] = font;

		// -- Y --
	  font.num_lines  = 3;
	  font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH/2, 	.y=0 },							.end = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT/2 }  };
	  font.lines[1] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH/2, 	.y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=0, 					.y=-BOX_HEIGHT }  };
		font.lines[2] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH/2, 	.y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=BOX_WIDTH, 	.y=-BOX_HEIGHT }  };

	  //Add entry to character to font map + mark it as implemented
	  char_font_map['Y'] = font;

	// -- Z --
	font.num_lines  = 3;
	font.lines[0] = (VideoLine){ .init = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT }, .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
 	font.lines[1] = (VideoLine){ .init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }, .end = (VideoXY){ .x=0, .y=0 } };
  font.lines[2] = (VideoLine){ .init = (VideoXY){ .x=0, 				.y=0 }, 					.end = (VideoXY){ .x=BOX_WIDTH, .y=0 } };

	char_font_map['Z'] = font;

//////////////////////////////////////////
/////////////////////////////////////////

// -- 1 --
font.num_lines  = 1;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH/2, 	.y=0 },	.end = (VideoXY){ .x=BOX_WIDTH/2, 	.y=-BOX_HEIGHT }  };

//Add entry to character to font map + mark it as implemented
char_font_map['1'] = font;

// -- 2 --
font.num_lines  = 3;
font.lines[0] = (VideoLine){ .init = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-5*BOX_HEIGHT/6 } };
font.lines[1] = (VideoLine){ .init = (VideoXY){ .x=BOX_WIDTH, .y=-5*BOX_HEIGHT/6 }, .end = (VideoXY){ .x=0, .y=0 } };
font.lines[2] = (VideoLine){ .init = (VideoXY){ .x=0, 				.y=0 }, 							.end = (VideoXY){ .x=BOX_WIDTH, .y=0 } };

char_font_map['2'] = font;

// -- 3 --
font.num_lines  = 4;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },									.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 				.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };
font.lines[3] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT },.end = (VideoXY){ .x=BOX_WIDTH, .y=0 } };

//Add entry to character to font map + mark it as implemented
char_font_map['3'] = font;

// -- 4 --
font.num_lines  = 3;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=BOX_WIDTH, .y=0 },									.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }  };
font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }, 				.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 } };
font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };

//Add entry to character to font map + mark it as implemented
char_font_map['4'] = font;

// -- 5 --
font.num_lines  = 5;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },							.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };
font.lines[3] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },		.end = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT/2 } };
font.lines[4] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH, .y=0 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };

//Add entry to character to font map + mark it as implemented
char_font_map['5'] = font;

// -- 6 --
font.num_lines  = 5;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },							.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };
font.lines[3] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },		.end = (VideoXY){ .x=0, 				.y=0 } };
font.lines[4] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH, .y=0 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };

//Add entry to character to font map + mark it as implemented
char_font_map['6'] = font;

// -- 7 --
font.num_lines  = 2;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT},	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }  };
font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=BOX_WIDTH,.y=-BOX_HEIGHT }, .end = (VideoXY){ .x=0,	 				.y=0 } };

//Add entry to character to font map + mark it as implemented
char_font_map['7'] = font;


// -- 8 --
font.num_lines  = 5;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },							.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };
font.lines[3] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },		.end = (VideoXY){ .x=0, 				.y=0 } };
font.lines[4] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH, .y=0 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };

//Add entry to character to font map + mark it as implemented
char_font_map['8'] = font;

// -- 9 --
font.num_lines  = 5;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },							.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT/2 },	.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/2 } };
font.lines[3] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },		.end = (VideoXY){ .x=0, 				.y=-BOX_HEIGHT/2 } };
font.lines[4] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH, .y=0 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };

//Add entry to character to font map + mark it as implemented
char_font_map['9'] = font;

// -- 0 --
font.num_lines  = 5;
font.lines[0] = (VideoLine){  .init = (VideoXY){ .x=0, .y=0 },							.end = (VideoXY){ .x=BOX_WIDTH, .y=0 }  };
font.lines[1] = (VideoLine){ 	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT }, 		.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
font.lines[2] = (VideoLine){	.init = (VideoXY){ .x=0, .y=0 },							.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };
font.lines[3] = (VideoLine){	.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT },		.end = (VideoXY){ .x=0, 				.y=0 } };
font.lines[4] = (VideoLine){	.init = (VideoXY){ .x=BOX_WIDTH, .y=0 },			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT } };

//Add entry to character to font map + mark it as implemented
char_font_map['0'] = font;

//////////////////////////////////////////
/////////////////////////////////////////

// -- DOLLAR_SIGN --
font.num_lines  = 4;
font.lines[0] = (VideoLine){
			.init = (VideoXY){ .x=0, .y=0 },
			 .end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 }
 };
font.lines[1] = (VideoLine){
			.init = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT/3 },
			.end = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(2.0/3.0) }
 };
font.lines[2] = (VideoLine){
			.init = (VideoXY){ .x=0, .y=-BOX_HEIGHT*(2.0/3.0) },
			.end = (VideoXY){ .x=BOX_WIDTH, .y=-BOX_HEIGHT }
};
font.lines[3] = (VideoLine){
			.init = (VideoXY){ .x=BOX_WIDTH/2, .y=-BOX_HEIGHT },
			.end = (VideoXY){ .x=BOX_WIDTH/2, .y=0 }
};


//Add entry to character to font map + mark it as implemented
char_font_map['$'] = font;


}
