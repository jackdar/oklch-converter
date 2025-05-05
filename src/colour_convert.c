#include "colour_convert.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159265358979323846

int hex_digit_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return 0;
}

RGB hex_to_rgb(const char *hex) {
    RGB rgb = {0, 0, 0};

    if (hex[0] == '#') {
        hex++;
    }

    size_t len = strlen(hex);

    if (len == 6) {
        rgb.r =
            (hex_digit_to_int(hex[0]) * 16 + hex_digit_to_int(hex[1])) / 255.0;
        rgb.g =
            (hex_digit_to_int(hex[2]) * 16 + hex_digit_to_int(hex[3])) / 255.0;
        rgb.b =
            (hex_digit_to_int(hex[4]) * 16 + hex_digit_to_int(hex[5])) / 255.0;
    } else if (len == 3) {
        rgb.r = (hex_digit_to_int(hex[0]) * 17) / 255.0;
        rgb.g = (hex_digit_to_int(hex[1]) * 17) / 255.0;
        rgb.b = (hex_digit_to_int(hex[2]) * 17) / 255.0;
    } else {
        fprintf(stderr, "Invalid hex colour format\n");
    }

    return rgb;
}

RGB rgb_to_linear_rgb(RGB rgb) {
    RGB linear_rgb;

    // Apply gamma correction
    linear_rgb.r =
        (rgb.r <= 0.04045) ? rgb.r / 12.92 : pow((rgb.r + 0.055) / 1.055, 2.4);
    linear_rgb.g =
        (rgb.g <= 0.04045) ? rgb.g / 12.92 : pow((rgb.g + 0.055) / 1.055, 2.4);
    linear_rgb.b =
        (rgb.b <= 0.04045) ? rgb.b / 12.92 : pow((rgb.b + 0.055) / 1.055, 2.4);

    return linear_rgb;
}

XYZ linear_rgb_to_xyz(RGB linear_rgb) {
    XYZ xyz;

    // sRGB to XYZ matrix multiplication
    xyz.x =
        0.4124 * linear_rgb.r + 0.3576 * linear_rgb.g + 0.1805 * linear_rgb.b;
    xyz.y =
        0.2126 * linear_rgb.r + 0.7152 * linear_rgb.g + 0.0722 * linear_rgb.b;
    xyz.z =
        0.0193 * linear_rgb.r + 0.1192 * linear_rgb.g + 0.9505 * linear_rgb.b;

    return xyz;
}

LMS xyz_to_lms(XYZ xyz) {
    LMS lms;

    // XYZ to LMS matrix multiplication
    lms.l = 0.8189 * xyz.x + 0.3618 * xyz.y - 0.1288 * xyz.z;
    lms.m = 0.0329 * xyz.x + 0.9293 * xyz.y + 0.0361 * xyz.z;
    lms.s = 0.0486 * xyz.x + 0.2621 * xyz.y + 0.6839 * xyz.z;

    return lms;
}

Oklab lms_to_oklab(LMS lms) {
    Oklab oklab;

    // Non-linear transformation (cube root)
    double l_prime = cbrt(lms.l);
    double m_prime = cbrt(lms.m);
    double s_prime = cbrt(lms.s);

    // LMS' to Oklab matrix multiplication
    oklab.l = 0.2104 * l_prime + 0.7936 * m_prime - 0.0040 * s_prime;
    oklab.a = 1.9779 * l_prime - 2.4285 * m_prime + 0.4506 * s_prime;
    oklab.b = 0.0259 * l_prime + 0.7827 * m_prime - 0.8086 * s_prime;

    return oklab;
}

OKLCH oklab_to_oklch(Oklab oklab) {
    OKLCH oklch;

    oklch.l = oklab.l;
    oklch.c = sqrt(oklab.a * oklab.a + oklab.b * oklab.b);
    oklch.h = atan2(oklab.b, oklab.a) * 180.0 / PI;

    // Ensure hue is in the range [0, 360)
    if (oklch.h < 0) {
        oklch.h += 360.0;
    }

    return oklch;
}

OKLCH hex_to_oklch(const char *hex) {
    RGB rgb = hex_to_rgb(hex);
    RGB linear_rgb = rgb_to_linear_rgb(rgb);
    XYZ xyz = linear_rgb_to_xyz(linear_rgb);
    LMS lms = xyz_to_lms(xyz);
    Oklab oklab = lms_to_oklab(lms);
    return oklab_to_oklch(oklab);
}
