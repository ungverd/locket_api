#ifndef LOCKET_API_COLOR_H
#define LOCKET_API_COLOR_H

// Simple implementation of RGB color.
// See end of file for constants for standard colors.

#include <cstdint>
#include <algorithm>

struct Color {
public:
    uint8_t r = 0, g = 0, b = 0;
    Color() {}
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b== other.b;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    };


    void Adjust(const Color &color) {
        AdjustComponent(r, color.r);
        AdjustComponent(g, color.g);
        AdjustComponent(b, color.b);
    }

    void AdjustBy(const Color &color, uint32_t step) {
        AdjustComponentBy(r, color.r, step);
        AdjustComponentBy(g, color.g, step);
        AdjustComponentBy(b, color.b, step);
    }

    uint32_t DelayToNextAdjustment(const Color &another, uint32_t smooth_value) {
        return std::max({
            ComponentDelayToNextAdjustment(r, another.r, smooth_value),
            ComponentDelayToNextAdjustment(g, another.g, smooth_value),
            ComponentDelayToNextAdjustment(b, another.b, smooth_value),
        });
    }

private:
    static uint32_t ComponentDelayToNextAdjustment(uint8_t component, uint8_t adjust_toward, uint32_t smooth_value) {
        if (component == adjust_toward) return 0;
        // This is magic. Why adjust_toward doesn't matter here?
        return ((smooth_value / (component + 4)) + 1);
    }

    static void AdjustComponent(uint8_t& component, uint8_t adjust_towards) {
        if (component < adjust_towards) ++component;
        if (component > adjust_towards) --component;
    }

    static void AdjustComponentBy(uint8_t& component, uint8_t adjust_towards, uint32_t step) {
        uint32_t threshold = 255 - step;
        if(component < adjust_towards) {
            if(component <= threshold) component += step;
            else component = 255;
        }
        else if(component > adjust_towards) {
            if(component >= step) component -= step;
            else component = 0;
        }
    }
} __attribute__((packed));

const Color kBlack = {0, 0, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kRed = {255, 0, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kLightRed = {127, 0, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kExtraLightRed{1, 0, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kGreen = {0, 255, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kLightGreen = {0, 127, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kExtraLightGreen{0, 1, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kBlue = {0, 0, 255}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kLightBlue = {0, 0, 255}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kExtraLightBlue{0, 0, 1}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kYellow = {255, 255, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kLightYellow = {127, 127, 0}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kMagenta = {255, 0, 255}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kLightMagenta = {100, 0, 100}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kExtraLightMagenta{1, 0, 1}; // NOLINT(bugprone-dynamic-static-initializers)
const Color kWhite = {255, 255, 255}; // NOLINT(bugprone-dynamic-static-initializers)


#endif //LOCKET_API_COLOR_H
