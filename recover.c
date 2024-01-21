#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover ./***.raw");
        return 1;
    }
    char *file = argv[1];
    FILE *input = fopen(file, "r");

    uint8_t buffer[512];
    char filename[8];
    FILE *output = NULL;
    int fileCount = 0;
    char namings[999][8];

    // While there's still data left to read from the memory card
    while (fread(&buffer, 1, 512, input) == 512)
    {
        // Create JPEGs from the data
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close the previous file
            if (output != NULL)
            {
                fclose(output);
            }

            // Open a new file
            sprintf(filename, "%03d.jpg", fileCount++);
            output = fopen(filename, "w");
        }

        // If a JPEG is already open, write to it
        if (output != NULL)
        {
            fwrite(buffer, 1, 512, output);
        }
    }

    if (output != NULL)
    {
        fclose(output);
    }

    fclose(input);
}
