#include <pebble.h>

Window *my_window;
TextLayer *s_time_layer;

void out_sent_handler(DictionaryIterator *sent, void *context) {
   // outgoing message was delivered
 }


 void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   // outgoing message failed
 }


 void in_received_handler(DictionaryIterator *received, void *context) {
   // incoming message received
 }


 void in_dropped_handler(AppMessageResult reason, void *context) {
   // incoming message dropped
 }

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
   text_layer_set_text(s_time_layer, "Retrieve");
  DictionaryIterator *iter;
 app_message_outbox_begin(&iter);
Tuplet value = TupletInteger(1, 42);
 dict_write_tuplet(iter, &value);
app_message_outbox_send();
}

void config_provider(void *context) {
   window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void handle_init(void) {
  my_window = window_create();

//  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(s_time_layer));
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();
  window_set_click_config_provider(my_window, config_provider);
  app_message_register_inbox_received(in_received_handler);
   app_message_register_inbox_dropped(in_dropped_handler);
   app_message_register_outbox_sent(out_sent_handler);
   app_message_register_outbox_failed(out_failed_handler);
const uint32_t inbound_size = 64;
   const uint32_t outbound_size = 64;
   app_message_open(inbound_size, outbound_size);
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  text_layer_destroy(s_time_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
