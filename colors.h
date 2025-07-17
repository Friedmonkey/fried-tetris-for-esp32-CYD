#include <TFT_eSPI.h>

struct RGBColor {
    uint8_t r, g, b;
};

// Define base colors (same as your defines)
static constexpr RGBColor darkGrey = {26, 31, 40};
static constexpr RGBColor green = {47, 230, 23};
static constexpr RGBColor red = {232, 18, 18};
static constexpr RGBColor orange = {226, 116, 17};
static constexpr RGBColor yellow = {237, 234, 4};
static constexpr RGBColor purple = {166, 0, 247};
static constexpr RGBColor cyan = {21, 204, 209};
static constexpr RGBColor blue = {13, 64, 216};
static constexpr RGBColor white = {255, 255, 255};


// static constexpr uint16_t lightblue = 0x3AB4;
// // static constexpr uint16_t darkblue  = 0x296F;
// #define LIGHTBLUE_BGR 0xA2A7  /* BGR(59,85,162)  */
// #define DARKBLUE_BGR  0x7965  /* BGR(44,44,127)  */


#define LIGHTBLUE 0x3AB4  /* RGB(59,  85, 162) */
#define DARKBLUE  0x296F  /* RGB(44,  44, 127) */
//#define TFT_DARK_GREEN       0x04B0      /*   0, ?,   0 */


// static constexpr uint16_t lightblue = rgbTo565_const(59, 85, 162);
// static constexpr uint16_t darkblue  = rgbTo565_const(44, 44, 127);


// constexpr uint16_t rgbTo565_const(uint8_t r, uint8_t g, uint8_t b)
// {
//     return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
// }


// Darkness factor for shadow colors (approx 40% brightness)
static constexpr float shadowFactor = 0.4f;

// Convert RGB to 16-bit color565
inline uint16_t rgbTo565(const RGBColor& color, float brightness = 1.0f)
{
    uint8_t r = static_cast<uint8_t>(color.r * brightness);
    uint8_t g = static_cast<uint8_t>(color.g * brightness);
    uint8_t b = static_cast<uint8_t>(color.b * brightness);
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


// Static array with all normal and shadow colors precomputed
// static const uint16_t CellColors[] = {
//     rgbTo565(darkGrey, 1.0f), rgbTo565(green, 1.0f), rgbTo565(red, 1.0f), rgbTo565(orange, 1.0f), rgbTo565(yellow, 1.0f), rgbTo565(purple, 1.0f), rgbTo565(cyan, 1.0f), rgbTo565(blue, 1.0f), rgbTo565(white, 1.0f),
//     rgbTo565(darkGrey, shadowFactor), rgbTo565(green, shadowFactor), rgbTo565(red, shadowFactor), rgbTo565(orange, shadowFactor), rgbTo565(yellow, shadowFactor), rgbTo565(purple, shadowFactor), rgbTo565(cyan, shadowFactor), rgbTo565(blue, shadowFactor), rgbTo565(white, shadowFactor)
// };

static const uint16_t CellColors[] = {
    TFT_DARKGREY, TFT_GREEN, TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_MAGENTA, TFT_CYAN, TFT_SKYBLUE, TFT_WHITE,
    TFT_DARKGREY, TFT_DARKGREEN, TFT_MAROON, TFT_BROWN, TFT_GOLD, TFT_PURPLE, TFT_DARKCYAN, TFT_BLUE, TFT_SILVER
};