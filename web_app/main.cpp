#include <sc2_nbdl/web/context.hpp>

#include <nbdl.hpp>
#include <nbdl/js.hpp>

int main() {
  nbdl::js::init();

  // auto web_main = nbdl::js::get_element_by_id("web_main")
  // ^ not working in debug mode for some reason
  nbdl::js::val web_main{};
  NBDL_JS_TRANSFORM(web_main, function() {
    return document.getElementById('web_main');
  });

  static auto context = sc2_nbdl::web::make_context(
    nbdl::actor("web_main", web_main)
  );

  // Events are driven by javascript so things
  // need to stay alive even after main exits.
}
