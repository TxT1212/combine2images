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
    int combine_image_rgb_rgb_mask();
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
    cout << "combineStyle " << combineStyle << endl;
    if (!combineStyle.compare("rgb_depth"))
    {
        combiner->combine_rgb_depth();
    }
    else if (!combineStyle.compare("image_mask"))
    {
        combiner->combine_image_mask();
    }
    else if (!combineStyle.compare("rgb_rgb_mask"))
    {
        combiner->combine_image_rgb_rgb_mask();
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
int Combine2images::combine_image_rgb_rgb_mask()
{
    Mat image_mask = imread(Settings_["mask_path"], -1); // depth
    Mat rgb_image_with_hole = imread(Settings_["rgb_image_with_hole"], -1);
    Mat rgb_image_reference = imread(Settings_["rgb_image_reference"], -1);
    string rgb_save_path = Settings_["rgb_save_path"];
    image_mask.convertTo(image_mask, CV_8UC1);
    Mat copy_rgb_mask(image_mask);
    image_mask.copyTo(copy_rgb_mask);
    vector<int> mask_value;
    Settings_["mask_value"] >> mask_value;
    for (size_t i = 0; i < image_mask.rows; i++)
    {
        for (size_t j = 0; j < image_mask.cols; j++)
        {
            if (rgb_image_with_hole.at<Vec3b>(i, j) == Vec3b(0, 0, 0) && image_mask.at<uchar>(i, j))
            {
                copy_rgb_mask.at<uchar>(i, j) = 255;
            }
            else
            {
                copy_rgb_mask.at<uchar>(i, j) = 0;
            }
        }
    }
    int kernel_size = Settings_["kernel_size"];
    Mat kernel_erode(kernel_size, kernel_size, CV_8UC1, 1);
    // cout << copy_rgb_mask.type() << copy_rgb_mask.empty() << endl;
    // cout << image_mask.empty() << endl;
    dilate(copy_rgb_mask, copy_rgb_mask, kernel_erode);
    rgb_image_reference.copyTo(rgb_image_with_hole, copy_rgb_mask);
    // imshow("1", rgb_image_with_hole);
    // imshow("3", rgb_image_reference);
    // imshow("2", copy_rgb_mask);
    // waitKey();
    imwrite(rgb_save_path, rgb_image_with_hole);

    return 0;
}

int Combine2images::combine_image_mask()
{
    // Mat image_in = imread(Settings_["image_path"], -1);
    Mat image_mask = imread(Settings_["mask_path"], -1);
    cout << "image_mask.empty() " << image_mask.empty() << image_mask.type() << endl;
    // cout << "image_in.empty() " << image_in.empty() << image_in.cols << endl;
    vector<int> mask_value;
    Settings_["mask_value"] >> mask_value;
    // cout << "helo" << endl;
    // for (size_t i = 0; i < mask_value.size(); i++)
    // {
    //     cout << mask_value[i] << " ";
    // }
    // cout << endl;

    // typedef cv::Point3_<uint8_t> Pixel;
    typedef uint8_t Pixel;
    image_mask.forEach<Pixel>([&](Pixel &pixel, const int position[]) -> void {
        uint8_t v = 1;
        for (size_t i = 0; i < mask_value.size(); i++)
        {
            if (pixel == mask_value[i])
            {
                pixel = 0;
                break;
            }
            pixel = 255;
        }
    });
    // Mat image_out;
    // image_in.copyTo(image_out, image_mask);
    // string image_out_path(Settings_["image_out_path"]);
    string mask_out_path(Settings_["mask_out_path"]);
    // cout << image_out_path << endl;
    // imwrite(image_out_path, image_out);
    cout << mask_out_path << endl;
    imwrite(mask_out_path, image_mask);

    return 0;
}