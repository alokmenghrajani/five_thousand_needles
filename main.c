/**
 * five thousand needles
 */


#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "rand.h"

char in_logo(float x, float y);
char _in_logo(float x, float y);
char in_rounded_square(float x, float y, float dist, float radius);
char in_circle(float x, float y, float cx, float cy, float r);
char in_rect(float x, float y, float left, float top, float width, float height);

int main(void) {
  if (init() != 0) {
    fprintf(stderr, "init() failed");
    return -1;
  }

  printf("<svg id=\"svg\" viewBox=\"-1 -1 2 2\" xmlns=\"http://www.w3.org/2000/svg\">\n");
  printf("  <rect x=\"-1\" y=\"-1\" width=\"2\" height=\"2\" stroke=\"#100f0d\" stroke-width=\"0.003\" fill-opacity=\"0\"/>\n");

  for (int i=0; i<5000; i++) {
    bool ok = false;
    float x1, y1, x2, y2;
    while (!ok) {
      x1 = get_rand() * 2.0 - 1.0;
      y1 = get_rand() * 2.0 - 1.0;
      if (in_logo(x1, y1)) {
        ok = get_rand() < 0.95;
      } else {
        ok = get_rand() < 0.05;
      }
    }

    ok = false;
    while (!ok) {
      float r = get_rand() * 2 * M_PI;
      float l = 20.0 / 517.0;
      x2 = x1 + cos(r) * l;
      y2 = y1 + sin(r) * l;
      ok = in_rect(x2, y2, -1, -1, 2, 2);
    }
    printf("  <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"#100f0d\" stroke-opacity=\"0.5\" stroke-width=\"%f\" stroke-linecap=\"round\"/>\n",
	   x1, y1, x2, y2, 2.0 / 517.0);
  }
  printf("</svg>");
  cleanup();
  return 0;
}

char in_logo(float x, float y) {
  return _in_logo(x, y) || _in_logo(-x, y) || _in_logo(x, -y) || _in_logo(-x, -y);
}

char _in_logo(float x, float y) {
  // small square
  if (in_rounded_square(x, y, 53.0/517.0, 15.0/517.0)) {
    return 1;
  }

  // medium square
  if (in_rounded_square(x, y, 153.0/517.0, 26.0/517.0)) {
    return 0;
  }

  // large square
  if (in_rounded_square(x, y, 166.0/517.0, 84.0/517.0)) {
    return 1;
  }

  // outside
  return 0;
}

char in_rounded_square(float x, float y, float dist, float radius) {
  if (in_circle(x, y, dist, dist, radius)) {
    return 1;
  }
  if (in_rect(x, y, 0, 0, dist, dist + radius)) {
    return 1;
  }
  if (in_rect(x, y, 0, 0, dist + radius, dist)) {
    return 1;
  }
  return 0;
}

char in_circle(float x, float y, float cx, float cy, float r) {
  return ((x-cx)*(x-cx) + (y-cy)*(y-cy)) < r*r;
}

char in_rect(float x, float y, float left, float top, float width, float height) {
  return (x >= left) && (x < left + width) && (y >= top) && (y < top + height);
}
