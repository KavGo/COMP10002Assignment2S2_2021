/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Kaveesh Gollaheennalage 1270280
  Dated:     5/10/21

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
/**************************************************************************/
/* some #define's from skeleton code*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

/*More #defines*/
#define ROWS_SHIFTED_RIGHT 2        // Creates diagonal grid
#define LAST_EMPTY_ROW 4            // Helps create two empty rows

#define PLAYERMOVE 2                //Player's next turn
#define BMOVE 0                     //Black's turn
#define WMOVE 1                     //White's turn
#define COL1 'A'                    //Column 1
#define COL8 'H'                    //Column 2
#define STAGE1 'A'                  //Stage 1 command
#define STAGE2 'P'                  //Stage 2 command

#define BLACK 'B'                    
#define WHITE 'W'
#define NE 1                        //North-East
#define SE 2                        //South-East
#define SW 3                        //South-West
#define NW 4                        //North-West

#define DIVIDER "+---+---+---+---+---+---+---+---+\n"  //Checker board
#define DIVIDER2 "=====================================\n"   //Seperates output
#define COLUMNS "A   B   C   D   E   F   G   H\n"      //Columns label
#define ERRORCODE1 "ERROR: Source cell is outside of the board.\n" 
#define ERRORCODE2 "ERROR: Target cell is outside of the board.\n"
#define ERRORCODE3 "ERROR: Source cell is empty.\n"
#define ERRORCODE4 "ERROR: Target cell is not empty.\n"
#define ERRORCODE5 "ERROR: Source cell holds opponent's piece/tower.\n"
#define ERRORCODE6 "ERROR: Illegal action.\n"
#define INITIALTXT1 "BOARD SIZE: 8x8\n"
#define INITIALTXT2 "#BLACK PIECES: 12\n"
#define INITIALTXT3 "#WHITE PIECES: 12\n"
/**************************************************************************/
/* Type definitions*/
typedef char checkboard_t[BOARD_SIZE][BOARD_SIZE];

typedef struct {
	checkboard_t cboard;
	int cost;
} board_t;

typedef struct node node_t;

struct node {
	board_t *pot_board;
	node_t  **nodes;
	int nnodes;
	int nummoves;
};

/**************************************************************************/
board_t *make_board(void);
void disp_initial(board_t*);
void disp_board(board_t*);
void cost_counter(board_t*);
int actions(board_t*,char*);
void error_check(board_t*,char,int,char,int,char);
void disp_action(board_t*,char,int,char,int,int);
void promote_pieces(board_t*,char);
int valid_action(board_t*,int,int,int);
int nactions(board_t*,char);
void end_game(board_t*,char);
void auto_command(board_t*,int,char);
board_t *auto_move(board_t*,int);
void fill3_node(node_t*);
void fill2_node(node_t*);
void fill_node(node_t*);
char turn(int);
node_t *create_tree(board_t*,int);
void copy_board(node_t*,node_t*);
void disp_auto(board_t*,int,char,int,char,int);
node_t *new_node(void);
node_t **empty_array(int);
int minimax(node_t*,int,char);
int maxcal(int,int);
int mincal(int,int);
void cost_counter2(board_t*,char);
board_t *searchboard(node_t*,int,int);
void disp_auto(board_t*,int,char,int,char,int);
void movecal(board_t*,board_t*,int,char*,int*,char*,int*);
/**************************************************************************/
int
main(int argc, char *argv[]) {
    char command;
    board_t *curr_board;
    int count;
    
    /*Stage 0*/
    curr_board = make_board();
    disp_initial(curr_board);
    count = actions(curr_board,&command);
    /*Stage 1 or 2*/
    auto_command(curr_board,count,command);
   
    free(curr_board);
    curr_board=NULL;
    
    return EXIT_SUCCESS;            // exit program with the success code
}
/**************************************************************************/
/*Function creates the initial checker board*/
board_t
*make_board(void) {
	board_t *board;
	int i,j;
	
	board = (board_t*)malloc(sizeof(board_t));
	assert(board);
	
	for (i=0;i<BOARD_SIZE;i++) {
		for (j=0;j<BOARD_SIZE;j++) {
			/*Rows containing white pieces*/
			if (i<ROWS_WITH_PIECES) {
				/*White pieces shifted to the right*/
				if (!(i%ROWS_SHIFTED_RIGHT)&&(j%ROWS_SHIFTED_RIGHT)) {
					(board->cboard)[i][j]=CELL_WPIECE;
				/*White pieces shifted to the left*/
				}else if ((i%ROWS_SHIFTED_RIGHT)&&!(j%ROWS_SHIFTED_RIGHT)) {
					(board->cboard)[i][j]=CELL_WPIECE;
				/*Empty regions in the white rows*/	
				}else {
					(board->cboard)[i][j]=CELL_EMPTY;
				}
			/*Rows containing black pieces*/
			} else if (i>LAST_EMPTY_ROW) {
				/*Black pieces shifted to the right*/
				if (!(i%ROWS_SHIFTED_RIGHT)&&(j%ROWS_SHIFTED_RIGHT)) {
					(board->cboard)[i][j]=CELL_BPIECE;
				/*Black pieces shifted to the left*/
				}else if ((i%ROWS_SHIFTED_RIGHT)&&!(j%ROWS_SHIFTED_RIGHT)) {
					(board->cboard)[i][j]=CELL_BPIECE;
				/*Empty regions in the white rows*/	
				}else {
					(board->cboard)[i][j]=CELL_EMPTY;
				}
			}else {
				(board->cboard)[i][j]=CELL_EMPTY;
			}
		}
	}
	cost_counter(board);
	return board;
}
/**************************************************************************/
/*Follow the directions given by the textfile and store the final
  character in the main stack*/
int
actions(board_t *board,char *x1) {
	int count=0;
	int y1,y2;
	char x2;
	char playerturn;
	char moved_piece;
	
	
	while(scanf("%c%d-%c%d\n",x1,&y1,&x2,&y2)==4) {
		moved_piece=(board->cboard)[y1-1][(int)(*x1-COL1)];
		playerturn=turn(count);
	    
		error_check(board,*x1,y1,x2,y2,playerturn);
		count++;
	    /*Since the move is valid, time to make changes to the board*/
	    if (abs((int)(x2-*x1))==1) {
	    	/*Simple forward move*/
	    	
	    	(board->cboard)[y2-1][(int)(x2-COL1)]=(board->cboard)[y1-1]
	    	                                     [(int)(*x1-COL1)];
	    	(board->cboard)[y1-1][(int)(*x1-COL1)]=CELL_EMPTY;
	    	/*No need to update cost, because no captures were made*/
	    	if (islower(moved_piece)) {
	    	    promote_pieces(board,playerturn);
	    	    cost_counter(board);
	        }
	    /*Capture*/
	    }else {
	    	/*Remove middle piece from the board*/
	    	(board->cboard)[(y1+y2-2)/2][(int)((*x1+x2)/2)-COL1]=CELL_EMPTY;
	        (board->cboard)[y2-1][(int)(x2-COL1)]=(board->cboard)[y1-1]
	                                             [(int)(*x1-COL1)];
	    	(board->cboard)[y1-1][(int)(*x1-COL1)]=CELL_EMPTY;
	    	/*Need to update costs*/
	    	if (islower(moved_piece)) {
	    	    promote_pieces(board,playerturn);
	        }
	    	cost_counter(board);
	    }
	    disp_action(board,*x1,y1,x2,y2,count);
    }
    playerturn=turn(count);
    end_game(board,playerturn);
      
    /*Last action number*/
    
    return count;
}

/*************************************************************************/
/*Determine the number of valid moves a player can make*/
int
nactions(board_t *board, char playerturn) {
	int i,j,k;
	int naction=0;
	char curr_cell;
	
	for (i=0;i<BOARD_SIZE;i++) {
		for (j=0;j<BOARD_SIZE;j++) {
			curr_cell = (board->cboard)[i][j];
			if (curr_cell==CELL_EMPTY) {
				continue;
		    }else if (toupper(curr_cell)!=playerturn) {
				continue;
			}
			/*NE SE SW NW*/
			for (k=1;k<=4;k++) {
				naction+=(valid_action(board,i+1,j+1,k)>0);		
			}
		}
	}
	return naction;
}
/*************************************************************************/
/*Returns 1 if it is a valid move or 2 if there is a valid capture*/
int
valid_action(board_t *board,int row,int col,int dir) {
	char curr_cell,target,playerturn;
	
	curr_cell = (board->cboard)[row-1][col-1];
	playerturn = toupper(curr_cell);
		
	/*North-East*/
	if (dir==NE) {
		/*White pieces can't move in that direction*/
		if (curr_cell==CELL_WPIECE) {
			return 0;
		}
		/*Can't move NE if the object is next to the border*/
		if (row==1 || col==BOARD_SIZE) {
			return 0;
		}
		
	    /*Move 1 unit*/
	    target = (board->cboard)[row-2][col];
	    /*Normal move possible*/
        if (target==CELL_EMPTY) {
        	return 1;
        /*Capture?*/
        }else {
        	if (row==2 || col==BOARD_SIZE-1) {
		        return 0;
		    }
	        if (toupper(target)!=playerturn) {
	        	/*Must make sure there is a safe place to land*/
		       	if ((board->cboard)[row-3][col+1]==CELL_EMPTY) {
	        		return 2;
	        	}
	        }
	    }
	    
	    
	    
	}else if (dir==NW) {
		/*White pieces can't move in that direction*/
		if (curr_cell==CELL_WPIECE) {
			return 0;
		}
		/*Can't move i unit NW, if the object is next to the border*/
		if (row==1 || col==1) {
			return 0;
		}
		
	    /*Move 1 unit*/
	    target = (board->cboard)[row-2][col-2];
	    /*Normal move possible*/
        if (target==CELL_EMPTY) {
        	return 1;
        /*Capture?*/
        }else {
        	if (row==2 || col==2) {
		        return 0;
		    }
	        if (toupper(target)!=playerturn) {
	        	/*Must make sure there is a safe place to land*/
		       	if ((board->cboard)[row-3][col-3]==CELL_EMPTY) {
	        		return 2;
	        	}
	        }
	    }
	    
	    
	    
	    
	}else if (dir==SE) {
		/*Black pieces can't move in that direction*/
		if (curr_cell==CELL_BPIECE) {
			return 0;
		}
		/*Can't move 1 unit SE, if the object is next to the border*/
		if (row==BOARD_SIZE || col==BOARD_SIZE) {
			return 0;
		}
		
	    /*Move 1 unit*/
	    target = (board->cboard)[row][col];
	    /*Normal move possible*/
        if (target==CELL_EMPTY) {
        	return 1;
        /*Capture?*/
        }else {
        	if (row==BOARD_SIZE-1 || col==BOARD_SIZE-1) {
		        return 0;
		    }
	        if (toupper(target)!=playerturn) {
	        	/*Must make sure there is a safe place to land*/
		       	if ((board->cboard)[row+1][col+1]==CELL_EMPTY) {
	        		return 2;
	        	}
	        }
	    }
	}else if (dir==SW) {
		/*Black pieces can't move in that direction*/
		if (curr_cell==CELL_BPIECE) {
			return 0;
		}
		/*Can't move 1 unit SW, if the object is next to the border*/
		if (row==BOARD_SIZE || col==1) {
			return 0;
		}
		
	    /*Move 1 unit*/
	    target = (board->cboard)[row][col-2];
	    /*Normal move possible*/
        if (target==CELL_EMPTY) {
        	return 1;
        /*Capture?*/
        }else {
        	if (row==BOARD_SIZE-1 || col==2) {
		        return 0;
		    }
	        if (toupper(target)!=playerturn) {
	        	/*Must make sure there is a safe place to land*/
		       	if ((board->cboard)[row-3][col+1]==CELL_EMPTY) {
	        		return 2;
	        	}
	        }
	    }
	    
    }  
		
	return 0;
}
	
/************************************************************************/
/*Uses the last character in the textfile to instruct what the AI should do*/ 
void
auto_command(board_t *board,int count,char command) {
	int i;
    
	if (command==STAGE1) {
		board=auto_move(board,count);
		
	}else if (command==STAGE2) {
		for (i=0;i<COMP_ACTIONS;i++) {
			/*Repeat the solution to stage1 10 times*/
			board=auto_move(board,count);
			count++;
		}
	}else {
		exit(EXIT_FAILURE);
	}
		
	return;
}
/*************************************************************************/
/*Solution to stage 1*/
board_t
*auto_move(board_t *board,int count) {
	int y1,y2;
    int costfinal;
    char playerturn=turn(count);
    char x1,x2;
    node_t *tree;
    board_t *final;
    
    /*creates a stem for the tree*/
    tree=create_tree(board,count);
    
    /*Depth 1*/
    fill_node(tree);
    /*Depth 2*/
    fill2_node(tree);
    /*Depth 3*/
    fill3_node(tree);
    
    /*Get the cost of the child node we are interested in*/
    costfinal=minimax(tree,TREE_DEPTH,playerturn);
    
    /*If no moves can be made*/
    if (costfinal==INT_MAX) {
    	printf("\nBLACK WIN!\n");
    	exit(EXIT_SUCCESS);
    }else if (costfinal==INT_MAX) {
    	printf("\nWHITE WIN!\n");
    	exit(EXIT_SUCCESS);
    }
    
    /*Find the branch that child belongs to*/
    final=searchboard(tree,TREE_DEPTH,costfinal);
    
    free(tree->nodes);
    free(tree);
    tree=NULL;
    
    /*Determine the move made by the AI*/     
    movecal(board,final,count,&x1,&y1,&x2,&y2);
    
    /*Display the new board*/
    disp_auto(final,count,x1,y1,x2,y2);
    
    free(board);
    board=NULL;
    return final;
}
/************************************************************************/
/*Function determines the move made by the AI*/
void
movecal(board_t *orig,board_t *final,int count,char *x1,int *y1,char *x2,
	                                                                int *y2) {
	int i,j;
	char player;
	
	
	player=turn(count);
	
	for (i=0;i<BOARD_SIZE;i++) {
		for (j=0;j<BOARD_SIZE;j++) {
			
			/*SOURCE CELL*/
			if ((isalpha((orig->cboard)[i][j]))&&
				                    (toupper((orig->cboard)[i][j])==player)) {
			    /*if the original board has a piece that the final board
			      doesn't have*/
				if ((final->cboard)[i][j]==CELL_EMPTY) {
					*x1=COL1+j;
					*y1=i+1;
				}
			}
			
			/*Target cell*/
            if ((isalpha(final->cboard[i][j])&&
            	                    toupper((final->cboard)[i][j])==player)) {
                  /*if the final board has a piece that the original board
			      doesn't have*/
				if ((orig->cboard)[i][j]==CELL_EMPTY) {
					*x2=COL1+j;
					*y2=i+1;
				}
			}
		}
	}
	return;
}


/*************************************************************************/
/*Diplays the AI board*/
void
disp_auto(board_t *board,int count,char x1,int y1,char x2,int y2) {
    printf("%s",DIVIDER2);
	printf("*** ");
	
	if (turn(count)==BLACK) {
		printf("BLACK ACTION #%d: %c%d-%c%d\n",count+1,x1,y1,x2,y2);
	}else {
		printf("WHITE ACTION #%d: %c%d-%c%d\n",count+1,x1,y1,x2,y2);
    }
    printf("BOARD COST: %d\n",(board->cost));
    disp_board(board);
	return;
}
/*************************************************************************/
/*Searches for the branch that contains the child node we are interested in*/
board_t
*searchboard(node_t *tree,int depth,int costfinal) {
	int i,j,k;
	node_t *branch;
	node_t *minibranch;
	node_t *stick;
	
	/*main tree*/
	for (i=0;i<tree->nnodes;i++) {
		/*Layer1*/
		branch=(tree->nodes)[i];
		if (branch->nnodes==0) {
			if (branch->pot_board->cost==costfinal) {
				return branch->pot_board;
			}
			
			free(branch->pot_board);
			free(branch);
		}else {
			for (j=0;j<branch->nnodes;j++) {
	            /*Layer2*/
				minibranch=(branch->nodes)[j];
				if (minibranch->nnodes==0) {
			        if (minibranch->pot_board->cost==costfinal) {
		    	    	return branch->pot_board;
		        	}
		        	
		        	free(minibranch->pot_board);
		        	free(minibranch);
		        	
			    }else {
			    	for (k=0;k<minibranch->nnodes;k++) {
			    		/*Layer3*/
			    		stick=(minibranch->nodes)[k];
			    		if (stick->pot_board->cost==costfinal) {
			    			return branch->pot_board;
			    		}
			    		free(stick->pot_board);
			    		free(stick);
			    	}
			    	free(minibranch->nodes);
			    	free(minibranch->pot_board);
			    	free(minibranch);
			    }
			}
		
		    free(branch->nodes);
        	free(branch->pot_board);
        	free(branch);
        }
    }
    exit(EXIT_FAILURE);
}
	
	

/*************************************************************************/
/*Adapted from https://www.youtube.com/watch?v=l-hh51ncgDI*/
int
minimax(node_t *branch,int depth,char player) {
	int i;
	int eval,minEval,maxEval;
	
	/*Reached the end of the tree, return the cost of the board*/
	if (depth==0 || branch->nnodes==0) {
		return branch->pot_board->cost;
	}
	
	/*Black wants to maximize*/
	if (player == BLACK) {
		maxEval=INT_MIN;
		for (i=0;i<branch->nnodes;i++) {
			eval=minimax((branch->nodes)[i],depth-1,WHITE);
			
			maxEval=maxcal(maxEval,eval);		
		
	    }
	    return maxEval;
	/*White wants to minimize*/
	}else {
		minEval=INT_MAX;
		for (i=0;i<branch->nnodes;i++) {
	        eval=minimax((branch->nodes)[i],depth-1,BLACK);
			minEval=mincal(minEval,eval);
	        
	    }
	    return minEval;
	}
	   
}
/*************************************************************************/
/*Return the maximum cost*/
int
maxcal(int currmax,int curr) {
	if (currmax>=curr) {
		return currmax;
	}else {
		return curr;
	}
}
/*************************************************************************/
/*Return the minimum cost*/
int
mincal(int currmin,int curr) {
	if (currmin<=curr) {
		return currmin;
	}else {
		return curr;
	}
}
/*************************************************************************/
/*Fills the third layer of a tree structure*/
void
fill3_node(node_t *tree) {
	int i;
	node_t *node1;
	
	for (i=0;i<tree->nnodes;i++) {
		
		node1=tree->nodes[i];
		fill2_node(node1);
	}
    return;
}
/*************************************************************************/
/*Fills the second layer of a tree structure*/
void
fill2_node(node_t *tree) {
	int i;
	node_t *node;
	
	for (i=0;i<(tree->nnodes);i++) {
		
		node=(tree->nodes)[i];
		fill_node(node);
	}
	
	return;
}
/*************************************************************************/
/*Fills the first layer of a tree structure*/
void
fill_node(node_t *node) {
	int row,col,dir,n=0;
	int flag=0;
	int count=node->nummoves;
	char playerturn;
	char childplayer;
	int nchildmoves;
	board_t *board=node->pot_board;	
	node_t *child;
	
	
	playerturn=turn(count);
	
	/*Goes through an array of pointers to nodes*/                                
    for (row=1;(row<=BOARD_SIZE)&&(n<node->nnodes);row++) {
        
	    for (col=1;(col<=BOARD_SIZE)&&(n<node->nnodes);col++) {
			
	    	/*Ignore empty cells*/
		    if (((board->cboard)[row-1][col-1])==CELL_EMPTY) {
			    continue;	
                
			/*Ignore pieces that belong to the opponent*/    
		    }else if ((toupper((board->cboard)[row-1][col-1]))!=playerturn) {
			    continue;
        	}
			
        	/*For each pieces look around in a clockwise direction*/
   	    	for (dir=NE;(dir<=NW)&&(n<node->nnodes);dir++) {
   		    	flag=0;
    	        flag=valid_action(node->pot_board,row,col,dir); 
    	      
    	        /*If there are no valid moves look at another piece*/
        	    if (!flag) {
        	    	continue;
        	    }
	    	    child = new_node(); 
	    		child->nummoves=count+1; 
	    	   	copy_board(node,child);
			    	
	    	   	/*Valid: North-East move*/
    		   	if (flag==1&&dir==NE) {
		      		child->pot_board->cboard[row-2][col]=
		        	child->pot_board->cboard[row-1][col-1];
					
    		    	child->pot_board->cboard[row-1][col-1]=CELL_EMPTY;
				/*Valid: North-East capture*/	
    		   	}else if (flag==2&&dir==NE) {
	    	    	child->pot_board->cboard[row-3][col+1]=
	    	    	child->pot_board->cboard[row-1][col-1];
			
	    		    child->pot_board->cboard[row-2][col]=CELL_EMPTY;
	        		child->pot_board->cboard[row-1][col-1]=CELL_EMPTY;
			    /*Valid: South-East move*/
	        	}else if (flag==1&&dir==SE) {
		      		child->pot_board->cboard[row][col]=
		     		child->pot_board->cboard[row-1][col-1];
                    
		        	child->pot_board->cboard[row-1][col-1]=CELL_EMPTY;
			    /*Valid: South-East capture*/
	            }else if (flag==2&&dir==SE) {
	    	       	child->pot_board->cboard[row+1][col+1]=
		     		child->pot_board->cboard[row-1][col-1];
				
		        	child->pot_board->cboard[row][col]=CELL_EMPTY;
		        	child->pot_board->cboard[row-1][col-1]=CELL_EMPTY;
				/*Valid: South-West move*/
	    	    }else if (flag==1&&dir==SW) {
		        	child->pot_board->cboard[row][col-2]=
		        	child->pot_board->cboard[row-1][col-1];
				
	    		   	child->pot_board->cboard[row-1][col-1]=CELL_EMPTY;
				/*Valid: South-West capture*/
		        }else if (flag==2&&dir==SW) {
		         	child->pot_board->cboard[row+1][col-3]=
		        	child->pot_board->cboard[row-1][col-1];
				
		         	child->pot_board->cboard[row][col-2]=CELL_EMPTY;
		         	child->pot_board->cboard[row-1][col-1]=CELL_EMPTY;
				/*Valid: North-West move*/
	     	   	}else if (flag==1&&dir==NW) {
		     		child->pot_board->cboard[row-2][col-2]=
		     		child->pot_board->cboard[row-1][col-1];
				
		        	child->pot_board->cboard[row-1][col-1]=CELL_EMPTY;
				/*Valid: North-West capture*/
	    	    }else if (flag==2&&dir==NW) {
		         	child->pot_board->cboard[row-3][col-3]=
		     		child->pot_board->cboard[row-1][col-1];
				
	    	    	child->pot_board->cboard[row-2][col-2]=CELL_EMPTY;
		     		child->pot_board->cboard[row-1][col-1]=CELL_EMPTY;
	            }
	           
	            promote_pieces(child->pot_board,playerturn);               
                childplayer=turn(count+1);
                /*Update the cost of the board*/
                cost_counter2(child->pot_board,childplayer);
                /*attach the child node to the array of pointers*/
		        (node->nodes)[n++]=child;
                /*Update count*/
                child->nummoves=count+1;
		        nchildmoves=nactions(child->pot_board,childplayer);
		        /*Insert the number of nodes a child node has in the struct*/
                child->nnodes=nchildmoves;
                /*create an array of pointers to child nodes*/
		        (child->nodes)=empty_array(nchildmoves);
		        child=NULL;
		            
		        
			}
		}
	}				        
	
	return;
}

/*************************************************************************/
/*Counts the board cost using a different method. Used AI system*/
void
cost_counter2(board_t *board,char player) {
	
	if (nactions(board,player)==0) {
		if (player==BMOVE) {
			board->cost=INT_MIN;
			return;
		}else {
			board->cost=INT_MAX;
			return;
		}
	}
	cost_counter(board);
    return;
}
/*************************************************************************/
/*Creates a new node using malloc*/
node_t
*new_node(void) {
	node_t *child;
	child = (node_t*)malloc(sizeof(node_t));
	assert(child);
    child->nodes=NULL;
    return child;
}
/*************************************************************************/
/*Creates an array of the correct size in order to hold pointers to nodes*/
node_t
**empty_array(int nmoves) {
	int i;
	node_t **array;
	
	array=(node_t**)malloc(sizeof(node_t*)*nmoves); 
    assert(array);

    for (i=0;i<nmoves;i++) {
    	array[i]=NULL;
    }
    
    return array;
}
/*************************************************************************/
/*Creates the stem of the tree structure. It conatins a pointer to the inital
board*/
node_t
*create_tree(board_t *board, int count) {
    int nmoves;
    char playerturn;
    node_t *tree;
    
    playerturn=turn(count);
    nmoves=nactions(board,playerturn);
    tree=(node_t*)malloc(sizeof(node_t));
    assert(tree);
    
    /*Fill in the stem node*/
    tree->nnodes=nmoves;
    tree->pot_board=board;
    cost_counter(tree->pot_board);
    tree->nummoves=count;
    (tree->nodes)=empty_array(nmoves);
    
    return tree;
}
/*************************************************************************/
/*Copies the pieces/towers from one board and pastes them onto another board*/
void
copy_board(node_t *orig, node_t *copy) {
	int i,j;
	board_t *tmpboard;
  
	tmpboard=(board_t*)malloc(sizeof(board_t));
	assert(tmpboard);
	copy->pot_board=tmpboard;
	for (i=0;i<BOARD_SIZE;i++) {
		for (j=0;j<BOARD_SIZE;j++) {
			(copy->pot_board->cboard)[i][j]=(orig->pot_board->cboard)[i][j];
		}
	}
	cost_counter(copy->pot_board);
	return;
}
/*************************************************************************/
/*Determines whether a piece needs to be promoted to a tower*/
void
promote_pieces(board_t *board,char playerturn) {
	int i;
	
	/*See if you can promote a black piece*/
	if (playerturn==BLACK) {
	    for (i=1;i<BOARD_SIZE;i+=2) {
		    if ((board->cboard)[0][i]==CELL_BPIECE) {
			    (board->cboard)[0][i]=CELL_BTOWER;
		    }
	    }
	/*See if you can promote a white piece*/
	}else {
		for (i=0;i<BOARD_SIZE;i+=2) {
		    if ((board->cboard)[0][i]==CELL_WPIECE) {
			    (board->cboard)[0][i]=CELL_WTOWER;
		    }
	    }
	}
	
	return;
}
/**************************************************************************/
/*Determine whether a player has won the game*/
void
end_game(board_t *board,char playerturn) {
	int flag=0;
	flag=nactions(board,playerturn);
    
	if (!flag&&playerturn==BLACK) {
		printf("WHITE WIN!\n");
		exit(EXIT_SUCCESS);
	}else if (!flag&&playerturn==WHITE) {
		printf("BLACK WIN!\n");
		exit(EXIT_SUCCESS);
	}
    
	return;
}

/**************************************************************************/
/*Returns a character which indicates who's turn it is*/
char
turn(int count) {
	char playerturn;
	if (count%PLAYERMOVE==BMOVE) {
		playerturn = BLACK;
	}else {
		playerturn = WHITE;
    }
    return playerturn;
}
/**************************************************************************/	
/*Display the directions that were given in the textfile*/
void
disp_action(board_t *board,char x1,int y1,char x2,int y2,int count) {
	printf("%s",DIVIDER2);
	if ((count-1)%PLAYERMOVE==BMOVE) {
		printf("BLACK ACTION #%d: %c%d-%c%d\n",count,x1,y1,x2,y2);
	}else {
		printf("WHITE ACTION #%d: %c%d-%c%d\n",count,x1,y1,x2,y2);
    }
    printf("BOARD COST: %d\n",board->cost);
    disp_board(board);
    return;
}
/**************************************************************************/
/*Updates the board cost. This function is used in stage 0*/
void
cost_counter(board_t *board) {
	int i,j;
	int nb=0,nw=0,nB=0,nW=0;
	for (i=0;i<BOARD_SIZE;i++) {
		for (j=0;j<BOARD_SIZE;j++) {
			nb+=((board->cboard)[i][j]==CELL_BPIECE);
			nB+=((board->cboard)[i][j]==CELL_BTOWER);
			nw+=((board->cboard)[i][j]==CELL_WPIECE);
			nW+=((board->cboard)[i][j]==CELL_WTOWER);
	    }
	}
	board->cost=nb+COST_TOWER*nB-nw-COST_TOWER*nW;
    return;
}
/**************************************************************************/
/*Reports the size of the board and the number of coloured pieces in the game*/
void
disp_initial(board_t *board) {
	printf("%s",INITIALTXT1);
	printf("%s",INITIALTXT2);
	printf("%s",INITIALTXT3);
	/*Shift the work to another friend*/
	disp_board(board);
    return;
}
/**************************************************************************/
/*Displays the current checkers board*/
void
disp_board(board_t *board) {
	int i,j;
	
	printf("     %s",COLUMNS);
	printf("   %s",DIVIDER);
	for (i=0;i<BOARD_SIZE;i++) {
		printf(" %d ",i+1);
		printf("|");
		for (j=0;j<BOARD_SIZE;j++) {
			printf(" %c |",board->cboard[i][j]);
		}
		printf("\n   %s",DIVIDER);
	}
	return;
}
/**************************************************************************/
/*Make sure that the move made is valid*/
void
error_check(board_t *board,char x1,int y1,char x2,int y2,char player) {
	char spiece,tpiece,pcaptured;
	
	/*Check invalid cells*/
	if (!(x1>=COL1 && x1<=COL8) || !(y1>=1 && y1<=BOARD_SIZE)) {
		printf("%s",ERRORCODE1);
		exit(EXIT_FAILURE);
	}else if (!(x2>=COL1 && x2<=COL8) || !(y2>=1 && y2<=BOARD_SIZE)) {
        printf("%s",ERRORCODE2);
        exit(EXIT_FAILURE);
    }
    
    /*Search for the source cell*/
    spiece = (board->cboard)[y1-1][(int)(x1-COL1)];
    tpiece = (board->cboard)[y2-1][(int)(x2-COL1)];
    
    /*Source cell is empty*/
    if (spiece==CELL_EMPTY) {
    	printf("%s",ERRORCODE3);
    	exit(EXIT_FAILURE);
    }
    /*Target cell is empty*/
    if (tpiece!=CELL_EMPTY) {
    	printf("%s",ERRORCODE4);
    	exit(EXIT_FAILURE);
    }
    /*Piece/tower doesn't belong to the player*/
    if (toupper(spiece)!=player) {
    	printf("%s",ERRORCODE5);
    	exit(EXIT_FAILURE);
    	
    	
    }else {
    	/*The player is moving a piece*/
    	if (islower(spiece)) {
    		/*Pieces can't go backwards or sideways*/
    		if ((player==BLACK && y2-y1>=0)||(player==WHITE && y2-y1<=0)){
    		    printf("%s",ERRORCODE6);
    		    exit(EXIT_FAILURE);
    		}
    		
    		
    	}else {
    		/*Towers can't move sideways*/
    		if ((player==BMOVE && y2-y1==0)||(player==WMOVE && y2-y1==0)){
    		    printf("%s",ERRORCODE6);
    		    exit(EXIT_FAILURE);
    		}
    		
    		
    	}
    }
 	/*Did the piece make a valid capture */
    if (abs((int)(x2-x1))==2) {
    	/*Capture is not valid given we know it doesn't go backwards*/
    	if (abs(y2-y1)!=2) {
    		printf("%s",ERRORCODE6);
    		exit(EXIT_FAILURE);
    	}else {
    		/*Movement is correct, but is the opponent's piece in the middle*/
    		pcaptured = (board->cboard)[(y2+y1-2)/2][(int)((x2+x1)/2)-COL1];
    		if (pcaptured==CELL_EMPTY) {
    			printf("%s",ERRORCODE6);
    		    exit(EXIT_FAILURE);
    		}else if (toupper(pcaptured)==player) {
    		    printf("%s",ERRORCODE6);
    		    exit(EXIT_FAILURE);
    		}
    	}
	/*Piece moves forward by one step*/
    }else if (abs((int)(x2-x1))==1) {
    	if (abs(y2-y1)!=1) {
  			printf("%s",ERRORCODE6);
    		exit(EXIT_FAILURE);
   		}
   		/*Since we checked whether the target cell is empty
   		the move is valid, so do nothing*/
   		
   	}else {
   		/*Moves to far horizontally*/
   		printf("%s",ERRORCODE6);
   		exit(EXIT_FAILURE);
   	}    			
        	
    return;
}
/**************************************************************************/
/*algorithms are fun*/



