/*******************************************************
 *
 * Team Members:
 *   - Khaled Ahmed Sayed (ID: 20240176)
 *   - Amr Yasser Mohamed (ID: 20240406)
 *   - Mohamed Hany Abdelhadi (ID: 20240532)
 *
 * Work Breakdown:
 *   - [Khaled Ahmed Sayed]: Implemented Filters (1, 4, 7, 10)
 *   - [Amr Yasser Mohamed]: Implemented Filters (2, 5, 8, 11 & 13 (bonus))
 *   - [Mohamed Hany Abdelhadi]: Implemented Filters (3, 6, 9, 12 & 16 (bonus))
 *   - All: Contributed to menu, testing, integration
 *
 *******************************************************/

#include <iostream>
#include <string>
#include "Image_Class.h"
using namespace std;

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

// Filter 1: Grayscale
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

// Filter 2: Black and White
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

// Filter 5: Flip
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

// Filter 6: Rotate
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

// Filter 7: Darken / Lighten
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

                if (c == 0) // Red
                    value = min(255, value + 50);
                else if (c == 1) // Green
                    value = max(0, value - 50);
                else if (c == 2) // Blue
                    value = min(255, value + 80);
                img.setPixel(x, y, c, value);
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
    bool loaded = false;
    while (!loaded)
    {
        cout << "Please Load an Image: ";
        cin >> currentFilename;
        try
        {
            currentImage.loadNewImage(currentFilename);
            loaded = true;
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

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
        cout << "15. Save image\n";
        cout << "16. Exit\n";
        cout << "Enter your choice number: ";
        cin >> choice;

        switch (choice)
        {
        case 0:
            cout << "Enter new image filename: ";
            cin >> currentFilename;
            try
            {
                currentImage.loadNewImage(currentFilename);
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
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
            break;
        case 9:
            AddFrame(currentImage);
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            PurpleTone(currentImage);
            break;
        case 15:
            save_image(currentImage, currentFilename);
            break;
        case 16:
            exit_program(currentImage, currentFilename);
            play = false;
            break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }
}
