#include "blending_config.hpp"

namespace Blending
{

unsigned char mixColor(unsigned char BG, unsigned char FG, unsigned char alpha)
{
    return ((unsigned int) FG * alpha + (unsigned int) BG * (255 - alpha)) / 255;
}

void Blend(sf::Image &BGImage, sf::Image &FGImage, sf::Vector2i &position)
{
    unsigned X = position.x;
    unsigned Y = position.y;

    unsigned FGwidth  = FGImage.getSize().x;
    unsigned FGheight = FGImage.getSize().y;

    unsigned BGwidth  = BGImage.getSize().x;
    unsigned BGheight = BGImage.getSize().y;

    unsigned int *back = (unsigned int *) BGImage.getPixelsPtr();
    unsigned int *fore = (unsigned int *) FGImage.getPixelsPtr();

    unsigned int *dest = (unsigned int *) back;

    #ifdef __SSE_OPTIMISATION__

    const unsigned char I = 255, Z = 0x80;

    const __m128i   _0 =                   _mm_set_epi8(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
    const __m128i _255 = _mm_cvtepu8_epi16(_mm_set_epi8(I,I,I,I, I,I,I,I, I,I,I,I, I,I,I,I));
    
    for (unsigned y = 0; y < FGheight; ++y)
    {
        for (unsigned x = 0; x < FGwidth; x += 4)
        {
            if (y + Y > BGheight)   return;
            if (x + X + 3 > BGwidth) break;

            __m128i fr = _mm_load_si128((__m128i *) &fore[y * FGwidth + x]);
            __m128i bk = _mm_load_si128((__m128i *) &back[(y + Y) * BGwidth + x + X]);

            __m128i FR = (__m128i) _mm_movehl_ps((__m128) _0, (__m128) fr);
            __m128i BK = (__m128i) _mm_movehl_ps((__m128) _0, (__m128) bk);

            fr = _mm_cvtepu8_epi16(fr);
            FR = _mm_cvtepu8_epi16(FR);

            bk = _mm_cvtepu8_epi16(bk);
            BK = _mm_cvtepu8_epi16(BK);

            static const __m128i moveA = _mm_set_epi8 (Z, 14, Z, 14, Z, 14, Z, 14,
                                                       Z,  6, Z,  6, Z,  6, Z,  6);
            __m128i a = _mm_shuffle_epi8 (fr, moveA);                                // a [for r0/b0/b0...] = a0...
            __m128i A = _mm_shuffle_epi8 (FR, moveA);

            fr = _mm_mullo_epi16 (fr, a);                                            // fr *= a
            FR = _mm_mullo_epi16 (FR, A);

            bk = _mm_mullo_epi16 (bk, _mm_sub_epi16 (_255, a));                      // bk *= (255-a)
            BK = _mm_mullo_epi16 (BK, _mm_sub_epi16 (_255, A));

            __m128i sum = _mm_add_epi16 (fr, bk);                                    // sum = fr*a + bk*(255-a)
            __m128i SUM = _mm_add_epi16 (FR, BK);

            static const __m128i moveSum = _mm_set_epi8 ( Z,  Z,  Z, Z, Z, Z, Z, Z,
                                                         15, 13, 11, 9, 7, 5, 3, 1);
            sum = _mm_shuffle_epi8 (sum, moveSum);                                   // sum[i] = (sium[i] >> 8) = (sum[i] / 256)
            SUM = _mm_shuffle_epi8 (SUM, moveSum);

            __m128i color = (__m128i) _mm_movelh_ps ((__m128) sum, (__m128) SUM);    // color = (sumHi << 8*8) | sum

            _mm_store_si128 ((__m128i *) &dest[(y + Y) * BGwidth + x + X], color);
       }
    }

    #else // __SSE_OPTIMISATION__

    for (unsigned y = 0; y < FGheight; ++y)
    {
        for (unsigned x = 0; x < FGwidth; ++x)
        {
            if (x + X >= BGwidth || y + Y >= BGheight) continue;

            unsigned int  BColor = back[(y + Y) * BGwidth + x + X];
            unsigned int  FColor = fore[y * FGwidth + x];

            unsigned char ALPHA  = FColor >> 24;
            unsigned char BAlpha = BColor >> 24;

            unsigned int  result = BAlpha << 24;

            for (int i = 0; i < 3; ++i)
            {
                unsigned char FCol = ((0xFF << (8 * i)) & FColor) >> (8 * i);
                unsigned char BCol = ((0xFF << (8 * i)) & BColor) >> (8 * i);
                result            +=  mixColor(BCol, FCol, ALPHA) << (8 * i);
            }

            dest[(y + Y) * BGwidth + x + X] = result;
        }
    }

    #endif // __SSE_OPTIMISATION__
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
               "fore.png & pair x, y - picture position\n");
        return -1;
    }

    sf::Image background;
    if (!background.loadFromFile(argv[1]))
    {
        printf("cant get background image\n");
        return -2;
    }

    sf::Image foreground;
    if (!foreground.loadFromFile(argv[2]))
    {
        printf("cant get foreground image\n");
        return -3;
    }

    if (!isNumber(argv[3]) || !isNumber(argv[4]))
    {
        printf("arguments 3 & 4 should be integer numbers\n");
        return -4;
    }

    sf::Vector2i position(atoi(argv[3]), atoi(argv[4]));

    sf::Clock clock;
    clock.restart();

    for (int i = 0; i < 100; ++i)
        Blending::Blend(background, foreground, position);

    sf::Time time = clock.getElapsedTime();
    printf("time in microseconds is %lld\n", time.asMicroseconds());

    if (!background.saveToFile("output.png"))
    {
        printf("cant save file with name output.png\n");
        return -5;
    }

    return 0;
}
