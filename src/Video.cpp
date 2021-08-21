#include "Video.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

void Video::linkAudio(std::string input, std::string output)
{
    std::string toExec("ffmpeg -y -i ");
    toExec += input + " -i " + RTX_MUSIC +
              " -b:a 384k -map 0:v:0 -map 1:a:0 -c:v "
              "copy -shortest -b:v 1024k ";
    toExec += output;

    std::cout << "Executing: " << toExec << "\n";
    std::system(toExec.c_str());
    std::cout << "FFmpeg: Linking Audio Success\n";
    return;
}
