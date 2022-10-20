//============================================================================
// Name        : lesson00.cpp
// Author      : Sven Gothel
// Version     : 0.1
// Copyright   : MIT
// Description : C++ Lesson 01
//============================================================================

#include <cstdio>
#include <cmath>
#include <string>

/**
 * Lesson 01
 *
 * Application of C++ functions rendering geometric Objects using ASCII-Art
 */

void print_space(const int n) {
    for(int i = 0; i < n; ++i) {
        printf(" "); // space
    }
}

void print_mark(const int n, const char c='X') {
    for(int i = 0; i < n; ++i) {
        printf("%c", c);
    }
}

void print_newline() {
    printf("\n"); // newline or line-feed (lf)
}

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
    for(int i=base_len; i > 0; i-=2) {
        print_space( dx + ( base_len - i ) / 2 );
        print_mark( i );
        print_newline();
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
    int i = 2 - ( base_len % 2 );
    for(; i <= base_len; i+=2) {
        print_space( dx + ( base_len - i ) / 2 );
        print_mark( i );
        print_newline();
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
    pyramid_up(base_len, dx, false);
    pyramid_down(base_len-2, dx+1, false);
}

/**
 * Disk rendering.
 *
 * Implementation uses a floating point centroid, or barycenter.
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
    const float r_sq = radius*radius; // square of disk radius
    const float disk_p0_x = radius - 0.5; // disk center point p0, x-component
    const float disk_p0_y = radius - 0.5; // disk center point p0, y-component
    const int aabbox_h = (int)std::floor(2 * radius); // disk AABBox height
    const int aabbox_w = (int)std::floor(2 * radius * sx); // disk AABBox width
    const float sx_r = aabbox_w / ( 2 * radius );

    for(int y=0; y<aabbox_h; ++y) {
        print_space(dx);
        for(int x=0; x<aabbox_w; ++x) {
            const float a = disk_p0_x - x/sx_r;
            const float b = disk_p0_y - y;
            const float dxy_p0_sq = a*a + b*b;
            if( dxy_p0_sq <= r_sq ) {
                print_mark( 1 );
            } else {
                print_space( 1 );
            }
        }
        print_newline();
    }
}

static std::string brightness("#=+-.");

/**
 * Return a char, representing given brightness.
 * @param b brightness ranging from [0..1]
 */
char get_char(const float b) {
    const size_t max_idx = brightness.size()-1;
    const float b_n = std::max<float>( 0.0, std::min<float>(1.0 , b) ); // normalize [0..1]
    return brightness[ std::min<size_t>(max_idx, std::round( b_n * max_idx ) ) ];
}

/**
 * Antialiased (AA) disk rendering using ASCII-Art alike brightness characters.
 *
 * Implementation uses a floating point centroid, or barycenter.
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
    const float disk_p0_x = radius - 0.5; // disk center point p0, x-component
    const float disk_p0_y = radius - 0.5; // disk center point p0, y-component
    const int aabbox_h = (int)std::floor(2 * radius); // disk AABBox height
    const int aabbox_w = (int)std::floor(2 * radius * sx); // disk AABBox width
    const float sx_r = aabbox_w / ( 2 * radius );

    for(int y=0; y<aabbox_h; ++y) {
        print_space(dx);
        for(int x=0; x<aabbox_w; ++x) {
            const float a = disk_p0_x - x/sx_r;
            const float b = disk_p0_y - y;
            const float dxy_p0 = std::sqrt( a*a + b*b );
            if( dxy_p0 <= radius - aa_seam ) {
                print_mark( 1, get_char( 0 ) );
            } else if( dxy_p0 <= radius ) {
                // dxy_p0 = ] ( r - aa_seam ) .. r ]
                const float d_r = ( radius - dxy_p0 ) / aa_seam ; // d_r ~ 1/brightness
                print_mark( 1, get_char( 1.0 - d_r ) );
            } else {
                print_space( 1 );
            }
        }
        print_newline();
    }
}

template<int max_diameter, int sx>
void disk_2b(const int r) {
    disk_2(r/2, max_diameter-r+1, sx);
}

typedef void(*paint_func)(const int p);

void paint(const int p, paint_func pf) {
    pf(p);
}

int main(int argc, const char* argv[]) {
    {
        // loop through all program invocation arguments and print them
        for(int i=0; i<argc; ++i) {
            printf("cmd_arg[%d]: %s\n", i, argv[i]);
        }
    }
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

    {
        const int sx = 2;
        const int min_size =  3;
        const int max_size = 10;
        printf("\nAlternate object with size [%d..%d]\n", min_size, max_size);

        paint_func p1 = [](const int p) { pyramid_up(p*sx, max_size-p); };
        paint_func paint_funcs[] = { p1,
                                     [](const int p) { pyramid_down(p*sx, max_size-p); },
                                     disk_2b<max_size, sx> };

        for(int i=min_size; i<=max_size; i++) {
            for(size_t t=0; t<sizeof(paint_funcs)/sizeof(paint_func); ++t) {
                paint(i, paint_funcs[t]);
            }
        }
    }

    return 0;
}
