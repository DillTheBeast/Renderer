#define STB_IMAGE_IMPLEMENTATION
#include "./stb-master/stb_image.h"
#include <iostream>

int main() {
    const char* imagePath = "/Users/dillonmaltese/Documents/GitHub/Renderer/C++/texture1.jpeg";

    int width, height, nrChannels;
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

    if (data) {
        std::cout << "Image loaded successfully!" << std::endl;
        std::cout << "Width: " << width << " pixels" << std::endl;
        std::cout << "Height: " << height << " pixels" << std::endl;
        std::cout << "Number of channels: " << nrChannels << std::endl;
    } else {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
    }

    stbi_image_free(data);

    return 0;
}
