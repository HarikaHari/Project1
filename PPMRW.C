#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>


char const *imageType;

typedef struct {
     unsigned char red,green,blue;
} pixel;

typedef struct {
     int x, y;
	 pixel *data;
} image;

#define RGB_COMPONENT_COLOR 255

static image *readImage(const char *filePath)
{
	 char buff[16];
	 image *img;
	 FILE *fp;
	 int c, rgb_comp_color;
	 //open file for reading usong fopen
	 fp = fopen(filePath, "rb");
	 if (!fp || fp == NULL) {
	      fprintf(stderr, "Error: in opening the file '%s'\n", filePath);
			getch();
	 }
	 //reading the format of image
	 if (!fgets(buff, sizeof(buff), fp)) {
	       fprintf(stderr, "Error: in reading the image format '%s'\n", filePath);
			getch();
	 }

    //image format check
	if (buff[0] != 'P' || buff[1] != '6') {
	 if(buff[0] != 'P' || buff[1] != '3') {
		if(buff[0] != 'p' || buff[1] != '7') {
			fprintf(stderr, "Error: Invalid image format (must be 'P6')\n");
			getch();
		}
		else {
		imageType = "7";
		}
	}
		else {
		imageType = "3";
		}
    }
	else {
	imageType = "6";
	}

    //alloc memory form image
    img = (image *)malloc(sizeof(image));
    if (!img) {
	 fprintf(stderr, "Error: Unable to allocate memory\n");
	getch();
    }
	//check for comments if comments exist remove comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
	 c = getc(fp);
    }

    ungetc(c, fp);
    //image size
    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
	 fprintf(stderr, "Error: Invalid image size (error loading '%s')\n", filePath);
	 exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
	 fprintf(stderr, "Error: Invalid rgb component (error loading '%s')\n", filePath);
	 exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
	 fprintf(stderr, "'%s' Error : does not have 8-bits components\n", filePath);
	 exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (pixel*)malloc(img->x * img->y * sizeof(pixel));

    if (!img) {
	 fprintf(stderr, "Error: Unable to allocate memory\n");
	 exit(1);
    }

    //read pixel data from file
    if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
	 fprintf(stderr, "Error: loading image '%s'\n", filePath);
	 exit(1);
    }

    fclose(fp);
    return img;

}
 void writeImage(const char *filePath, image *img, char imageTypeToConvert)
	{
	
    FILE *fp;
     fp = fopen(filePath, "wb+");
     if (!fp) {
	 fprintf(stderr, "Error: Unable to open file '%s'\n", filePath);

    }
	if( imageTypeToConvert == '6' ) {
    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //comments
    fprintf(fp, "# Created by %s\n","Harika");

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
	fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
	}

	if( imageTypeToConvert == '3') {
		
    //write the header file
    //image format
    fprintf(fp, "P3\n");

    //comments
    fprintf(fp, "# Created by %s\n","Harika");

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
	fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
	}

	if( imageTypeToConvert == '7' ) {
    //write the header file
    //image format
    fprintf(fp, "P7\n");

    //image size
    fprintf(fp, "%s %d \n","WIDTH",img->x);
	fprintf(fp, "%s %d \n","HEIGHT",img->y);

    // rgb component depth
    fprintf(fp, "%s %d\n","DEPTH",RGB_COMPONENT_COLOR);

	//image maxval
	fprintf(fp, "%s %d\n","MAXVAL",RGB_COMPONENT_COLOR);


    // pixel data
	fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
	}

	}



int main(int argc, char *argv[]) {

	if( argc == 4 ) {

	image *image;
	image = readImage(argv[2]);
	printf("Current PPM file format is %s",imageType);
	printf("\n \n Converting to Image Format %s",argv[1]);	
	writeImage(argv[3],image, argv[1][0]);
    printf("\n File Conversion Successful - Press any key to exit...");	
    getch();
   }
   else if( argc > 4 ) {
      printf("Too many arguments supplied.\n");
   }
   else {
      printf("argument expected.\n");
   }

    return(0);
}