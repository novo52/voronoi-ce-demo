// Ti 84 Plus CE
#include <tice.h>
#include <graphx.h>
#include <debug.h>

// C
#include <stdbool.h>
#include <math.h>

#define seedPointCount 16 // Number of seed points in seedPoints[]

#define width 320
#define height 240
#define widthPow2 9 // The nearest power of two, rounding up
#define heightPow2 8

#define maxDist 512 // any two points on the screen are guaranteed closer than this: maxDist > sqrt(width*2 + height*2)

typedef struct {
    uint24_t x;
    uint24_t y;
    uint8_t index; // Palette color
} seedPoint;

// The distance metric to use to render the voronoi graph
// return abs(dx) + abs(dy); < - Manhattan distance (https://en.wikipedia.org/wiki/Taxicab_geometry)
// return dx^2 + dy^2; <- euclidean (normal) distance (sqrt is not necessary because the function is only for comparison)
// Be weary of bugs caused by use of unsigned ints in distance functions!
uint24_t distanceFunction(uint24_t x1, uint24_t y1, uint24_t x2, uint24_t y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;

    return abs(dx) + abs(dy);
}

// Returns the color index of the closest seed point to a pixel
uint8_t calculateIndex(seedPoint seedPoints[], uint24_t x, uint24_t y) {

    // Simple find min algorithm O(N)
    seedPoint *closestPoint;
    uint24_t closestDist = maxDist * maxDist;

    for (int i = 0; i < seedPointCount; i++) {
        seedPoint *currentPoint = &seedPoints[i];
        uint24_t currentDist = distanceFunction(x, y, currentPoint->x, currentPoint->y);

        if (currentDist < closestDist) {
            closestPoint = currentPoint;
            closestDist = currentDist;
        }
    }

    return closestPoint->index;
}

// Renders the graph in variable resolution
// Draws in 2^step size chunks
void renderChunks(int step, seedPoint seedPoints[], bool partial) {

    int fillSize = 1 << (step - 1);
    bool skipY = true;
    bool skipX = true;

    for (int y = 0; y < height; y += fillSize, skipY = !skipY) {
        for (int x = 0; x < width; x += fillSize, skipX = !skipX) {
            if (partial && skipX && skipY) continue;
            uint8_t index = calculateIndex(seedPoints, x, y);
            gfx_SetColor(index);
            gfx_FillRectangle(x, y, fillSize, fillSize);
        }
    }
}

// Draws squares over the seed points to visualize them
void drawSeedPoints(seedPoint seedPoints[]) {
    for (int i = 0; i < seedPointCount; i++) {
        seedPoint *seedPoint = &seedPoints[i];

        const int seedPointDisplaySize = 10; // Size of seed point dot
        gfx_SetColor(0);
        gfx_FillRectangle(seedPoint->x - (seedPointDisplaySize >> 1), seedPoint->y - (seedPointDisplaySize >> 1),
                          seedPointDisplaySize, seedPointDisplaySize);
    }
}


int main() {
    gfx_Begin();
    gfx_SetDrawBuffer(); // Draw to the buffer to avoid rendering artifacts
    gfx_SetDefaultPalette(gfx_8bpp);

    // Hardcoded demo points
    seedPoint seedPoints[seedPointCount] = {
            {101, 208, 163},
            {60,  38,  57},
            {253, 150, 11},
            {34,  211, 150},
            {65,  142, 79},
            {306, 63,  210},
            {157, 54,  120},
            {18,  115, 24},
            {199, 76,  17},
            {81,  237, 55},
            {292, 128, 200},
            {283, 49,  178},
            {34,  20,  48},
            {215, 201, 98},
            {56,  76,  36},
            {107, 113, 219},
    };

    int step = heightPow2;
    bool renderPartial = false; // Render partially on every frame except the first

    while (!os_GetCSC()) { // Exit on any button pressed
        gfx_BlitScreen(); // Copy previous frame as a base for this frame

        if (step > 1) {
            renderChunks(step, seedPoints, renderPartial); // Rasterize and draw at current resolution
            step--; // Increase resolution
            renderPartial = true;
        }

        drawSeedPoints(seedPoints); // Draw black dots over the seed points

        gfx_SwapDraw(); // Queue the buffered frame to be displayed
    }

    gfx_End(); // Return to normal calculator operation
}




