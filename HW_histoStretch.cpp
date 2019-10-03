#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_histoStretch:
//
// Apply histogram stretching to I1. Output is in I2.
// Stretch intensity values between t1 and t2 to fill the range [0,255].
//
void
HW_histoStretch(ImagePtr I1, int t1, int t2, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);
	int w = I1->width();
	int h = I1->height();
	int total = w * h;

	// Compute constants for histo stretch formula
	int newRange = t2 - t1;
	double scale = (double)MaxGray / newRange;

	int type, diff;
	ChannelPtr<uchar> p1, p2, endd;

	// for each channel
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type);

		// Evaluate output: apply histo stretch formula to each input pixel
		for (endd = p1 + total; p1 < endd;) {
			// Compute and clip the diff (input intensity - min intensity)
			diff = *p1++ - t1;
			diff = CLIP(diff, 0, newRange);

			// Scale and store diff
			*p2++ = scale * diff;
		}
	}
}
