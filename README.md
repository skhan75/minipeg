# MiniPeg Video Codec

## Overview

MiniPeg is a minimalistic video codec designed to showcase basic principles of video compression using discrete cosine transform (DCT) and run-length encoding (RLE). It provides a simple mechanism to perform video encoding and decoding on raw video frames.

## Key Features

1. **Color Space Conversion**: 
    - The codec supports videos in both RGB and YCbCr color spaces. Videos in RGB format are converted to YCbCr, leveraging the benefits of separating luminance and chrominance components.
    
2. **Quantization**: 
    - Uses quantization tables, including the standard JPEG luminance table, to compress DCT coefficients. Different quantization levels can be set to adjust the quality vs compression ratio trade-off. The decoding process includes dequantization to restore pixel values.
    
3. **DCT Transformation**: 
    - Implements Discrete Cosine Transform to convert video data into the frequency domain. This transformation facilitates better compression of the video data by focusing on the most significant frequency components.
    
4. **RLE (Run-Length Encoding)**: 
    - RLE is applied to further compress sequences of repeated values. For decoding, the RLE encoded data is expanded back to its original form.
    
5. **Modular Design**:
    - The project is structured into separate modules (`encoder`, `decoder`, `dct`, `color_conversion`) to ensure clarity and ease of maintenance.

## Usage

### Encoding

1. Convert raw video data into the desired format using the `encode_video` function. Pass the video data, configuration, quantization level, and block size (for DCT).
   
2. If the video is in RGB format, it will be automatically converted to YCbCr color space for optimal encoding.

### Decoding

1. Use the `decode_video` function by passing the encoded data and configuration.
   
2. The function will expand the RLE data, apply dequantization, and return the original video buffer.

## Future Improvements

- Incorporation of more advanced entropy encoding techniques, such as Huffman coding.
- Handling of edge cases where the video buffer size isn't a multiple of the frame size.
- Enhanced error handling and input validation for robustness.

## Conclusion

MiniPeg offers a basic yet illustrative demonstration of fundamental video encoding and decoding techniques. It serves as a springboard for those eager to delve into the world of video codecs, providing hands-on experience and a foundation for more advanced explorations.
