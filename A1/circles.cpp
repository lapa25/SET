#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

int main() {
  double x1 = 1;
  double y1 = 1;
  double r1 = 1;

  double x2 = 1.5;
  double y2 = 2;
  double r2 = std::sqrt(5) / 2;

  double x3 = 2;
  double y3 = 1.5;
  double r3 = std::sqrt(5) / 2;

  double area = 0.25 * M_PI + 1.25 * std::asin(0.8) - 1;

  double right_xl = std::max({x1 + r1, x2 + r2, x3 + r3});
  double up_yl = std::max({y1 + r1, y2 + r2, y3 + r3});
  double left_xl = std::min({x1 - r1, x2 - r2, x3 - r3});
  double down_yl = std::min({y1 - r1, y2 - r2, y3 - r3});

  double right_xs = std::min({x1 + r1, x2 + r2, x3 + r3});
  double up_ys = std::min({y1 + r1, y2 + r2, y3 + r3});
  double left_xs = std::max({x1 - r1, x2 - r2, x3 - r3});
  double down_ys = std::max({y1 - r1, y2 - r2, y3 - r3});

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> xl(left_xl, right_xl);
  std::uniform_real_distribution<double> yl(down_yl, up_yl);
  std::uniform_real_distribution<double> xs(left_xs, right_xs);
  std::uniform_real_distribution<double> ys(down_ys, up_ys);

  std::ofstream out("results.csv");
  out << "N,Small,Large,Small_Error,Large_Error\n";

  double sl = (right_xl - left_xl) * (up_yl - down_yl);
  double ss = (right_xs - left_xs) * (up_ys - down_ys);

  for (int n = 100; n <= 100000; n += 500) {
    int ml = 0;
    int ms = 0;

    for (int i = 0; i < n; ++i) {
      double x = xl(gen);
      double y = yl(gen);

      if (((x - x1) * (x - x1) + (y - y1) * (y - y1) <= r1 * r1) &&
          ((x - x2) * (x - x2) + (y - y2) * (y - y2) <= r2 * r2) &&
          ((x - x3) * (x - x3) + (y - y3) * (y - y3) <= r3 * r3)) {
        ++ml;
      }
    }

    for (int i = 0; i < n; ++i) {
      double x = xs(gen);
      double y = ys(gen);

      if (((x - x1) * (x - x1) + (y - y1) * (y - y1) <= r1 * r1) &&
          ((x - x2) * (x - x2) + (y - y2) * (y - y2) <= r2 * r2) &&
          ((x - x3) * (x - x3) + (y - y3) * (y - y3) <= r3 * r3)) {
        ++ms;
      }
    }

    double area_large = (static_cast<double>(ml) / n) * sl;
    double area_small = (static_cast<double>(ms) / n) * ss;

    double error_large = std::abs(area_large - area) / area;
    double error_small = std::abs(area_small - area) / area;

    out << n << "," << area_small << "," << area_large << ","
        << error_small << "," << error_large << "\n";
  }
  out.close();
}