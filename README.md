# Qwen3.5-Vision C++ Web App

A C++ project wrapper around a rich Web App for Qwen3.5 and Qwen3-VL models. Powered by `pybind11` and an embedded native C++ HTTP server (`httplib`).

It provides a modern GUI for:
- Image queries
- Image captioning
- Point extraction
- Object detection

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

You can easily compile and run this C++ application directly inside a Google Colab notebook.

1. **Open a new Colab Notebook** and switch the runtime to **GPU** (T4 or better is recommended for faster inference).
2. **Install required dependencies**:
   ```python
   !apt-get update
   !apt-get install -y cmake build-essential
   !pip install pybind11 torch transformers pillow torchvision
   ```
3. **Clone the repository**:
   ```python
   !git clone https://github.com/PRITHIVSAKTHIUR/Qwen3.5-Vision.cpp.git
   %cd Qwen3.5-Vision.cpp
   ```
4. **Build the C++ app**:
   ```python
   !cmake -B build -S .
   !cmake --build build --config Release
   ```
5. **Run the app and expose the port**:
   To access the local web server from Colab, you can use `localtunnel` or Colab's built-in proxy. 
   
   Using Colab's proxy:
   ```python
   import threading
   import time
   import os
   from google.colab import output

   def run_app():
       os.system("./build/visionapp --port 7860")

   # Start the C++ server in the background
   threading.Thread(target=run_app, daemon=True).start()

   # Wait briefly for it to spin up, then expose the port
   time.sleep(15) # Give the model time to download & load
   output.serve_kernel_port_as_window(7860)
   ```
