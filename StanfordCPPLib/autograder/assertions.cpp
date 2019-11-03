/*
 * File: assertions.cpp
 * --------------------
 * 
 * @author Marty Stepp
 * @version 2018/10/04
 * - initial version (code moved from autograder.cpp)
 */

#include "assertions.h"

void assertEqualsImage(const std::string& msg,
                       GBufferedImage& image1,
                       GBufferedImage& image2) {
    bool imagesAreEqual = image1.equals(image2);

    stanfordcpplib::autograder::Autograder* autograder = stanfordcpplib::autograder::Autograder::instance();
    autograder->setFailDetails(stanfordcpplib::autograder::UnitTestDetails(
            stanfordcpplib::autograder::UnitTestType::TEST_ASSERT_DIFF_IMAGE,
            msg,
            image1.getFilename().empty() ? std::string("image 1") : image1.getFilename(),
            image2.getFilename().empty() ? std::string("image 2") : image2.getFilename(),
            "image",
            imagesAreEqual));
    EXPECT_TRUE(imagesAreEqual);
}

void assertEqualsImage(const std::string& msg,
                       const std::string& imagefile1,
                       const std::string& imagefile2) {
    GBufferedImage image1(imagefile1);
    GBufferedImage image2(imagefile2);
    assertEqualsImage(msg, image1, image2);
}

void assertSimilarImage(const std::string& msg,
                        GBufferedImage& image1,
                        GBufferedImage& image2,
                        int diffPixelTolerance,
                        int xmin, int ymin,
                        int xmax, int ymax) {
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
            msg,
            image1.getFilename().empty() ? std::string("image 1") : image1.getFilename(),
            image2.getFilename().empty() ? std::string("image 2") : image2.getFilename(),
            "image",
            imagesAreEqual));
    EXPECT_TRUE(imagesAreEqual);
}

void assertSimilarImage(const std::string& msg,
                        const std::string& imagefile1,
                        const std::string& imagefile2,
                        int diffPixelTolerance,
                        int xmin, int ymin,
                        int xmax, int ymax) {
    GBufferedImage image1(imagefile1);
    GBufferedImage image2(imagefile2);
    assertSimilarImage(msg, image1, image2, diffPixelTolerance, xmin, ymin, xmax, ymax);
}
