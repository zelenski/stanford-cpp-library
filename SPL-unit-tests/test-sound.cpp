/*
 * Test file for verifying the Stanford C++ lib sound module.
 */

#include "sound.h"
#include "gwindow.h"
#include <iostream>
#include <string>
#include "SimpleTest.h"

PROVIDED_TEST("Sound::playFile wav Joshua") {
    std::string filename = "res/shall-we-play-a-game.wav";
    Sound::playFile(filename);
    pause(1000);    // silence between tests
}

PROVIDED_TEST("Sound::playFile mp3 All Right Now") {
    std::string filename = "res/all-right-now.mp3";
    Sound::playFile(filename);
    pause(1000);    // silence between tests
}

PROVIDED_TEST("Sound::playURL wav url") {
    std::string url = "https://download.samplelib.com/wav/sample-3s.wav";
    Sound::playURL(url);
    pause(1000);    // silence between tests
}

PROVIDED_TEST("Sound::playFile error while file doesn't exist") {
    std::string filename = "NoSuchFile.mp3";
    EXPECT_ERROR(Sound::playFile(filename));
}

PROVIDED_TEST("Sound::playURL error when url doesn't exist") {
    std::string url = "http://nosuchfile.nosuchdomain.com";
    EXPECT_ERROR(Sound::playURL(url));
}

PROVIDED_TEST("Sound::playURL error on malformed url") {
    std::string url = "phtt://bogus";
    EXPECT_ERROR(Sound::playURL(url));
}

PROVIDED_TEST("Sound::playFile error when file not valid sound format") {
    std::string filename = "test-sound.cpp";
    EXPECT_ERROR(Sound::playFile(filename));
}

PROVIDED_TEST("Sound::playURL error when url not valid sound format") {
    std::string url = "http://www.google.com";
    EXPECT_ERROR(Sound::playURL(url));
}


