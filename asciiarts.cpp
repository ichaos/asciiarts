#include "CImg.h"
#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>

using namespace cimg_library;

// visible characters
#define LETTERS 126
// 16 * 8 pixels block for one character
#define WIDTH 4
#define HEIGHT 8

int greyScale[LETTERS];
char ascCode[LETTERS];

void ascii_init(void) {
	//Start from ' ' character.
	//End with '~'.
        int i = 31;
        while (i++ < 126) {
                greyScale[i] = i;
                ascCode[i] = i;
        }
}

unsigned int rgb2gray(unsigned int r, unsigned int g, unsigned int b) {
        double d = r * 0.2126 + g * 0.7152 + b * 0.0722;
        return (unsigned int)d;
}

char getAscii(int grey) {
        int max = LETTERS, min = 31, mid = 0;
        //grey = 110 - grey;
        if (grey <= greyScale[min]) {
                return ascCode[min];
        } else if (grey >= greyScale[max]) {
                return ascCode[max];
        } else {
                // binary search
                while (max > min + 1) {
                        mid = (max + min) >> 1;
                        if (grey > greyScale[mid]) {
                                min = mid;
                        } else {
                                max = mid;
                        }
                }
                return ascCode[mid];
        }
}

std::map<char, std::vector<unsigned char> > letters;

void getLetterDensity() {
    const unsigned char black[] = { 0,0,0 };
    for (int i = 33; i < 127; i++) {
        char letter[2] = "G";
        letter[0] = (char)i;
        //letter++;
        CImg<unsigned char> text(WIDTH, HEIGHT, 1, 1, 255);
        text.draw_text(0, 0, letter, black, 0, 1, 7);
        // get density
        //printf("=====%d: %c=======\n", i, letter[0]);
        std::vector<unsigned char> v;
        for (int h = 0; h < HEIGHT; h++) {
            for (int w = 0; w < WIDTH; w++) {
                //printf("%d ", text(w, h, 0));
                v.push_back(text(w, h, 0));
            }
            //printf("\n");
        }
        letters[i] = v;
        //text.display();
    }
}

int abs(int v) {
    if (v < 0) {
        v = -v;
    }
    return v;
}

char getLetter(std::vector<unsigned char> &block) {
    int dif = 1 << 30;
    int sum = 0;
    char target = 0;
    for (char i = 33; i < 127; i++) {
        sum = 0;
        for (int j = 0; j < block.size(); j++) {
            sum += abs(letters[i][j] - block[j]);
        }
        if (sum < dif) {
            dif = sum;
            target = i;
        }
    }
    return target;
}

int main(int argc,char **argv) {
        // load img
        const char *file_i = cimg_option("-i", "img/parrot_original.ppm", "Input image");
        const CImg<float> image(file_i);// = CImg<float>(file_i);//.normalize(0, 255);
        CImg<unsigned char> res = image.get_norm().normalize(0, 255);
        // convert image and print characters
        int imageWidth = 0, imageHeight = 0, imageGrey = 0;
        const unsigned char black[] = { 0,0,0 };
        //CImg<unsigned char> text(48, 32, 1, 3, 255);
        CImg<unsigned char> text(image.width() * 2, image.height() * 2, 1, 1, 255);
        //text.draw_text(0,0,"Google\nGoogle", black, 0, 1, 16);
	    imageWidth = image.width() / WIDTH;
    	imageHeight = image.height() / HEIGHT;

        ascii_init();
        getLetterDensity();
        //return 0;
        char letter[2] = "G";
        char buf[(imageWidth + 1) * imageHeight];
        std::vector<unsigned char> v(HEIGHT * WIDTH, '\0');
        for (int i = 0; i < imageHeight; i++) {
                for (int j = 0; j < imageWidth; j++) {
                        imageGrey = 0;
                        for (int h = 0; h < HEIGHT; h++) {
                                for (int w = 0; w < WIDTH; w++) {
                                        unsigned int
                                                r = image(j * WIDTH + w,
                                                          i * HEIGHT + h, 0),
                                                g = image(j * WIDTH + w,
                                                          i * HEIGHT + h, 1),
                                                b = image(j * WIDTH + w,
                                                          i * HEIGHT + h, 2);
                                        v[w + h * WIDTH] = rgb2gray(r, g, b);
                                        imageGrey += rgb2gray(r, g, b);
                                }
                        }
                        imageGrey /= (WIDTH * HEIGHT);
                        //imageGrey = (255 - imageGrey) * 90 / 255;
                        //imageGrey = imageGrey * 90 / 255;
                        //printf("%d", getAscii(imageGrey));
                        
                        //buf[j + i * (imageWidth + 1)] = getAscii(imageGrey);
                        letter[0] = getLetter(v); //getAscii(imageGrey);
                        //putchar(getAscii(imageGrey));
                        //putchar(letter);
                        text.draw_text(j * WIDTH * 2, i * HEIGHT * 2, letter, black, 0, 1, 13);
                }
                //putchar('=');
                //putchar('\n');
                buf[i * (imageWidth + 1) - 1] = '\n';
                text.draw_text(image.width(), i * HEIGHT, "\n", black, 0, 1, 13);
        }
        //const unsigned char black[] = { 0,0,0 };
        //CImg<unsigned char> text(48, 26, 1, 3, 255);
        //text.draw_text(0, 0, buf, black, 0, 1, 16);
        //CImg<unsigned char> text = CImg<>();

        (image, res.normalize(0, 255), text).display();
        //(image).display();
}
