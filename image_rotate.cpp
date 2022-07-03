#include<iostream>
#include<algorithm>
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
                if(i % width == 0){
                    std::cout << std::endl;
                }
                std::cout << img.img_arr[i];
            } 
        }

        float getAverageBrightness(PGMImage img){
            long average_brightness = 0.0;
            int imgsize = img.height * img.width;
            for(int i = 0; i < imgsize; i++){
                average_brightness += img.img_arr[i];
            }
            float ab = float(average_brightness) / float(imgsize);
            float result = ab/float(img.denom);
            return result;
        }

        void rotateLeft(PGMImage *img){
           std::vector<int> lrimg;
           for(int i = (img->width-1); i >= 0; i--){
               for(int j = 0; j < img->height; j++){
                   lrimg.push_back(img->img_arr[img->width * j + i]);
               }
           }
           int temp = img->width;
           img->width = img->height;
           img->height = temp;
           std::swap(img->img_arr, lrimg);
        }

        void rotateRight(PGMImage *img){
           std::vector<int> lrimg;
           for(int i = 0; i < img->width; i++){
               for(int j = (height-1); j >= 0; j--){
                   lrimg.push_back(img->img_arr[img->width * j + i]);
               }
           }
           int temp = img->width;
           img->width = img->height;
           img->height = temp;
           std::swap(img->img_arr, lrimg);
        }

        void rotateOneEighty(PGMImage *img){
            img->rotateLeft(img);
            img->rotateLeft(img);
        }

        void writeImage(PGMImage img){
            std::ofstream myfile;
            myfile.open("output.pgm");
            myfile << "P2\n";
            myfile << "# out.ascii.pgm \n";
            // format a string buffer of the height and width
            std::string linebuffer;
            linebuffer = std::to_string(img.width) + " " + std::to_string(img.height) + '\n';
            myfile << linebuffer;
            myfile << std::to_string(img.denom) + '\n';
            //clear the buffer and go print lines of image data
            linebuffer.clear();
            for(int row = 0; row < img.height; row++){
                for(int col = 0; col < img.width; col++){
                    linebuffer = linebuffer + std::to_string(img.img_arr[img.width * row + col]) + " ";  
                }
                linebuffer = linebuffer + '\n';
                myfile << linebuffer;
                linebuffer.clear();
            }

            myfile.close();
        }
};

PGMImage readImage(std::string fname){
    int h = 0;
    int w = 0;
    int d = 0;
    std::ifstream infile(fname);
    std::stringstream ss;
    std::string buffer = "";

    //read first line to check pgm version
    std::getline(infile, buffer);
    if(buffer.compare("P2") != 0) std::cerr << "Invalid PGM Image" << std::endl;

    //ignore second line comment
    std::getline(infile, buffer);

    //get dims from third line
    ss << infile.rdbuf();
    ss >> w >> h >> d;

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

int main(int argc, char* argv[]){
    if(argc > 3 || argc < 2) std::cerr << "Invalid Input| Usage: ./image_rotate <pgm image>" << std::endl;
    const std::vector<std::string> options = {"-L", "-l", "-R", "-r", "-180"};
    std::string opcode(argv[1]);
    std::string fname = argv[2];
    PGMImage img = readImage(fname);

    //check users commandline rotation option, and rotate if a valid input is received
    if(std::find(options.begin(), options.end(), opcode) != options.end()){
        if(opcode == "-L" || opcode == "-l"){
            img.rotateLeft(&img);
            img.writeImage(img);
        }
        else if(opcode == "-R" || opcode == "-r"){
            img.rotateRight(&img);
            img.writeImage(img);
        }
        else{
            img.rotateOneEighty(&img);
            img.writeImage(img);
        }
    }
    else{
        std::cerr << "invalid option; Options are -L, -R, -180\n" << std::endl;  
    }
    return 0;
}