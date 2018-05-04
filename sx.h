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
*/

typedef enum sxb {square, cross, blank} sxb_t;
typedef enum bool {false, true} boolean;

int check_switches(int state);
void move_location(uint8_t direction);
void initialise_board(void);
void display_board(void);
void draw_grid(void);
void draw_square(uint8_t board_location);
void draw_cross(uint8_t board_location);
void generate_square(uint16_t top, uint16_t left);
void generate_cross(uint16_t top, uint16_t left);
void highlight_location(void);
uint8_t make_move(void);
uint8_t make_computer_move(void);
void update_board(uint8_t move);
boolean gamewon(void);
boolean gamedrawn(void);