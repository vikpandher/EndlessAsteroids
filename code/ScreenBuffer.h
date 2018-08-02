/* ==========================================================================
   >File: ScreenBuffer.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This object holds the pixels that will be displayed. The origin
             is the bottom left. It has a lot of functions to draw with.
   ========================================================================== */

#pragma once
#include <Windows.h>
#include "SizedIntegers.h"
#include "ColorUtilities.h"
#include "MathUtilities.h"
#include "GraphicsUtilities.h"
#include "MenuUtilities.h"
#include "GameUtilities.h"
#include "AsciiCharacterDefines.h"
#include "String.h"


// -------------------------------------------------------------------------- //
class ScreenBuffer
{
public:
    // ScreenBuffer
    // ====================================================================== //
    // Iniitialize the Device Independent Bitmap stored in m_memory.
    //
    // @params
    // * int width, new width of the buffer
    // * int height, new height of the buffer
    ScreenBuffer(int width, int height);

    // ~ScreenBuffer
    // ====================================================================== //
    // Free the DIB, a.k.a. the Device Independent Bitmap, a.k.a. m_memory.
    ~ScreenBuffer();

    // resizeDIBSection
    // ====================================================================== //
    // Ressizes the Device Independent Bitmap stored in m_memory;
    //
    // @params
    // * int width, new width of the buffer
    // * int height, new height of the buffer
    void resizeDIBSection(int width, int height);

    // paintWindow
    // ====================================================================== //
    // Apply the back buffer onto the given device context.
    // The back buffer is bottom-up and the origin is the lower-left corner.
    //
    // If an aspectRatio is provided, StretchDIBits will print to an area
    // that conforms to it.
    //
    // @params
    // * HDC deviceContext, pretty self explanitory
    // * RECT destRect, rect containing the destination width and height
    // * float aspectRatio = 0, the destination aspect ratio (width / height)
    void paintWindow(HDC deviceContext, RECT destRect, float aspectRatio = 0);

    // getWidth
    // ====================================================================== //
    // 
    // @return
    // The width of this buffer in pixels.
    inline long getWidth() const
    {
        return m_info.bmiHeader.biWidth;
    }
    
    // getHeight
    // ====================================================================== //
    // 
    // @return
    // The height of this buffer in pixels.
    inline long getHeight() const
    {
        return m_info.bmiHeader.biHeight;
    }

    // getStringPixelWidth
    // ====================================================================== //
    // Tells how many pixels wide the given string, scaled at the given
    // x-scale, is.
    // 
    // @params
    // * const String & s, the string being measured
    // * int xScale = 1, a multiplier to increase the width of the string
    // 
    // @return
    // How many pixels wide the string is.
    inline static int getStringPixelWidth(const String & s, int xScale = 1)
    {
        return ((s.size() * ASCII_WIDTH) + (s.size() - 1)) * xScale;
    }

    // getStringPixelHeight
    // ====================================================================== //
    // Tells how many pixels tall the given string, scaled at the given
    // y-scale, is.
    // 
    // Height is really affected by the number of '\n' (new line) characters.
    // 
    // @params
    // * const String & s, the string being measured
    // * int yScale = 1, a multiplier to increase the height of the string
    // 
    // @return
    // How many pixels tall the string is.
    inline static int getStringPixelHeight(const String & s, int yScale = 1)
    {
        return (((s.countNewLines() + 1) * ASCII_HEIGHT)) * yScale;
    }

    // getTextBoxPixelWidth
    // ====================================================================== //
    // Tells how many pixels wide the given TextBox is.
    //
    // @param
    // * const TextBox & t, the text box
    // - or -
    // * const String & s, the string in the text box
    // * unsigned scale = 1, the width sclaing of the text box string
    // 
    // @return
    // How many pixels wide the text box is.
    inline static unsigned getTextBoxPixelWidth(const TextBox & t)
    {
        return (getStringPixelWidth(t.m_text) + 4) * t.m_xScale;
    }
    inline static unsigned getTextBoxPixelWidth(const String & s, unsigned scale = 1)
    {
        return (getStringPixelWidth(s) + 4) * scale;
    }

    // getTextBoxPixelHeight
    // ====================================================================== //
    // Tells how many pixels tall the given TextBox is.
    //
    // @param
    // * const TextBox & t, the text box
    // - or -
    // * const String & s, the string in the text box
    // * unsigned scale = 1, the height sclaing of the text box string
    // 
    // @return
    // How many pixels tall the text box is.
    inline static unsigned getTextBoxPixelHeight(const TextBox & t)
    {
        return (getStringPixelHeight(t.m_text) + 4) * t.m_yScale;
    }
    inline static unsigned getTextBoxPixelHeight(const String & s, unsigned scale = 1)
    {
        return (getStringPixelHeight(s) + 4) * scale;
    }


    // ********************************************************************** //
    // vvv                 Functions to draw the Overlay                  vvv //
    // ********************************************************************** //
    // NOTE: These ignore the z buffer

    // fill
    // ====================================================================== //
    // Fills the global back buffer with the given color.
    //
    // @params
    // * const Color & color, struct containing the rgb color values
    void fill(const Color & color);

    // fade
    // ====================================================================== //
    // Fades each pixel in the back buffer to black by dividing by a given
    // value.
    // 
    // @params
    // * int f, fade value
    void fade(unsigned f);

    // drawPointOnBackBuffer
    // ====================================================================== //
    // Draw a single pixel at the given pixel coordinates.
    // Remember the backbuffer only has positive coordinates.
    //
    // @params
    // * int x, x coordinate
    // * int y, y coordinate
    // * Pair<int> v, struct holding the x and y coordinates
    // * const Color & color color, struct containing the rgb color values
    inline void drawPoint(int x, int y, const Color & color)
    {
        *(m_memory + x + y * m_info.bmiHeader.biWidth) = GET_RGB(color.r, color.g, color.b);
    }
    inline void drawPoint(Pair<int> v, const Color & color)
    {
        drawPoint(v.x, v.y, color);
    }

    // drawPointOnBackBufferSafely
    // ====================================================================== //
    // Draw a single pixel at the given pixel coordinates.
    // Makes sure that x and y are within the width and height of the buffer.
    //
    // @params
    // * int x, x coordinate
    // * int y, y coordinate
    // * Pair<int> v, struct holding the x and y coordinates
    // * const Color & color, struct containing the rgb color values
    inline void drawPointSafely(int x, int y, const Color & color)
    {
        if (x > 0 && x < m_info.bmiHeader.biWidth && y > 0 && y < m_info.bmiHeader.biHeight)
            *(m_memory + x + y * m_info.bmiHeader.biWidth) = GET_RGB(color.r, color.g, color.b);
        ///else
        ///    throw ERROR_OUTSIDE_BUFFER_BOUNDS;
    }
    inline void drawPointSafely(Pair<int> v, const Color & color) {
        drawPointSafely(v.x, v.y, color);
    }

    // drawLine
    // ====================================================================== //
    // Draw a pixel wide line between the given pixel coordinates.
    // 
    // The line is inclusive, so both coodinates are drawn. This means the
    // length of a line from (0, 0) to (3, 0) would be 4.
    // 
    // Diagonal lines are drawn using the Bresenham's line algorithm.
    //
    // @params
    // * int x0, first x coordinate
    // * int y0, first y coordinate
    // * int x1, second x coordinate
    // * int y1, second y coordinate
    // * Pair<int> v0, struct holding the first x and y coordinates
    // * Pair<int> v1, struct holding the second x and y coordinates
    // * const Color & color, struct containing the rgb color values
    void drawLine(int x0, int y0, int x1, int y1, const Color & color);
    inline void drawLine(Pair<int> v0, Pair<int> v1, const Color & color)
    {
        drawLine(v0.x, v0.y, v1.x, v1.y, color);
    }
    

    // drawDottedLine
    // ====================================================================== //
    // Draw a pixel wide dotted line between the given pixel coordinates. The
    // ending point may not be drawn depending on the number of points.
    // 
    // The line is inclusive, so both coodinates are drawn. This means the
    // length of a line from (0, 0) to (3, 0) would be 4. This being said,
    // the ending point will not be drawn if there is an even number of
    // points.
    // 
    // Diagonal lines are drawn using the Bresenham's line algorithm.
    //
    // @params
    // * int x0, first x coordinate
    // * int y0, first y coordinate
    // * int x1, second x coordinate
    // * int y1, second y coordinate
    // * Pair<int> v0, struct holding the first x and y coordinates
    // * Pair<int> v1, struct holding the second x and y coordinates
    // * const Color & color, struct containing the rgb color values
    void drawDottedLine(int x0, int y0, int x1, int y1, const Color & color);
    inline void drawDottedLine(Pair<int> v0, Pair<int> v1, const Color & color)
    {
        drawDottedLine(v0.x, v0.y, v1.x, v1.y, color);
    }

    // drawChaikinCurve
    // ====================================================================== //
    // Draw a curve using Chaikin's Algorith. The curve's path will be
    // determined by the points given and the curve's curvyness will
    // be determined by the iterations parameter.
    // 
    // Each iteration invovles calulating new points between each of the
    // given points.
    // 
    // @params
    // * Array< Pair<int> > & points, the points the path will follow
    // * const Color & color, struct containing the rgb color values
    // * unsigned iterations, iterations of Chainkin's algorithm.
    void drawChaikinCurve(Array< Pair<int> > & points, const Color & color, unsigned iterations = 2);

    // drawRectangle
    // ====================================================================== //
    // Draw a pixel wide rectangle where (x0,y0) and (x1, y1) are two
    // opposite corners.
    // 
    // @params
    // * int x0, first x coordinate
    // * int y0, first y coordinate
    // * int x1, second x coordinate
    // * int y1, second y coordinate
    // * Pair<int> v0, struct holding the first x and y coordinates
    // * Pair<int> v1, struct holding the second x and y coordinates
    // * const Color & color, struct containing the rgb color values
    void drawRectangle(int x0, int y0, int x1, int y1, const Color & color);
    inline void drawRectangle(Pair<int> v0, Pair<int> v1, const Color & color)
    {
        drawRectangle(v0.x, v0.y, v1.x, v1.y, color);
    }

    // drawFilledRectangle
    // ====================================================================== //
    // Draw a filled rectangle where (x0,y0) and (x1, y1) are two opposite 
    // corners.
    // 
    // @params
    // * int x0, first x coordinate
    // * int y0, first y coordinate
    // * int x1, second x coordinate
    // * int y1, second y coordinate
    // * Pair<int> v0, struct holding the first x and y coordinates
    // * Pair<int> v1, struct holding the second x and y coordinates
    // * const Color & color, struct containing the rgb color values
    void drawFilledRectangle(int x0, int y0, int x1, int y1, const Color & color);
    inline void drawFilledRectangle(Pair<int> v0, Pair<int> v1, const Color & color)
    {
        drawFilledRectangle(v0.x, v0.y, v1.x, v1.y, color);
    }

    // drawTriangle
    // ====================================================================== //
    // Draw a pixel wide triangle with verticies (x0, y0), (x1, y1), and
    // (x2, y2).
    // 
    // @params
    // * int x0, first vertex's x coordinate
    // * int y0, first vertex's y coordinate
    // * int x1, second veretx's x coordinate
    // * int y1, second vertex's y coordinate
    // * int x2, third veretx's x coordinate
    // * int y2, third vertex's y coordinate
    // * Pair<int> v0, struct holding the first x and y coordinates
    // * Pair<int> v1, struct holding the second x and y coordinates
    // * Pair<int> v2, struct holding the third x and y coordinates
    // * const Color & color, struct containing the rgb color values
    void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const Color & color);
    inline void drawTriangle(Pair<int> v0, Pair<int> v1, Pair<int> v2, const Color & color)
    {
        drawTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color);
    }

    // drawFilledTriangle
    // ====================================================================== //
    // Draw a filled in triangle with verticies (x0, y0), (x1, y1), and
    // (x2, y2).
    // 
    // Uses the Barycentric Algorithm.
    // 
    // @params
    // * int x0, first vertex's x coordinate
    // * int y0, first vertex's y coordinate
    // * int x1, second veretx's x coordinate
    // * int y1, second vertex's y coordinate
    // * int x2, third veretx's x coordinate
    // * int y2, third vertex's y coordinate
    // * Pair<int> v0, struct holding the first x and y coordinates
    // * Pair<int> v1, struct holding the second x and y coordinates
    // * Pair<int> v2, struct holding the third x and y coordinates
    // * const Color & color, struct containing the rgb color values
    void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const Color & color);
    inline void drawFilledTriangle(Pair<int> v0, Pair<int> v1, Pair<int> v2, const Color & color)
    {
        drawFilledTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color);
    }

    // drawCharacters
    // ====================================================================== //
    // Draws a sting of characters onto the backbuffer. The given x and y
    // represent the bottom left of the first character. The new line
    // character shifts to a new line. Also the string must be
    // null terminated!
    // 
    // @params
    // * int x, the x coordinate for the bottom left of the first character
    // * int y, the y coordinate for the bottom left of the first character
    // * Pair<int> v, struct holding the x and y coordinates for the bottom
    //                left of the first character
    // * char * string, pointer to a null terminated sting of characters
    // * const Color & color, struct containing the red-green-blue color
    // * int xScale = 1, a multiplier to increase the width of the character
    // * int yScale = 1, a multiplier to increase the height of the character
    void drawCharacters(int x, int y, char * string, const Color & color, int xScale = 1, int yScale = 1);
    inline void drawCharacters(Pair<int> v, char * string, const Color & color, int xScale = 1, int yScale = 1)
    {
        drawCharacters(v.x, v.y, string, color, xScale, yScale);
    }

    // drawString
    // ====================================================================== //
    // Draws a sting of characters onto the backbuffer. The given x and y
    // representthe bottom left of the first character. The new line
    // character shifts to a new line.
    // 
    // @params
    // * int x, the x coordinate for the bottom left of the first character
    // * int y, the y coordinate for the bottom left of the first character
    // * Pair<int> v, struct holding the x and y coordinates for the bottom
    //                left of the first character
    // * const String & string, pointer to a String object
    // * const Color & color, struct containing the red-green-blue color 
    // * int xScale = 1, a multiplier to increase the width of the character
    // * int yScale = 1, a multiplier to increase the height of the character
    void drawString(int x, int y, const String & string, const Color & color, int xScale = 1, int yScale = 1);
    inline void drawString(Pair<int> v, const String & string, const Color & color, int xScale = 1, int yScale = 1)
    {
        drawString(v.x, v.y, string, color, xScale, yScale);
    }

    // drawTextBox
    // ====================================================================== //
    // Draws a textbox object. It'll look like a string surrounded by a
    // rectangular box.
    // 
    // @params
    // * TextBox textBox, text box to be drawn (has location and color info)
    void drawTextBox(const TextBox textBox);

    // drawHeart
    // ====================================================================== //
    // Draw a heart shape. he given x and y represent the bottom left of the
    // heart.
    // 
    // @params
    // * int x, the x coordinate for the bottom left of the heart
    // * int y, the y coordinate for the bottom left of the heart
    // * const Color & color, struct containing the red-green-blue color
    void drawHeart(int x, int y, const Color & color);

    // drawHearts
    // ====================================================================== //
    // Draw a row of hearts.
    // 
    // @params
    // * int n, the number of hearts in the row
    // * int x, the x coordinate for the bottom left of the leftmost heart
    // * int y, the y coordinate for the bottom left of the leftmost heart
    // * const Color & color, struct containing the red-green-blue color
    void drawHearts(int n, int x, int y, const Color & color);


    // ********************************************************************** //
    // vvv                 Functions to draw the Graphics                 vvv //
    // ********************************************************************** //
    // NOTE: These use the z buffer

    // clear
    // ====================================================================== //
    // Fills the global back buffer with the given color and sets all the 
    // values in the z buffer to the given float, default bein -1000000.
    //
    // @params
    // * const Color & color = COLOR_BLACK, struct containing the rgb color value
    // * float w = -1000000, value the z buffer will be filled with
    void clear(const Color & color = COLOR_BLACK, float w = -1000000);

    // drawPoint3D
    // ====================================================================== //
    // Draw a single pixel at the given 3D pixel coordinates. The coordinates
    // will be in Image space, typically ranging from -1 to 1.
    //
    // @params
    // * const Point & point, point object with coordinates and color
    void drawPoint3D(const Point & point);

    // drawLine3D
    // ====================================================================== //
    // Draw a pixel wide line given 3D pixel coordinates. The coordinates
    // will be in Image space, typically ranging from -1 to 1.
    // 
    // The color of the line will be iterpolated by the color of the two
    // points making up the line.
    // 
    // The line is inclusive, so both coodinates are drawn. This means the
    // length of a line from (0, 0) to (3, 0) would be 4.
    // 
    // Diagonal lines are drawn using the Bresenham's line algorithm.
    //
    // @params
    // * const Point & p0, starting point
    // * const Point & p1, ending point
    // * const Line & l, line structure with pointers to two points
    void drawLine3D(const Point & p0, const Point & p1);
    inline void drawLine3D(const Line & l)
    {
        drawLine3D(l.p0, l.p1);
    }

    // drawTriangleOutline3D
    // ====================================================================== //
    // Draw the outlien of a triangle, given 3D coordinates. The coordinates
    // will be in Image space, typically ranging from -1 to 1.
    // 
    // The color of the line will be iterpolated by the color of the three
    // points making up the triangle.
    // 
    // @parms
    // * const Point & p0, a vertex of the triangle
    // * const Point & p1, another vertex of the triangle
    // * const Point & p2, the third vertex of the triangle
    // * const Triangle & t, triangle structure with pointers to three points
    void drawTriangleOutline3D(const Point & p0, const Point & p1, const Point & p2);
    inline void drawTriangleOutline3D(const Triangle & t)
    {
        drawTriangleOutline3D(t.p0, t.p1, t.p2);
    }

    // drawTriangle3D
    // ====================================================================== //
    // Draw a filled in triangle given 3D coordinates. The coordinates
    // will be in Image space, typically ranging from -1 to 1.
    // 
    // The color of the line will be iterpolated by the color of the three
    // points making up the triangle.
    // 
    // Uses the Barycentric Algorithm.
    // 
    // @params
    // * const Point & p0, a vertex of the triangle
    // * const Point & p1, another vertex of the triangle
    // * const Point & p2, the third vertex of the triangle
    // * const Triangle & t, triangle structure with pointers to three points
    void drawTriangle3D(const Point & p0, const Point & p1, const Point & p2);
    inline void drawTriangle3D(const Triangle & t)
    {
        drawTriangle3D(t.p0, t.p1, t.p2);
    }

    // drawPoints3D
    // ====================================================================== //
    // Draws all the given points. The coordinates are expected to be in
    // Image space, ranging from -1 to 1 in value.
    // 
    // @params
    //  * const Array<Point> & points, an array of points
    void drawPoints3D(const Array<Point> & points);

    // drawLines3D
    // ====================================================================== //
    // Similar to drawPoints3D, but lines.
    // 
    // @params
    // * const Array<Line> & lines, an array of lines
    void drawLines3D(const Array<Line> & lines);

    // drawTriangles3D
    // ====================================================================== //
    // Similar to drawPoints3D, but triangles
    // 
    // @params
    // * const Array<Triangle> & triangles, an array of triangles
    void drawTriangles3D(const Array<Triangle> & triangles);

    // drawTriangleOutliness3D
    // ====================================================================== //
    // Similar to drawPoints3D, but triangle Oulines
    // 
    // @params
    // * const Array<Triangle> & triangles, an array of triangles
    void drawTriangleOutlines3D(const Array<Triangle> & triangles);

    // rasterize
    // ====================================================================== //
    // Converts an entity, represented as points, lines, and triangles in 3D
    // space, into a 2D image on this screen.
    // 
    // @params
    // * const Camera & camera, pinhole camera looking at the entites
    // * const Entity * entity, a single entity
    void rasterize(const Camera & camera, const Entity * entity);

    // rasterizeGroup
    // ====================================================================== //
    // Converts multiple entities, represented as points, lines, and
    // triangles in 3D splace, into a 2D image on this screen.
    // 
    // @params
    // * const Camera & camera, pinhole camera looking at the entites
    // * const Array<Entity*> & entities, an array of entities
    void rasterizeGroup(const Camera & camera, const Array<Entity*> & entities);

private:
    // drawLineEx
    // ====================================================================== //
    // Draw a pixel wide line between the given pixel coordinates.
    // 
    // The line is exclusive, so the (x1, y1) coodinates are not drawn. This
    // means the length of a line from (0, 0) to (3, 0) would be 3.
    // 
    // Diagonal lines are drawn using the Bresenham's line algorithm.
    //
    // @params
    // * int x0, first x coordinate
    // * int y0, first y coordinate
    // * int x1, second x coordinate
    // * int y1, second y coordinate
    // * const Color & color, struct containing the rgb color values
    void drawLineEx(int x0, int y0, int x1, int y1, const Color & color);

    // drawHorizontalLineEx
    // ====================================================================== //
    // Draw a pixel wide horizontal line from x0 to x1 at height y. The last
    // pixel at x1 isn't included.
    // 
    // @params
    // * int x0, first x coordinate
    // * int x1, second x coordinate
    // * int y, height a.k.a y coordinate
    // * const Color & color, struct containing the rgb color values
    void drawHorizontalLineEx(int x0, int x1, int y, const Color & color);
    
    // drawVerticalLineEx
    // ====================================================================== //
    // Draw a pixel wide vertical line from y0 to y1 at x. The last pixel at 
    // y1 isn't included.
    // 
    // @params
    // * int x, x coordinate
    // * int y0, first y coordinate
    // * int y1, second y coordinate
    // * const Color & color, struct containing the rgb color values
    void drawVerticalLineEx(int x, int y0, int y1, const Color & color);

    // drawCharacter
    // ====================================================================== //
    // Draws a character on the back buffer. The given x and y represent the
    // bottom left of the character. The character is specified using
    // #deffine mask in AsciiCaracterDefines.h. The character can be scaled
    // by specifying a scale other than 1.
    //
    // @params
    // * int x, the x coordinate for the bottom left of the character
    // * int y, the y coordinate for the bottom left of the character
    // * uint32_t character, 32 bits representing the chosen character
    //                       (THIS IS NOT AN ACTUAL CHAR)
    // * const Color & color, struct containing the rgb color values
    // * int xScale = 1, a multiplier to increase the width of the character
    // * int yScale = 1, a multiplier to increase the height of the character
    void drawCharacter(int x, int y, uint32_t character, const Color & color, int xScale = 1, int yScale = 1);

    // selectCharacter
    // ====================================================================== //
    // Used by the DrawSring functions. Contains a massive switch statement
    // used to draw a given character.
    // 
    // Since all the characters are in order. I should probably do this a
    // nicer way.
    // 
    // @params
    // * int initialX, the initial x (go back to this on new line)
    // * int& currX, a reference to the x coordinate (bottom left of the char)
    // * int& currY, a reference to the y coordinate (bottom left of the char)
    // * char c, the character to be drawn
    // * const Color & color, stuct containing the rgb color values
    // * int xScale, a multiplier to increase the width of the character
    // * int yScale, a multiplier to increase the height of the character
    void selectCharacter(int initialX, int& currX, int& currY, char c, const Color & color, int xScale, int yScale);
    
    // getTriangleArea
    // ====================================================================== //
    // Calculate the area of a triangle.
    // 
    // @params
    // * int x0, first vertex's x coordinate
    // * int y0, first vertex's y coordinate
    // * int x1, second veretx's x coordinate
    // * int y1, second vertex's y coordinate
    // * int x2, third veretx's x coordinate
    // * int y2, third vertex's y coordinate
    // 
    // @return
    // The area of the triangle.
    inline float getTriangleArea(float x0, float y0, float x1, float y1, float x2, float y2)
    {
        return ABS(x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)) / 2;
    }

    // drawPointWithZCheck
    // ====================================================================== //
    // Draw a single pixel at the given pixel coordinates.
    // Makes sure that x and y are within the width and height of the buffer.
    // Uses the z coordinate and the z buffer to determine if pixel is drawn.
    // 
    // @params
    // * int x, x coordinate
    // * int y, y coordinate
    // * float z, z coordinate
    // * const Color & color, struct containing the rgb color values
    inline void drawPointWithZCheck(int x, int y, float z, const Color & color)
    {
        
        if (x > borderOffset && x < m_info.bmiHeader.biWidth - borderOffset &&
            y > borderOffset && y < m_info.bmiHeader.biHeight - borderOffset &&
            z >= *(m_zBuffer + x + y * m_info.bmiHeader.biWidth))
        {
            *(m_memory + x + y * m_info.bmiHeader.biWidth) = GET_RGB(color.r, color.g, color.b);
            *(m_zBuffer + x + y * m_info.bmiHeader.biWidth) = z;
        }
    }

    // drawLine3DEx
    // ====================================================================== //
    // Draw a pixel wide line given 3D pixel coordinates. The coordinates
    // will be in Image space, typically ranging from -1 to 1.
    // 
    // The color of the line will be iterpolated by the color of the two
    // points making up the line.
    //
    // The line is exclusive, so the last coodinate isn't drawn. This means
    // the length of a line from (0, 0) to (3, 0) would be 3.
    // 
    // Diagonal lines are drawn using the Bresenham's line algorithm.
    //
    // @params
    // * const Point & p0, starting point
    // * const Point & p1, ending point
    void drawLine3DEx(const Point & p0, const Point & p1);


    // ====================================================================== //
    // vvv                        Rasterization!                          vvv //
    // ---------------------------------------------------------------------- //

    // applySimpleLighting
    // ====================================================================== //
    // Apply a simple lighting algorithm with the given point representing
    // the light source.
    // 
    // @params
    // * const Point & source, light source
    // * float ambiant, constant multiplyer to add light in general
    // * float diffuse, constant multiplier relying on surface normal and
    //                  light source location
    // * Shape & shape, shape the lighting is applied to
    void applySimpleLighting(const Point & source, float ambiant, float diffuse, Shape & shape) const;

    // applySimpleDistanceShading
    // ====================================================================== //
    // Make points that are farther from the origin darker.
    // 
    // @params
    // * Shape & shape, shape getting shaded
    void applySimpleDistanceShading(Shape & shape) const;

    // cullBackfaces
    // ====================================================================== //
    // Remove triangles that have a surface normal facing away from the
    // camera.
    // 
    // @param
    // * Shape & shape, shape being culled
    void cullBackfaces(Shape & shape) const;

    // moveOffCameraLocation
    // ====================================================================== //
    // After applying the camera transform, some points might end up with a z
    // value of zero. Then when the viewing transform is applied, you get a w
    // value that is 0. After this, when converting back to 3d you end up 
    // dividing by zero and get undefined values for x, y, and z.
    // 
    // This function slightly moves points at z = 0 onto z = -0.001.
    // 
    // @param
    // * Shape & shape, structure holding the points
    void moveOffCameraLocation(Shape & shape) const;

    // clipBehindCamera
    // ====================================================================== //
    // Applied in camera space (after camera transform), this removes all the
    // points, clips lines, and clips triangles, behind the camera point.
    // 
    // This is so they don't flip infront of the camera when the viewing
    // transformed is applied and the point are in image space.
    // 
    // Bounds are exclusive, points at z == 0 arn't clipped.
    // 
    // @param
    // * Shape & shape, structure holding the points
    void clipBehindCamera(Shape & shape) const;

    // clipLeft
    // ====================================================================== //
    // Applied in image space (after the viewing transform), this removes
    // points, clips lines, and clips trianges, that are outside and to the
    // left of the viewing frustrum.
    // 
    // Bounds are exclusive, points at x == -1 arn't clipped.
    // 
    // @param
    // * Shape & shape, structure holding the points
    void clipLeft(Shape & shape) const;

    // clipRight
    // ====================================================================== //
    // Applied in image space (after the viewing transform), this removes
    // points, clips lines, and clips trianges, that are outside and to the 
    // right of the viewing frustrum.
    // 
    // Boudns are exclusive, points at x == 1 arn't clipped.
    // 
    // @param
    // * Shape & shape, structure holding the points
    void clipRight(Shape & shape) const;

    // clipBottom
    // ====================================================================== //
    // Applied in image space (after the viewing transform), this removes
    // points, clips lines, and clips trianges, that are outside and below
    // the viewing frustrum.
    // 
    // Boudns are exclusive, points at y == -1 arn't clipped.
    // 
    // @param
    // * Shape & shape, structure holding the points
    void clipBottom(Shape & shape) const;

    // clipTop
    // ====================================================================== //
    // Applied in image space (after the viewing transform), this removes
    // points, clips lines, and clips trianges, that are outside and above
    // the viewing frustrum.
    // 
    // Boudns are exclusive, points at y == 1 arn't clipped.
    // 
    // @param
    // * Shape & shape, structure holding the points
    void clipTop(Shape & shape) const;

    // clipFar
    // ====================================================================== //
    // Applied in image space (after the viewing transform), this removes
    // points, clips lines, and clips trianges, that are in behind of the
    // viewing frustrum's far plane.
    // 
    // Boudns are exclusive, points at z == -1 arn't clipped.
    // 
    // @param
    // * Shape & shape, structure holding the points
    void clipFar(Shape & shape) const;

    // clipNear
    // ====================================================================== //
    // Applied in image space (after the viewing transform), this removes
    // points, clips lines, and clips trianges, that are in front of the
    // viewing frustrum's near plane.
    // 
    // Boudns are exclusive, points at z == 1 arn't clipped.
    // 
    // @param
    // * Shape & shape, structure holding the points
    void clipNear(Shape & shape) const;


    // ====================================================================== //
    // !!!              The following didn't work so well.                !!! //
    // ---------------------------------------------------------------------- //

    /* ----------------------------------------------------------------------
       NOTES:
       drawQuadraticBezierCurveEx and drawCubicBezierCurveEx don't work well
       because I'm unable to acuratly determine how much I need to increment
       the t value for their Bernstein Polynomials:
       ex:
       c0 = (1 - t)^2          <= equals 1 when t equals 0
       c1 = 2 * t * (1 - t)
       c2 = t^2                <= equals 1 when t equals 1
       P(t) = c0 * p0 + c1 * p1 + c2 * p2

       I'm just gonna use Chaikin's Algorith...
       ---------------------------------------------------------------------- */

    // drawQuadraticBezierCurveEx
    // ====================================================================== //
    // Draw a parabola that passes through two of the given points and gets
    // pulled toward the other. It's a Quadratic Bezier curve. The last point
    // doesn't get drawn.
    // 
    // @params
    // * Pair<int> v0, first point, on the curve.
    // * Pair<int> v1, second point. This is the attractor
    // * Pair<int> v2, third point, on the curve, but not drawn.
    // * const Color & color, struct containing the rgb color values
    void drawQuadraticBezierCurveEx(Pair<int> v0, Pair<int> v1, Pair<int> v2, const Color & color);

    // drawCubicBezierCurveEx
    // ====================================================================== //
    // Draw a curve that passes through two of the given points and gets
    // pulled toward the other two. It's a Cubic Bezier curve. The last point
    // doesn't get drawn.
    // 
    // @params
    // * Pair<int> v0, first point, on the curve.
    // * Pair<int> v1, second point. This is the attractor
    // * Pair<int> v2, third point. This is the attractor
    // * Pair<int> v3, fourth point, on the curve, but not drawn.
    // * const Color & color, struct containing the rgb color values
    void drawCubicBezierCurveEx(Pair<int> v0, Pair<int> v1, Pair<int> v2, Pair<int> v3, const Color & color);

    // getQuadraticBezierPoint
    // ====================================================================== //
    // Get the quadratic bezier point at a given t.
    // Uses Bernstein Polynomials where 0 <= t <= 1
    // c0 = (1 - t)^2          <= equals 1 when t equals 0
    // c1 = 2 * t * (1 - t)
    // c2 = t^2                <= equals 1 when t equals 1
    // P(t) = c0 * p0 + c1 * p1 + c2 * p2
    // 
    // @params
    // * float t
    // * Pair<int> v0, first point, when t = 0 the output will equal v0
    // * Pair<int> v1, second point; this is the attractor
    // * Pair<int> v2, third point, when t = 1 the output will equal v1
    // 
    // @return
    // A pair containing the point generated.
    Pair<int> getQuadraticBezierPoint(float t, Pair<int> v0, Pair<int> v1, Pair<int> v2);

    // getCubicBezierPoint
    // ====================================================================== //
    // Get the cubic bezier point at a given t.
    // Uses Bernstein Polynomials where 0 <= t <= 1
    // c0 = (1 - t)^3          <= equals 1 when t equals 0
    // c1 = 3 * t * (1 - t)^2
    // c2 = 3 * t^2 * (1 - t)
    // c3 = t^3                <= equals 1 when t equals 1
    // P(t) = c0 * p0 + c1 * p1 + c2 * p2 + c3 * c3
    // 
    // @params
    // * float t
    // * Pair<int> v0, first point, when t = 0 the output will equal v0
    // * Pair<int> v1, second point; this is an attractor
    // * Pair<int> v2, thrid point; this is another attractor
    // * Pair<int> v3, fourth point, when t = 1 the output will equal v1
    // 
    // @return
    // A pair containing the point generated.
    Pair<int> getCubicBezierPoint(float t, Pair<int> v0, Pair<int> v1, Pair<int> v2, Pair<int> v3);
    
private:
    // The BITMAPINFO struct defines dimensions and color information for a DIB
    BITMAPINFO m_info;
    // .  BITMAPINFOHEADER bmiHeader;
    //     .  DWORD            biSize;
    //     .  LONG             biWidth;
    //     .  LONG             biHeight;
    //     .  WORD             biPlanes;
    //     .  WORD             biBitCount;
    //     .  DWORD            biCompression;
    //     .  DWORD            biSizeImage;
    //     .  LONG             biXPelsPerMeter;
    //     .  LONG             biYPelsPerMeter;
    //     .  DWORD            biClrUsed;
    //     .  DWORD            biClrImportant;
    // .  RGBQUAD          bmiColors[1];
    //     .  BYTE             rgbBlue;
    //     .  BYTE             rgbGreen;
    //     .  BYTE             rgbRed;
    //     .  BYTE             rgbReserved;

    // A pixel in memory looks like 0x 00 RR GG BB
    // NOTE: This is a little Endian Architecture so the first byte is blue
    uint32_t * m_memory;
    
    // Used to determine which pixel two draw when objects overlap
    float * m_zBuffer;

    // Pixel boarder around the 3D drawing space seperating it from the buffer edge
    unsigned borderOffset;
};