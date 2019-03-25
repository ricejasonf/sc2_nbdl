#include <sc2_nbdl/web/context.hpp>

#include <nbdl.hpp>
#include <nbdl/js.hpp>

int main() {
  nbdl::js::init();

  static auto context = sc2_nbdl::web::make_context(
    nbdl::actor("web_main", nbdl::js::get_element_by_id("web_main"))
  );

  // Events are driven by javascript so things
  // need to stay alive even after main exits.
}
