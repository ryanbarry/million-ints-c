#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

struct timespec start, arrayCreated, arrayRead, arraySorted, arrayWritten;

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
  timespec_get(&arrayCreated, TIME_UTC);

  int numwritten = fwrite(&millionInts, sizeof millionInts[0], million, outfile);
  timespec_get(&arrayWritten, TIME_UTC);
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
  timespec_get(&arrayRead, TIME_UTC);
}

void printTimespecPrefixed(char *prefix, struct timespec *ts) {
  char buff[100];
  strftime(buff, sizeof buff, "%Y-%m-%d %T", gmtime(&ts->tv_sec));
  printf("%s: %s.%09ld UTC", prefix, buff, ts->tv_nsec);
}

const char *create = "create", *sort = "sort";

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("usage: %s {create|sort} path/to/million/int/file\n", argv[0]);
  }

  timespec_get(&start, TIME_UTC);

  int ret;
  if (strncmp(argv[1], create, strlen(create))) {
    ret = makeMillionIntegerFile(argv[1]);
    printTimespecPrefixed("started", &start);
    printTimespecPrefixed("created", &arrayCreated);
    printTimespecPrefixed("written", &arrayWritten);
    return ret;
  } else {
    ret = sortMillionIntegerFile(argv[1]);
    printTimespecPrefixed("started", &start);
    printTimespecPrefixed("   read", &arrayRead);
    printTimespecPrefixed(" sorted", &arraySorted);
    printTimespecPrefixed("written", &arrayWritten);
    return ret;
  }
}
