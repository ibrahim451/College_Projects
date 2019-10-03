#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_quantize:
//
// Quantize I1 to specified number of levels. Apply dither if flag is set.
// Output is in I2.
//
void
HW_quantize(ImagePtr I1, int levels, bool dither, ImagePtr I2)
{
	// copy image header (width, height) of input image I1 to output image I2
	IP_copyImageHeader(I1, I2);

	// init vars for width, height, and total number of pixels
	int w = I1->width();
	int h = I1->height();
	int total = w * h;

	// init lookup table
	int i, lut[MXGRAY];
	double scale = MXGRAY / levels;
	double bias = 128 / levels;
	for (int i = 0; i < MXGRAY; i++) {
		lut[i] = scale * (int)(i / scale) + bias;
	}

	//declaration for image channel pointers and datatype
	ChannelPtr<uchar> p1, p2;
	int type;
	if (!dither) {
		//visit all image channles and evaluate output image using no dithering before quantization
		for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) { // get input pointer for channel ch
			IP_getChannel(I2, ch, p2, type);                      // get output pointer for channel ch 
			for (int i = 0; i < total; i++) *p2++ = lut[*p1++];
		}
	}
	else {
		int j, k, s;
		//visit all image channels and evaluate output image using dithering (jitter) before quantization
		for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) { // get input pointer for channel ch
			IP_getChannel(I2, ch, p2, type);                      // get output pointer for channel ch 

			//visit all rows
			for (int y = 0; y < h; y++) {
				//first sign value alternates in each row
				s = (y % 2) ? 1 : -1;

				//proceaa all pixels in a row, alternating sign value
				for (int x = 0; x < w; x++) {
					// jitter is in [0,bias] range
					j = ((double)rand() / RAND_MAX) * bias;

					// add signed jitter value
					k = *p1++ + j * s;

					//alternate sign for next pixel
					s *= -1;

					//eval output using jittered value 
					*p2++ = lut[CLIP(k, 0, MXGRAY)];
				}
			}
		}



	}
}
