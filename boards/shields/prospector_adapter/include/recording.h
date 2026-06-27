#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_recording_indicator {
  sys_snode_t node;
  lv_obj_t *obj;
  bool initialized;
};

int zmk_widget_recording_indicator_init(
    struct zmk_widget_recording_indicator *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_recording_indicator_obj(
    struct zmk_widget_recording_indicator *widget);
