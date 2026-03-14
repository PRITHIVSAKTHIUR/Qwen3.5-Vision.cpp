#include "vision_app.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

#include "httplib.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace vision_app {

const char* HTML_CONTENT = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Qwen3.5-Vision C++ App</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --primary: #4F46E5;
            --primary-hover: #4338CA;
            --bg-color: #F3F4F6;
            --panel-bg: #FFFFFF;
            --text-main: #111827;
            --text-muted: #6B7280;
            --border-color: #E5E7EB;
            --border-radius: 12px;
        }

        body { 
            font-family: 'Inter', sans-serif; 
            background-color: var(--bg-color); 
            color: var(--text-main); 
            margin: 0; 
            padding: 2rem; 
            min-height: 100vh;
            display: flex;
            justify-content: center;
        }

        .container { 
            max-width: 1200px; 
            width: 100%;
            display: flex;
            flex-direction: column;
            gap: 2rem;
        }

        .header {
            text-align: center;
        }

        .header h1 {
            margin: 0;
            font-size: 2.5rem;
            font-weight: 700;
            color: var(--text-main);
            letter-spacing: -0.025em;
        }

        .header p {
            margin: 0.5rem 0 0;
            color: var(--text-muted);
            font-size: 1.125rem;
        }

        .main-content {
            display: grid;
            grid-template-columns: 1fr 1.2fr;
            gap: 2rem;
        }

        @media (max-width: 768px) {
            .main-content {
                grid-template-columns: 1fr;
            }
        }

        .panel {
            background: var(--panel-bg);
            padding: 2rem;
            border-radius: var(--border-radius);
            box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1), 0 2px 4px -1px rgba(0, 0, 0, 0.06);
            border: 1px solid var(--border-color);
        }

        .form-group {
            margin-bottom: 1.5rem;
        }

        label { 
            font-weight: 500; 
            display: block; 
            margin-bottom: 0.5rem; 
            color: var(--text-main); 
            font-size: 0.875rem;
        }

        .image-upload-wrapper {
            position: relative;
            border: 2px dashed var(--border-color);
            border-radius: var(--border-radius);
            padding: 2rem;
            text-align: center;
            cursor: pointer;
            transition: all 0.2s ease;
            background: #F9FAFB;
        }

        .image-upload-wrapper:hover {
            border-color: var(--primary);
            background: #EEF2FF;
        }

        .image-upload-wrapper input[type="file"] {
            position: absolute;
            top: 0; left: 0; width: 100%; height: 100%;
            opacity: 0; cursor: pointer;
        }

        .upload-text {
            color: var(--text-muted);
            font-size: 0.875rem;
            pointer-events: none;
        }

        #preview { 
            max-width: 100%; 
            max-height: 300px; 
            display: none; 
            margin-top: 1rem; 
            border-radius: 8px; 
            object-fit: contain; 
            box-shadow: 0 1px 3px rgba(0,0,0,0.1);
        }

        select, textarea { 
            width: 100%; 
            padding: 0.75rem 1rem; 
            border-radius: 8px; 
            border: 1px solid var(--border-color); 
            font-family: inherit; 
            font-size: 0.875rem;
            box-sizing: border-box; 
            transition: border-color 0.2s;
            background: #FFFFFF;
        }

        select:focus, textarea:focus {
            outline: none;
            border-color: var(--primary);
            box-shadow: 0 0 0 3px rgba(79, 70, 229, 0.1);
        }

        textarea { resize: vertical; min-height: 100px; }

        button { 
            background: var(--primary); 
            color: white; 
            border: none; 
            padding: 0.875rem 1.5rem; 
            font-size: 1rem; 
            font-weight: 500; 
            border-radius: 8px; 
            cursor: pointer; 
            width: 100%; 
            transition: background-color 0.2s, transform 0.1s; 
        }

        button:hover { background: var(--primary-hover); }
        button:active { transform: scale(0.98); }
        button:disabled { background: #9CA3AF; cursor: not-allowed; transform: none; }

        .output-panel {
            display: flex;
            flex-direction: column;
        }

        .output-container {
            flex-grow: 1;
            background: #F9FAFB;
            padding: 1.5rem;
            border-radius: 8px;
            border: 1px solid var(--border-color);
            font-family: 'JetBrains Mono', 'Courier New', Courier, monospace;
            font-size: 0.875rem;
            white-space: pre-wrap;
            word-wrap: break-word;
            overflow-y: auto;
            color: #374151;
            min-height: 400px;
        }

        /* Spinner */
        .loader-container {
            display: none;
            align-items: center;
            justify-content: center;
            gap: 0.5rem;
            margin-top: 1rem;
            color: var(--primary);
            font-weight: 500;
            font-size: 0.875rem;
        }

        .spinner {
            width: 1.5rem;
            height: 1.5rem;
            border: 3px solid rgba(79, 70, 229, 0.2);
            border-top-color: var(--primary);
            border-radius: 50%;
            animation: spin 1s linear infinite;
        }

        @keyframes spin {
            to { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Qwen3.5 Vision</h1>
            <p>Advanced Image Understanding Native C++ Interface</p>
        </div>
        
        <div class="main-content">
            <!-- Left Panel: Controls -->
            <div class="panel">
                <div class="form-group">
                    <label>Upload Image</label>
                    <div class="image-upload-wrapper" id="dropZone">
                        <div class="upload-text" id="uploadText">
                            <svg style="width: 2rem; height: 2rem; margin: 0 auto 0.5rem auto; color: #9CA3AF;" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 16v1a3 3 0 003 3h10a3 3 0 003-3v-1m-4-8l-4-4m0 0L8 8m4-4v12"></path></svg>
                            <br/>Click or drag image here
                        </div>
                        <input type="file" id="imageInput" accept="image/*" />
                    </div>
                    <img id="preview" />
                </div>

                <div class="form-group">
                    <label>Task Category</label>
                    <select id="category">
                        <option value="Query">General Query</option>
                        <option value="Caption">Image Captioning</option>
                        <option value="Point">Point Coordinates</option>
                        <option value="Detect">Object Detection</option>
                    </select>
                </div>

                <div class="form-group">
                    <label>Prompt</label>
                    <textarea id="prompt" placeholder="e.g., Count the total number of objects and describe their context..."></textarea>
                </div>

                <button id="processBtn">Process Image</button>
                <div class="loader-container" id="loader">
                    <div class="spinner"></div>
                    <span>Processing through C++ Pybind11 Engine...</span>
                </div>
            </div>

            <!-- Right Panel: Output -->
            <div class="panel output-panel">
                <label>Inference Result</label>
                <div class="output-container" id="output">Waiting for input...</div>
            </div>
        </div>
    </div>

    <script>
        let base64Image = "";
        const imageInput = document.getElementById('imageInput');
        const preview = document.getElementById('preview');
        const uploadText = document.getElementById('uploadText');
        const dropZone = document.getElementById('dropZone');

        function handleFile(file) {
            if (!file) return;
            const reader = new FileReader();
            reader.onload = function(event) {
                base64Image = event.target.result;
                preview.src = base64Image;
                preview.style.display = 'block';
                uploadText.style.display = 'none';
                dropZone.style.padding = '0';
                dropZone.style.border = 'none';
            };
            reader.readAsDataURL(file);
        }

        imageInput.addEventListener('change', (e) => handleFile(e.target.files[0]));

        // Drag and drop handling
        dropZone.addEventListener('dragover', (e) => { e.preventDefault(); dropZone.style.borderColor = 'var(--primary)'; });
        dropZone.addEventListener('dragleave', () => { dropZone.style.borderColor = 'var(--border-color)'; });
        dropZone.addEventListener('drop', (e) => {
            e.preventDefault();
            dropZone.style.borderColor = 'var(--border-color)';
            if (e.dataTransfer.files.length) {
                imageInput.files = e.dataTransfer.files;
                handleFile(e.dataTransfer.files[0]);
            }
        });

        document.getElementById('processBtn').addEventListener('click', async function() {
            if (!base64Image) { alert("Please upload an image first."); return; }
            
            const category = document.getElementById('category').value;
            const prompt = document.getElementById('prompt').value;
            
            document.getElementById('loader').style.display = 'flex';
            document.getElementById('output').innerText = "Running inference...\nThis might take a moment depending on your hardware.";
            document.getElementById('processBtn').disabled = true;

            try {
                const response = await fetch('/api/process', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ image: base64Image, category: category, prompt: prompt })
                });
                
                const result = await response.json();
                if (result.error) {
                    document.getElementById('output').innerText = "Error:\n\n" + result.error;
                    document.getElementById('output').style.color = "#DC2626";
                } else {
                    document.getElementById('output').innerText = result.text;
                    document.getElementById('output').style.color = "#374151";
                }
            } catch (err) {
                document.getElementById('output').innerText = "Request failed:\n\n" + err;
                document.getElementById('output').style.color = "#DC2626";
            } finally {
                document.getElementById('loader').style.display = 'none';
                document.getElementById('processBtn').disabled = false;
            }
        });
    </script>
</body>
</html>
)HTML";


bool launch_app(
    const std::string& model_name,
    const std::string& architecture,
    int port
) {
    try {
        py::print("Initializing Python environment for Inference...");

        // Load Python modules
        py::module_ torch = py::module_::import("torch");
        py::module_ transformers = py::module_::import("transformers");

        // Compute Devices & Dtypes
        py::object cuda = torch.attr("cuda");
        std::string device = cuda.attr("is_available")().cast<bool>() ? "cuda" : "cpu";
        py::object dtype = torch.attr("float16");
        
        if (device == "cuda" && cuda.attr("is_bf16_supported")().cast<bool>()) {
            dtype = torch.attr("bfloat16");
        }

        py::print("Loading model:", model_name, "with architecture:", architecture);
        py::object ModelClass = transformers.attr(architecture.c_str());
        
        py::object model = ModelClass.attr("from_pretrained")(
            model_name,
            "torch_dtype"_a=dtype,
            "device_map"_a=device
        ).attr("eval")();

        py::object processor = transformers.attr("AutoProcessor").attr("from_pretrained")(model_name);
        
        py::print("Model loaded successfully. Starting Native C++ HTTP Server on port", port, "...");

        // Start HTTP Server
        httplib::Server svr;

        // Route: Serve Frontend GUI
        svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
            res.set_content(HTML_CONTENT, "text/html");
        });

        // Route: API Endpoint (Handled by Pybind)
        svr.Post("/api/process", [&model, &processor, &device](const httplib::Request& req, httplib::Response& res) {
            
            // Re-acquire the Global Interpreter Lock since httplib handles requests on background threads
            py::gil_scoped_acquire acquire; 
            
            try {
                py::module_ base64 = py::module_::import("base64");
                py::module_ io = py::module_::import("io");
                py::module_ PIL_Image = py::module_::import("PIL.Image");
                py::module_ json = py::module_::import("json");

                // Decode incoming JSON
                py::object parsed = json.attr("loads")(req.body);
                std::string image_b64_full = parsed["image"].cast<std::string>();
                std::string category = parsed["category"].cast<std::string>();
                std::string prompt = parsed["prompt"].cast<std::string>();

                // Strip data URI standard: 'data:image/jpeg;base64,'
                size_t comma_pos = image_b64_full.find(',');
                std::string image_b64 = (comma_pos != std::string::npos) ? image_b64_full.substr(comma_pos + 1) : image_b64_full;

                // Load image into PIL
                py::object image_bytes = base64.attr("b64decode")(image_b64);
                py::object bytes_io = io.attr("BytesIO")(image_bytes);
                py::object rgb_image = PIL_Image.attr("open")(bytes_io).attr("convert")("RGB");
                rgb_image.attr("thumbnail")(py::make_tuple(512, 512));

                // Process Prompt
                std::string full_prompt = prompt;
                if (category == "Caption") full_prompt = "Provide a " + prompt + " length caption for the image.";
                else if (category == "Point") full_prompt = "Provide 2d point coordinates for " + prompt + ". Report in JSON format.";
                else if (category == "Detect") full_prompt = "Provide bounding box coordinates for " + prompt + ". Report in JSON format.";

                py::list content;
                content.append(py::dict("type"_a="image", "image"_a=rgb_image));
                content.append(py::dict("type"_a="text", "text"_a=full_prompt));

                py::list messages;
                messages.append(py::dict("role"_a="user", "content"_a=content));

                py::object text_prompt = processor.attr("apply_chat_template")(
                    messages, "tokenize"_a=false, "add_generation_prompt"_a=true
                );

                py::list texts; texts.append(text_prompt);
                py::list images; images.append(rgb_image);
                
                py::object batch_encoding = processor(
                    "text"_a=texts, 
                    "images"_a=images, 
                    "return_tensors"_a="pt", 
                    "padding"_a=true
                );
                
                py::object inputs = batch_encoding.attr("to")(device);
                py::dict inputs_dict = py::dict(inputs);

                py::object generated_ids = model.attr("generate")(
                    **inputs_dict, 
                    "max_new_tokens"_a=512
                );

                // Strip the input prompt from generated output ids
                py::object input_ids = inputs.attr("get")("input_ids");
                py::int_ input_len = py::len(input_ids[py::int_(0)]);
                py::object output_ids = generated_ids[py::make_tuple(py::int_(0), py::slice(input_len, py::none(), py::none()))];
                
                // Final Decoding
                py::object out_text = processor.attr("decode")(
                    output_ids, 
                    "skip_special_tokens"_a=true, 
                    "clean_up_tokenization_spaces"_a=false
                );

                // Format successful return packet
                py::dict result;
                result["text"] = out_text;
                std::string json_res = json.attr("dumps")(result).cast<std::string>();
                
                res.set_content(json_res, "application/json");

            } catch (const std::exception& e) {
                std::string error_msg = std::string(R"({"error": ")") + e.what() + R"("})";
                res.set_content(error_msg, "application/json");
            }
        });

        std::string url = "http://localhost:" + std::to_string(port);
        std::cout << "\n=============================================\n";
        std::cout << "App ready! Automatically opening: " << url << "\n";
        std::cout << "=============================================\n\n";

        // Launch browser in a detached thread slightly delayed so the server is up
        std::thread([url]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
#if defined(_WIN32)
            std::string cmd = "start " + url;
            system(cmd.c_str());
#elif defined(__APPLE__)
            std::string cmd = "open " + url;
            system(cmd.c_str());
#else
            std::string cmd = "xdg-open " + url;
            system(cmd.c_str());
#endif
        }).detach();

        // Release the Python GIL for the duration of the server lifecycle so incoming requests can acquire it
        py::gil_scoped_release release; 

        // Block execution and run the server
        svr.listen("0.0.0.0", port);

        return true;
    } catch (const py::error_already_set& e) {
        std::cerr << "Python error occurred: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "C++ error occurred: " << e.what() << std::endl;
        return false;
    }
}

} // namespace vision_app