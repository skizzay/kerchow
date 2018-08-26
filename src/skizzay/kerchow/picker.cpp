#include "skizzay/kerchow/picker.h"
#include <cstdlib>
#include <sstream>

namespace {

inline unsigned int generate_seed_for_picker() {
   char *buffer = ::getenv("SKIZZAY_KERCHOW_SEED");

   if (nullptr != buffer) {
      unsigned int value;
      std::istringstream s{buffer};

      s >> value;
      if (s) {
         return value;
      }
   }

#if defined(KERCHOW_USE_DEV_URANDOM)
   std::random_device generator{"/dev/urandom"};
#elif defined(KERCHOW_USE_DEV_RANDOM)
   std::random_device generator{"/dev/random"};
#else
   std::random_device generator;
#endif

   return generator();
}

}


namespace skizzay {
namespace kerchow {

random_picker picker{generate_seed_for_picker()};

}
}
