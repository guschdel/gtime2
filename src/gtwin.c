#include "gtwin.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_condensed_21;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_gothic_18;
static GFont s_res_gothic_14;
static TextLayer *wday_layer;
static TextLayer *date_layer;
static TextLayer *time_layer;
static TextLayer *week_layer;
static TextLayer *right_info_layer;
static BitmapLayer *img_left_info_layer;
static BitmapLayer *img_right_info_layer;
static TextLayer *sec_layer;
static TextLayer *left_info_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  // wday_layer
  wday_layer = text_layer_create(GRect(0, 0, 144, 30));
  text_layer_set_background_color(wday_layer, GColorClear);
  text_layer_set_text_color(wday_layer, GColorWhite);
  text_layer_set_text(wday_layer, "Sonntag");
  text_layer_set_text_alignment(wday_layer, GTextAlignmentCenter);
  text_layer_set_font(wday_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)wday_layer);
  
  // date_layer
  date_layer = text_layer_create(GRect(0, 30, 144, 30));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text(date_layer, "23. Dezember");
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)date_layer);
  
  // time_layer
  time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text(time_layer, "23:59");
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_layer);
  
  // week_layer
  week_layer = text_layer_create(GRect(0, 122, 144, 30));
  text_layer_set_background_color(week_layer, GColorClear);
  text_layer_set_text_color(week_layer, GColorWhite);
  text_layer_set_text(week_layer, "KW 52");
  text_layer_set_text_alignment(week_layer, GTextAlignmentCenter);
  text_layer_set_font(week_layer, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)week_layer);
  
  // right_info_layer
  right_info_layer = text_layer_create(GRect(72, 148, 52, 20));
  text_layer_set_background_color(right_info_layer, GColorClear);
  text_layer_set_text_color(right_info_layer, GColorWhite);
  text_layer_set_text(right_info_layer, "Info recht");
  text_layer_set_text_alignment(right_info_layer, GTextAlignmentRight);
  text_layer_set_font(right_info_layer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)right_info_layer);
  
  // img_left_info_layer
  img_left_info_layer = bitmap_layer_create(GRect(0, 148, 20, 20));
  layer_add_child(window_get_root_layer(s_window), (Layer *)img_left_info_layer);
  
  // img_right_info_layer
  img_right_info_layer = bitmap_layer_create(GRect(124, 148, 20, 20));
  layer_add_child(window_get_root_layer(s_window), (Layer *)img_right_info_layer);
  
  // sec_layer
  sec_layer = text_layer_create(GRect(0, 105, 144, 20));
  text_layer_set_background_color(sec_layer, GColorClear);
  text_layer_set_text_color(sec_layer, GColorWhite);
  text_layer_set_text(sec_layer, "59");
  text_layer_set_text_alignment(sec_layer, GTextAlignmentCenter);
  text_layer_set_font(sec_layer, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)sec_layer);
  
  // left_info_layer
  left_info_layer = text_layer_create(GRect(20, 148, 52, 20));
  text_layer_set_background_color(left_info_layer, GColorClear);
  text_layer_set_text_color(left_info_layer, GColorWhite);
  text_layer_set_text(left_info_layer, "--°C");
  text_layer_set_font(left_info_layer, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)left_info_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(wday_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(time_layer);
  text_layer_destroy(week_layer);
  text_layer_destroy(right_info_layer);
  bitmap_layer_destroy(img_left_info_layer);
  bitmap_layer_destroy(img_right_info_layer);
  text_layer_destroy(sec_layer);
  text_layer_destroy(left_info_layer);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_gtwin(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);

  app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "UI fertig");
}

void hide_gtwin(void) {
  window_stack_remove(s_window, true);
}

//app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Fenster gebaut!");

void update_right_info_img(void *bmp)
{
  bitmap_layer_set_bitmap(img_right_info_layer, (GBitmap*)bmp);
}

void update_left_info_img(void *bmp)
{
  bitmap_layer_set_bitmap(img_left_info_layer, (GBitmap*)bmp);
}

void update_right_info(char *text) {
  text_layer_set_text(right_info_layer, text);
}

void update_left_info(char *text) {
  text_layer_set_text(left_info_layer, text);
}

void update_wday(char *text) {
  text_layer_set_text(wday_layer, text);
}

void update_date(char *text) {
  text_layer_set_text(date_layer, text);
}

void update_time(char *text) {
  text_layer_set_text(time_layer, text);
}

void update_sec(char *text) {
  text_layer_set_text(sec_layer, text);
}

void update_week(char *text) {
  text_layer_set_text(week_layer, text);
}