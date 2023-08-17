# Minipeg
## Mini video processing library like ffmpeg

### Video Encoder
- Goal is to represent same visual information using fewer bits.
- Rudimentary Video Encoding Ideas:
    - Frame Difference
        - Only store the difference between consecutive frames instead of full frames. For static scenes, the difference would be minimal, resulting in considerable data savings.
    - Quantization
        - Reduce the precision of pixel values. For example, if a pixel's RGB value ranges from 0-255, reduce its precision by only allowing multiples of 4 or 8, leading to data compression but reduced quality.
    - Simple Run Length Encoding (RLE)
        - If there are consecutive pixels of the same color, store the color once followed by a count.
- encoder.c
    - Step-1: Quantization (using multiples of 4 for simplicity)
        - Quantization, in general, is the process of constraining or reducing the number of possible values in a dataset, typically to compress the data.
        - In the context of our rudimentary video encoder, the quantization process aims to simplify the color values to make them more amenable to compression.
        
        ```jsx
        for (int i = 0; i < buffer_size; i++) {
            raw_buffer[i] = (raw_buffer[i] / 4) * 4;
        }
        ```
        
        - Here's what's happening in this loop:
            1. We iterate over each byte in the **`raw_buffer`**.
            2. For each byte (representing a color value ranging from 0 to 255), we divide it by 4 and then multiply the result by 4.
        - What does this achieve?
            - The division by 4 followed by multiplication is a way of rounding down the value to the nearest multiple of 4. This effectively reduces the precision of color values.
            - For a concrete example, consider a single byte value of 157:
                - Dividing by 4 gives 157/4 =39.25
                - When we convert this to an integer (in C, integer division automatically floors the result), we get 39.
                - Multiplying by 4 gives 39×4=156.
                
                Thus, a value of 157 gets "quantized" to 156. Similarly:
                
                - 158, 159, and 160 would all get quantized to 156.
                - 161, 162, 163, and 164 would get quantized to 160.
                
                And so on.
                
        - Why is this useful?
            - By reducing the number of unique values (in this case, mapping every group of 4 values to a single representative value), we're simplifying the data. This simplification can make subsequent compression steps, like Run-Length Encoding (RLE), more effective because we're more likely to have longer runs of identical values.
            - However, it's important to understand that this type of quantization introduces a loss of information. The process is irreversible; once you've quantized the data, you can't get back the original values. In real-world video encoding, quantization is used in more sophisticated ways and is one of the steps that can lead to a loss of visual quality, especially if done aggressively.