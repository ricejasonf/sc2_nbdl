//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_SERVER_SERIALIZER_HPP
#define SC2_NBDL_SERVER_SERIALIZER_HPP

#include <sc2_nbdl/server/context.hpp>

#include <nbdl.hpp>
#include <nbdl/binder/jsoncpp.hpp>

namespace sc2_nbdl::server {
  struct serializer {
    std::string buffer = {};

    serializer(serializer const&) = delete;

    // converts to the downstream variant type
    // and serializes
    template <typename Message>
    std::string& serialize(Message&& m) {
      using variant = api::downstream_variant;
      using nbdl::binder::jsoncpp::to_string;
      buffer = to_string(variant(std::forward<Message>(m)));
      return (buffer);
    }
  };
}

#endif
