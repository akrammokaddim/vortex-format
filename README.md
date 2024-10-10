# Vortex (.vtx) Format

The **Vortex (.vtx)** format is a custom file format designed to bundle video, thumbnail image, and metadata into a single, easy-to-manage package. This format was developed specifically for video streaming projects that require the storage of additional data (such as thumbnail images and metadata) alongside the video file. By combining these elements into one file, Vortex simplifies backend management while still allowing the video to be streamed seamlessly.

## Motivation

While working on a movie and video streaming project, I faced the challenge of storing video thumbnails and metadata (like tags and descriptions) separately in the database. Although existing formats like **MKV** (Matroska) support metadata embedding, they aren't fully supported by HTML video tags, making them less suitable for modern web streaming applications. To overcome this limitation, I created the **Vortex (.vtx)** format, which bundles an **MP4 video**, a **JPG thumbnail**, and **JSON metadata** into a single file.

## Features

- **Bundled Content**: Each `.vtx` file includes:
  - An **MP4 video** file for streaming.
  - A **JPG image** for use as a thumbnail.
  - A **JSON file** containing metadata such as tags, description, and other relevant information.
  
- **Backend Streaming Support**: The `.vtx` file can be processed on the backend to extract the video, thumbnail, and metadata, making it simple to serve the correct content without requiring multiple files or database lookups. This requires a custom backend function to handle streaming.

## Usage

### Creating Vortex Files

To create a `.vtx` file, you need to bundle three components:

1. **MP4 Video** - The primary video file.
2. **JPG Image** - The thumbnail that will represent the video.
3. **JSON Metadata** - Contains key details about the video, such as:
   - Title
   - Tags
   - Description
   - Duration
   - Any other relevant information

These three components are bundled together in the `.vtx` format for easy distribution and management.

### Streaming Vortex Files

On the backend, the `.vtx` file can be processed to extract the video for streaming and display the thumbnail and metadata. A dedicated backend function is required to handle the streaming of the video content from the bundled `.vtx` file.

## Command-Line Interface

The Vortex tool supports several commands for creating, extracting, and updating `.vtx` files. Below is a detailed usage guide for the command-line interface (CLI).

### Example Commands

#### 1. Convert to VTX

This command creates a `.vtx` file by bundling the video, thumbnail, and metadata together.

```bash
vortex convert -v <video.mp4> -i <thumbnail.jpg> -d <data.json> -o <output.vtx>
```

- `-v`: Path to the video file.
- `-i`: Path to the thumbnail image file.
- `-d`: Path to the JSON metadata file.
- `-o`: Path for the output `.vtx` file. If not provided, it defaults to the video file name with `.vtx` extension.

#### 2. Extract Content from VTX

This command extracts the video, image, or metadata from a `.vtx` file.

```bash
vortex extract -i <input.vtx> <video|image|data> --save <output_file>
```

- `-i`: Path to the input `.vtx` file.
- `<video|image|data>`: Type of content to extract.
- `--save`: Path to save the extracted file.

#### 3. Update VTX File

This command updates an existing `.vtx` file with new video, thumbnail, or metadata.

```bash
vortex update <input.vtx> -v <video.mp4> -i <thumbnail.jpg> -d <data.json>
```

- `<input.vtx>`: Path to the `.vtx` file to update.
- `-v`: New video file to update.
- `-i`: New thumbnail image file to update.
- `-d`: New JSON metadata file to update.

#### 4. Help Command

To display the help message with all available commands:

```bash
vortex help
```

## Benefits

- **Simplified Content Management**: Keep the video, thumbnail, and metadata in one file, avoiding the need for multiple separate files or database entries.
  
- **Custom Backend Requirement**: Unlike formats such as MKV, Vortex requires a custom backend solution for processing and streaming, allowing for tailored implementations based on specific project needs.

- **Efficient Backend Processing**: Easily extract the video, image, and metadata on the backend to serve the required components as needed.

## Example

Here's an example of the JSON metadata that could be included in a `.vtx` file:

```json
{
  "title": "Sample Video",
  "tags": ["action", "adventure", "drama"],
  "description": "This is a sample video description.",
  "duration": "1200",
  "thumbnail": "image.jpg"
}
```

## Conclusion

The **Vortex (.vtx)** format simplifies video streaming projects by bundling the video, thumbnail, and metadata into a single file. It requires an easy-to-implement backend function to manage streaming, making content management more straightforward.
