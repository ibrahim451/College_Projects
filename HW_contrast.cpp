#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_contrast:
//
// Apply contrast enhancement to I1. Output is in I2.
// Stretch intensity difference from reference value (128) by multiplying
// difference by "contrast" and adding it back to 128. Shift result by
// adding "brightness" value.
//
void
HW_contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);
	int w = I1->width();
	int h = I1->height();
	int total = w * h;

	// init lookup table and variables
	int i, fin_val,lut[MXGRAY];

	double ref_val;
	
	int mid_val = MXGRAY / 2;

	// Contrast Stretching Formula
	for (i = 0; i < MXGRAY; i++)
	{
		// the contrast/brightness formula
		ref_val = ((i - mid_val) * contrast) + mid_val + brightness;
		//round the reference value
		fin_val = ROUND(ref_val);
		// value cannot execeed the range of [0,255] so the arithmetic operation is clamped
	
		/*if (fin_val > MXGRAY)
			lut[i] = 255;
		if (fin_val < 0)
			lut[i] = 0;*/
		lut[i] = CLIP(fin_val, 0, MXGRAY);
	}
	// declarations for image channel pointers and datatype
	ChannelPtr<uchar> p1, p2;
	int type;
	// visit all image channels and evaluate output image
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {    // get input  pointer for channel ch
		IP_getChannel(I2, ch, p2, type);        // get output pointer for channel ch
		for (i = 0; i < total; i++) *p2++ = lut[*p1++];    // use lut[] to eval output
	}
}
