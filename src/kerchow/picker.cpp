// vim: sw=3 ts=3 expandtab smartindent autoindent cindent
#include "kerchow/picker.h"
#include <cstdlib>
#include <sstream>

namespace {

inline unsigned int generate_seed_for_picker() {
   char *buffer = ::getenv("KERCHOW_SEED");

   if (buffer) {
      unsigned int value;
      std::istringstream s{buffer};

      s >> value;
      if (s) {
         return value;
      }
   }

#ifdef __linux__
# ifndef KERCHOW_USE_DEV_RANDOM
   std::random_device generator{"/dev/urandom"};
#  else
   std::random_device generator{"/dev/random"};
#  endif
#else
   std::random_device generator{"/dev/urandom"};
#endif

   return generator();
}

}


namespace kerchow {

random_picker picker{generate_seed_for_picker()};

}
