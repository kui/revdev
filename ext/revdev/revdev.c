// -*- coding:utf-8; mode:c; -*-

#include <stdlib.h>
#include <stdio.h>
#include <linux/input.h>

#include <ruby.h>
#include <rubyio.h>

#define NAME "Revdev"
#define BUFF_SIZE 255

#define FALSE 0
#define TRUE !FALSE

// #define FILENO fileno(RFILE(file)->fptr->f);

VALUE module_revdev;
VALUE class_event_device;
VALUE class_input_event;

void print_input_event_type(struct input_event *ie)
{
  switch(ie->type){
  case EV_SYN: printf("ev_syn\n"); break;
  case EV_KEY: printf("ev_key\n"); break;
  case EV_REL: printf("ev_rel\n"); break;
  case EV_ABS: printf("ev_abs\n"); break;
  case EV_LED: printf("ev_led\n"); break;
  case EV_SND: printf("ev_snd\n"); break;
  case EV_REP: printf("ev_rep\n"); break;
  case EV_FF: printf("ev_ff\n"); break;
  case EV_PWR: printf("ev_pwr\n"); break;
  case EV_FF_STATUS: printf("ev_ff_status\n"); break;
  case EV_MAX: printf("ev_max\n"); break;
  case EV_CNT: printf("ev_cnt\n"); break;
  default: printf("unknown type\n");
  }
}

VALUE input_event_initialize(VALUE self, VALUE byte)
{
  struct input_event *ie;
  struct timeval t;

  ie = RSTRING(byte)->ptr;
  t = ie->time;
  // printf("input_event type:%d, code:%d, value:%d\n", ie->type, ie->code, ie->value);

  rb_iv_set(self, "@time", rb_time_new(t.tv_sec, t.tv_usec));
  rb_iv_set(self, "@type", INT2FIX(ie->type));
  rb_iv_set(self, "@code", INT2FIX(ie->code));
  rb_iv_set(self, "@value", INT2NUM(ie->value));
  return self;
}

void Init_revdev(void)
{
  module_revdev = rb_define_module(NAME);
  class_event_device =
    rb_define_class_under(module_revdev, "EventDevice", rb_cObject);

  // import request constants for ioctl(2) on <input.h>
  /* get driver version */
  rb_define_const(class_event_device, "EVIOCGVERSION", LONG2FIX(EVIOCGVERSION));
  /* get device ID */
  rb_define_const(class_event_device, "EVIOCGID", LONG2FIX(EVIOCGID));
  /* get repeat settings */
  rb_define_const(class_event_device, "EVIOCGREP", LONG2FIX(EVIOCGREP));
  /* set repeat settings */
  rb_define_const(class_event_device, "EVIOCSREP", LONG2FIX(EVIOCSREP));
  /* get keycode */
  rb_define_const(class_event_device, "EVIOCGKEYCODE", LONG2FIX(EVIOCGKEYCODE));
  rb_define_const(class_event_device, "EVIOCGKEYCODE_V2", LONG2FIX(EVIOCGKEYCODE_V2));
  /* set keycode */
  rb_define_const(class_event_device, "EVIOCSKEYCODE", LONG2FIX(EVIOCSKEYCODE));
  rb_define_const(class_event_device, "EVIOCSKEYCODE_V2", LONG2FIX(EVIOCSKEYCODE_V2));

  /* get device name */
  rb_define_const(class_event_device, "EVIOCGNAME", LONG2FIX(EVIOCGNAME(BUFF_SIZE)));
  /* get physical location */
  rb_define_const(class_event_device, "EVIOCGPHYS", LONG2FIX(EVIOCGPHYS(BUFF_SIZE)));
  /* get unique identifier */
  rb_define_const(class_event_device, "EVIOCGUNIQ", LONG2FIX(EVIOCGUNIQ(BUFF_SIZE)));
  /* get device properties */
  rb_define_const(class_event_device, "EVIOCGPROP", LONG2FIX(EVIOCGPROP(BUFF_SIZE)));
  /* get global key state */
  rb_define_const(class_event_device, "EVIOCGKEY", LONG2FIX(EVIOCGKEY(BUFF_SIZE)));
  /* get all LEDs */
  rb_define_const(class_event_device, "EVIOCGLED", LONG2FIX(EVIOCGLED(BUFF_SIZE)));
  /* get all sounds status */
  rb_define_const(class_event_device, "EVIOCGSND", LONG2FIX(EVIOCGSND(BUFF_SIZE)));
  /* get all switch states */
  rb_define_const(class_event_device, "EVIOCGSW", LONG2FIX(EVIOCGSW(BUFF_SIZE)));

  /* get event bits */
  // rb_define_const(class_event_device, "EVIOCGBIT(ev,len)", LONG2FIX());
  /* get abs value/limits */
  // rb_define_const(class_event_device, "EVIOCGABS(abs)", LONG2FIX());
  /* set abs value/limits */
  // rb_define_const(class_event_device, "EVIOCSABS(abs)", LONG2FIX());
  /* send a force effect to a force feedback device */
  rb_define_const(class_event_device, "EVIOCSFF", LONG2FIX(EVIOCSFF));
  /* Erase a force effect */
  rb_define_const(class_event_device, "EVIOCRMFF", LONG2FIX(EVIOCRMFF));
  /* Report number of effects playable at the same time */
  rb_define_const(class_event_device, "EVIOCGEFFECTS", LONG2FIX(EVIOCGEFFECTS));

  /* Grab/Release device */
  rb_define_const(class_event_device, "EVIOCGRAB", LONG2FIX(EVIOCGRAB));

  class_input_event =
    rb_define_class_under(module_revdev, "InputEvent", rb_cObject);
  rb_define_method(class_input_event, "initialize", input_event_initialize, 1);

  rb_define_attr(class_input_event, "time", TRUE, FALSE);
  rb_define_attr(class_input_event, "type", TRUE, FALSE);
  rb_define_attr(class_input_event, "code", TRUE, FALSE);
  rb_define_attr(class_input_event, "value", TRUE, FALSE);

  rb_define_const(class_input_event, "SIZEOF", INT2NUM(sizeof(struct input_event)));

  rb_define_const(class_input_event, "EV_SYN", INT2NUM(EV_SYN));
  rb_define_const(class_input_event, "EV_KEY", INT2NUM(EV_KEY));
  rb_define_const(class_input_event, "EV_REL", INT2NUM(EV_REL));
  rb_define_const(class_input_event, "EV_ABS", INT2NUM(EV_ABS));
  rb_define_const(class_input_event, "EV_MSC", INT2NUM(EV_MSC));
  rb_define_const(class_input_event, "EV_SW", INT2NUM(EV_SW));
  rb_define_const(class_input_event, "EV_LED", INT2NUM(EV_LED));
  rb_define_const(class_input_event, "EV_SND", INT2NUM(EV_SND));
  rb_define_const(class_input_event, "EV_REP", INT2NUM(EV_REP));
  rb_define_const(class_input_event, "EV_FF", INT2NUM(EV_FF));
  rb_define_const(class_input_event, "EV_PWR", INT2NUM(EV_PWR));
  rb_define_const(class_input_event, "EV_FF_STATUS", INT2NUM(EV_FF_STATUS));
  rb_define_const(class_input_event, "EV_MAX", INT2NUM(EV_MAX));

}
