#include <Arduino.h>
#include <WiFiClient.h>
#include "camera_config.h"
#include "img_converters.h"
struct FrameResolution
{
  int width;
  int height;
};
OV2640 camera;
camera_config_t esp32cam_ESPCam_config{

    .pin_pwdn = CAMERA_PIN_PWDN,
    .pin_reset = CAMERA_PIN_RESET,
    .pin_xclk = CAMERA_PIN_XCLK,
    .pin_sscb_sda = CAMERA_PIN_SIOD,
    .pin_sscb_scl = CAMERA_PIN_SIOC,
    .pin_d7 = CAMERA_PIN_D7,
    .pin_d6 = CAMERA_PIN_D6,
    .pin_d5 = CAMERA_PIN_D5,
    .pin_d4 = CAMERA_PIN_D4,
    .pin_d3 = CAMERA_PIN_D3,
    .pin_d2 = CAMERA_PIN_D2,
    .pin_d1 = CAMERA_PIN_D1,
    .pin_d0 = CAMERA_PIN_D0,
    .pin_vsync = CAMERA_PIN_VSYNC,
    .pin_href = CAMERA_PIN_HREF,
    .pin_pclk = CAMERA_PIN_PCLK,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,

    .frame_size = FRAMESIZE_QQVGA, // 160x120
#define WIDTH 160
#define HEIGHT 120
    // .frame_size = FRAMESIZE_QCIF,     // 176x144
    // #define WIDTH 176
    // #define HEIGHT 144
    // .frame_size = FRAMESIZE_HQVGA,    // 240x176
    // #define WIDTH 240
    // #define HEIGHT 176
    // .frame_size = FRAMESIZE_240X240,  // 240x240
    // #define WIDTH 240
    // #define HEIGHT 240
    // .frame_size = FRAMESIZE_QVGA,     // 320x240
    // #define WIDTH 320
    // #define HEIGHT 240
    // .frame_size = FRAMESIZE_CIF,      // 400x296
    // #define WIDTH 400
    // #define HEIGHT 296
    // .frame_size = FRAMESIZE_HVGA,     // 480x320
    // #define WIDTH 480
    // #define HEIGHT 320
    // .frame_size = FRAMESIZE_VGA,      // 640x480
    // #define WIDTH 640
    // #define HEIGHT 480
    // .frame_size = FRAMESIZE_SVGA,     // 800x600
    // #define WIDTH 800
    // #define HEIGHT 600
    // .frame_size = FRAMESIZE_XGA,      // 1024x768
    // #define WIDTH 1024
    // #define HEIGHT 768
    // .frame_size = FRAMESIZE_HD,       // 1280x720
    // #define WIDTH 1280
    // #define HEIGHT 720
    // .frame_size = FRAMESIZE_SXGA,     // 1280x1024
    // #define WIDTH 1280
    // #define HEIGHT 1024
    //.frame_size = FRAMESIZE_UXGA,     // 1600x1200
    // #define WIDTH 1600
    // #define HEIGHT 1200
    .jpeg_quality = 25,
    .fb_count = 1};
int width = WIDTH;
int height = HEIGHT;
//
// std::vector<FrameResolution> frameResolutions = {
//     {160, 120},
//     {176, 144},
//     {240, 176},
//     {240, 240},
//     {320, 240},
//     {400, 296},
//     {480, 320},
//     {640, 480},
//     {800, 600},
//     {1024, 768},
//     {1280, 720},
//     {1280, 1024},
//     {1600, 1200}};
/// @brief Writes the jpeg bytes to the serial port as binary
/// use JpegFilter to extract the jpeg bytes and save them to a file
/// @param jpegBuffer // the jpeg bytes to write
/// @param jpegSize // the length of the jpeg bytes
void serialWriteJpeg(uint8_t *jpegBuffer, size_t jpegSize)
{
  Serial.print("StartJPEG123456");
  Serial.write(jpegBuffer, jpegSize);
  Serial.print("EndJPEG123456");
}
void setup()
{
  Serial.begin(115200);
  Serial.println();
  delay(5000);
  camera.init(esp32cam_ESPCam_config);
}

void loop()
{
  camera.run();

  auto captureStart = millis();
  camera.run();
  auto captureEnd = millis();

  uint8_t *rgbBuffer = (uint8_t *)ps_malloc(width * height * 3);
  camera_fb_t *fb = new camera_fb_t();

  fb->buf = rgbBuffer;
  fb->format = PIXFORMAT_RGB888;
  fb->len = width * height * 3;
  fb->timestamp = {0, 0};
  fb->height = height;
  fb->width = width;
  uint8_t *jpegBuffer;
  size_t jpegSize;
  serialWriteJpeg(camera.getfb(), camera.getSize());
  auto frameToRgbStart = millis();
  fmt2rgb888(camera.getfb(), camera.getSize(), PIXFORMAT_JPEG, rgbBuffer);
  auto frameToRgbEnd = millis();
  auto frameToJpegStart = millis();
  frame2jpg(fb, 63, &jpegBuffer, &jpegSize);
  auto frameToJpegEnd = millis();

  free(rgbBuffer);
  free(jpegBuffer);
  Serial.printf("res: %ldx%ld, capture: %ldms, frameToRgb: %ldms, frameToJpeg: %ldms\n",
                width, height, captureEnd - captureStart, frameToRgbEnd - frameToRgbStart,
                frameToJpegEnd - frameToJpegStart);
  delay(10000);
}