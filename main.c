#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

void swap(int a[], int from, int to) {
  int tmp = a[to];
  a[to] = a[from];
  a[from] = tmp;
}

void quicksort(int thelist[], int start, int end) {
  // base case is array of 0 or 1 length
  if(end - start <= 0) {
    return;
  }

  int pivot = thelist[end]; // choose last element's value as pivot

  // partition array, moving elements less than pivot to the left half, and greater elements to the right
  int j = end;
  for(int i = start; i < j;) {
    if (thelist[i] > pivot) {
      j--; // move swap-to position down the array
      swap(thelist, i, j);
    } else {
      i++; // only move on if element is less-than-or-equal-to pivot
    }
  }

  // move pivot into place, if necessary
  if(thelist[j] > pivot) {
    swap(thelist, end, j);
  }

  //recurse for left and right "halves"
  quicksort(thelist, start, j-1);
  quicksort(thelist, j+1, end);
}

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

  int millionInts[million];
  for (int i=0; i < million; i++) {
    millionInts[i] = rand();
  }
  gettimeofday(&arrayCreated, NULL);

  int numwritten = fwrite(&millionInts, sizeof millionInts[0], million, outfile);
  gettimeofday(&arrayWritten, NULL);
  if (numwritten == million) {
    return 0;
  } else {
    // does ferror indicate EOF? or will i need to try feof()?
    printf("error writing ints, only wrote %d ints before error: %d\n", numwritten, ferror(outfile));
    return 2;
  }
}

int sortMillionIntegerFile(char *filepath) {
  int millionInts[million];
  FILE *infile = fopen(filepath, "w");
  if (infile == NULL) {
    printf("error opening file at '%s': %d\n", filepath, errno);
    return 1;
  }
  fread(&millionInts, sizeof millionInts[0], million, infile);
  gettimeofday(&arrayRead, NULL);
  return 0;
}

void printTimePrefixed(char *prefix, struct timeval *tv) {
  char buff[100];
  strftime(buff, sizeof buff, "%Y-%m-%d %T", gmtime(&tv->tv_sec));
  printf("%s: %s.%06d UTC\n", prefix, buff, tv->tv_usec);
}

const char *create = "create", *sort = "sort";

int main(int argc, char **argv) {
  gettimeofday(&start, &tz);

  int ret;
  if (argc == 3 && strncmp(argv[1], create, strlen(create)) == 0) {
    ret = makeMillionIntegerFile(argv[2]);
    printTimePrefixed("started", &start);
    printTimePrefixed("created", &arrayCreated);
    printTimePrefixed("written", &arrayWritten);
    return ret+1;
  } else if (argc == 3 && strncmp(argv[1], sort, strlen(sort)) == 0) {
    ret = sortMillionIntegerFile(argv[2]);
    printTimePrefixed("started", &start);
    printTimePrefixed("   read", &arrayRead);
    printTimePrefixed(" sorted", &arraySorted);
    printTimePrefixed("written", &arrayWritten);
    return ret+3;
  } else {
    printf("usage: %s {create|sort} path/to/million/int/file\n", argv[0]);
    return 1;
  }
}
