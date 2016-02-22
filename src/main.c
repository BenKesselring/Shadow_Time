#include <pebble.h>
#include "effect_layer.h"
#include "main_utils.h"  
  
#define KEY_SHADOW_DIRECTION 0
#define KEY_H1_SHADOW_COLOR 1
#define KEY_H2_SHADOW_COLOR 2
#define KEY_M1_SHADOW_COLOR 3
#define KEY_M2_SHADOW_COLOR 4
#define KEY_BG_COLOR 5
#define KEY_TIME_COLOR 6  
#define KEY_DATE_COLOR 7
#define KEY_BAR_COLOR 8  
  
#define SHADOW_DIRECTION_TO_BOTTOM_RIGHT 1
#define SHADOW_DIRECTION_TO_BOTTOM_LEFT 2
#define SHADOW_DIRECTION_TO_TOP_LEFT 3
#define SHADOW_DIRECTION_TO_TOP_RIGHT 4  

Window *my_window;
TextLayer *text_layer_hours, *text_layer_minutes, *text_layer_date;
EffectLayer *h1, *h2, *m1, *m2;
EffectOffset oh1, oh2, om1, om2;
GColor time_color, bg_color, date_color, bar_color; 

char s_hours[3], s_minutes[3];

#ifndef PBL_ROUND
char s_date[] = "Fri, Apr 24 ";
#endif


#ifndef PBL_SDK_2
static void app_focus_changed(bool focused) {
  if (focused) { // on resuming focus - restore background
    layer_mark_dirty(effect_layer_get_layer(h1));
  }
}
#endif



  
// changes shadows direction
void direct_shadow(uint8_t direction) {
  
  Layer *layer;
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);
  
  #ifdef PBL_PLATFORM_BASALT
    uint8_t length = 90;
  #else // chalk shadow
    uint8_t length = 90;
  #endif
    
  switch (direction) {
    case SHADOW_DIRECTION_TO_BOTTOM_RIGHT:
      oh1.offset_x = length; oh1.offset_y = length;
      oh2.offset_x = length; oh2.offset_y = length;
      om1.offset_x = length; om1.offset_y = length;
      om2.offset_x = length; om2.offset_y = length;    
    
      layer = effect_layer_get_layer(h2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(m2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);

      #ifndef PBL_ROUND
        layer_set_frame(text_layer_get_layer(text_layer_date), GRect(0,-2 ,bounds.size.w, 23)); text_layer_set_text_alignment(text_layer_date, GTextAlignmentRight);
      #endif
    
      break;
    case SHADOW_DIRECTION_TO_BOTTOM_LEFT:
      oh1.offset_x = -length; oh1.offset_y = length;
      oh2.offset_x = -length; oh2.offset_y = length;
      om1.offset_x = -length; om1.offset_y = length;
      om2.offset_x = -length; om2.offset_y = length;    
    
      layer = effect_layer_get_layer(h1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(m1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      
      #ifndef PBL_ROUND
        layer_set_frame(text_layer_get_layer(text_layer_date), GRect(0,-2,bounds.size.w, 23)); text_layer_set_text_alignment(text_layer_date, GTextAlignmentLeft);
      #endif
        
      break;
    case SHADOW_DIRECTION_TO_TOP_LEFT:
      oh1.offset_x = -length; oh1.offset_y = -length;
      oh2.offset_x = -length; oh2.offset_y = -length;
      om1.offset_x = -length; om1.offset_y = -length;
      om2.offset_x = -length; om2.offset_y = -length;    
    
      layer = effect_layer_get_layer(m1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(h2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(h1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
    
      #ifndef PBL_ROUND
        layer_set_frame(text_layer_get_layer(text_layer_date), GRect(0,146,bounds.size.w,23)); text_layer_set_text_alignment(text_layer_date, GTextAlignmentLeft);
      #endif  
    
      break;
    case SHADOW_DIRECTION_TO_TOP_RIGHT:
      oh1.offset_x = length; oh1.offset_y = -length;
      oh2.offset_x = length; oh2.offset_y = -length;
      om1.offset_x = length; om1.offset_y = -length;
      om2.offset_x = length; om2.offset_y = -length;    
    
      layer = effect_layer_get_layer(h1); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(h2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
      layer = effect_layer_get_layer(m2); layer_remove_from_parent(layer); layer_add_child(window_layer, layer);
    
      #ifndef PBL_ROUND
        layer_set_frame(text_layer_get_layer(text_layer_date), GRect(0,146,bounds.size.w,23)); text_layer_set_text_alignment(text_layer_date, GTextAlignmentRight);
      #endif  
      
      break;
  }  
    
}  
  
  
// handle configuration change
static void in_recv_handler(DictionaryIterator *iterator, void *context) {
  Tuple *t = dict_read_first(iterator);

  while (t)  {
    
    switch(t->key)    {

      case KEY_H1_SHADOW_COLOR:
         persist_write_int(KEY_H1_SHADOW_COLOR, t->value->uint8);
         oh1.offset_color = (GColor){.argb =t->value->uint8};
         break;
      case KEY_H2_SHADOW_COLOR:
         persist_write_int(KEY_H2_SHADOW_COLOR, t->value->uint8);
         oh2.offset_color = (GColor){.argb =t->value->uint8};
         break;
      case KEY_M1_SHADOW_COLOR:
         persist_write_int(KEY_M1_SHADOW_COLOR, t->value->uint8);
         om1.offset_color = (GColor){.argb =t->value->uint8};
         break;
      case KEY_M2_SHADOW_COLOR:
         persist_write_int(KEY_M2_SHADOW_COLOR, t->value->uint8);
         om2.offset_color = (GColor){.argb =t->value->uint8};
         break;
      case KEY_BG_COLOR:
        persist_write_int(KEY_BG_COLOR, t->value->uint8);
       
        window_set_background_color(my_window,  (GColor){.argb =t->value->uint8});
        break;
      case KEY_TIME_COLOR:
        persist_write_int(KEY_TIME_COLOR, t->value->uint8);
 
        text_layer_set_text_color(text_layer_hours, (GColor){.argb =t->value->uint8});
        text_layer_set_text_color(text_layer_minutes, (GColor){.argb =t->value->uint8}); 
    
        oh1.orig_color = (GColor){.argb =t->value->uint8};
        oh2.orig_color = (GColor){.argb =t->value->uint8};
        om1.orig_color = (GColor){.argb =t->value->uint8};
        om2.orig_color = (GColor){.argb =t->value->uint8};
        break;
      
      case KEY_SHADOW_DIRECTION:
        persist_write_int(KEY_SHADOW_DIRECTION, t->value->uint8);
        direct_shadow(t->value->uint8);
        break;
      case KEY_DATE_COLOR:
        persist_write_int(KEY_DATE_COLOR, t->value->uint8);
    
        text_layer_set_text_color(text_layer_date, (GColor){.argb =t->value->uint8});
        break;
      case KEY_BAR_COLOR:
        persist_write_int(KEY_BAR_COLOR, t->value->uint8);
    
        text_layer_set_background_color(text_layer_date, (GColor){.argb =t->value->uint8});
        break;
     
    }    
    
    t = dict_read_next(iterator);
  
  }
  layer_mark_dirty(window_get_root_layer(my_window));
}
  



void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
   if (!clock_is_24h_style()) {
    
        if( tick_time->tm_hour > 11 ) {   // YG Jun-25-2014: 0..11 - am 12..23 - pm
            if( tick_time->tm_hour > 12 ) tick_time->tm_hour -= 12;
        } else {
            if( tick_time->tm_hour == 0 ) tick_time->tm_hour = 12;
        }        
    }
  
    if (units_changed & MINUTE_UNIT) { // on minutes change - change time
      strftime(s_hours, sizeof(s_hours), "%H", tick_time);
      text_layer_set_text(text_layer_hours, s_hours);
      strftime(s_minutes, sizeof(s_hours), "%M", tick_time);
      text_layer_set_text(text_layer_minutes, s_minutes);
    }  
    
    if (units_changed & DAY_UNIT) { // on day change - change date
      #ifndef PBL_ROUND
        strftime(s_date, sizeof(s_date), "%a, %b %d ", tick_time);
        text_layer_set_text(text_layer_date, s_date);
      #endif
      
    }
  
}


void handle_init(void) {
  
  #ifndef PBL_SDK_2
  // need to catch when app resumes focus after notification, otherwise background won't restore
  app_focus_service_subscribe_handlers((AppFocusHandlers){
    .did_focus = app_focus_changed
  });
  #endif

  
  my_window = window_create();
  
  bg_color =  persist_read_int(KEY_BG_COLOR) ? (GColor){.argb =  persist_read_int(KEY_BG_COLOR) }: GColorBulgarianRose;
  window_set_background_color(my_window, bg_color );
  
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);

  time_color = persist_read_int(KEY_TIME_COLOR)? (GColor){.argb = persist_read_int(KEY_TIME_COLOR)} : GColorWhite;
  date_color = persist_read_int(KEY_DATE_COLOR)? (GColor){.argb = persist_read_int(KEY_DATE_COLOR)} : GColorBulgarianRose;
  bar_color = persist_read_int(KEY_BAR_COLOR)? (GColor){.argb = persist_read_int(KEY_BAR_COLOR)} : GColorWhite;
  text_layer_hours = create_text_layer(GRect(30,10,bounds.size.w-30,85), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_UBUNTU_B_72)), time_color, GColorClear, GTextAlignmentCenter, my_window);
  text_layer_minutes = create_text_layer(GRect(0,70,bounds.size.w-30,85), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_UBUNTU_B_72)), time_color, GColorClear, GTextAlignmentCenter, my_window);
  #ifndef PBL_ROUND
    text_layer_date = create_text_layer(GRect(0,-2,bounds.size.w,23), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PROTOTYPE_18)), date_color , bar_color, GTextAlignmentRight, my_window);
  #endif
 
  
  //creating shadow layers
  h1 = create_shadow_layer(GRect(bounds.size.w - PBL_IF_RECT_ELSE(100, 116) ,25,45,60), time_color, persist_read_int(KEY_H1_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_H1_SHADOW_COLOR)} : GColorBlack, 90, 90, 1, &oh1, my_window);
  h2 = create_shadow_layer(GRect(bounds.size.w - PBL_IF_RECT_ELSE(59, 76), 25,45,60), time_color, persist_read_int(KEY_H2_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_H2_SHADOW_COLOR)} : GColorBlack, 90, 90, 1, &oh2, my_window);
  m1 = create_shadow_layer(GRect(bounds.size.w - PBL_IF_RECT_ELSE(130, 146) ,85,45,70), time_color, persist_read_int(KEY_M1_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_M1_SHADOW_COLOR)}: GColorBlack, 90, 90, 1, &om1, my_window);
  m2 = create_shadow_layer(GRect(bounds.size.w - PBL_IF_RECT_ELSE(89, 106) ,85,45,70), time_color, persist_read_int(KEY_M2_SHADOW_COLOR)? (GColor){.argb = persist_read_int(KEY_M2_SHADOW_COLOR)}: GColorBlack, 90, 90, 1, &om2, my_window);
    
    
  direct_shadow( persist_read_int(KEY_SHADOW_DIRECTION)?  persist_read_int(KEY_SHADOW_DIRECTION) : SHADOW_DIRECTION_TO_BOTTOM_RIGHT);  
  
  window_stack_push(my_window, true);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
   //Get a time structure so that the face doesn't start blank
  time_t temp = time(NULL);
  struct tm *t = localtime(&temp);
 
  //Manually call the tick handler when the window is loading
  tick_handler(t, DAY_UNIT | MINUTE_UNIT);
 
  // subscribing to JS messages
  app_message_register_inbox_received(in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
}

void handle_deinit(void) {
  
  #ifndef PBL_SDK_2
    app_focus_service_unsubscribe();
  #endif
  
  text_layer_destroy(text_layer_hours);
  text_layer_destroy(text_layer_minutes);
  text_layer_destroy(text_layer_date);
    
  effect_layer_destroy(h1); effect_layer_destroy(h2);
  effect_layer_destroy(m1); effect_layer_destroy(m2);
  
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  app_message_deregister_callbacks();
 
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
