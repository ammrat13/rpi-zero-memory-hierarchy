#include "perfcount.h"

const char *perfcount_event_name(perfcount_event_t event) {
  switch(event) {

  #define XX(num, name) \
    case PERFCOUNT_EVENT_ ## name : \
      return #name ;
  #include "perfcount-types.h"

  default:
    return "unknown";
  }
}
