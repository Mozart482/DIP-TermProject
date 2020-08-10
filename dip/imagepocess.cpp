#include "imageprocess.h"

/*
*Summary: zero padding for filtering operations
*Parameters:
*    const float *r : pointer to the starting location of orginal rgb/red channel's image memory block
*    const float *g : pointer to the starting location of orginal green channel's image memory block
*    const float *b : pointer to the starting location of orginal green channel's image memory block
*    const int width : image width
*    const int height : image height
*    const int half_pad_width : filter size
*    const int half_pad_height : filter size
*    float *nr : pointer to the starting location of padded rgb/red channel's  image memory block
*    float *ng : pointer to the starting location of padded green channel's image memory block
*    float *nb : pointer to the starting location of padded green channel's image memory block
*
*/

void paddingZeros(const float *r, const float *g, const float *b,
                   const int width, const int height,
                   const int half_pad_width, const int half_pad_height,
                   float *nr, float *ng, float *nb)
{
    int nw = width + 2*half_pad_width;
    int nh = height + 2*half_pad_height;

    // set the value of the first nh*nw*sizeof(float) bytes of the memory space nr/ng/nb to the value 0
    memset(nr, 0, nh*nw*sizeof(float));
    memset(ng, 0, nh*nw*sizeof(float));
    memset(nb, 0, nh*nw*sizeof(float));

    for (int j = half_pad_height; j < nh-half_pad_height; j++) {
        // Copy the values of "width*sizeof(float)" bytes from the location pointed to by source directly ("r+(j-half_pad_height)*width")
        // to the memory block pointed to by destination.
        memcpy(nr+j*nw+half_pad_width, r+(j-half_pad_height)*width, width*sizeof(float));
        memcpy(ng+j*nw+half_pad_width, g+(j-half_pad_height)*width, width*sizeof(float));
        memcpy(nb+j*nw+half_pad_width, b+(j-half_pad_height)*width, width*sizeof(float));
    }
}

/*
*Summary: Split the rgb image into three channels : r, g, b
*Parameters:
*    QImage &image : orginal image
*    float *r : Spilt r channel's image
*    float *g : Spilt g channel's image
*    float *b : Spilt b channel's image
*/

void splitImageChannel(QImage &image, float *r, float *g, float *b)
{
    int width = image.width();
    int height = image.height();
    int count = 0;
    // Traverse the entire image and obtain the corresponding channel's pixel value
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            r[count] = qRed(pixel);
            g[count] = qGreen(pixel);
            b[count] = qBlue(pixel);
            count++;
        }
    }
}

void splitImageChannel(QImage &image, float *rgb)
{
    int width = image.width();
    int height = image.height();
    int count = 0;
    // Traverse the entire image and obtain the corresponding channel's pixel value
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            rgb[j*3*width+3*i] = (float)qRed(pixel);
            rgb[j*3*width+3*i+1] = (float)qGreen(pixel);
            rgb[j*3*width+3*i+2] = (float)qBlue(pixel);
            count++;
        }
    }
}

void splitImageChannel(QImage &image, uchar *r, uchar *g, uchar *b)
{
    int width = image.width();
    int height = image.height();
    int count = 0;
    // Traverse the entire image and obtain the corresponding channel's pixel value
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            r[count] = qRed(pixel);
            g[count] = qGreen(pixel);
            b[count] = qBlue(pixel);
            count++;
        }
    }
}

void splitImageChannel(QImage &image, uchar *rgb)
{
    int width = image.width();
    int height = image.height();
    int count = 0;
    // Traverse the entire image and obtain the corresponding channel's pixel value
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            rgb[j*3*width+3*i] = (uchar)qRed(pixel);
            rgb[j*3*width+3*i+1] = (uchar)qGreen(pixel);
            rgb[j*3*width+3*i+2] = (uchar)qBlue(pixel);
            count++;
        }
    }
}

/*
*Summary: Combine three image channels into rgb image
*Parameters:
*    float *r : r channel's image
*    float *g : g channel's image
*    float *b : b channel's image
*    int w : the image width
*    int h : the image height
*    QImage &image : Combined rgb image
*/

void concatenateImageChannel(float *r, float *g, float *b, int w, int h, QImage &image)
{
    image = QImage(w, h, QImage::Format_RGB888);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            QRgb pixel = qRgb((int)r, (int)g, (int)b);
            image.setPixel(i, j, pixel);
        }
    }
}

void concatenateImageChannel(float *rgb, int w, int h, QImage &image)
{
    image = QImage(w, h, QImage::Format_RGB888);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            int r = (int)rgb[j*3*w+3*i];
            int g = (int)rgb[j*3*w+3*i+1];
            int b = (int)rgb[j*3*w+3*i+2];
            QRgb pixel = qRgb(r, g, b);
            image.setPixel(i, j, pixel);
        }
    }
}

void concatenateImageChannel(uchar *r, uchar *g, uchar *b, int w, int h, QImage &image)
{
    image = QImage(w, h, QImage::Format_RGB888);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            QRgb pixel = qRgb((int)r, (int)g, (int)b);
            image.setPixel(i, j, pixel);
        }
    }
}

void concatenateImageChannel(uchar *rgb, int w, int h, QImage &image)
{
    image = QImage(w, h, QImage::Format_RGB888);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            int r = (int)rgb[j*3*w+3*i];
            int g = (int)rgb[j*3*w+3*i+1];
            int b = (int)rgb[j*3*w+3*i+2];
            QRgb pixel = qRgb(r, g, b);
            image.setPixel(i, j, pixel);
        }
    }
}

/*
*Summary: Convert rgb(ycrcb) image to ycrcb(rgb) image
*Parameters:
*    uchar *r : r channel's image
*    uchar *g : g channel's image
*    uchar *b : b channel's image
*    int size : image size( width × height )
*    float *y : the luma component
*    float *cr: the red-difference chroma component
*    float *cb: the blue-difference chroma component
*/

void rgb2ycrcb(uchar *r, uchar *g, uchar *b, int size, float *y, float *cr, float *cb)
{
    for (int i=0; i<size; i++)
    {
        y[i] = 0.256789 * r[i] + 0.504129 * g[i] + 0.097906 * b[i] + 16;
        cb[i] = -0.148223 * r[i] - 0.290992 * g[i] + 0.439215 * b[i] + 128;
        cr[i] = 0.439215 * r[i] - 0.367789 * g[i] - 0.071426 * b[i] + 128;
    }
}

void ycrcb2rgb(float *y, float *cr, float *cb, int size, uchar *r, uchar *g, uchar *b)
{
    for (int i=0; i<size; i++)
    {
        r[i] = 1.164383 * (y[i]-16) + 1.596027 * (cr[i]-128);
        g[i] = 1.164383 * (y[i]-16) - 0.391762 * (cb[i]-128)- 0.812969 * (cr[i]-128);
        b[i] = 1.164383 * (y[i]-16) + 2.017230 * (cb[i]-128);
    }
}

void qimage2ycrcb(QImage image, float *y, float *cr, float *cb)
{
    int pixel_num = image.width()*image.height();
    // obtain image channels
    uchar *channels = new uchar[pixel_num*3];
    uchar *r = channels;
    uchar *g = channels+pixel_num;
    uchar *b = channels+2*pixel_num;
    splitImageChannel(image, r, g, b);

    // rgb to ycrcb
    rgb2ycrcb(r, g, b, pixel_num, y, cr, cb);

    delete [] channels;
}

void ycrcb2qimage(float *y, float *cr, float *cb, int width, int height, QImage &image)
{
    int pixel_num = width*height;

    uchar *channels = new uchar[pixel_num*3];
    uchar *r = channels;
    uchar *g = channels+pixel_num;
    uchar *b = channels+2*pixel_num;

    // ycrcb to rgb
    ycrcb2rgb(y, cr, cb, pixel_num, r, g, b);

    // update image
    int count = 0;
    image = QImage(width, height, QImage::Format_RGB888);
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            int nr = r[count];
            int ng = g[count];
            int nb = b[count];
            count++;
            image.setPixel(i, j, qRgb(nr,ng,nb));
        }
    }
    delete [] channels;
}

/*
*Summary: calculating image histogram of single-channel
*Parameters:
*    QImage &image : input original image
*    ImageChannel channel : specify image channel(Y/R/G/B)
*/

QImage calculateHistogram(QImage &image, ImageChannel channel)
{
    // obtain gray image
    QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
    int width = grayImage.width();
    int height = grayImage.height();
    uchar *r = new uchar[width*height];
    uchar *g = new uchar[width*height];
    uchar *b = new uchar[width*height];

    // seperate RGB channel
    splitImageChannel(image, r, g, b);

    QRgb hist_ior;

    uchar *bits = nullptr;
    switch (channel) {
        case ImageChannel::Y:
            hist_ior = qRgba(128, 128, 128, 255);
            bits = grayImage.bits();
            break;
        case ImageChannel::R:
            hist_ior = qRgba(255, 0, 0, 255);
            bits = r;
            break;
        case ImageChannel::G:
            hist_ior = qRgba(0, 255, 0, 255);
            bits = g;
            break;
        case ImageChannel::B:
            hist_ior = qRgba(0, 0, 255, 255);
            bits = b;
            break;
    }

    const int gray_level = 256;
    int hist[gray_level] = {0};

    // calculate histogram
    for (int i=0; i<width*height; i++)
    {
       int val = bits[i];
       hist[val]++;
    }

    // compress histogram into hist_image height
    int max_hist_val = hist[0];
    for (int i=1; i<gray_level; i++)
    {
        max_hist_val = hist[i] > max_hist_val ? hist[i] : max_hist_val;
    }

    int s_w = 2;
    int w = s_w*gray_level;
    int h = w;//width*height;
    float s_h = 0.8;

    for (int i=0; i<gray_level; i++)
    {
        int v = hist[i];
        hist[i] = int(h*1.0/max_hist_val * s_h * v);
    }

    QImage hist_image(w, h, QImage::Format_RGBA8888);
    QRgb value;
    for (int j=0; j<h; j++)
    {
        for (int i=0; i<w; i++)
        {
            if ( (hist[i/s_w] > 0) && (j >= h-hist[i/s_w]) )
                value = hist_ior;
            else
                value = qRgba(255, 255, 255, 255);
            hist_image.setPixel(i, j, value);
        }
    }
    delete [] r;
    delete [] g;
    delete [] b;

    return hist_image;
}

/*
*Summary: calculating the negative image
*Parameters:
*    QImage &image : input original image
*    ImageChannel channel : specify image channel(Y/R/G/B)
*/
QImage calculateNegative(QImage &image, ImageChannel channel)
{
    // obtain gray image
    int width = image.width();
    int height = image.height();

    QImage newImage = image; // Create a new QImage type image for return
    int r, g, b;
    // Traverse the entire image and modify the corresponding pixel value
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = image.pixel(i, j);
            r = qRed(pixel);
            g = qGreen(pixel);
            b = qBlue(pixel);
            QRgb newPixel;
            switch (channel) {
                case ImageChannel::Y:
                    newPixel = qRgb(255-r, 255-g, 255-b); // negative for all channels
                    break;
                case ImageChannel::R:
                    newPixel = qRgb(255-r, g, b);  // negative for Red channel
                    break;
                case ImageChannel::G:
                    newPixel = qRgb(r, 255-g, b);   // negative for Green channel
                    break;
                case ImageChannel::B:
                    newPixel = qRgb(r, g, 255-b);   // negative for Blue channel
                    break;
            }
            newImage.setPixel(i, j, newPixel);   // replace the original pixel with the calculated new pixel
          }
    }

    return newImage;
}


/*
*Summary: calculating the pseudo color image
*        (A colormap is an m-by-3 matrix of real numbers between 0.0 and 1.0.
*         Each row is an RGB vector that defines one color.)
*Parameters:
*    QImage &image : input original image
*    ColorMap map : specify image channel(Y/R/G/B)
*Describe：currently our software supports three types of Colormap：Jet,Hot and Parula
*/
QImage convertToPseudoColor(QImage &image, ColorMap map)
{
    int width = image.width();
    int height = image.height();
    // obtain gray image
    QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);

    // seperate RGB channel
    QImage newImage(width, height, QImage::Format_RGB888);
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            QRgb pixel = grayImage.pixel(i, j);
            QRgb newPixel;
            int index, r, g, b;
            index = qRed(pixel); // gray scale
            //because it is a grayscale image, the values of the three channels are the same
            r = qRed(pixel);
            g = qGreen(pixel);
            b = qBlue(pixel);
            switch (map) {
                case ColorMap::Jet:
                    r = jet_table[index*3];
                    g = jet_table[index*3+1];
                    b = jet_table[index*3+2];
                    break;
                case ColorMap::Parula:
                    r = parula_table[index*3];
                    g = parula_table[index*3+1];
                    b = parula_table[index*3+2];
                    break;
                case ColorMap::Hot:
                    r = hot_table[index*3];
                    g = hot_table[index*3+1];
                    b = hot_table[index*3+2];
                    break;
            }
            // convert grayscale image's pixel into pseudo-color according to the type of colormap
            newPixel = qRgb(r, g, b);
            newImage.setPixel(i, j, newPixel);
          }
    }
    return newImage;
}

/*
* Histogram Equalization is a method that improves the contrast in an image, in order to stretch out the intensity range.
* Equalization implies mapping one distribution (the given histogram) to another distribution
* (a wider and more uniform distribution of intensity values) so the intensity values are spreaded over the whole range
*
*/
QImage equalizeHistogramProc1(QImage &image)
{
    int width = image.width();
    int height = image.height();
    int pixel_num = width*height;

    // obtain image channels
    uchar *channels = new uchar[pixel_num*3];
    uchar *r = channels;
    uchar *g = channels+pixel_num;
    uchar *b = channels+2*pixel_num;
    splitImageChannel(image, r, g, b);

    // rgb to ycbcr
    float *ycrcb = new float[pixel_num*3];
    float *y = ycrcb;
    float *cr = ycrcb+pixel_num;
    float *cb = ycrcb+2*pixel_num;
    rgb2ycrcb(r, g, b, pixel_num, y, cr, cb);

    // calculate hist/pdf
    int *hist = new int[pixel_num]; // hist/pdf
    const int gray_level = 256;
    float *gray_distribution = new float[gray_level];// cdf

    uchar *gray_equal = new uchar[gray_level]; // equalized gray

        // calculate pdf
        memset(hist, 0, pixel_num*sizeof(int));
        for (int i=0; i<pixel_num; i++)
        {
            int index = (int)y[i]; // gray scale
            hist[index]++;
        }

        // calculate cdf
        memset(gray_distribution, 0, gray_level*sizeof(float));
        gray_distribution[0] = hist[0]*1.0f/pixel_num;
        for (int i = 1; i < gray_level; i++)
        {
            gray_distribution[i] = gray_distribution[i-1] + hist[i]*1.0f/pixel_num;
        }

        // recalculate equalized gray
        memset(gray_equal, 0, gray_level*sizeof(uchar));
        for (int i = 0; i < gray_level; i++)
        {
            gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
        }

        // new gray channel
        for (int i=0; i<pixel_num; i++)
        {
            int index = (int)y[i]; // gray scale
            y[i] = gray_equal[index];
        }

        // ycrcb to rgb
    ycrcb2rgb(y, cr, cb, pixel_num, r, g, b);

    // update image
    int count = 0;
    QImage newImage = image;//grayImage;
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            int nr = r[count];
            int ng = g[count];
            int nb = b[count];
            count++;
            newImage.setPixel(i, j, qRgb(nr,ng,nb));
        }
    }
    delete [] gray_equal;
    delete [] gray_distribution;
    delete [] channels;
    delete [] ycrcb;

    return newImage;
}

QImage equalizeHistogramProc(QImage &image)
{
    int width = image.width();
    int height = image.height();
    int pixel_num = width*height;

    // obtain gray image
    uchar *channels = new uchar[width*height*3];    // dynamically allocate memory for the array of three channels' pixels(R,G,B)
    uchar *r = channels; //the R channel is stored at the beginning of this memory space
    //the interval between the starting positions of the r,g,and b channels in the memory space is the space occupied by a grayscale image
    uchar *g = channels+width*height;
    uchar *b = channels+2*width*height;
    splitImageChannel(image, r, g, b);

    uchar *c[4] = {r, g, b, 0};

    // calculate hist/pdf(probability density function)
    int *hist = new int[pixel_num]; // dynamically allocate memory for the array of hist
    const int gray_level = 256;
    float *gray_distribution = new float[gray_level];// cdf

    uchar *gray_equal = new uchar[gray_level]; // equalized gray
    for (uchar **p=c; (*p) != 0; p++)
    {

        memset(hist, 0, pixel_num*sizeof(int)); // set the value of the first pixel_num bytes of the allocated memory space hist to the value 0
         // calculate hist
        for (int i=0; i<pixel_num; i++)
        {
            int index = (*p)[i]; // gray scale
            hist[index]++;
        }

        memset(gray_distribution, 0, gray_level*sizeof(float)); // set the value of the first gray_level bytes of the allocated memory space gray_distribution to the value 0

        // calculate pdf&cdf
        gray_distribution[0] = hist[0]*1.0f/pixel_num;
        for (int i = 1; i < gray_level; i++)
        {
            gray_distribution[i] = gray_distribution[i-1] + hist[i]*1.0f/pixel_num;
        }

        // recalculate equalized gray
        memset(gray_equal, 0, gray_level*sizeof(uchar)); // set the value of the first gray_equal bytes of the allocated memory space gray_level to the value 0
        for (int i = 0; i < gray_level; i++)
        {
            gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
        }

        // new gray channel
        for (int i=0; i<pixel_num; i++)
        {
            int index = (*p)[i]; // current pixel's gray scale
            (*p)[i] = gray_equal[index];
        }
    }

    // update image
    int count = 0;
    QImage newImage = image; // grayImage;
    for (int j=0; j<height; j++) {
        for (int i=0; i<width; i++) {
            int nr = r[count];
            int ng = g[count];
            int nb = b[count];
            count++;
            newImage.setPixel(i, j, qRgb(nr,ng,nb));
        }
    }
    // release the occupied memory
    delete [] gray_equal;
    delete [] gray_distribution;
    delete [] channels;

    return newImage;
}

/*
*Summary: perform filtering operation
*
*Parameters:
*    uchar *src : input original image
*    int image_width :
*    int image_height :
*    float *kernel :
*    int kernel_width :
*    int kernel_height :
*    uchar *dst :
*/

void filter(uchar *src, int image_width, int image_height,
            float *kernel, int kernel_width, int kernel_height, uchar *dst)
{
    int i, j, m, n;
    float val;

    memset(dst, 0, image_width*image_height);

    for (j=kernel_height/2; j<image_height-kernel_height/2; j++)
    {
        for (i = kernel_width/2; i<image_width-kernel_width/2; i++)
        {
            val = 0;
            for (n=-kernel_height/2; n<=kernel_height/2; n++)
            {
                for (m=-kernel_width/2; m<=kernel_width/2; m++)
                {
                    val += src[(j-n)*image_width+(i-m)] *
                            kernel[(n+kernel_height/2)*kernel_width+m+kernel_width/2];
                }
            }
            val = val>255 ? 255 : val;
            dst[j*image_width+i] = (uchar)(val<0 ? 0 : val);
        }
    }
}

/*
*Summary: Adaptive Contrast Enhancement
*
*Parameters:
*    QImage &src_image : input original image
*    float *rgb : pointer to the starting location of orginal rgb image memory block
*    float *rgb_ii : pointer to the starting location of integral image memory block
*    float *rgb_ii_power : pointer to the starting location of integral image power memory block
*    int max_window_size : the local area size
*    int half_window_size : half window size
*    float alpha : constrast gain factor
*    float max_cg : max cg
*    QImage &dst_image : output enhanced image
* Describtion: The procedures of the algorithm are as follows:
*     (1) Calculate the low-frequency part of the image by low-pass filtering;
*     (2) Get the high-frequency part of the image by subtracting the original image and the low-frequency component;
*     (3) Amplify the high-frequency part and superimpose it with the low-frequency part, then we can get the enhanced image.
*
*/
void adaptiveContrastEnhancement(QImage &src_image, float *rgb, float *rgb_ii, float *rgb_ii_power, int max_window_size,
                                 int half_window_size, float alpha, float max_cg, QImage &dst_image)
{
    dst_image = src_image;
    int image_width = src_image.width();
    int image_height = src_image.height();
    int pixel_num = image_width*image_height;

    int max_image_width = src_image.width() + 2*max_window_size;
    int max_image_height = src_image.height() + 2*max_window_size;
    int max_pixel_num = max_image_width*max_image_height;
    int max_kernel_height = 2*max_window_size+1;
    int max_kernel_width = 2*max_window_size+1;

    int i=0, j=0;
    int kernel_height = 2*half_window_size+1;
    int kernel_width = 2*half_window_size+1;
    int kernel_size = kernel_height*kernel_width;
    float image_mean=0, image_std=0;
    for (int c=0; c<3; c++)
    {
        // image mean
        image_mean = box_integral(rgb_ii+c*max_pixel_num, max_image_width, max_image_height,
                               max_window_size, max_window_size + image_width-1,
                               max_window_size, max_window_size + image_height-1);
        image_mean /= pixel_num;

        // image std
        image_std = box_integral(rgb_ii_power+c*max_pixel_num, max_image_width, max_image_height,
                               max_window_size, max_window_size + image_width-1,
                               max_window_size, max_window_size + image_height-1);
        image_std /= pixel_num;
        image_std -= image_mean*image_mean;

        image_std = sqrtf(image_std);

        // local area mean and std
        for (j=max_kernel_height/2; j<max_image_height-max_kernel_height/2; j++)
        {
            for (i=max_kernel_width/2; i<max_image_width-max_kernel_width/2; i++)
            {
                // mean
                float mean = box_integral(rgb_ii+c*max_pixel_num, max_image_width, max_image_height,
                                       i-kernel_width/2, i+kernel_width/2,
                                       j-kernel_height/2, j+kernel_height/2);
                mean /= kernel_size;

                // std
                float std= box_integral(rgb_ii_power+c*max_pixel_num, max_image_width, max_image_height,
                                       i-kernel_width/2, i+kernel_width/2,
                                       j-kernel_height/2, j+kernel_height/2);
                std = std/kernel_size - mean*mean;
                std = sqrtf(std);

                // constrast gain
                float cg = alpha*image_std/std;
                if (cg>max_cg) cg = max_cg;

                // Amplify the high-frequency part and superimpose it with the low-frequency part, then get the enhanced image.
                float dst_val = mean + cg * (rgb[c*max_pixel_num + j*max_image_width+i] - mean);
                if (dst_val > 255) dst_val = 255;
                if (dst_val < 0) dst_val = 0;
                QRgb temp = dst_image.pixel(i-max_kernel_width/2, j-max_kernel_height/2);
                int temp_r = qRed(temp);
                int temp_g = qGreen(temp);
                int temp_b = qBlue(temp);
                if (c==0) temp_r = dst_val;
                if (c==1) temp_g = dst_val;
                if (c==2) temp_b = dst_val;
                dst_image.setPixel(i-max_kernel_width/2, j-max_kernel_height/2, qRgb(temp_r, temp_g, temp_b));
            }
        }
    }
}

/*
*Summary: calculate the integral image，to improve computing efficiency
*
*Parameters:
*    float *image : input original image
*    int width : iamge width
*    int height : image height
*    float *integral_image : output integral image
*/

void calculate_integral_image(float *image, int width, int height, float *integral_image)
{
    int i, j;

    // first row only
    float rs = 0;
    for(j=0; j<width; j++)
    {
        rs += image[j];
        integral_image[j] = rs;
    }
    for(i=1; i<height; ++i)
    {
        rs = 0;
        for(j=0; j<width; ++j)
        {
            rs += image[i*width+j];
            integral_image[i*width+j] = rs + integral_image[(i-1)*width+j];
        }
    }
}

// calculate the integral image power
void calculate_integral_image_power(float *image, int width, int height, float *integral_image)
{
    int i, j;

    // first row only
    float rs = 0;
    for(j=0; j<width; j++)
    {
        rs += image[j]*image[j];
        integral_image[j] = rs;
    }
    for(i=1; i<height; ++i)
    {
        rs = 0;
        for(j=0; j<width; ++j)
        {
            rs += image[i*width+j]*image[i*width+j];
            integral_image[i*width+j] = rs + integral_image[(i-1)*width+j];
        }
    }
}

/*
*Summary:
*
*Parameters:
*    float *integral_image : input integral image
*    int width : image width
*    int height : image height
*    int c1 :
*    int c2 :
*    int r1 :
*    int r2 :
*/
__inline float box_integral(float *integral_image, int width, int height, int c1, int c2, int r1, int r2)
{
    float a, b, c, d;

    a = (c1-1<0 || r1-1<0)	? 0 : integral_image[(r1-1) * width + (c1-1)];
    b = r1-1<0				? 0 : integral_image[(r1-1) * width + c2];
    c = c1-1<0				? 0 : integral_image[r2 * width + (c1-1)];
    d = integral_image[r2 * width + c2];

    return a - b - c + d;
}



/*
*Summary: dilation operation implemented by minimal description algorithm
*Parameters:
*    QImage &src_image : input original image
*    QImage &dst_image : output dilated image
*Describtion:
*    (1) detect pixels corresponding to all positions of 1 in the structure element template in turn;
*    (2) take the maximum value max among them;
*    (3) set the pixel value at the position of the current structure element to max.
*/

void Dilation(QImage *src_image, QImage *dst_image)
{
    dst_image = new QImage(src_image->width(), src_image->height(), QImage::Format_ARGB32);
    int kernel[5][5] = {
        { 0,0,1,0,0 },
        { 0,1,1,1,0 },
        { 1,1,1,1,1 },
        { 0,1,1,1,0 },
        { 0,0,1,0,0 } };

    int sizeKernel = 5;
    QColor color;
    QColor Rcolor;

    for (int y = sizeKernel / 2; y < src_image->height() - sizeKernel / 2; y++)
    {
        for (int x = sizeKernel / 2; x < src_image->width() - sizeKernel / 2; x++)
        {
            int kr = 0;
            int kg = 0;
            int kb = 0;
            Rcolor = QColor(src_image->pixel(x, y));
            for (int j = -sizeKernel / 2; j <= sizeKernel / 2; j++)
            {
                for (int i = -sizeKernel / 2; i <= sizeKernel / 2; i++)
                {
                    color = QColor(src_image->pixel(x + i, y + j));
                    while (color.red() > kr && kernel[sizeKernel / 2 + i][sizeKernel / 2 + j])
                    {
                        kr = color.red();
                    }
                    while (color.green() > kg && kernel[sizeKernel / 2 + i][sizeKernel / 2 + j])
                    {
                        kg = color.green();
                    }
                    while (color.blue() > kb && kernel[sizeKernel / 2 + i][sizeKernel / 2 + j])
                    {
                        kb = color.blue();
                    }
                }
            }
            dst_image->setPixel(x, y, qRgb(kr, kg, kb));
        }
    }
}
