/* ==========================================================================
   >File: ScreenBuffer.cpp
   >Date: 20180713
   >Author: Vik Pandher
   >Details: This object holds the pixels that will be displayed. The origin
             is the bottom left. It has a lot of functions to draw with.
   ========================================================================== */

#include "ScreenBuffer.h"



// public:

ScreenBuffer::ScreenBuffer(int width, int height) : borderOffset(0)
{
    m_info.bmiHeader.biSize = sizeof(m_info.bmiHeader);
    m_info.bmiHeader.biWidth = width;

    // make height negative to make this a top-down DIB
    m_info.bmiHeader.biHeight = height;
    m_info.bmiHeader.biPlanes = 1;

    // 8 bits red, 8 green, 8 blue, and a padding byte.
    // Why the padding byte? To avoid unaligned accessing penalties.
    m_info.bmiHeader.biBitCount = 32;
    m_info.bmiHeader.biCompression = BI_RGB;

    // 4 bytes per pixel
    int bitmapMemprySize = width * height * 4;
    ///m_memory = (uint32_t *)g_memoryManager.allocate(bitmapMemprySize);
    m_memory = (uint32_t*)VirtualAlloc(
        0,                // lpAddress, we don't care where the memory is
        bitmapMemprySize, // dwSize, size of the region in bytes
        MEM_COMMIT,       // flAllocationType
        PAGE_READWRITE);  // flProtect
    
    // a float for each pixel
    int zBufferSize = width * height * sizeof(float);
    ///m_zBuffer = (float *)g_memoryManager.allocate(zBufferSize);
    m_zBuffer = (float*)VirtualAlloc(
        0,                // lpAddress, we don't care where the memory is
        zBufferSize,      // dwSize, size of the region in bytes
        MEM_COMMIT,       // flAllocationType
        PAGE_READWRITE);  // flProtect
}


ScreenBuffer::~ScreenBuffer()
{
    ///g_memoryManager.free(m_memory);
    ///g_memoryManager.free(m_zBuffer);
    
    LONG & width = m_info.bmiHeader.biWidth;
    LONG & height = m_info.bmiHeader.biHeight;

    int bitmapMemprySize = width * height * 4;
    VirtualFree(
        m_memory,
        bitmapMemprySize,
        MEM_DECOMMIT
    );

    int zBufferSize = width * height * sizeof(float);
    VirtualFree(
        m_zBuffer,
        zBufferSize,
        MEM_DECOMMIT
    );
}


void ScreenBuffer::resizeDIBSection(int width, int height)
{
    // Free the old memory if it exists
    if (m_memory)
    {
        ///g_memoryManager.free(m_memory);
        ///g_memoryManager.free(m_zBuffer);

        LONG & width = m_info.bmiHeader.biWidth;
        LONG & height = m_info.bmiHeader.biHeight;

        int bitmapMemprySize = width * height * 4;
        VirtualFree(
            m_memory,
            bitmapMemprySize,
            MEM_DECOMMIT
        );

        int zBufferSize = width * height * sizeof(float);
        VirtualFree(
            m_zBuffer,
            zBufferSize,
            MEM_DECOMMIT
        );
    }

    m_info.bmiHeader.biSize = sizeof(m_info.bmiHeader);
    m_info.bmiHeader.biWidth = width;
    m_info.bmiHeader.biHeight = height;
    m_info.bmiHeader.biPlanes = 1;
    m_info.bmiHeader.biBitCount = 32;
    m_info.bmiHeader.biCompression = BI_RGB;

    int bitmapMemprySize = width * height * 4;
    ///m_memory = (uint32_t *)g_memoryManager.allocate(bitmapMemprySize);
    m_memory = (uint32_t*)VirtualAlloc(
        0,                // lpAddress, we don't care where the memory is
        bitmapMemprySize, // dwSize, size of the region in bytes
        MEM_COMMIT,       // flAllocationType
        PAGE_READWRITE);  // flProtect

                          // a float for each pixel
    int zBufferSize = width * height * sizeof(float);
    ///m_zBuffer = (float *)g_memoryManager.allocate(zBufferSize);
    m_zBuffer = (float*)VirtualAlloc(
        0,                // lpAddress, we don't care where the memory is
        zBufferSize,      // dwSize, size of the region in bytes
        MEM_COMMIT,       // flAllocationType
        PAGE_READWRITE);  // flProtect
}


void ScreenBuffer::paintWindow(HDC deviceContext, RECT destRect, float aspectRatio /*= 0*/)
{
    // This function copies the color data for a rectangle of pixels in a DIB,
    // JPEG, or PNG image to the specified destination rectangle. Scaling it 
    // if need be.
    // NOTE: The origin of a bottom-up DIB is the lower-left corner;
    //       the origin of a top-down DIB is the upper-left corner.
    int destWidth = destRect.right - destRect.left;
    int destHeight = destRect.bottom - destRect.top;
    int srcWidth = m_info.bmiHeader.biWidth;
    int srcHeight = m_info.bmiHeader.biHeight;
    int destX = 0;
    int destY = 0;

    if (aspectRatio != 0)
    {
        int newDestWidth = destHeight * aspectRatio;
        int newDestHeight = destWidth / aspectRatio;
        if (newDestWidth > destWidth) newDestWidth = destWidth;
        if (newDestHeight > destHeight) newDestHeight = destHeight;
        if (newDestWidth < destWidth) destX = (destWidth - newDestWidth) / 2;
        if (newDestHeight < destHeight) destY = (destHeight - newDestHeight) / 2;
        destWidth = newDestWidth;
        destHeight = newDestHeight;
    }

    StretchDIBits(
        deviceContext,       // hdc
        destX,               // destX
        destY,               // destY
        destWidth,           // destWidth
        destHeight,          // destHeight
        0,                   // srcX
        0,                   // srcY
        srcWidth,            // srcWidth
        srcHeight,           // srcHeight
        m_memory,            // *lpBits
        &m_info,             // *lpBitsInfo
        DIB_RGB_COLORS,      // iUsage
        SRCCOPY);            // dwRop
}


void ScreenBuffer::fill(const Color & color)
{
    int height = m_info.bmiHeader.biHeight;
    int width = m_info.bmiHeader.biWidth;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            *(m_memory + x + y * width) = GET_RGB(color.r, color.g, color.b);
        }
    }
}


void ScreenBuffer::fade(unsigned f)
{
    int height = m_info.bmiHeader.biHeight;
    int width = m_info.bmiHeader.biWidth;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            Color newC = Color(*(m_memory + x + y * width)) / f;
            *(m_memory + x + y * width) = GET_RGB(newC.r, newC.g, newC.b);

        }
    }
}


void ScreenBuffer::drawLine(int x0, int y0, int x1, int y1, const Color & color)
{
    drawLineEx(x0, y0, x1, y1, color);
    drawPointSafely(x1, y1, color); // include point (x1,y1) in the line
}


void ScreenBuffer::drawDottedLine(int x0, int y0, int x1, int y1, const Color & color)
{
    float deltaX = x1 - x0; // positive if line drawn from x0 to x1
    float deltaY = y1 - y0; // positive if line drawn from y0 to y1
    int xIncrement = SIGN(deltaX);
    int yIncrement = SIGN(deltaY);
    bool skip = false;

    if (deltaY == 0)
    {
        for (int x = x0; x != x1; x += xIncrement)
        {
            if (!skip) drawPointSafely(x, y0, color);
            skip = !skip;
        }
    }
    else if (deltaX == 0)
    {
        for (int y = y0; y != y1; y += yIncrement)
        {
            if (!skip) drawPointSafely(x0, y, color);
            skip = !skip;
        }
    }
    else
    {
        float xError = 0;
        float yError = 0;
        int x = x0;
        int y = y0;
        while (x != x1 || y != y1)
        {
            // error will grow faster for whichever is delta is bigger
            xError += ABS(deltaX / deltaY);
            yError += ABS(deltaY / deltaX);
            if (xError >= 0.5 && yError >= 0.5)
            {
                if (!skip) drawPointSafely(x, y, color);
                skip = !skip;
                x += xIncrement;
                xError--;
                y += yIncrement;
                yError--;
            }
            else if (xError >= 0.5)
            {
                if (!skip) drawPointSafely(x, y, color);
                skip = !skip;
                x += xIncrement;
                xError--;
            }
            else if (yError >= 0.5)
            {
                if (!skip) drawPointSafely(x, y, color);
                skip = !skip;
                y += yIncrement;
                yError--;
            }
        }
    }
    // draw last point
    if (!skip) drawPointSafely(x1, y1, color);
}


void ScreenBuffer::drawChaikinCurve(Array< Pair<int> > & points, const Color & color, unsigned iterations /*= 0*/)
{
    for (int i = 0; i < iterations; i++)
    {
        Array< Pair<int> > newPoints;
        newPoints.setCapacity(2 + points.size() * 2);
        newPoints.pushBack(points[0]); // keep the first point
        for (int j = 0; j < points.size() - 1; j++)
        {
            int x1 = points[j].x + (points[j + 1].x - points[j].x) / 4;
            int y1 = points[j].y + (points[j + 1].y - points[j].y) / 4;
            int x2 = points[j].x + (points[j + 1].x - points[j].x) * 3 / 4;
            int y2 = points[j].y + (points[j + 1].y - points[j].y) * 3 / 4;

            newPoints.pushBack(Pair<int>(x1, y1));
            newPoints.pushBack(Pair<int>(x2, y2));
        }
        newPoints.pushBack(points[points.size() - 1]); // keep the last point
        points = newPoints;
    }

    for (int i = 0; i < points.size() - 1; i++)
    {
        drawLineEx(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
    }
    drawPointSafely(points[points.size() - 1].x, points[points.size() - 1].y, color);
}


void ScreenBuffer::drawRectangle(int x0, int y0, int x1, int y1, const Color & color)
{
    drawHorizontalLineEx(x0, x1, y0, color);
    drawVerticalLineEx(x1, y0, y1, color);
    drawHorizontalLineEx(x1, x0, y1, color);
    drawVerticalLineEx(x0, y1, y0, color);
}


void ScreenBuffer::drawFilledRectangle(int x0, int y0, int x1, int y1, const Color & color)
{
    if (y0 > y1) SWAP(y0, y1);
    if (x0 > x1) SWAP(x0, x1);

    for (int y = y0; y <= y1; y++)
    {
        for (int x = x0; x <= x1; x++)
        {
            drawPointSafely(x, y, color);
        }
    }
}


void ScreenBuffer::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const Color & color)
{
    drawLineEx(x0, y0, x1, y1, color);
    drawLineEx(x1, y1, x2, y2, color);
    drawLineEx(x2, y2, x0, y0, color);
}


void ScreenBuffer::drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const Color & color)
{
    int minX = MIN(x0, MIN(x1, x2));
    int maxX = MAX(x0, MAX(x1, x2));
    int minY = MIN(y0, MIN(y1, y2));
    int maxY = MAX(y0, MAX(y1, y2));

    // Pair representing a spanning vector on the edge (v0, v1)
    int x01 = x1 - x0;
    int y01 = y1 - y0;

    // Pair representing a spanning vector on the edge (v0, v2)
    int x02 = x2 - x0;
    int y02 = y2 - y0;
    
    // iterate over all points possibly in the triangle
    for (int xP = minX; xP <= maxX; xP++)
    {
        for (int yP = minY; yP <= maxY; yP++)
        {
            // Pair representing the spanning vector (v0, vP)
            int x0P = xP - x0;
            int y0P = yP - y0;

            // cross product of v01 and v02
            int c0102 = x01 * y02 - y01 * x02;

            // cross product of v0P and v02
            int c0P02 = x0P * y02 - y0P * x02;

            // cross product of v01 and v0P
            int c010P = x01 * y0P - y01 * x0P;

            float a = (float)c0P02 / c0102;
            float b = (float)c010P / c0102;

            // draw the point if it's inside the triangle
            if (a >= 0 && b >= 0 && a + b <= 1)
            {
                drawPointSafely(xP, yP, color);
            }

        }
    }
}


void ScreenBuffer::drawCharacters(int x, int y, char * string, const Color & color, int xScale /*= 1*/, int yScale /*= 1*/)
{
    int currX = x;
    int currY = y;
    for (int stringPos = 0; string[stringPos] != '\0'; stringPos++)
    {
        selectCharacter(x, currX, currY, string[stringPos], color, xScale, yScale);
    }
}


void ScreenBuffer::drawString(int x, int y, const String & string, const Color & color, int xScale /*= 1*/, int yScale /*= 1*/)
{
    int currX = x;
    int currY = y;
    for (int stringPos = 0; stringPos < string.size(); stringPos++)
    {
        selectCharacter(x, currX, currY, string[stringPos], color, xScale, yScale);
    }
}


void ScreenBuffer::drawTextBox(const TextBox textBox)
{
    const String & text = textBox.m_text;
    const unsigned & bottomLeftX = textBox.m_x;
    const unsigned & bottomLeftY = textBox.m_y;
    unsigned topRightX = bottomLeftX + getTextBoxPixelWidth(textBox) - 1; // (ScreenBuffer::getStringPixelWidth(text) + 3) * textBox.xScale;
    unsigned topRightY = bottomLeftY + getTextBoxPixelHeight(textBox) - 1; // (ScreenBuffer::getStringPixelHeight(text) + 3) * textBox.yScale;
    unsigned textBottomLeftX = bottomLeftX + textBox.m_xScale * 2;
    unsigned textBottomLeftY = bottomLeftY + textBox.m_yScale * 2;
    
    if (!textBox.m_selected)
    {
        if (textBox.m_hasBorder)
        {
            drawRectangle(bottomLeftX, bottomLeftY, topRightX, topRightY, textBox.m_color1);
        }
        if (textBox.m_hasBackground)
        {
            drawFilledRectangle(bottomLeftX + 1, bottomLeftY + 1, topRightX - 1, topRightY - 1, textBox.m_color2);
        }
        drawString(textBottomLeftX, textBottomLeftY, text, textBox.m_color0, textBox.m_xScale, textBox.m_yScale);
    }
    else
    {
        drawFilledRectangle(bottomLeftX + 1, bottomLeftY + 1, topRightX - 1, topRightY - 1, textBox.m_color0);
        drawString(textBottomLeftX, textBottomLeftY, text, textBox.m_color2, textBox.m_xScale, textBox.m_yScale);
    }
}


void ScreenBuffer::drawHeart(int x, int y, const Color & color)
{
    uint64_t image = 0b0000000000000000000000011011011111111111111011111000111000001000;
    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 7; col++)
        {
            if (image & 1)
            {
                drawPointSafely(x + col, y + row, color);
            }
            image = image >> 1;
        }
    }
}


void ScreenBuffer::drawHearts(int n, int x, int y, const Color & color)
{
    for (int i = 0; i < n; i++)
    {
        drawHeart(x, y,color);
        x += 8;
    }
}


void ScreenBuffer::clear(const Color & color /*= COLOR_BLACK*/, float w /*= -1000000*/)
{
    int height = m_info.bmiHeader.biHeight;
    int width = m_info.bmiHeader.biWidth;

    for (int y = borderOffset; y < height - borderOffset; y++)
    {
        for (int x = borderOffset; x < width - borderOffset; x++)
        {

            *(m_memory + x + y * width) = GET_RGB(color.r, color.g, color.b);
            *(m_zBuffer + x + y * width) = w;
        }
    }

    drawRectangle(borderOffset, borderOffset, width - borderOffset, height - borderOffset, COLOR_WHITE);
}


void ScreenBuffer::drawPoint3D(const Point & point)
{
    int borderHeight = m_info.bmiHeader.biHeight - borderOffset * 2;
    int borderWidth = m_info.bmiHeader.biWidth - borderOffset * 2;

    int viewX = borderOffset;
    int viewY = borderOffset;
    if (borderWidth >= borderHeight)
    {
        viewX += (long long)((point.x + 1) / 2 * borderWidth);
        viewY += (long long)((point.y + 1) / 2 * borderWidth) - (borderWidth - borderHeight) / 2;
    }
    else
    {
        viewX += (long long)((point.x + 1) / 2 * borderHeight) - (borderHeight - borderWidth) / 2;
        viewY += (long long)((point.y + 1) / 2 * borderHeight);
    }

    drawPointWithZCheck(viewX, viewY, point.z, point.m_color);
}


void ScreenBuffer::drawLine3D(const Point & p0, const Point & p1)
{
    drawLine3DEx(p0, p1);
    drawPointWithZCheck(p1.x, p1.y, p1.z, p1.m_color); // draw end point
}


void ScreenBuffer::drawTriangleOutline3D(const Point & p0, const Point & p1, const Point & p2)
{
    drawLine3DEx(p0, p1);
    drawLine3DEx(p1, p2);
    drawLine3DEx(p0, p2);
}


void ScreenBuffer::drawTriangle3D(const Point & p0, const Point & p1, const Point & p2)
{
    int borderHeight = m_info.bmiHeader.biHeight - borderOffset * 2;
    int borderWidth = m_info.bmiHeader.biWidth - borderOffset * 2;

    int x0, y0, x1, y1, x2, y2;
    if (borderWidth >= borderHeight)
    {
        x0 = borderOffset + (long long)((p0.x + 1) / 2 * borderWidth);
        y0 = borderOffset + (long long)((p0.y + 1) / 2 * borderWidth) - (borderWidth - borderHeight) / 2;
        x1 = borderOffset + (long long)((p1.x + 1) / 2 * borderWidth);
        y1 = borderOffset + (long long)((p1.y + 1) / 2 * borderWidth) - (borderWidth - borderHeight) / 2;
        x2 = borderOffset + (long long)((p2.x + 1) / 2 * borderWidth);
        y2 = borderOffset + (long long)((p2.y + 1) / 2 * borderWidth) - (borderWidth - borderHeight) / 2;
    }
    else
    {
        x0 = borderOffset + (long long)((p0.x + 1) / 2 * borderHeight) - (borderHeight - borderWidth) / 2;
        y0 = borderOffset + (long long)((p0.y + 1) / 2 * borderHeight);
        x1 = borderOffset + (long long)((p1.x + 1) / 2 * borderHeight) - (borderHeight - borderWidth) / 2;
        y1 = borderOffset + (long long)((p1.y + 1) / 2 * borderHeight);
        x2 = borderOffset + (long long)((p2.x + 1) / 2 * borderHeight) - (borderHeight - borderWidth) / 2;
        y2 = borderOffset + (long long)((p2.y + 1) / 2 * borderHeight);
    }

    int minX = MIN(x0, MIN(x1, x2));
    int maxX = MAX(x0, MAX(x1, x2));
    int minY = MIN(y0, MIN(y1, y2));
    int maxY = MAX(y0, MAX(y1, y2));

    float triangleArea = getTriangleArea(x0, y0, x1, y1, x2, y2);

    // Pair representing a spanning vector on the edge (v0, v1)
    int x01 = x1 - x0;
    int y01 = y1 - y0;

    // Pair representing a spanning vector on the edge (v0, v2)
    int x02 = x2 - x0;
    int y02 = y2 - y0;
    
    // iterate over all points possibly in the triangle
    for (int xP = minX; xP <= maxX; xP++)
    {
        for (int yP = minY; yP <= maxY; yP++)
        {
            // Pair representing the spanning vector (v0, vP)
            int x0P = xP - x0;
            int y0P = yP - y0;

            // cross product of v01 and v02
            int c0102 = x01 * y02 - y01 * x02;

            // cross product of v0P and v02
            int c0P02 = x0P * y02 - y0P * x02;

            // cross product of v01 and v0P
            int c010P = x01 * y0P - y01 * x0P;

            float a = (float)c0P02 / c0102;
            float b = (float)c010P / c0102;

            // draw the point if it's inside the triangle
            if (a >= 0 && b >= 0 && a + b <= 1)
            {
                // figure out the z value at this point
                float area12P = getTriangleArea(x1, y1, x2, y2, xP, yP);
                float area20P = getTriangleArea(x2, y2, x0, y0, xP, yP);
                float area01P = getTriangleArea(x0, y0, x1, y1, xP, yP);

                float lambda0 = area12P / triangleArea;
                float lambda1 = area20P / triangleArea;
                float lambda2 = area01P / triangleArea;

                float zP = lambda0 * p0.z + lambda1 * p1.z + lambda2 * p2.z;
                Color cP = p0.m_color.getFade(lambda0) + p1.m_color.getFade(lambda1) + p2.m_color.getFade(lambda2);
                drawPointWithZCheck(xP, yP, zP, cP);
            }

        }
    }
}


void ScreenBuffer::drawPoints3D(const Array<Point> & points)
{
    for (int i = 0; i < points.size(); i++)
    {
        drawPoint3D(points[i]);
    }
}


void ScreenBuffer::drawLines3D(const Array<Line> & lines)
{
    for (int i = 0; i < lines.size(); i++)
    {
        drawLine3D(lines[i]);
    }
}


void ScreenBuffer::drawTriangles3D(const Array<Triangle> & triangles)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        drawTriangle3D(triangles[i]);
    }
}


void ScreenBuffer::drawTriangleOutlines3D(const Array<Triangle> & triangles)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        drawTriangleOutline3D(triangles[i]);
    }
}


void ScreenBuffer::rasterize(const Camera & camera, const Entity * entity)
{
    Shape localShape;

    // If drawing points or normals, need to regenerate the world space shape
    // with points and normals. Else just copy the one generated for collision
    // detection.
    if (entity->drawProperties & DRAW_POINTS || entity->drawProperties & DRAW_NORMALS)
    {
        localShape = entity->getShapeInWorldSpace();
    }
    else
    {
        localShape = entity->worldSpaceShape;
    }

    // Set up the Camera Transform
    Matrix cameraTransform;
    Vector viewingVector = camera.centerOfAttention - camera.cameraLocation;
    cameraTransform.addCameraTransform(camera.cameraLocation, viewingVector, camera.upDirection);

    // Set up the Viewing Transform
    Matrix viewingTransform;
    viewingTransform.addViewingTransform(camera.viewingAngle, camera.nearPlane, camera.farPlane);

    localShape *= cameraTransform; // maybe apply this transform to the frame structure?
    cullBackfaces(localShape);
    clipBehindCamera(localShape);
    applySimpleLighting(Point(), 0.2, 0.8, localShape);
    if (!(entity->drawProperties & DRAW_DISTANCE_SHADING_OFF))
    {
        applySimpleDistanceShading(localShape);
    }
    moveOffCameraLocation(localShape);
    localShape *= viewingTransform;
    localShape.to3D();
    clipLeft(localShape);
    clipRight(localShape);
    clipBottom(localShape);
    clipTop(localShape);
    clipFar(localShape);
    clipNear(localShape);

    if (entity->drawProperties & DRAW_POINTS)
    {
        drawPoints3D(localShape.points);
    }
    if ((entity->drawProperties & DRAW_LINES) || entity->drawProperties & DRAW_NORMALS)
    {
        drawLines3D(localShape.lines);
    }
    if (entity->drawProperties & DRAW_TRIANGLES)
    {
        if (entity->drawProperties & DRAW_TRIANGLE_FRAMES)
        {
            drawTriangleOutlines3D(localShape.triangles);
        }
        else
        {
            drawTriangles3D(localShape.triangles);
        }
    }
}


void ScreenBuffer::rasterizeGroup(const Camera & camera, const Array<Entity*> & entities)
{
    // Create an array to hold shapes for all entities and their respective properties 
    Array<Shape> localShapes;
    Array<uint8_t> localShapeDrawProperties;

    for (int i = 0; i < entities.size(); i++)
    {
        localShapeDrawProperties += entities[i]->drawProperties;
        if (localShapeDrawProperties[i] & DRAW_POINTS || localShapeDrawProperties[i] & DRAW_NORMALS)
        {
            localShapes += entities[i]->getShapeInWorldSpace();
        }
        else
        {
            localShapes += entities[i]->worldSpaceShape;
        }
    }
    
    // Set up the Camera Transform
    Matrix cameraTransform;
    Vector viewingVector = camera.centerOfAttention - camera.cameraLocation;
    cameraTransform.addCameraTransform(camera.cameraLocation, viewingVector, camera.upDirection);

    // Set up the Viewing Transform
    Matrix viewingTransform;
    viewingTransform.addViewingTransform(camera.viewingAngle, camera.nearPlane, camera.farPlane);

    for (int i = 0; i < localShapes.size(); i++)
    {
        localShapes[i] *= cameraTransform; // maybe apply this transform to the frame structure?
        cullBackfaces(localShapes[i]);
        clipBehindCamera(localShapes[i]);
        applySimpleLighting(Point(), 0.2, 0.8, localShapes[i]);
        if (!(localShapeDrawProperties[i] & DRAW_DISTANCE_SHADING_OFF))
        {
            applySimpleDistanceShading(localShapes[i]);
        }
        moveOffCameraLocation(localShapes[i]);
        localShapes[i] *= viewingTransform;
        localShapes[i].to3D();
        clipLeft(localShapes[i]);
        clipRight(localShapes[i]);
        clipBottom(localShapes[i]);
        clipTop(localShapes[i]);
        clipFar(localShapes[i]);
        clipNear(localShapes[i]);
    }

    for (int i = 0; i < localShapes.size(); i++)
    {
        if (localShapeDrawProperties[i] & DRAW_POINTS)
        {
            drawPoints3D(localShapes[i].points);
        }
        if ((localShapeDrawProperties[i] & DRAW_LINES) || localShapeDrawProperties[i] & DRAW_NORMALS)
        {
            drawLines3D(localShapes[i].lines);
        }
        if (localShapeDrawProperties[i] & DRAW_TRIANGLES)
        {
            if (localShapeDrawProperties[i] & DRAW_TRIANGLE_FRAMES)
            {
                drawTriangleOutlines3D(localShapes[i].triangles);
            }
            else
            {
                drawTriangles3D(localShapes[i].triangles);
            }
        }

    }
}


// private:

void ScreenBuffer::drawLineEx(int x0, int y0, int x1, int y1, const Color & color)
{
    float deltaX = x1 - x0; // positive if line drawn from x0 to x1
    float deltaY = y1 - y0; // positive if line drawn from y0 to y1

    if (deltaX == 0)
    {
        drawVerticalLineEx(x0, y0, y1, color);
    }
    else if (deltaY == 0)
    {
        drawHorizontalLineEx(x0, x1, y0, color);
    }
    else
    {
        float xError = 0;
        float yError = 0;
        int x = x0;
        int y = y0;
        while (x != x1 || y != y1)
        {
            // error will grow faster for whichever is delta is bigger
            xError += ABS(deltaX / deltaY);
            yError += ABS(deltaY / deltaX);
            if (xError >= 0.5 && yError >= 0.5)
            {
                drawPointSafely(x, y, color);
                x += SIGN(deltaX);
                xError--;
                y += SIGN(deltaY);
                yError--;
            }
            else if (xError >= 0.5)
            {
                drawPointSafely(x, y, color);
                x += SIGN(deltaX);
                xError--;
            }
            else if (yError >= 0.5)
            {
                drawPointSafely(x, y, color);
                y += SIGN(deltaY);
                yError--;
            }
        }
    }
}


void ScreenBuffer::drawHorizontalLineEx(int x0, int x1, int y, const Color & color)
{
    int increment = SIGN(x1 - x0);
    for (int x = x0; x != x1; x += increment)
    {
        drawPointSafely(x, y, color);
    }
}


void ScreenBuffer::drawVerticalLineEx(int x, int y0, int y1, const Color & color)
{
    int increment = SIGN(y1 - y0);
    for (int y = y0; y != y1; y += increment)
    {
        drawPointSafely(x, y, color);
    }
}


void ScreenBuffer::drawCharacter(int x, int y, uint32_t character, const Color & color, int xScale /*= 1*/, int yScale /*= 1*/)
{
    uint32_t mask = 0b00100000000000000000000000000000;
    for (int row = 0; row < ASCII_HEIGHT * yScale; row += yScale)
    {
        for (int col = 0; col < ASCII_WIDTH * xScale; col += xScale)
        {
            if (character & mask)
            {
                for (int pRow = 0; pRow < yScale; pRow++)
                {
                    for (int pCol = 0; pCol < xScale; pCol++)
                    {
                        drawPointSafely(x + col + pCol, y + row + pRow, color);
                    }
                }
            }
            mask = mask >> 1;
        }
    }
}


void ScreenBuffer::selectCharacter(int initialX, int& currX, int& currY, char c, const Color & color, int xScale, int yScale)
{
    switch (c)
    {
    case ' ':
    {
        drawCharacter(currX, currY, ASCII_SPACE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '!':
    {
        drawCharacter(currX, currY, ASCII_EXCLAMATION_MARK, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '"':
    {
        drawCharacter(currX, currY, ASCII_DOUBLE_QUOTES, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '#':
    {
        drawCharacter(currX, currY, ASCII_NUMBER_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '$':
    {
        drawCharacter(currX, currY, ASCII_DOLLAR_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '%':
    {
        drawCharacter(currX, currY, ASCII_PRECENT_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '&':
    {
        drawCharacter(currX, currY, ASCII_AMPERSAND, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '\'':
    {
        drawCharacter(currX, currY, ASCII_APOSTROPHE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '(':
    {
        drawCharacter(currX, currY, ASCII_LEFT_ROUND_BRACKET, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case ')':
    {
        drawCharacter(currX, currY, ASCII_RIGHT_ROUND_BRACKET, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '*':
    {
        drawCharacter(currX, currY, ASCII_ASTERISK, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '+':
    {
        drawCharacter(currX, currY, ASCII_PLUS_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case ',':
    {
        drawCharacter(currX, currY, ASCII_COMMA, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '-':
    {
        drawCharacter(currX, currY, ASCII_MINUS_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '.':
    {
        drawCharacter(currX, currY, ASCII_PERIOD, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '/':
    {
        drawCharacter(currX, currY, ASCII_FORWARD_SLASH, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '0':
    {
        drawCharacter(currX, currY, ASCII_ZERO, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '1':
    {
        drawCharacter(currX, currY, ASCII_ONE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '2':
    {
        drawCharacter(currX, currY, ASCII_TWO, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '3':
    {
        drawCharacter(currX, currY, ASCII_THREE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '4':
    {
        drawCharacter(currX, currY, ASCII_FOUR, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '5':
    {
        drawCharacter(currX, currY, ASCII_FIVE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '6':
    {
        drawCharacter(currX, currY, ASCII_SIX, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '7':
    {
        drawCharacter(currX, currY, ASCII_SEVEN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '8':
    {
        drawCharacter(currX, currY, ASCII_EIGHT, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '9':
    {
        drawCharacter(currX, currY, ASCII_NINE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case ':':
    {
        drawCharacter(currX, currY, ASCII_COLON, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case ';':
    {
        drawCharacter(currX, currY, ASCII_SEMICOLON, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '<':
    {
        drawCharacter(currX, currY, ASCII_LESS_THAN_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '=':
    {
        drawCharacter(currX, currY, ASCII_EQUALS_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '>':
    {
        drawCharacter(currX, currY, ASCII_GREATER_THAN_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '?':
    {
        drawCharacter(currX, currY, ASCII_QUESTION_MARK, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case '@':
    {
        drawCharacter(currX, currY, ASCII_AT_SIGN, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case 'A':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_A, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case 'B':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_B, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case 'C':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_C, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case 'D':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_D, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case 'E':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_E, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1) * xScale;
        break;
    }

    case 'F':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_F, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'G':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_G, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'H':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_H, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'I':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_I, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'J':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_J, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'K':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_K, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'L':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_L, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'M':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_M, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'N':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_N, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'O':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_O, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'P':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_P, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'Q':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_Q, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'R':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_R, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'S':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_S, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'T':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_T, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'U':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_U, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'V':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_V, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'W':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_W, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'X':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_X, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'Y':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_Y, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'Z':
    {
        drawCharacter(currX, currY, ASCII_CAPITAL_Z, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '[':
    {
        drawCharacter(currX, currY, ASCII_LEFT_SQUARE_BRACKET, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '\\':
    {
        drawCharacter(currX, currY, ASCII_BACK_SLASH, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case ']':
    {
        drawCharacter(currX, currY, ASCII_RIGHT_SQUARE_BRACKET, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '^':
    {
        drawCharacter(currX, currY, ASCII_CARET, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '_':
    {
        drawCharacter(currX, currY, ASCII_UNDERSCORE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '`':
    {
        drawCharacter(currX, currY, ASCII_ACUTE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'a':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_A, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'b':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_B, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'c':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_C, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'd':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_D, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'e':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_E, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'f':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_F, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'g':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_G, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'h':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_H, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'i':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_I, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'j':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_J, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'k':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_K, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'l':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_L, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'm':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_M, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'n':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_N, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'o':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_O, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'p':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_P, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'q':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_Q, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'r':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_R, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 's':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_S, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 't':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_T, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'u':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_U, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'v':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_V, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'w':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_W, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'x':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_X, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'y':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_Y, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case 'z':
    {
        drawCharacter(currX, currY, ASCII_LOWER_CASE_Z, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '{':
    {
        drawCharacter(currX, currY, ASCII_LEFT_CURLY_BRACKET, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '|':
    {
        drawCharacter(currX, currY, ASCII_VERTICAL_BAR, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '}':
    {
        drawCharacter(currX, currY, ASCII_RIGHT_CURLY_BRACKET, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '~':
    {
        drawCharacter(currX, currY, ASCII_TILDE, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }

    case '\n':
    {
        currX = initialX;
        currY -= (ASCII_HEIGHT + 1)  * yScale;
        break;
    }

    default:
    {
        drawCharacter(currX, currY, ASCII_UNDEFINED, color, xScale, yScale);
        currX += (ASCII_WIDTH + 1)  * xScale;
        break;
    }
    }
}


void ScreenBuffer::drawLine3DEx(const Point & p0, const Point & p1)
{
    int borderHeight = m_info.bmiHeader.biHeight - borderOffset * 2;
    int borderWidth = m_info.bmiHeader.biWidth - borderOffset * 2;

    int x0, y0, x1, y1;
    if (borderWidth >= borderHeight)
    {
        x0 = borderOffset + (long long)((p0.x + 1) / 2 * borderWidth);
        y0 = borderOffset + (long long)((p0.y + 1) / 2 * borderWidth) - (borderWidth - borderHeight) / 2;
        x1 = borderOffset + (long long)((p1.x + 1) / 2 * borderWidth);
        y1 = borderOffset + (long long)((p1.y + 1) / 2 * borderWidth) - (borderWidth - borderHeight) / 2;
    }
    else
    {
        x0 = borderOffset + (long long)((p0.x + 1) / 2 * borderHeight) - (borderHeight - borderWidth) / 2;
        y0 = borderOffset + (long long)((p0.y + 1) / 2 * borderHeight);
        x1 = borderOffset + (long long)((p1.x + 1) / 2 * borderHeight) - (borderHeight - borderWidth) / 2;
        y1 = borderOffset + (long long)((p1.y + 1) / 2 * borderHeight);
    }

    // NOTE!!!
    // When things are going towards the camera and off to the sides, they end
    // up with huge x and y values. This can be avoided with propper clipping
    // prior to drawing. Without clipping, the range of x and y values that
    // need to be covered is huge, and this method takes forever!!!

    int lineLength = sqrt((float)(x1 - x0) * (x1 - x0) + (float)(y1 - y0) * (y1 - y0));
    float deltaZ = p1.z - p0.z;
    float deltaX = x1 - x0; // positive if line drawn from x0 to x1
    float deltaY = y1 - y0; // positive if line drawn from y0 to y1
    int xIncrement = SIGN(x1 - x0);
    int yIncrement = SIGN(y1 - y0);

    if (equalULP(deltaY, 0))
    {
        for (int x = x0; x != x1; x += xIncrement)
        {
            // NOTE: Convert to float early because these values will overflow
            //       the integer cap and you'll end up trying to take the square
            //       root of a negative.
            float currentLength = sqrt((float)(x - x0) * (x - x0));
            float currentZ = p0.z + deltaZ * currentLength / lineLength;
            Color currentColor = p0.m_color.getFade(1 - currentLength / lineLength) + p1.m_color.getFade(currentLength / lineLength);
            drawPointWithZCheck(x, y0, currentZ, currentColor);
        }
    }
    else if (equalULP(deltaX, 0))
    {
        for (int y = y0; y != y1; y += yIncrement)
        {
            float currentLength = sqrt((float)(y - y0) * (y - y0));
            float currentZ = p0.z + deltaZ * currentLength / lineLength;
            Color currentColor = p0.m_color.getFade(1 - currentLength / lineLength) + p1.m_color.getFade(currentLength / lineLength);
            drawPointWithZCheck(x0, y, currentZ, currentColor);
        }
    }
    else
    {
        float xError = 0;
        float yError = 0;
        int x = x0;
        int y = y0;
        while (x != x1 || y != y1)
        {
            // error will grow faster for whichever is delta is bigger
            xError += ABS(deltaX / deltaY);
            yError += ABS(deltaY / deltaX);
            if (xError >= 0.5 && yError >= 0.5)
            {
                float currentLength = sqrt((float)(x - x0) * (x - x0) + (float)(y - y0) * (y - y0));
                float currentZ = p0.z + deltaZ * currentLength / lineLength;
                Color currentColor = p0.m_color.getFade(1 - currentLength / lineLength) + p1.m_color.getFade(currentLength / lineLength);
                drawPointWithZCheck(x, y, currentZ, currentColor);
                x += xIncrement;
                xError--;
                y += yIncrement;
                yError--;
            }
            else if (xError >= 0.5)
            {
                float currentLength = sqrt((float)(x - x0) * (x - x0) + (float)(y - y0) * (y - y0));
                float currentZ = p0.z + deltaZ * currentLength / lineLength;
                Color currentColor = p0.m_color.getFade(1 - currentLength / lineLength) + p1.m_color.getFade(currentLength / lineLength);
                drawPointWithZCheck(x, y, currentZ, currentColor);
                x += xIncrement;
                xError--;
            }
            else if (yError >= 0.5)
            {
                float currentLength = sqrt((float)(x - x0) * (x - x0) + (float)(y - y0) * (y - y0));
                float currentZ = p0.z + deltaZ * currentLength / lineLength;
                Color currentColor = p0.m_color.getFade(1 - currentLength / lineLength) + p1.m_color.getFade(currentLength / lineLength);
                drawPointWithZCheck(x, y, currentZ, currentColor);
                y += yIncrement;
                yError--;
            }
        }
    }
}


void ScreenBuffer::applySimpleLighting(const Point & source, float ambiant, float diffuse, Shape & shape) const
{
    for (int i = 0; i < shape.triangles.size(); i++)
    {
        Vector n = shape.triangles[i].getNormal();
        Point & p0 = shape.triangles[i].p0;
        Point & p1 = shape.triangles[i].p1;
        Point & p2 = shape.triangles[i].p2;

        Vector l0 = source - p0;
        Vector l1 = source - p1;
        Vector l2 = source - p2;

        l0.normalize();
        l1.normalize();
        l2.normalize();

        float nDotL0 = n.dotProduct(l0);
        float nDotL1 = n.dotProduct(l1);
        float nDotL2 = n.dotProduct(l2);

        if (nDotL0 < 0) nDotL0 = 0;
        if (nDotL1 < 0) nDotL1 = 0;
        if (nDotL2 < 0) nDotL2 = 0;

        p0.m_color = p0.m_color.getFade(ambiant + diffuse * nDotL0);
        p1.m_color = p1.m_color.getFade(ambiant + diffuse * nDotL1);
        p2.m_color = p2.m_color.getFade(ambiant + diffuse * nDotL2);
    }
}


void ScreenBuffer::applySimpleDistanceShading(Shape & shape) const
{
    Array<Point> & points = shape.points;
    Array<Line> & lines = shape.lines;
    Array<Triangle> & triangles = shape.triangles;

    for (int i = 0; i < points.size(); i++)
    {
        Point & p = points[i];
        p.m_color = p.m_color.getFade(-12.0 / p.z);
    }

    for (int i = 0; i < lines.size(); i++)
    {
        Point & p0 = lines[i].p0;
        p0.m_color = p0.m_color.getFade(-12.0 / p0.z);

        Point & p1 = lines[i].p1;
        p1.m_color = p1.m_color.getFade(-12.0 / p1.z);
    }

    for (int i = 0; i < triangles.size(); i++)
    {
        Point & p0 = triangles[i].p0;
        p0.m_color = p0.m_color.getFade(-12.0 / p0.z);

        Point & p1 = triangles[i].p1;
        p1.m_color = p1.m_color.getFade(-12.0 / p1.z);

        Point & p2 = triangles[i].p2;
        p2.m_color = p2.m_color.getFade(-12.0 / p2.z);
    }
}


void ScreenBuffer::cullBackfaces(Shape & shape) const
{
    for (int i = 0; i < shape.triangles.size();)
    {
        Vector n = shape.triangles[i].getNormal();

        Vector l = Point(0, 0, 0) - shape.triangles[i].p0;

        if (n.dotProduct(l) <= 0)
        {
            shape.triangles.remove(i);
        }
        else
        {
            i++;
        }
    }
}


void ScreenBuffer::moveOffCameraLocation(Shape & shape) const
{
    for (int i = 0; i < shape.triangles.size(); i++)
    {

        if (equalULP(shape.triangles[i].p0.z, 0)) shape.triangles[i].p0.z = -0.001;
        if (equalULP(shape.triangles[i].p1.z, 0)) shape.triangles[i].p1.z = -0.001;
        if (equalULP(shape.triangles[i].p2.z, 0)) shape.triangles[i].p2.z = -0.001;
    }

    for (int i = 0; i < shape.lines.size(); i++)
    {
        if (equalULP(shape.lines[i].p0.z, 0)) shape.lines[i].p0.z = -0.001;
        if (equalULP(shape.lines[i].p1.z, 0)) shape.lines[i].p1.z = -0.001;
    }

    for (int i = 0; i < shape.points.size(); i++)
    {
        if (equalULP(shape.points[i].z, 0)) shape.points[i].z = -0.001;
    }
}


void ScreenBuffer::clipBehindCamera(Shape & shape) const
{
    int triangleCount = shape.triangles.size();
    for (int i = 0; i < triangleCount;)
    {
        // clip triangles
        Point & p0 = shape.triangles[i].p0;
        Point & p1 = shape.triangles[i].p1;
        Point & p2 = shape.triangles[i].p2;

        if (p0.z > 0)
        {
            if (p1.z > 0)
            {
                if (p2.z > 0)
                {
                    // all vertices need to be clipped
                    shape.triangles.remove(i);
                    triangleCount--;
                }
                else
                {
                    // p0 and p1 need to be clipped

                    // calculating a new point between p0 and p2
                    // pNew0.x = p0.x + t * (p2.x - p0.x)
                    // pNew0.y = p0.y + t * (p2.y - p0.y)
                    // pNew0.z = p0.z + t * (p2.z - p0.z)
                    // pNew0.z = 0;
                    // t is positive because p0.z > 0 && p2.z <= 0
                    float t02 = p0.z / (p0.z - p2.z);
                    float x02 = p0.x + t02 * (p2.x - p0.x);
                    float y02 = p0.y + t02 * (p2.y - p0.y);
                    Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                    // calculating a new point between p1 and p2
                    // t is positive because p1.z > 0 && p2.z <= 0
                    float t12 = p1.z / (p1.z - p2.z);
                    float x12 = p1.x + t12 * (p2.x - p1.x);
                    float y12 = p1.y + t12 * (p2.y - p1.y);
                    Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                    Triangle newT(Point(x02, y02, 0, c02), Point(x12, y12, 0, c12), p2);
                    shape.triangles[i] = newT;
                    i++;
                }
            }
            else if (p2.z > 0)
            {
                // p0 and p2 need to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.z > 0 && p1.z <= 0
                float t01 = p0.z / (p0.z - p1.z);
                float x01 = p0.x + t01 * (p1.x - p0.x);
                float y01 = p0.y + t01 * (p1.y - p0.y);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p2 and p1
                // t is positive because p2.z > 0 && p1.z <= 0
                float t21 = p2.z / (p2.z - p1.z);
                float x21 = p2.x + t21 * (p1.x - p2.x);
                float y21 = p2.y + t21 * (p1.y - p2.y);
                Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

                Triangle newT(Point(x01, y01, 0, c01), p1, Point(x21, y21, 0, c21));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p0 needs to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.z > 0 && p1.z <= 0
                float t01 = p0.z / (p0.z - p1.z);
                float x01 = p0.x + t01 * (p1.x - p0.x);
                float y01 = p0.y + t01 * (p1.y - p0.y);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p0 and p2
                // t is positive because p0.z > 0 && p2.z <= 0
                float t02 = p0.z / (p0.z - p2.z);
                float x02 = p0.x + t02 * (p2.x - p0.x);
                float y02 = p0.y + t02 * (p2.y - p0.y);
                Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                Triangle newTb(p1, p2, Point(x02, y02, 0, c02));
                shape.triangles += newTb;
                Triangle newTa(p1, Point(x02, y02, 0, c02), Point(x01, y01, 0, c01));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p1.z > 0)
        {
            if (p2.z > 0)
            {
                // p1 and p2 need to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.z > 0 && p0.z <= 0
                float t10 = p1.z / (p1.z - p0.z);
                float x10 = p1.x + t10 * (p0.x - p1.x);
                float y10 = p1.y + t10 * (p0.y - p1.y);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p2 and p0
                // t is positive because p2.z > 0 && p0.z <= 0
                float t20 = p2.z / (p2.z - p0.z);
                float x20 = p2.x + t20 * (p0.x - p2.x);
                float y20 = p2.y + t20 * (p0.y - p2.y);
                Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

                Triangle newT(p0, Point(x10, y10, 0, c10), Point(x20, y20, 0, c20));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p1 needs to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.z > 0 && p0.z <= 0
                float t10 = p1.z / (p1.z - p0.z);
                float x10 = p1.x + t10 * (p0.x - p1.x);
                float y10 = p1.y + t10 * (p0.y - p1.y);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p1 and p1
                // t is positive because p1.z > 0 && p2.z <= 0
                float t12 = p1.z / (p1.z - p2.z);
                float x12 = p1.x + t12 * (p2.x - p1.x);
                float y12 = p1.y + t12 * (p2.y - p1.y);
                Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                Triangle newTb(p2, p0, Point(x12, y12, 0, c12));
                shape.triangles += newTb;
                Triangle newTa(p0, Point(x10, y10, 0, c10), Point(x12, y12, 0, c12));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p2.z > 0)
        {
            // just p2 needs to be clipped

            // calculating a new point between p2 and p0
            // t is positive because p2.z > 0 && p0.z <= 0
            float t20 = p2.z / (p2.z - p0.z);
            float x20 = p2.x + t20 * (p0.x - p2.x);
            float y20 = p2.y + t20 * (p0.y - p2.y);
            Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

            // calculating a new point between p2 and p1
            // t is positive because p2.z > 0 && p1.z <= 0
            float t21 = p2.z / (p2.z - p1.z);
            float x21 = p2.x + t21 * (p1.x - p2.x);
            float y21 = p2.y + t21 * (p1.y - p2.y);
            Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

            Triangle newTb(p0, p1, Point(x21, y21, 0, c21));
            shape.triangles += newTb;
            Triangle newTa(p0, Point(x21, y21, 0, c21), Point(x20, y20, 0, c20));
            shape.triangles[i] = newTa;
            i++;
        }
        else
        {
            // triangle doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.lines.size();)
    {
        Point & p0 = shape.lines[i].p0;
        Point & p1 = shape.lines[i].p1;

        // clip lines
        if (p0.z > 0)
        {
            if (p1.z > 0)
            {
                // both ends of the line need to be clipped
                shape.lines.remove(i);
            }
            else
            {
                // p0 end of the line needs to be clipped
                // 
                // pNew.x = p0.x + t * (p1.x - p0.x)
                // pNew.y = p0.y + t * (p1.y - p0.y)
                // pNew.z = p0.z + t * (p1.z - p0.z)
                // pNew.z = 0
                // t is positive because p0.z > 0 && p1.z <= 0
                float t = p0.z / (p0.z - p1.z);
                float nX = p0.x + t * (p1.x - p0.x);
                float nY = p0.y + t * (p1.y - p0.y);
                Color nC = p0.m_color.getFade(1 - t) + p1.m_color.getFade(t);

                Line newL(Point(nX, nY, 0, nC), p1);
                shape.lines[i] = newL;
                i++;
            }
        }
        else if (p1.z > 0)
        {
            // p1 end of the line needs to be clipped
            float t = p1.z / (p1.z - p0.z);
            float nX = p1.x + t * (p0.x - p1.x);
            float nY = p1.y + t * (p0.y - p1.y);
            Color nC = p1.m_color.getFade(1 - t) + p0.m_color.getFade(t);

            Line newL(p0, Point(nX, nY, 0, nC));
            shape.lines[i] = newL;
            i++;
        }
        else
        {
            // line doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.points.size();)
    {
        // clip near points
        if (shape.points[i].z > 0)/// || equalULP(object.m_points[i].z, 0))
        {
            shape.points.remove(i);
        }
        else
        {
            i++;
        }
    }
}


void ScreenBuffer::clipLeft(Shape & shape) const
{
    int triangleCount = shape.triangles.size();
    for (int i = 0; i < triangleCount;)
    {
        // clip triangles
        Point & p0 = shape.triangles[i].p0;
        Point & p1 = shape.triangles[i].p1;
        Point & p2 = shape.triangles[i].p2;

        if (p0.x < -1)
        {
            if (p1.x < -1)
            {
                if (p2.x < -1)
                {
                    // all vertices need to be clipped
                    shape.triangles.remove(i);
                    triangleCount--;
                }
                else
                {
                    // p0 and p1 need to be clipped

                    // calculating a new point between p0 and p2;
                    // t is positive because p0.x < -1 && p2.x >= -1
                    float t02 = (p0.x + 1) / (p0.x - p2.x);
                    float y02 = p0.y + t02 * (p2.y - p0.y);
                    float z02 = p0.z + t02 * (p2.z - p0.z);
                    Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                    // calculating a new point between p1 and p2
                    // t is positive because p1.x < -1 && p2.x >= -1
                    float t12 = (p1.x + 1) / (p1.x - p2.x);
                    float y12 = p1.y + t12 * (p2.y - p1.y);
                    float z12 = p1.z + t12 * (p2.z - p1.z);
                    Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                    Triangle newT(Point(-1, y02, z02, c02), Point(-1, y12, z12, c12), p2);
                    shape.triangles[i] = newT;
                    i++;
                }
            }
            else if (p2.x < -1)
            {
                // p0 and p2 need to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.x < -1 && p1.x >= -1
                float t01 = (p0.x + 1) / (p0.x - p1.x);
                float y01 = p0.y + t01 * (p1.y - p0.y);
                float z01 = p0.z + t01 * (p1.z - p0.z);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p2 and p1
                // t is positive because p2.x < -1 && p1.x >= -1
                float t21 = (p2.x + 1) / (p2.x - p1.x);
                float y21 = p2.y + t21 * (p1.y - p2.y);
                float z21 = p2.z + t21 * (p1.z - p2.z);
                Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

                Triangle newT(Point(-1, y01, z01, c01), p1, Point(-1, y21, z21, c21));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p0 needs to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.x < -1 && p1.x >= -1
                float t01 = (p0.x + 1) / (p0.x - p1.x);
                float y01 = p0.y + t01 * (p1.y - p0.y);
                float z01 = p0.z + t01 * (p1.z - p0.z);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p0 and p2;
                // t is positive because p0.x < -1 && p2.x >= -1
                float t02 = (p0.x + 1) / (p0.x - p2.x);
                float y02 = p0.y + t02 * (p2.y - p0.y);
                float z02 = p0.z + t02 * (p2.z - p0.z);
                Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                Triangle newTb(p1, p2, Point(-1, y02, z02, c02));
                shape.triangles += newTb;
                Triangle newTa(p1, Point(-1, y02, z02, c02), Point(-1, y01, z01, c01));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p1.x < -1)
        {
            if (p2.x < -1)
            {
                // p1 and p2 need to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.x < -1 && p0.x >= -1
                float t10 = (p1.x + 1) / (p1.x - p0.x);
                float y10 = p1.y + t10 * (p0.y - p1.y);
                float z10 = p1.z + t10 * (p0.z - p1.z);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p2 and p0
                // t is positive because p2.x < -1 && p0.x >= -1
                float t20 = (p2.x + 1) / (p2.x - p0.x);
                float y20 = p2.y + t20 * (p0.y - p2.y);
                float z20 = p2.z + t20 * (p0.z - p2.z);
                Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

                Triangle newT(p0, Point(-1, y10, z10, c10), Point(-1, y20, z20, c20));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p1 needs to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.x < -1 && p0.x >= -1
                float t10 = (p1.x + 1) / (p1.x - p0.x);
                float y10 = p1.y + t10 * (p0.y - p1.y);
                float z10 = p1.z + t10 * (p0.z - p1.z);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p1 and p2
                // t is positive because p1.x < -1 && p2.x >= -1
                float t12 = (p1.x + 1) / (p1.x - p2.x);
                float y12 = p1.y + t12 * (p2.y - p1.y);
                float z12 = p1.z + t12 * (p2.z - p1.z);
                Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                Triangle newTb(p2, p0, Point(-1, y12, z12, c12));
                shape.triangles += newTb;
                Triangle newTa(p0, Point(-1, y10, z10, c10), Point(-1, y12, z12, c12));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p2.x < -1)
        {
            // just p2 needs to be clipped

            // calculating a new point between p2 and p0
            // t is positive because p2.x < -1 && p0.x >= -1
            float t20 = (p2.x + 1) / (p2.x - p0.x);
            float y20 = p2.y + t20 * (p0.y - p2.y);
            float z20 = p2.z + t20 * (p0.z - p2.z);
            Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

            // calculating a new point between p2 and p1
            // t is positive because p2.x < -1 && p1.x >= -1
            float t21 = (p2.x + 1) / (p2.x - p1.x);
            float y21 = p2.y + t21 * (p1.y - p2.y);
            float z21 = p2.z + t21 * (p1.z - p2.z);
            Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

            Triangle newTb(p0, p1, Point(-1, y21, z21, c21));
            shape.triangles += newTb;
            Triangle newTa(p0, Point(-1, y21, z21, c21), Point(-1, y20, z20, c20));
            shape.triangles[i] = newTa;
            i++;
        }
        else
        {
            // triangle doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.lines.size();)
    {
        Point & p0 = shape.lines[i].p0;
        Point & p1 = shape.lines[i].p1;

        // clip lines
        if (p0.x < -1)
        {
            if (p1.x < -1)
            {
                // both ends of the line need to be clipped
                shape.lines.remove(i);
            }
            else
            {
                // p0 end of the line needs to be clipped
                // 
                // pNew.x = p0.x + t * (p1.x - p0.x)
                // pNew.y = p0.y + t * (p1.y - p0.y)
                // pNew.z = p0.z + t * (p1.z - p0.z)
                // pNew.x = -1
                // t is positive because p0.x < -1 && p1.x >= -1
                float t = (-1 - p0.x) / (p1.x - p0.x);
                float nY = p0.y + t * (p1.y - p0.y);
                float nZ = p0.z + t * (p1.z - p0.z);
                Color nC = p0.m_color.getFade(1 - t) + p1.m_color.getFade(t);

                Line newL(Point(-1, nY, nZ, nC), p1);
                shape.lines[i] = newL;
                i++;
            }
        }
        else if (p1.x < -1)
        {
            // p1 end of the line needs to be clipped
            float t = (-1 - p1.x) / (p0.x - p1.x);
            float nY = p1.y + t * (p0.y - p1.y);
            float nZ = p1.z + t * (p0.z - p1.z);
            Color nC = p1.m_color.getFade(1 - t) + p0.m_color.getFade(t);

            Line newL(p0, Point(-1, nY, nZ, nC));
            shape.lines[i] = newL;
            i++;
        }
        else
        {
            // line doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.points.size();)
    {
        // clip near points
        if (shape.points[i].x < -1)
        {
            shape.points.remove(i);
        }
        else
        {
            i++;
        }
    }
}


void ScreenBuffer::clipRight(Shape & shape) const
{
    int triangleCount = shape.triangles.size();
    for (int i = 0; i < triangleCount;)
    {
        // clip triangles
        Point & p0 = shape.triangles[i].p0;
        Point & p1 = shape.triangles[i].p1;
        Point & p2 = shape.triangles[i].p2;

        if (p0.x > 1)
        {
            if (p1.x > 1)
            {
                if (p2.x > 1)
                {
                    // all vertices need to be clipped
                    shape.triangles.remove(i);
                    triangleCount--;
                }
                else
                {
                    // p0 and p1 need to be clipped

                    // calculating a new point between p0 and p2;
                    // t is positive because p0.x > 1 && p2.x <= 1
                    float t02 = (p0.x - 1) / (p0.x - p2.x);
                    float y02 = p0.y + t02 * (p2.y - p0.y);
                    float z02 = p0.z + t02 * (p2.z - p0.z);
                    Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                    // calculating a new point between p1 and p2
                    // t is positive because p1.x > 1 && p2.x <= 1
                    float t12 = (p1.x - 1) / (p1.x - p2.x);
                    float y12 = p1.y + t12 * (p2.y - p1.y);
                    float z12 = p1.z + t12 * (p2.z - p1.z);
                    Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                    Triangle newT(Point(1, y02, z02, c02), Point(1, y12, z12, c12), p2);
                    shape.triangles[i] = newT;
                    i++;
                }
            }
            else if (p2.x > 1)
            {
                // p0 and p2 need to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.x > 1 && p1.x <= 1
                float t01 = (p0.x - 1) / (p0.x - p1.x);
                float y01 = p0.y + t01 * (p1.y - p0.y);
                float z01 = p0.z + t01 * (p1.z - p0.z);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p2 and p1
                // t is positive because p2.x > 1 && p1.x <= 1
                float t21 = (p2.x - 1) / (p2.x - p1.x);
                float y21 = p2.y + t21 * (p1.y - p2.y);
                float z21 = p2.z + t21 * (p1.z - p2.z);
                Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

                Triangle newT(Point(1, y01, z01, c01), p1, Point(1, y21, z21, c21));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p0 needs to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.x > 1 && p1.x <= 1
                float t01 = (p0.x - 1) / (p0.x - p1.x);
                float y01 = p0.y + t01 * (p1.y - p0.y);
                float z01 = p0.z + t01 * (p1.z - p0.z);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p0 and p2;
                // t is positive because p0.x > 1 && p2.x <= 1
                float t02 = (p0.x - 1) / (p0.x - p2.x);
                float y02 = p0.y + t02 * (p2.y - p0.y);
                float z02 = p0.z + t02 * (p2.z - p0.z);
                Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                Triangle newTb(p1, p2, Point(1, y02, z02, c02));
                shape.triangles += newTb;
                Triangle newTa(p1, Point(1, y02, z02, c02), Point(1, y01, z01, c01));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p1.x > 1)
        {
            if (p2.x > 1)
            {
                // p1 and p2 need to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.x > 1 && p0.x <= 1
                float t10 = (p1.x - 1) / (p1.x - p0.x);
                float y10 = p1.y + t10 * (p0.y - p1.y);
                float z10 = p1.z + t10 * (p0.z - p1.z);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p2 and p0
                // t is positive because p2.x > 1 && p0.x <= 1
                float t20 = (p2.x - 1) / (p2.x - p0.x);
                float y20 = p2.y + t20 * (p0.y - p2.y);
                float z20 = p2.z + t20 * (p0.z - p2.z);
                Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

                Triangle newT(p0, Point(1, y10, z10, c10), Point(1, y20, z20, c20));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p1 needs to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.x > 1 && p0.x <= 1
                float t10 = (p1.x - 1) / (p1.x - p0.x);
                float y10 = p1.y + t10 * (p0.y - p1.y);
                float z10 = p1.z + t10 * (p0.z - p1.z);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p1 and p2
                // t is positive because p1.x > 1 && p2.x <= 1
                float t12 = (p1.x - 1) / (p1.x - p2.x);
                float y12 = p1.y + t12 * (p2.y - p1.y);
                float z12 = p1.z + t12 * (p2.z - p1.z);
                Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                Triangle newTb(p2, p0, Point(1, y12, z12, c12));
                shape.triangles += newTb;
                Triangle newTa(p0, Point(1, y10, z10, c10), Point(1, y12, z12, c12));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p2.x > 1)
        {
            // just p2 needs to be clipped

            // calculating a new point between p2 and p0
            // t is positive because p2.x > 1 && p0.x <= 1
            float t20 = (p2.x - 1) / (p2.x - p0.x);
            float y20 = p2.y + t20 * (p0.y - p2.y);
            float z20 = p2.z + t20 * (p0.z - p2.z);
            Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

            // calculating a new point between p2 and p1
            // t is positive because p2.x > 1 && p1.x <= 1
            float t21 = (p2.x - 1) / (p2.x - p1.x);
            float y21 = p2.y + t21 * (p1.y - p2.y);
            float z21 = p2.z + t21 * (p1.z - p2.z);
            Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

            Triangle newTb(p0, p1, Point(1, y21, z21, c21));
            shape.triangles += newTb;
            Triangle newTa(p0, Point(1, y21, z21, c21), Point(1, y20, z20, c20));
            shape.triangles[i] = newTa;
            i++;
        }
        else
        {
            // triangle doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.lines.size();)
    {
        Point & p0 = shape.lines[i].p0;
        Point & p1 = shape.lines[i].p1;

        // clip lines
        if (p0.x > 1)
        {
            if (p1.x > 1)
            {
                // both ends of the line need to be clipped
                shape.lines.remove(i);
            }
            else
            {
                // p0 end of the line needs to be clipped
                // 
                // pNew.x = p0.x + t * (p1.x - p0.x)
                // pNew.y = p0.y + t * (p1.y - p0.y)
                // pNew.z = p0.z + t * (p1.z - p0.z)
                // pNew.x = 1
                // t is positive because p0.x > 1 && p1.x <= 1
                float t = (p0.x - 1) / (p0.x - p1.x);
                float nY = p0.y + t * (p1.y - p0.y);
                float nZ = p0.z + t * (p1.z - p0.z);
                Color nC = p0.m_color.getFade(1 - t) + p1.m_color.getFade(t);

                Line newL(Point(1, nY, nZ, nC), p1);
                shape.lines[i] = newL;
                i++;
            }
        }
        else if (p1.x > 1)
        {
            // p1 end of the line needs to be clipped
            float t = (p1.x - 1) / (p1.x - p0.x);
            float nY = p1.y + t * (p0.y - p1.y);
            float nZ = p1.z + t * (p0.z - p1.z);
            Color nC = p1.m_color.getFade(1 - t) + p0.m_color.getFade(t);

            Line newL(p0, Point(1, nY, nZ, nC));
            shape.lines[i] = newL;
            i++;
        }
        else
        {
            // line doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.points.size();)
    {
        // clip near points
        if (shape.points[i].x > 1)
        {
            shape.points.remove(i);
        }
        else
        {
            i++;
        }
    }
}


void ScreenBuffer::clipBottom(Shape & shape) const
{
    int triangleCount = shape.triangles.size();
    for (int i = 0; i < triangleCount;)
    {
        // clip triangles
        Point & p0 = shape.triangles[i].p0;
        Point & p1 = shape.triangles[i].p1;
        Point & p2 = shape.triangles[i].p2;

        if (p0.y < -1)
        {
            if (p1.y < -1)
            {
                if (p2.y < -1)
                {
                    // all vertices need to be clipped
                    shape.triangles.remove(i);
                    triangleCount--;
                }
                else
                {
                    // p0 and p1 need to be clipped

                    // calculating a new point between p0 and p2;
                    // t is positive because p0.y < -1 && p2.y >= -1
                    float t02 = (p0.y + 1) / (p0.y - p2.y);
                    float x02 = p0.x + t02 * (p2.x - p0.x);
                    float z02 = p0.z + t02 * (p2.z - p0.z);
                    Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                    // calculating a new point between p1 and p2
                    // t is positive because p1.y < -1 && p2.y >= -1
                    float t12 = (p1.y + 1) / (p1.y - p2.y);
                    float x12 = p1.x + t12 * (p2.x - p1.x);
                    float z12 = p1.z + t12 * (p2.z - p1.z);
                    Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                    Triangle newT(Point(x02, -1, z02, c02), Point(x12, -1, z12, c12), p2);
                    shape.triangles[i] = newT;
                    i++;
                }
            }
            else if (p2.y < -1)
            {
                // p0 and p2 need to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.y < -1 && p1.y >= -1
                float t01 = (p0.y + 1) / (p0.y - p1.y);
                float x01 = p0.x + t01 * (p1.x - p0.x);
                float z01 = p0.z + t01 * (p1.z - p0.z);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p2 and p1
                // t is positive because p2.y < -1 && p1.y >= -1
                float t21 = (p2.y + 1) / (p2.y - p1.y);
                float x21 = p2.x + t21 * (p1.x - p2.x);
                float z21 = p2.z + t21 * (p1.z - p2.z);
                Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

                Triangle newT(Point(x01, -1, z01, c01), p1, Point(x21, -1, z21, c21));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p0 needs to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.y < -1 && p1.y >= -1
                float t01 = (p0.y + 1) / (p0.y - p1.y);
                float x01 = p0.x + t01 * (p1.x - p0.x);
                float z01 = p0.z + t01 * (p1.z - p0.z);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p0 and p2;
                // t is positive because p0.y < -1 && p2.y >= -1
                float t02 = (p0.y + 1) / (p0.y - p2.y);
                float x02 = p0.x + t02 * (p2.x - p0.x);
                float z02 = p0.z + t02 * (p2.z - p0.z);
                Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                Triangle newTb(p1, p2, Point(x02, -1, z02, c02));
                shape.triangles += newTb;
                Triangle newTa(p1, Point(x02, -1, z02, c02), Point(x01, -1, z01, c01));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p1.y < -1)
        {
            if (p2.y < -1)
            {
                // p1 and p2 need to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.y < -1 && p0.y >= -1
                float t10 = (p1.y + 1) / (p1.y - p0.y);
                float x10 = p1.x + t10 * (p0.x - p1.x);
                float z10 = p1.z + t10 * (p0.z - p1.z);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p2 and p0
                // t is positive because p2.y < -1 && p0.y >= -1
                float t20 = (p2.y + 1) / (p2.y - p0.y);
                float x20 = p2.x + t20 * (p0.x - p2.x);
                float z20 = p2.z + t20 * (p0.z - p2.z);
                Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

                Triangle newT(p0, Point(x10, -1, z10, c10), Point(x20, -1, z20, c20));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p1 needs to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.y < -1 && p0.y >= -1
                float t10 = (p1.y + 1) / (p1.y - p0.y);
                float x10 = p1.x + t10 * (p0.x - p1.x);
                float z10 = p1.z + t10 * (p0.z - p1.z);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p1 and p2
                // t is positive because p1.y < -1 && p2.y >= -1
                float t12 = (p1.y + 1) / (p1.y - p2.y);
                float x12 = p1.x + t12 * (p2.x - p1.x);
                float z12 = p1.z + t12 * (p2.z - p1.z);
                Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                Triangle newTb(p2, p0, Point(x12, -1, z12, c12));
                shape.triangles += newTb;
                Triangle newTa(p0, Point(x10, -1, z10, c10), Point(x12, -1, z12, c12));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p2.y < -1)
        {
            // just p2 needs to be clipped

            // calculating a new point between p2 and p0
            // t is positive because p2.y < -1 && p0.y >= -1
            float t20 = (p2.y + 1) / (p2.y - p0.y);
            float x20 = p2.x + t20 * (p0.x - p2.x);
            float z20 = p2.z + t20 * (p0.z - p2.z);
            Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

            // calculating a new point between p2 and p1
            // t is positive because p2.y < -1 && p1.y >= -1
            float t21 = (p2.y + 1) / (p2.y - p1.y);
            float x21 = p2.x + t21 * (p1.x - p2.x);
            float z21 = p2.z + t21 * (p1.z - p2.z);
            Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

            Triangle newTb(p0, p1, Point(x21, -1, z21, c21));
            shape.triangles += newTb;
            Triangle newTa(p0, Point(x21, -1, z21, c21), Point(x20, -1, z20, c20));
            shape.triangles[i] = newTa;
            i++;
        }
        else
        {
            // triangle doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.lines.size();)
    {
        Point & p0 = shape.lines[i].p0;
        Point & p1 = shape.lines[i].p1;

        // clip lines
        if (p0.y < -1)
        {
            if (p1.y < -1)
            {
                // both ends of the line need to be clipped
                shape.lines.remove(i);
            }
            else
            {
                // p0 end of the line needs to be clipped
                // 
                // pNew.x = p0.x + t * (p1.x - p0.x)
                // pNew.y = p0.y + t * (p1.y - p0.y)
                // pNew.z = p0.z + t * (p1.z - p0.z)
                // pNew.y = -1
                // t is positive because p0.y < -1 && p1.y >= -1
                float t = (-1 - p0.y) / (p1.y - p0.y);
                float nX = p0.x + t * (p1.x - p0.x);
                float nZ = p0.z + t * (p1.z - p0.z);
                Color nC = p0.m_color.getFade(1 - t) + p1.m_color.getFade(t);

                Line newL(Point(nX, -1, nZ, nC), p1);
                shape.lines[i] = newL;
                i++;
            }
        }
        else if (p1.y < -1)
        {
            // p1 end of the line needs to be clipped
            float t = (-1 - p1.y) / (p0.y - p1.y);
            float nX = p1.x + t * (p0.x - p1.x);
            float nZ = p1.z + t * (p0.z - p1.z);
            Color nC = p1.m_color.getFade(1 - t) + p0.m_color.getFade(t);

            Line newL(p0, Point(nX, -1, nZ, nC));
            shape.lines[i] = newL;
            i++;
        }
        else
        {
            // line doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.points.size();)
    {
        // clip near points
        if (shape.points[i].y < -1)
        {
            shape.points.remove(i);
        }
        else
        {
            i++;
        }
    }
}


void ScreenBuffer::clipTop(Shape & shape) const
{
    int triangleCount = shape.triangles.size();
    for (int i = 0; i < triangleCount;)
    {
        // clip triangles
        Point & p0 = shape.triangles[i].p0;
        Point & p1 = shape.triangles[i].p1;
        Point & p2 = shape.triangles[i].p2;

        if (p0.y > 1)
        {
            if (p1.y > 1)
            {
                if (p2.y > 1)
                {
                    // all vertices need to be clipped
                    shape.triangles.remove(i);
                    triangleCount--;
                }
                else
                {
                    // p0 and p1 need to be clipped

                    // calculating a new point between p0 and p2;
                    // t is positive because p0.y > 1 && p2.y <= 1
                    float t02 = (p0.y - 1) / (p0.y - p2.y);
                    float x02 = p0.x + t02 * (p2.x - p0.x);
                    float z02 = p0.z + t02 * (p2.z - p0.z);
                    Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                    // calculating a new point between p1 and p2
                    // t is positive because p1.y > 1 && p2.y <= 1
                    float t12 = (p1.y - 1) / (p1.y - p2.y);
                    float x12 = p1.x + t12 * (p2.x - p1.x);
                    float z12 = p1.z + t12 * (p2.z - p1.z);
                    Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                    Triangle newT(Point(x02, 1, z02, c02), Point(x12, 1, z12, c12), p2);
                    shape.triangles[i] = newT;
                    i++;
                }
            }
            else if (p2.y > 1)
            {
                // p0 and p2 need to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.y > 1 && p1.y <= 1
                float t01 = (p0.y - 1) / (p0.y - p1.y);
                float x01 = p0.x + t01 * (p1.x - p0.x);
                float z01 = p0.z + t01 * (p1.z - p0.z);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p2 and p1
                // t is positive because p2.y > 1 && p1.y <= 1
                float t21 = (p2.y - 1) / (p2.y - p1.y);
                float x21 = p2.x + t21 * (p1.x - p2.x);
                float z21 = p2.z + t21 * (p1.z - p2.z);
                Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

                Triangle newT(Point(x01, 1, z01, c01), p1, Point(x21, 1, z21, c21));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p0 needs to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.y > 1 && p1.y <= 1
                float t01 = (p0.y - 1) / (p0.y - p1.y);
                float x01 = p0.x + t01 * (p1.x - p0.x);
                float z01 = p0.z + t01 * (p1.z - p0.z);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p0 and p2;
                // t is positive because p0.y > 1 && p2.y <= 1
                float t02 = (p0.y - 1) / (p0.y - p2.y);
                float x02 = p0.x + t02 * (p2.x - p0.x);
                float z02 = p0.z + t02 * (p2.z - p0.z);
                Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                Triangle newTb(p1, p2, Point(x02, 1, z02, c02));
                shape.triangles += newTb;
                Triangle newTa(p1, Point(x02, 1, z02, c02), Point(x01, 1, z01, c01));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p1.y > 1)
        {
            if (p2.y > 1)
            {
                // p1 and p2 need to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.y > 1 && p0.y <= 1
                float t10 = (p1.y - 1) / (p1.y - p0.y);
                float x10 = p1.x + t10 * (p0.x - p1.x);
                float z10 = p1.z + t10 * (p0.z - p1.z);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p2 and p0
                // t is positive because p2.y > 1 && p0.y <= 1
                float t20 = (p2.y - 1) / (p2.y - p0.y);
                float x20 = p2.x + t20 * (p0.x - p2.x);
                float z20 = p2.z + t20 * (p0.z - p2.z);
                Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

                Triangle newT(p0, Point(x10, 1, z10, c10), Point(x20, 1, z20, c20));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p1 needs to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.y > 1 && p0.y <= 1
                float t10 = (p1.y - 1) / (p1.y - p0.y);
                float x10 = p1.x + t10 * (p0.x - p1.x);
                float z10 = p1.z + t10 * (p0.z - p1.z);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p1 and p2
                // t is positive because p1.y > 1 && p2.y <= 1
                float t12 = (p1.y - 1) / (p1.y - p2.y);
                float x12 = p1.x + t12 * (p2.x - p1.x);
                float z12 = p1.z + t12 * (p2.z - p1.z);
                Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                Triangle newTb(p2, p0, Point(x12, 1, z12, c12));
                shape.triangles += newTb;
                Triangle newTa(p0, Point(x10, 1, z10, c10), Point(x12, 1, z12, c12));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p2.y > 1)
        {
            // just p2 needs to be clipped

            // calculating a new point between p2 and p0
            // t is positive because p2.y > 1 && p0.y <= 1
            float t20 = (p2.y - 1) / (p2.y - p0.y);
            float x20 = p2.x + t20 * (p0.x - p2.x);
            float z20 = p2.z + t20 * (p0.z - p2.z);
            Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

            // calculating a new point between p2 and p1
            // t is positive because p2.y > 1 && p1.y <= 1
            float t21 = (p2.y - 1) / (p2.y - p1.y);
            float x21 = p2.x + t21 * (p1.x - p2.x);
            float z21 = p2.z + t21 * (p1.z - p2.z);
            Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

            Triangle newTb(p0, p1, Point(x21, 1, z21, c21));
            shape.triangles += newTb;
            Triangle newTa(p0, Point(x21, 1, z21, c21), Point(x20, 1, z20, c20));
            shape.triangles[i] = newTa;
            i++;
        }
        else
        {
            // triangle doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.lines.size();)
    {
        Point & p0 = shape.lines[i].p0;
        Point & p1 = shape.lines[i].p1;

        // clip lines
        if (p0.y > 1)
        {
            if (p1.y > 1)
            {
                // both ends of the line need to be clipped
                shape.lines.remove(i);
            }
            else
            {
                // p0 end of the line needs to be clipped
                // 
                // pNew.x = p0.x + t * (p1.x - p0.x)
                // pNew.y = p0.y + t * (p1.y - p0.y)
                // pNew.z = p0.z + t * (p1.z - p0.z)
                // pNew.y = 1
                // t is positive because p0.y > 1 && p1.y <= 1
                float t = (p0.y - 1) / (p0.y - p1.y);
                float nX = p0.x + t * (p1.x - p0.x);
                float nZ = p0.z + t * (p1.z - p0.z);
                Color nC = p0.m_color.getFade(1 - t) + p1.m_color.getFade(t);

                Line newL(Point(nX, 1, nZ, nC), p1);
                shape.lines[i] = newL;
                i++;
            }
        }
        else if (p1.y > 1)
        {
            // p1 end of the line needs to be clipped
            float t = (p1.y - 1) / (p1.y - p0.y);
            float nX = p1.x + t * (p0.x - p1.x);
            float nZ = p1.z + t * (p0.z - p1.z);
            Color nC = p1.m_color.getFade(1 - t) + p0.m_color.getFade(t);

            Line newL(p0, Point(nX, 1, nZ, nC));
            shape.lines[i] = newL;
            i++;
        }
        else
        {
            // line doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.points.size();)
    {
        // clip near points
        if (shape.points[i].y > 1)
        {
            shape.points.remove(i);
        }
        else
        {
            i++;
        }
    }
}


void ScreenBuffer::clipFar(Shape & shape) const
{
    for (int i = 0; i < shape.triangles.size(); i++)
    {
        int triangleCount = shape.triangles.size();
        for (int i = 0; i < triangleCount;)
        {
            // clip triangles
            Point & p0 = shape.triangles[i].p0;
            Point & p1 = shape.triangles[i].p1;
            Point & p2 = shape.triangles[i].p2;

            if (p0.z < -1)
            {
                if (p1.z < -1)
                {
                    if (p2.z < -1)
                    {
                        // all vertices need to be clipped
                        shape.triangles.remove(i);
                        triangleCount--;
                    }
                    else
                    {
                        // p0 and p1 need to be clipped

                        // calculating a new point between p0 and p2;
                        // t is positive because p0.z < -1 && p2.z >= -1
                        float t02 = (p0.z + 1) / (p0.z - p2.z);
                        float x02 = p0.x + t02 * (p2.x - p0.x);
                        float y02 = p0.y + t02 * (p2.y - p0.y);
                        Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                        // calculating a new point between p1 and p2
                        // t is positive because p1.z < -1 && p2.z >= -1
                        float t12 = (p1.z + 1) / (p1.z - p2.z);
                        float x12 = p1.x + t12 * (p2.x - p1.x);
                        float y12 = p1.y + t12 * (p2.y - p1.y);
                        Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                        Triangle newT(Point(x02, y02, -1, c02), Point(x12, y12, -1, c12), p2);
                        shape.triangles[i] = newT;
                        i++;
                    }
                }
                else if (p2.z < -1)
                {
                    // p0 and p2 need to be clipped

                    // calculating a new point between p0 and p1
                    // t is positive because p0.z < -1 && p1.z >= -1
                    float t01 = (p0.z + 1) / (p0.z - p1.z);
                    float x01 = p0.x + t01 * (p1.x - p0.x);
                    float y01 = p0.y + t01 * (p1.y - p0.y);
                    Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                    // calculating a new point between p2 and p1
                    // t is positive because p2.z < -1 && p1.z >= -1
                    float t21 = (p2.z + 1) / (p2.z - p1.z);
                    float x21 = p2.x + t21 * (p1.x - p2.x);
                    float y21 = p2.y + t21 * (p1.y - p2.y);
                    Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

                    Triangle newT(Point(x01, y01, -1, c01), p1, Point(x21, y21, -1, c21));
                    shape.triangles[i] = newT;
                    i++;
                }
                else
                {
                    // just p0 needs to be clipped

                    // calculating a new point between p0 and p1
                    // t is positive because p0.z < -1 && p1.z >= -1
                    float t01 = (p0.z + 1) / (p0.z - p1.z);
                    float x01 = p0.x + t01 * (p1.x - p0.x);
                    float y01 = p0.y + t01 * (p1.y - p0.y);
                    Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                    // calculating a new point between p0 and p2;
                    // t is positive because p0.z < -1 && p2.z >= -1
                    float t02 = (p0.z + 1) / (p0.z - p2.z);
                    float x02 = p0.x + t02 * (p2.x - p0.x);
                    float y02 = p0.y + t02 * (p2.y - p0.y);
                    Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                    Triangle newTb(p1, p2, Point(x02, y02, -1, c02));
                    shape.triangles += newTb;
                    Triangle newTa(p1, Point(x02, y02, -1, c02), Point(x01, y01, -1, c01));
                    shape.triangles[i] = newTa;
                    i++;
                }
            }
            else if (p1.z < -1)
            {
                if (p2.z < -1)
                {
                    // p1 and p2 need to be clipped

                    // calculating a new point between p1 and p0
                    // t is positive because p1.z < -1 && p0.z >= -1
                    float t10 = (p1.z + 1) / (p1.z - p0.z);
                    float x10 = p1.x + t10 * (p0.x - p1.x);
                    float y10 = p1.y + t10 * (p0.y - p1.y);
                    Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                    // calculating a new point between p2 and p0
                    // t is positive because p2.z < -1 && p0.z >= -1
                    float t20 = (p2.z + 1) / (p2.z - p0.z);
                    float x20 = p2.x + t20 * (p0.x - p2.x);
                    float y20 = p2.y + t20 * (p0.y - p2.y);
                    Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

                    Triangle newT(p0, Point(x10, y10, -1, c10), Point(x20, y20, -1, c20));
                    shape.triangles[i] = newT;
                    i++;
                }
                else
                {
                    // just p1 needs to be clipped

                    // calculating a new point between p1 and p0
                    // t is positive because p1.z < -1 && p0.z >= -1
                    float t10 = (p1.z + 1) / (p1.z - p0.z);
                    float x10 = p1.x + t10 * (p0.x - p1.x);
                    float y10 = p1.y + t10 * (p0.y - p1.y);
                    Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                    // calculating a new point between p1 and p2
                    // t is positive because p1.z < -1 && p2.z >= -1
                    float t12 = (p1.z + 1) / (p1.z - p2.z);
                    float x12 = p1.x + t12 * (p2.x - p1.x);
                    float y12 = p1.y + t12 * (p2.y - p1.y);
                    Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                    Triangle newTb(p2, p0, Point(x12, y12, -1, c12));
                    shape.triangles += newTb;
                    Triangle newTa(p0, Point(x10, y10, -1, c10), Point(x12, y12, -1, c12));
                    shape.triangles[i] = newTa;
                    i++;
                }
            }
            else if (p2.y < -1)
            {
                // just p2 needs to be clipped

                // calculating a new point between p2 and p0
                // t is positive because p2.z < -1 && p0.z >= -1
                float t20 = (p2.z + 1) / (p2.z - p0.z);
                float x20 = p2.x + t20 * (p0.x - p2.x);
                float y20 = p2.y + t20 * (p0.y - p2.y);
                Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

                // calculating a new point between p2 and p1
                // t is positive because p2.z < -1 && p1.z >= -1
                float t21 = (p2.z + 1) / (p2.z - p1.z);
                float x21 = p2.x + t21 * (p1.x - p2.x);
                float y21 = p2.y + t21 * (p1.y - p2.y);
                Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

                Triangle newTb(p0, p1, Point(x21, y21, -1, c21));
                shape.triangles += newTb;
                Triangle newTa(p0, Point(x21, y21, -1, c21), Point(x20, y20, -1, c20));
                shape.triangles[i] = newTa;
                i++;
            }
            else
            {
                // triangle doesn't need to be clipped
                i++;
            }
        }
    }

    for (int i = 0; i < shape.lines.size();)
    {
        Point & p0 = shape.lines[i].p0;
        Point & p1 = shape.lines[i].p1;

        // clip lines
        if (p0.z < -1)
        {
            if (p1.z < -1)
            {
                // both ends of the line need to be clipped
                shape.lines.remove(i);
            }
            else
            {
                // p0 end of the line needs to be clipped
                // 
                // pNew.x = p0.x + t * (p1.x - p0.x)
                // pNew.y = p0.y + t * (p1.y - p0.y)
                // pNew.z = p0.z + t * (p1.z - p0.z)
                // pNew.z = -1
                // t is positive because p0.z < -1 && p1.z >= -1
                float t = (-1 - p0.z) / (p1.z - p0.z);
                float nX = p0.x + t * (p1.x - p0.x);
                float nY = p0.y + t * (p1.y - p0.y);
                Color nC = p0.m_color.getFade(1 - t) + p1.m_color.getFade(t);

                Line newL(Point(nX, nY, -1, nC), p1);
                shape.lines[i] = newL;
                i++;
            }
        }
        else if (p1.z < -1)
        {
            // p1 end of the line needs to be clipped
            float t = (-1 - p1.z) / (p0.z - p1.z);
            float nX = p1.x + t * (p0.x - p1.x);
            float nY = p1.y + t * (p0.y - p1.y);
            Color nC = p1.m_color.getFade(1 - t) + p0.m_color.getFade(t);

            Line newL(p0, Point(nX, nY, -1, nC));
            shape.lines[i] = newL;
            i++;
        }
        else
        {
            // line doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.points.size();)
    {
        // clip near points
        if (shape.points[i].z < -1)
        {
            shape.points.remove(i);
        }
        else
        {
            i++;
        }
    }
}


void ScreenBuffer::clipNear(Shape & shape) const
{
    int triangleCount = shape.triangles.size();
    for (int i = 0; i < triangleCount;)
    {
        // clip triangles
        Point & p0 = shape.triangles[i].p0;
        Point & p1 = shape.triangles[i].p1;
        Point & p2 = shape.triangles[i].p2;

        if (p0.z > 1)
        {
            if (p1.z > 1)
            {
                if (p2.z > 1)
                {
                    // all vertices need to be clipped
                    shape.triangles.remove(i);
                    triangleCount--;
                }
                else
                {
                    // p0 and p1 need to be clipped

                    // calculating a new point between p0 and p2;
                    // t is positive because p0.z > 1 && p2.z <= 1
                    float t02 = (p0.z - 1) / (p0.z - p2.z);
                    float x02 = p0.x + t02 * (p2.x - p0.x);
                    float y02 = p0.y + t02 * (p2.y - p0.y);
                    Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                    // calculating a new point between p1 and p2
                    // t is positive because p1.z > 1 && p2.z <= 1
                    float t12 = (p1.z - 1) / (p1.z - p2.z);
                    float x12 = p1.x + t12 * (p2.x - p1.x);
                    float y12 = p1.y + t12 * (p2.y - p1.y);
                    Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                    Triangle newT(Point(x02, y02, 1, c02), Point(x12, y12, 1, c12), p2);
                    shape.triangles[i] = newT;
                    i++;
                }
            }
            else if (p2.z > 1)
            {
                // p0 and p2 need to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.z > 1 && p1.z <= 1
                float t01 = (p0.z - 1) / (p0.z - p1.z);
                float x01 = p0.x + t01 * (p1.x - p0.x);
                float y01 = p0.y + t01 * (p1.y - p0.y);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p2 and p1
                // t is positive because p2.z > 1 && p1.z <= 1
                float t21 = (p2.z - 1) / (p2.z - p1.z);
                float x21 = p2.x + t21 * (p1.x - p2.x);
                float y21 = p2.y + t21 * (p1.y - p2.y);
                Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

                Triangle newT(Point(x01, y01, 1, c01), p1, Point(x21, y21, 1, c21));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p0 needs to be clipped

                // calculating a new point between p0 and p1
                // t is positive because p0.z > 1 && p1.z <= 1
                float t01 = (p0.z - 1) / (p0.z - p1.z);
                float x01 = p0.x + t01 * (p1.x - p0.x);
                float y01 = p0.y + t01 * (p1.y - p0.y);
                Color c01 = p0.m_color.getFade(1 - t01) + p1.m_color.getFade(t01);

                // calculating a new point between p0 and p2;
                // t is positive because p0.z > 1 && p2.z <= 1
                float t02 = (p0.z - 1) / (p0.z - p2.z);
                float x02 = p0.x + t02 * (p2.x - p0.x);
                float y02 = p0.y + t02 * (p2.y - p0.y);
                Color c02 = p0.m_color.getFade(1 - t02) + p2.m_color.getFade(t02);

                Triangle newTb(p1, p2, Point(x02, y02, 1, c02));
                shape.triangles += newTb;
                Triangle newTa(p1, Point(x02, y02, 1, c02), Point(x01, y01, 1, c01));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p1.z > 1)
        {
            if (p2.z > 1)
            {
                // p1 and p2 need to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.z > 1 && p0.z <= 1
                float t10 = (p1.z - 1) / (p1.z - p0.z);
                float x10 = p1.x + t10 * (p0.x - p1.x);
                float y10 = p1.y + t10 * (p0.y - p1.y);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p2 and p0
                // t is positive because p2.z > 1 && p0.z <= 1
                float t20 = (p2.z - 1) / (p2.z - p0.z);
                float x20 = p2.x + t20 * (p0.x - p2.x);
                float y20 = p2.y + t20 * (p0.y - p2.y);
                Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

                Triangle newT(p0, Point(x10, y10, 1, c10), Point(x20, y20, 1, c20));
                shape.triangles[i] = newT;
                i++;
            }
            else
            {
                // just p1 needs to be clipped

                // calculating a new point between p1 and p0
                // t is positive because p1.z > 1 && p0.z <= 1
                float t10 = (p1.z - 1) / (p1.z - p0.z);
                float x10 = p1.x + t10 * (p0.x - p1.x);
                float y10 = p1.y + t10 * (p0.y - p1.y);
                Color c10 = p1.m_color.getFade(1 - t10) + p0.m_color.getFade(t10);

                // calculating a new point between p1 and p2
                // t is positive because p1.z > 1 && p2.z <= 1
                float t12 = (p1.z - 1) / (p1.z - p2.z);
                float x12 = p1.x + t12 * (p2.x - p1.x);
                float y12 = p1.y + t12 * (p2.y - p1.y);
                Color c12 = p1.m_color.getFade(1 - t12) + p2.m_color.getFade(t12);

                Triangle newTb(p2, p0, Point(x12, y12, 1, c12));
                shape.triangles += newTb;
                Triangle newTa(p0, Point(x10, y10, 1, c10), Point(x12, y12, 1, c12));
                shape.triangles[i] = newTa;
                i++;
            }
        }
        else if (p2.z > 1)
        {
            // just p2 needs to be clipped

            // calculating a new point between p2 and p0
            // t is positive because p2.z > 1 && p0.z <= 1
            float t20 = (p2.z - 1) / (p2.z - p0.z);
            float x20 = p2.x + t20 * (p0.x - p2.x);
            float y20 = p2.y + t20 * (p0.y - p2.y);
            Color c20 = p2.m_color.getFade(1 - t20) + p0.m_color.getFade(t20);

            // calculating a new point between p2 and p1
            // t is positive because p2.z > 1 && p1.z <= 1
            float t21 = (p2.z - 1) / (p2.z - p1.z);
            float x21 = p2.x + t21 * (p1.x - p2.x);
            float y21 = p2.y + t21 * (p1.y - p2.y);
            Color c21 = p2.m_color.getFade(1 - t21) + p1.m_color.getFade(t21);

            Triangle newTb(p0, p1, Point(x21, y21, 1, c21));
            shape.triangles += newTb;
            Triangle newTa(p0, Point(x21, y21, 1, c21), Point(x20, y20, 1, c20));
            shape.triangles[i] = newTa;
            i++;
        }
        else
        {
            // triangle doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.lines.size();)
    {
        Point & p0 = shape.lines[i].p0;
        Point & p1 = shape.lines[i].p1;

        // clip lines
        if (p0.z > 1)
        {
            if (p1.z > 1)
            {
                // both ends of the line need to be clipped
                shape.lines.remove(i);
            }
            else
            {
                // p0 end of the line needs to be clipped
                // 
                // pNew.x = p0.x + t * (p1.x - p0.x)
                // pNew.y = p0.y + t * (p1.y - p0.y)
                // pNew.z = p0.z + t * (p1.z - p0.z)
                // pNew.z = 1
                // t is positive because p0.z > 1 && p1.z <= 1
                float t = (p0.z - 1) / (p0.z - p1.z);
                float nX = p0.x + t * (p1.x - p0.x);
                float nY = p0.y + t * (p1.y - p0.y);
                Color nC = p0.m_color.getFade(1 - t) + p1.m_color.getFade(t);

                Line newL(Point(nX, nY, 1, nC), p1);
                shape.lines[i] = newL;
                i++;
            }
        }
        else if (p1.z > 1)
        {
            // p1 end of the line needs to be clipped
            float t = (p1.z - 1) / (p1.z - p0.z);
            float nX = p1.x + t * (p0.x - p1.x);
            float nY = p1.y + t * (p0.y - p1.y);
            Color nC = p1.m_color.getFade(1 - t) + p0.m_color.getFade(t);

            Line newL(p0, Point(nX, nY, 1, nC));
            shape.lines[i] = newL;
            i++;
        }
        else
        {
            // line doesn't need to be clipped
            i++;
        }
    }

    for (int i = 0; i < shape.points.size();)
    {
        // clip near points
        if (shape.points[i].z > 1)
        {
            shape.points.remove(i);
        }
        else
        {
            i++;
        }
    }
}


void ScreenBuffer::drawQuadraticBezierCurveEx(Pair<int> v0, Pair<int> v1, Pair<int> v2, const Color & color)
{
    float length01 = sqrt((v0.x - v1.x) * (v0.x - v1.x) + (v0.y - v1.y) * (v0.y - v1.y));
    float length12 = sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));

    // The number of pixels drawn will equal the length of the curve. The
    // curve's length must be smaller then the distance from point 0 to
    // point 1 to point 2. Ill just use that as an estime to determine
    // how many points to draw. Since it's an over estime, some points
    // will be drawn multiple times, but I'm fine with that.
    //
    // ...
    // 
    // Actually this doesn't work that well.
    float arcLengthEstimate = length01 + length12;

    for (float i = 0; i <= 1; i += 1 / arcLengthEstimate)
    {
        drawPointSafely(getQuadraticBezierPoint(i, v0, v1, v2), color);
    }
}


void ScreenBuffer::drawCubicBezierCurveEx(Pair<int> v0, Pair<int> v1, Pair<int> v2, Pair<int> v3, const Color & color)
{
    float length01 = sqrt((v0.x - v1.x) * (v0.x - v1.x) + (v0.y - v1.y) * (v0.y - v1.y));
    float length12 = sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
    float length23 = sqrt((v2.x - v3.x) * (v2.x - v3.x) + (v2.y - v3.y) * (v2.y - v3.y));

    // The reason for this is somewhat explained in drawQuadraticBexierCurve
    float arcLengthEstimate = length01 + length12 + length23;

    for (float i = 0; i <= 1; i += 1 / arcLengthEstimate)
    {
        drawPointSafely(getCubicBezierPoint(i, v0, v1, v2, v3), color);
    }
}


Pair<int> ScreenBuffer::getQuadraticBezierPoint(float t, Pair<int> v0, Pair<int> v1, Pair<int> v2)
{
    float c0 = (1 - t) * (1 - t);
    float c1 = 2 * t * (1 - t);
    float c2 = t * t;

    int x = (int)(c0 * v0.x + c1 * v1.x + c2 * v2.x);
    int y = (int)(c0 * v0.y + c1 * v1.y + c2 * v2.y);
    return Pair<int>(x, y);
}


Pair<int> ScreenBuffer::getCubicBezierPoint(float t, Pair<int> v0, Pair<int> v1, Pair<int> v2, Pair<int> v3)
{
    float c0 = (1 - t) * (1 - t) * (1 - t);
    float c1 = 3 * t * (1 - t) * (1 - t);
    float c2 = 3 * t * t * (1 - t);
    float c3 = t * t * t;

    int x = (int)(c0 * v0.x + c1 * v1.x + c2 * v2.x + c3 * v3.x);
    int y = (int)(c0 * v0.y + c1 * v1.y + c2 * v2.y + c3 * v3.y);
    return Pair<int>(x, y);
}
