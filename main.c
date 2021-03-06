#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

struct timeval start, arrayCreated, arrayRead, arraySorted, arrayWritten;
struct timezone tz;

const int million = 1000000;

int makeMillionIntegerFile(char *filepath) {
  FILE *outfile = fopen(filepath, "w");
  if (outfile == NULL) {
    printf("error opening file at '%s': %d\n", filepath, errno);
    return 1;
  }
  srand(time(NULL));

  int64_t millionInts[million];
  for (int i=0; i < million; i++) {
    millionInts[i] = rand();
  }
  gettimeofday(&arrayCreated, NULL);

  int numwritten = fwrite(millionInts, sizeof millionInts[0], million, outfile);
  gettimeofday(&arrayWritten, NULL);
  if (numwritten == million) {
    return 0;
  } else {
    // does ferror indicate EOF? or will i need to try feof()?
    printf("error writing ints, only wrote %d ints before error: %d\n", numwritten, ferror(outfile));
    return 2;
  }
}

int compare_ints(const void* a, const void* b) {
  int first = *(const int*)a;
  int second = *(const int*)b;

  if (first < second) {
    return -1;
  } else if (second < first) {
    return 1;
  } else {
    return 0;
  }
}

int sortMillionIntegerFile(char *filepath) {
  int64_t millionInts[million];
  FILE *infile = fopen(filepath, "r");
  if (infile == NULL) {
    printf("error opening file at '%s': %d\n", filepath, errno);
    return 1;
  }
  fread(millionInts, sizeof millionInts[0], million, infile);
  gettimeofday(&arrayRead, NULL);

  qsort(millionInts, sizeof millionInts / sizeof millionInts[0], sizeof millionInts[0], compare_ints);
  gettimeofday(&arraySorted, NULL);

  char outfilepath[strlen(filepath)+8+1];
  snprintf(outfilepath, strlen(filepath)+8, "%s_sorted", filepath);
  FILE *outfile = fopen(outfilepath, "w");
  int numwritten = fwrite(millionInts, sizeof millionInts[0], million, outfile);
  gettimeofday(&arrayWritten, NULL);
  if (numwritten == million) {
    return 0;
  } else {
    // does ferror indicate EOF? or will i need to try feof()?
    printf("error writing ints, only wrote %d ints before error: %d\n", numwritten, ferror(outfile));
    return 2;
  }

  return 0;
}

void printTimePrefixed(char *prefix, struct timeval *tv) {
  char buff[100];
  strftime(buff, sizeof buff, "%Y-%m-%d %T", gmtime(&tv->tv_sec));
  printf("%s: %s.%06d\n", prefix, buff, tv->tv_usec);
}

void printTimePrefixedAndDelta(char *prefix, struct timeval *tv, struct timeval *prevTv) {
  char buff[100];
  strftime(buff, sizeof buff, "%Y-%m-%d %T", gmtime(&tv->tv_sec));
  printf("%s: %s.%06d ", prefix, buff, tv->tv_usec);

  int64_t delta = ((int64_t)tv->tv_sec * 1000000 + (int64_t)tv->tv_usec) - ((int64_t)prevTv->tv_sec * 1000000 + (int64_t)prevTv->tv_usec);
  printf("delta: %6lldµs\n", delta);
}

const char *create = "create", *sort = "sort";

int main(int argc, char **argv) {
  gettimeofday(&start, &tz);

  int ret;
  if (argc == 3 && strncmp(argv[1], create, strlen(create)) == 0) {
    ret = makeMillionIntegerFile(argv[2]);
    printTimePrefixed("started", &start);
    printTimePrefixedAndDelta("created", &arrayCreated, &start);
    printTimePrefixedAndDelta("written", &arrayWritten, &arrayCreated);
    return ret;
  } else if (argc == 3 && strncmp(argv[1], sort, strlen(sort)) == 0) {
    ret = sortMillionIntegerFile(argv[2]);
    printTimePrefixed("started", &start);
    printTimePrefixedAndDelta("   read", &arrayRead, &start);
    printTimePrefixedAndDelta(" sorted", &arraySorted, &arrayRead);
    printTimePrefixedAndDelta("written", &arrayWritten, &arraySorted);
    return ret;
  } else {
    printf("usage: %s {create|sort} path/to/million/int/file\n", argv[0]);
    return 1;
  }
}
