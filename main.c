#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "t.h"

#define lprintf(...) do {\
	fprintf(stderr, "\e[1m[%s:%i]\e[0m ", __FILE__, __LINE__);\
	fprintf(stderr, __VA_ARGS__);\
	fputc('\n', stderr);\
} while (0)

int SCREEN_W = 600;
int SCREEN_H = 400;

int program_ended = 0;

ALLEGRO_BITMAP* layout = NULL;
ALLEGRO_JOYSTICK* joystick = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;

void process_event(ALLEGRO_EVENT*);

int main() {
	lprintf("initializing");
	#define tryto(x) if(!(x)) {lprintf("failed to do " #x); return -1;}
	
	tryto(al_init());

	tryto(al_init_font_addon());
	tryto(al_init_primitives_addon());
	tryto(al_init_image_addon());

	tryto(al_install_keyboard());
	tryto(al_install_mouse());
	tryto(al_install_joystick());

	al_set_new_window_title("gcview");
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	tryto(display = al_create_display(SCREEN_W,SCREEN_H));

	SCREEN_W = al_get_display_width(display);
	SCREEN_H = al_get_display_height(display);

	tryto(event_queue = al_create_event_queue());

	layout = al_load_bitmap("layout.png");
	if (!layout) {
		ALLEGRO_PATH* path1 = al_get_standard_path(ALLEGRO_USER_DATA_PATH);
		ALLEGRO_PATH* path2 = al_create_path("layout.png");
		al_join_paths(path1,path2);
		const char* path = al_path_cstr(path1, '/');
		lprintf("%s",path);
		layout = al_load_bitmap(path);
		al_destroy_path(path1);
		al_destroy_path(path2);
	}
	font = al_create_builtin_font();

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	//int jscount = al_get_num_joysticks();
	joystick = al_get_joystick(0);
	/*for (int i = 0; i < al_get_joystick_num_sticks(joystick); i++)
		lprintf("%i: %i",i,al_get_joystick_num_axes(joystick,i));*/

	while (!program_ended) {
		ALLEGRO_EVENT ev;
		ALLEGRO_TIMEOUT timeout;
		al_init_timeout(&timeout, 1.0/60.0);
		int get_event = al_wait_for_event_until(event_queue, &ev, &timeout);
		if (get_event) process_event(&ev);
		while ((get_event = al_get_next_event(event_queue, &ev))) {
			if (get_event) process_event(&ev);
		}
		
		al_clear_to_color(al_map_rgb(5,5,5));
		if (!joystick) {
			al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2-20, SCREEN_H/2-4, 0, "no joy stick");
		} else {
			ALLEGRO_JOYSTICK_STATE st;
			al_get_joystick_state(joystick, &st);
			
			al_draw_tinted_bitmap(layout,al_map_rgb(10,10,10),0,0,0);

			int bcount = al_get_joystick_num_buttons(joystick);
			for (int i = 0; i < bcount; i++) {
				if (!button_valid(i)) continue;

				if (st.button[i]) {
					int x = buttons_places[i].x;
					int y = buttons_places[i].y;
					al_draw_bitmap_region(layout,x,y,102,89,x,y,0);
				}
			}

			int factor = 25;

			int jsx = 28+st.stick[0].axis[0]*factor;
			int jsy = 153+st.stick[0].axis[1]*factor;
			al_draw_bitmap_region(layout,28,153,88,88, jsx, jsy, 0);

			int cstickx = 370+st.stick[2].axis[1]*factor;
			int csticky = 313+st.stick[1].axis[0]*factor;
			al_draw_bitmap_region(layout,370,313,54,54, cstickx, csticky, 0);

			int lshoulder = (st.stick[1].axis[1]+.7)*factor;
			al_draw_filled_rectangle(12, 8, 4, 8+lshoulder, al_map_rgb(0,0,255));
			
			int rshoulder = (st.stick[2].axis[0]+.7)*factor;
			al_draw_filled_rectangle(SCREEN_W-12, 8, SCREEN_W-4, 8+rshoulder, al_map_rgb(0,0,255));
		}
		
		al_flip_display();
	}
	
	lprintf("shutdown");
	al_destroy_font(font);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
//	al_destroy_config(al64_config);

	lprintf("GOOD BYE");

	return 0;
}

void process_event(ALLEGRO_EVENT* ev) {
	switch (ev->type) {		
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			al_acknowledge_resize(display);
			SCREEN_W = al_get_display_width(display);
			SCREEN_H = al_get_display_height(display);
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			lprintf("close");
			program_ended = 1;
			break;

	/*	case ALLEGRO_EVENT_MOUSE_AXES:
			m_scroll = ev->mouse.dz;
*/
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (ev->keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE: 
					lprintf("close");
					program_ended = 1;
					break;
				case ALLEGRO_KEY_R:
					al_reconfigure_joysticks();
					joystick = al_get_joystick(0);
					break;
			}
			break;
	}
}
