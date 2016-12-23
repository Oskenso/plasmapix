#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
//#include <png.h>
#include <string.h>

#include <math.h>

#define PI 3.141592653589793

/*
	Todo: output png data libpng
*/

//#include <png.h>

void showHelp() {
	puts("Usage: plasmapix width height time [density] [weight]");
	puts("Outputs image structure in JSON");
	puts("{width: int, height: int, data: Uint32Array}");
}

typedef struct {
	unsigned int height;
	unsigned int width;
	float time;
	float density;
	float weight;
} FrameOpt;

typedef struct {
	uint32_t *data;
	unsigned int size;
} PixelData;

PixelData pixplot(FrameOpt opt)
{
	PixelData frame = {NULL, 0};

	unsigned int w = opt.width;
	unsigned int h = opt.height;

	float time = opt.time;
	float density = opt.density;
	float weight = opt.weight;

	frame.size = w * h;
	frame.data = malloc(sizeof(uint32_t) * w * h);

	if (frame.data == NULL) {
		return frame;
	}

	float kx = w / h;
	// algo from: www.bidouille.org/prog/plasma

	for (float y = 0; y < h; y++) {
		float yy = (y / h) - 0.5;

		float cy = yy + (0.5 * cos(time / 3.0));
		float densityY = (yy * density);
		float cy2 = cy * cy;

		for (float x = 0; x < w; x++) {
			float xx = ((kx * x) / w) - (kx / 2);
			float cx = xx + (0.5 * sin(time / 5.0));

			/* plasma map*/
			float v = 0;
			v += sin((xx * density) + time);
			v += sin( (densityY + time) / 2);
			v += sin((((xx * density) + (densityY)) + time) / 2.0);

			v += sin(sqrt((100 * ((cx * cx) + (cy2))) + 1) + time);
			v += sin(sqrt(100*(cx*cx+cy2)+1)+time);
			v /=  weight;

			float piv = PI * v;
			/* color map */
			frame.data[(int) ((y * w) + x)] =

			/* original */
			//(int) (255 * (0.5 + (0.5 * sin(piv)))) << 24 |
			//(int) (255 * (0.5 + (0.5 * cos(piv)))) << 16 |
			//0 |;

			/*rainbow*/
			(int) (255 * (0.5 + (0.5 * sin(piv)))) << 24 |
			(int) (255 * (0.5 + (0.5 * sin((piv) + (2 * (PI / 3)))))) << 16 |
			(int) (255 * (0.5 + (0.5 * sin((piv) + (4 * (PI / 3)))))) << 8 |
			255;

		}
	}

	return frame;
}

FrameOpt parseStdin()
{
	FrameOpt opt = {0, 0, 0.0f, 5.0f, 2.0f};
	char stdinBuf[64] = {0};
	fgets(stdinBuf, sizeof(stdinBuf), stdin);
	if (stdinBuf[strlen(stdinBuf)-1] == '\n') {
		sscanf(stdinBuf, "%d %d %f %f %f", &(opt.width), &(opt.height),
				&(opt.time), &(opt.density), &(opt.weight));
	}
	return opt;
}

FrameOpt parseArgs(int argc, char* argv[])
{
	FrameOpt opts = {0, 0, 0.0f, 5.0f, 2.0f};

	if (argc <= 1)
		return opts;

	if (argc < 4)
		return opts;

	sscanf(argv[1], "%d", &(opts.width));
	sscanf(argv[2], "%d", &(opts.height));
	sscanf(argv[3], "%f", &(opts.time));
	if (argc >= 5)
		sscanf(argv[4], "%f", &(opts.density));

	if (argc >= 6)
		sscanf(argv[5], "%f", &(opts.weight));


	return opts;
}

int main(int argc, char* argv[])
{
	FrameOpt opt = parseArgs(argc, argv);
	if (isatty(0)) {
		//is a tty do nothing?
	} else {

		if ((opt.width == 0) || (opt.height == 0)) {
			opt = parseStdin();
		}
	}

	if ((opt.width == 0) || (opt.height == 0)) {
		showHelp();
		return EXIT_SUCCESS;
	}

	PixelData pixelData = pixplot(opt);

	printf("{\"width\":%d,\"height\":%d,\"data\":", opt.width, opt.height);
		printf("[");
		for (int i = 0; i < pixelData.size; i++) {
			//printf("\"%d\":%u", i, pixelData.data[i]);
			printf("%u", pixelData.data[i]);
			if (i < pixelData.size-1)
				printf(",");
		}
	printf("]}\n");

	free(pixelData.data);
	return 0;
}
