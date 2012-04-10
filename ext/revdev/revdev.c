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
  // rb_iv_set(self, "raw", Data_Wrap_Struct(rb_cObject, 0, 0, ie));
  return self;
}

VALUE input_event_to_byte_string(VALUE self)
{
  struct input_event ie;
  struct timeval *t;
  VALUE tmp;

  tmp = rb_iv_get(self, "@time");
  Data_Get_Struct(tmp, struct time_object, t);
  //GetTimeval(tmp, t);
  // memcpy(&ie.time, &t, sizeof(struct timeval));
  ie.time = *t;

  tmp = rb_iv_get(self, "@type");
  ie.type = FIX2UINT(tmp);

  tmp = rb_iv_get(self, "@code");
  ie.code = FIX2UINT(tmp);

  tmp = rb_iv_get(self, "@value");
  ie.value = NUM2INT(tmp);

  printf("input_event type:%d, code:%d, value:%d\n", ie.type, ie.code, ie.value);
  return rb_str_new(&ie, sizeof(struct input_event));
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

  class_input_event = rb_define_class_under(module_revdev, "InputEvent", rb_cObject);
  rb_define_method(class_input_event, "raw_initialize", input_event_initialize, 1);
  rb_define_method(class_input_event, "to_byte_string", input_event_to_byte_string, 0);
  rb_define_attr(class_input_event, "time", TRUE, TRUE);
  rb_define_attr(class_input_event, "type", TRUE, TRUE);
  rb_define_attr(class_input_event, "code", TRUE, TRUE);
  rb_define_attr(class_input_event, "value", TRUE, TRUE);

  rb_define_const(class_input_event, "SIZEOF", INT2NUM(sizeof(struct input_event)));

  /*
    follwing code generated by
    $ tail -990 /usr/include/linux/input.h | head -774 | sed -e 's/#define \([A-Z0-9_]*\)   *[^\/]*[^\/]/rb_define_const(class_input_event, "\1", INT2NUM(\1));/'
   */

  /*
   * Device properties and quirks
   */

  rb_define_const(class_input_event, "INPUT_PROP_POINTER", INT2NUM(INPUT_PROP_POINTER));/* needs a pointer */
  rb_define_const(class_input_event, "INPUT_PROP_DIRECT", INT2NUM(INPUT_PROP_DIRECT));/* direct input devices */
  rb_define_const(class_input_event, "INPUT_PROP_BUTTONPAD", INT2NUM(INPUT_PROP_BUTTONPAD));/* has button(s) under pad */
  rb_define_const(class_input_event, "INPUT_PROP_SEMI_MT", INT2NUM(INPUT_PROP_SEMI_MT));/* touch rectangle only */

  rb_define_const(class_input_event, "INPUT_PROP_MAX", INT2NUM(INPUT_PROP_MAX));
  rb_define_const(class_input_event, "INPUT_PROP_CNT", INT2NUM(INPUT_PROP_CNT));

  /*
   * Event types
   */

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
  rb_define_const(class_input_event, "EV_CNT", INT2NUM(EV_CNT));

  /*
   * Synchronization events.
   */

  rb_define_const(class_input_event, "SYN_REPORT", INT2NUM(SYN_REPORT));
  rb_define_const(class_input_event, "SYN_CONFIG", INT2NUM(SYN_CONFIG));
  rb_define_const(class_input_event, "SYN_MT_REPORT", INT2NUM(SYN_MT_REPORT));
  rb_define_const(class_input_event, "SYN_DROPPED", INT2NUM(SYN_DROPPED));

  /*
   * Keys and buttons
   *
   * Most of the keys/buttons are modeled after USB HUT 1.12
   * (see http://www.usb.org/developers/hidpage).
   * Abbreviations in the comments:
   * AC - Application Control
   * AL - Application Launch Button
   * SC - System Control
   */

  rb_define_const(class_input_event, "KEY_RESERVED", INT2NUM(KEY_RESERVED));
  rb_define_const(class_input_event, "KEY_ESC", INT2NUM(KEY_ESC));
  rb_define_const(class_input_event, "KEY_1", INT2NUM(KEY_1));
  rb_define_const(class_input_event, "KEY_2", INT2NUM(KEY_2));
  rb_define_const(class_input_event, "KEY_3", INT2NUM(KEY_3));
  rb_define_const(class_input_event, "KEY_4", INT2NUM(KEY_4));
  rb_define_const(class_input_event, "KEY_5", INT2NUM(KEY_5));
  rb_define_const(class_input_event, "KEY_6", INT2NUM(KEY_6));
  rb_define_const(class_input_event, "KEY_7", INT2NUM(KEY_7));
  rb_define_const(class_input_event, "KEY_8", INT2NUM(KEY_8));
  rb_define_const(class_input_event, "KEY_9", INT2NUM(KEY_9));
  rb_define_const(class_input_event, "KEY_0", INT2NUM(KEY_0));
  rb_define_const(class_input_event, "KEY_MINUS", INT2NUM(KEY_MINUS));
  rb_define_const(class_input_event, "KEY_EQUAL", INT2NUM(KEY_EQUAL));
  rb_define_const(class_input_event, "KEY_BACKSPACE", INT2NUM(KEY_BACKSPACE));
  rb_define_const(class_input_event, "KEY_TAB", INT2NUM(KEY_TAB));
  rb_define_const(class_input_event, "KEY_Q", INT2NUM(KEY_Q));
  rb_define_const(class_input_event, "KEY_W", INT2NUM(KEY_W));
  rb_define_const(class_input_event, "KEY_E", INT2NUM(KEY_E));
  rb_define_const(class_input_event, "KEY_R", INT2NUM(KEY_R));
  rb_define_const(class_input_event, "KEY_T", INT2NUM(KEY_T));
  rb_define_const(class_input_event, "KEY_Y", INT2NUM(KEY_Y));
  rb_define_const(class_input_event, "KEY_U", INT2NUM(KEY_U));
  rb_define_const(class_input_event, "KEY_I", INT2NUM(KEY_I));
  rb_define_const(class_input_event, "KEY_O", INT2NUM(KEY_O));
  rb_define_const(class_input_event, "KEY_P", INT2NUM(KEY_P));
  rb_define_const(class_input_event, "KEY_LEFTBRACE", INT2NUM(KEY_LEFTBRACE));
  rb_define_const(class_input_event, "KEY_RIGHTBRACE", INT2NUM(KEY_RIGHTBRACE));
  rb_define_const(class_input_event, "KEY_ENTER", INT2NUM(KEY_ENTER));
  rb_define_const(class_input_event, "KEY_LEFTCTRL", INT2NUM(KEY_LEFTCTRL));
  rb_define_const(class_input_event, "KEY_A", INT2NUM(KEY_A));
  rb_define_const(class_input_event, "KEY_S", INT2NUM(KEY_S));
  rb_define_const(class_input_event, "KEY_D", INT2NUM(KEY_D));
  rb_define_const(class_input_event, "KEY_F", INT2NUM(KEY_F));
  rb_define_const(class_input_event, "KEY_G", INT2NUM(KEY_G));
  rb_define_const(class_input_event, "KEY_H", INT2NUM(KEY_H));
  rb_define_const(class_input_event, "KEY_J", INT2NUM(KEY_J));
  rb_define_const(class_input_event, "KEY_K", INT2NUM(KEY_K));
  rb_define_const(class_input_event, "KEY_L", INT2NUM(KEY_L));
  rb_define_const(class_input_event, "KEY_SEMICOLON", INT2NUM(KEY_SEMICOLON));
  rb_define_const(class_input_event, "KEY_APOSTROPHE", INT2NUM(KEY_APOSTROPHE));
  rb_define_const(class_input_event, "KEY_GRAVE", INT2NUM(KEY_GRAVE));
  rb_define_const(class_input_event, "KEY_LEFTSHIFT", INT2NUM(KEY_LEFTSHIFT));
  rb_define_const(class_input_event, "KEY_BACKSLASH", INT2NUM(KEY_BACKSLASH));
  rb_define_const(class_input_event, "KEY_Z", INT2NUM(KEY_Z));
  rb_define_const(class_input_event, "KEY_X", INT2NUM(KEY_X));
  rb_define_const(class_input_event, "KEY_C", INT2NUM(KEY_C));
  rb_define_const(class_input_event, "KEY_V", INT2NUM(KEY_V));
  rb_define_const(class_input_event, "KEY_B", INT2NUM(KEY_B));
  rb_define_const(class_input_event, "KEY_N", INT2NUM(KEY_N));
  rb_define_const(class_input_event, "KEY_M", INT2NUM(KEY_M));
  rb_define_const(class_input_event, "KEY_COMMA", INT2NUM(KEY_COMMA));
  rb_define_const(class_input_event, "KEY_DOT", INT2NUM(KEY_DOT));
  rb_define_const(class_input_event, "KEY_SLASH", INT2NUM(KEY_SLASH));
  rb_define_const(class_input_event, "KEY_RIGHTSHIFT", INT2NUM(KEY_RIGHTSHIFT));
  rb_define_const(class_input_event, "KEY_KPASTERISK", INT2NUM(KEY_KPASTERISK));
  rb_define_const(class_input_event, "KEY_LEFTALT", INT2NUM(KEY_LEFTALT));
  rb_define_const(class_input_event, "KEY_SPACE", INT2NUM(KEY_SPACE));
  rb_define_const(class_input_event, "KEY_CAPSLOCK", INT2NUM(KEY_CAPSLOCK));
  rb_define_const(class_input_event, "KEY_F1", INT2NUM(KEY_F1));
  rb_define_const(class_input_event, "KEY_F2", INT2NUM(KEY_F2));
  rb_define_const(class_input_event, "KEY_F3", INT2NUM(KEY_F3));
  rb_define_const(class_input_event, "KEY_F4", INT2NUM(KEY_F4));
  rb_define_const(class_input_event, "KEY_F5", INT2NUM(KEY_F5));
  rb_define_const(class_input_event, "KEY_F6", INT2NUM(KEY_F6));
  rb_define_const(class_input_event, "KEY_F7", INT2NUM(KEY_F7));
  rb_define_const(class_input_event, "KEY_F8", INT2NUM(KEY_F8));
  rb_define_const(class_input_event, "KEY_F9", INT2NUM(KEY_F9));
  rb_define_const(class_input_event, "KEY_F10", INT2NUM(KEY_F10));
  rb_define_const(class_input_event, "KEY_NUMLOCK", INT2NUM(KEY_NUMLOCK));
  rb_define_const(class_input_event, "KEY_SCROLLLOCK", INT2NUM(KEY_SCROLLLOCK));
  rb_define_const(class_input_event, "KEY_KP7", INT2NUM(KEY_KP7));
  rb_define_const(class_input_event, "KEY_KP8", INT2NUM(KEY_KP8));
  rb_define_const(class_input_event, "KEY_KP9", INT2NUM(KEY_KP9));
  rb_define_const(class_input_event, "KEY_KPMINUS", INT2NUM(KEY_KPMINUS));
  rb_define_const(class_input_event, "KEY_KP4", INT2NUM(KEY_KP4));
  rb_define_const(class_input_event, "KEY_KP5", INT2NUM(KEY_KP5));
  rb_define_const(class_input_event, "KEY_KP6", INT2NUM(KEY_KP6));
  rb_define_const(class_input_event, "KEY_KPPLUS", INT2NUM(KEY_KPPLUS));
  rb_define_const(class_input_event, "KEY_KP1", INT2NUM(KEY_KP1));
  rb_define_const(class_input_event, "KEY_KP2", INT2NUM(KEY_KP2));
  rb_define_const(class_input_event, "KEY_KP3", INT2NUM(KEY_KP3));
  rb_define_const(class_input_event, "KEY_KP0", INT2NUM(KEY_KP0));
  rb_define_const(class_input_event, "KEY_KPDOT", INT2NUM(KEY_KPDOT));

  rb_define_const(class_input_event, "KEY_ZENKAKUHANKAKU", INT2NUM(KEY_ZENKAKUHANKAKU));
  rb_define_const(class_input_event, "KEY_102ND", INT2NUM(KEY_102ND));
  rb_define_const(class_input_event, "KEY_F11", INT2NUM(KEY_F11));
  rb_define_const(class_input_event, "KEY_F12", INT2NUM(KEY_F12));
  rb_define_const(class_input_event, "KEY_RO", INT2NUM(KEY_RO));
  rb_define_const(class_input_event, "KEY_KATAKANA", INT2NUM(KEY_KATAKANA));
  rb_define_const(class_input_event, "KEY_HIRAGANA", INT2NUM(KEY_HIRAGANA));
  rb_define_const(class_input_event, "KEY_HENKAN", INT2NUM(KEY_HENKAN));
  rb_define_const(class_input_event, "KEY_KATAKANAHIRAGANA", INT2NUM(KEY_KATAKANAHIRAGANA));
  rb_define_const(class_input_event, "KEY_MUHENKAN", INT2NUM(KEY_MUHENKAN));
  rb_define_const(class_input_event, "KEY_KPJPCOMMA", INT2NUM(KEY_KPJPCOMMA));
  rb_define_const(class_input_event, "KEY_KPENTER", INT2NUM(KEY_KPENTER));
  rb_define_const(class_input_event, "KEY_RIGHTCTRL", INT2NUM(KEY_RIGHTCTRL));
  rb_define_const(class_input_event, "KEY_KPSLASH", INT2NUM(KEY_KPSLASH));
  rb_define_const(class_input_event, "KEY_SYSRQ", INT2NUM(KEY_SYSRQ));
  rb_define_const(class_input_event, "KEY_RIGHTALT", INT2NUM(KEY_RIGHTALT));
  rb_define_const(class_input_event, "KEY_LINEFEED", INT2NUM(KEY_LINEFEED));
  rb_define_const(class_input_event, "KEY_HOME", INT2NUM(KEY_HOME));
  rb_define_const(class_input_event, "KEY_UP", INT2NUM(KEY_UP));
  rb_define_const(class_input_event, "KEY_PAGEUP", INT2NUM(KEY_PAGEUP));
  rb_define_const(class_input_event, "KEY_LEFT", INT2NUM(KEY_LEFT));
  rb_define_const(class_input_event, "KEY_RIGHT", INT2NUM(KEY_RIGHT));
  rb_define_const(class_input_event, "KEY_END", INT2NUM(KEY_END));
  rb_define_const(class_input_event, "KEY_DOWN", INT2NUM(KEY_DOWN));
  rb_define_const(class_input_event, "KEY_PAGEDOWN", INT2NUM(KEY_PAGEDOWN));
  rb_define_const(class_input_event, "KEY_INSERT", INT2NUM(KEY_INSERT));
  rb_define_const(class_input_event, "KEY_DELETE", INT2NUM(KEY_DELETE));
  rb_define_const(class_input_event, "KEY_MACRO", INT2NUM(KEY_MACRO));
  rb_define_const(class_input_event, "KEY_MUTE", INT2NUM(KEY_MUTE));
  rb_define_const(class_input_event, "KEY_VOLUMEDOWN", INT2NUM(KEY_VOLUMEDOWN));
  rb_define_const(class_input_event, "KEY_VOLUMEUP", INT2NUM(KEY_VOLUMEUP));
  rb_define_const(class_input_event, "KEY_POWER", INT2NUM(KEY_POWER));/* SC System Power Down */
  rb_define_const(class_input_event, "KEY_KPEQUAL", INT2NUM(KEY_KPEQUAL));
  rb_define_const(class_input_event, "KEY_KPPLUSMINUS", INT2NUM(KEY_KPPLUSMINUS));
  rb_define_const(class_input_event, "KEY_PAUSE", INT2NUM(KEY_PAUSE));
  rb_define_const(class_input_event, "KEY_SCALE", INT2NUM(KEY_SCALE));/* AL Compiz Scale (Expose) */

  rb_define_const(class_input_event, "KEY_KPCOMMA", INT2NUM(KEY_KPCOMMA));
  rb_define_const(class_input_event, "KEY_HANGEUL", INT2NUM(KEY_HANGEUL));
  rb_define_const(class_input_event, "KEY_HANGUEL", INT2NUM(KEY_HANGUEL));
  rb_define_const(class_input_event, "KEY_HANJA", INT2NUM(KEY_HANJA));
  rb_define_const(class_input_event, "KEY_YEN", INT2NUM(KEY_YEN));
  rb_define_const(class_input_event, "KEY_LEFTMETA", INT2NUM(KEY_LEFTMETA));
  rb_define_const(class_input_event, "KEY_RIGHTMETA", INT2NUM(KEY_RIGHTMETA));
  rb_define_const(class_input_event, "KEY_COMPOSE", INT2NUM(KEY_COMPOSE));

  rb_define_const(class_input_event, "KEY_STOP", INT2NUM(KEY_STOP));/* AC Stop */
  rb_define_const(class_input_event, "KEY_AGAIN", INT2NUM(KEY_AGAIN));
  rb_define_const(class_input_event, "KEY_PROPS", INT2NUM(KEY_PROPS));/* AC Properties */
  rb_define_const(class_input_event, "KEY_UNDO", INT2NUM(KEY_UNDO));/* AC Undo */
  rb_define_const(class_input_event, "KEY_FRONT", INT2NUM(KEY_FRONT));
  rb_define_const(class_input_event, "KEY_COPY", INT2NUM(KEY_COPY));/* AC Copy */
  rb_define_const(class_input_event, "KEY_OPEN", INT2NUM(KEY_OPEN));/* AC Open */
  rb_define_const(class_input_event, "KEY_PASTE", INT2NUM(KEY_PASTE));/* AC Paste */
  rb_define_const(class_input_event, "KEY_FIND", INT2NUM(KEY_FIND));/* AC Search */
  rb_define_const(class_input_event, "KEY_CUT", INT2NUM(KEY_CUT));/* AC Cut */
  rb_define_const(class_input_event, "KEY_HELP", INT2NUM(KEY_HELP));/* AL Integrated Help Center */
  rb_define_const(class_input_event, "KEY_MENU", INT2NUM(KEY_MENU));/* Menu (show menu) */
  rb_define_const(class_input_event, "KEY_CALC", INT2NUM(KEY_CALC));/* AL Calculator */
  rb_define_const(class_input_event, "KEY_SETUP", INT2NUM(KEY_SETUP));
  rb_define_const(class_input_event, "KEY_SLEEP", INT2NUM(KEY_SLEEP));/* SC System Sleep */
  rb_define_const(class_input_event, "KEY_WAKEUP", INT2NUM(KEY_WAKEUP));/* System Wake Up */
  rb_define_const(class_input_event, "KEY_FILE", INT2NUM(KEY_FILE));/* AL Local Machine Browser */
  rb_define_const(class_input_event, "KEY_SENDFILE", INT2NUM(KEY_SENDFILE));
  rb_define_const(class_input_event, "KEY_DELETEFILE", INT2NUM(KEY_DELETEFILE));
  rb_define_const(class_input_event, "KEY_XFER", INT2NUM(KEY_XFER));
  rb_define_const(class_input_event, "KEY_PROG1", INT2NUM(KEY_PROG1));
  rb_define_const(class_input_event, "KEY_PROG2", INT2NUM(KEY_PROG2));
  rb_define_const(class_input_event, "KEY_WWW", INT2NUM(KEY_WWW));/* AL Internet Browser */
  rb_define_const(class_input_event, "KEY_MSDOS", INT2NUM(KEY_MSDOS));
  rb_define_const(class_input_event, "KEY_COFFEE", INT2NUM(KEY_COFFEE));/* AL Terminal Lock/Screensaver */
  rb_define_const(class_input_event, "KEY_SCREENLOCK", INT2NUM(KEY_SCREENLOCK));
  rb_define_const(class_input_event, "KEY_DIRECTION", INT2NUM(KEY_DIRECTION));
  rb_define_const(class_input_event, "KEY_CYCLEWINDOWS", INT2NUM(KEY_CYCLEWINDOWS));
  rb_define_const(class_input_event, "KEY_MAIL", INT2NUM(KEY_MAIL));
  rb_define_const(class_input_event, "KEY_BOOKMARKS", INT2NUM(KEY_BOOKMARKS));/* AC Bookmarks */
  rb_define_const(class_input_event, "KEY_COMPUTER", INT2NUM(KEY_COMPUTER));
  rb_define_const(class_input_event, "KEY_BACK", INT2NUM(KEY_BACK));/* AC Back */
  rb_define_const(class_input_event, "KEY_FORWARD", INT2NUM(KEY_FORWARD));/* AC Forward */
  rb_define_const(class_input_event, "KEY_CLOSECD", INT2NUM(KEY_CLOSECD));
  rb_define_const(class_input_event, "KEY_EJECTCD", INT2NUM(KEY_EJECTCD));
  rb_define_const(class_input_event, "KEY_EJECTCLOSECD", INT2NUM(KEY_EJECTCLOSECD));
  rb_define_const(class_input_event, "KEY_NEXTSONG", INT2NUM(KEY_NEXTSONG));
  rb_define_const(class_input_event, "KEY_PLAYPAUSE", INT2NUM(KEY_PLAYPAUSE));
  rb_define_const(class_input_event, "KEY_PREVIOUSSONG", INT2NUM(KEY_PREVIOUSSONG));
  rb_define_const(class_input_event, "KEY_STOPCD", INT2NUM(KEY_STOPCD));
  rb_define_const(class_input_event, "KEY_RECORD", INT2NUM(KEY_RECORD));
  rb_define_const(class_input_event, "KEY_REWIND", INT2NUM(KEY_REWIND));
  rb_define_const(class_input_event, "KEY_PHONE", INT2NUM(KEY_PHONE));/* Media Select Telephone */
  rb_define_const(class_input_event, "KEY_ISO", INT2NUM(KEY_ISO));
  rb_define_const(class_input_event, "KEY_CONFIG", INT2NUM(KEY_CONFIG));/* AL Consumer Control Configuration */
  rb_define_const(class_input_event, "KEY_HOMEPAGE", INT2NUM(KEY_HOMEPAGE));/* AC Home */
  rb_define_const(class_input_event, "KEY_REFRESH", INT2NUM(KEY_REFRESH));/* AC Refresh */
  rb_define_const(class_input_event, "KEY_EXIT", INT2NUM(KEY_EXIT));/* AC Exit */
  rb_define_const(class_input_event, "KEY_MOVE", INT2NUM(KEY_MOVE));
  rb_define_const(class_input_event, "KEY_EDIT", INT2NUM(KEY_EDIT));
  rb_define_const(class_input_event, "KEY_SCROLLUP", INT2NUM(KEY_SCROLLUP));
  rb_define_const(class_input_event, "KEY_SCROLLDOWN", INT2NUM(KEY_SCROLLDOWN));
  rb_define_const(class_input_event, "KEY_KPLEFTPAREN", INT2NUM(KEY_KPLEFTPAREN));
  rb_define_const(class_input_event, "KEY_KPRIGHTPAREN", INT2NUM(KEY_KPRIGHTPAREN));
  rb_define_const(class_input_event, "KEY_NEW", INT2NUM(KEY_NEW));/* AC New */
  rb_define_const(class_input_event, "KEY_REDO", INT2NUM(KEY_REDO));/* AC Redo/Repeat */

  rb_define_const(class_input_event, "KEY_F13", INT2NUM(KEY_F13));
  rb_define_const(class_input_event, "KEY_F14", INT2NUM(KEY_F14));
  rb_define_const(class_input_event, "KEY_F15", INT2NUM(KEY_F15));
  rb_define_const(class_input_event, "KEY_F16", INT2NUM(KEY_F16));
  rb_define_const(class_input_event, "KEY_F17", INT2NUM(KEY_F17));
  rb_define_const(class_input_event, "KEY_F18", INT2NUM(KEY_F18));
  rb_define_const(class_input_event, "KEY_F19", INT2NUM(KEY_F19));
  rb_define_const(class_input_event, "KEY_F20", INT2NUM(KEY_F20));
  rb_define_const(class_input_event, "KEY_F21", INT2NUM(KEY_F21));
  rb_define_const(class_input_event, "KEY_F22", INT2NUM(KEY_F22));
  rb_define_const(class_input_event, "KEY_F23", INT2NUM(KEY_F23));
  rb_define_const(class_input_event, "KEY_F24", INT2NUM(KEY_F24));

  rb_define_const(class_input_event, "KEY_PLAYCD", INT2NUM(KEY_PLAYCD));
  rb_define_const(class_input_event, "KEY_PAUSECD", INT2NUM(KEY_PAUSECD));
  rb_define_const(class_input_event, "KEY_PROG3", INT2NUM(KEY_PROG3));
  rb_define_const(class_input_event, "KEY_PROG4", INT2NUM(KEY_PROG4));
  rb_define_const(class_input_event, "KEY_DASHBOARD", INT2NUM(KEY_DASHBOARD));/* AL Dashboard */
  rb_define_const(class_input_event, "KEY_SUSPEND", INT2NUM(KEY_SUSPEND));
  rb_define_const(class_input_event, "KEY_CLOSE", INT2NUM(KEY_CLOSE));/* AC Close */
  rb_define_const(class_input_event, "KEY_PLAY", INT2NUM(KEY_PLAY));
  rb_define_const(class_input_event, "KEY_FASTFORWARD", INT2NUM(KEY_FASTFORWARD));
  rb_define_const(class_input_event, "KEY_BASSBOOST", INT2NUM(KEY_BASSBOOST));
  rb_define_const(class_input_event, "KEY_PRINT", INT2NUM(KEY_PRINT));/* AC Print */
  rb_define_const(class_input_event, "KEY_HP", INT2NUM(KEY_HP));
  rb_define_const(class_input_event, "KEY_CAMERA", INT2NUM(KEY_CAMERA));
  rb_define_const(class_input_event, "KEY_SOUND", INT2NUM(KEY_SOUND));
  rb_define_const(class_input_event, "KEY_QUESTION", INT2NUM(KEY_QUESTION));
  rb_define_const(class_input_event, "KEY_EMAIL", INT2NUM(KEY_EMAIL));
  rb_define_const(class_input_event, "KEY_CHAT", INT2NUM(KEY_CHAT));
  rb_define_const(class_input_event, "KEY_SEARCH", INT2NUM(KEY_SEARCH));
  rb_define_const(class_input_event, "KEY_CONNECT", INT2NUM(KEY_CONNECT));
  rb_define_const(class_input_event, "KEY_FINANCE", INT2NUM(KEY_FINANCE));/* AL Checkbook/Finance */
  rb_define_const(class_input_event, "KEY_SPORT", INT2NUM(KEY_SPORT));
  rb_define_const(class_input_event, "KEY_SHOP", INT2NUM(KEY_SHOP));
  rb_define_const(class_input_event, "KEY_ALTERASE", INT2NUM(KEY_ALTERASE));
  rb_define_const(class_input_event, "KEY_CANCEL", INT2NUM(KEY_CANCEL));/* AC Cancel */
  rb_define_const(class_input_event, "KEY_BRIGHTNESSDOWN", INT2NUM(KEY_BRIGHTNESSDOWN));
  rb_define_const(class_input_event, "KEY_BRIGHTNESSUP", INT2NUM(KEY_BRIGHTNESSUP));
  rb_define_const(class_input_event, "KEY_MEDIA", INT2NUM(KEY_MEDIA));

  rb_define_const(class_input_event, "KEY_SWITCHVIDEOMODE", INT2NUM(KEY_SWITCHVIDEOMODE));/* Cycle between available video
                                                                                             outputs (Monitor/LCD/TV-out/etc) */
  rb_define_const(class_input_event, "KEY_KBDILLUMTOGGLE", INT2NUM(KEY_KBDILLUMTOGGLE));
  rb_define_const(class_input_event, "KEY_KBDILLUMDOWN", INT2NUM(KEY_KBDILLUMDOWN));
  rb_define_const(class_input_event, "KEY_KBDILLUMUP", INT2NUM(KEY_KBDILLUMUP));

  rb_define_const(class_input_event, "KEY_SEND", INT2NUM(KEY_SEND));/* AC Send */
  rb_define_const(class_input_event, "KEY_REPLY", INT2NUM(KEY_REPLY));/* AC Reply */
  rb_define_const(class_input_event, "KEY_FORWARDMAIL", INT2NUM(KEY_FORWARDMAIL));/* AC Forward Msg */
  rb_define_const(class_input_event, "KEY_SAVE", INT2NUM(KEY_SAVE));/* AC Save */
  rb_define_const(class_input_event, "KEY_DOCUMENTS", INT2NUM(KEY_DOCUMENTS));

  rb_define_const(class_input_event, "KEY_BATTERY", INT2NUM(KEY_BATTERY));

  rb_define_const(class_input_event, "KEY_BLUETOOTH", INT2NUM(KEY_BLUETOOTH));
  rb_define_const(class_input_event, "KEY_WLAN", INT2NUM(KEY_WLAN));
  rb_define_const(class_input_event, "KEY_UWB", INT2NUM(KEY_UWB));

  rb_define_const(class_input_event, "KEY_UNKNOWN", INT2NUM(KEY_UNKNOWN));

  rb_define_const(class_input_event, "KEY_VIDEO_NEXT", INT2NUM(KEY_VIDEO_NEXT));/* drive next video source */
  rb_define_const(class_input_event, "KEY_VIDEO_PREV", INT2NUM(KEY_VIDEO_PREV));/* drive previous video source */
  rb_define_const(class_input_event, "KEY_BRIGHTNESS_CYCLE", INT2NUM(KEY_BRIGHTNESS_CYCLE));/* brightness up, after max is min */
  rb_define_const(class_input_event, "KEY_BRIGHTNESS_ZERO", INT2NUM(KEY_BRIGHTNESS_ZERO));/* brightness off, use ambient */
  rb_define_const(class_input_event, "KEY_DISPLAY_OFF", INT2NUM(KEY_DISPLAY_OFF));/* display device to off state */

  rb_define_const(class_input_event, "KEY_WIMAX", INT2NUM(KEY_WIMAX));
  rb_define_const(class_input_event, "KEY_RFKILL", INT2NUM(KEY_RFKILL));/* Key that controls all radios */

  /* Code 255 is reserved for special needs of AT keyboard driver */

  rb_define_const(class_input_event, "BTN_MISC", INT2NUM(BTN_MISC));
  rb_define_const(class_input_event, "BTN_0", INT2NUM(BTN_0));
  rb_define_const(class_input_event, "BTN_1", INT2NUM(BTN_1));
  rb_define_const(class_input_event, "BTN_2", INT2NUM(BTN_2));
  rb_define_const(class_input_event, "BTN_3", INT2NUM(BTN_3));
  rb_define_const(class_input_event, "BTN_4", INT2NUM(BTN_4));
  rb_define_const(class_input_event, "BTN_5", INT2NUM(BTN_5));
  rb_define_const(class_input_event, "BTN_6", INT2NUM(BTN_6));
  rb_define_const(class_input_event, "BTN_7", INT2NUM(BTN_7));
  rb_define_const(class_input_event, "BTN_8", INT2NUM(BTN_8));
  rb_define_const(class_input_event, "BTN_9", INT2NUM(BTN_9));

  rb_define_const(class_input_event, "BTN_MOUSE", INT2NUM(BTN_MOUSE));
  rb_define_const(class_input_event, "BTN_LEFT", INT2NUM(BTN_LEFT));
  rb_define_const(class_input_event, "BTN_RIGHT", INT2NUM(BTN_RIGHT));
  rb_define_const(class_input_event, "BTN_MIDDLE", INT2NUM(BTN_MIDDLE));
  rb_define_const(class_input_event, "BTN_SIDE", INT2NUM(BTN_SIDE));
  rb_define_const(class_input_event, "BTN_EXTRA", INT2NUM(BTN_EXTRA));
  rb_define_const(class_input_event, "BTN_FORWARD", INT2NUM(BTN_FORWARD));
  rb_define_const(class_input_event, "BTN_BACK", INT2NUM(BTN_BACK));
  rb_define_const(class_input_event, "BTN_TASK", INT2NUM(BTN_TASK));

  rb_define_const(class_input_event, "BTN_JOYSTICK", INT2NUM(BTN_JOYSTICK));
  rb_define_const(class_input_event, "BTN_TRIGGER", INT2NUM(BTN_TRIGGER));
  rb_define_const(class_input_event, "BTN_THUMB", INT2NUM(BTN_THUMB));
  rb_define_const(class_input_event, "BTN_THUMB2", INT2NUM(BTN_THUMB2));
  rb_define_const(class_input_event, "BTN_TOP", INT2NUM(BTN_TOP));
  rb_define_const(class_input_event, "BTN_TOP2", INT2NUM(BTN_TOP2));
  rb_define_const(class_input_event, "BTN_PINKIE", INT2NUM(BTN_PINKIE));
  rb_define_const(class_input_event, "BTN_BASE", INT2NUM(BTN_BASE));
  rb_define_const(class_input_event, "BTN_BASE2", INT2NUM(BTN_BASE2));
  rb_define_const(class_input_event, "BTN_BASE3", INT2NUM(BTN_BASE3));
  rb_define_const(class_input_event, "BTN_BASE4", INT2NUM(BTN_BASE4));
  rb_define_const(class_input_event, "BTN_BASE5", INT2NUM(BTN_BASE5));
  rb_define_const(class_input_event, "BTN_BASE6", INT2NUM(BTN_BASE6));
  rb_define_const(class_input_event, "BTN_DEAD", INT2NUM(BTN_DEAD));

  rb_define_const(class_input_event, "BTN_GAMEPAD", INT2NUM(BTN_GAMEPAD));
  rb_define_const(class_input_event, "BTN_A", INT2NUM(BTN_A));
  rb_define_const(class_input_event, "BTN_B", INT2NUM(BTN_B));
  rb_define_const(class_input_event, "BTN_C", INT2NUM(BTN_C));
  rb_define_const(class_input_event, "BTN_X", INT2NUM(BTN_X));
  rb_define_const(class_input_event, "BTN_Y", INT2NUM(BTN_Y));
  rb_define_const(class_input_event, "BTN_Z", INT2NUM(BTN_Z));
  rb_define_const(class_input_event, "BTN_TL", INT2NUM(BTN_TL));
  rb_define_const(class_input_event, "BTN_TR", INT2NUM(BTN_TR));
  rb_define_const(class_input_event, "BTN_TL2", INT2NUM(BTN_TL2));
  rb_define_const(class_input_event, "BTN_TR2", INT2NUM(BTN_TR2));
  rb_define_const(class_input_event, "BTN_SELECT", INT2NUM(BTN_SELECT));
  rb_define_const(class_input_event, "BTN_START", INT2NUM(BTN_START));
  rb_define_const(class_input_event, "BTN_MODE", INT2NUM(BTN_MODE));
  rb_define_const(class_input_event, "BTN_THUMBL", INT2NUM(BTN_THUMBL));
  rb_define_const(class_input_event, "BTN_THUMBR", INT2NUM(BTN_THUMBR));

  rb_define_const(class_input_event, "BTN_DIGI", INT2NUM(BTN_DIGI));
  rb_define_const(class_input_event, "BTN_TOOL_PEN", INT2NUM(BTN_TOOL_PEN));
  rb_define_const(class_input_event, "BTN_TOOL_RUBBER", INT2NUM(BTN_TOOL_RUBBER));
  rb_define_const(class_input_event, "BTN_TOOL_BRUSH", INT2NUM(BTN_TOOL_BRUSH));
  rb_define_const(class_input_event, "BTN_TOOL_PENCIL", INT2NUM(BTN_TOOL_PENCIL));
  rb_define_const(class_input_event, "BTN_TOOL_AIRBRUSH", INT2NUM(BTN_TOOL_AIRBRUSH));
  rb_define_const(class_input_event, "BTN_TOOL_FINGER", INT2NUM(BTN_TOOL_FINGER));
  rb_define_const(class_input_event, "BTN_TOOL_MOUSE", INT2NUM(BTN_TOOL_MOUSE));
  rb_define_const(class_input_event, "BTN_TOOL_LENS", INT2NUM(BTN_TOOL_LENS));
  rb_define_const(class_input_event, "BTN_TOUCH", INT2NUM(BTN_TOUCH));
  rb_define_const(class_input_event, "BTN_STYLUS", INT2NUM(BTN_STYLUS));
  rb_define_const(class_input_event, "BTN_STYLUS2", INT2NUM(BTN_STYLUS2));
  rb_define_const(class_input_event, "BTN_TOOL_DOUBLETAP", INT2NUM(BTN_TOOL_DOUBLETAP));
  rb_define_const(class_input_event, "BTN_TOOL_TRIPLETAP", INT2NUM(BTN_TOOL_TRIPLETAP));
  rb_define_const(class_input_event, "BTN_TOOL_QUADTAP", INT2NUM(BTN_TOOL_QUADTAP));/* Four fingers on trackpad */

  rb_define_const(class_input_event, "BTN_WHEEL", INT2NUM(BTN_WHEEL));
  rb_define_const(class_input_event, "BTN_GEAR_DOWN", INT2NUM(BTN_GEAR_DOWN));
  rb_define_const(class_input_event, "BTN_GEAR_UP", INT2NUM(BTN_GEAR_UP));

  rb_define_const(class_input_event, "KEY_OK", INT2NUM(KEY_OK));
  rb_define_const(class_input_event, "KEY_SELECT", INT2NUM(KEY_SELECT));
  rb_define_const(class_input_event, "KEY_GOTO", INT2NUM(KEY_GOTO));
  rb_define_const(class_input_event, "KEY_CLEAR", INT2NUM(KEY_CLEAR));
  rb_define_const(class_input_event, "KEY_POWER2", INT2NUM(KEY_POWER2));
  rb_define_const(class_input_event, "KEY_OPTION", INT2NUM(KEY_OPTION));
  rb_define_const(class_input_event, "KEY_INFO", INT2NUM(KEY_INFO));/* AL OEM Features/Tips/Tutorial */
  rb_define_const(class_input_event, "KEY_TIME", INT2NUM(KEY_TIME));
  rb_define_const(class_input_event, "KEY_VENDOR", INT2NUM(KEY_VENDOR));
  rb_define_const(class_input_event, "KEY_ARCHIVE", INT2NUM(KEY_ARCHIVE));
  rb_define_const(class_input_event, "KEY_PROGRAM", INT2NUM(KEY_PROGRAM));/* Media Select Program Guide */
  rb_define_const(class_input_event, "KEY_CHANNEL", INT2NUM(KEY_CHANNEL));
  rb_define_const(class_input_event, "KEY_FAVORITES", INT2NUM(KEY_FAVORITES));
  rb_define_const(class_input_event, "KEY_EPG", INT2NUM(KEY_EPG));
  rb_define_const(class_input_event, "KEY_PVR", INT2NUM(KEY_PVR));/* Media Select Home */
  rb_define_const(class_input_event, "KEY_MHP", INT2NUM(KEY_MHP));
  rb_define_const(class_input_event, "KEY_LANGUAGE", INT2NUM(KEY_LANGUAGE));
  rb_define_const(class_input_event, "KEY_TITLE", INT2NUM(KEY_TITLE));
  rb_define_const(class_input_event, "KEY_SUBTITLE", INT2NUM(KEY_SUBTITLE));
  rb_define_const(class_input_event, "KEY_ANGLE", INT2NUM(KEY_ANGLE));
  rb_define_const(class_input_event, "KEY_ZOOM", INT2NUM(KEY_ZOOM));
  rb_define_const(class_input_event, "KEY_MODE", INT2NUM(KEY_MODE));
  rb_define_const(class_input_event, "KEY_KEYBOARD", INT2NUM(KEY_KEYBOARD));
  rb_define_const(class_input_event, "KEY_SCREEN", INT2NUM(KEY_SCREEN));
  rb_define_const(class_input_event, "KEY_PC", INT2NUM(KEY_PC));/* Media Select Computer */
  rb_define_const(class_input_event, "KEY_TV", INT2NUM(KEY_TV));/* Media Select TV */
  rb_define_const(class_input_event, "KEY_TV2", INT2NUM(KEY_TV2));/* Media Select Cable */
  rb_define_const(class_input_event, "KEY_VCR", INT2NUM(KEY_VCR));/* Media Select VCR */
  rb_define_const(class_input_event, "KEY_VCR2", INT2NUM(KEY_VCR2));/* VCR Plus */
  rb_define_const(class_input_event, "KEY_SAT", INT2NUM(KEY_SAT));/* Media Select Satellite */
  rb_define_const(class_input_event, "KEY_SAT2", INT2NUM(KEY_SAT2));
  rb_define_const(class_input_event, "KEY_CD", INT2NUM(KEY_CD));/* Media Select CD */
  rb_define_const(class_input_event, "KEY_TAPE", INT2NUM(KEY_TAPE));/* Media Select Tape */
  rb_define_const(class_input_event, "KEY_RADIO", INT2NUM(KEY_RADIO));
  rb_define_const(class_input_event, "KEY_TUNER", INT2NUM(KEY_TUNER));/* Media Select Tuner */
  rb_define_const(class_input_event, "KEY_PLAYER", INT2NUM(KEY_PLAYER));
  rb_define_const(class_input_event, "KEY_TEXT", INT2NUM(KEY_TEXT));
  rb_define_const(class_input_event, "KEY_DVD", INT2NUM(KEY_DVD));/* Media Select DVD */
  rb_define_const(class_input_event, "KEY_AUX", INT2NUM(KEY_AUX));
  rb_define_const(class_input_event, "KEY_MP3", INT2NUM(KEY_MP3));
  rb_define_const(class_input_event, "KEY_AUDIO", INT2NUM(KEY_AUDIO));/* AL Audio Browser */
  rb_define_const(class_input_event, "KEY_VIDEO", INT2NUM(KEY_VIDEO));/* AL Movie Browser */
  rb_define_const(class_input_event, "KEY_DIRECTORY", INT2NUM(KEY_DIRECTORY));
  rb_define_const(class_input_event, "KEY_LIST", INT2NUM(KEY_LIST));
  rb_define_const(class_input_event, "KEY_MEMO", INT2NUM(KEY_MEMO));/* Media Select Messages */
  rb_define_const(class_input_event, "KEY_CALENDAR", INT2NUM(KEY_CALENDAR));
  rb_define_const(class_input_event, "KEY_RED", INT2NUM(KEY_RED));
  rb_define_const(class_input_event, "KEY_GREEN", INT2NUM(KEY_GREEN));
  rb_define_const(class_input_event, "KEY_YELLOW", INT2NUM(KEY_YELLOW));
  rb_define_const(class_input_event, "KEY_BLUE", INT2NUM(KEY_BLUE));
  rb_define_const(class_input_event, "KEY_CHANNELUP", INT2NUM(KEY_CHANNELUP));/* Channel Increment */
  rb_define_const(class_input_event, "KEY_CHANNELDOWN", INT2NUM(KEY_CHANNELDOWN));/* Channel Decrement */
  rb_define_const(class_input_event, "KEY_FIRST", INT2NUM(KEY_FIRST));
  rb_define_const(class_input_event, "KEY_LAST", INT2NUM(KEY_LAST));/* Recall Last */
  rb_define_const(class_input_event, "KEY_AB", INT2NUM(KEY_AB));
  rb_define_const(class_input_event, "KEY_NEXT", INT2NUM(KEY_NEXT));
  rb_define_const(class_input_event, "KEY_RESTART", INT2NUM(KEY_RESTART));
  rb_define_const(class_input_event, "KEY_SLOW", INT2NUM(KEY_SLOW));
  rb_define_const(class_input_event, "KEY_SHUFFLE", INT2NUM(KEY_SHUFFLE));
  rb_define_const(class_input_event, "KEY_BREAK", INT2NUM(KEY_BREAK));
  rb_define_const(class_input_event, "KEY_PREVIOUS", INT2NUM(KEY_PREVIOUS));
  rb_define_const(class_input_event, "KEY_DIGITS", INT2NUM(KEY_DIGITS));
  rb_define_const(class_input_event, "KEY_TEEN", INT2NUM(KEY_TEEN));
  rb_define_const(class_input_event, "KEY_TWEN", INT2NUM(KEY_TWEN));
  rb_define_const(class_input_event, "KEY_VIDEOPHONE", INT2NUM(KEY_VIDEOPHONE));/* Media Select Video Phone */
  rb_define_const(class_input_event, "KEY_GAMES", INT2NUM(KEY_GAMES));/* Media Select Games */
  rb_define_const(class_input_event, "KEY_ZOOMIN", INT2NUM(KEY_ZOOMIN));/* AC Zoom In */
  rb_define_const(class_input_event, "KEY_ZOOMOUT", INT2NUM(KEY_ZOOMOUT));/* AC Zoom Out */
  rb_define_const(class_input_event, "KEY_ZOOMRESET", INT2NUM(KEY_ZOOMRESET));/* AC Zoom */
  rb_define_const(class_input_event, "KEY_WORDPROCESSOR", INT2NUM(KEY_WORDPROCESSOR));/* AL Word Processor */
  rb_define_const(class_input_event, "KEY_EDITOR", INT2NUM(KEY_EDITOR));/* AL Text Editor */
  rb_define_const(class_input_event, "KEY_SPREADSHEET", INT2NUM(KEY_SPREADSHEET));/* AL Spreadsheet */
  rb_define_const(class_input_event, "KEY_GRAPHICSEDITOR", INT2NUM(KEY_GRAPHICSEDITOR));/* AL Graphics Editor */
  rb_define_const(class_input_event, "KEY_PRESENTATION", INT2NUM(KEY_PRESENTATION));/* AL Presentation App */
  rb_define_const(class_input_event, "KEY_DATABASE", INT2NUM(KEY_DATABASE));/* AL Database App */
  rb_define_const(class_input_event, "KEY_NEWS", INT2NUM(KEY_NEWS));/* AL Newsreader */
  rb_define_const(class_input_event, "KEY_VOICEMAIL", INT2NUM(KEY_VOICEMAIL));/* AL Voicemail */
  rb_define_const(class_input_event, "KEY_ADDRESSBOOK", INT2NUM(KEY_ADDRESSBOOK));/* AL Contacts/Address Book */
  rb_define_const(class_input_event, "KEY_MESSENGER", INT2NUM(KEY_MESSENGER));/* AL Instant Messaging */
  rb_define_const(class_input_event, "KEY_DISPLAYTOGGLE", INT2NUM(KEY_DISPLAYTOGGLE));/* Turn display (LCD) on and off */
  rb_define_const(class_input_event, "KEY_SPELLCHECK", INT2NUM(KEY_SPELLCHECK));/* AL Spell Check */
  rb_define_const(class_input_event, "KEY_LOGOFF", INT2NUM(KEY_LOGOFF));/* AL Logoff */

  rb_define_const(class_input_event, "KEY_DOLLAR", INT2NUM(KEY_DOLLAR));
  rb_define_const(class_input_event, "KEY_EURO", INT2NUM(KEY_EURO));

  rb_define_const(class_input_event, "KEY_FRAMEBACK", INT2NUM(KEY_FRAMEBACK));/* Consumer - transport controls */
  rb_define_const(class_input_event, "KEY_FRAMEFORWARD", INT2NUM(KEY_FRAMEFORWARD));
  rb_define_const(class_input_event, "KEY_CONTEXT_MENU", INT2NUM(KEY_CONTEXT_MENU));/* GenDesc - system context menu */
  rb_define_const(class_input_event, "KEY_MEDIA_REPEAT", INT2NUM(KEY_MEDIA_REPEAT));/* Consumer - transport control */
  rb_define_const(class_input_event, "KEY_10CHANNELSUP", INT2NUM(KEY_10CHANNELSUP));/* 10 channels up (10+) */
  rb_define_const(class_input_event, "KEY_10CHANNELSDOWN", INT2NUM(KEY_10CHANNELSDOWN));/* 10 channels down (10-) */
  rb_define_const(class_input_event, "KEY_IMAGES", INT2NUM(KEY_IMAGES));/* AL Image Browser */

  rb_define_const(class_input_event, "KEY_DEL_EOL", INT2NUM(KEY_DEL_EOL));
  rb_define_const(class_input_event, "KEY_DEL_EOS", INT2NUM(KEY_DEL_EOS));
  rb_define_const(class_input_event, "KEY_INS_LINE", INT2NUM(KEY_INS_LINE));
  rb_define_const(class_input_event, "KEY_DEL_LINE", INT2NUM(KEY_DEL_LINE));

  rb_define_const(class_input_event, "KEY_FN", INT2NUM(KEY_FN));
  rb_define_const(class_input_event, "KEY_FN_ESC", INT2NUM(KEY_FN_ESC));
  rb_define_const(class_input_event, "KEY_FN_F1", INT2NUM(KEY_FN_F1));
  rb_define_const(class_input_event, "KEY_FN_F2", INT2NUM(KEY_FN_F2));
  rb_define_const(class_input_event, "KEY_FN_F3", INT2NUM(KEY_FN_F3));
  rb_define_const(class_input_event, "KEY_FN_F4", INT2NUM(KEY_FN_F4));
  rb_define_const(class_input_event, "KEY_FN_F5", INT2NUM(KEY_FN_F5));
  rb_define_const(class_input_event, "KEY_FN_F6", INT2NUM(KEY_FN_F6));
  rb_define_const(class_input_event, "KEY_FN_F7", INT2NUM(KEY_FN_F7));
  rb_define_const(class_input_event, "KEY_FN_F8", INT2NUM(KEY_FN_F8));
  rb_define_const(class_input_event, "KEY_FN_F9", INT2NUM(KEY_FN_F9));
  rb_define_const(class_input_event, "KEY_FN_F10", INT2NUM(KEY_FN_F10));
  rb_define_const(class_input_event, "KEY_FN_F11", INT2NUM(KEY_FN_F11));
  rb_define_const(class_input_event, "KEY_FN_F12", INT2NUM(KEY_FN_F12));
  rb_define_const(class_input_event, "KEY_FN_1", INT2NUM(KEY_FN_1));
  rb_define_const(class_input_event, "KEY_FN_2", INT2NUM(KEY_FN_2));
  rb_define_const(class_input_event, "KEY_FN_D", INT2NUM(KEY_FN_D));
  rb_define_const(class_input_event, "KEY_FN_E", INT2NUM(KEY_FN_E));
  rb_define_const(class_input_event, "KEY_FN_F", INT2NUM(KEY_FN_F));
  rb_define_const(class_input_event, "KEY_FN_S", INT2NUM(KEY_FN_S));
  rb_define_const(class_input_event, "KEY_FN_B", INT2NUM(KEY_FN_B));

  rb_define_const(class_input_event, "KEY_BRL_DOT1", INT2NUM(KEY_BRL_DOT1));
  rb_define_const(class_input_event, "KEY_BRL_DOT2", INT2NUM(KEY_BRL_DOT2));
  rb_define_const(class_input_event, "KEY_BRL_DOT3", INT2NUM(KEY_BRL_DOT3));
  rb_define_const(class_input_event, "KEY_BRL_DOT4", INT2NUM(KEY_BRL_DOT4));
  rb_define_const(class_input_event, "KEY_BRL_DOT5", INT2NUM(KEY_BRL_DOT5));
  rb_define_const(class_input_event, "KEY_BRL_DOT6", INT2NUM(KEY_BRL_DOT6));
  rb_define_const(class_input_event, "KEY_BRL_DOT7", INT2NUM(KEY_BRL_DOT7));
  rb_define_const(class_input_event, "KEY_BRL_DOT8", INT2NUM(KEY_BRL_DOT8));
  rb_define_const(class_input_event, "KEY_BRL_DOT9", INT2NUM(KEY_BRL_DOT9));
  rb_define_const(class_input_event, "KEY_BRL_DOT10", INT2NUM(KEY_BRL_DOT10));

  rb_define_const(class_input_event, "KEY_NUMERIC_0", INT2NUM(KEY_NUMERIC_0));/* used by phones, remote controls, */
  rb_define_const(class_input_event, "KEY_NUMERIC_1", INT2NUM(KEY_NUMERIC_1));/* and other keypads */
  rb_define_const(class_input_event, "KEY_NUMERIC_2", INT2NUM(KEY_NUMERIC_2));
  rb_define_const(class_input_event, "KEY_NUMERIC_3", INT2NUM(KEY_NUMERIC_3));
  rb_define_const(class_input_event, "KEY_NUMERIC_4", INT2NUM(KEY_NUMERIC_4));
  rb_define_const(class_input_event, "KEY_NUMERIC_5", INT2NUM(KEY_NUMERIC_5));
  rb_define_const(class_input_event, "KEY_NUMERIC_6", INT2NUM(KEY_NUMERIC_6));
  rb_define_const(class_input_event, "KEY_NUMERIC_7", INT2NUM(KEY_NUMERIC_7));
  rb_define_const(class_input_event, "KEY_NUMERIC_8", INT2NUM(KEY_NUMERIC_8));
  rb_define_const(class_input_event, "KEY_NUMERIC_9", INT2NUM(KEY_NUMERIC_9));
  rb_define_const(class_input_event, "KEY_NUMERIC_STAR", INT2NUM(KEY_NUMERIC_STAR));
  rb_define_const(class_input_event, "KEY_NUMERIC_POUND", INT2NUM(KEY_NUMERIC_POUND));

  rb_define_const(class_input_event, "KEY_CAMERA_FOCUS", INT2NUM(KEY_CAMERA_FOCUS));
  rb_define_const(class_input_event, "KEY_WPS_BUTTON", INT2NUM(KEY_WPS_BUTTON));/* WiFi Protected Setup key */

  rb_define_const(class_input_event, "KEY_TOUCHPAD_TOGGLE", INT2NUM(KEY_TOUCHPAD_TOGGLE));/* Request switch touchpad on or off */
  rb_define_const(class_input_event, "KEY_TOUCHPAD_ON", INT2NUM(KEY_TOUCHPAD_ON));
  rb_define_const(class_input_event, "KEY_TOUCHPAD_OFF", INT2NUM(KEY_TOUCHPAD_OFF));

  rb_define_const(class_input_event, "KEY_CAMERA_ZOOMIN", INT2NUM(KEY_CAMERA_ZOOMIN));
  rb_define_const(class_input_event, "KEY_CAMERA_ZOOMOUT", INT2NUM(KEY_CAMERA_ZOOMOUT));
  rb_define_const(class_input_event, "KEY_CAMERA_UP", INT2NUM(KEY_CAMERA_UP));
  rb_define_const(class_input_event, "KEY_CAMERA_DOWN", INT2NUM(KEY_CAMERA_DOWN));
  rb_define_const(class_input_event, "KEY_CAMERA_LEFT", INT2NUM(KEY_CAMERA_LEFT));
  rb_define_const(class_input_event, "KEY_CAMERA_RIGHT", INT2NUM(KEY_CAMERA_RIGHT));

  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY", INT2NUM(BTN_TRIGGER_HAPPY));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY1", INT2NUM(BTN_TRIGGER_HAPPY1));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY2", INT2NUM(BTN_TRIGGER_HAPPY2));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY3", INT2NUM(BTN_TRIGGER_HAPPY3));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY4", INT2NUM(BTN_TRIGGER_HAPPY4));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY5", INT2NUM(BTN_TRIGGER_HAPPY5));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY6", INT2NUM(BTN_TRIGGER_HAPPY6));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY7", INT2NUM(BTN_TRIGGER_HAPPY7));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY8", INT2NUM(BTN_TRIGGER_HAPPY8));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY9", INT2NUM(BTN_TRIGGER_HAPPY9));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY10", INT2NUM(BTN_TRIGGER_HAPPY10));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY11", INT2NUM(BTN_TRIGGER_HAPPY11));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY12", INT2NUM(BTN_TRIGGER_HAPPY12));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY13", INT2NUM(BTN_TRIGGER_HAPPY13));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY14", INT2NUM(BTN_TRIGGER_HAPPY14));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY15", INT2NUM(BTN_TRIGGER_HAPPY15));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY16", INT2NUM(BTN_TRIGGER_HAPPY16));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY17", INT2NUM(BTN_TRIGGER_HAPPY17));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY18", INT2NUM(BTN_TRIGGER_HAPPY18));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY19", INT2NUM(BTN_TRIGGER_HAPPY19));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY20", INT2NUM(BTN_TRIGGER_HAPPY20));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY21", INT2NUM(BTN_TRIGGER_HAPPY21));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY22", INT2NUM(BTN_TRIGGER_HAPPY22));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY23", INT2NUM(BTN_TRIGGER_HAPPY23));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY24", INT2NUM(BTN_TRIGGER_HAPPY24));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY25", INT2NUM(BTN_TRIGGER_HAPPY25));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY26", INT2NUM(BTN_TRIGGER_HAPPY26));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY27", INT2NUM(BTN_TRIGGER_HAPPY27));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY28", INT2NUM(BTN_TRIGGER_HAPPY28));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY29", INT2NUM(BTN_TRIGGER_HAPPY29));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY30", INT2NUM(BTN_TRIGGER_HAPPY30));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY31", INT2NUM(BTN_TRIGGER_HAPPY31));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY32", INT2NUM(BTN_TRIGGER_HAPPY32));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY33", INT2NUM(BTN_TRIGGER_HAPPY33));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY34", INT2NUM(BTN_TRIGGER_HAPPY34));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY35", INT2NUM(BTN_TRIGGER_HAPPY35));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY36", INT2NUM(BTN_TRIGGER_HAPPY36));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY37", INT2NUM(BTN_TRIGGER_HAPPY37));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY38", INT2NUM(BTN_TRIGGER_HAPPY38));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY39", INT2NUM(BTN_TRIGGER_HAPPY39));
  rb_define_const(class_input_event, "BTN_TRIGGER_HAPPY40", INT2NUM(BTN_TRIGGER_HAPPY40));

  /* We avoid low common keys in module aliases so they don't get huge. */
  rb_define_const(class_input_event, "KEY_MIN_INTERESTING", INT2NUM(KEY_MIN_INTERESTING));
  rb_define_const(class_input_event, "KEY_MAX", INT2NUM(KEY_MAX));
  rb_define_const(class_input_event, "KEY_CNT", INT2NUM(KEY_CNT));

  /*
   * Relative axes
   */

  rb_define_const(class_input_event, "REL_X", INT2NUM(REL_X));
  rb_define_const(class_input_event, "REL_Y", INT2NUM(REL_Y));
  rb_define_const(class_input_event, "REL_Z", INT2NUM(REL_Z));
  rb_define_const(class_input_event, "REL_RX", INT2NUM(REL_RX));
  rb_define_const(class_input_event, "REL_RY", INT2NUM(REL_RY));
  rb_define_const(class_input_event, "REL_RZ", INT2NUM(REL_RZ));
  rb_define_const(class_input_event, "REL_HWHEEL", INT2NUM(REL_HWHEEL));
  rb_define_const(class_input_event, "REL_DIAL", INT2NUM(REL_DIAL));
  rb_define_const(class_input_event, "REL_WHEEL", INT2NUM(REL_WHEEL));
  rb_define_const(class_input_event, "REL_MISC", INT2NUM(REL_MISC));
  rb_define_const(class_input_event, "REL_MAX", INT2NUM(REL_MAX));
  rb_define_const(class_input_event, "REL_CNT", INT2NUM(REL_CNT));

  /*
   * Absolute axes
   */

  rb_define_const(class_input_event, "ABS_X", INT2NUM(ABS_X));
  rb_define_const(class_input_event, "ABS_Y", INT2NUM(ABS_Y));
  rb_define_const(class_input_event, "ABS_Z", INT2NUM(ABS_Z));
  rb_define_const(class_input_event, "ABS_RX", INT2NUM(ABS_RX));
  rb_define_const(class_input_event, "ABS_RY", INT2NUM(ABS_RY));
  rb_define_const(class_input_event, "ABS_RZ", INT2NUM(ABS_RZ));
  rb_define_const(class_input_event, "ABS_THROTTLE", INT2NUM(ABS_THROTTLE));
  rb_define_const(class_input_event, "ABS_RUDDER", INT2NUM(ABS_RUDDER));
  rb_define_const(class_input_event, "ABS_WHEEL", INT2NUM(ABS_WHEEL));
  rb_define_const(class_input_event, "ABS_GAS", INT2NUM(ABS_GAS));
  rb_define_const(class_input_event, "ABS_BRAKE", INT2NUM(ABS_BRAKE));
  rb_define_const(class_input_event, "ABS_HAT0X", INT2NUM(ABS_HAT0X));
  rb_define_const(class_input_event, "ABS_HAT0Y", INT2NUM(ABS_HAT0Y));
  rb_define_const(class_input_event, "ABS_HAT1X", INT2NUM(ABS_HAT1X));
  rb_define_const(class_input_event, "ABS_HAT1Y", INT2NUM(ABS_HAT1Y));
  rb_define_const(class_input_event, "ABS_HAT2X", INT2NUM(ABS_HAT2X));
  rb_define_const(class_input_event, "ABS_HAT2Y", INT2NUM(ABS_HAT2Y));
  rb_define_const(class_input_event, "ABS_HAT3X", INT2NUM(ABS_HAT3X));
  rb_define_const(class_input_event, "ABS_HAT3Y", INT2NUM(ABS_HAT3Y));
  rb_define_const(class_input_event, "ABS_PRESSURE", INT2NUM(ABS_PRESSURE));
  rb_define_const(class_input_event, "ABS_DISTANCE", INT2NUM(ABS_DISTANCE));
  rb_define_const(class_input_event, "ABS_TILT_X", INT2NUM(ABS_TILT_X));
  rb_define_const(class_input_event, "ABS_TILT_Y", INT2NUM(ABS_TILT_Y));
  rb_define_const(class_input_event, "ABS_TOOL_WIDTH", INT2NUM(ABS_TOOL_WIDTH));

  rb_define_const(class_input_event, "ABS_VOLUME", INT2NUM(ABS_VOLUME));

  rb_define_const(class_input_event, "ABS_MISC", INT2NUM(ABS_MISC));

  rb_define_const(class_input_event, "ABS_MT_SLOT", INT2NUM(ABS_MT_SLOT));/* MT slot being modified */
  rb_define_const(class_input_event, "ABS_MT_TOUCH_MAJOR", INT2NUM(ABS_MT_TOUCH_MAJOR));/* Major axis of touching ellipse */
  rb_define_const(class_input_event, "ABS_MT_TOUCH_MINOR", INT2NUM(ABS_MT_TOUCH_MINOR));/* Minor axis (omit if circular) */
  rb_define_const(class_input_event, "ABS_MT_WIDTH_MAJOR", INT2NUM(ABS_MT_WIDTH_MAJOR));/* Major axis of approaching ellipse */
  rb_define_const(class_input_event, "ABS_MT_WIDTH_MINOR", INT2NUM(ABS_MT_WIDTH_MINOR));/* Minor axis (omit if circular) */
  rb_define_const(class_input_event, "ABS_MT_ORIENTATION", INT2NUM(ABS_MT_ORIENTATION));/* Ellipse orientation */
  rb_define_const(class_input_event, "ABS_MT_POSITION_X", INT2NUM(ABS_MT_POSITION_X));/* Center X ellipse position */
  rb_define_const(class_input_event, "ABS_MT_POSITION_Y", INT2NUM(ABS_MT_POSITION_Y));/* Center Y ellipse position */
  rb_define_const(class_input_event, "ABS_MT_TOOL_TYPE", INT2NUM(ABS_MT_TOOL_TYPE));/* Type of touching device */
  rb_define_const(class_input_event, "ABS_MT_BLOB_ID", INT2NUM(ABS_MT_BLOB_ID));/* Group a set of packets as a blob */
  rb_define_const(class_input_event, "ABS_MT_TRACKING_ID", INT2NUM(ABS_MT_TRACKING_ID));/* Unique ID of initiated contact */
  rb_define_const(class_input_event, "ABS_MT_PRESSURE", INT2NUM(ABS_MT_PRESSURE));/* Pressure on contact area */
  rb_define_const(class_input_event, "ABS_MT_DISTANCE", INT2NUM(ABS_MT_DISTANCE));/* Contact hover distance */


  rb_define_const(class_input_event, "ABS_MAX", INT2NUM(ABS_MAX));
  rb_define_const(class_input_event, "ABS_CNT", INT2NUM(ABS_CNT));

  /*
   * Switch events
   */

  rb_define_const(class_input_event, "SW_LID", INT2NUM(SW_LID));/* set = lid shut */
  rb_define_const(class_input_event, "SW_TABLET_MODE", INT2NUM(SW_TABLET_MODE));/* set = tablet mode */
  rb_define_const(class_input_event, "SW_HEADPHONE_INSERT", INT2NUM(SW_HEADPHONE_INSERT));/* set = inserted */
  rb_define_const(class_input_event, "SW_RFKILL_ALL", INT2NUM(SW_RFKILL_ALL));/* rfkill master switch, type "any"
                                                                                 set = radio enabled */
  rb_define_const(class_input_event, "SW_RADIO", INT2NUM(SW_RADIO));/* deprecated */
  rb_define_const(class_input_event, "SW_MICROPHONE_INSERT", INT2NUM(SW_MICROPHONE_INSERT));/* set = inserted */
  rb_define_const(class_input_event, "SW_DOCK", INT2NUM(SW_DOCK));/* set = plugged into dock */
  rb_define_const(class_input_event, "SW_LINEOUT_INSERT", INT2NUM(SW_LINEOUT_INSERT));/* set = inserted */
  rb_define_const(class_input_event, "SW_JACK_PHYSICAL_INSERT", INT2NUM(SW_JACK_PHYSICAL_INSERT));/* set = mechanical switch set */
  rb_define_const(class_input_event, "SW_VIDEOOUT_INSERT", INT2NUM(SW_VIDEOOUT_INSERT));/* set = inserted */
  rb_define_const(class_input_event, "SW_CAMERA_LENS_COVER", INT2NUM(SW_CAMERA_LENS_COVER));/* set = lens covered */
  rb_define_const(class_input_event, "SW_KEYPAD_SLIDE", INT2NUM(SW_KEYPAD_SLIDE));/* set = keypad slide out */
  rb_define_const(class_input_event, "SW_FRONT_PROXIMITY", INT2NUM(SW_FRONT_PROXIMITY));/* set = front proximity sensor active */
  rb_define_const(class_input_event, "SW_ROTATE_LOCK", INT2NUM(SW_ROTATE_LOCK));/* set = rotate locked/disabled */
  rb_define_const(class_input_event, "SW_MAX", INT2NUM(SW_MAX));
  rb_define_const(class_input_event, "SW_CNT", INT2NUM(SW_CNT));

  /*
   * Misc events
   */

  rb_define_const(class_input_event, "MSC_SERIAL", INT2NUM(MSC_SERIAL));
  rb_define_const(class_input_event, "MSC_PULSELED", INT2NUM(MSC_PULSELED));
  rb_define_const(class_input_event, "MSC_GESTURE", INT2NUM(MSC_GESTURE));
  rb_define_const(class_input_event, "MSC_RAW", INT2NUM(MSC_RAW));
  rb_define_const(class_input_event, "MSC_SCAN", INT2NUM(MSC_SCAN));
  rb_define_const(class_input_event, "MSC_MAX", INT2NUM(MSC_MAX));
  rb_define_const(class_input_event, "MSC_CNT", INT2NUM(MSC_CNT));

  /*
   * LEDs
   */

  rb_define_const(class_input_event, "LED_NUML", INT2NUM(LED_NUML));
  rb_define_const(class_input_event, "LED_CAPSL", INT2NUM(LED_CAPSL));
  rb_define_const(class_input_event, "LED_SCROLLL", INT2NUM(LED_SCROLLL));
  rb_define_const(class_input_event, "LED_COMPOSE", INT2NUM(LED_COMPOSE));
  rb_define_const(class_input_event, "LED_KANA", INT2NUM(LED_KANA));
  rb_define_const(class_input_event, "LED_SLEEP", INT2NUM(LED_SLEEP));
  rb_define_const(class_input_event, "LED_SUSPEND", INT2NUM(LED_SUSPEND));
  rb_define_const(class_input_event, "LED_MUTE", INT2NUM(LED_MUTE));
  rb_define_const(class_input_event, "LED_MISC", INT2NUM(LED_MISC));
  rb_define_const(class_input_event, "LED_MAIL", INT2NUM(LED_MAIL));
  rb_define_const(class_input_event, "LED_CHARGING", INT2NUM(LED_CHARGING));
  rb_define_const(class_input_event, "LED_MAX", INT2NUM(LED_MAX));
  rb_define_const(class_input_event, "LED_CNT", INT2NUM(LED_CNT));

  /*
   * Autorepeat values
   */

  rb_define_const(class_input_event, "REP_DELAY", INT2NUM(REP_DELAY));
  rb_define_const(class_input_event, "REP_PERIOD", INT2NUM(REP_PERIOD));
  rb_define_const(class_input_event, "REP_MAX", INT2NUM(REP_MAX));
  rb_define_const(class_input_event, "REP_CNT", INT2NUM(REP_CNT));

  /*
   * Sounds
   */

  rb_define_const(class_input_event, "SND_CLICK", INT2NUM(SND_CLICK));
  rb_define_const(class_input_event, "SND_BELL", INT2NUM(SND_BELL));
  rb_define_const(class_input_event, "SND_TONE", INT2NUM(SND_TONE));
  rb_define_const(class_input_event, "SND_MAX", INT2NUM(SND_MAX));
  rb_define_const(class_input_event, "SND_CNT", INT2NUM(SND_CNT));

  /*
   * IDs.
   */

  rb_define_const(class_input_event, "ID_BUS", INT2NUM(ID_BUS));
  rb_define_const(class_input_event, "ID_VENDOR", INT2NUM(ID_VENDOR));
  rb_define_const(class_input_event, "ID_PRODUCT", INT2NUM(ID_PRODUCT));
  rb_define_const(class_input_event, "ID_VERSION", INT2NUM(ID_VERSION));

  rb_define_const(class_input_event, "BUS_PCI", INT2NUM(BUS_PCI));
  rb_define_const(class_input_event, "BUS_ISAPNP", INT2NUM(BUS_ISAPNP));
  rb_define_const(class_input_event, "BUS_USB", INT2NUM(BUS_USB));
  rb_define_const(class_input_event, "BUS_HIL", INT2NUM(BUS_HIL));
  rb_define_const(class_input_event, "BUS_BLUETOOTH", INT2NUM(BUS_BLUETOOTH));
  rb_define_const(class_input_event, "BUS_VIRTUAL", INT2NUM(BUS_VIRTUAL));

  rb_define_const(class_input_event, "BUS_ISA", INT2NUM(BUS_ISA));
  rb_define_const(class_input_event, "BUS_I8042", INT2NUM(BUS_I8042));
  rb_define_const(class_input_event, "BUS_XTKBD", INT2NUM(BUS_XTKBD));
  rb_define_const(class_input_event, "BUS_RS232", INT2NUM(BUS_RS232));
  rb_define_const(class_input_event, "BUS_GAMEPORT", INT2NUM(BUS_GAMEPORT));
  rb_define_const(class_input_event, "BUS_PARPORT", INT2NUM(BUS_PARPORT));
  rb_define_const(class_input_event, "BUS_AMIGA", INT2NUM(BUS_AMIGA));
  rb_define_const(class_input_event, "BUS_ADB", INT2NUM(BUS_ADB));
  rb_define_const(class_input_event, "BUS_I2C", INT2NUM(BUS_I2C));
  rb_define_const(class_input_event, "BUS_HOST", INT2NUM(BUS_HOST));
  rb_define_const(class_input_event, "BUS_GSC", INT2NUM(BUS_GSC));
  rb_define_const(class_input_event, "BUS_ATARI", INT2NUM(BUS_ATARI));
  rb_define_const(class_input_event, "BUS_SPI", INT2NUM(BUS_SPI));

  /*
   * MT_TOOL types
   */
  rb_define_const(class_input_event, "MT_TOOL_FINGER", INT2NUM(MT_TOOL_FINGER));
  rb_define_const(class_input_event, "MT_TOOL_PEN", INT2NUM(MT_TOOL_PEN));
  rb_define_const(class_input_event, "MT_TOOL_MAX", INT2NUM(MT_TOOL_MAX));

  /*
   * Values describing the status of a force-feedback effect
   */
  rb_define_const(class_input_event, "FF_STATUS_STOPPED", INT2NUM(FF_STATUS_STOPPED));
  rb_define_const(class_input_event, "FF_STATUS_PLAYING", INT2NUM(FF_STATUS_PLAYING));
  rb_define_const(class_input_event, "FF_STATUS_MAX", INT2NUM(FF_STATUS_MAX));


  /*
   * Force feedback effect types
   */

  rb_define_const(class_input_event, "FF_RUMBLE", INT2NUM(FF_RUMBLE));
  rb_define_const(class_input_event, "FF_PERIODIC", INT2NUM(FF_PERIODIC));
  rb_define_const(class_input_event, "FF_CONSTANT", INT2NUM(FF_CONSTANT));
  rb_define_const(class_input_event, "FF_SPRING", INT2NUM(FF_SPRING));
  rb_define_const(class_input_event, "FF_FRICTION", INT2NUM(FF_FRICTION));
  rb_define_const(class_input_event, "FF_DAMPER", INT2NUM(FF_DAMPER));
  rb_define_const(class_input_event, "FF_INERTIA", INT2NUM(FF_INERTIA));
  rb_define_const(class_input_event, "FF_RAMP", INT2NUM(FF_RAMP));

  rb_define_const(class_input_event, "FF_EFFECT_MIN", INT2NUM(FF_EFFECT_MIN));
  rb_define_const(class_input_event, "FF_EFFECT_MAX", INT2NUM(FF_EFFECT_MAX));

  /*
   * Force feedback periodic effect types
   */

  rb_define_const(class_input_event, "FF_SQUARE", INT2NUM(FF_SQUARE));
  rb_define_const(class_input_event, "FF_TRIANGLE", INT2NUM(FF_TRIANGLE));
  rb_define_const(class_input_event, "FF_SINE", INT2NUM(FF_SINE));
  rb_define_const(class_input_event, "FF_SAW_UP", INT2NUM(FF_SAW_UP));
  rb_define_const(class_input_event, "FF_SAW_DOWN", INT2NUM(FF_SAW_DOWN));
  rb_define_const(class_input_event, "FF_CUSTOM", INT2NUM(FF_CUSTOM));

  rb_define_const(class_input_event, "FF_WAVEFORM_MIN", INT2NUM(FF_WAVEFORM_MIN));
  rb_define_const(class_input_event, "FF_WAVEFORM_MAX", INT2NUM(FF_WAVEFORM_MAX));

  /*
   * Set ff device properties
   */

  rb_define_const(class_input_event, "FF_GAIN", INT2NUM(FF_GAIN));
  rb_define_const(class_input_event, "FF_AUTOCENTER", INT2NUM(FF_AUTOCENTER));

  rb_define_const(class_input_event, "FF_MAX", INT2NUM(FF_MAX));
  rb_define_const(class_input_event, "FF_CNT", INT2NUM(FF_CNT));

}
