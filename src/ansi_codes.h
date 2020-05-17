#pragma once

//
// ansi_codes.h - Utility for printing ansi escape codes to the console.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-05-17
//

#include <cstdint>
#include <iostream>
#include <string>

namespace rda
{
    namespace ansi
    {
        constexpr const char *ANSI_ESCAPE = "\033[";
        constexpr const char *ANSI_ESCAPE_SUFFIX = "m";

        // ansi text codes
        enum class ansi_code : uint8_t
        {
            NORMAL = 0,
            RESET = 0,
            BOLD = 1,
            FAINT = 2,
            ITALIC = 3,
            UNDERLINE = 4,
            SLOW_BLINK = 5,
            RAPID_BLINK = 6,
            REVERSE_VIDEO = 7,
            CONCEAL = 8,
            STRIKE_OUT = 9,
            FOREGROUND = 30,
            BACKGROUND = 40,
            BRIGHT = 60

        }; // enum ansi_code

        // ansi colors
        enum class ansi_color : uint8_t
        {
            NORMAL = 0,
            RESET = 0,

            BLACK = 0,
            RED = 1,
            GREEN = 2,
            YELLOW = 3,
            BLUE = 4,
            MAGENTA = 5,
            CYAN = 6,
            WHITE = 7

        }; // enum ansi_color

        // reset ansi codes, in order to display default text style again
        struct TextNormal
        {
            friend std::ostream &operator<<(std::ostream &os, const TextNormal &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::NORMAL))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text bold (or increased intensity)
        struct TextBold
        {
            friend std::ostream &operator<<(std::ostream &os, const TextBold &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::BOLD))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text faint (decreased intensity)
        struct TextFaint
        {
            friend std::ostream &operator<<(std::ostream &os, const TextFaint &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::FAINT))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text italic
        struct TextItalic
        {
            friend std::ostream &operator<<(std::ostream &os, const TextItalic &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::ITALIC))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text underline
        struct TextUnderline
        {
            friend std::ostream &operator<<(std::ostream &os, const TextUnderline &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::UNDERLINE))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text slow blink
        struct TextSlowBlink
        {
            friend std::ostream &operator<<(std::ostream &os, const TextSlowBlink &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::SLOW_BLINK))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text rapid blink
        struct TextRapidBlink
        {
            friend std::ostream &operator<<(std::ostream &os, const TextRapidBlink &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::RAPID_BLINK))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text reverse video
        struct TextReverseVideo
        {
            friend std::ostream &operator<<(std::ostream &os, const TextReverseVideo &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::REVERSE_VIDEO))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text conceal (hidden)
        struct TextConceal
        {
            friend std::ostream &operator<<(std::ostream &os, const TextConceal &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::CONCEAL))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the text strike out
        struct TextStrikeOut
        {
            friend std::ostream &operator<<(std::ostream &os, const TextStrikeOut &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::STRIKE_OUT))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // reset ansi codes, in order to display default text style again
        struct ColorReset
        {
            friend std::ostream &operator<<(std::ostream &os, const ColorReset &)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::RESET))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the color of both foreground, background, and intensities
        struct Color
        {
            ansi_color foreground_color;
            ansi_color background_color;

            bool is_foreground_high_intensity = false;
            bool is_background_high_intensity = false;

            Color(ansi_color fg_color, ansi_color bg_color, bool fg_high_intensity = false, bool bg_high_intensity = false)
                : foreground_color(fg_color),
                  background_color(bg_color),
                  is_foreground_high_intensity(fg_high_intensity),
                  is_background_high_intensity(bg_high_intensity)
            {
            }

            friend std::ostream &operator<<(std::ostream &os, const Color &c)
            {
                uint8_t fg = static_cast<uint8_t>(c.foreground_color) + static_cast<uint8_t>(ansi_code::FOREGROUND);
                if (c.is_foreground_high_intensity)
                {
                    fg += static_cast<uint8_t>(ansi_code::BRIGHT);
                }

                uint8_t bg = static_cast<uint8_t>(c.background_color) + static_cast<uint8_t>(ansi_code::BACKGROUND);
                if (c.is_background_high_intensity)
                {
                    bg += static_cast<uint8_t>(ansi_code::BRIGHT);
                }

                os << ANSI_ESCAPE << std::to_string(fg) << ANSI_ESCAPE_SUFFIX
                   << ANSI_ESCAPE << std::to_string(bg) << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the color of foreground text output, with normal color intensity
        struct ColorFG
        {
            ansi_color color;

            ColorFG(ansi_color c)
                : color(c)
            {
            }

            friend std::ostream &operator<<(std::ostream &os, const ColorFG &c)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::FOREGROUND) + static_cast<uint8_t>(c.color))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the color of foreground text output, with bright color intensity
        struct ColorFGBright
        {
            ansi_color color;

            ColorFGBright(ansi_color c)
                : color(c)
            {
            }

            friend std::ostream &operator<<(std::ostream &os, const ColorFGBright &c)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::FOREGROUND) + static_cast<uint8_t>(ansi_code::BRIGHT) + static_cast<uint8_t>(c.color))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the color of background text output, with normal color intensity
        struct ColorBG
        {
            ansi_color color;

            ColorBG(ansi_color c)
                : color(c)
            {
            }

            friend std::ostream &operator<<(std::ostream &os, const ColorBG &c)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::BACKGROUND) + static_cast<uint8_t>(c.color))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

        // set the color of background text output, with bright color intensity
        struct ColorBGBright
        {
            ansi_color color;

            ColorBGBright(ansi_color c)
                : color(c)
            {
            }

            friend std::ostream &operator<<(std::ostream &os, const ColorBGBright &c)
            {
                os << ANSI_ESCAPE
                   << std::to_string(static_cast<uint8_t>(ansi_code::BACKGROUND) + static_cast<uint8_t>(ansi_code::BRIGHT) + static_cast<uint8_t>(c.color))
                   << ANSI_ESCAPE_SUFFIX;

                return os;
            }
        };

    } // namespace ansi

} // namespace rda
