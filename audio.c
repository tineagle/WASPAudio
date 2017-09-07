#include "audio.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct RIFF_header {
  char chunkID[4];
  uint32_t chunkSize;
  char format[4];
};

struct WAVE_format {
  char chunkID[4];
  uint32_t chunkSize;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  uint16_t extraParamSize;
  char* extraParams;
};

struct WAVE_data {
  char chunkID[4];
  uint32_t chunkSize;
  char* data;
};

struct WAVE_file {
  struct RIFF_header riff;
  struct WAVE_format format;
  struct WAVE_data data;
};

void readRIFFheader(struct RIFF_header* header, FILE* file) {
  fread(header->chunkID, sizeof(header->chunkID), 1, file);
  fread(&header->chunkSize, sizeof(header->chunkSize), 1, file);
  fread(header->format, sizeof(header->format), 1, file);
}

void readWAVEformat(struct WAVE_format* format, FILE* file) {
  fread(format->chunkID, sizeof(format->chunkID), 1, file);
  fread(&format->chunkSize, sizeof(format->chunkSize), 1, file);
  fread(&format->audioFormat, sizeof(format->audioFormat), 1, file);
  fread(&format->numChannels, sizeof(format->numChannels), 1, file);
  fread(&format->sampleRate, sizeof(format->sampleRate), 1, file);
  fread(&format->byteRate, sizeof(format->byteRate), 1, file);
  fread(&format->blockAlign, sizeof(format->blockAlign), 1, file);
  fread(&format->bitsPerSample, sizeof(format->bitsPerSample), 1, file);
  fread(&format->extraParamSize, sizeof(format->extraParamSize), 1, file);
  fread(format->extraParams, format->extraParamSize, 1, file);
}

void printRIFFheader(struct RIFF_header* header) {
  printf("RIFF Header\n");
  printf("Chunk ID  : %.4s\n", header->chunkID);
  printf("Chunk Size: %u\n", header->chunkSize);
  printf("Format    : %.4s\n", header->format);
  printf("\n");
}

void printWAVEformat(struct WAVE_format* format) {
  printf("WAVE Format\n");
  printf("Chunk ID       : %.4s\n", format->chunkID);
  printf("Chunk Size     : %u\n", format->chunkSize);
  printf("Audio Format   : %hu\n", format->audioFormat);
  printf("NumChannels    : %hu\n", format->numChannels);
  printf("Sample Rate    : %u\n", format->sampleRate);
  printf("Byte Rate      : %u\n", format->byteRate);
  printf("Block Align    :  %hu\n", format->blockAlign);
  printf("BitsPerSample  : %hu\n", format->bitsPerSample);
  printf("ExtraParamSize : %hu\n", format->extraParamSize);
  printf("\n");
}

void printWAVEdata(struct WAVE_data* data) {
  printf("WAVE Data\n");
  printf("Chunk ID   : %.4s\n", data->chunkID);
  printf("Chunk Size : %u\n", data->chunkSize);
  printf("\n");
}

void readWAVEdata(struct WAVE_data* data, FILE* file) {
  fread(data->chunkID, sizeof(data->chunkID), 1, file);
  fread(&data->chunkSize, sizeof(data->chunkSize), 1, file);
  data->data = malloc(data->chunkSize);
  fread(data->data, data->chunkSize, 1, file);
}

void openWav(struct WAVE_file* wave, char* fileName) {
  FILE* file = fopen(fileName, "rb");
  readRIFFheader(&wave->riff, file);
  readWAVEformat(&wave->format, file);
  readWAVEdata(&wave->data, file);
  fclose(file);
}

int main() {
  struct WAVE_file wave;
  openWav(&wave, "test.wav");
  printRIFFheader(&wave.riff);
  printWAVEformat(&wave.format);
  printWAVEdata(&wave.data);
  return 0;
}
