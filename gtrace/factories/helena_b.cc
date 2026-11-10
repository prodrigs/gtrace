#include <gtrace/boxes/helena_b.hh>

std::unique_ptr<field_box_t> create_linked_field_box(
    const argh::parser& arghs) {
  return std::move(std::make_unique<helena_b>(arghs));
}
