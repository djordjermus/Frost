#include "../include.hpp"
#include <xaudio2.h>
#include <wrl.h>
#include <thread>

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT  'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

FROST_API u32 _stdcall frost_api_audio_test(void* riff_fmt, void* riff_data, u64 data_byte_size)
{
	/*
	 * INIT
	 */

	HRESULT hr;
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IXAudio2> pXAudio2 = nullptr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;

	IXAudio2MasteringVoice* pMasterVoice = nullptr;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;
	/* * * SETUP * * */

	/*
	 * DECLARE STRUCTURES
	 */

	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	::memcpy(&wfx, riff_fmt, 16);
	/*
	 * Populate an XAUDIO2_BUFFER structure.
	 */

	buffer.AudioBytes = data_byte_size;			// size of the audio buffer in bytes
	buffer.pAudioData = (const u8*)riff_data;	// buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM;		// tell the source voice not to expect any data after this buffer

	/* * * PLAY SOUND * * */

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
		return hr;
	if (FAILED(hr = pSourceVoice->SetVolume(1.0f)))
		return hr;
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
		return hr;
	if (FAILED(hr = pSourceVoice->Start()))
		return hr;

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	pMasterVoice->DestroyVoice();
	pSourceVoice->DestroyVoice();

	return 0;
}
