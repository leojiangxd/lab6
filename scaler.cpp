#include <iostream>
#include "Image.h"
#include "ConsoleGfx.h"
#include <cmath>

ConsoleGfx *cg = ConsoleGfx::getInstance();
Image image = Image(nullptr);

int menu();
int findCommon(int arr[]);
unsigned char* enlargeImage(int scale, int width, int height, unsigned char* imagePtr);
unsigned char* reduceImage(int scale, int width, int height, unsigned char* imagePtr);

int main()
{
    std::cout << "Welcome to the Image Scaler!\n" << std::endl;
    std::cout << "Displaying Spectrum Image:" << std::endl;
    cg->displayImage(cg->testRainbow);
    std::cout << std::endl;
    std::string fileName;
    int scale;
    int width;
    int height;

    while(true)
    {
        int selection = menu();
        switch(selection)
        {
            case 0: // exits program
                return 0;
            case 1:
                std::cout << "Enter name of file to load: ";
                std::cin>> fileName;
                std::cout << std::endl;
                image.setImageData(cg->loadFile(fileName));
                break;
            case 2: // loads the test image
                image.setImageData(cg->testImage);
                std::cout << "Test Image data loaded.\n" << std::endl;
                break;
            case 3: // displays image
                if (image.getImageData() == nullptr)
                {
                    std::cout << "Error: No image data loaded.\n" << std::endl;
                    break;
                }
                cg->displayImage(image.getImageData());
                std::cout << std::endl;
                break;
            case 4: // enlarges image by up to 2^4
                if (image.getImageData() == nullptr)
                {
                    std::cout << "Error: No image data loaded.\n" << std::endl;
                    break;
                }
                std::cout << "Enter orders of magnitude for enlargement: ";
                std::cin >> scale;
                while(scale > 4 || scale < 0)
                {
                    std::cout << "Must be between [0,4]." << std::endl;
                    std::cout << "Enter orders of magnitude for enlargement: ";
                    std::cin >> scale;
                }

                width = int(image.getWidth());
                height = int(image.getHeight());
                image.setImageData(enlargeImage(scale, width, height, image.getImageData()));

                std::cout << "Image Enlarged!\n" << std::endl;
                break;
            case 5: // reduces image by up to 2^4
                if (image.getImageData() == nullptr)
                {
                    std::cout << "Error: No image data loaded.\n" << std::endl;
                    break;
                }
                std::cout << "Enter orders of magnitude for reduction: ";
                std::cin >> scale;
                while(scale < -4 || scale > 0)
                {
                    std::cout << "Must be between [-4,0]." << std::endl;
                    std::cout << "Enter orders of magnitude for reduction: ";
                    std::cin >> scale;
                }

                width = int(image.getWidth());
                height = int(image.getHeight());
                image.setImageData(reduceImage(scale, width, height, image.getImageData()));

                std::cout << "Image Reduced!\n" << std::endl;
                break;
            case 6: // displays image dimensions
                if (image.getImageData() == nullptr)
                {
                    std::cout << "Error: No image data loaded.\n" << std::endl;
                    break;
                }
                std::cout << "Image Dimensions: ";
                std::cout << "(" << int(image.getWidth()) << ", " << int(image.getHeight()) << ")\n" << std::endl;
                break;
            default:
                exit(0);
        }
    }
}

int menu()
{
    std::cout << "Scaler Menu" << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << "0. Exit"     << std::endl;
    std::cout << "1. Load File"<< std::endl;
    std::cout << "2. Load Test Image"<< std::endl;
    std::cout << "3. Display Image"<< std:: endl;
    std::cout << "4. Enlarge Image"<< std::endl;
    std::cout << "5. Shrink Image"<< std::endl;
    std::cout << "6. Show Image Properties\n" << std::endl;
    std::cout << "Select a Menu Option: ";

    int selection;
    std::cin>> selection;
    return selection;
}

unsigned char* enlargeImage(int scale, int width, int height, unsigned char* imagePtr)
{
    if(scale == 0)
        return imagePtr;

    int area = width * height;
    int scaledWidth = width * pow(2,scale);
    int scaledHeight = height * pow(2,scale);

    while(scaledWidth > 256 || scaledHeight > 256) // max image size
    {
        scale--;
        scaledWidth = width * pow(2,scale);
        scaledHeight = height * pow(2,scale);
        if (scaledWidth <= 1 || scaledHeight <= 1)
            return imagePtr;
    }

    int scaledArea = width * height * pow(2,scale) * pow(2,scale);;

    unsigned char *arrPtr = new unsigned char[scaledArea+2];
    *(arrPtr + 0) = scaledWidth;
    *(arrPtr + 1) = scaledHeight;
    int count = 0;
    for(int i = 2; i < area; i++)
    {
        for(int j = 0; j < pow(2,scale); j++)
        {
//            *(arrPtr + count +2) = *(imagePtr + i);

            for(int k = 0; k <= pow(2,scale)-1; k++) // copies row by scale
            {
                *(arrPtr + count +scaledWidth*k + 2) = *(imagePtr + i); // copies column by scale
            }

            if((count+1)%scaledWidth==0) // continues count after the duplicated rows
            {
                count += (pow(2,scale)-1)*scaledWidth;
            }
            count++;
        }
    }
//    image.setImageData(arrPtr);
//    cg->displayImage(image.getImageData());
    delete imagePtr;
    return arrPtr;
}

unsigned char* reduceImage(int scale, int width, int height, unsigned char* imagePtr)
{
    if (scale == 0) // returns if scale is 0
        return imagePtr;

    int scaledWidth = width / pow(2,abs(scale));
    int scaledHeight = height / pow(2,abs(scale));

    while(scaledWidth <= 1 || scaledHeight <= 1) // min image size
    {
        scale++;
        std::cout << scale << std::endl;
        scaledWidth = width / pow(2,abs(scale));
        scaledHeight = height / pow(2,abs(scale));
        if (scale == 0)
            return imagePtr;
    }

    int scaledArea = scaledWidth * scaledHeight;
    unsigned char *arrPtr = new unsigned char[scaledArea + 2];
    *(arrPtr + 0) = scaledWidth;
    *(arrPtr + 1) = scaledHeight;

    int count = 0;
    int arr2d[height][width]; // creates a 2d array of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            arr2d[i][j] = *(imagePtr + count++);
        }
    }

    int counter2 = 2;
    for (int r = 0; r < height; r += pow(2,abs(scale))) // iterates through chunks of the 2d array
    {
        for (int c = 0; c < width; c += pow(2,abs(scale)))
        {
            int temp[int(pow(2,abs(scale) * pow(2,abs(scale))))];
            int counter = 0;

            for (int i = 0; i < pow(2,abs(scale)); i++)
            {
                for (int j = 0; j < pow(2,abs(scale)); j++)
                {
                    temp[counter] = arr2d[r + i][c + j];
                    counter++;
                }
            }
            *(arrPtr + counter2++) = findCommon(temp); // adds the most frequent value in each chunk
        }
    }
    delete imagePtr;
    return arrPtr;
}

int findCommon(int arr[])
{
    int arrLength = sizeof(arr) / sizeof(int);
    int element = arr[0];
    int mostCount = 0;
    for (int i = 0; i < arrLength; i++)
    {
        int count = 0;
        for (int j = i; j < arrLength; j++)
        {
            if (arr[i] == arr[j]) // counts frequency of element after its index
                count++;
        }
        if (count > mostCount)  // changes element to the element with the greatest frequency
        {
            mostCount = count;
            element = arr[i];
        }
    }
    return element; // returns element with the greatest frequency
}
