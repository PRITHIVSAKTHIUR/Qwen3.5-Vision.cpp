# Qwen3.5-Vision C++ Web App

A C++ project wrapper around a rich Web App for Qwen3.5 and Qwen3-VL models. Powered by `pybind11` and an embedded native C++ HTTP server (`httplib`).

It provides a modern GUI for:
- Image queries
- Image captioning
- Point extraction
- Object detection


![1](https://cdn-uploads.huggingface.co/production/uploads/65bb837dbfb878f46c77de4c/b8ERcd1r5dRgInumQfsUu.png)


## Build Instructions (Local)

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Usage

```bash
# By default runs Qwen3_5ForConditionalGeneration model: prithivMLmods/Qwen3.5-0.8B-Unredacted-MAX
./build/Release/visionapp.exe 

# Or use specific architectures
./build/Release/visionapp.exe --model_name Qwen/Qwen3-VL-2B-Instruct --arch Qwen3VLForConditionalGeneration --port 7860
```

## Running in Google Colab

You can easily run this C++ application in Google Colab. Colab already provides the essential tools (Python 3, GCC, and CMake).

1. Open a new [Google Colab Notebook](https://colab.research.google.com/).
2. Create a new code cell and paste the following block to clone, build, and run the project all at once:

```bash
# Clone the repository
!git clone https://github.com/PRITHIVSAKTHIUR/Qwen3.5-Vision.cpp.git
%cd Qwen3.5-Vision.cpp

# Install dependencies (Python dev headers & libraries)
!apt-get update && apt-get install -y python3-dev
!pip install pybind11 torch transformers pillow torchvision

# Build the project
!mkdir build
%cd build
!cmake ..
!cmake --build . --config Release

# Run the executable (replace arguments with your specific model details)
!./visionapp --model_name prithivMLmods/Qwen3.5-0.8B-Unredacted-MAX \
             --arch Qwen3_5ForConditionalGeneration \
             --port 7860
```
