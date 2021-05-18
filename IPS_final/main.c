// [main.c]
// this template is provided for the 2D shooter game.

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED

/* Constants. */

// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
// Display (screen) height.
const int SCREEN_H = 600;
// At most 4 audios can be played at a time.
const int RESERVE_SAMPLES = 4;
// Same as:
// const int SCENE_MENU = 1;
// const int SCENE_START = 2;
enum {
	SCENE_MENU = 1,
	SCENE_START = 2,
	// [HACKATHON 3-7]
	// TODO: Declare a new scene id.
    SCENE_SETTINGS = 3,
    SCENE_LOSE = 4,
    SCENE_BOSS = 5,
    SCENE_WIN  = 6,
    SCENE_CHAR = 7,
    SCENE_HELP = 8,
    SCENE_SETTINGS2 = 9,
    SCENE_HELP_1 = 10
};

/* Input states */

// The active scene id.
int active_scene;
int cha = 2;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool *mouse_state;
// Mouse position.
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...
int score = 0;
int defeat = 0;
int hp = 1;
int check = 0;
/* Variables for allegro basic routines. */
bool done = false;
ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/
ALLEGRO_FONT* font_pirulen_16;
ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
// TODO: More shared resources or data that needed to be accessed
// across different scenes.

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
// [HACKATHON 3-1]
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_BITMAP* img_setting_bg;
ALLEGRO_BITMAP* img_backicon1;
ALLEGRO_BITMAP* img_backicon2;
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;
ALLEGRO_SAMPLE* boss_bgm;
ALLEGRO_SAMPLE_ID boss_bgm_id;
ALLEGRO_BITMAP* img_win;
ALLEGRO_BITMAP* img_char_bg;
ALLEGRO_BITMAP* img_apa;
ALLEGRO_BITMAP* img_f22;
ALLEGRO_BITMAP* img_space;
ALLEGRO_BITMAP* img_apachar;
ALLEGRO_BITMAP* img_f22char;
ALLEGRO_BITMAP* img_spacechar;
ALLEGRO_BITMAP* img_help_bg;

/* Start Scene resources*/
ALLEGRO_BITMAP* img_lose;
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_SAMPLE* gun_bgm;
ALLEGRO_SAMPLE_ID gun_bgm_id;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
ALLEGRO_SAMPLE* win_bgm;
ALLEGRO_SAMPLE_ID win_bgm_id;
ALLEGRO_SAMPLE* setting_bgm;
ALLEGRO_SAMPLE_ID setting_bgm_id;
ALLEGRO_SAMPLE* lose_bgm;
ALLEGRO_SAMPLE_ID lose_bgm_id;
ALLEGRO_BITMAP* img_restart;
ALLEGRO_BITMAP* img_restart1;
ALLEGRO_BITMAP* img_boss_bg;
ALLEGRO_BITMAP* img_boss;
ALLEGRO_BITMAP* img_boss_bul_l;
ALLEGRO_BITMAP* img_boss_bul_r;
// [HACKATHON 2-1]
// TODO: Declare a variable to store your bullet's image.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_bullet;
ALLEGRO_BITMAP* img_enebul;

typedef struct {
	// The center coordinate of the image.
	float x, y;
	// The width and height of the object.
	float w, h;
	// The velocity in x, y axes.
	float vx, vy;
	// Should we draw this object on the screen.
	bool hidden;
	// The pointer to the object¡¦s image.
	ALLEGRO_BITMAP* img;
} MovableObject;
void draw_movable_object(MovableObject obj);
#define MAX_ENEMY 4
// [HACKATHON 2-2]
// TODO: Declare the max bullet count that will show on screen.
// You can try max 4 bullets here and see if you needed more.
// Uncomment and fill in the code below.
#define MAX_BULLET 4
#define MAX_BOSS_BULLET 5


int flag[MAX_ENEMY];
int bossflag;
MovableObject plane;
MovableObject enemies[MAX_ENEMY];
MovableObject boss;
MovableObject boss_bul_l[MAX_BOSS_BULLET];
MovableObject boss_bul_r[MAX_BOSS_BULLET];
// [HACKATHON 2-3]
// TODO: Declare an array to store bullets with size of max bullet count.
// Uncomment and fill in the code below.
MovableObject bullets[MAX_BULLET];
MovableObject enebul[MAX_ENEMY][MAX_BULLET];
// [HACKATHON 2-4]
// TODO: Set up bullet shooting cool-down variables.
// 1) Declare your shooting cool-down time as constant. (0.2f will be nice)
// 2) Declare your last shoot timestamp.
// Uncomment and fill in the code below.
const float MAX_COOLDOWN = 0.2f;
const float MAX_ENE_CD = 0.5f;
const float MAX_BOSS_CD = 0.3f;
double last_shoot_timestamp;
double last_shoot_timestamp_1;
double last_shoot_timestamp_2;
double last_shoot_timestamp_3;
double last_shoot_timestamp_4;
double last_shoot_timestamp_boss_l;
double last_shoot_timestamp_boss_r;
double last_shoot_timestamp_5;
/* Declare function prototypes. */

// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);
bool pnt_in_multirect(int px,int py,int xl,int xr,int yu,int yd);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);

int main(int argc, char** argv) {
	// Set random seed for better random outcome.
	srand(time(NULL));
	allegro5_init();
	game_log("Allegro5 initialized");
	game_log("Game begin");
	// Initialize game variables.
	game_init();
	game_log("Game initialized");
	// Draw the first frame.
	game_draw();
	game_log("Game start event loop");
	// This call blocks until the game is finished.
	game_start_event_loop();
	game_log("Game end");
	game_destroy();
	return 0;
}

void allegro5_init(void) {
	if (!al_init())
		game_abort("failed to initialize allegro");

	// Initialize add-ons.
	if (!al_init_primitives_addon())
		game_abort("failed to initialize primitives add-on");
	/*if (!al_init_font_addon())
		game_abort("failed to initialize font add-on");
	*/
	al_init_font_addon();

	if (!al_init_ttf_addon())
		game_abort("failed to initialize ttf add-on");
	if (!al_init_image_addon())
		game_abort("failed to initialize image add-on");
	if (!al_install_audio())
		game_abort("failed to initialize audio add-on");
	if (!al_init_acodec_addon())
		game_abort("failed to initialize audio codec add-on");
	if (!al_reserve_samples(RESERVE_SAMPLES))
		game_abort("failed to reserve samples");
	if (!al_install_keyboard())
		game_abort("failed to install keyboard");
	if (!al_install_mouse())
		game_abort("failed to install mouse");
	// TODO: Initialize other addons such as video, ...

	// Setup game display.
	game_display = al_create_display(SCREEN_W, SCREEN_H);
	if (!game_display)
		game_abort("failed to create display");
	al_set_window_title(game_display, "I2P(I)_2019 Final Project <107023058>");

	// Setup update timer.
	game_update_timer = al_create_timer(1.0f / FPS);
	if (!game_update_timer)
		game_abort("failed to create timer");

	// Setup event queue.
	game_event_queue = al_create_event_queue();
	if (!game_event_queue)
		game_abort("failed to create event queue");

	// Malloc mouse buttons state according to button counts.
	const unsigned m_buttons = al_get_mouse_num_buttons();
	game_log("There are total %u supported mouse buttons", m_buttons);
	// mouse_state[0] will not be used.
	mouse_state = malloc((m_buttons + 1) * sizeof(bool));
	memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

	// Register display, timer, keyboard, mouse events to the event queue.
	al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
	al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
	al_register_event_source(game_event_queue, al_get_keyboard_event_source());
	al_register_event_source(game_event_queue, al_get_mouse_event_source());
	// TODO: Register other event sources such as timer, video, ...

	// Start the timer to update and draw the game.
	al_start_timer(game_update_timer);
}

void game_init(void) {
	/* Shared resources*/
	font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
	if (!font_pirulen_32)
		game_abort("failed to load font: pirulen.ttf with size 32");
    font_pirulen_16 = al_load_font("pirulen.ttf",16,0);
    if(!font_pirulen_16)
        game_abort("failed to load font: pirulen.ttf with size 20");
	font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
	if (!font_pirulen_24)
		game_abort("failed to load font: pirulen.ttf with size 24");

	/* Menu Scene resources*/
	main_img_background = load_bitmap_resized("main-bg.jpg", SCREEN_W, SCREEN_H);

	img_help_bg = load_bitmap_resized("help_bg.png",SCREEN_W,SCREEN_H);
	if(!img_help_bg)
        game_abort("failed to load image: help_bg.png");

    boss_bgm = al_load_sample("boss_bgm.ogg");
    if(!boss_bgm)
        game_abort("failed to load audio: boss_bgm.ogg");
	main_bgm = al_load_sample("mainbgm.ogg");
	if (!main_bgm)
		game_abort("failed to load audio: mainbgm.ogg");
    img_lose = load_bitmap_resized("lose_bg.jpg",SCREEN_W,SCREEN_H);
    if(!img_lose)
        game_abort("failed to load image: lose_bg.jpg");
    img_restart1 = al_load_bitmap("restart1.png");
    if(!img_restart1)
        game_abort("failed to load image: restart1.jpg");
    img_win = load_bitmap_resized("win_bg.jpg",SCREEN_W,SCREEN_H);
    if(!img_win)
        game_abort("failed to load image: win_bg.jpg");
    img_restart = al_load_bitmap("restart.png");
    if(!img_restart)
        game_abort("failed to load image: restart.jpg");
    img_boss_bg = al_load_bitmap("boss_bg.jpg");
    if(!img_boss_bg)
        game_abort("failed to load image: boss_bg.jpg");
    img_setting_bg = load_bitmap_resized("setting_bg.jpg",SCREEN_W,SCREEN_H);
    if(!img_setting_bg)
        game_abort("failed to load image: setting_bg.jpg");
    img_boss = al_load_bitmap("boss.png");
    if(!img_boss)
        game_abort("failed to load image: boss.png");
    img_boss_bul_l = al_load_bitmap("boss_bul_l.png");
    if(!img_boss_bul_l)
        game_abort("failed to load image: boss_bul_l.png");
    img_boss_bul_r = al_load_bitmap("boss_bul_r.png");
    if(!img_boss_bul_r)
        game_abort("failed to load image: boss_bul_r.png");
    img_char_bg = load_bitmap_resized("char.jpg",SCREEN_W,SCREEN_H);
    if(!img_char_bg)
        game_abort("failed to load image: char.jpg");
    img_apachar = al_load_bitmap("apacha.jpg");
    if(!img_apachar)
        game_abort("failed to load image: apacha.jpg");
    img_f22char = al_load_bitmap("f22cha.jpg");
    if(!img_f22char)
        game_abort("failed to load image: f22cha.jpg");
    img_spacechar = al_load_bitmap("spacha.jpg");
    if(!img_spacechar)
        game_abort("failed to load image: spacha.jpg");
    img_apa = al_load_bitmap("apa.png");
    if(!img_apa)
        game_abort("failed to load image: apa.png");
    img_f22 = al_load_bitmap("plane.png");
    if(!img_f22)
        game_abort("failed to load image: plane.png");
    img_space = al_load_bitmap("space.png");
    if(!img_space)
        game_abort("failed to load image: space.png");
    gun_bgm = al_load_sample("gun_bgm.ogg");
    if(!gun_bgm)
        game_abort("failed to load audio: gun_bgm.ogg");


	// [HACKATHON 3-4]
	// TODO: Load settings images.
	// Don't forget to check their return values.
	// Uncomment and fill in the code below.
	img_backicon1 = al_load_bitmap("back1.png");
	if (!img_backicon1)
		game_abort("failed to load image: back1.png");
	img_backicon2 = al_load_bitmap("back2.png");
	if (!img_backicon2)
		game_abort("failed to load image: back2.png");

	img_settings = al_load_bitmap("settings.png");
	if (!img_settings)
		game_abort("failed to load image: settings.png");
	img_settings2 = al_load_bitmap("settings2.png");
	if (!img_settings2)
		game_abort("failed to load image: settings2.png");

	/* Start Scene resources*/
	start_img_background = load_bitmap_resized("start-bg.jpg", SCREEN_W, SCREEN_H);

	start_img_plane = al_load_bitmap("plane.png");
	if (!start_img_plane)
		game_abort("failed to load image: plane.png");

	start_img_enemy = al_load_bitmap("smallfighter0006.png");
	if (!start_img_enemy)
		game_abort("failed to load image: smallfighter0006.png");

	setting_bgm = al_load_sample("setting.ogg");
	if(!setting_bgm)
        game_abort("failed to load audio: setting.ogg");
	start_bgm = al_load_sample("start.ogg");
	if (!start_bgm)
		game_abort("failed to load audio: start.ogg");
    win_bgm = al_load_sample("win.ogg");
    if(!win_bgm)
        game_abort("failed to load audio: win.ogg");
    lose_bgm = al_load_sample("sad.ogg");
    if(!win_bgm)
        game_abort("failed to load audio: sad.ogg");

	// [HACKATHON 2-5]
	// TODO: Initialize bullets.
	// 1) Search for a bullet image online and put it in your project.
	//    You can use the image we provided.
	// 2) Load it in by 'al_load_bitmap' or 'load_bitmap_resized'.
	// 3) If you use 'al_load_bitmap', don't forget to check its return value.
	// Uncomment and fill in the code below.
	img_bullet = al_load_bitmap("image12.png");
	if (!img_bullet)
		game_abort("failed to load image: image12.png");

    img_enebul = al_load_bitmap("bomb.png");
    if(!img_enebul)
        game_abort("failed to load image: bomb.png");

	// Change to first scene.
	game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void) {
	//bool done = false;
	ALLEGRO_EVENT event;
	int redraws = 0;
	while (!done) {
		al_wait_for_event(game_event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// Event for clicking the window close button.
			game_log("Window close button clicked");
			done = true;
		} else if (event.type == ALLEGRO_EVENT_TIMER) {
			// Event for redrawing the display.
			if (event.timer.source == game_update_timer)
				// The redraw timer has ticked.
				redraws++;
		} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			// Event for keyboard key down.
			game_log("Key with keycode %d down", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = true;
			on_key_down(event.keyboard.keycode);
		} else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			// Event for keyboard key up.
			game_log("Key with keycode %d up", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = false;
		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			// Event for mouse key down.
			game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = true;
			on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			// Event for mouse key up.
			game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = false;
		} else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (event.mouse.dx != 0 || event.mouse.dy != 0) {
				// Event for mouse move.
				game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
				mouse_x = event.mouse.x;
				mouse_y = event.mouse.y;
			} else if (event.mouse.dz != 0) {
				// Event for mouse scroll.
				game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
			}
		}
		// TODO: Process more events and call callbacks by adding more
		// entries inside Scene.

		// Redraw
		if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
			// if (redraws > 1)
			// 	game_log("%d frame(s) dropped", redraws - 1);
			// Update and draw the next frame.
			game_update();
			game_draw();
			redraws = 0;
		}
	}
}

void game_update(void) {
	if (active_scene == SCENE_START) {
		plane.vx = plane.vy = 0;
		if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
			plane.vy -= 1;
		if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
			plane.vy += 1;
		if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
			plane.vx -= 1;
		if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
			plane.vx += 1;
		// 0.71 is (1/sqrt(2)).
		plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
		plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);

				int i;

		for(i=0;i<MAX_ENEMY;i++){
            if(i%2==0) {
                enemies[i].x += enemies[i].vx;
                enemies[i].y -= enemies[i].vy;
            }
            else{
                enemies[i].x -= enemies[i].vx;
                enemies[i].y += enemies[i].vy;
            }
            if(enemies[i].x <= 0 + 50)
            {
                enemies[i].x = 51;
                enemies[i].vx = - enemies[i].vx ;
            }

            else if(enemies[i].x >= SCREEN_W - 50)
            {
                enemies[i].x = SCREEN_W - 51;
                enemies[i].vx = - enemies[i].vx ;
            }
            if(enemies[i].y <= 0 + 60 || enemies[i].y >= SCREEN_H/2 - 100)
                enemies[i].vy = - enemies[i].vy ;

		}

//	}

		// [HACKATHON 1-1]
		// TODO: Limit the plane's collision box inside the frame.
		//       (x, y axes can be separated.)
		// Uncomment and fill in the code below.
		if (plane.x - (plane.w)/2 < 0)
			plane.x = plane.w/2;
		else if (plane.x+(plane.w)/2 > SCREEN_W)
			plane.x = SCREEN_W-(plane.w)/2;
		if (plane.y - (plane.h)/2 < 0)
			plane.y = (plane.h)/2;
		else if (plane.y+(plane.h)/2 > SCREEN_H)
			plane.y = SCREEN_H-(plane.h)/2;

			int k;



            for(k=0;k<MAX_ENEMY;k++)
        {
            if (enemies[k].x - (enemies[k].w)/2 < 0)
			enemies[k].x = enemies[k].w/2;
		    else if (enemies[k].x+(enemies[k].w)/2 > SCREEN_W)
			enemies[k].x = SCREEN_W-(enemies[k].w)/2;

            if (enemies[k].y - (enemies[k].h)/2 < 0)
			enemies[k].y = (enemies[k].h)/2;
		    else if (enemies[k].y+(enemies[k].h)/2 > SCREEN_H)
			enemies[k].y = SCREEN_H-(enemies[k].h)/2;


        }





		// [HACKATHON 2-7]
		// TODO: Update bullet coordinates.
		// 1) For each bullets, if it's not hidden, update x, y
		// according to vx, vy.
		// 2) If the bullet is out of the screen, hide it.
		// Uncomment and fill in the code below.

		for (i=0;i<MAX_BULLET;i++) {
			if (bullets[i].hidden)
				continue;
			bullets[i].x += bullets[i].vx;
			bullets[i].y += bullets[i].vy;
			if (bullets[i].y - (bullets[i].h/2)< 0)
				bullets[i].hidden = true;
            for(int k=0;k<MAX_ENEMY;k++)
            {
                if(pnt_in_multirect(bullets[i].x,bullets[i].y,enemies[k].x-enemies[k].w/2,enemies[k].x+enemies[k].w/2,enemies[k].y+enemies[k].h/2,enemies[k].y-enemies[k].h/2))
                {
                    if(enemies[k].hidden!=true)
                    {

                        score+=20;
                        flag[k]++;
                        //al_draw_filled_rectangle(enemies[i].x-50,enemies[i].y-enemies[i].h/2-15,enemies[i].x+30,enemies[i].y-enemies[i].h/2-5,al_map_rgb(0,0,0));
                    }

                    bullets[i].hidden = true;
                    if(flag[k]==5&&enemies[k].hidden!=true)
                    {
                        enemies[k].hidden = true;
                        defeat++;
                    }

                }
            }

		}
		int j;

		for(i = 0;i<MAX_ENEMY;i++)
        {
            for(j=0;j<MAX_BULLET;j++)
            {
                if(enebul[i][j].hidden)
                    continue;
                enebul[i][j].x+= enebul[i][j].vx;
                enebul[i][j].y+= enebul[i][j].vy;
                if(enebul[i][j].y+(enebul[i][j].h/2)>SCREEN_H)
                    enebul[i][j].hidden = true;
                if(pnt_in_multirect(enebul[i][j].x,enebul[i][j].y,plane.x-plane.w/2+5,plane.x+plane.w/2-5,plane.y+plane.h/2,plane.y-plane.h/2+15))
                {
                    if(enebul[i][j].hidden != true&&active_scene==SCENE_START)
                    {
                        hp--;
                        score-=10;
                        if(score<0)
                        {
                            score=0;
                        }
                    }

                    enebul[i][j].hidden = true;

                }

                /**/
            }

        }

		// [HACKATHON 2-8]
		// TODO: Shoot if key is down and cool-down is over.
		// 1) Get the time now using 'al_get_time'.
		// 2) If Space key is down in 'key_state' and the time
		//    between now and last shoot is not less that cool
		//    down time.
		// 3) Loop through the bullet array and find one that is hidden.
		//    (This part can be optimized.)
		// 4) The bullet will be found if your array is large enough.
		// 5) Set the last shoot time to now.
		// 6) Set hidden to false (recycle the bullet) and set its x, y to the
		//    front part of your plane.
		// Uncomment and fill in the code below.
		double now = al_get_time();
		if (key_state[ALLEGRO_KEY_LCTRL] && now - last_shoot_timestamp >= MAX_COOLDOWN) {
		    for (i = 0;i<MAX_BULLET;i++) {
		        if (bullets[i].hidden)
		            break;
		    }
		   if (i < MAX_BULLET) {
		        last_shoot_timestamp = now;
		        bullets[i].hidden = false;
		        bullets[i].x = plane.x ;
		        bullets[i].y = plane.y - plane.h/2;
                al_stop_sample(&gun_bgm_id);
            if (!al_play_sample(gun_bgm, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &gun_bgm_id))
                game_abort("failed to play audio (bgm)");
		    }
		}

		double now1 = al_get_time();

		while(enemies[0].hidden!=true&&now1 - last_shoot_timestamp_1>=MAX_ENE_CD)
        {
            for(j = 0;j<MAX_BULLET;j++)
            {
                if(enebul[0][j].hidden)
                    break;
            }
            if(j==MAX_BULLET)
                return;
            last_shoot_timestamp_1 = now1;
            enebul[0][j].hidden = false;
            enebul[0][j].x = enemies[0].x;
            enebul[0][j].y = enemies[0].y+enemies[0].h/2;
        }


        double now2 = al_get_time();

		while(enemies[1].hidden!=true&&now2 - last_shoot_timestamp_2>=MAX_ENE_CD)
        {
            for(j = 0;j<MAX_BULLET;j++)
            {
                if(enebul[1][j].hidden)
                    break;
            }
            if(j==MAX_BULLET)
                return;
            last_shoot_timestamp_2 = now2;
            enebul[1][j].hidden = false;
            enebul[1][j].x = enemies[1].x;
            enebul[1][j].y = enemies[1].y+enemies[1].h/2;
        }
        double now3 = al_get_time();

		while(enemies[2].hidden!=true&&now3 - last_shoot_timestamp_3>=MAX_ENE_CD)
        {
            for(j = 0;j<MAX_BULLET;j++)
            {
                if(enebul[2][j].hidden)
                    break;
            }
            if(j==MAX_BULLET)
                return;
            last_shoot_timestamp_3 = now3;
            enebul[2][j].hidden = false;
            enebul[2][j].x = enemies[2].x;
            enebul[2][j].y = enemies[2].y+enemies[2].h/2;
        }
        double now4 = al_get_time();

		while(enemies[3].hidden!=true&&now4 - last_shoot_timestamp_4>=MAX_ENE_CD)
        {
            for(j = 0;j<MAX_BULLET;j++)
            {
                if(enebul[3][j].hidden)
                    break;
            }
            if(j==MAX_BULLET)
                return;
            last_shoot_timestamp_4 = now4;
            enebul[3][j].hidden = false;
            enebul[3][j].x = enemies[3].x;
            enebul[3][j].y = enemies[3].y+enemies[3].h/2;
        }

}
        if(active_scene == SCENE_BOSS)
        {
            		plane.vx = plane.vy = 0;
		if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
			plane.vy -= 1;
		if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
			plane.vy += 1;
		if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
			plane.vx -= 1;
		if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
			plane.vx += 1;
		// 0.71 is (1/sqrt(2)).
		plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
		plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);

        if (plane.x - plane.w/2 < 0)
			plane.x = plane.w/2;
        else if (plane.x + plane.w/2 > SCREEN_W)
			plane.x = SCREEN_W - plane.w/2;
		if (plane.y - plane.h/2 < 0)
			plane.y = plane.h/2;
		else if (plane.y + plane.h/2 > SCREEN_H)
			plane.y = SCREEN_H -plane.h/2;

        boss.hidden = false;
        boss.x += boss.vx;
        boss.y += boss.vy;
        if (boss.x - boss.w/2 < 0){
            boss.vx = - boss.vx ;
            boss.x = boss.w/2;
        }
        else if (boss.x + boss.w/2 > SCREEN_W){
            boss.vx = - boss.vx ;
            boss.x = SCREEN_W - boss.w/2 ;
        }
        if (boss.y > SCREEN_H/2)
        {
            boss.vy = -boss.vy;
            boss.y  = SCREEN_H/2;
        }
        else if(boss.y < boss.h/2)
        {
            boss.vy = -boss.vy;
            boss.y = boss.h/2;
        }
        int i,j;

        for (i=0;i<MAX_BULLET;i++) {
			if (bullets[i].hidden)
				continue;
			bullets[i].x += bullets[i].vx;
			bullets[i].y += bullets[i].vy;
			if (bullets[i].y - (bullets[i].h/2)< 0)
				bullets[i].hidden = true;

            if(pnt_in_multirect(bullets[i].x,bullets[i].y,boss.x-boss.w/2,boss.x+boss.w/2,boss.y+boss.h/2,boss.y-boss.h/2))
            {
                if(boss.hidden!=true)
                {
                    score+=30;
                    bossflag++;
                }
                bullets[i].hidden =true;
                if(bossflag == 20&&boss.hidden!=true)
                {
                    boss.hidden = true;
                    defeat++;
                }
            }
            }
            int m;
            for(m = 0;m<MAX_BOSS_BULLET;m++)
            {
                if(boss_bul_l[m].hidden)
                    continue;
                boss_bul_l[m].x += boss_bul_l[m].vx;
                boss_bul_l[m].y += boss_bul_l[m].vy;
                if(boss_bul_l[m].y+boss_bul_l[m].h/2>SCREEN_H)
                    boss_bul_l[m].hidden = true;
                if(pnt_in_multirect(boss_bul_l[m].x,boss_bul_l[m].y,plane.x-plane.w/2+5,plane.x+plane.w/2-5,plane.y+plane.h/2,plane.y-plane.h/2+15))
                {
                    if(boss_bul_l[m].hidden!= true&&active_scene ==SCENE_BOSS)
                    {
                        hp -= 2;
                        score -=20;
                        if(score < 0)
                        {
                            score = 0;
                        }
                    }
                    boss_bul_l[m].hidden=true;
                }
            }
            for(m = 0;m<MAX_BOSS_BULLET;m++)
            {
                if(boss_bul_r[m].hidden)
                    continue;
                boss_bul_r[m].x += boss_bul_r[m].vx;
                boss_bul_r[m].y += boss_bul_r[m].vy;
                if(boss_bul_r[m].y+boss_bul_r[m].h/2>SCREEN_H)
                    boss_bul_r[m].hidden = true;
                if(pnt_in_multirect(boss_bul_r[m].x,boss_bul_r[m].y,plane.x-plane.w/2+5,plane.x+plane.w/2-5,plane.y+plane.h/2,plane.y-plane.h/2+15))
                {
                    if(boss_bul_r[m].hidden!= true&&active_scene ==SCENE_BOSS)
                    {
                        hp = hp-2;
                        score -=20;
                        if(score < 0)
                        {
                            score = 0;
                        }
                    }
                    boss_bul_r[m].hidden=true;
                }
            }


        double now5 = al_get_time();
		if (key_state[ALLEGRO_KEY_LCTRL] && now5 - last_shoot_timestamp_5 >= MAX_COOLDOWN) {
		    for (i = 0;i<MAX_BULLET;i++) {
		        if (bullets[i].hidden)
		            break;
		    }
		   if (i < MAX_BULLET) {
		        last_shoot_timestamp_5 = now5;
		        bullets[i].hidden = false;
		        bullets[i].x = plane.x ;
		        bullets[i].y = plane.y - plane.h/2;
		        al_stop_sample(&gun_bgm_id);
            if (!al_play_sample(gun_bgm, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &gun_bgm_id))
                game_abort("failed to play audio (bgm)");
		    }
		}

		double now_boss_l = al_get_time();

		while(boss.hidden!=true&&now_boss_l - last_shoot_timestamp_boss_l>=MAX_BOSS_CD)
        {
            for(j = 0;j<MAX_BOSS_BULLET;j++)
            {
                if(boss_bul_l[j].hidden)
                    break;
            }
            if(j==MAX_BOSS_BULLET)
                return;
            last_shoot_timestamp_boss_l = now_boss_l;
            boss_bul_l[j].hidden = false;
            boss_bul_l[j].x = boss.x-60;
            boss_bul_l[j].y = boss.y+boss.h/2;
        }

        double now_boss_r = al_get_time();

		while(boss.hidden!=true&&now_boss_r - last_shoot_timestamp_boss_r>=MAX_BOSS_CD)
        {
            for(j = 0;j<MAX_BOSS_BULLET;j++)
            {
                if(boss_bul_r[j].hidden)
                    break;
            }
            if(j==MAX_BOSS_BULLET)
                return;
            last_shoot_timestamp_boss_r = now_boss_r;
            boss_bul_r[j].hidden = false;
            boss_bul_r[j].x = boss.x+60;
            boss_bul_r[j].y = boss.y+boss.h/2;
        }








}
}

void game_draw(void) {
	if (active_scene == SCENE_MENU) {
		al_draw_bitmap(main_img_background, 0, 0, 0);
		al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Space Shooter");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "Press enter key to start");
		// [HACKATHON 3-5]
		// TODO: Draw settings images.
		// The settings icon should be located at (x, y, w, h) =
		// (SCREEN_W - 48, 10, 38, 38).
		// Change its image according to your mouse position.
		// Uncomment and fill in the code below.
		hp = 10;
		score = 0;
		defeat = 0;
		for(int k = 0;k<MAX_ENEMY;k++)
        {
            flag[k]=0;
        }
        bossflag = 0;
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W-48, 10, 38, 38))
			al_draw_bitmap(img_settings2, SCREEN_W-48, 10, 0);
		else
			al_draw_bitmap(img_settings, SCREEN_W-48, 10, 0);

	} else if (active_scene == SCENE_START) {

	    al_draw_bitmap(start_img_background, 0, 0, 0);
	    if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W-48, 10, 38, 38))
			al_draw_bitmap(img_settings2, SCREEN_W-48, 10, 0);
		else
			al_draw_bitmap(img_settings, SCREEN_W-48, 10, 0);
		int i;

		al_draw_textf(font_pirulen_16,al_map_rgb(0,0,0),20,SCREEN_H-40,0,"Your HP:%d",hp);
		al_draw_textf(font_pirulen_16,al_map_rgb(0,0,0),20,SCREEN_H-60,0,"your score:%d",score);
		al_draw_textf(font_pirulen_16,al_map_rgb(0,0,0),20,SCREEN_H-80,0,"defeated enemies:%d",defeat);
		//al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H - 30, ALLEGRO_ALIGN_CENTER,"score:%d",score);

		// [HACKATHON 2-9]
		// TODO: Draw all bullets in your bullet array.
		// Uncomment and fill in the code below.
		for (i = 0;i<MAX_BULLET;i++)
			draw_movable_object(bullets[i]);
		draw_movable_object(plane);
		for (i = 0; i < MAX_ENEMY; i++)
        {
            draw_movable_object(enemies[i]);
            al_draw_filled_rectangle(enemies[i].x-50,enemies[i].y-enemies[i].h/2-15,enemies[i].x+50-flag[i]*20,enemies[i].y-enemies[i].h/2-5,al_map_rgb(255,0,0));
            if(flag[i]!=5)
            {
                al_draw_rectangle(enemies[i].x-50,enemies[i].y-enemies[i].h/2-15,enemies[i].x+50,enemies[i].y-enemies[i].h/2-5,al_map_rgb(0,0,0),5);
            }

        }

        for(i=0;i<MAX_ENEMY;i++)
        {
            for(int j=0;j<MAX_BULLET;j++)
            {
                draw_movable_object(enebul[i][j]);
            }
        }

        if (hp<=0)
        {
            game_change_scene(SCENE_LOSE);
        }

        if(defeat == MAX_ENEMY)
        {
            game_change_scene(SCENE_BOSS);
        }
	}
	// [HACKATHON 3-9]
	// TODO: If active_scene is SCENE_SETTINGS.
	// Draw anything you want, or simply clear the display.


    else if (active_scene == SCENE_SETTINGS) {

            al_draw_bitmap(img_setting_bg, 0, 0, 0);
		    //al_clear_to_color(al_map_rgb(0, 0, 0));

/*        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W-60, 10, 50, 50))
			al_draw_bitmap(img_backicon2, SCREEN_W-60, 10, 0);
		else
			al_draw_bitmap(img_backicon1, SCREEN_W-60, 10, 0);*/

        if(pnt_in_multirect(mouse_x,mouse_y,300,500,135,60))
			al_draw_filled_rectangle(290,60,510,135,al_map_rgb(255,255,0));
        else
            al_draw_filled_rectangle(290,60,510,135,al_map_rgb(255,165,0));

        al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),300,85,0,"character");

        if(pnt_in_multirect(mouse_x,mouse_y,300,500,270,195))
			al_draw_filled_rectangle(300,195,500,270,al_map_rgb(255,255,0));
        else
            al_draw_filled_rectangle(300,195,500,270,al_map_rgb(255,165,0));

            al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),350,220,0,"help");

        if(pnt_in_multirect(mouse_x,mouse_y,300,500,405,330))
			al_draw_filled_rectangle(300,330,500,405,al_map_rgb(255,255,0));
        else
            al_draw_filled_rectangle(300,330,500,405,al_map_rgb(255,165,0));

        al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),350,355,0,"back");

        if(pnt_in_multirect(mouse_x,mouse_y,300,500,540,465))
			al_draw_filled_rectangle(300,465,500,540,al_map_rgb(255,255,0));
        else
            al_draw_filled_rectangle(300,465,500,540,al_map_rgb(255,165,0));

            al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),360,490,0,"exit");

	}
	else if(active_scene == SCENE_LOSE)
    {
        al_draw_bitmap(img_lose,0,0,0);
        al_draw_text(font_pirulen_32,al_map_rgb(220,20,60),SCREEN_W/2,100,ALLEGRO_ALIGN_CENTER,"You Lose!");
        al_draw_textf(font_pirulen_32,al_map_rgb(220,20,60),SCREEN_W/2,150,ALLEGRO_ALIGN_CENTER,"your score:%d",score);
        al_draw_textf(font_pirulen_32,al_map_rgb(220,20,60),SCREEN_W/2,200,ALLEGRO_ALIGN_CENTER,"defeated enemies:%d",defeat);

        //al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),SCREEN_W/2,SCREEN_H-200,ALLEGRO_ALIGN_CENTER,"Restart!");

        if (pnt_in_multirect(mouse_x, mouse_y, SCREEN_W/2-50, SCREEN_W/2+50, SCREEN_H-50, SCREEN_H-150))
        {
            al_draw_bitmap(img_restart, SCREEN_W/2-50,SCREEN_H-150, 0);
            al_draw_text(font_pirulen_24,al_map_rgb(255,255,255),SCREEN_W/2,SCREEN_H-200,ALLEGRO_ALIGN_CENTER,"Restart!");
        }

		else
        {
            al_draw_bitmap(img_restart1, SCREEN_W/2-50, SCREEN_H-150, 0);
            al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),SCREEN_W/2,SCREEN_H-200,ALLEGRO_ALIGN_CENTER,"Restart!");
        }


    }
    else if(active_scene == SCENE_BOSS)
    {
        al_draw_bitmap(img_boss_bg,0,0,0);
        al_draw_textf(font_pirulen_16,al_map_rgb(255,255,255),20,SCREEN_H-40,0,"Your HP:%d",hp);
		al_draw_textf(font_pirulen_16,al_map_rgb(255,255,255),20,SCREEN_H-60,0,"your score:%d",score);
		al_draw_textf(font_pirulen_16,al_map_rgb(255,255,255),20,SCREEN_H-80,0,"defeated enemies:%d",defeat);
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W-48, 10, 38, 38))
			al_draw_bitmap(img_settings2, SCREEN_W-48, 10, 0);
		else
			al_draw_bitmap(img_settings, SCREEN_W-48, 10, 0);

		int i;

		//al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H - 30, ALLEGRO_ALIGN_CENTER,"score:%d",score);

		// [HACKATHON 2-9]
		// TODO: Draw all bullets in your bullet array.
		// Uncomment and fill in the code below.

		draw_movable_object(plane);
		draw_movable_object(boss);
        al_draw_filled_rectangle(boss.x-50,boss.y-boss.h/2-15,boss.x+50-bossflag*5,boss.y-boss.h/2-5,al_map_rgb(255,0,0));
        if(bossflag!=20)
        {
            al_draw_rectangle(boss.x-50,boss.y-boss.h/2-15,boss.x+50,boss.y-boss.h/2-5,al_map_rgb(0,0,0),3);
        }

        for(i = 0;i<MAX_BULLET;i++)
        {
            draw_movable_object(bullets[i]);
        }
        for(i = 0;i<MAX_BOSS_BULLET;i++)
        {
            draw_movable_object(boss_bul_l[i]);
        }
        for(i=0;i<MAX_BOSS_BULLET;i++)
        {
            draw_movable_object(boss_bul_r[i]);
        }

        if (hp<=0)
        {
            game_change_scene(SCENE_LOSE);
        }

        if(boss.hidden == true)
        {
            game_change_scene(SCENE_WIN);
        }

        /*for (i = 0;i<MAX_BULLET;i++)
			draw_movable_object(bullets[i]);*/
    }
    else if(active_scene==SCENE_WIN)
    {
        al_draw_bitmap(img_win,0,0,0);
        al_draw_text(font_pirulen_32,al_map_rgb(102,225,0),SCREEN_W/2,100,ALLEGRO_ALIGN_CENTER,"You WIN!");
        al_draw_textf(font_pirulen_32,al_map_rgb(102,225,0),SCREEN_W/2,150,ALLEGRO_ALIGN_CENTER,"your score:%d",score);
        al_draw_textf(font_pirulen_32,al_map_rgb(102,225,0),SCREEN_W/2,200,ALLEGRO_ALIGN_CENTER,"defeated enemies:%d",defeat);

        al_draw_text(font_pirulen_24,al_map_rgb(255,255,255),SCREEN_W/2,SCREEN_H-200,ALLEGRO_ALIGN_CENTER,"play again!");

        if (pnt_in_multirect(mouse_x, mouse_y, SCREEN_W/2-50, SCREEN_W/2+50, SCREEN_H-50, SCREEN_H-150))
        {
            al_draw_bitmap(img_restart, SCREEN_W/2-50,SCREEN_H-150, 0);
            al_draw_text(font_pirulen_24,al_map_rgb(255,255,255),SCREEN_W/2,SCREEN_H-200,ALLEGRO_ALIGN_CENTER,"play again!");
        }

		else
        {
            al_draw_text(font_pirulen_24,al_map_rgb(0,255,255),SCREEN_W/2,SCREEN_H-200,ALLEGRO_ALIGN_CENTER,"play again!");
            al_draw_bitmap(img_restart1, SCREEN_W/2-50, SCREEN_H-150, 0);
        }


    }
    else if(active_scene ==SCENE_CHAR)
    {
        al_draw_bitmap(img_char_bg,0,0,0);
        al_draw_text(font_pirulen_32,al_map_rgb(0,0,0),100,60,0,"choose your character !");

        al_draw_bitmap(img_apachar,100,200,0);
        al_draw_bitmap(img_f22char,325,200,0);
        al_draw_bitmap(img_spacechar,550,200,0);

        if(pnt_in_multirect(mouse_x,mouse_y,100,250,510,410))
            al_draw_filled_rectangle(100,410,250,485,al_map_rgb(128,128,128));
        else
            al_draw_filled_rectangle(100,410,250,485,al_map_rgb(0,0,0));

        al_draw_text(font_pirulen_24,al_map_rgb(255,255,255),110,435,0,"apache");

        if(pnt_in_multirect(mouse_x,mouse_y,325,475,510,410))
            al_draw_filled_rectangle(325,410,475,485,al_map_rgb(128,128,128));
        else
            al_draw_filled_rectangle(325,410,475,485,al_map_rgb(0,0,0));
        al_draw_text(font_pirulen_24,al_map_rgb(255,255,255),365,435,0,"F-22");

        if(pnt_in_multirect(mouse_x,mouse_y,550,700,510,410))
            al_draw_filled_rectangle(550,410,700,485,al_map_rgb(128,128,128));
        else
            al_draw_filled_rectangle(550,410,700,485,al_map_rgb(0,0,0));
        al_draw_text(font_pirulen_24,al_map_rgb(255,255,255),570,420,0,"space");
        al_draw_text(font_pirulen_24,al_map_rgb(255,255,255),570,450,0,"craft");
    }
    else if(active_scene == SCENE_HELP)
    {
        al_draw_bitmap(img_help_bg,0,0,0);

        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W-60, 10, 50, 50))
			al_draw_bitmap(img_backicon2, SCREEN_W-60, 10, 0);
		else
			al_draw_bitmap(img_backicon1, SCREEN_W-60, 10, 0);

    }
    else if(active_scene == SCENE_HELP_1)
    {
        al_draw_bitmap(img_help_bg,0,0,0);

        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W-60, 10, 50, 50))
			al_draw_bitmap(img_backicon2, SCREEN_W-60, 10, 0);
		else
			al_draw_bitmap(img_backicon1, SCREEN_W-60, 10, 0);

    }
    else if(active_scene == SCENE_SETTINGS2)
    {
        al_draw_bitmap(img_setting_bg,0,0,0);
                if(pnt_in_multirect(mouse_x,mouse_y,300,500,135,60))
			al_draw_filled_rectangle(290,60,510,135,al_map_rgb(255,255,0));
        else
            al_draw_filled_rectangle(290,60,510,135,al_map_rgb(255,165,0));

        al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),330,85,0,"RESTART");

        if(pnt_in_multirect(mouse_x,mouse_y,300,500,270,195))
			al_draw_filled_rectangle(300,195,500,270,al_map_rgb(255,255,0));
        else
            al_draw_filled_rectangle(300,195,500,270,al_map_rgb(255,165,0));

            al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),350,220,0,"help");

        if(pnt_in_multirect(mouse_x,mouse_y,300,500,405,330))
			al_draw_filled_rectangle(300,330,500,405,al_map_rgb(255,255,0));
        else
            al_draw_filled_rectangle(300,330,500,405,al_map_rgb(255,165,0));

        al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),350,355,0,"menu");

        if(pnt_in_multirect(mouse_x,mouse_y,300,500,540,465))
			al_draw_filled_rectangle(300,465,500,540,al_map_rgb(255,255,0));
        else
            al_draw_filled_rectangle(300,465,500,540,al_map_rgb(255,165,0));

            al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),360,490,0,"exit");
    }

	al_flip_display();
}

void game_destroy(void) {
	// Destroy everything you have created.
	// Free the memories allocated by malloc or allegro functions.
	// Destroy shared resources.
	al_destroy_font(font_pirulen_32);
	al_destroy_font(font_pirulen_24);
	al_destroy_font(font_pirulen_16);

	/* Menu Scene resources*/
	al_destroy_bitmap(main_img_background);
	al_destroy_sample(main_bgm);
	al_destroy_sample(boss_bgm);
	// [HACKATHON 3-6]
	// TODO: Destroy the 2 settings images.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_settings);
	al_destroy_bitmap(img_settings2);
	al_destroy_bitmap(img_apachar);
	al_destroy_bitmap(img_f22char);
	al_destroy_bitmap(img_spacechar);
	al_destroy_bitmap(img_char_bg);

	/* Start Scene resources*/
	al_destroy_bitmap(start_img_background);
	al_destroy_bitmap(start_img_plane);
	al_destroy_bitmap(start_img_enemy);
	al_destroy_sample(start_bgm);
	al_destroy_sample(lose_bgm);
	al_destroy_sample(win_bgm);
	al_destroy_sample(gun_bgm);
	al_destroy_sample(setting_bgm);
	al_destroy_bitmap(img_boss_bg);
	al_destroy_bitmap(img_boss);
	al_destroy_bitmap(img_boss_bul_l);
	al_destroy_bitmap(img_boss_bul_r);
	al_destroy_bitmap(img_win);
	al_destroy_bitmap(img_apa);
	al_destroy_bitmap(img_space);
	al_destroy_bitmap(img_f22);

	// [HACKATHON 2-10]
	// TODO: Destroy your bullet image.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_bullet);
	al_destroy_bitmap(img_enebul);
	al_destroy_bitmap(img_lose);
	al_destroy_bitmap(img_restart1);
	al_destroy_bitmap(img_restart);
	al_destroy_bitmap(img_setting_bg);
	al_destroy_bitmap(img_boss_bg);

	al_destroy_timer(game_update_timer);
	al_destroy_event_queue(game_event_queue);
	al_destroy_display(game_display);
	free(mouse_state);
}

void game_change_scene(int next_scene) {
	game_log("Change scene from %d to %d", active_scene, next_scene);
	// TODO: Destroy resources initialized when creating scene.
	if (active_scene == SCENE_MENU) {
		al_stop_sample(&main_bgm_id);
		game_log("stop audio (bgm)");
	} else if (active_scene == SCENE_START) {
		al_stop_sample(&start_bgm_id);
		game_log("stop audio (bgm)");
	}else if(active_scene == SCENE_BOSS){
	    al_stop_sample(&boss_bgm_id);
	    game_log("stop audio (bgm)");
	}else if(active_scene == SCENE_SETTINGS&&next_scene!=SCENE_CHAR)
	{
	    al_stop_sample(&setting_bgm_id);
	    game_log("stop audio (bgm)");
	}else if(active_scene == SCENE_CHAR)
	{
	    al_stop_sample(&setting_bgm_id);
	    game_log("stop audio (bgm)");
	}else if(active_scene == SCENE_HELP)
	{
	    al_stop_sample(&setting_bgm_id);
	    game_log("stop audio (bgm)");
	}
	else if(active_scene == SCENE_HELP_1)
	{
	    al_stop_sample(&setting_bgm_id);
	    game_log("stop audio (bgm)");
	}
	else if(active_scene == SCENE_WIN)
	{
	    al_stop_sample(&win_bgm_id);
	    game_log("stop audio (bgm)");
	}else if(active_scene == SCENE_LOSE)
	{
	    al_stop_sample(&lose_bgm_id);
	    game_log("stop audio (bgm)");
	}
	else if(active_scene == SCENE_SETTINGS2)
    {
        al_stop_sample(&setting_bgm_id);
        game_log("stop audio (bgm)");
    }
	active_scene = next_scene;
	// TODO: Allocate resources before entering scene.
	if (active_scene == SCENE_MENU) {
		if (!al_play_sample(main_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
			game_abort("failed to play audio (bgm)");
	} else if (active_scene == SCENE_START) {
		int i;
		if(cha ==1 )
            plane.img = img_apa;
        else if(cha==2)
            plane.img = img_f22;
        else if(cha==3)
            plane.img = img_space;
		plane.x = 400;
		plane.y = 500;
		plane.w = al_get_bitmap_width(plane.img);
        plane.h = al_get_bitmap_height(plane.img);

        enemies[-1].vx = 1;
        enemies[-1].vy = 1.5;
		for (i = 0; i < MAX_ENEMY; i++) {
			enemies[i].img = start_img_enemy;
			enemies[i].w = al_get_bitmap_width(start_img_enemy);
			//printf("enemies %f \n",enemies[i].w);
			enemies[i].h = al_get_bitmap_height(start_img_enemy);
			enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
			enemies[i].y = 80;
			enemies[i].hidden = false;
			enemies[i].vy = enemies[i-1].vy-0.25;
			enemies[i].vx = enemies[i-1].vx+0.25;
		}
		// [HACKATHON 2-6]
		// TODO: Initialize bullets.
		// For each bullets in array, set their w and h to the size of
		// the image, and set their img to bullet image, hidden to true,
		// (vx, vy) to (0, -3).
		// Uncomment and fill in the code below.
		/*enemies[-1].vx = 1;
        enemies[-1].vy = 1.5;
        int m;
        for(m = 0;m<MAX_ENEMY;m++)
        {
            enemies[m].w = al_get_bitmap()

        }*/
        int j;
        for(i = 0;i<MAX_ENEMY;i++)
        {
            for (j=0;j<MAX_BULLET;j++)
            {
                enebul[i][j].w = al_get_bitmap_width(img_enebul);
                //printf("%f \n",enebul[i][j].w);
			    enebul[i][j].h = al_get_bitmap_height(img_enebul);
			    enebul[i][j].img = img_enebul;
			    enebul[i][j].vx = 0;
			    enebul[i][j].vy = 3;
			    enebul[i][j].hidden = true;
		    }
        }


		for (i=0;i<MAX_BULLET;i++) {
			bullets[i].w = al_get_bitmap_width(img_bullet);
			bullets[i].h = al_get_bitmap_height(img_bullet);
			bullets[i].img = img_bullet;
			bullets[i].vx = 0;
			bullets[i].vy = -3;
			bullets[i].hidden = true;
		}
		if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
			game_abort("failed to play audio (bgm)");


	}

	else if (active_scene == SCENE_BOSS)
    {
        if(!al_play_sample(boss_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &boss_bgm_id))
            game_abort("failed to play audio (bgm)");

		if(cha ==1 )
            plane.img = img_apa;
        else if(cha==2)
            plane.img = img_f22;
        else if(cha==3)
            plane.img = img_space;
		plane.x = 400;
		plane.y = 500;
		plane.w = al_get_bitmap_width(plane.img);
        plane.h = al_get_bitmap_height(plane.img);

        boss.img = img_boss;
        boss.x =  boss.w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - boss.w);
        boss.y =  80;
        boss.w = al_get_bitmap_width(img_boss);
        boss.h = al_get_bitmap_height(img_boss);
        boss.hidden = false;
        boss.vx =3;
        boss.vy = 2;
        int j;

        for (j=0;j<MAX_BOSS_BULLET;j++)
            {
                boss_bul_l[j].w = al_get_bitmap_width(img_boss_bul_l);
                //printf("%f \n",enebul[i][j].w);
			    boss_bul_l[j].h = al_get_bitmap_height(img_boss_bul_l);
			    boss_bul_l[j].img = img_boss_bul_l;
			    boss_bul_l[j].vx = 0;
			    boss_bul_l[j].vy = 3;
			    boss_bul_l[j].hidden = true;
		    }

        for (j=0;j<MAX_BOSS_BULLET;j++)
            {
                boss_bul_r[j].w = al_get_bitmap_width(img_boss_bul_r);
                //printf("%f \n",enebul[i][j].w);
			    boss_bul_r[j].h = al_get_bitmap_height(img_boss_bul_r);
			    boss_bul_r[j].img = img_boss_bul_r;
			    boss_bul_r[j].vx = 0;
			    boss_bul_r[j].vy = 3;
			    boss_bul_r[j].hidden = true;
		    }


		    int i;

		for (i=0;i<MAX_BULLET;i++) {
			bullets[i].w = al_get_bitmap_width(img_bullet);
			bullets[i].h = al_get_bitmap_height(img_bullet);
			bullets[i].img = img_bullet;
			bullets[i].vx = 0;
			bullets[i].vy = -3;
			bullets[i].hidden = true;
		}


    }
    if(active_scene == SCENE_SETTINGS)
    {
        if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
    }
    if(active_scene == SCENE_HELP)
    {
        if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
    }
    if(active_scene == SCENE_HELP_1)
    {
        if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
    }
    if(active_scene == SCENE_WIN)
    {
        if (!al_play_sample(win_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &win_bgm_id))
			game_abort("failed to play audio (bgm)");
    }
    if(active_scene == SCENE_LOSE)
    {
        if (!al_play_sample(lose_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &lose_bgm_id))
			game_abort("failed to play audio (bgm)");
    }
    if(active_scene == SCENE_SETTINGS2)
    {
        if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
    }

}

void on_key_down(int keycode) {
	if (active_scene == SCENE_MENU) {
		if (keycode == ALLEGRO_KEY_ENTER)
			game_change_scene(SCENE_START);
	}
}

void on_mouse_down(int btn, int x, int y) {
	// [HACKATHON 3-8]
	// TODO: When settings clicked, switch to settings scene.
	// Uncomment and fill in the code below.

	if (active_scene == SCENE_MENU) {
		if (btn == al_get_mouse_num_buttons()-15) {
			if (pnt_in_rect(x, y, SCREEN_W-48, 10, 38, 38))
				game_change_scene(SCENE_SETTINGS);
		}
	}
	else if (active_scene == SCENE_START) {
		if (btn == al_get_mouse_num_buttons()-15) {
			if (pnt_in_rect(x, y, SCREEN_W-48, 10, 38, 38))
				game_change_scene(SCENE_SETTINGS2);
		}
	}
	else if (active_scene == SCENE_BOSS) {
		if (btn == al_get_mouse_num_buttons()-15) {
			if (pnt_in_rect(x, y, SCREEN_W-48, 10, 38, 38))
				game_change_scene(SCENE_SETTINGS2);
		}
	}

	else if (active_scene == SCENE_SETTINGS) {
		if (btn == al_get_mouse_num_buttons()-15) {
			if (pnt_in_multirect(x,y,300,500,540,465))
				   done = true;
		}

		if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(x,y,290,510,135,60))
            {
                game_change_scene(SCENE_CHAR);
            }
        }
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(x,y,300,500,405,330))
            {
                game_change_scene(SCENE_MENU);
            }
        }
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(x,y,300,500,270,195))
            {
                game_change_scene(SCENE_HELP);
            }
        }

	}
		else if (active_scene == SCENE_SETTINGS2) {
		if (btn == al_get_mouse_num_buttons()-15) {
			if (pnt_in_multirect(x,y,300,500,540,465))
				   done = true;
		}

		if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(x,y,290,510,135,60))
            {
                hp = 10;
		        score = 0;
		        defeat = 0;
		        for(int k = 0;k<MAX_ENEMY;k++)
                {
                flag[k]=0;
                }
                bossflag = 0;
                game_change_scene(SCENE_START);
            }
        }
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(x,y,300,500,405,330))
            {
                game_change_scene(SCENE_MENU);
            }
        }
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(x,y,300,500,270,195))
            {
                game_change_scene(SCENE_HELP_1);
            }
        }

	}
	else if(active_scene == SCENE_LOSE)
    {
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(mouse_x, mouse_y, SCREEN_W/2-50, SCREEN_W/2+50, SCREEN_H-50, SCREEN_H-150))
                game_change_scene(SCENE_MENU);
        }
    }
    else if(active_scene == SCENE_WIN)
    {
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(mouse_x, mouse_y, SCREEN_W/2-50, SCREEN_W/2+50, SCREEN_H-50, SCREEN_H-150))
                game_change_scene(SCENE_MENU);
        }
    }
    else if(active_scene == SCENE_CHAR)
    {
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_multirect(mouse_x,mouse_y,100,250,510,410))
                cha = 1;
            else if(pnt_in_multirect(mouse_x,mouse_y,325,475,510,410))
                cha = 2;
            else if(pnt_in_multirect(mouse_x,mouse_y,550,700,510,410))
                cha = 3;

            game_change_scene(SCENE_MENU);
        }

    }
    else if(active_scene == SCENE_HELP)
    {
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W-60, 10, 50, 50))
                game_change_scene(SCENE_SETTINGS);
        }
    }
    else if(active_scene == SCENE_HELP_1)
    {
        if(btn == al_get_mouse_num_buttons()-15)
        {
            if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W-60, 10, 50, 50))
                game_change_scene(SCENE_SETTINGS2);
        }
    }


}

void draw_movable_object(MovableObject obj) {
	if (obj.hidden)
		return;
	al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
	ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
	if (!loaded_bmp)
		game_abort("failed to load image: %s", filename);
	ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
	ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

	if (!resized_bmp)
		game_abort("failed to create bitmap when creating resized image: %s", filename);
	al_set_target_bitmap(resized_bmp);
	al_draw_scaled_bitmap(loaded_bmp, 0, 0,
		al_get_bitmap_width(loaded_bmp),
		al_get_bitmap_height(loaded_bmp),
		0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	game_log("resized image: %s", filename);

	return resized_bmp;
}

// [HACKATHON 3-3]
// TODO: Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
// Uncomment and fill in the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
    if(px<SCREEN_W-y&&px>x&&py>y&&py<h+y)
	    return true;
	else
        return false;
}
bool pnt_in_multirect(int px,int py,int xl,int xr,int yu,int yd)
{
    if(px<xr&&px>xl&&py<yu&&py>yd)
        return true;
    else
        return false;
}

// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the fALLEunctions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
	fprintf(stderr, "error occured, exiting after 2 secs");
	// Wait 2 secs before exiting.
	al_rest(2);
	// Force exit program.
	exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
	static bool clear_file = true;
	vprintf(format, arg);
	printf("\n");
	// Write log to file for later debugging.
	FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
	if (pFile) {
		vfprintf(pFile, format, arg);
		fprintf(pFile, "\n");
		fclose(pFile);
	}
	clear_file = false;
#endif
}
