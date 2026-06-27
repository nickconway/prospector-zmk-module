#include <recording.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>

#include <display_colors.h>
#include <fonts.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(recording_indicator, CONFIG_ZMK_LOG_LEVEL);

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

#ifdef CONFIG_ZMK_BEHAVIOR_DYNAMIC_MACRO
#include <zmk/events/dynamic_macros_changed.h>

struct recording_update_state {
  uint8_t count;
};

static lv_obj_t *indicator;

void recording_indicator_update_cb(struct recording_update_state state) {
  struct zmk_widget_recording_indicator *widget;
  SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
    if (widget->initialized) {
      int size = state.count == 0 ? 0 : 8;

      lv_obj_set_size(indicator, size * 2, size * 2);
      lv_obj_set_pos(indicator, 10, 10);
      lv_obj_set_style_radius(indicator, LV_RADIUS_CIRCLE, LV_PART_MAIN);
      lv_obj_set_style_bg_color(
          indicator, lv_color_hex(DISPLAY_COLOR_REC_INDICATOR), LV_PART_MAIN);
      lv_obj_set_style_bg_opa(
          indicator, size == 0 ? LV_OPA_TRANSP : LV_OPA_COVER, LV_PART_MAIN);
      lv_obj_set_style_pad_all(indicator, 0, LV_PART_MAIN);
    }
  }
}

static struct recording_update_state
recording_indicator_get_recording_state(const zmk_event_t *eh) {
  if (eh == NULL) {
    return (struct recording_update_state){.count = 0};
  }

  const struct zmk_dynamic_macros_changed *ev =
      as_zmk_dynamic_macros_changed(eh);
  if (ev == NULL) {
    return (struct recording_update_state){.count = 0};
  }

  return (struct recording_update_state){
      .count = ev->recording_count,
  };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_recording_indicator,
                            struct recording_update_state,
                            recording_indicator_update_cb,
                            recording_indicator_get_recording_state);
ZMK_SUBSCRIPTION(widget_recording_indicator, zmk_dynamic_macros_changed);
#endif

int zmk_widget_recording_indicator_init(
    struct zmk_widget_recording_indicator *widget, lv_obj_t *parent) {
  widget->obj = lv_obj_create(parent);
  widget->initialized = true;
  indicator = lv_obj_create(widget->obj);
  sys_slist_append(&widgets, &widget->node);

  return 0;
}

lv_obj_t *zmk_widget_recording_indicator_obj(
    struct zmk_widget_recording_indicator *widget) {
  return widget->obj;
}
