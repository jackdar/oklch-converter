#ifndef COLOUR_CONVERT_H
#define COLOUR_CONVERT_H

typedef struct {
    double r;
    double g;
    double b;
} RGB;

typedef struct {
    double x;
    double y;
    double z;
} XYZ;

typedef struct {
    double l;
    double m;
    double s;
} LMS;

typedef struct {
    double l;
    double a;
    double b;
} Oklab;

typedef struct {
    double l;
    double c;
    double h;
} OKLCH;

// Convert hex to RGB
RGB hex_to_rgb(const char *hex);

// Convert RGB to linear RGB
RGB rgb_to_linear_rgb(RGB rgb);

// Convert linear RGB to XYZ
XYZ linear_rgb_to_xyz(RGB linear_rgb);

// Convert XYZ to LMS
LMS xyz_to_lms(XYZ xyz);

// Convert LMS to Oklab
Oklab lms_to_oklab(LMS lms);

// Convert Oklab to OKLCH
OKLCH oklab_to_oklch(Oklab oklab);

// Main conversion function: hex to OKLCH
OKLCH hex_to_oklch(const char *hex);

#endif
