#include "helpers.h"
#include <math.h>
#include <string.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int red = image[i][j].rgbtRed;
      int green = image[i][j].rgbtGreen;
      int blue = image[i][j].rgbtBlue;

      int average = round((red + green + blue) / 3.0);

      image[i][j].rgbtRed = average;
      image[i][j].rgbtGreen = average;
      image[i][j].rgbtBlue = average;
    }
  }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int red = image[i][j].rgbtRed;
      int green = image[i][j].rgbtGreen;
      int blue = image[i][j].rgbtBlue;

      int sepia_red = round(0.393 * red + 0.769 * green + 0.189 * blue);
      int sepia_green = round(0.349 * red + 0.686 * green + 0.168 * blue);
      int sepia_blue = round(0.272 * red + 0.534 * green + 0.131 * blue);

      image[i][j].rgbtRed = sepia_red < 255 ? sepia_red : 255;
      image[i][j].rgbtGreen = sepia_green < 255 ? sepia_green : 255;
      image[i][j].rgbtBlue = sepia_blue < 255 ? sepia_blue : 255;
    }
  }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width / 2; j++) {
      RGBTRIPLE left = image[i][j];
      RGBTRIPLE right = image[i][width - (j + 1)];

      image[i][j] = right;
      image[i][width - (j + 1)] = left;
    }
  }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {

  RGBTRIPLE temp[height][width];
  memcpy(temp, image, sizeof(RGBTRIPLE) * height * width);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int total_red = 0;
      int total_green = 0;
      int total_blue = 0;
      int counter = 0;

      for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
          if (i + r != height && i + r != -1 && j + c != width && j + c != -1) {
            total_red += temp[i + r][j + c].rgbtRed;
            total_green += temp[i + r][j + c].rgbtGreen;
            total_blue += temp[i + r][j + c].rgbtBlue;

            counter += 1;
          }
        }
      }

      image[i][j].rgbtRed = round(total_red / (float) counter);
      image[i][j].rgbtGreen = round(total_green / (float) counter);
      image[i][j].rgbtBlue = round(total_blue / (float) counter);
    }
  }
}
