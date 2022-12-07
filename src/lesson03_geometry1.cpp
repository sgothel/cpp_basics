//============================================================================
// Author      : Sven Gothel, Qun Gothel, Svenson Gothel
// Copyright   : 2022 Gothel Software e.K.
// License     : MIT
// Description : C++ Lesson 0.3 Geometry 1 (Complex Version w/ x-axis scale factor)
//============================================================================

#include <cstdio>
#include <cmath>
#include <string>

/**
 * Lesson 0.3
 *
 * Implementing rendering of simple geometric Objects using ASCII-Art (w/ x-axis scale factor)
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
 * Round given float and return its integer representation.
 *
 * This function utilizes `constexpr`, i.e. might be evaluated at compile time
 * if the used parameters are of constexpr nature.
 *
 * @param v the float
 * @return the rounded integer representation
 */
constexpr int round_to_int(const float v) noexcept {
    return static_cast<int>( std::round( v ) );
}

/**
 * Paint a simple square, not-filled
 * @param len length of each side of the square
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 */
void square(const int len, const int dx=0, const float sx=1.0) {
    const int len_sx = round_to_int( len * sx );
    print_newline();
    for(int y=0; y<len; ++y) {
        print_space(dx);
        for(int x=0; x<len_sx; ++x) {
            if( 0 == y || len-1 == y || 0 == x || len_sx-1 == x ) {
                print_mark( 1 );
            } else {
                print_space( 1 );
            }
        }
        print_newline();
    }
}

/**
 * Pyramid w/ base_len == 4
 ```
 *    XXXX
 *     XX
 ```
 *
 * Pyramid w/ base_len == 5
 ```
 *    XXXXX
 *     XXX
 *      X
 ```
 *
 * @param base_len length of pyramid's base
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 * @param show_title
 */
void pyramid_down(const int base_len, const int dx=0, const float sx=1.0, const bool show_title=true) {
    if( show_title ) {
        printf("\nPyramid(down, l %d, dx %d, sx %.2f)\n", base_len, dx, sx);
    }
    const int base_len_sx = round_to_int( base_len * sx );
    const int step_sx = round_to_int( 2 * sx );
    for(int i=base_len_sx; i > 0; i-=step_sx) {
        print_space( dx + ( base_len_sx - i ) / 2 );
        print_mark( i );
        print_newline();
    }
}

/**
 * Pyramid w/ base_len == 4
 ```
 *     XX
 *    XXXX
 ```
 *
 * Pyramid w/ base_len == 5
 ```
 *      X
 *     XXX
 *    XXXXX
 ```
 *
 * @param base_len length of pyramid's base
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 * @param show_title
 */
void pyramid_up(const int base_len, const int dx=0, const float sx=1.0, const bool show_title=true) {
    if( show_title ) {
        printf("\nPyramid(up, l %d, dx %d, sx %.2f)\n", base_len, dx, sx);
    }
    const int base_len_sx = round_to_int( base_len * sx );
    const int step_sx = round_to_int( 2 * sx );
    int i = round_to_int( ( 2 - ( base_len % 2 ) ) * sx );
    for(; i <= base_len_sx; i+=step_sx) {
        print_space( dx + ( base_len_sx - i ) / 2 );
        print_mark( i );
        print_newline();
    }
}

/**
 * Salino w/ base_len == 4
 ```
 *     XX
 *    XXXX
 *     XX
 ```
 *
 * Salino w/ base_len == 5
 ```
 *      X
 *     XXX
 *    XXXXX
 *     XXX
 *      X
 ```
 *
 * @param base_len length of pyramid's base
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 * @param show_title
 */
void salino(const int base_len, const int dx=0, const float sx=1.0, const bool show_title=true) {
    if( show_title ) {
        printf("\nSalino(l %d, dx %d, sx %.2f)\n", base_len, dx, sx);
    }
    pyramid_up(base_len, dx, sx, false);
    pyramid_down(base_len-2, dx + round_to_int( 1 * sx ), sx, false);
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
 *
 * @param radius radius of disk
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 * @param show_title
 */
void disk_1(const int radius, const int dx=0, const float sx=1.0, const bool show_title=true) {
    if( show_title ) {
        printf("\nDisk1(r %d, dx %d, sx %.2f)\n", radius, dx, sx);
    }
    const float r_sq = radius*radius;     // square of disk radius
    const float disk_p0_x = radius - 0.5; // disk center point p0, x-component, a centroid
    const float disk_p0_y = radius - 0.5; // disk center point p0, y-component, a centroid
    const int aabbox_h = 2 * radius;      // disk AABBox height
    const int aabbox_w = round_to_int( 2 * radius * sx ); // disk AABBox width
    const float sx_r = aabbox_w / ( 2.0 * radius );

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

 * @param radius radius of disk
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 * @param aa_seam AA seam of pixels considerd for AA brightness adjusted rendering
 * @param show_title
 */
void disk_2(const int radius, const int dx=0, const float sx=1.0, const float aa_seam_=1.0, const bool show_title=true) {
    const float aa_seam = std::max<float>(1.0, aa_seam_);
    if( show_title ) {
        printf("\nDisk2(r %d, dx %d, sx %.2f, aa_seam %.2f)\n", radius, dx, sx, aa_seam);
    }
    const float disk_p0_x = radius - 0.5; // disk center point p0, x-component, a centroid
    const float disk_p0_y = radius - 0.5; // disk center point p0, y-component, a centroid
    const int aabbox_h = 2 * radius;      // disk AABBox height
    const int aabbox_w = round_to_int( 2 * radius * sx ); // disk AABBox width
    const float sx_r = aabbox_w / ( 2.0 * radius );

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

/**
 * disk_2 variant using diameter instead of radius
 *
 * @param d disc diameter size argument
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 */
void disk_2b(const int d, const int dx, const float sx) {
    disk_2(d/2, dx+1, sx);
}

/**
 * Function-pointer to draw an object with given given arguments `sz`, `dx` and `sx`.
 * @param sz object's size argument. Semantic depends on the actual object defintion.
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 */
typedef void(*paint_func)(const int sz, const int dx, const float sx);

/**
 * Paint something using the given paint_func
 * @param sz object's size argument. Semantic depends on the actual object defintion.
 * @param dx distance on x-axis, aka the x-position of object's left edge
 * @param sx scale factor on x-axis, only for object w/o dx
 * @param pf function pointer to draw an object with given arguments `sz`, `dx` and `sx`.
 */
void paint(const int sz, const int dx, const float sx, paint_func pf) {
    pf(sz, dx, sx);
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
        /**
         * This block paints all objects in all sizes using
         * an array of object agnostic `paint_func` function-pointer
         * to apply the paint functions for all object types.
         *
         * This allows polymorphic rendering of objects,
         * i.e. painting different objects by invoking instances
         * conforming to the common `paint_func` function-type (callable). <br />
         * These `paint_func` instances paint the different objects,
         * e.g. `pyramid_up`, `pyramid_down` etc.
         */
        const float sx = 2.0;
        const int min_size =  3;
        const int max_size = 10;
        printf("\nAlternate object with size [%d..%d]\n", min_size, max_size);

        /*
         * Pyramid_up non-capturing lambda function pointer.
         *
         * Assignment to a variable of type `paint_func` is only possible here,
         * since the lambda type is non-capturing.
         */
        paint_func p1 = [](const int sz, const int dx, const float sx) { pyramid_up(sz, dx, sx); };

        /* Array of paint_func function pointer */
        paint_func paint_funcs[] = {
                /* Assigning `square` function-pointer for index 0.*/
                square,

                /* Assigning pyramid_up non-capturing lambda function-pointer for index 1.*/
                p1,

                /* Create and assigning pyramid_down non-capturing lambda function-pointer for index 2. */
                [](const int sz, const int dx, const float sx) { pyramid_down(sz, dx, sx); },

                /* Assigning `disk_2b` function-pointer for index 3 */
                disk_2b
            };

        // for all sizes
        for(int i=min_size; i<=max_size; i++) {
            // for all object types
            for(size_t t=0; t<sizeof(paint_funcs)/sizeof(paint_func); ++t) {
                paint(i, max_size-i, sx, paint_funcs[t]);
            }
        }
    }

    return 0;
}
