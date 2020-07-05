#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image");
        return 1;
    }
    // file name
    char *raw_file = argv[1];

    // char array to store the resultant string
    char store[8];

    // open file
    FILE *inptr = fopen(raw_file, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", raw_file);
        return 2;
    }
    // read into memory card

    unsigned char buffer[512];

    // counter
    int track = 0;

    FILE *img = 0;

    while (fread(buffer, 512, 1, inptr) == 1)
    {
        // check for jpeg
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL)
            {
                fclose(img);
            }
            // create file name for new JPEG
            sprintf(store, "%03i.jpg", track);
            // open JPEG file
            img = fopen(store, "w");
            // write into file
            fwrite(buffer, 512, 1, img);
            // keep track
            track ++;
        }
        else
        {
            if (track > 0)
            {
                fwrite(buffer, 512, 1, img);
            }

        }
    }
    // close and return 0 for success
    fclose(img);

    return 0;
}
