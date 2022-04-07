#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_mid_layer;
static TextLayer *s_up_layer;
static TextLayer *s_low_layer;

static void update_time() {
  time_t epochtime = time(NULL);
  struct tm *tick_time = localtime(&epochtime);
  time_t alku = 1649257200; // <-----------------------------------------
  int erotus = (epochtime - alku) / (24 * 60 * 60);
  int quiet_time_status = quiet_time_is_active();
  
  static char s_buffer[5];
  snprintf(s_buffer, sizeof(s_buffer), "%d", erotus);
  text_layer_set_text(s_mid_layer, s_buffer);
  
  static char s_buffer2[3];
  strftime(s_buffer2, sizeof(s_buffer2), "%H", tick_time);
  text_layer_set_text(s_low_layer, s_buffer2);

  if (quiet_time_status == 1) {
    text_layer_set_background_color(s_mid_layer, GColorBlack);
    text_layer_set_background_color(s_up_layer, GColorBlack);
    text_layer_set_background_color(s_low_layer, GColorBlack);
    text_layer_set_text_color(s_mid_layer, GColorGreen);
    text_layer_set_text_color(s_up_layer, GColorGreen);
    text_layer_set_text_color(s_low_layer, GColorBlack);
  } else {
    text_layer_set_background_color(s_mid_layer, GColorFromRGB(200, 200, 255));
    text_layer_set_background_color(s_up_layer, GColorFromRGB(200, 200, 255));
    text_layer_set_background_color(s_low_layer, GColorFromRGB(200, 200, 255));
    text_layer_set_text_color(s_mid_layer, GColorOxfordBlue);
    text_layer_set_text_color(s_up_layer, GColorOxfordBlue);
    text_layer_set_text_color(s_low_layer, GColorOxfordBlue);
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {  
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_up_layer = text_layer_create(
      GRect(0, 0, bounds.size.w, 52));
  s_mid_layer = text_layer_create(
      GRect(0, 52, bounds.size.w, 98));
  s_low_layer = text_layer_create(
      GRect(0, 52+98, bounds.size.w, bounds.size.h));

  text_layer_set_background_color(s_up_layer, GColorWhite);
  text_layer_set_text_color(s_up_layer, GColorBlack);
  text_layer_set_text(s_up_layer, "FooBar");
  text_layer_set_font(s_up_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_up_layer, GTextAlignmentCenter);
  
  text_layer_set_background_color(s_mid_layer, GColorWhite);
  text_layer_set_text_color(s_mid_layer, GColorBlack);
  text_layer_set_text(s_mid_layer, " ");
  text_layer_set_font(s_mid_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(s_mid_layer, GTextAlignmentCenter);
  
  text_layer_set_background_color(s_low_layer, GColorWhite);
  text_layer_set_text_color(s_low_layer, GColorBlack);
  text_layer_set_text(s_low_layer, "FooBar");
  text_layer_set_font(s_low_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_low_layer, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(s_up_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_mid_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_low_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_up_layer);
  text_layer_destroy(s_mid_layer);
  text_layer_destroy(s_low_layer);
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
