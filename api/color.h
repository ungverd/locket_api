#ifndef LOCKET_API_COLOR_H
#define LOCKET_API_COLOR_H

#include <cstdint>

struct Color {
public:
    uint8_t r = 0, g = 0, b = 0;
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

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

private:
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

#endif //LOCKET_API_COLOR_H

