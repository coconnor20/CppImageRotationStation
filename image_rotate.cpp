#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

class PGMImage {
    private:
        int height;
        int width;
        int denom;
        std::vector<int> img_arr;
    public:
        PGMImage(int h, int w, int d, std::vector<int> ia){
            height = h;
            width = w;
            denom = d;
            img_arr = ia;
        }
        void printImg(PGMImage img){
            int size = img.height * img.width;
            for(int i = 0; i < size; i++){
                if(i % height == 0){
                    std::cout << std::endl;
                }
                std::cout << img.img_arr[i];
            }
        }
};

PGMImage readFile(std::string fname){
    int h = 0;
    int w = 0;
    int d = 0;
    std::ifstream infile(fname);
    std::stringstream ss;
    std::string buffer = "";

    //read first line to check pgm version
    std::getline(infile, buffer);
    if(buffer.compare("P5") != 0) std::cerr << "Invalid PGM Image" << std::endl;

    //ignore second line comment
    std::getline(infile, buffer);

    //get dims from third line
    ss << infile.rdbuf();
    ss >> h >> w >> d;

    //read in the rest of the image
    std::vector<int> img;
    int pbuf = 0;
    for(int i = 0; i < w*h; i++){
        ss >> pbuf;
        img.push_back(pbuf);
    }
    
    PGMImage myimg(h, w, d, img);
    return myimg;
}

int main(int argc, char** argv){
    if(argc > 2 || argc < 1) std::cerr << "Invalid Input| Usage: ./image_rotate <pgm image>" << std::endl;
    std::string fname = argv[1];
    PGMImage img = readFile(fname);
    img.printImg(img);
}