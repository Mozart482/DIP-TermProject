#include "thresholddialog.h"

ThresholdDialog::ThresholdDialog(QImage inputImage)
{
    srcImage = inputImage;

    Threshold_Otsu(srcImage, dstImage);

    iniUI();

    srcImageLabel->setPixmap(QPixmap::fromImage(srcImage));
    dstImageLabel->setPixmap(QPixmap::fromImage(dstImage));

}

void ThresholdDialog::iniUI()
{
    // two image labels
    srcImageLabel = new QLabel();
    srcImageLabel->setAlignment(Qt::AlignCenter);
    dstImageLabel = new QLabel();
    dstImageLabel->setAlignment(Qt::AlignCenter);

    // three buttons
    btnOK = new QPushButton(tr("OK"));
    btnCancel = new QPushButton(tr("Cancel"));
    btnClose = new QPushButton(tr("Exit"));

    connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(srcImageLabel);
    layout1->addWidget(dstImageLabel);

//    srcLabel = new QLabel(tr("Original"));
//    srcLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout3=new QHBoxLayout;
    layout3->addStretch();
    layout3->addWidget(btnOK);
    layout3->addWidget(btnCancel);
    layout3->addStretch();
    layout3->addWidget(btnClose);


    // main layout
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addLayout(layout1);
    mainlayout->addLayout(layout3);
    setLayout(mainlayout);
}

void ThresholdDialog::setImage(QImage image, QLabel *label)
{
    QPixmap pix;
    pix.fromImage(image);
    label->setPixmap(pix);
}
/*
*Summary: Threshold segmentation by otsu algorithm
*Parameters:
*    QImage &src_image : input original image
*    QImage &dst_image : output opened image
*Describtion:
*    intra-class variance / the variance within the class
*    by selecting a structural template, the filled content can have certain geometric characteristics
*/

void ThresholdDialog::Threshold_Otsu(const QImage &src_image, QImage &dst_image)
{
    dst_image = src_image;
    double hist[256];
    normalizedHistogram(src_image, hist);

    double omega[256];
    double mu[256];

    omega[0] = hist[0];
    mu[0] = 0;
    for(int i = 1; i < 256; i++)
    {
        omega[i] = omega[i-1] + hist[i]; // cdf
        mu[i] = mu[i-1] + i * hist[i];
    }
    double mean = mu[255]; // mean gray value
    double max = 0;
    int k_max = 0;
    for(int k = 1; k < 255; k++)
    {
        double PA = omega[k]; // proportion of category A
        double PB = 1 - omega[k]; // proportion of category B
        double value = 0;
        if( fabs(PA) > 0.001 && fabs(PB) > 0.001)
        {
            double MA = mu[k] / PA; // mean gray value of class A
            double MB = (mean - mu[k]) / PB; // mean gray value of class B
            value = PA * (MA - mean) * (MA - mean) + PB * (MB - mean) * (MB - mean); // the variance within the class

            if (value > max)
            {
                max = value;
                k_max = k;
            }
        }
    }

    int threshold = k_max;

    for(int i = 0; i < src_image.width(); i++)
    {
       for(int j = 0; j < src_image.height(); j++)
       {
         QRgb pixel = src_image.pixel(i, j);
         int r = qRed(pixel) * 0.3;
         int g = qGreen(pixel) * 0.59;
         int b = qBlue(pixel) * 0.11;
         int rgb = r + g + b; // convert the color image to grayscale image
         if (rgb > threshold) // binarize according to the calculated threshold
         {
           rgb = 255;
         }else
         {
           rgb = 0;
         }
         QRgb newPixel = qRgb(rgb, rgb, rgb);
         dst_image.setPixel(i, j, newPixel);
       }
    }

}

void ThresholdDialog::normalizedHistogram(const QImage &image, double hist[256])
{
    for(int i = 0; i < 256; i++)
    {
        hist[i] = 0.0;
    }
    int height = image.height();
    int width = image.width();
    int N = height * width;
//    if(image.format() != QImage::Format_Indexed8)
//    {
//        return false;
//    }
    for(int i = 0; i < height; i++)
    {
        const quint8 *pData = (const quint8 *)image.constScanLine(i);
        for(int j = 0; j < width; j++)
        {
            ++hist[ pData[j] ];
        }
    }
    for(int i = 0; i < 256; i++)
    {
        hist[i] = hist[i] / N;
    }
}
