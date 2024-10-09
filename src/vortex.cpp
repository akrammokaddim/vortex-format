#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "nlohmann/json.hpp"
#include <cstring>

using json = nlohmann::json;

// Function Prototypes
void create_vtx(const std::string &video_path, const std::string &thumb_path, const json &json_data, const std::string &output_path);
void extract_vtx(const std::string &vtx_path, const std::string &type, const std::string &output_path);
void read_vtx_json(const std::string &vtx_path);
void update_vtx(const std::string &vtx_path, const std::string &video_path, const std::string &thumb_path, const json &json_data);


void print_help() {
    std::cout << "=======================================\n";
    std::cout << "           vortex Command Line          \n";
    std::cout << "=======================================\n";
    std::cout << "Usage: vortex <command> [options]\n\n";

    std::cout << "Commands:\n";
    
    std::cout << "  update <input.vtx> -d <data.json> -v <video.mp4> -i <image.jpg>\n";
    std::cout << "      Update an existing .vtx file with new video, thumbnail, and/or JSON data.\n";
    std::cout << "      Options:\n";
    std::cout << "        -d <data.json> : Specify the JSON data file (optional).\n";
    std::cout << "        -v <video.mp4> : Specify the new video file (optional).\n";
    std::cout << "        -i <image.jpg> : Specify the new thumbnail image file (optional).\n\n";

    std::cout << "  extract <input.vtx> -t <type> -o <output.file>\n";
    std::cout << "      Extract video, thumbnail, or JSON data from a .vtx file.\n";
    std::cout << "      Options:\n";
    std::cout << "        -t <type> : Specify the type to extract (video, image, data).\n";
    std::cout << "        -o <output.file> : Specify the output file name.\n\n";

    std::cout << "  convert <input.file> <output.vtx>\n";
    std::cout << "      Convert a regular video file and thumbnail into a .vtx file.\n";
    std::cout << "      Options:\n";
    std::cout << "        <input.file> : Specify the input video file (with thumbnail).\n";
    std::cout << "        <output.vtx> : Specify the output .vtx file name.\n\n";

    std::cout << "  help, -h, --help : Display this help message.\n";
    
    std::cout << "\nExamples:\n";
    std::cout << "  vortex update video.vtx -d data.json -v new_video.mp4 -i new_image.jpg\n";
    std::cout << "  vortex extract video.vtx -t video -o extracted_video.mp4\n";
    std::cout << "  vortex convert my_video.mp4 my_video.vtx\n";
    std::cout << "=======================================\n";
}



// Helper to read the file into a vector of bytes
std::vector<char> read_file(const std::string &file_path) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    std::vector<char> data;
    if (file) {
        size_t size = file.tellg();
        data.resize(size);
        file.seekg(0, std::ios::beg);
        file.read(data.data(), size);
    }
    return data;
}

// Write file from a byte vector
void write_file(const std::string &file_path, const std::vector<char> &data) {
    std::ofstream file(file_path, std::ios::binary);
    if (file) {
        file.write(data.data(), data.size());
    }
}


// Create a .vtx file
void create_vtx(const std::string &video_path, const std::string &thumb_path, const json &json_data, const std::string &output_path) {
    std::vector<char> video_data = read_file(video_path);
    std::vector<char> thumb_data = read_file(thumb_path);
    std::string json_str = json_data.dump();

    size_t video_len = video_data.size();
    size_t thumb_len = thumb_data.size();
    size_t json_len = json_str.size();

    // Improved print statements for better readability
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "          Creating .vtx File            " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Video Size      : " << video_len << " bytes" << std::endl;
    std::cout << "Thumbnail Size  : " << thumb_len << " bytes" << std::endl;
    std::cout << "JSON Size       : " << json_len << " bytes" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    std::ofstream output(output_path, std::ios::binary);
    if (output) {
        // Write the lengths of the components
        output.write(reinterpret_cast<const char *>(&video_len), sizeof(video_len));
        output.write(reinterpret_cast<const char *>(&thumb_len), sizeof(thumb_len));
        output.write(reinterpret_cast<const char *>(&json_len), sizeof(json_len));

        // Write the actual data
        output.write(video_data.data(), video_len);
        output.write(thumb_data.data(), thumb_len);
        output.write(json_str.c_str(), json_len);

        // Check if all data was written
        if (output.fail()) {
            std::cerr << "Error writing to the output file: " << output_path << std::endl;
            return;
        }

        std::cout << "------------------------------------------" << std::endl;
        std::cout << "Successfully created vortex file: " << output_path << std::endl;
        std::cout << "------------------------------------------" << std::endl;
    } else {
        std::cerr << "Failed to open output file for writing: " << output_path << std::endl;
    }
}


// Extract individual components
void extract_vtx(const std::string &vtx_path, const std::string &type, const std::string &output_path) {
    std::ifstream vtx_file(vtx_path, std::ios::binary);
    if (!vtx_file) {
        std::cerr << "Failed to open vortex file: " << vtx_path << std::endl;
        return;
    }

    size_t video_len, thumb_len, json_len;
    vtx_file.read(reinterpret_cast<char *>(&video_len), sizeof(video_len));
    vtx_file.read(reinterpret_cast<char *>(&thumb_len), sizeof(thumb_len));
    vtx_file.read(reinterpret_cast<char *>(&json_len), sizeof(json_len));

    // Prepare buffers for data extraction
    std::vector<char> video_data(video_len);
    std::vector<char> thumb_data(thumb_len);
    std::vector<char> json_data(json_len);

    // Read the actual data
    vtx_file.read(video_data.data(), video_len);
    vtx_file.read(thumb_data.data(), thumb_len);
    vtx_file.read(json_data.data(), json_len);

    // Enhanced print statements for better readability
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "          Extracting from vortex File      " << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    if (type == "video") {
        write_file(output_path, video_data);
        std::cout << "Successfully extracted video to: " << output_path << std::endl;
    } else if (type == "image") {
        write_file(output_path, thumb_data);
        std::cout << "Successfully extracted image to: " << output_path << std::endl;
    } else if (type == "data") {
        std::ofstream json_file(output_path);
        json_file.write(json_data.data(), json_len);
        std::cout << "Successfully extracted JSON data to: " << output_path << std::endl;
    } else {
        std::cerr << "Unknown extraction type: " << type << std::endl;
    }

    std::cout << "------------------------------------------" << std::endl;
}


// Update an existing .vtx file
void update_vtx(const std::string &vtx_path, const std::string &video_path, const std::string &thumb_path, const json &json_data) {
    std::ifstream vtx_file(vtx_path, std::ios::binary);
    if (!vtx_file) {
        std::cerr << "Failed to open vortex file: " << vtx_path << std::endl;
        return;
    }

    size_t video_len, thumb_len, json_len;
    vtx_file.read(reinterpret_cast<char *>(&video_len), sizeof(video_len));
    vtx_file.read(reinterpret_cast<char *>(&thumb_len), sizeof(thumb_len));
    vtx_file.read(reinterpret_cast<char *>(&json_len), sizeof(json_len));

    std::vector<char> current_video_data(video_len);
    std::vector<char> current_thumb_data(thumb_len);
    std::vector<char> current_json_data(json_len);
    
    // Read current data
    vtx_file.read(current_video_data.data(), video_len);
    vtx_file.read(current_thumb_data.data(), thumb_len);
    vtx_file.read(current_json_data.data(), json_len);

    // Update only if new files are provided
    if (!video_path.empty()) {
        current_video_data = read_file(video_path);
        video_len = current_video_data.size();
    } else {
        std::cout << "No new video provided, keeping existing video data." << std::endl;
    }

    if (!thumb_path.empty()) {
        current_thumb_data = read_file(thumb_path);
        thumb_len = current_thumb_data.size();
    } else {
        std::cout << "No new thumbnail provided, keeping existing thumbnail data." << std::endl;
    }

    // Use empty JSON if no data provided
    std::string json_str = json_data.empty() ? "{}" : json_data.dump();
    json_len = json_str.size();

    // Create a new .vtx file with updated data
    std::ofstream output(vtx_path, std::ios::binary);
    if (output) {
        output.write(reinterpret_cast<const char *>(&video_len), sizeof(video_len));
        output.write(reinterpret_cast<const char *>(&thumb_len), sizeof(thumb_len));
        output.write(reinterpret_cast<const char *>(&json_len), sizeof(json_len));
        output.write(current_video_data.data(), video_len);
        output.write(current_thumb_data.data(), thumb_len);
        output.write(json_str.c_str(), json_len);

        // Check if all data was written successfully
        if (output.fail()) {
            std::cerr << "Error writing to the vortex file: " << vtx_path << std::endl;
            return;
        }

        // Success message
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "          Updated vortex File              " << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "Video size: " << video_len << " bytes" << std::endl;
        std::cout << "Thumbnail size: " << thumb_len << " bytes" << std::endl;
        std::cout << "JSON size: " << json_len << " bytes" << std::endl;
        std::cout << "Successfully updated vortex file: " << vtx_path << std::endl;
        std::cout << "------------------------------------------" << std::endl;
    } else {
        std::cerr << "Failed to open vortex file for writing: " << vtx_path << std::endl;
    }
}

std::string get_video_name(const std::string &video_path) {
    return video_path.substr(0, video_path.find_last_of('.'));
}


// Parse command-line arguments
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: vortex <command> [options]" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "convert") {
        std::string video_path, thumb_path, json_path, output_path;

        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-v" && i + 1 < argc) {
                video_path = argv[++i];
            } else if (arg == "-i" && i + 1 < argc) {
                thumb_path = argv[++i];
            } else if (arg == "-d" && i + 1 < argc) {
                json_path = argv[++i];
            } else if (arg == "-o" && i + 1 < argc) {
                output_path = argv[++i];
            }
        }

        // Use empty JSON if not provided
        json j = json_path.empty() ? json::object() : json::parse(read_file(json_path));

        // Set default output path if not provided
        if (video_path.empty()) {
            std::cerr << "Invalid arguments for convert. Usage: vortex convert -v <video.mp4> -i <thumbnail.jpg> -d <data.json> -o <output.vtx>" << std::endl;
            return 1;
        }

        if (output_path.empty()) {
            output_path = get_video_name(video_path) + ".vtx"; // Create default .vtx name
        }

        create_vtx(video_path, thumb_path, j, output_path);

    } else if (command == "extract") {
        std::string vtx_path, type, output_path;

        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                vtx_path = argv[++i];
            } else if (arg == "--save" && i + 1 < argc) {
                output_path = argv[++i];
            } else if (arg == "video" || arg == "image" || arg == "data") {
                type = arg;  // video/image/data
            }
        }

        if (vtx_path.empty() || type.empty()) {
            std::cerr << "Invalid arguments for extract. Usage: vortex extract -i <input.vtx> <video|image|data> --save <output_file>" << std::endl;
            return 1;
        }

        // Set default output path if not provided
        if (output_path.empty()) {
            // Derive the output file name based on the type
            std::string extension;
            if (type == "video") {
                extension = ".mp4";
            } else if (type == "image") {
                extension = ".jpg";
            } else if (type == "data") {
                extension = ".json";
            } else {
                std::cerr << "Invalid type specified for extraction. Use video|image|data." << std::endl;
                return 1;
            }
            output_path = get_video_name(vtx_path) + extension; // Create default output name based on vtx name
        }

        extract_vtx(vtx_path, type, output_path);

    } else if (command == "update") {
        std::string vtx_path, video_path, thumb_path, json_path;

        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-v" && i + 1 < argc) {
                video_path = argv[++i];
            } else if (arg == "-i" && i + 1 < argc) {
                thumb_path = argv[++i];
            } else if (arg == "-d" && i + 1 < argc) {
                json_path = argv[++i];
            } else {
                vtx_path = arg; // last argument is always the vtx path
            }
        }

        // Read json data
        json j = json_path.empty() ? json::object() : json::parse(read_file(json_path));

        if (vtx_path.empty()) {
            std::cerr << "Invalid arguments for update. Usage: vortex update <input.vtx> -d <data.json> -v <video.mp4> -i <image.jpg>" << std::endl;
            return 1;
        }

        update_vtx(vtx_path, video_path, thumb_path, j);

    } else if (command == "help" || command == "-h" || command == "--help") {
        print_help();
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        std::cerr << "Please use -h or --help to see the list of available commands." << std::endl;
    }

    return 0;
}