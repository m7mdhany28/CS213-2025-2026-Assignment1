/*******************************************************
 *
 * Team Members:
 *   - Khaled Ahmed Sayed (ID: 20240176)
 *   - Amr Yasser Mohamed (ID: 20240406)
 *   - Mohamed Hany Abdelhadi (ID: 20240532)
 *
 * Work Breakdown:
 *   - [Khaled Ahmed Sayed]: Implemented Filters (1, 4, 7, 10 & 15 (bonus))
 *   - [Amr Yasser Mohamed]: Implemented Filters (2, 5, 8, 11 & 13 (bonus))
 *   - [Mohamed Hany Abdelhadi]: Implemented Filters (3, 6, 9, 12 & 14 (bonus))
 *   - All: Contributed to menu, testing, integration
 *
 * Description:
 *   This program implements a simple image processing tool.
 *   The user can:
 *     1. Load an image (jpg, jpeg, png, bmp)
 *     2. Apply filters
 *     3. Save the modified image (overwrite or new file)
 *     4. Exit the program (with save prompt)
 *
 *******************************************************/

#include <iostream>
#include <string>
#include <random>
#include "Image_Class.h"
using namespace std;

// ------------------ FILE CHECK ------------------
bool file_check(Image &img, string &filename)
{
    bool loaded = false;
    while (!loaded)
    {
        cout << "Enter image filename: ";
        cin >> filename;
        try
        {
            img.loadNewImage(filename);
            loaded = true;
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
            cout << "Please try again.\n";
        }
    }
    return true;
}

// ------------------ SAVE ------------------
void save_image(Image &image, string &filename)
{
    bool running = true;
    while (running)
    {
        int choice;
        cout << "1. Save on the same file\n";
        cout << "2. Change file name\n";
        cin >> choice;

        if (choice == 1)
        {
            try
            {
                image.saveImage(filename);
                running = false;
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
        }
        else if (choice == 2)
        {
            bool rename_done = false;
            while (!rename_done)
            {
                string newFilename;
                cout << "Enter new file name: ";
                cin >> newFilename;
                try
                {
                    image.saveImage(newFilename);
                    filename = newFilename;
                    rename_done = true;
                    running = false;
                }
                catch (const exception &e)
                {
                    cout << "Error: " << e.what() << endl;
                    cout << "Please try another filename.\n";
                }
            }
        }
        else
        {
            cout << "Error: Invalid choice\n";
        }
    }
}

// ------------------ EXIT ------------------
void exit_program(Image &image, string &filename)
{
    bool running = true;
    while (running)
    {
        int choice;
        cout << "Do you want to save before exiting?\n";
        cout << "1. Save image (same or new name)\n";
        cout << "2. Exit without saving\n";
        cin >> choice;

        if (choice == 1)
        {
            save_image(image, filename);
            running = false;
        }
        else if (choice == 2)
        {
            cout << "Exiting without saving...\n";
            running = false;
        }
        else
        {
            cout << "Error: Invalid choice\n";
        }
    }
}

// ------------------ FILTERS ------------------

// Filter 1: Grayscale Image
void grayscale(Image &img)
{
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            unsigned int avg = 0;
            for (int k = 0; k < img.channels; k++)
            {
                avg += img(i, j, k);
            }
            avg /= 3;
            for (int k = 0; k < img.channels; k++)
            {
                img(i, j, k) = avg;
            }
        }
    }
}

// Filter 2: Black and White Image
void black_and_white(Image &img)
{
    for (int j = 0; j < img.height; j++)
    {
        for (int i = 0; i < img.width; i++)
        {
            unsigned char R = img(i, j, 0);
            unsigned char G = img(i, j, 1);
            unsigned char B = img(i, j, 2);
            unsigned char Gray = (R + G + B) / 3;
            img(i, j, 0) = img(i, j, 1) = img(i, j, 2) = (Gray > 127 ? 255 : 0);
        }
    }
}

// Filter 3: Invert
void invertImage(Image &img)
{
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            for (int x = 0; x < 3; x++)
            {
                img(i, j, x) = 255 - img(i, j, x);
            }
        }
    }
}

// Filter 4: Merge Two Images
void mergeImages(Image &img1)
{
    string secondImgName;
    Image img2;
    file_check(img2, secondImgName);

    cout << "\nChoose merging type:\n";
    cout << "1. Resize smaller image to match the larger one\n";
    cout << "2. Merge only the common region\n";
    int mergingMode;
    cin >> mergingMode;

    if (mergingMode == 1)
    {
        int maxW = max(img1.width, img2.width);
        int maxH = max(img1.height, img2.height);

        Image resized1(maxW, maxH);
        Image resized2(maxW, maxH);

        for (int y = 0; y < maxH; y++)
        {
            for (int x = 0; x < maxW; x++)
            {
                int srcX1 = x * img1.width / maxW;
                int srcY1 = y * img1.height / maxH;
                int srcX2 = x * img2.width / maxW;
                int srcY2 = y * img2.height / maxH;

                for (int c = 0; c < 3; c++)
                {
                    resized1(x, y, c) = img1(srcX1, srcY1, c);
                    resized2(x, y, c) = img2(srcX2, srcY2, c);
                }
            }
        }
        for (int y = 0; y < maxH; y++)
        {
            for (int x = 0; x < maxW; x++)
            {
                for (int c = 0; c < 3; c++)
                {
                    img1(x, y, c) = (resized1(x, y, c) + resized2(x, y, c)) / 2;
                }
            }
        }
    }
    else
    {
        int newW = min(img1.width, img2.width);
        int newH = min(img1.height, img2.height);

        for (int y = 0; y < newH; y++)
        {
            for (int x = 0; x < newW; x++)
            {
                for (int c = 0; c < 3; c++)
                {
                    img1(x, y, c) = (img1(x, y, c) + img2(x, y, c)) / 2;
                }
            }
        }
    }
}

// Filter 5: Flip Image
void flipImage(Image &img)
{
    int choice;
    while (true)
    {
        cout << "Choose flip type:\n";
        cout << "1. Horizontal Flip\n";
        cout << "2. Vertical Flip\n";
        cin >> choice;

        if (choice == 1)
        {
            for (int j = 0; j < img.height; j++)
            {
                for (int i = 0; i < img.width / 2; i++)
                {
                    for (int c = 0; c < img.channels; c++)
                    {
                        swap(img(i, j, c), img(img.width - i - 1, j, c));
                    }
                }
            }
            break;
        }
        else if (choice == 2)
        {
            for (int j = 0; j < img.height / 2; j++)
            {
                for (int i = 0; i < img.width; i++)
                {
                    for (int c = 0; c < img.channels; c++)
                    {
                        swap(img(i, j, c), img(i, img.height - j - 1, c));
                    }
                }
            }
            break;
        }
        else
        {
            cout << "Invalid choice, try again.\n";
        }
    }
}

// Filter 6: Rotate Image
void rotateImage(Image &img)
{
    int angle;
    while (true)
    {
        cout << "Enter rotation angle (90, 180, 270): ";
        cin >> angle;

        Image newImg;
        if (angle == 90)
        {
            newImg = Image(img.height, img.width);
            for (int i = 0; i < img.width; i++)
            {
                for (int j = 0; j < img.height; j++)
                {
                    for (int c = 0; c < 3; c++)
                    {
                        newImg(newImg.width - 1 - j, i, c) = img(i, j, c);
                    }
                }
            }
            img = newImg;
            break;
        }
        else if (angle == 180)
        {
            newImg = Image(img.width, img.height);
            for (int i = 0; i < img.width; i++)
            {
                for (int j = 0; j < img.height; j++)
                {
                    for (int c = 0; c < 3; c++)
                    {
                        newImg(newImg.width - 1 - i, newImg.height - 1 - j, c) = img(i, j, c);
                    }
                }
            }
            img = newImg;
            break;
        }
        else if (angle == 270)
        {
            newImg = Image(img.height, img.width);
            for (int i = 0; i < img.width; i++)
            {
                for (int j = 0; j < img.height; j++)
                {
                    for (int c = 0; c < 3; c++)
                    {
                        newImg(j, newImg.height - 1 - i, c) = img(i, j, c);
                    }
                }
            }
            img = newImg;
            break;
        }
        else
        {
            cout << "Invalid choice, try again.\n";
        }
    }
}

// Filter 7: Darken / Lighten Image
unsigned char clamp(int value)
{
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return static_cast<unsigned char>(value);
}

void darken_lighten(Image &img)
{
    int choice;
    float factor;

    while (true)
    {
        cout << "Choose filter:\n1. Darken (50%)\n2. Lighten (50%)\n";
        cin >> choice;

        if (choice == 1)
        {
            factor = 0.5f;
            break;
        }
        else if (choice == 2)
        {
            factor = 1.5f;
            break;
        }
        else
        {
            cout << "Invalid choice, try again.\n";
        }
    }

    for (int y = 0; y < img.height; ++y)
    {
        for (int x = 0; x < img.width; ++x)
        {
            for (int c = 0; c < 3; ++c)
            {
                int original = img(x, y, c);
                int modified = static_cast<int>(original * factor);
                img(x, y, c) = clamp(modified);
            }
        }
    }
}

// Filter 8: Crop Image
void CropImage(Image &img)
{

    bool Crop = false;
    while (!Crop)
    {
        int Start_Width, Start_Height, New_Width, New_Height;

        cout << "Enter start Width : ";
        cin >> Start_Width;
        cout << "Enter start Height : ";
        cin >> Start_Height;
        cout << "Enter new width : ";
        cin >> New_Width;
        cout << "Enter new height : ";
        cin >> New_Height;

        if (Start_Width < 0 || Start_Height < 0 || New_Width <= 0 || New_Height <= 0 || Start_Width + New_Width > img.width || Start_Height + New_Height > img.height)
        {
            cout << "\n Invalid crop ! Try again.\n";
            continue;
        }
        Image Cropped_Image(New_Width, New_Height);

        for (int y = 0; y < New_Height; y++)
        {
            for (int x = 0; x < New_Width; x++)
            {
                for (int c = 0; c < img.channels; c++)
                {
                    Cropped_Image(x, y, c) = img(Start_Width + x, Start_Height + y, c);
                }
            }
        }
        img = Cropped_Image;
        Crop = true;
    }
}

// Filter 9: Add Frame
void AddFrame(Image &img)
{
    int choice;

    cout << "Choose the frame type:\n";
    cout << "1. Simple Frame\n";
    cout << "2. Fancy Frame\n";
    cin >> choice;

    if (choice == 1)
    {
        int Thikness;
        cout << "Enter frame thickness (blue): ";
        cin >> Thikness;
        for (int y = 0; y < img.height; y++)
        {
            for (int x = 0; x < img.width; x++)
            {
                if (y < Thikness || y >= img.height - Thikness || x < Thikness || x >= img.width - Thikness)
                {
                    for (int c = 0; c < 3; c++)
                        img.setPixel(x, y, 0, 40);
                    img.setPixel(x, y, 1, 52);
                    img.setPixel(x, y, 2, 130);
                }
            }
        }
    }
    else if (choice == 2)
    {
        int outerThickness, innerThickness;
        cout << "Enter the outer border thickness (white): ";
        cin >> outerThickness;

        cout << "Enter the inner border thickness (black): ";
        cin >> innerThickness;

        for (int y = 0; y < img.height; y++)
        {
            for (int x = 0; x < img.width; x++)
            {
                if (y < outerThickness || y >= img.height - outerThickness ||
                    x < outerThickness || x >= img.width - outerThickness)
                {
                    for (int c = 0; c < 3; c++)
                        img.setPixel(x, y, c, 255);
                }
                else if (y < outerThickness + innerThickness || y >= img.height - (outerThickness + innerThickness) ||
                         x < outerThickness + innerThickness || x >= img.width - (outerThickness + innerThickness))
                {
                    for (int c = 0; c < 3; c++)
                        img.setPixel(x, y, c, 0);
                }
            }
        }
    }
    else
    {
        cout << "Invalid choice.\n";
    }
}

// Filter 10: Edge Detection
void detectEdges(Image &img)
{
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    Image gray = img;
    Image result(img.width, img.height);

    // Convert to grayscale first
    for (int y = 0; y < img.height; y++)
    {
        for (int x = 0; x < img.width; x++)
        {
            unsigned char r = img(x, y, 0);
            unsigned char g = img(x, y, 1);
            unsigned char b = img(x, y, 2);
            unsigned char grayVal = static_cast<unsigned char>(0.3 * r + 0.59 * g + 0.11 * b);
            for (int c = 0; c < 3; c++)
                gray(x, y, c) = grayVal;
        }
    }

    // Apply Sobel filter
    for (int y = 1; y < img.height - 1; y++)
    {
        for (int x = 1; x < img.width - 1; x++)
        {
            int gradX = 0, gradY = 0;
            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    gradX += gray(x + kx, y + ky, 0) * sobelX[ky + 1][kx + 1];
                    gradY += gray(x + kx, y + ky, 0) * sobelY[ky + 1][kx + 1];
                }
            }
            int magnitude = sqrt(gradX * gradX + gradY * gradY);
            magnitude = min(255, magnitude);
            int inverted = 255 - magnitude;
            for (int c = 0; c < 3; c++)
                result(x, y, c) = inverted;
        }
    }

    img = result;
}

// Filter 11: Resize Image
void ResizeImage(Image &img)
{

    bool Resize = false;
    while (!Resize)
    {

        int New_Width, New_Height;
        cout << "\nEnter new width: ";
        cin >> New_Width;
        cout << "Enter new height: ";
        cin >> New_Height;

        if (New_Width <= 0 || New_Height <= 0)
        {
            cout << "\nInvalid !\n Try again \n  Width and height must be positive.\n";
            continue;
        }

        Image Resized_Image(New_Width, New_Height);

        float Width_Size = (float)img.width / New_Width;
        float Height_Size = (float)img.height / New_Height;

        for (int y = 0; y < New_Height; y++)
        {
            for (int x = 0; x < New_Width; x++)
            {
                int New_X = (int)(x * Width_Size);
                int New_Y = (int)(y * Height_Size);

                for (int c = 0; c < img.channels; c++)
                {
                    Resized_Image(x, y, c) = img(New_X, New_Y, c);
                }
            }
        }
        img = Resized_Image;
        Resize = true;
    }
}

// Filter 12: Blur
void blurImage(Image &img)
{
    int radius;
    cout << "Enter blur level (1 = light, 5 = medium, 10 = strong): ";
    cin >> radius;

    if (radius < 1)
        radius = 1;
    if (radius > 25)
        radius = 25; // limit

    Image temp(img.width, img.height);

    for (int y = 0; y < img.height; y++)
    {
        for (int x = 0; x < img.width; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                int sum = 0;
                int count = 0;

                for (int dy = -radius; dy <= radius; dy++)
                {
                    int ny = y + dy;
                    if (ny < 0 || ny >= img.height)
                        continue;

                    for (int dx = -radius; dx <= radius; dx++)
                    {
                        int nx = x + dx;
                        if (nx < 0 || nx >= img.width)
                            continue;

                        sum += img(nx, ny, c);
                        count++;
                    }
                }

                temp(x, y, c) = sum / count;
            }
        }
    }
    img = temp;
}

// Filter 13: Sunlight Image
void SunlightImage(Image &img)
{
    for (int y = 0; y < img.height; y++)
    {
        for (int x = 0; x < img.width; x++)
        {

            int r = img(x, y, 0);
            int g = img(x, y, 1);
            int b = img(x, y, 2);

            r = min(255, r + 50);
            g = min(255, g + 40);
            b = min(255, b + 5);

            img(x, y, 0) = r;
            img(x, y, 1) = g;
            img(x, y, 2) = b;
        }
    }
}

// Filter 14: Purple Tone Image
void PurpleTone(Image &img)
{
    for (int y = 0; y < img.height; y++)
    {
        for (int x = 0; x < img.width; x++)
        {
            for (int c = 0; c < 3; c++) // loop over R,G,B channels
            {
                int value = img.getPixel(x, y, c);

                if (c == 0)
                    value = min(255, value + 50);
                else if (c == 1)
                    value = max(0, value - 50);
                else if (c == 2)
                    value = min(255, value + 80);
                img.setPixel(x, y, c, value);
            }
        }
    }
}

// Filter 15: TV Static Effect
void TVStaticEffect(Image &img)
{
    //Add Noise
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-static_cast<int>(255 * 0.5), static_cast<int>(255 * 0.5));

    for (int y = 0; y < img.height; y++)
    {
        for (int x = 0; x < img.width; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                int pixelValue = img(x, y, c) + dis(gen);
                pixelValue = max(0, min(255, pixelValue)); // Clamp 0–255
                img(x, y, c) = static_cast<unsigned char>(pixelValue);
            }
        }
    }

    //Add Scanlines
    int lineThickness = 4;
    for (int y = 0; y < img.height; y++)
    {
        if (y % lineThickness == 0)
        {
            for (int x = 0; x < img.width; x++)
            {
                for (int c = 0; c < 3; c++)
                {
                    img(x, y, c) = 0; // Black horizontal line
                }
            }
        }
    }
}

// ------------------ MAIN ------------------
int main()
{
    string currentFilename;
    Image currentImage;

    // Initial load
    file_check(currentImage, currentFilename);

    int choice;
    bool play = true;
    while (play)
    {
        // ------------------ MENU ------------------
        cout << "\n===== MENU =====\n";
        cout << "0. Load new image\n";
        cout << "1. Apply Filter 1 (Grayscale Conversion)\n";
        cout << "2. Apply Filter 2 (Black and White)\n";
        cout << "3. Apply Filter 3 (Invert)\n";
        cout << "4. Apply Filter 4 (Merge Images)\n";
        cout << "5. Apply Filter 5 (Flip)\n";
        cout << "6. Apply Filter 6 (Rotate)\n";
        cout << "7. Apply Filter 7 (Darken and Lighten)\n";
        cout << "8. Apply Filter 8 (Crop)\n";
        cout << "9. Apply Filter 9 (Add Frame)\n";
        cout << "10. Apply Filter 10 (Detect Edges)\n";
        cout << "11. Apply Filter 11 (Resizing)\n";
        cout << "12. Apply Filter 12 (Blur)\n";
        cout << "13. Apply Filter 13 (Add sunlight)\n";
        cout << "14. Apply Filter 14 (Purple Tone)\n";
        cout << "15. Apply Filter 15 (TV Static Effect)\n";
        cout << "16. Save image\n";
        cout << "17. Exit\n";
        cout << "Enter your choice number: ";
        cin >> choice;

        switch (choice)
        {
        case 0:
            file_check(currentImage, currentFilename);
            break;
        case 1:
            grayscale(currentImage);
            break;
        case 2:
            black_and_white(currentImage);
            break;
        case 3:
            invertImage(currentImage);
            break;
        case 4:
            mergeImages(currentImage);
            break;
        case 5:
            flipImage(currentImage);
            break;
        case 6:
            rotateImage(currentImage);
            break;
        case 7:
            darken_lighten(currentImage);
            break;
        case 8:
            CropImage(currentImage);
            break;
        case 9:
            AddFrame(currentImage);
            break;
        case 10:
            detectEdges(currentImage);
            break;
        case 11:
            ResizeImage(currentImage);
            break;
        case 12:
            blurImage(currentImage);
            break;
        case 13:
            SunlightImage(currentImage);
            break;
        case 14:
            PurpleTone(currentImage);
            break;
        case 15:
            TVStaticEffect(currentImage);
            break;
        case 16:
            save_image(currentImage, currentFilename);
            break;
        case 17:
            exit_program(currentImage, currentFilename);
            play = false;
            break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }
}

