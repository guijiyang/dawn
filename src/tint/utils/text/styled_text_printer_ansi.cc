// Copyright 2024 The Dawn & Tint Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <array>
#include <cstring>

#include "src/tint/utils/containers/hashmap.h"
#include "src/tint/utils/text/styled_text.h"
#include "src/tint/utils/text/styled_text_printer.h"
#include "src/tint/utils/text/styled_text_theme.h"
#include "src/tint/utils/text/text_style.h"

namespace tint {
namespace {

template <typename T>
bool Equal(const std::optional<T>& lhs, const std::optional<T>& rhs) {
    if (lhs.has_value() != rhs.has_value()) {
        return false;
    }
    if (!lhs.has_value()) {
        return true;
    }
    return lhs.value() == rhs.value();
}

#define ESCAPE "\u001b"

class Printer24Bit : public StyledTextPrinter {
  public:
    Printer24Bit(FILE* f, const StyledTextTheme& t) : file_(f), theme_(t) {}

    void Print(const StyledText& style_text) override {
        StyledTextTheme::Attributes current;

        style_text.Walk([&](std::string_view text, TextStyle text_style) {
            auto style = theme_.Get(text_style);
            if (!Equal(current.foreground, style.foreground)) {
                current.foreground = style.foreground;
                if (current.foreground.has_value()) {
                    fprintf(file_, ESCAPE "[38;2;%d;%d;%dm",  //
                            static_cast<int>(style.foreground->r),
                            static_cast<int>(style.foreground->g),
                            static_cast<int>(style.foreground->b));
                } else {
                    fprintf(file_, ESCAPE "[39m");
                }
            }
            if (!Equal(current.background, style.background)) {
                current.background = style.background;
                if (current.background.has_value()) {
                    fprintf(file_, ESCAPE "[48;2;%d;%d;%dm",  //
                            static_cast<int>(style.background->r),
                            static_cast<int>(style.background->g),
                            static_cast<int>(style.background->b));
                } else {
                    fprintf(file_, ESCAPE "[49m");
                }
            }
            if (!Equal(current.underlined, style.underlined)) {
                current.underlined = style.underlined;
                if (current.underlined == true) {
                    fprintf(file_, ESCAPE "[4m");
                } else {
                    fprintf(file_, ESCAPE "[24m");
                }
            }
            if (!Equal(current.bold, style.bold)) {
                current.bold = style.bold;
                if (current.bold == true) {
                    fprintf(file_, ESCAPE "[1m");
                } else {
                    fprintf(file_, ESCAPE "[22m");
                }
            }
            fwrite(text.data(), 1, text.size(), file_);
        });
        fprintf(file_, ESCAPE "[m");
        fflush(file_);
    }

  private:
    FILE* const file_;
    const StyledTextTheme& theme_;
};

class Printer8Bit : public StyledTextPrinter {
  public:
    static constexpr std::array<uint32_t, 256> kPalette = {
        0x000000, 0x800000, 0x008000, 0x808000, 0x000080, 0x800080, 0x008080, 0xc0c0c0, 0x808080,
        0xff0000, 0x00ff00, 0xffff00, 0x0000ff, 0xff00ff, 0x00ffff, 0xffffff, 0x000000, 0x00005f,
        0x000087, 0x0000af, 0x0000d7, 0x0000ff, 0x005f00, 0x005f5f, 0x005f87, 0x005faf, 0x005fd7,
        0x005fff, 0x008700, 0x00875f, 0x008787, 0x0087af, 0x0087d7, 0x0087ff, 0x00af00, 0x00af5f,
        0x00af87, 0x00afaf, 0x00afd7, 0x00afff, 0x00d700, 0x00d75f, 0x00d787, 0x00d7af, 0x00d7d7,
        0x00d7ff, 0x00ff00, 0x00ff5f, 0x00ff87, 0x00ffaf, 0x00ffd7, 0x00ffff, 0x5f0000, 0x5f005f,
        0x5f0087, 0x5f00af, 0x5f00d7, 0x5f00ff, 0x5f5f00, 0x5f5f5f, 0x5f5f87, 0x5f5faf, 0x5f5fd7,
        0x5f5fff, 0x5f8700, 0x5f875f, 0x5f8787, 0x5f87af, 0x5f87d7, 0x5f87ff, 0x5faf00, 0x5faf5f,
        0x5faf87, 0x5fafaf, 0x5fafd7, 0x5fafff, 0x5fd700, 0x5fd75f, 0x5fd787, 0x5fd7af, 0x5fd7d7,
        0x5fd7ff, 0x5fff00, 0x5fff5f, 0x5fff87, 0x5fffaf, 0x5fffd7, 0x5fffff, 0x870000, 0x87005f,
        0x870087, 0x8700af, 0x8700d7, 0x8700ff, 0x875f00, 0x875f5f, 0x875f87, 0x875faf, 0x875fd7,
        0x875fff, 0x878700, 0x87875f, 0x878787, 0x8787af, 0x8787d7, 0x8787ff, 0x87af00, 0x87af5f,
        0x87af87, 0x87afaf, 0x87afd7, 0x87afff, 0x87d700, 0x87d75f, 0x87d787, 0x87d7af, 0x87d7d7,
        0x87d7ff, 0x87ff00, 0x87ff5f, 0x87ff87, 0x87ffaf, 0x87ffd7, 0x87ffff, 0xaf0000, 0xaf005f,
        0xaf0087, 0xaf00af, 0xaf00d7, 0xaf00ff, 0xaf5f00, 0xaf5f5f, 0xaf5f87, 0xaf5faf, 0xaf5fd7,
        0xaf5fff, 0xaf8700, 0xaf875f, 0xaf8787, 0xaf87af, 0xaf87d7, 0xaf87ff, 0xafaf00, 0xafaf5f,
        0xafaf87, 0xafafaf, 0xafafd7, 0xafafff, 0xafd700, 0xafd75f, 0xafd787, 0xafd7af, 0xafd7d7,
        0xafd7ff, 0xafff00, 0xafff5f, 0xafff87, 0xafffaf, 0xafffd7, 0xafffff, 0xd70000, 0xd7005f,
        0xd70087, 0xd700af, 0xd700d7, 0xd700ff, 0xd75f00, 0xd75f5f, 0xd75f87, 0xd75faf, 0xd75fd7,
        0xd75fff, 0xd78700, 0xd7875f, 0xd78787, 0xd787af, 0xd787d7, 0xd787ff, 0xd7af00, 0xd7af5f,
        0xd7af87, 0xd7afaf, 0xd7afd7, 0xd7afff, 0xd7d700, 0xd7d75f, 0xd7d787, 0xd7d7af, 0xd7d7d7,
        0xd7d7ff, 0xd7ff00, 0xd7ff5f, 0xd7ff87, 0xd7ffaf, 0xd7ffd7, 0xd7ffff, 0xff0000, 0xff005f,
        0xff0087, 0xff00af, 0xff00d7, 0xff00ff, 0xff5f00, 0xff5f5f, 0xff5f87, 0xff5faf, 0xff5fd7,
        0xff5fff, 0xff8700, 0xff875f, 0xff8787, 0xff87af, 0xff87d7, 0xff87ff, 0xffaf00, 0xffaf5f,
        0xffaf87, 0xffafaf, 0xffafd7, 0xffafff, 0xffd700, 0xffd75f, 0xffd787, 0xffd7af, 0xffd7d7,
        0xffd7ff, 0xffff00, 0xffff5f, 0xffff87, 0xffffaf, 0xffffd7, 0xffffff, 0x080808, 0x121212,
        0x1c1c1c, 0x262626, 0x303030, 0x3a3a3a, 0x444444, 0x4e4e4e, 0x585858, 0x606060, 0x666666,
        0x767676, 0x808080, 0x8a8a8a, 0x949494, 0x9e9e9e, 0xa8a8a8, 0xb2b2b2, 0xbcbcbc, 0xc6c6c6,
        0xd0d0d0, 0xdadada, 0xe4e4e4, 0xeeeeee,
    };

    Printer8Bit(FILE* f, const StyledTextTheme& t) : file_(f), theme_(t) {}

    void Print(const StyledText& style_text) override {
        StyledTextTheme::Attributes current;

        style_text.Walk([&](std::string_view text, TextStyle text_style) {
            auto style = theme_.Get(text_style);
            if (!Equal(current.foreground, style.foreground)) {
                current.foreground = style.foreground;
                if (current.foreground.has_value()) {
                    uint8_t color = Quantize(*style.foreground);
                    fprintf(file_, ESCAPE "[38;5;%dm", static_cast<int>(color));
                } else {
                    fprintf(file_, ESCAPE "[39m");
                }
            }
            if (!Equal(current.background, style.background)) {
                current.background = style.background;
                if (current.background.has_value()) {
                    uint8_t color = Quantize(*style.background);
                    fprintf(file_, ESCAPE "[48;5;%dm", static_cast<int>(color));
                } else {
                    fprintf(file_, ESCAPE "[49m");
                }
            }
            if (!Equal(current.underlined, style.underlined)) {
                current.underlined = style.underlined;
                if (current.underlined == true) {
                    fprintf(file_, ESCAPE "[4m");
                } else {
                    fprintf(file_, ESCAPE "[24m");
                }
            }
            if (!Equal(current.bold, style.bold)) {
                current.bold = style.bold;
                if (current.bold == true) {
                    fprintf(file_, ESCAPE "[1m");
                } else {
                    fprintf(file_, ESCAPE "[22m");
                }
            }
            fwrite(text.data(), 1, text.size(), file_);
        });
        fprintf(file_, ESCAPE "[m");
        fflush(file_);
    }

  private:
    uint8_t Quantize(const StyledTextTheme::Color& color) {
        return colors_.GetOrAdd(color, [&] {
            uint8_t best_color = 0;
            int best_score = std::numeric_limits<int>::max();
            for (size_t i = 0; i < 256; i++) {
                int r = static_cast<int>((kPalette[i] >> 16) & 0xff);
                int g = static_cast<int>((kPalette[i] >> 8) & 0xff);
                int b = static_cast<int>((kPalette[i] >> 0) & 0xff);
                int diff = std::abs(r - static_cast<int>(color.r)) +
                           std::abs(g - static_cast<int>(color.g)) +
                           std::abs(b - static_cast<int>(color.b));
                if (diff == 0) {
                    return static_cast<uint8_t>(i);
                }
                if (diff < best_score) {
                    best_score = diff;
                    best_color = static_cast<uint8_t>(i);
                }
            }
            return best_color;
        });
    }

    FILE* const file_;
    const StyledTextTheme& theme_;
    Hashmap<StyledTextTheme::Color, uint8_t, 16> colors_;
};
}  // namespace

std::unique_ptr<StyledTextPrinter> StyledTextPrinter::CreateANSI(FILE* out,
                                                                 const StyledTextTheme& theme,
                                                                 ANSIColors colors) {
    switch (colors) {
        case ANSIColors::k24Bit:
            return std::make_unique<Printer24Bit>(out, theme);
        case ANSIColors::k8Bit:
            return std::make_unique<Printer8Bit>(out, theme);
    }

    // Should be unreachable.
    return std::make_unique<Printer8Bit>(out, theme);
}

}  // namespace tint
