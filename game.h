struct pipe {
  int top; // the height of the top section of the pipe
  int bottom; // the height of bottom section of the pipe
  int x; // the x position of the right side of the pipe on the screen
  pipe * next;
}; // the width of a pipe is 10 pixels
typedef struct pipe pipe;

// Singly linked list of all pipes on the screen
pipe * pipe_list; 

// The x position of the leftmost pipe in the previous frame
int first_pipe = 141;

// The x position of the rightmost pipe in the previous frame
int last_pipe = 141;

// Whether or not the game is over
int game_over = 0;

// The bird looks like this in pixels, with a radius of 3 pixels
//   xxx
//  xxxxx
// xxxxxxx
// xxxxxxx
// xxxxxxx
//  xxxxx
//   xxx

// x position of the center of the bird on the screen
int bird_x = 20;

// y position of the center of the bird on the screen
int bird_y = 16;

// whether the button was pressed in the previous frame
int last_pressed = 0;

// The number of frames since the last bird movement
int time_since_flap = 10;

// The number of pipes that the bird has passed
int score = 0;

// The string to be displayed when the game is over
char * loss_msg = "GAME OVER!";

// String to be displayed while showing the player their score
char * score_msg = "YOUR SCORE: ";

/* Set up the GPIO for the button and display */
void initialize (void); 

/* Reset game variables to start a new game */
void reset_game (void);

/* The vertical movement of the bird based on the time since the last flap of the bird */
int vert_movement (void);

/* Creates a new pipe with difficulty based on the score and appends it to the pipe_list */
void create_pipe (void);

/* Whether or not the bird has collided with a pipe */
int collides (pipe * p);

/* Update the pipes and the bird based on joystick input */
void update (void);

/* Draw the pipes and bird to the display */
void draw (void);

/* Display a message that the game over to the display */
void display_loss_msg (void);

/* Display the player's score to the display */
void display_score_msg (void);