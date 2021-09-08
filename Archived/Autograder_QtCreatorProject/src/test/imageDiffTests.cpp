/*
 * Test file for verifying the Stanford C++ lib image diff lib functionality.
 */

#include "testcases.h"
#include "assertions.h"
#include "gtest-marty.h"
#include "assertions.h"
#include "gbufferedimage.h"
#include "grid.h"
#include <iostream>
#include <string>
using namespace std;

#define TEST_TIMEOUT_DEFAULT_IMAGE_DIFF 300000

TEST_CATEGORY(ImageDiffTests, "string tests");

void amplifyContrast(GBufferedImage& image, int threshold = 0x01) {
    Grid<int> pixels = image.getPixelsARGB();
    for (int x = 0, w = image.getWidth(), h = image.getHeight(); x < w; x++) {
        for (int y = 0; y < h; y++) {
            int px = pixels[y][x];
            int red, green, blue;
            GBufferedImage::getRedGreenBlue(px, red, green, blue);
            red   = red   >= threshold ? 0xff : 0x00;
            green = green >= threshold ? 0xff : 0x00;
            blue  = blue  >= threshold ? 0xff : 0x00;
            pixels[y][x] = GBufferedImage::createArgbPixel(255, red, green, blue);
        }
    }
    image.setPixelsARGB(pixels);
}

void pixelate(GBufferedImage& image, int factor = 2) {
    Grid<int> pixels = image.getPixelsARGB();
    Grid<int> zoomed(pixels.numRows() / factor, pixels.numCols() / factor);

    // shrink
    int w = zoomed.width();
    int h = zoomed.height();
    int bigw = pixels.width();
    int bigh = pixels.height();
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            // set zoomed-out pixel to average of surrounding pixels
            int r = 0;
            int g = 0;
            int b = 0;
            int count = 0;

            int bigx = x * factor;
            int bigy = y * factor;
            for (int nx = bigx; nx < bigx + factor; nx++) {
                for (int ny = bigy; ny < bigy + factor; ny++) {
                    if (!pixels.inBounds(nx, ny)) {
                        continue;
                    }
                    count++;
                    int nr, ng, nb;
                    GBufferedImage::getRedGreenBlue(pixels[ny][nx], nr, ng, nb);
                    r += nr;
                    g += ng;
                    b += nb;
                }
            }

            if (count > 0) {
                int argb = GBufferedImage::createArgbPixel(255, r / count, g / count, b / count);
                zoomed[y][x] = argb;
            }
        }
    }

    // grow
    for (int x = 0; x < bigw; x++) {
        for (int y = 0; y < bigh; y++) {
            if (zoomed.inBounds(y / factor, x / factor)) {
                pixels[y][x] = zoomed[y / factor][x / factor];
            }
        }
    }
    image.setPixelsARGB(pixels);
}

TIMED_TEST(ImageDiffTests, imageDiffLineOrderTest1, TEST_TIMEOUT_DEFAULT_IMAGE_DIFF) {
    GBufferedImage image1("sierpinski-order1-1.png");
    GBufferedImage image2("sierpinski-order1-2.png");
    amplifyContrast(image1);
    amplifyContrast(image2);
//    pixelate(image1, 3);
//    pixelate(image2, 3);
//    amplifyContrast(image1);
//    amplifyContrast(image2);
    image1.save("sierpinski-order1-1-out.png");
    image2.save("sierpinski-order1-2-out.png");
    const int TOLERANCE = 400;
    assertSimilarImage("Sierpinski order-1 drawn opposite way",
                       image1, image2, TOLERANCE);
}

TIMED_TEST(ImageDiffTests, imageDiffLineOrderTest2, TEST_TIMEOUT_DEFAULT_IMAGE_DIFF) {
    GBufferedImage image1("sierpinski-order2-1.png");
    GBufferedImage image2("sierpinski-order2-2.png");
    amplifyContrast(image1);
    amplifyContrast(image2);
//    pixelate(image1);
//    pixelate(image2);
    image1.save("sierpinski-order2-1-out.png");
    image2.save("sierpinski-order2-2-out.png");
    const int TOLERANCE = 600;
    assertSimilarImage("Sierpinski order-2 drawn opposite way",
                       image1, image2, TOLERANCE);
}

TIMED_TEST(ImageDiffTests, imageDiffLineOrderTest5, TEST_TIMEOUT_DEFAULT_IMAGE_DIFF) {
    GBufferedImage image1("sierpinski-order5-1.png");
    GBufferedImage image2("sierpinski-order5-2.png");
    amplifyContrast(image1);
    amplifyContrast(image2);
//    pixelate(image1);
//    pixelate(image2);
    image1.save("sierpinski-order5-1-out.png");
    image2.save("sierpinski-order5-2-out.png");
    const int TOLERANCE = 1500;
    assertSimilarImage("Sierpinski order-5 drawn opposite way",
                       image1, image2, TOLERANCE);
}
