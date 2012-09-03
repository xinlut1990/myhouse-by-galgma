#include "stdafx.h"
#include "sound1.h"
void PlayLoop(char* wavName,IDirectSound8 **ppDS,IDirectSoundBuffer8 *&pDSB)
{
	pDSB=LoadWAV(wavName,ppDS);
	if(pDSB!= NULL) {
		pDSB->SetCurrentPosition(0);
		pDSB->SetVolume(0);
		pDSB->Play(0,0,DSBPLAY_LOOPING);
	}
}
IDirectSoundBuffer8 *CreateBufferFromWAV(FILE *fp, sWaveHeader *Hdr,IDirectSound8 **ppDS)
{
	IDirectSoundBuffer *pDSB;
	IDirectSoundBuffer8 *pDSBuffer;
	DSBUFFERDESC dsbd;
	WAVEFORMATEX wfex;

	// read in the header from beginning of file
	fseek(fp, 0, SEEK_SET);
	fread(Hdr, 1, sizeof(sWaveHeader), fp);

	// check the sig fields, returning if an error
	if(memcmp(Hdr->RiffSig, "RIFF", 4) ||                     
		memcmp(Hdr->WaveSig, "WAVE", 4) ||                      
		memcmp(Hdr->FormatSig, "fmt ", 4) ||                    
		memcmp(Hdr->DataSig, "data",4))
		return NULL;

	// setup the playback format
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.nChannels = Hdr->Channels;
	wfex.nSamplesPerSec = Hdr->SampleRate;
	wfex.wBitsPerSample = Hdr->BitsPerSample;
	wfex.nBlockAlign = wfex.wBitsPerSample / 8 * wfex.nChannels;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

	// create the sound buffer using the header data
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = Hdr->DataSize;
	dsbd.lpwfxFormat = &wfex;
	if(FAILED((**ppDS).CreateSoundBuffer(&dsbd, &pDSB, NULL)))
		return NULL;

	// get newer interface
	if(FAILED(pDSB->QueryInterface(IID_IDirectSoundBuffer8,     
		(void**)&pDSBuffer))) {
			pDSB->Release();
			return NULL;
	}

	// return the interface
	return pDSBuffer;
}

BOOL LoadSoundData(IDirectSoundBuffer8 *pDSBuffer,            
				   long LockPos, FILE *fp, long Size)
{
	BYTE *Ptr1, *Ptr2;
	DWORD Size1, Size2;

	if(!Size)
		return FALSE;

	// lock the sound buffer at position specified
	if(FAILED(pDSBuffer->Lock(LockPos, Size,                    
		(void**)&Ptr1, &Size1,                          
		(void**)&Ptr2, &Size2, 0)))
		return FALSE;

	// read in the data
	fread(Ptr1, 1, Size1, fp);
	if(Ptr2 != NULL)
		fread(Ptr2, 1, Size2, fp);

	// unlock it
	pDSBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);

	// return a success
	return TRUE;
}

IDirectSoundBuffer8 *LoadWAV(char *Filename,IDirectSound8 **ppDS)
{
	IDirectSoundBuffer8 *pDSBuffer;
	sWaveHeader Hdr;
	FILE *fp;

	// open the source file
	if((fp=fopen(Filename, "rb"))==NULL)
		return NULL;

	// create the sound buffer
	if((pDSBuffer = CreateBufferFromWAV(fp, &Hdr,ppDS)) == NULL) {
		fclose(fp);
		return NULL;
	}

	// read in the data
	fseek(fp, sizeof(sWaveHeader), SEEK_SET);
	LoadSoundData(pDSBuffer, 0, fp, Hdr.DataSize);

	// close the source file
	fclose(fp);

	// return the new sound buffer fully loaded with sound
	return pDSBuffer;
}

void PlaySND(char* wavName,IDirectSound8 **ppDS,int volume,int flag)
{
	IDirectSoundBuffer8 *pDSBuffer=LoadWAV(wavName,ppDS);
	if(pDSBuffer!= NULL) {
		pDSBuffer->SetCurrentPosition(0);
		pDSBuffer->SetVolume(volume);
		pDSBuffer->Play(0,0,flag);
		pDSBuffer->Release();
	}
}

