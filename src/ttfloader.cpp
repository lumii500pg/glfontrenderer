#include "ttfloader.h"
#include "logger.h"

#include "fileutils.h"
#include "timeutils.h"

#include "bitmap.h"
#include "font.h"
#include <tuple>

namespace glfr {
    const uint32_t PADDING_X = 2;
    const uint32_t PADDING_Y = 2;

    font* ttfloader::load_font(const char *name, size_t font_height) {
        std::vector<int8_t> readBytes = file_utils::read_bytes(std::string("fonts/") + name);
        return load_font(readBytes.data(), readBytes.size(), font_height);
    }

    void handleFTError(const FT_Error error) {
        logger::error("an error occurred loading TTF: {}", error);
    }

    void calcGlyphSize(FT_Face face, const FT_ULong &character, uint32_t preferred_width,
                       uint32_t &current_atlas_width, uint32_t &max_atlas_width,
                       uint32_t &max_glyph_height, uint32_t &current_y_offset, uint32_t &current_y_height,
                       std::vector<std::pair<uint32_t, uint32_t>> &y_data) {
        FT_Error error = FT_Load_Char(face, character, FT_LOAD_DEFAULT);
        if (error) {
            logger::warn("skipped char '{}' because of an error: {}", character, error);
            return;
        }

        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if (error) {
            logger::warn("couldn't render glyph: {}", error);
            return;
        }

        current_atlas_width += face->glyph->bitmap.width + PADDING_X;
        uint32_t current_height = face->glyph->bitmap.rows;

        if (current_height > current_y_height) {
            current_y_height = current_height;
        }

        if (current_atlas_width > preferred_width) {
            if (current_atlas_width > max_atlas_width) {
                max_atlas_width = current_atlas_width;
            }
            current_atlas_width = 0;
            current_y_height += PADDING_Y;

            y_data.emplace_back(current_y_height, current_y_offset);
            current_y_offset += current_y_height;
            current_y_height = 0;
        }

        if (current_height > max_glyph_height) {
            max_glyph_height = current_height;
        }
    }

    font* ttfloader::load_font(const void *ptr, size_t num_bytes, size_t font_height) {
        uint64_t millis = current_time<std::chrono::milliseconds>();
        uint64_t nanos = current_time<std::chrono::nanoseconds>();
        uint32_t millisAll = current_time<std::chrono::milliseconds>();
        uint32_t nanosAll = current_time<std::chrono::nanoseconds>();
        FT_Face face;
        FT_Error error;

        static FT_Library library = nullptr;
        if (!library) {
            error = FT_Init_FreeType(&library);
            if (error) {
                logger::error("couldn't initialize FreeType: {}", error);
                return nullptr;
            }
        }

        logger::info("initialized FreeType in {}ms or {}ns", current_time<std::chrono::milliseconds>() - millis,
                     (current_time<std::chrono::nanoseconds>() - nanos) * 1000000.);
        millis = current_time<std::chrono::milliseconds>();
        nanos = current_time<std::chrono::nanoseconds>();

        error = FT_New_Memory_Face(library, (const FT_Byte *) ptr,
                                   (FT_Long) num_bytes, 0, &face);
        if (error) {
            handleFTError(error);
            return nullptr;
        }
        logger::info("created new memory face in {}ms or {}ns", current_time<std::chrono::milliseconds>() - millis,
                     (current_time<std::chrono::nanoseconds>() - nanos) * 1000000.);
        millis = current_time<std::chrono::milliseconds>();
        nanos = current_time<std::chrono::nanoseconds>();

        error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
        if (error) {
            handleFTError(error);
            return nullptr;
        }
        logger::info("selected char map in {}ms or {}ns", current_time<std::chrono::milliseconds>() - millis,
                     (current_time<std::chrono::nanoseconds>() - nanos) * 1000000.);
        millis = current_time<std::chrono::milliseconds>();
        nanos = current_time<std::chrono::nanoseconds>();

        error = FT_Set_Pixel_Sizes(face, 0, font_height);
        if (error) {
            handleFTError(error);
            return nullptr;
        }
        logger::info("set pixel sizes in {}ms or {}ns", current_time<std::chrono::milliseconds>() - millis,
                     (current_time<std::chrono::nanoseconds>() - nanos) * 1000000.);
        millis = current_time<std::chrono::milliseconds>();
        nanos = current_time<std::chrono::nanoseconds>();

        const size_t preferred_width = font_height * 32;
        uint32_t current_atlas_width = 0;
        uint32_t max_atlas_width = 0;
        uint32_t temp_y_offset = 0;
        uint32_t temp_y_height = 0;
        uint32_t glyph_height = 0;
        std::vector<FT_ULong> char_codes;
        std::vector<std::pair<uint32_t, uint32_t>> y_data;

        FT_UInt glyph_index;
        FT_ULong char_code = FT_Get_First_Char(face, &glyph_index);
        if (glyph_index == 0) {
            logger::error("an error occurred loading TTF: first glyph is invalid");
            return nullptr;
        }
        logger::info("got first char in {}ms or {}ns", current_time<std::chrono::milliseconds>() - millis,
                     (current_time<std::chrono::nanoseconds>() - nanos) * 1000000.);
        millis = current_time<std::chrono::milliseconds>();
        nanos = current_time<std::chrono::nanoseconds>();

        calcGlyphSize(face, char_code, preferred_width, current_atlas_width, max_atlas_width, glyph_height,
                      temp_y_offset, temp_y_height, y_data);
        char_codes.push_back(char_code);
        logger::info("calculated first char size in {}ms or {}ns", current_time<std::chrono::milliseconds>() - millis,
                     (current_time<std::chrono::nanoseconds>() - nanos) * 1000000.);
        millis = current_time<std::chrono::milliseconds>();
        nanos = current_time<std::chrono::nanoseconds>();

        while (glyph_index != 0) {
            char_code = FT_Get_Next_Char(face, char_code, &glyph_index);
            calcGlyphSize(face, char_code, preferred_width, current_atlas_width, max_atlas_width, glyph_height,
                          temp_y_offset, temp_y_height, y_data);
            char_codes.push_back(char_code);
        }
        y_data.emplace_back(temp_y_height, temp_y_offset);

        logger::info("got and calculated other chars in {}ms or {}ns", current_time<std::chrono::milliseconds>()
                                                                       - millis,
                     (current_time<std::chrono::nanoseconds>() - nanos) * 1000000.);
        millis = current_time<std::chrono::milliseconds>();
        nanos = current_time<std::chrono::nanoseconds>();

        const auto height = std::get<0>(y_data.back()) + std::get<1>(y_data.back());
        bitmap my_bitmap(max_atlas_width, height);

        uint32_t offset_x = 0, y_index = 0;

        std::unordered_map<uint64_t, glyph_info> glyph_infos;

        for (unsigned long current_code: char_codes) {
            error = FT_Load_Char(face, current_code, FT_LOAD_DEFAULT);
            if (error) {
                logger::warn("couldn't load char '{}': {}", current_code, error);
                continue;
            }

            error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            if (error) {
                logger::warn("couldn't render char '{}': {}", current_code, error);
                continue;
            }

            const auto glyph_width = face->glyph->bitmap.width;
            const auto glyph_height = face->glyph->bitmap.rows;

            const auto offset_y = std::get<1>(y_data[y_index]);

            for (auto y = 0; y < glyph_height; y++) {
                for (auto x = 0; x < glyph_width; x++) {
                    const auto value = face->glyph->bitmap.buffer[y * glyph_width + x];
                    my_bitmap.get(offset_x + x, offset_y + y) = (value << 24) | (value << 16) | (value << 8) | value;
                }
            }

            glyph_infos.emplace(std::piecewise_construct, std::make_tuple(current_code), std::make_tuple(
                    current_code, static_cast<float>(face->glyph->advance.x >> 6),
                    static_cast<float>(face->glyph->advance.y >> 6),
                    static_cast<float>(glyph_width), static_cast<float>(glyph_height),
                    static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top), offset_x,
                    offset_y));

            offset_x += glyph_width + PADDING_X;
            if (offset_x > preferred_width) {
                offset_x = 0;
                ++y_index;
            }
        }
        logger::info("filled bitmap in {}ms or {}ns", current_time<std::chrono::milliseconds>() - millis,
                     (current_time<std::chrono::nanoseconds>() - nanos) * 1000000.);
        logger::info("Total time: {}ms or {}ns", current_time<std::chrono::milliseconds>() - millisAll,
                     (current_time<std::chrono::nanoseconds>() - nanosAll) * 1000000.);

        auto *my_font = new font(my_bitmap, glyph_infos, font_height);
        return my_font;
    }
}
