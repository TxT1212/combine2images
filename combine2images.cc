#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Combine2images
{
private:
    FileStorage Settings_;

public:
    Combine2images(FileStorage Settings);
    int combine_rgb_depth();
    int combine_image_mask();
    ~Combine2images();
};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "missing config" << endl;
        exit(1);
    }
    string yaml_path = argv[1];
    FileStorage Settings(yaml_path, FileStorage::READ);
    Combine2images *combiner = new Combine2images(Settings);
    string combineStyle = Settings["combineStyle"];
    if (!combineStyle.compare("rgb_depth"))
    {
        combiner->combine_rgb_depth();
    }
    else if (!combineStyle.compare("image_mask"))
    {
        combiner->combine_image_mask();
    }
    else
    {
        cout << "wrong style!" << endl;
        exit(-1);
    }

    return 0;
}

Combine2images::Combine2images(FileStorage Settings) : Settings_(Settings)
{
}

Combine2images::~Combine2images()
{
}
int Combine2images::combine_rgb_depth()
{

    return 0;
}
int Combine2images::combine_image_mask()
{
    int mask_alpha = Settings_["mask_alpha"];
    Mat image_rgb = imread(Settings_["image_path"], -1);
    Mat image_mask = imread(Settings_["mask_path"], -1);
    vector<int> mask_value;
    Settings_["mask_value"] >> mask_value;
    cout << "helo" << endl;
    for (size_t i = 0; i < mask_value.size(); i++)
    {
        cout << mask_value[i] << " ";
    }
    cout << endl;

    typedef cv::Point3_<uint8_t> Pixel;
    // Mat_<Pixel> image = Mat::zeros(3, sizes, CV_8UC3);
    image_mask.forEach<Pixel>([&](Pixel &pixel, const int position[]) -> void {
        for (size_t i = 0; i < mask_value.size(); i++)
        {
            if(pixel.x == mask_value[i])
            {
                pixel.x = pixel.y = pixel.z = 0;
                break;
            }
        }
    });

    return 0;
}