#include "Video.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

#include "AppConfig.hpp"

void Video::linkAudio(std::string fileName, double videoLength) {
    std::string MusicUsed = RTX_MUSIC;

    if (videoLength > RTX_LEN) {
        int concatLen = videoLength / RTX_LEN + 1;

        std::string subconcat = RTX_MUSIC;
        for (int i = 1; i < concatLen; i++) {
            subconcat += '|';
            subconcat += RTX_MUSIC;
        }

        std::string concatCMD = "ffmpeg -y -i ";
        concatCMD += "\"concat:" + subconcat +
                     "\" -c:a copy -codec copy ./temp/RTXLONG.mp3";

        std::cout << "Executing: " << concatCMD << "\n";
        std::system(concatCMD.c_str());

        MusicUsed = "./temp/RTXLONG.mp3";
    }

    std::string safeFileName;
    for (char c : fileName) {
        if (c == ' ') {
            safeFileName.push_back('\\');
        }
        safeFileName.push_back(c);
    }

    // TODO Make Bitrate adjustable
    std::string lowerBitrateCommand =
        "ffmpeg -y -i " + safeFileName +
        ".avi -b:v 1024k -minrate 128k -maxrate 16M " + safeFileName +
        ".temp.avi";

    std::cout << "Executing: " << lowerBitrateCommand << "\n";
    std::system(lowerBitrateCommand.c_str());

    std::string linkAudioCommand("ffmpeg -y -i ");
    linkAudioCommand += safeFileName + ".temp.avi -i " + MusicUsed +
                        " -b:a 384k -map 0:v:0 -map 1:a:0 -c:v "
                        "copy -shortest ";
    linkAudioCommand += safeFileName + ".mp4";

    std::cout << "Executing: " << linkAudioCommand << "\n";
    std::system(linkAudioCommand.c_str());

    std::string cleanTempCmd;
    if (OS::isWindows())
        cleanTempCmd = "del";
    else
        cleanTempCmd = "rm";
    cleanTempCmd += " " + safeFileName + ".temp.avi " + safeFileName + ".avi";
    std::cout << "Executing: " << cleanTempCmd << "\n";
    std::system(cleanTempCmd.c_str());

    std::cout << "FFmpeg: Linking Audio Success\n";
    return;
}
