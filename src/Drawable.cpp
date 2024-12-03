#include "Drawable.hpp"

#include <array>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using color_rgba = std::array<int, 4>;

std::string int_to_hex(int c) {
  std::ostringstream oss;
  oss << std::hex << std::setw(2) << std::setfill('0') << c;
  return oss.str();
}

inline bool is_shortable_hex(int v) {
  return (v % 17 == 0);
}

bool is_shortable_rgba(const color_rgba& rgba) {
  for (auto v : rgba) {
    if (!is_shortable_hex(v)) {
      return false;
    }
  }
  return true;
}

color_rgba nullify_rgba(const color_rgba& rgba) {
  if (rgba.size() > 3 && rgba[3] <= 0)
    return {0, 0, 0, 0};
  return rgba;
}

std::string rgba_to_hex(int r, int g, int b, int a) {
  color_rgba rgba = {std::min(r, 255), std::min(g, 255), std::min(b, 255),
                     std::min(a, 255)};
  if (a == -1)
    rgba[3] = 255;

  std::string hex = "";
  if (is_shortable_rgba(rgba)) {
    for (auto v : rgba) {
      hex += int_to_hex(v).substr(0, 1);
    }
  } else {
    for (auto v : rgba) {
      hex += int_to_hex(v);
    }
  }
  return hex;
}

color_rgba pre_shorten_color(const color_rgba& c) {
  return {(int)std::round(c[0] / 17.0) * 17, (int)std::round(c[1] / 17.0) * 17,
          (int)std::round(c[2] / 17.0) * 17, (int)std::round(c[3] / 17.0) * 17};
}

std::map<std::string, int> count_palettes(
    const std::vector<std::vector<color_rgba>>& image) {
  std::map<std::string, int> palettes;
  // Iterate through image pixels, assuming image is a 2D vector of RGBA colors
  for (const auto& row : image) {
    for (const auto& color : row) {
      std::string hex = rgba_to_hex(color[0], color[1], color[2], color[3]);
      palettes[hex] = palettes[hex] + 1;
    }
  }
  return palettes;
}

std::map<std::string, std::string> generate_palette_substitutes(
    const std::map<std::string, int>& palettes,
    int green_index,
    bool replace_transparents) {
  std::map<std::string, std::string> replaces;
  color_rgba index = {0, 0, 0, 15};
  int green_index_min = (int)std::ceil(green_index / 17.0) + 1;
  bool stop = false;
  if (green_index < 119) {
    index[1] = green_index_min;
  }

  auto increment_index = [&]() {
    index[0] += 1;
    if (index[0] > 15) {
      index[0] = 0;
      index[1] += 1;
    }
    if (index[1] > 15) {
      index[1] = green_index_min;
      index[2] += 1;
    }
    if (index[2] > 15) {
      index[2] = 0;
      index[3] -= 1;
    }
    if (index[3] < 0) {
      stop = true;
    }
  };

  for (const auto& palette : palettes) {
    if ((palette.second > 3) ||
        (replace_transparents && palette.first.length() > 4)) {
      replaces[palette.first] = rgba_to_hex(index[0] * 17, index[1] * 17,
                                            index[2] * 17, index[3] * 17);
      increment_index();
      if (stop) {
        break;
      }

      while (palettes.count(rgba_to_hex(index[0] * 17, index[1] * 17,
                                        index[2] * 17, index[3] * 17))) {
        increment_index();
        if (stop) {
          break;
        }
      }
    }
  }
  return replaces;
}

std::string substitutes_to_directives(
    const std::map<std::string, std::string>& substitutes) {
  std::string directives = "?replace";
  for (const auto& substitute : substitutes) {
    directives += ";" + substitute.second + "=" + substitute.first;
  }
  return directives == "?replace" ? "" : directives;
}

// The `types` map can be used similarly to JS objects as function containers.

std::string generate_fade(const std::vector<std::vector<color_rgba>>& image,
                          const std::map<std::string, int>& options) {
  int width = options.count("width") ? options.at("width") : image[0].size();
  int height = options.count("height") ? options.at("height") : image.size();
  int offset_x = 0;
  int offset_y = 0;
  std::string src_image_directory = "/assetmissing.png";

  int green_index = 0;

  std::string base_directives =
      "?crop;0;0;1;1?setcolor=fff?replace;fff0=fff?border=1;fff;000?scale=1.15;"
      "1.12?crop;1;1;3;3";
  base_directives += "?replace;fbfbfb=" + rgba_to_hex(0, green_index, 0, 0) +
                     ";eaeaea=" + rgba_to_hex(width, green_index, 0, 0) +
                     ";e4e4e4=" + rgba_to_hex(0, green_index, height, 0) +
                     ";6a6a6a=" + rgba_to_hex(width, green_index, height, 0);

  base_directives += "?scale=" + std::to_string(width - 0.5);
  if (width != height) {
    base_directives += ";" + std::to_string(height - 0.5);
  }

  std::vector<int> crop = {0, 0, width, height};
  if (width > 255) {
    crop[0] += 1;
    crop[2] += 1;
  }
  if (height > 255) {
    crop[1] += 1;
    crop[3] += 1;
  }
  base_directives += "?crop=" + std::to_string(crop[0]) + ";" +
                     std::to_string(crop[1]) + ";" + std::to_string(crop[2]) +
                     ";" + std::to_string(crop[3]);

  std::map<std::string, std::string> to_replace;
  if (!options.count("compressColor") &&
      !options.count("disablePalettesSubstitutes")) {
    to_replace = generate_palette_substitutes(
        count_palettes(image), green_index, options.count("retainColorValue"));
  }
  std::string to_original = substitutes_to_directives(to_replace);

  std::string pixel_data = "?replace";
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      color_rgba color = image[height - y - 1][x];
      if (!options.count("retainColorValue")) {
        color = nullify_rgba(color);
      }

      if (color[3] > 0 || options.count("retainColorValue")) {
        if (options.count("compressColor")) {
          color = pre_shorten_color(color);
        }

        std::string color_hex =
            rgba_to_hex(color[0], color[1], color[2], color[3]);
        pixel_data +=
            ";" + rgba_to_hex(x, green_index, y, 0) + "=" +
            (to_replace.count(color_hex) ? to_replace[color_hex] : color_hex);
      }
    }
  }

  if (options.count("pixelMapOnly")) {
    src_image_directory = "";
    base_directives = "";
  }
  if (pixel_data == "?replace") {
    return src_image_directory;
  }
  return src_image_directory + base_directives + pixel_data + to_original;
}

// Sample function registration
// types["fade"] = [](const std::vector<std::vector<int>>& image,
//                    const std::map<std::string, int>& options) {
//   std::string result =
//       "?crop;0;0;1;1?setcolor=fff?replace;fff0=fff?border=1;fff;000";
//   // Add more processing based on the input image and options
//   return result;
// };

// int main() {
//   // Usage example
//   std::vector<std::vector<int>> image = {
//       {{255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}},
//       {{255, 255, 0, 255}, {0, 255, 255, 255}, {255, 0, 255, 255}}};

//   std::map<std::string, int> options = {{"width", 100}, {"height", 100}};

//   // Call fade or other types
//   std::string result = types["fade"](image, options);
//   std::cout << result << std::endl;

//   return 0;
// }