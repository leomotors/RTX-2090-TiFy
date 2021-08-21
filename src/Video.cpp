#include "Video.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

#include "AppConstants.h"

void Video::linkAudio(std::string fileName)
{
    std::string safeFileName;
    for (char c : fileName)
    {
        if (c == ' ')
        {
            safeFileName.push_back('\\');
        }
        safeFileName.push_back(c);
    }

    // TODO Make Bitrate adjustable
    std::string lowerBitrateCommand = "ffmpeg -y -i " + safeFileName +
                                      ".avi -b:v 1024k -minrate 128k -maxrate 16M " + safeFileName +
                                      ".temp.avi";

    std::cout << "Executing: " << lowerBitrateCommand << "\n";
    std::system(lowerBitrateCommand.c_str());

    std::string linkAudioCommand("ffmpeg -y -i ");
    linkAudioCommand += safeFileName + ".temp.avi -i " + RTX_MUSIC +
                        " -b:a 384k -map 0:v:0 -map 1:a:0 -c:v "
                        "copy -shortest ";
    linkAudioCommand += safeFileName + ".mp4";

    std::cout << "Executing: " << linkAudioCommand << "\n";
    std::system(linkAudioCommand.c_str());

    std::cout << "FFmpeg: Linking Audio Success\n";
    return;
}
