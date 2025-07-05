#include <stdio.h>
#include <stdlib.h>

void rleCompress(FILE *inputFile, FILE *outputFile) {
    int ch, prevCh = EOF;
    int count = 0;

    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch == prevCh) {
            count++;
            if (count == 255) {
                fputc(prevCh, outputFile);
                fputc(count, outputFile);
                printf("Compressing char '%c' with count %d\n", prevCh, count);
                count = 0;
            }
        } else {
            if (prevCh != EOF) {
                fputc(prevCh, outputFile);
                fputc(count, outputFile);
                printf("Compressing char '%c' with count %d\n", prevCh, count);
            }
            prevCh = ch;
            count = 1;
        }
    }
    if (prevCh != EOF) {
        fputc(prevCh, outputFile);
        fputc(count, outputFile);
        printf("Compressing char '%c' with count %d\n", prevCh, count);
    }
}

void rleDecompress(FILE *inputFile, FILE *outputFile) {
    int ch, count;

    while ((ch = fgetc(inputFile)) != EOF) {
        count = fgetc(inputFile);
        if (count == EOF) {
            fprintf(stderr, "Error: Corrupted compressed file.\n");
            exit(1);
        }
        printf("Decompressing char '%c' with count %d\n", ch, count);
        for (int i = 0; i < count; i++) {
            fputc(ch, outputFile);
        }
    }
}

int main() {
    char inputFilename[256], compressedFilename[256], decompressedFilename[256];

    printf("Enter input text filename to compress: ");
    scanf("%s", inputFilename);

    printf("Enter filename to save compressed data: ");
    scanf("%s", compressedFilename);

    printf("Enter filename to save decompressed data: ");
    scanf("%s", decompressedFilename);

    FILE *inputFile = fopen(inputFilename, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        return 1;
    }

    FILE *compressedFile = fopen(compressedFilename, "wb");
    if (!compressedFile) {
        perror("Error opening compressed file");
        fclose(inputFile);
        return 1;
    }

    rleCompress(inputFile, compressedFile);

    fclose(inputFile);
    fclose(compressedFile);

    printf("Compression complete.\n");

    FILE *compressedFileRead = fopen(compressedFilename, "rb");
    if (!compressedFileRead) {
        perror("Error opening compressed file for reading");
        return 1;
    }

    FILE *decompressedFile = fopen(decompressedFilename, "wb");
    if (!decompressedFile) {
        perror("Error opening decompressed file");
        fclose(compressedFileRead);
        return 1;
    }

    rleDecompress(compressedFileRead, decompressedFile);

    fclose(compressedFileRead);
    fclose(decompressedFile);

    printf("Decompression complete.\n");

    return 0;
}
