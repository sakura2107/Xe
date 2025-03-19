import xe.xeTesture.decode.private_Imgdecoder;

import std;

import xe.xeCore.xeOrdinals;
import xe.xeCore.xeBaseDataType;
import xe.xeCore.xeCoreClrOutput;

import xe.xeTesture.xeTestureCore;

#include "avif/avif.h"

namespace xe
{
	bool DecodeAVIF(std::unique_ptr<Testure> img, xeByte* avif_buffer, xeSize file_size)
	{
		avifRGBImage avif_rgb = { 0 };

		avifDecoder* decoder = avifDecoderCreate();
		if (decoder == NULL) {
			XE_WARNING_OUTPUT("LIB <DecodeAVIF> Memory allocation failure");
			return false;
		}

		avifResult result = avifDecoderSetIOMemory(decoder, avif_buffer, file_size);
		if (result != AVIF_RESULT_OK) {
			XE_WARNING_OUTPUT("LIB <DecodeAVIF> Cannot set IO on avifDecoder:");
			goto cleanup;
		}

		result = avifDecoderParse(decoder);
		if (result != AVIF_RESULT_OK) {
			XE_WARNING_OUTPUT(std::format("MOD <DecodeAVIF> Failed to decode image: {}", avifResultToString(result)).c_str());
			goto cleanup;
		}

		avifRGBImageSetDefaults(&avif_rgb, decoder->image);
		result = avifRGBImageAllocatePixels(&avif_rgb);
		if (result != AVIF_RESULT_OK) {
			XE_WARNING_OUTPUT(std::format("LIB <DecodeAVIF> allocation of RGB samples failed: {}\n", avifResultToString(result)).c_str());
			goto cleanup;
		}

		img = std::make_unique<Testure>(avif_rgb.width, avif_rgb.height, xeColorChannel::RGB);

		std::memcpy(img->pixel_data, avif_rgb.pixels, img->testure_size);
	cleanup:
		avifRGBImageFreePixels(&avif_rgb);
		avifDecoderDestroy(decoder);
		return false;
	}
}