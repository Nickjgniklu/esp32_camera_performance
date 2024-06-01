# ESP32 Camera performance
While testing tensorflow models on the esp32 I found that getting the image to be ready to be passed to the model can take a significant amount of time. Here are some metrics that I find useful when estimating possible fps for vision based ML projects on the esp32.

These result where recorded on the seeed xiao esp32s3

| Resolution | Jpeg Capture Time | Jpeg to RGB Time | RGB to JPEG Time |
|------------|--------------|-------------------|--------------------|
| 160x120    | 40ms         | 13ms              | 18ms               |
| 176x144    | 40ms         | 18ms              | 22ms               |
| 240x176    | 40ms         | 29ms              | 37ms               |
| 240x240    | 40ms         | 39ms              | 49ms               |
| 320x240    | 40ms         | 53ms              | 66ms               |
| 400x296    | 40ms         | 80ms              | 104ms              |
| 480x320    | 80ms         | 102ms             | 133ms              |
| 640x480    | 80ms         | 202ms             | 265ms              |
| 800x600    | 80ms         | 315ms             | 422ms              |
| 1024x768   | 160ms        | 503ms             | 686ms              |
| 1280x720   | 160ms        | 588ms             | 801ms              |
| 1280x1024  | 160ms        | 835ms             | 1140ms             |
| 1600x1200  | 160ms        | 1229ms            | 1699ms             |
## Recording method
See [main.cpp](src/main.cpp). Each resolution's results were recorded one at a time.