//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_SYSTEM_MESSAGE_HPP
#define SC2_NBDL_SYSTEM_MESSAGE_HPP

#include <nbdl.hpp>
#include <string>

namespace sc2_nbdl {
  struct system_message {
    int code;
    std::string text;
  };
}

namespace nbdl {
  NBDL_ENTITY(sc2_nbdl::system_message, code, text);
}

#endif
