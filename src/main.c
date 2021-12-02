#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int32_t totaltriangles = 0;

void readstl_numtris(const char* filename)
{
    FILE* f = fopen(filename, "rb");

    if(f)
    {
        char header[80] = {"solid"};
        fread(header, 80, 1, f);

        if(strncmp(header, "solid", 5) == 0)
        {
            printf("ascii file %s\n", filename);
        }
        else
        {
            int32_t numtris = 0;
            fread(&numtris, 4, 1, f);
            totaltriangles += numtris;
        }

        fclose(f);
    }
    else
    {
        fprintf(stderr, "file %s not found\n", filename);
    }
}


void mergestl(const char* filename, FILE* of)
{
    FILE* f = fopen(filename, "rb");

    if(f)
    {
        char header[80] = {"solid"};
        fread(header, 80, 1, f);

        if(strncmp(header, "solid", 5) == 0)
        {
            printf("ascii file %s\n", filename);
        }
        else
        {
            int32_t numtris = 0;
            fread(&numtris, 4, 1, f);
            char* tris = malloc(50 * numtris);

            if(tris)
            {
                fread(tris, 50 * numtris, 1, f);
                fwrite(tris, 50 * numtris, 1, of);
                free(tris);
            }
        }

        fclose(f);
    }
    else
    {
        fprintf(stderr, "file %s not found\n", filename);
    }
}

int main(int argc, char* argv[])
{
    int c;
    int numinputs = 0;

    while((c = getopt(argc, argv, "i:o:")) != -1)
    {
        switch(c)
        {
            case 'i':
            {
                numinputs++;
                break;
            }

            default:
                break;
        }
    }

    optind = 1;
    char** inputfiles = malloc(sizeof(char*)*numinputs);
    char* outputfile = 0;
    numinputs = 0; // use as counter when allocating the filenames next

    while((c = getopt(argc, argv, "i:o:")) != -1)
    {
        switch(c)
        {
            case 'i':
            {
                inputfiles[numinputs] = malloc(strlen(optarg) + 1);
                strcpy(inputfiles[numinputs], optarg);
                numinputs++;
                break;
            }

            case 'o':
            {
                outputfile = malloc(strlen(optarg) + 1);
                strcpy(outputfile, optarg);
                break;
            }

            default:
                break;
        }
    }


    if(inputfiles != 0)
    {
        for(int i = 0; i  < numinputs; i ++)
        {
            readstl_numtris(inputfiles[i]);
        }

        FILE* of = fopen(outputfile, "wb");

        if(of)
        {
            char header[80] = "writen by mergestl";
            fwrite(header, 80, 1, of);
            fwrite(&totaltriangles, 4, 1, of);

            for(int i = 0; i  < numinputs; i ++)
            {
                mergestl(inputfiles[i], of);
            }

            fclose(of);
        }

        for(int i = 0; i  < numinputs; i ++)
        {
            if(inputfiles[i] != 0)
            {
                free(inputfiles[i]);
            }
        }
    }

    if(outputfile)
    {
        free(outputfile);
    }

    return 0;
}
