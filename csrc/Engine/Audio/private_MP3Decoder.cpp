﻿import xe.Audio.Instance.private_Audiodecoder;

import std;

import xe.Core.xeBaseDataType;
import xe.Core.CoreClrOutput;
import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;

import xe.Audio.AudioCore;

#include "minimp3/minimp3_ex.h"

namespace xe
{
	using MP3Type = mp3dec_ex_t;

	xeSize OpenMP3Data(std::unique_ptr<AudioEncodedData> mp3_data, xeAnyType dec_typpe)
	{
		xeSize pcm_size;

		MP3Type* dec_mp3_type = xeMalloc<MP3Type>(1);
		dec_mp3_type = xeMalloc<mp3dec_ex_t>(1);
		mp3dec_ex_open_buf(dec_mp3_type, mp3_data->data, mp3_data->_size, MP3D_SEEK_TO_SAMPLE);

		pcm_size = dec_mp3_type->samples * sizeof(mp3d_sample_t);
		dec_typpe = reinterpret_cast<xeAnyType>(dec_mp3_type);
		return pcm_size;
	}

	bool GetMP3Pcm(xeAnyType dec_typpe, std::unique_ptr<PcmBlock> pcmdata)
	{
		MP3Type* dec_mp3_type = reinterpret_cast<MP3Type*>(dec_typpe);
		auto readed = mp3dec_ex_read(dec_mp3_type, reinterpret_cast<xeInt16*>(pcmdata->data), dec_mp3_type->samples);
		if (readed != dec_mp3_type->samples)
		{
			if (dec_mp3_type->last_error)
			{
				XE_WARNING_OUTPUT("<LIB: MINIMP3> Read MP3 file is failed");
				return false;
			}
		}
		return true;
	}

	bool MP3Seek(xeAnyType dec_typpe, xeSize point)
	{
		MP3Type* dec_mp3_type = reinterpret_cast<MP3Type*>(dec_typpe);
		if (mp3dec_ex_seek(dec_mp3_type, point))
		{
			XE_WARNING_OUTPUT("<LIB: MINIMP3> Seek MP3 file is failed");
			return false;
		}
		return true;
	}

	void CloseMP3Data(xeAnyType dec_typpe)
	{
		MP3Type* dec_mp3_type = reinterpret_cast<MP3Type*>(dec_typpe);
		mp3dec_ex_close(dec_mp3_type);
		xeFree(dec_mp3_type);
	}
}