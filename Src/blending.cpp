#include "blending_config.hpp"

namespace Blending
{

unsigned char mixColor(unsigned char BG, unsigned char FG, unsigned char alpha)
{
    return ((unsigned int) FG * alpha + (unsigned int) BG * (255 - alpha)) / 255;
}

void Blend(sf::Image &BG, sf::Image &FG, sf::Vector2i &position)
{
    int X = position.x;
    int Y = position.y;

    int FGwidth  = FG.getSize().x;
    int FGheight = FG.getSize().y;

    int BGwidth  = BG.getSize().x;
    int BGheight = BG.getSize().y;

    // #define max(x, y) ((x) > (y)) ? (x) : (y)
    // #define min(x, y) ((x) < (y)) ? (x) : (y)

    // for (int i = max(Y, 0); i < min(FGheight + Y, BGheight); ++i)
    for (int y = 0; y < FGheight; ++y)
    {
        // for (int j = max(X, 0); j < min(FGwidth + X, BGwidth); ++j)
        for (int x = 0; x < FGwidth; ++x)
        {
            if (x + X + 1 > BGwidth || y + Y + 1 > BGheight) continue;

            sf::Color BGcolor = BG.getPixel(x + X, y + Y);
            sf::Color FGcolor = FG.getPixel(x, y);

            unsigned char r = 0, g = 0, b = 0, a = 255;

            r = mixColor(BGcolor.r, FGcolor.r, FGcolor.a);
            g = mixColor(BGcolor.g, FGcolor.g, FGcolor.a);
            b = mixColor(BGcolor.b, FGcolor.b, FGcolor.a);

            sf::Color resultPixel(r, g, b, a);

            BG.setPixel(x + X, y + Y, resultPixel);
        }
    }

    // #undef max
    // #undef min
}

};

bool isNumber(char *string)
{
    if (string == nullptr)      return false;

    char c = *string;
    if (c == '\0')              return false;

    if (c == '-' || c == '+') c = *(++string);

    do
    {
        if (c < '0' || c > '9') return false;
        c = *(++string);
    }
    while (c != '\0');

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("there is not enough arguments. I want back.png, "
               "fore.png & pair x, y - picture position");
        return -1;
    }

    sf::Image background;
    if (!background.loadFromFile(argv[1]))
    {
        printf("cant get background image");
        return -2;
    }

    sf::Image foreground;
    if (!foreground.loadFromFile(argv[2]))
    {
        printf("cant get foreground image");
        return -3;
    }

    if (!isNumber(argv[3]) || !isNumber(argv[4]))
    {
        printf("arguments 3 & 4 should be integer numbers");
        return -4;
    }

    sf::Vector2i position(atoi(argv[3]), atoi(argv[4]));

    Blending::Blend(background, foreground, position);

    if (!background.saveToFile("output.png"))
    {
        printf("cant save file with name output.png");
        return -5;
    }

    return 0;
}
