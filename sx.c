/*
 _____                                 
/  ___|                                
\ `--.  __ _ _   _  __ _ _ __ ___  ___ 
 `--. \/ _` | | | |/ _` | '__/ _ \/ __|
/\__/ / (_| | |_| | (_| | | |  __/\__ \
\____/ \__, |\__,_|\__,_|_|  \___||___/
          | |                          
          |_|                          
                        _              
                       | |             
         __ _ _ __   __| |             
        / _` | '_ \ / _` |             
       | (_| | | | | (_| |             
        \__,_|_| |_|\__,_|             
                                       
                                       
 _____                                 
/  __ \                                
| /  \/_ __ ___  ___ ___  ___  ___ 
| |   | '__/ _ \/ __/ __|/ _ \/ __|    
| \__/\ | | (_) \__ \__ \  __/\__ \    
 \____/_|  \___/|___/___/\___||___/

Ryan Gregory - rdg1g16
CPU Inspiration From: https://gist.github.com/EightAndAHalfTails/1347738
*/

#include "os.h"
#include "color.h" //Colours used to change colour of drawings
#include "sx.h"
#include <util/delay.h>

sxb_t board[10]; //Indexed at 1, 0 not used, really not sure why I've done this
sxb_t current_player;
uint8_t location; 

int main(void)
{
    os_init();
    os_add_task(check_switches, 100, 1); //schedule os to check switches every 100ms
    boolean restart = false;

    rectangle rect;
    rect.top = 0; 
    rect.bottom = 240;
    rect.left = 0;
    rect.right = 320;
    fill_rectangle(rect, GRAY_0);

    display_string("\n");
    display_string("\n");
    display_string("\n");
    display_string("\n");
    display_string("          _____\n");                               
    display_string("         /  ___|\n");
    display_string("         \\ `--.  __ _ _   _  __ _ _ __ ___  ___\n");
    display_string("          `--. \\/ _` | | | |/ _` | '__/ _ \\/ __|\n");
    display_string("         /\\__/ / (_| | |_| | (_| | | |  __/\\__ \\\n");
    display_string("         \\____/ \\__, |\\__,_|\\__,_|_|  \\___||___/\n");
    display_string("                   | |\n");
    display_string("                   |_|\n");
    display_string("                                 _\n");
    display_string("                                | |\n");
    display_string("                  __ _ _ __   __| |\n");
    display_string("                 / _` | '_ \\ / _` |\n");
    display_string("                | (_| | | | | (_| |\n");
    display_string("                 \\__,_|_| |_|\\__,_|\n");                                                                       
    display_string("          _____\n");
    display_string("         /  __ \\\n");
    display_string("         | /  \\/_ __ ___  ___ ___  ___  ___\n");
    display_string("         | |   | '__/ _ \\/ __/ __|/ _ \\/ __|\n");
    display_string("         | \\__/\\ | | (_) \\__ \\__ \\  __/\\__ \\\n");
    display_string("          \\____/_|  \\___/|___/___/\\___||___/\n");
    _delay_ms(2000);
    
    do
    {
        restart = false;
        current_player = square;
        location = 5;
        clear_screen();
        initialise_board(); //make all locations blank
        display_board();

        /* RUN GAME */
        while((gamewon() == 0) && (gamedrawn() == 0)) //Game is in play
        {
	          if(current_player == square) //User makes move
	          {
	              update_board(make_move());
	          }
	          else //CPU makes move
	          {
	              update_board(make_computer_move());
            }
	          display_board(); //Show the new state of the board
	          current_player = !current_player; //Swap player
        }

        /* OUTPUT RESULT OF GAME */
        if (gamewon() == 1)
        {
            switch (current_player)
            {
                case square : display_string("\n\n\n\n\n\n\n\n  You Lose!\n\n Press Centre\nto Play Again"); break;
                case cross : display_string("\n\n\n\n\n\n\n\n   You Win!\n\n Press Centre\nto Play Again"); break;
                default: display_string("\nERROR"); //Unreachable
            }
        }
        else {display_string("\n\n\n\n\n\n\n\n    Draw!\n\n Press Centre\nto Play Again");}

        /* WAIT ON USER TO PRESS TO RESTART */
        while(!restart)
        {
            if(get_switch_short(_BV(SWC)))
            {
                restart = true;
            }
        }
    }while(restart);

    return 0;  
}

/* Scheduled to check for button press and move stored location */
int check_switches(int state) 
{
	if(get_switch_press(_BV(SWW))){move_location(1);}
	if(get_switch_press(_BV(SWE))){move_location(2);}
	if(get_switch_press(_BV(SWN))){move_location(3);}
	if(get_switch_press(_BV(SWS))){move_location(4);}
  return state;
}

/* Move current location */
void move_location(uint8_t direction)
{
    switch(direction)
    {
        case 1 : //Left
            if(location % 3 == 0 || location % 3 == 2){location--;}
            break;
        case 2 : //Right
            if(location % 3 == 1 || location % 3 == 2){location++;}
            break;
        case 3 : //Up
            if(location != 7 && location != 8 && location != 9){location += 3;}
            break;
        case 4 : //Down
            if(location / 3 >= 1){location -= 3;}
            break;
    }
    display_board(); //Show updated state of the board
}

/* Resets board to be blank */
void initialise_board(void) 
{ 
    uint8_t i;
    for(i = 0 ; i < 10 ; i++){board[i] = blank;}
}

/* Highlight current location and draw current state of board */
void display_board(void) 
{ 
    uint8_t i;
    highlight_location();
    for(i = 1; i < 10; i++)
    {
        switch(board[i])
        {
           case square : draw_square(i); break;
           case cross : draw_cross(i); break;
           case blank : break;
        }
    }
}

/* Draw 3x3 grid using fill_rectangle() from lcd.c*/
void draw_grid(void)
{
    rectangle leftV, rightV, topH, bottomH, bar;

    leftV.top = 0; 
    leftV.bottom = 240;
    leftV.left = 160;
    leftV.right = 161;

    rightV.top = 0; 
    rightV.bottom = 240;
    rightV.left = 240;
    rightV.right = 241;

    topH.top = 80; 
    topH.bottom = 81;
    topH.left = 80;
    topH.right = 320; 

    bottomH.top = 160; 
    bottomH.bottom = 161;
    bottomH.left = 80;
    bottomH.right = 320;

    bar.top = 0; 
    bar.bottom = 240;
    bar.left = 0;
    bar.right = 80;

    fill_rectangle(leftV, display.foreground);
    fill_rectangle(rightV, display.foreground);
    fill_rectangle(topH, display.foreground);
    fill_rectangle(bottomH, display.foreground);
    fill_rectangle(bar, GRAY_0);
    display_string("\n   Squares\n     and\n   Crosses\n");
}

/* Call square generation by passing top left co-ordinates for square to be drawn from */
void draw_square(uint8_t board_location)
{
  switch(board_location)
  {
      case 1 : generate_square(170, 90); break;
      case 2 : generate_square(170, 170); break;
      case 3 : generate_square(170, 250); break;
      case 4 : generate_square(90, 90); break;
      case 5 : generate_square(90, 170); break;
      case 6 : generate_square(90, 250); break;
      case 7 : generate_square(10, 90); break;
      case 8 : generate_square(10, 170); break;
      case 9 : generate_square(10, 250); break;
  }
}

/* Call cross generation by passing top left co-ordinates for cross to be drawn from */
void draw_cross(uint8_t board_location)
{
  switch(board_location)
  {
      case 1 : generate_cross(170, 90); break;
      case 2 : generate_cross(170, 170); break;
      case 3 : generate_cross(170, 250); break;
      case 4 : generate_cross(90, 90); break;
      case 5 : generate_cross(90, 170); break;
      case 6 : generate_cross(90, 250); break;
      case 7 : generate_cross(10, 90); break;
      case 8 : generate_cross(10, 170); break;
      case 9 : generate_cross(10, 250); break;
  }
}

/* Draw 4 lines using rectangles for each side of a square */
void generate_square(uint16_t top, uint16_t left)
{
    rectangle rect;
    uint8_t i;
    uint16_t top1 = top; //Top line of square
    uint16_t left1 = left; //Left line of square
    uint16_t right = left + 60; //Right line of square
    uint16_t right1 = left + 60; //Used to draw bottom line of square accross
    uint16_t bottom = top + 60; //Level for bottom of square
    for(i = 0; i < 60; i++)
    {
       rect.top = top; 
       rect.bottom = top;
       rect.left = left;
       rect.right = left;
       fill_rectangle(rect, GREEN_2);
       rect.top = top; 
       rect.bottom = top;
       rect.left = right;
       rect.right = right;
       fill_rectangle(rect, GREEN_2);
       rect.top = top1; 
       rect.bottom = top1;
       rect.left = left1;
       rect.right = left1;
       fill_rectangle(rect, GREEN_2);
       rect.top = bottom; 
       rect.bottom = bottom;
       rect.left = right1;
       rect.right = right1;
       fill_rectangle(rect, GREEN_2);

       top++;
       left1++;
       right1--;
    }
}

/* Draw 2 diagonal lines using rectangles to create a cross */
void generate_cross(uint16_t top, uint16_t left)
{
    rectangle rect;
    uint8_t i;
    uint16_t right = left + 60;
    for(i = 0; i < 60; i++)
    {
       rect.top = top; 
       rect.bottom = top;
       rect.left = left;
       rect.right = left;
       fill_rectangle(rect, RED_1);
       rect.top = top; 
       rect.bottom = top;
       rect.left = right;
       rect.right = right;
       fill_rectangle(rect, RED_1);

       top++;
       left++;
       right--;
    }
}

/* Draw white filled square to fill/highlight location on the board */
void highlight_location(void)
{
    clear_screen(); //Remove previous location highlight
    draw_grid(); //Draw grid back on
    rectangle rect;

    switch(location) //Define rectangle sides based on location on board, could be written more efficiently if I had time
    {
        case 1 : 
            rect.top = 160; 
            rect.bottom = 240;
            rect.left = 80;
            rect.right = 160;
            break;
        case 2 : 
            rect.top = 160; 
            rect.bottom = 240;
            rect.left = 160;
            rect.right = 240;
            break;
        case 3 : 
            rect.top = 160; 
            rect.bottom = 240;
            rect.left = 240;
            rect.right = 320;
            break;
        case 4 : 
            rect.top = 80; 
            rect.bottom = 160;
            rect.left = 80;
            rect.right = 160;
            break;
        case 5 : 
            rect.top = 80; 
            rect.bottom = 160;
            rect.left = 160;
            rect.right = 240;
            break;
        case 6 : 
            rect.top = 80; 
            rect.bottom = 160;
            rect.left = 240;
            rect.right = 320;
            break;
        case 7 : 
            rect.top = 0; 
            rect.bottom = 80;
            rect.left = 80;
            rect.right = 160;
            break;
        case 8 : 
            rect.top = 0; 
            rect.bottom = 80;
            rect.left = 160;
            rect.right = 240;
            break;
        case 9 : 
            rect.top = 0; 
            rect.bottom = 80;
            rect.left = 240;
            rect.right = 320;
            break;
        default : return;
    }
    fill_rectangle(rect, display.foreground); //display.foreground set to WHITE
    // char buf[30];
    // sprintf(buf, "\n\n\n\n\n\n\nBoard: %d\nLocation: %d", board[location], location);
    // display_string(buf);
}

/* Wait until centre button pressed in blank space */
uint8_t make_move(void)  
{
    LOOP:while (!get_switch_short(_BV(SWC))){}
    if(board[location] != blank)
    {
      goto LOOP; //I'm aware this is bad practice but it's simple and works  
    }
    return location;
}

/* Computer to make move based on board state */
uint8_t make_computer_move(void)
{
    uint8_t mask[8][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {1, 4, 7}, {2, 5, 8}, {3, 6, 9}, {1, 5, 9},{3, 5, 7}};
    uint8_t i, j, keysquare1 = 0, keysquare2 = 0;

    /* Check for winning opportunities */
    for(i=0; i < 8; i++) 
    {
        for(j = 0; j < 3; j++)
        {
            if((board[mask[i][j]] == board[mask[i][((j+1)%3)]]) && (board[mask[i][((j+2)%3)]] == blank) && (board[mask[i][j]] != blank) && (board[mask[i][j]] == cross))
            {
		            return mask[i][((j+2)%3)];
            }
        }
    }
    /* Check for blocking opportunities */
    for(i=0; i < 8; i++) 
    {
        for(j = 0; j < 3; j++)
        {
            if((board[mask[i][j]] == board[mask[i][((j+1)%3)]]) && (board[mask[i][((j+2)%3)]] == blank) && (board[mask[i][j]] != blank))
            {
		            return mask[i][((j+2)%3)];
            }
        }
    }
  
    /*checks for forking opportunities*/
    for(i=0; i < 8; i++) 
    {
        for(j = 0; j < 3; j++)
	    {
            if((board[mask[i][j]] == board[mask[i][(j+1)%3]]) && (board[mask[i][(j+2)%3]] != blank) && (board[mask[i][j]] == blank))
            {
	              if((mask[i][j] == keysquare1) || (mask[i][(j+1)%3] == keysquare1))
		            {
		                return keysquare1 ;
		            }
		            else if ((mask[i][j] == keysquare2) || (mask[i][(j+1)%3] == keysquare2))
		            {
		                return keysquare2 ;
		            }
		            else
		            {
		                keysquare1 = mask[i][j] ;
		                keysquare2 = mask[i][(j+1)%3] ;
		            }
            }
        } 
    }

    /*Opposite Corner*/
    if((board[1] == !current_player) && (board[9] == blank)){return 9;}
    if((board[9] == !current_player) && (board[1] == blank)){return 1;} 
    if((board[7] == !current_player) && (board[3] == blank)){return 3;}
    if((board[3] == !current_player) && (board[7] == blank)){return 7;}

    /*Centre*/
    if(board[5] == blank){return 5;}

    /*Empty Corner*/
    if(board[1] == blank){return 1;}
    if(board[3] == blank){return 3;}
    if(board[7] == blank){return 7;}
    if(board[9] == blank){return 9;}

    /*Empty Side*/
    if(board[2] == blank){return 2;} 
    if(board[4] == blank){return 4;}
    if(board[6] == blank){return 6;}
    if(board[8] == blank){return 8;}

    /*Won't be reached*/
    else{return 0;}
}

/*updates board with a given player's move*/
void update_board(uint8_t move) 
{ 
    board[move] = current_player;
}

boolean gamewon(void) /*returns a true value if game has been won, false otherwise*/
{ 
    uint8_t i;
    uint8_t mask[8][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {1, 4, 7}, {2, 5, 8}, {3, 6, 9}, {1, 5, 9}, {3, 5, 7}};
    boolean result = false;
  
    for(i = 0; i < 8; i++)
    {
        result = result || ((board[mask[i][0]] != blank) && (board[mask[i][0]] == board[mask[i][1]]) && (board[mask[i][1]] == board[mask[i][2]]));
    }
    return result;
}

boolean gamedrawn(void) /*returns a true value if game is a draw*/
{ 
    uint8_t i;
    boolean result;
    boolean unfilled = false;

    for(i = 1; i < 10; i++ )
    {
        unfilled = unfilled || (board[i] == blank);
    }
    result = (!unfilled && !(gamewon()));
    return result;
}