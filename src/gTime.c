#include <pebble.h>
#include "gtwin.h"
#include "gTime.h"

static GBitmap *batt_state_unload;
static GBitmap *batt_state_load;
static GBitmap *bt_state_con;
static GBitmap *bt_state_dis;
static int8_t tofd = -1; // tofd als Zahl fuer spaeter
static bool first_run = true; // Zur Initialisierung, am Ende von init auf false setzen
static char temperatur[6];

// h = 168 | w = 144


void make_temp(int temp) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Empfangene Temperatur: %d", temp);
  snprintf(temperatur, 4, "%d", temp);
  strcat(temperatur, "°C");
  update_left_info(temperatur);
}

// Handler fuer die Kommunikation mit dem Smartphone
static void inbox_received_cb(DictionaryIterator *iterator, void *context) {
  Tuple *t = dict_read_first(iterator);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Daten empfangen!");
  while(t != NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY: %d, VALUE: %d", (int)t->key, (int)t->value->int32);
    switch(t->key) {
      case KEY_TEMPERATURE:
        make_temp((int)t->value->int32);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key nicht erkannt! %d", (int)t->key);
        break;
    }
    t = dict_read_next(iterator);
  }
} 

static void inbox_dropped_cb(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Nachricht wurde verworfen!");
}

static void outbox_failed_cb(DictionaryIterator *iterator, AppMessageResult reason,void *context) {
  
}

static void outbox_sent_cb(DictionaryIterator *iterator, void *context) {
  
}

void make_date(char **datum){
  uint16_t wday;
  uint16_t mon;
  
  wday = atoi(datum[1]);
  mon = atoi(datum[2]);

  app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Datum %d %d", wday, mon);

  // Hier wird mit Absicht kein strncpy benutzt, da datum[] an der Stelle
  // zu wenig Zeichen enthaelt. Spaeter werden 11 Zeichen benoetigt, wie reserviert wurden!
  strcpy(datum[1], weekday[wday]);
  strcpy(datum[2], month[mon - 1]);
   
}

void handle_battery(BatteryChargeState batt) {
  static char batt_text[] = "100% ";

  if(batt.is_charging){
    snprintf(batt_text, sizeof(batt_text), "%d%%", batt.charge_percent);
    update_right_info_img(batt_state_load);
  }
  else {
    snprintf(batt_text, sizeof(batt_text), "%d%%", batt.charge_percent);
    update_right_info_img(batt_state_unload);
  }

  update_right_info(batt_text);

}

void handle_connection(bool con) {
  
  if(con) {
    update_left_info_img(bt_state_con);
    if(!first_run) {
      vibes_double_pulse();
    }
  }
  else {
    update_left_info_img(bt_state_dis);
    if(!first_run) {
      vibes_long_pulse();
    }
  }
}

void handle_second_tick(struct tm *time_tick, TimeUnits units_changed) {
  static char sec_text[] = "00";
  if(!time_tick) {
    time_t now = time(NULL);
    time_tick = localtime(&now);
  }
  
  strftime(sec_text, sizeof(sec_text), "%S", time_tick);
  update_sec(sec_text);
}

void handle_minute_tick(struct tm *time_tick, TimeUnits units_changed) {
  static char time_text[] = "00:00";
  static char sec_text[] = "00";
  static char date_text[] = "00. Xxxxxxxxx";
  static char week_text[] = "KW 00";
  static char d[] = "00";
  static char wday[11];
  static char week_format[] = "KW %V";
  char *time_format;
  char **datum; 
  uint16_t i = 0;
  uint16_t t = 0;

  if(!time_tick) {
    time_t now = time(NULL);
    time_tick = localtime(&now);
  }
 
  if(clock_is_24h_style()) {
    time_format = "%H:%M";
  }
  else {
    time_format = "%I:%M";
  }
  
  strftime(sec_text, sizeof(sec_text), "%S", time_tick);
  update_sec(sec_text);

  strftime(time_text, sizeof(time_text), time_format, time_tick);
  update_time(time_text);

  strftime(d, sizeof(d), "%d", time_tick);
  t = atoi(d);  

  if(tofd != t) { // only if the day changes, then we re-read the date

    tofd = t;
  
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Speicher holen!");

    datum = (char **)malloc(3 * sizeof(char *));
    for(i = 0; i < 3; i++){
      datum[i] = (char *)malloc(11 * sizeof(char));
    }

    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Datum auslesen!");

    // Datum auslesen
    strftime(date_text, sizeof(date_text), "%d%w%m", time_tick); // Tag im Monat (01-31) + Tag in der Woche (00-06) So-Sa / Monat (01-12)
    
    // Woche auslesen
    strftime(week_text, sizeof(week_text), week_format, time_tick); // Woche nach ISO8601 
    update_week(week_text);
  
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Datum gelesen! %s", date_text); //ddWmm

    memcpy(datum[0], date_text, 2);
    memcpy(datum[1], date_text+2, 1);
    memcpy(datum[2], date_text+3, 2);
  
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Datum bauen! %s / %s / %s", datum[0], datum[1], datum[2]);
  
    make_date(datum);
  
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Datum gebaut %s %s %s!", datum[0], datum[1], datum[2]);
    
    strncpy(date_text, datum[0], sizeof(date_text));  // Tag im Monat
    strncat(date_text, ". ", sizeof(date_text));
    strncat(date_text, datum[2], sizeof(date_text));  // Monat in Worten
    strncpy(wday, datum[1], sizeof(wday));       // Wochentag
  
    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Datum: %s, %s!", wday, date_text); 
  
    update_wday(wday);
    update_date(date_text);

    app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "Speicher freigeben "); 
    
    free(datum);
  }
}

void init(void) {
  app_message_register_inbox_received(inbox_received_cb);
  app_message_register_inbox_dropped(inbox_dropped_cb);
  app_message_register_outbox_failed(outbox_failed_cb);
  app_message_register_outbox_sent(outbox_sent_cb);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  show_gtwin();
  
  // Icon fuer den Akkuzustand laden
  batt_state_load = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_UP);
  batt_state_unload = gbitmap_create_with_resource(RESOURCE_ID_IMG_ARROW_DOWN);
  
  // Icon fuer den Bluetooth Verbindungszustand
  bt_state_con = gbitmap_create_with_resource(RESOURCE_ID_IMG_BT_CON);
  bt_state_dis = gbitmap_create_with_resource(RESOURCE_ID_IMG_BT_DIS);
  
  tick_timer_service_subscribe(SECOND_UNIT, handle_minute_tick);
  //tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_connection);
  handle_minute_tick(NULL, MINUTE_UNIT);
  //handle_second_tick(NULL, SECOND_UNIT);
  handle_battery(battery_state_service_peek());
  handle_connection(bluetooth_connection_service_peek());
  
  first_run = false;
}

void deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  
  gbitmap_destroy(batt_state_load);
  gbitmap_destroy(batt_state_unload);
  gbitmap_destroy(bt_state_con);
  gbitmap_destroy(bt_state_dis);
  hide_gtwin();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}