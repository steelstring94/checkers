/*  Program Description:  Simple line-drawn command-line checkers game.

    Date:  April 14-17, 2014

    Author:  Steelstring94
*/
#include <iostream>
#include <limits>
using namespace std;

//drawHeader prints out the numbers and top border
//line of the checkers board.
void drawHeader();

//drawRow() takes the board array and turn number
//(1 for player 1's turn, 2 for player 2's) and
//draws one row of the checkers board, which consists
//of one line as the top border line of the
//row and the actual pieces and border lines
//between the pieces on the next line in the row itself.
void drawRow(int grid[][8], int turn);

//arrangeGrid() takes the array representing the
//checkers board as an argument and sets it up
//to represent the initial configuration of the
//checkers board.  This function is naturally
//called only once.
void arrangeGrid(int grid[][8]);

//printHelp() writes the welcome/help statement to
//std::cout, providing instructions on how to play
//the game.
void printHelp();

//getMove() takes four reference integers in the order
//of x-coordinate from which the player is moving,
//x-coordinate of destination, and y coordinates
//of source and destination.  The int parameter is
//the turn number, and the array is the array of
//the board.  getMove returns false if the player
//tries to select a location at which they have no
//piece, and true otherwise.
bool getMove(int &xFrom, int &xTo, int &yFrom, int &yTo, int turn, int grid[][8]);

//validMove() takes the same arguments as getMove()
//in the same order, but only the last is a reference
//parameter, and it is an argument of the number of
//pieces which the current player still has on the
//board (used to determine win and loss).  The function
//of validMove() is to return false if the player has
//attempted to make an illegal move and true otherwise.
//This function also has the responsibility of deleting
//jumped pieces when a valid jump move is made.
//Note, however, that it does NOT move the original piece
//making the jump - that is handled by main(), or, in the
//case of a double jump, grantDoubleJump().
bool validMove(int xFrom, int xTo, int yFrom, int yTo, int turn, int grid[][8], int &pieces);

//cls() clears the screen.
void cls();

//isDoubleJumpAvailable() returns true if,
//after a turn, a player has a double jump
//available to him.  First two parameters
//are x and y coordinates of the piece,
//and the final tells the function
//whether the piece is a King or not
//(because that affects the available
//directions of movement and, consequently,
//available double jumps).  The last parameter
//is an array which this function will populate
//to contain a registry of legitimate double jumps.
//This is done to prevent the player from taking
//advantage of the double jump system to abuse
//the opportunity for a double jump to make an
//otherwise legal move which nevertheless is not
//a jump (in other words, failing to jump on the
//double jump, and, instead, moving off in another
//direction without jumping).
bool isDoubleJumpAvailable(int x, int y, int turn, int grid[][8], int jumpReg[][2]);

//grantDoubleJump() takes the player whose turn
//it is, x and y coordinate of the location of the
//piece to which a double jump is to be granted, the
//game board, a reference to the pieces of the player
//whose piece will be jumped (which it must pass to
//validMove()), and the jump registry.  grantDoubleJump()
//as well as the game in general do not enforce that
//a player must jump if he has the opportunity.
void grantDoubleJump(int player, int &x, int &y, int grid[][8], int &pieces, int jumpReg[][2]);

//Called to draw the current game board
//to std::cout.
void drawBoard(int grid[][8]);


int main()
{
  /* Variable description:

     xFrom = x-coorindate of piece to be moved.
     xTo = x-coordinate of destination square.
     yFrom = y-coordinate of piece to be moved.
     yTo = y-coordinate of destination square.
     turn = Whose turn it currently is.

     p1Pieces = Number of pieces player 1 currently
                has on the board.
     p2Pieces = Number of pieces player 2 currently
                has on the board.

     grid = Array representation of the checkers board.
            8 rows, 8 columns.

	    Key:
	    	0 - Empty square
			1 - Player 1 standard piece
			2 - Player 2 standard piece
			3 - Player 1 King piece
			4 - Player 2 King piece

     jumpReg = Registry array used to log available
               double jumps to ensure the user does
	       not take advantage of the double jump
	       system to move his piece somewhere other
	       than into an open space for a double jump.
  */
  int xFrom, xTo, yFrom, yTo, turn = 1;
  int p1Pieces = 12, p2Pieces = 12;
  int grid[8][8];
  int jumpReg[4][2];

  //Set up the grid array to have the initial
  //configuration of a checkerboard.
  arrangeGrid(grid);

  //The help/welcome text is the first thing printed
  //to std::cout when a user loads the program.
  printHelp();

  //This while loop will run as long as both
  //players still have at least one piece
  //remaining on the board.
  while(p1Pieces > 0 && p2Pieces > 0)
    {

      //If it is player 1's turn, do the following
      //if block.
      if(turn == 1)
	{
	  //This variable is used to determine whether
	  //a piece was jumped in a given move.
	  //That is needed because we only check
	  //for available double jumps if the player
	  //made an initial jump to begin with.
	  bool jumped = false;

	  //Draw the game board.
	  drawBoard(grid);

	  //getMove() will return false if the player attempts
	  //to select a location from which to move at which they
	  //have no piece.  It will print an error message itself
	  //before doing so.
	  if(!getMove(xFrom, xTo, yFrom, yTo, turn, grid))
	     continue;

	  //The player may enter a zero at any prompt to
	  //exit the program.  If they do so, getMove will set
	  //the other variables to -1 (input - 1) so that all four
	  //equal -1.
	  if(xFrom == -1 && yFrom == -1 && xTo == -1 && yTo == -1)
	    {
	      cout << "\nExiting program.  Have a nice day!\n";

	      return 0;
	    }

	  //If xTo is grater than xFrom + 1 or less than
	  //xFrom - 1, it means they have jumped another
	  //piece.
	  if(xTo < xFrom - 1 || xTo > xFrom + 1)
	    jumped = true;


	  //validMove() returns false if the player attempts to make
	  //an illegal move and returns true otherwise.  Note that we
	  //pass validMove() the OPPOSITE player's pieces, because
	  //validMove() will decrement the piece count of a player
	  //whenever a move is a legal jump of another piece.
	  if(!validMove(xFrom, xTo, yFrom, yTo, turn, grid, p2Pieces))
	    {
	      //If it was not a valid move, then we set jumped
	      //back to false.
	      jumped = false;
	      cls();
	      cerr << "Invalid move!" << endl;
	      continue;
	    }

	  //If everything has been shown to be in order,
	  //remove the piece at the location from which
	  //the player has just moved.  Make the new piece a
	  //King if the old piece was a King (must be checked
	  //before deleting old piece).  Also give them a King
	  //if they have reached the opposite side of the board.

	  if(grid[xFrom][yFrom] == 3 || xTo == 7)
	    grid[xTo][yTo] = 3;
	  else
	    grid[xTo][yTo] = 1;

	  grid[xFrom][yFrom] = 0;

	  //If a piece was jumped, we check to see if there
	  //are any double jumps available to the player from the
	  //new location.  If that is the case, we grant them the
	  //opportunity to take advantage of that double jump.
	  if(jumped)
	    {
	      while(isDoubleJumpAvailable(xTo, yTo, turn, grid, jumpReg))
		grantDoubleJump(turn, xTo, yTo, grid, p2Pieces, jumpReg);

	    }

	  //It is now player 2's turn.
	  turn = 2;

	  //Clear the screen for the next board.
	  cls();

	}
      else
	{
   	  //Everything inside this else block is exactly the
	  //same as in the if block above, except that it works
	  //for player 2.

	  bool jumped = false;

	  drawBoard(grid);

	  if(!getMove(xFrom, xTo, yFrom, yTo, turn, grid))
	    continue;

	   if(xFrom == -1 && yFrom == -1 && xTo == -1 && yTo == -1)
	    {
	      cout << "\nExiting program.  Have a nice day!\n";

	      return 0;
	    }

	   if(xTo < xFrom - 1 || xTo > xFrom + 1)
	     jumped = true;

	   if(!validMove(xFrom, xTo, yFrom, yTo, turn, grid, p1Pieces))
	    {
	      jumped = false;
	      cls();
	      cerr << "Invalid move!";
	      continue;
	    }

	 if(grid[xFrom][yFrom] == 4 || xTo == 0)
	    grid[xTo][yTo] = 4;
	  else
	    grid[xTo][yTo] = 2;

	  grid[xFrom][yFrom] = 0;

	  if(jumped)
	    {
	      while(isDoubleJumpAvailable(xTo, yTo, turn, grid, jumpReg))
		grantDoubleJump(turn, xTo, yTo, grid, p1Pieces, jumpReg);
	    }

	  turn = 1;

	  cls();
	}
    }

  //Newline between last board and congratulatory
  //message.
  cout << endl;

  //If the while loop has broken, we know at least
  //one player has 0 pieces.  If it is not player 1,
  //then he is the winner.  Otherwise, it must be player 2.

  drawBoard(grid);

  if(p1Pieces > 0)
    cout << "Congratulations, Player 1!  You win!" << endl;
  else
    cout << "Congratulations, Player 2!  You win!" << endl;

  return 0;
}

void drawHeader()
{
  //Print the top column numbers as well
  //as a new line (on which to print the
  //top border line).
  cout << "\n   1   2   3   4   5   6   7   8" << endl;

  //Draw the top border line of the board
  //(testing showed that 19 lines looked
  //best).
 for(int i = 0; i < 36; i++)
   cout << '-';

 cout << endl;
}

void drawRow(int grid[][8], int row)
{
  //The row given to drawRow() for its
  //row variable will be a value from 0
  //to 7, because arrays are zero-indexed.
  //However, we should add one to the value of
  //the row which we write out, so that they
  //are numbered 1-8, to look more user-friendly.
  //Since we otherwise need the 0-7 row value,
  //we create a new variable to hold the output
  //row value.
  int outRow = row + 1;


  //Since drawRow() is run from within a for
  //loop, the presence of this for loop may
  //be considered to constitute a nested
  //for loop.

  //The outer loop here is used to draw each
  //row three times, to enlarge the board and
  //make it more easily visible.

  for(int i = 0; i < 3; i++)
    {
      //Draw the row number to output,
      //and the leftmost border line of the row.
      //We use just a space instead if we're not writing
      //a number so that all spaces are even.
      if(i == 1)
	cout << outRow << '|';
      else
	cout << " |";

      for(int j = 0; j < 8; j++)
	{
	  //If the place indicated by
	  //the row given from the calling function and
	  //the column of the current iteration of this for
	  //loop is empty, output a blank line in that place, as
	  //well as its right border line.
	  if(grid[row][j] == 0)
	    cout << "   |";


	  //If that location in the grid array is a 1,
	  //it contains a standard piece from player 1,
	  //so output a lower-case x.
	  else if(grid[row][j] == 1)
	    cout << "xxx|";

	  //A 2 indicates a standard piece from player 2,
	  //represented by a lower-case o.
	  else if(grid[row][j] == 2)
	    cout << "ooo|";

	  //3 stands for a player 1 King, so output a
	  //capital X.
	  else if(grid[row][j] == 3)
	    cout << "XXX|";

	  //4 is a player 2 King, represented by
	  //a capital O.
	  else if(grid[row][j] == 4)
	    cout << "OOO|";


	}

      //Print the row at the end of each
      //middle line, otherwise just return.
      if(i == 1)
	cout << outRow << endl;
      else if(i == 2)
	{
	  cout << endl;

	  //Drawing the bottom line of every row.
	  for(int k = 0; k < 12; k++)
	    {
	      cout << "---";
	    }
	  cout << endl;
	}
      else
	cout << endl;
    }

  //Print the row number at the end
  //of the row.

  //If we are on the final row of the
  //board, draw the bottom column
  //enumeration.
  if(row == 7)
    {
      cout << "   1   2   3   4   5   6   7   8\n";
    }
}

//arrangeGrid() is called only once.
void arrangeGrid(int grid[][8])
{
  //The following for loops iterate through
  //the grid array and initialize all values
  //to 0.
    for(int i = 0; i < 8; i++)
    {
      for(int k = 0; k < 8; k++)
	{
	  grid[i][k] = 0;
	}
    }

  for(int i = 0; i < 8; i++)
    {
      //Following if statements read:
      //If this is row one or three...
      if(i == 0 || i == 2)
	{
	  //...begin on the second square and
	  //mark every second square with a standard
	  //piece for player one.
	  for(int k = 1; k < 8; k+=2)
	    {
	      grid[i][k] = 1;
	    }
	}
      //If this is row two...
      else if(i == 1)
	{
	  //...start on the first square and
	  //put a player 1 piece on every second
	  //square.
	  for(int k = 0; k < 8; k +=2)
	    {
	    grid[i][k] = 1;
	    }
	}

      //If it's row 6 or 8...
      else if(i == 5 || i == 7)
	{
	  //...start on the first square and
	  //make it a player 2 standard piece.
	  for(int k = 0; k < 8; k+=2)
	    {
	      grid[i][k] = 2;
	    }
	}
      //For row 7...
	  else if (i == 6)
	    {
	      //...start at the second square.
	      for(int k = 1; k < 8; k+=2)
		{
		  grid[i][k] = 2;
		}
	    }

      //Otherwise, don't print any pieces
      //on this row.
	  else
	    continue;
    }
}

void printHelp()
{
  //Simple help/welcome text.
  cls();
  cout << "\n                     Welcome to CLI Checkers v3.0!\n";
  cout << "*********************************************************************\n";
  cout << "\n\nPlayer 1's pieces are represented"
       << " as x, while player 2's\nare represented"
       << " as o.  X or O (capital letter)"
       << " denotes\na King.";

  cout << "\n\nTo move a piece, enter the row first"
       << " and the column second\n(separated by a space)"
       << " when prompted!";

  cout << "\n\nTo quit, enter a zero for any move"
       << " prompt (except double jumps).";

  cout << "\n\nFor double jumps, you only need to enter"
       << " the destination - you don't\nhave to select the piece!";

  cout << "\n\nDon't enter anything but numbers for any prompt, or the"
       << "\nprogram will yell at you!";

  cout << "\n\nGood luck!\n";
  cout << "**********************************************************************\n";
  cout << "Press <enter> to begin";

  //Requiring them to press enter, and then clearing
  //the help text off the screen.
  cin.get();
  cls();
}

bool getMove(int &xFrom, int &xTo, int &yFrom, int &yTo, int turn, int grid[][8])
{
  //Requesting the x and y coordinates from the player whose
  //turn it is of the piece they wish to move.
  cout << "Player " << turn << ", enter piece to move: ";

  //Read in the x coordinate of the origin
  //piece.  The use of the while loop in this and
  //other inputs ensures that the user may not enter
  //a character instead of an integer.
  while(!(cin >> xFrom))
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cls();
      drawBoard(grid);
      cerr << "ENTER NUMBERS ONLY!";
      cout << "\nPlayer " << turn << ", enter piece to move: ";
    }

  //If they enter even one zero for any prompt,
  //we set all values to -1 and return to main(),
  //which will result in a program exit.
  if(xFrom == 0)
    {
      xFrom = -1;
      xTo = -1;
      yFrom = -1;
      yTo = -1;
      return true;
    }

  //Next while and if statements are just like
  //above, only for the y coordinate.
  while(!(cin >> yFrom))
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cls();
      drawBoard(grid);
      cerr << "ENTER NUMBERS ONLY!\n";
      cout << "Player " << turn << ", enter piece to move: ";
    }

  if(yFrom == 0)
    {
      xFrom = -1;
      xTo = -1;
      yFrom = -1;
      yTo = -1;
      return true;
    }

  //Ensure that xFrom and yFrom are within the proper range.
  if(xFrom < 0 || xFrom > 8 || yFrom < 0 || yFrom > 8)
    {
      cls();
      cerr << "Invalid input!" << endl;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      return false;
    }

  //The player will enter, for instance, 2 1, to indicate row 2, column 1.
  //For us, that is really 1 0, because it must be converted to the array's
  //zero-indexed format.  So, the location of the player's source x-coordinate,
  //for example, is always xFrom - 1.  The destination x-coordinate is xTo - 1.
  //So we format the input to be that way from the start.

  xFrom--;
  yFrom--;

  //Turn will always be either 1 or 2.  1 or 2 represent standard pieces for each
  //player, and 1 + 2 is 3, representing a Player 1 King.  2 + 2 is 4, representing
  //a player 2 King.  Therefore, (turn || turn + 2) will always represent a piece of
  //any type from the player whose turn it currently is.

  //All that being true, the follwoing if statement reads:  If the source square for
  //the player's move does not contain a standard piece belonging to that player, and
  //it also does not contain a King piece belonging to that player, then
  //output an error message stating that they have no piece at those coordinates.
  if(grid[xFrom][yFrom] != turn && grid[xFrom][yFrom] != turn + 2)
    {
      cls();
      //Must add 1 to xFrom and yFrom when outputting to make it
      //look right from the player's perspective.
      cerr << "You have no piece at " << xFrom+1 << ' ' << yFrom+1
	   << '!' << endl;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return false;
    }


  //Read in the x and y coordinate, in that order,
  //of the desired destination.
  cout << "Enter destination: ";

  while(!(cin >> xTo))
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cls();
      drawBoard(grid);
      cerr << "ENTER NUMBERS ONLY!\n";
      cout << "Player " << turn << ", enter destination ";
    }

  if(xTo == 0)
    {
      xFrom = -1;
      xTo = -1;
      yFrom = -1;
      yTo = -1;
      return true;
    }


  while(!(cin >> yTo))
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cls();
      drawBoard(grid);
      cerr << "ENTER NUMBERS ONLY!\n";
      cout << "Player " << turn << ", enter destination: ";
    }

  if(yTo == 0)
    {
      xFrom = -1;
      xTo = -1;
      yFrom = -1;
      yTo = -1;
      return true;
    }

  if(xTo < 0 || xTo > 8 || yTo < 0 || yFrom > 8)
    {
      cls();
      cerr << "Invalid input!" << endl;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      return false;
    }

  xTo--;
  yTo--;

  //This return statement executes in the
  //event that the function has executed
  //normally, and indicates a proper move.
  return true;
}

bool validMove(int xFrom, int xTo, int yFrom, int yTo, int turn, int grid[][8], int &pieces)
{
  //"If the destination is not
  //empty, return false."
  if(grid[xTo][yTo] != 0)
    return false;

  //"If it is not the case that the desired
  //destination row is 1 or 2 rows below the source
  //row, and the desired column is within 2 columns
  //of the source (but is not the source column),
  //skip this block."

  //In other words, this block executes if the piece is
  //moving down on the board.
  if((xTo == xFrom + 1 || xTo == xFrom + 2) && (yTo == yFrom + 1 || yTo == yFrom - 1 || yTo == yFrom + 2 || yTo == yFrom - 2))
    {
      //Code to check validity of a downward move applies
      //equally to all pieces except a player 2 standard.
      if(grid[xFrom][yFrom] == 2)
	return false;

      //"If the player is attempting to move by
      //2 columns (jumping another piece) and they
      //are not moving down by two rows (they are only going
      //one or more than two, or they are going up, which
      //they may only do if they are a King)..."
      if((yTo == yFrom + 2 || yTo == yFrom - 2) && !(xTo == xFrom + 2))
	return false;

      //"If they jumped a piece (with a standard piece)..."
      if(xTo == xFrom + 2)
	{
	  //"...and they moved LEFT on the board..."
	  if(yTo < yFrom)
	    {
	      //"...if the square they are jumping is
	      //empty, it is an illegal move."

	      //xTo and yTo represent the destination.
	      //If yTo < yFrom, then adding to yTo moves it
	      //closer to yFrom.  If xTo > xFrom, subtracting from
	      //it gets closer to xFrom.  Therefore, xTo-1 and yTo+1
	      //represent the piece they would have jumped over.
	      if(grid[xTo-1][yTo+1] == 0)
		return false;
	      //"If they are trying to jump their own piece, it is an
	      //illegal move."
	      else if(grid[xTo-1][yTo+1] == turn || grid[xTo-1][yTo+1] == turn + 2 || grid[xTo-1][yTo+1] == turn - 2)
		return false;
	      //"Otherwise, remove the piece they jumped."
	      else
		grid[xTo-1][yTo+1] = 0;
	    }
	  else //If yTo > yFrom, or, if they moved RIGHT on the board.
	    {
	      //Same code, with the math flipped.  We now subtract 1
	      //from yTo to get the y in between them and their destination.
	      if(grid[xTo-1][yTo-1] == 0)
		return false;
	      else if(grid[xTo-1][yTo-1] == turn || grid[xTo-1][yTo-1] == turn + 2 || grid[xTo-1][yTo-1] == turn - 2)
		return false;
	      else
		grid[xTo-1][yTo-1] = 0;
	    }

	  //Take one piece off of player 2's
	  //piece count (this is a refernece
	  //variable).
	  pieces--;

	  return true;
	}

      //Returning true also in the case the
      //move was valid but nevertheless not
      //a jump.
      return true;
    }

  //"If the piece is moving up on the board..."
     else
	{
	  //This code applies to all pieces except
	  //player 1 standard.
	  if(grid[xFrom][yFrom] == 1)
	    return false;

	  //"If they have moved more than two columns, or tried to
	  //move in the same column, return false."
	  if(yTo == yFrom || yTo > yFrom + 2 || yTo < yFrom - 2)
	    return false;

	  //"If they have moved two columns but only one row,
	  //the move is illegal."
	  if((yTo == yFrom + 2 || yTo == yFrom - 2) && (xTo == xFrom + 1 || xTo == xFrom - 1))
	    return false;

	  //"If they have jumped a piece..."
	  //(We do not need to account for jumping
	  //a piece in a downward direction
	  //beacuse that will already be caught by the
	  //other if bock).
	  if(xTo == xFrom - 2)
	    {

	      //"If they have moved left (and up, which
	      //is already implied by the execution of this
	      //block at all)..."
	      if(yTo < yFrom)
		{
		  //Checking the same things as in the block above.
		  //xTo and yTo are the destination.  If xTo
		  //is less than xFrom, and yTo is less than yFrom,
		  //adding to both gets us to the in-between square.
		  //If that's empty, move is illegal.
		  if(grid[xTo+1][yTo+1] == 0)
		    return false;

		  //Again, if they jump their own piece, it's illegal.
		  else if(grid[xTo+1][yTo+1] == turn || grid[xTo+1][yTo+1] == turn + 2 || grid[xTo+1][yTo+1] == turn - 2)
		    return false;

		  //Otherwise, remove the piece they jumped.
		  else
		    grid[xTo+1][yTo+1] = 0;
		}

	      else //If yTo > yFrom (if they moved right).
		{
		  if(grid[xTo+1][yTo-1] == 0)
		    return false;
		  else if(grid[xTo+1][yTo-1] == turn || grid[xTo+1][yTo-1] == turn + 2 || grid[xTo+1][yTo-1] == turn -2)
		    return false;
		  else
		    grid[xTo+1][yTo-1] = 0;
		}

	      //Decrement OPPOSING player's pieces,
	      //return true.
	      pieces--;

	      return true;
	    }
	  //Again, returning true in the event of
	  //a non-jump move.
	  return true;
	}

      //If we got to this point, the move was illegal.
      return false;
}

void cls()
{
  //Used for clearing the screen after each move to avoid
  //distracting clutter.

  //Not always called from the same location, because
  //it is necessary to ensure that any error messages
  //are printed AFTER the screen is cleared.

  for(int i = 0; i < 32; i++)
    cout << endl;
}

bool isDoubleJumpAvailable(int x, int y, int turn, int grid[][8], int jumpReg[][2])
{
  int pieceType = grid[x][y];
  bool retVal = false;

  //The following if statement fixes a bug that grants a double jump if
  //the player has just double-jumped into the second-to-last x row of
  //the board.   It reads:  "If they are at the second-to-last x row at
  //either end, and there are no jumps available in a direction AWAY
  //from the x end of the board, there are no jumps to grant."
  if(x == 1 && !((grid[x+1][y+1] != 0 && grid[x+2][y+2] == 0) || (grid[x+1][y-1] != 0 && grid[x+2][y-2] == 0)) || x == 7 && !((grid[x-1][y-1] != 0 && grid[x-2][y-2] == 0) || (grid[x-1][y+1] != 0 && grid[x-2][y+2] == 0)))
    return false;

  //Initialize the jump registry to -1 (because
  //(0,0) is actually a place on the board and
  //could potentially be considered valid by
  //the program).
  for(int i = 0; i < 4; i++)
    {
      for(int j = 0; j < 2; j++)
	{
	  jumpReg[i][j] = -1;
	}
    }

  //If x or y are -1, there is no way the jump
  //can be valid.
  if(x == -1 || y == -1)
    retVal = false;

  //Checking for player 1.
  if(turn == 1)
    {
      //Each check is enclosed inside an if statement similar to
      //this one.  It makes sure that the check that will be executed
      //will not search an out-of-bounds array index, because that returns
      //random values that interfere wtih normal program function.
      if(!(x+2 > 7) && !(y+2 > 7))
	{
	  //All of the if statements that check for available double
	  //jumps check each square, one at a time, around the
	  //piece to see if there is an opposing piece there to be
	  //jumped and, additionally, if there is an empty space
	  //beyond that opposing piece into which to jump.
	  if(grid[x+1][y+1] == 2 || grid[x+1][y+1] == 4)
	    {
	      if(grid[x+2][y+2] == 0)
		{
		  jumpReg[0][0] = x+2;
		  jumpReg[0][1] = y+2;
		  retVal = true;
		}
	    }
	}

      if(!(x+2 > 7) && !(y-2 < 0))
	{
	  if(grid[x+1][y-1] == 2 || grid[x+1][y-1] == 4)
	    {
	      if(grid[x+2][y-2] == 0)
		{
		  jumpReg[1][0] = x+2;
		  jumpReg[1][1] = y-2;
		  retVal = true;
		}
	    }
	}


      //Now checking for player 1 king
      //("backwards" moves).
      if(pieceType == 3)
	{
	  if(!(x-2 < 0) && !(y+2 > 7))
	    {
	      if(grid[x-1][y+1] == 2 || grid[x-1][y+1] == 4)
		{
		  if(grid[x-2][y+2] == 0)
		    {
		      jumpReg[2][0] = x-2;
		      jumpReg[2][1] = y+2;
		      retVal = true;
		    }
		}
	    }

	  if(!(x-2 < 0) && !(y-2 < 0))
	    {
	      if(grid[x-1][y-1] == 2 || grid[x-1][y-1] == 4)
		{
		  if(grid[x-2][y-2] == 0)
		    {
		      jumpReg[3][0] = x-2;
		      jumpReg[3][1] = y-2;
		      retVal = true;
		    }
		}
	    }
	}
    }
  else if(turn == 2)
    {
      //Same code exactly, math inverted for player 2.
      if(!(x-2 < 0) && !(y+2 > 7))
	{
	  if(grid[x-1][y+1] == 1 || grid[x-1][y+1] == 3)
	    {
	      if(grid[x-2][y+2] == 0)
		{
		  jumpReg[0][0] = x-2;
		  jumpReg[0][1] = y+2;
		  retVal = true;
		}
	    }
	}

      if(!(x-2 < 0) && !(y-2 < 0))
	{
	  if(grid[x-1][y-1] == 1 || grid[x-1][y-1] == 3)
	    {
	      if(grid[x-2][y-2] == 0)
		{
		  jumpReg[1][0] = x-2;
		  jumpReg[1][1] = y-2;
		  retVal = true;
		}
	    }
	}


      //Player 2's king.
      if(pieceType == 4)
	{
	  if(!(x+2 > 7) && !(y+2 > 7))
	    {
	      if(grid[x+1][y+1] == 1 || grid[x+1][y+1] == 3)
		{
		  if(grid[x+2][y+2] == 0)
		    {
		      jumpReg[2][0] = x+2;
		      jumpReg[2][1] = y+2;
		      retVal = true;
		    }
		}
	    }

	  if(!(x+2 > 7) && !(y-2 < 0))
	    {
	      if(grid[x+1][y-1] == 1 || grid[x+1][y-1] == 3)
		{
		  if(grid[x+2][y-2] == 0)
		    {
		      jumpReg[3][0] = x+2;
		      jumpReg[3][1] = y-2;
		      retVal = true;
		    }
		}
	    }
	}
    }
  return retVal;
}

void grantDoubleJump(int player, int &x, int &y, int grid[][8], int &pieces, int jumpReg[][2])
{
  int xDest, yDest;

  //Validity and legality are not the same thing -
  //validity is whether validMove() returns true -
  //in other words, mere mechanical validity of
  //the move.  Legality refers to whether the move
  //the player has requested is logged in the jump registry.
  bool valid = false, legal = false;

  cls();
  drawBoard(grid);

  do
    {
      cout << endl << "Player " << player << ", you have a double jump"
	   << " available.\nEnter target coordinates or enter 0 to"
	   << " decline: ";

      //Reading in the coordinates of ONLY the target
      //destination - the player does NOT have to enter the
      //location of the source piece.
      while(!(cin >> xDest))
	{
	  cin.clear();
	  cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  cls();
	  drawBoard(grid);
	  cerr << "ENTER NUMBERS ONLY!\n";
	  cout << "Player " << player
	       << ", enter target coordinates or enter"
	       << " a zero to decline: ";
	}

      //If they enter a zero, they have declined
      //the double jump.
      if(xDest == 0)
	{
	  x = -1;
	  y = -1;;
	  return;
	}

      while(!(cin >> yDest))
	{
	  cin.clear();
	  cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  cls();
	  drawBoard(grid);
	  cerr << "ENTER NUMBERS ONLY!\n";
	  cout << "Player " << player
	       << ", enter target coordinates or enter"
	       << " 0 0 to decline: ";
	}

      //Again, zero is a decline.
      if(yDest == 0)
	{
	  x = -1;
	  y = -1;
	  return;
	}

      //Transforming the numbers they enter
      //into usable values for accessing the grid
      //array.
      xDest--;
      yDest--;

      //If the values are not in range, we set x and y
      //to these values and return.  This we do because the
      //while loop in main that is seeing if there are
      //available double jumps at every new location
      //(in case of a triple or more jump possibility)
      //will use these garbage values on
      //isDoulbeJumpAvailable() and, consequently, be broken.
      if(xDest < 0 || xDest > 7 || yDest < 0 || yDest > 7)
	{
	  x = xDest;
	  y = yDest;
	  return;
	}

      //This for loop scans the jump registry and determines
      //whether the coordinates the player entered are registered
      //as legitimate moves.
      for(int i = 0; i < 4; i++)
	{
	  if(jumpReg[i][0] == xDest && jumpReg[i][1] == yDest)
	    {
	      legal = true;
	      break;
	    }

	  //If we have scanned the entire registry
	  //without finding the move they want,
	  //it is invalid.
	  else if(i == 3)
	    {
	      cls();
	      cout << "Invalid move!";
	      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	      drawBoard(grid);
	    }

	}

      //If the move is not legal per the jump registry,
      //we do not even bother to check for its validity.
      if(legal)
	{
	  if(!validMove(x, xDest, y, yDest, player, grid, pieces))
	    {
	      cls();
	      cout << "Invalid move!" << endl;
	      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	      drawBoard(grid);
	    }
	  else
	    valid = true;
	}

    } while(!valid || !legal);


  //If their piece was a King, or they jumped into the last
  //row, make the piece at the new location a King.
  if((grid[x][y] == player + 2) || (xDest == 7 && player == 1) || (xDest == 0 && player == 2))
    grid[xDest][yDest] = player + 2;

  //Otherwise, make it a normal piece.
  else
    grid[xDest][yDest] = player;

  //Remove the piece at the source square.
  grid[x][y] = 0;

  x = xDest;
  y = yDest;
}

void drawBoard(int grid[][8])
{
  //Draw the top row of column numbers
  //and the upper borderline of the board.
  drawHeader();

  //Drawing the checkerboard to the screen,
  //row-by-row.
  for(int i = 0; i < 8; i++)
    drawRow(grid, i);


}
