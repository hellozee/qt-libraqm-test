#ifndef SVGHANDLER_H
#define SVGHANDLER_H

#include <rapidxml_ns/rapidxml_ns.hpp>
#include <svgpp/policy/xml/rapidxml_ns.hpp>
#include <svgpp/svgpp.hpp>

typedef rapidxml_ns::xml_node<> const * xml_element_t;

class Context
{
public:
  void on_enter_element(svgpp::tag::element::any)
  {}

  void on_exit_element()
  {}

  void path_move_to(double x, double y, svgpp::tag::coordinate::absolute)
  {}

  void path_line_to(double x, double y, svgpp::tag::coordinate::absolute)
  {}

  void path_cubic_bezier_to(
    double x1, double y1,
    double x2, double y2,
    double x, double y,
    svgpp::tag::coordinate::absolute)
  {}

  void path_quadratic_bezier_to(
    double x1, double y1,
    double x, double y,
    svgpp::tag::coordinate::absolute)
  {}

  void path_elliptical_arc_to(
    double rx, double ry, double x_axis_rotation,
    bool large_arc_flag, bool sweep_flag,
    double x, double y,
    svgpp::tag::coordinate::absolute)
  {}

  void path_close_subpath()
  {}

  void path_exit()
  {}
};

typedef
  boost::mpl::set<
    // SVG Structural Elements
    svgpp::tag::element::svg,
    svgpp::tag::element::g,
    // SVG Shape Elements
    svgpp::tag::element::circle,
    svgpp::tag::element::ellipse,
    svgpp::tag::element::line,
    svgpp::tag::element::path,
    svgpp::tag::element::polygon,
    svgpp::tag::element::polyline,
    svgpp::tag::element::rect
  >::type processed_elements_t;


class SvgHandler
{
public:
    SvgHandler();
};

#endif // SVGHANDLER_H
