#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_gammaCorrect:
//
// Gamma correct image I1. Output is in I2.
//
void
HW_gammaCorrect(ImagePtr I1, double gamma, ImagePtr I2)
{
	// copy image header (width, height) of input image I1 to output image I2
	IP_copyImageHeader(I1, I2);

	// init vars for width, height, and total number of pixels
	int w = I1->width();
	int h = I1->height();
	int total = w * h;

	// init lookup table
	int i, lut[MXGRAY];
	double exponent = 1 / gamma;
	double correctd_value;
	for (i = 0; i < MXGRAY; ++i)
	{
		/* Put current intensity value in range [0, 1], then raise value to
	the exponent, then scale the value to [0, 255]. */
		correctd_value = pow((double)i / MXGRAY, exponent) * MXGRAY;  
		lut[i] = correctd_value;
	}
	//declaration for image channel pointers and datatype
	ChannelPtr<uchar> p1, p2;
	int type;
	//visit all image channels and evaluate output image using dithering (jitter) before quantization
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) { // get input pointer for channel ch
		IP_getChannel(I2, ch, p2, type);                      // get output pointer for channel ch 
		for (i = 0; i < total; i++) *p2++ = lut[*p1++];	// use lut[] to eval output
	}
}
