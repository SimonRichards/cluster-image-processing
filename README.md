# A distributed approach to image processing

## Figures
Image size = sizeof(double) * num_channels * width * height = (8 * 3 * 1024 * 1024) = 25,165,824 bytes = 24 megabytes
Size of dataset @ 10 million images = 24 megabytes * 10 000 000 = 228.881836 terabytes
Cost of 24 Teslas and 16 CPUs - USD$99,000: http://developer.nvidia.com/content/gpu-starter-kit-stirs-excitement

Import into Amazon's Elastic Block Storage product 
Total Terabytes to Load	 TB & 228.88 \\
Number of Devices & 46 (using a hard drive size of 5TB) \\
Wipe Device After Import & No

Estimated Transfer Speed  \\
Interface Type & eSATA \\
Transfer Speed = 100.00 MB/sec

Return Shipping \\
Return Shipping Destination	 & China
Total Weight of Packages Shipped & 50lbs

Estimated AWS Import Costs \\
Device Charges & \$3680.00 - 46 devices \\
Data Loading Charges & \$2.49 X 690 hour(s) = \$1718.10 \\
Data Wiping Charges & $0.00 \\
Return Shipping Charges & $117.56 \\
Total Estimated Charges & $5515.66\\