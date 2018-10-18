/*
 * File: assertions.cpp
 * --------------------
 * 
 * @author Marty Stepp
 * @version 2018/10/04
 * - initial version (code moved from autograder.cpp)
 */

#define INTERNAL_INCLUDE 1
#include "assertions.h"
#include "gbufferedimage.h"
#undef INTERNAL_INCLUDE

// declared in assertions.h
void assertSimilarImage(const std::string& msg,
                        const std::string& imagefile1,
                        const std::string& imagefile2,
                        int diffPixelTolerance,
                        int xmin, int ymin,
                        int xmax, int ymax) {
    GBufferedImage image1(imagefile1);
    GBufferedImage image2(imagefile2);
    int diffPixels;
    if (xmin >= 0 && ymin >= 0 && xmax >= xmin && ymax >= ymin) {
        diffPixels = image1.countDiffPixels(image2, xmin, ymin, xmax, ymax);
    } else {
        diffPixels = image1.countDiffPixels(image2);
    }
    bool imagesAreEqual = diffPixels <= diffPixelTolerance;

    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    autograder->setFailDetails(stanfordcpplib::autograder::UnitTestDetails(
        stanfordcpplib::autograder::UnitTestType::TEST_ASSERT_DIFF_IMAGE,
        msg, imagefile1, imagefile2, "image",
        imagesAreEqual));
    EXPECT_TRUE(imagesAreEqual);
}

// declared in assertions.h
void assertEqualsImage(const std::string& msg,
                       const std::string& imagefile1,
                       const std::string& imagefile2) {
    GBufferedImage image1(imagefile1);
    GBufferedImage image2(imagefile2);
    bool imagesAreEqual = image1.equals(image2);

    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    autograder->setFailDetails(stanfordcpplib::autograder::UnitTestDetails(
        stanfordcpplib::autograder::UnitTestType::TEST_ASSERT_DIFF_IMAGE,
        msg, imagefile1, imagefile2, "image",
        imagesAreEqual));
    EXPECT_TRUE(imagesAreEqual);
}
