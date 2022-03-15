#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_teksti_layer;
static TextLayer *s_aika_layer;

static void update_time() {
  time_t epochtime = time(NULL);
  struct tm *tick_time = localtime(&epochtime);
  time_t alku = 1646964000; // <-----------------------------------------
  int erotus = (epochtime - alku) / (24 * 60 * 60);
  
  static char s_buffer[5];
  snprintf(s_buffer, sizeof(s_buffer), "%d", erotus);
  text_layer_set_text(s_time_layer, s_buffer);
  
  // Write the current hours into a buffer
  static char s_buffer2[3];
  strftime(s_buffer2, sizeof(s_buffer2), " ", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_aika_layer, s_buffer2);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {  
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, 52, bounds.size.w, bounds.size.h));
  
  // Tehdaan toinen TextLayer
  s_teksti_layer = text_layer_create(
      GRect(0, 0, bounds.size.w, 52));
  s_aika_layer = text_layer_create(
      GRect(0, 150, bounds.size.w, 52));


  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorWhite);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, " ");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Oman tekstilayerin setit
  text_layer_set_background_color(s_teksti_layer, GColorWhite);
  text_layer_set_text_color(s_teksti_layer, GColorBlack);
  text_layer_set_text(s_teksti_layer, "0.0");
  text_layer_set_font(s_teksti_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_teksti_layer, GTextAlignmentCenter);
  
  
  text_layer_set_background_color(s_aika_layer, GColorWhite);
  text_layer_set_text_color(s_aika_layer, GColorBlack);
  text_layer_set_text(s_aika_layer, "NN");
  text_layer_set_font(s_aika_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_aika_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_teksti_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_aika_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_teksti_layer);
  text_layer_destroy(s_aika_layer);
}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
