//============================================================================
// Name        : lesson00.cpp
// Author      : Sven Gothel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 00
//============================================================================

#include <cstdio>
#include <cmath>
#include <string>

/**
 * Lesson 01
 *
 * Application of C++ functions rendering geometric Objects using ASCII-Art
 */

/**
 * Pyramid w/ base_len == 4
 *    XXXX
 *     XX
 *
 * Pyramid w/ base_len == 5
 *    XXXXX
 *     XXX
 *      X
 */
void pyramid_down(const int base_len, const int dx=0, const bool show_title=true) {
    if( show_title ) {
        printf("\nPyramid(down, l %d, dx %d)\n", base_len, dx);
    }
}

/**
 * Pyramid w/ base_len == 4
 *     XX
 *    XXXX
 *
 * Pyramid w/ base_len == 5
 *      X
 *     XXX
 *    XXXXX
 */
void pyramid_up(const int base_len, const int dx=0, const bool show_title=true) {
    if( show_title ) {
        printf("\nPyramid(up, l %d, dx %d)\n", base_len, dx);
    }
}

/**
 * Salino w/ base_len == 4
 *     XX
 *    XXXX
 *     XX
 *
 * Salino w/ base_len == 5
 *      X
 *     XXX
 *    XXXXX
 *     XXX
 *      X
 */
void salino(const int base_len, const int dx=0, const bool show_title=true) {
    if( show_title ) {
        printf("\nSalino(l %d, dx %d)\n", base_len, dx);
    }
}

/**
 * Disk rendering.
 *
 * Disk1(r 1.00, dx 4, sx 1.00)
 ```
    XX
    XX
 ```
 * Disk1(r 2.00, dx 3, sx 1.00)
 ```
    XX
   XXXX
   XXXX
    XX
 ```

 * Disk1(r 3.00, dx 2, sx 1.00)
 ```
   XXXX
  XXXXXX
  XXXXXX
  XXXXXX
  XXXXXX
   XXXX
 ```

 * Disk1(r 3.00, dx 2, sx 2.00)
     ```
    XXXXXXX
  XXXXXXXXXXX
  XXXXXXXXXXX
  XXXXXXXXXXX
  XXXXXXXXXXX
    XXXXXXX
 ```
 */
void disk_1(const float radius, const int dx=0, const float sx=1.0, const bool show_title=true) {
    if( show_title ) {
        printf("\nDisk1(r %.2f, dx %d, sx %.2f)\n", radius, dx, sx);
    }
}

/**
 * Antialiased (AA) disk rendering using ASCII-Art alike brightness characters.
 *
 * Disk2(r 1.00, dx 4, sx 1.00, aa_seam 1.00)
 ```
    --
    --
 ```

 * Disk2(r 2.00, dx 3, sx 1.00, aa_seam 1.00)
 ```
    ++
   +##+
   +##+
    ++
 ```

 * Disk2(r 3.00, dx 2, sx 1.00, aa_seam 1.00)
 ```
   .++.
  .####.
  +####+
  +####+
  .####.
   .++.
 ```

 * Disk2(r 3.00, dx 2, sx 2.00, aa_seam 1.00)
 ```
    .-+++-.
  .+#######+.
  +#########+
  +#########+
  .+#######+.
    .-+++-.
 ```
 */
void disk_2(const float radius, const int dx=0, const float sx=1.0, const float aa_seam_=1.0, const bool show_title=true) {
    const float aa_seam = std::max<float>(1.0, aa_seam_);
    if( show_title ) {
        printf("\nDisk2(r %.2f, dx %d, sx %.2f, aa_seam %.2f)\n", radius, dx, sx, aa_seam);
    }
}

int main(int argc, const char* argv[]) {
    printf("\nPyramids base_len 4\n");
    pyramid_up(4);
    pyramid_down(4);
    salino(4);

    printf("\nPyramids base_len 5\n");
    pyramid_up(5);
    pyramid_down(5);
    salino(5);

    printf("\nDisks radius 1, scale-x 1\n");
    disk_1(1, 1);
    disk_1(2, 1);
    disk_2(1, 1);

    {
        const int max_size = 5;

        printf("\nDisks radius [1..5], sx 1.0\n");
        for(int i=1; i<=max_size; ++i) {
            disk_1(i, max_size-i, 1.0);
            disk_2(i, max_size-i, 1.0);
        }

        printf("\nDisks radius [1..5], sx 1.5\n");
        for(int i=1; i<=max_size; ++i) {
            disk_1(i, max_size-i, 2);
            disk_2(i, max_size-i, 2);
        }
    }

    return 0;
}
