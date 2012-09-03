#pragma once
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
// .WAV file header

typedef struct sWaveHeader {
	char  RiffSig[4];         // 'RIFF'
	long  WaveformChunkSize;  // 8
	char  WaveSig[4];         // 'WAVE'
	char  FormatSig[4];       // 'fmt ' (notice space after)
	long  FormatChunkSize;    // 16
	short FormatTag;          // WAVE_FORMAT_PCM
	short Channels;           // # of channels
	long  SampleRate;         // sampling rate
	long  BytesPerSec;        // bytes per second
	short BlockAlign;         // sample block alignment
	short BitsPerSample;      // bits per second
	char  DataSig[4];         // 'data'
	long  DataSize;           // size of waveform data
} sWaveHeader;

IDirectSoundBuffer8 *CreateBufferFromWAV(FILE *fp, sWaveHeader *Hdr,IDirectSound8 **ppDS);
BOOL LoadSoundData(IDirectSoundBuffer8 *pDSBuffer, long LockPos, FILE *fp, long Size);
IDirectSoundBuffer8 *LoadWAV(char *Filename,IDirectSound8 **ppDS);
void PlaySND(char* wavName,IDirectSound8 **ppDS,int volume=0,int flag=0);
void PlayLoop(char* wavName,IDirectSound8 **ppDS,IDirectSoundBuffer8 *&pDSB);

 