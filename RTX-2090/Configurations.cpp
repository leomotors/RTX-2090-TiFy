#include "Configurations.hpp"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "ImageHandler.hpp"

#define DEFAULT_OUTVID_RESOLUTION 480, 480
#define DEFAULT_FPS 30
#define DEFAULT_LOOP_DURATION 5.5
#define DEFAULT_LOOPS_COUNT 6
#define DEFAULT_ALGORITHM BLEND_S

const auto ItemsName = std::vector<std::string>{
    "Output Video Path", "Output Video Resolution",
    "Output Video FPS",  "Output Video Length Per Loop",
    "Number of Loops",   "Algorithm"};

const auto ItemsGuide = std::vector<std::string>{
    "",
    "Resolution of Output Video\nExample: 480x480",
    "FPS of Output Video",
    "Length of each Loop in the Video",
    "Number of Loops in the Video",
    "Algorithm used in rendering video\nPlease enter ID of your desire "
    "algorithm\n"};

const auto Algorithms = std::map<int, std::string>{
    {CORGI_LEGACY, "Corgi Legacy"},
    {BRIGHTNESS_COMPENSATE, "Corgi Legacy with Brightness Compensate"},
    {CORGI_HSV, "Corgi HIV"},
    {BLEND_S, "Blend Sukhoi"}};

Configurations::Configurations(ImageHandler &ImageHandlerRef)
    : ImageHandlerRef(ImageHandlerRef),
      ItemsName(ItemsName),
      ItemsGuide(ItemsGuide),
      Algorithms(Algorithms),
      Resolution({DEFAULT_OUTVID_RESOLUTION}),
      FPS(DEFAULT_FPS),
      LoopDuration(DEFAULT_LOOP_DURATION),
      nLoops(DEFAULT_LOOPS_COUNT),
      chosenAlgorithm(DEFAULT_ALGORITHM) {
    ItemsGuide[5] += [this]() -> std::string {
        std::string algodesc;
        for (auto algorithm : this->Algorithms)
            algodesc +=
                algorithm.second +
                ((algorithm.first == chosenAlgorithm) ? " (Default) " : "") +
                " : " + std::to_string(algorithm.first) + "\n";
        return algodesc;
    }();

    setWarpPosition();
}

void Configurations::setOutputPath(std::string &outputPath) {
    if (!outputPath.ends_with(".mp4")) outputPath += ".mp4";

    OutVideoPath = outputPath.substr(0, outputPath.size() - 4);
}

std::string Configurations::getPositionsAsString() {
    std::string result_str;
    for (std::pair<int, int> pos : WarpPosition) {
        result_str +=
            std::to_string(pos.first) + "x" + std::to_string(pos.second) + " ";
    }
    return result_str;
}

std::string Configurations::validateWarp(std::string toValidate) {
    std::stringstream ss(toValidate);

    std::vector<std::string> word_arr;
    std::string tmp;
    while (ss >> tmp) {
        word_arr.push_back(tmp);
    }

    if (word_arr.size() > nLoops) {
        return "Too Many Points Recieved";
    }

    std::vector<std::pair<int, int>> newWarp;
    for (std::string word : word_arr) {
        int x, y;
        std::sscanf(word.c_str(), "%dx%d", &x, &y);

        if (x < 0 || x >= Resolution.first) {
            std::string traceback = "The Value x of " + std::to_string(x) +
                                    " from " + word +
                                    " is not in the acceptable range.";
            return traceback;
        }

        if (y < 0 || y >= Resolution.second) {
            std::string traceback = "The Value y of " + std::to_string(y) +
                                    " from " + word +
                                    " is not in the acceptable range.";
            return traceback;
        }

        newWarp.push_back({x, y});
    }

    std::cout << "Warp Points Settings Validation Success and Set\n";
    setWarpPosition(newWarp);
    return "";
}

std::string Configurations::isRTXReady() {
    if (ImageHandlerRef.getImagePath().empty() || !ImageHandlerRef.status) {
        return "Input Path is not set or Image isn't loaded properly";
    }

    if (OutVideoPath.empty()) {
        return "Output Path is not set";
    }

    return "";
}

void Configurations::setWarpPosition(std::vector<std::pair<int, int>> newWarp) {
    while (newWarp.size() < nLoops)
        newWarp.push_back(
            {std::rand() % Resolution.first, std::rand() % Resolution.second});

    WarpPosition = newWarp;
}

std::string Configurations::validateConfig(int itemID, std::string toValidate) {
    switch (itemID) {
        case 1: {
            int w{0}, h{0};
            sscanf(toValidate.c_str(), "%dx%d", &w, &h);

            if (w < 144)
                return "Width of " + std::to_string(w) +
                       " is too small! Minimum is 144";
            if (h < 144)
                return "Height of " + std::to_string(h) +
                       " is too small! Minimum is 144";

            Resolution = {w, h};
            setWarpPosition();
            break;
        }
        case 2: {
            int fps{0};
            sscanf(toValidate.c_str(), "%d", &fps);

            if (fps < 1)
                return "FPS of " + std::to_string(fps) + " is too Small";

            FPS = fps;
            break;
        }
        case 3: {
            double vidlen{0.0};
            sscanf(toValidate.c_str(), "%lf", &vidlen);

            if (vidlen < 1.0)
                return "Video Length Per Loop of " + std::to_string(vidlen) +
                       " is too Small. Minimum is 1 second";

            LoopDuration = vidlen;
            break;
        }
        case 4: {
            int nLoopsNew{0};
            sscanf(toValidate.c_str(), "%d", &nLoopsNew);

            if (nLoopsNew < 1)
                return "Number of Loops of " + std::to_string(nLoopsNew) +
                       " will make video not exist!";

            nLoops = nLoopsNew;
            setWarpPosition();
            break;
        }
        case 5: {
            int newAlgorithm{0};
            sscanf(toValidate.c_str(), "%d", &newAlgorithm);

            if (Algorithms.find(newAlgorithm) == Algorithms.end())
                return "Algorithm with ID of " + std::to_string(newAlgorithm) +
                       " does not exist";

            chosenAlgorithm = newAlgorithm;
            break;
        }
        default:
            throw "Some Member Function or Friend Class is SUS";
    }
    return "";
}
