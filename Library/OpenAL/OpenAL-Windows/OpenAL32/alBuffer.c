/**
 * OpenAL cross platform audio library
 * Copyright (C) 1999-2000 by authors.
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA  02111-1307, USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */

#define _CRT_SECURE_NO_DEPRECATE // get rid of sprintf security warnings on VS2005

#include <stdlib.h>
#include <stdio.h>
#include "alMain.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "alError.h"
#include "alBuffer.h"

#ifdef _DEBUG
 char szDebug[256];
#endif

/*
*	AL Buffer Functions
*
*	AL Buffers are shared amoung Contexts, so we store the list of generated Buffers
*	as a global variable in this module.   (A valid context is not required to make
*	AL Buffer function calls
*
*/

/*
* Global Variables
*/

static ALbuffer *	g_pBuffers = NULL;			// Linked List of Buffers
static ALuint		g_uiBufferCount = 0;		// Buffer Count

static const long g_IMAStep_size[89]={			// IMA ADPCM Stepsize table
	   7,    8,    9,   10,   11,   12,   13,   14,   16,   17,   19,   21,   23,   25,   28,   31,
	  34,   37,   41,   45,   50,   55,   60,   66,   73,   80,   88,   97,  107,  118,  130,  143,
	 157,  173,  190,  209,  230,  253,  279,  307,  337,  371,  408,  449,  494,  544,  598,  658,
	 724,  796,  876,  963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
	3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493,10442,11487,12635,13899,
   15289,16818,18500,20350,22358,24633,27086,29794,32767
};

static const long g_IMACodeword_4[16]={			// IMA4 ADPCM Codeword decode table
	1, 3, 5, 7, 9, 11, 13, 15,
   -1,-3,-5,-7,-9,-11,-13,-15,
};

static const long g_IMAIndex_adjust_4[16]={		// IMA4 ADPCM Step index adjust decode table
   -1,-1,-1,-1, 2, 4, 6, 8,
   -1,-1,-1,-1, 2, 4, 6, 8
};

/*
*	alGenBuffers(ALsizei n, ALuint *puiBuffers)
*
*	Generates n AL Buffers, and stores the Buffers Names in the array pointed to by puiBuffers
*/
ALAPI ALvoid ALAPIENTRY alGenBuffers(ALsizei n,ALuint *puiBuffers)
{
	ALCcontext *Context;
	ALbuffer *ALBuf;
	ALsizei i=0;

	Context = alcGetCurrentContext();
	SuspendContext(Context);

	// Check that we are actually generation some Buffers
	if (n > 0)
	{
		// Check the pointer is valid (and points to enough memory to store Buffer Names)
		if (!IsBadWritePtr((void*)puiBuffers, n * sizeof(ALuint)))
		{
			// Is this the first Buffer created
			if (!g_pBuffers)
			{
				g_pBuffers = malloc(sizeof(ALbuffer));
				if (g_pBuffers)
				{
					memset(g_pBuffers, 0, sizeof(ALbuffer));
                    puiBuffers[i]=(ALuint)ALTHUNK_ADDENTRY(g_pBuffers);
					g_pBuffers->state=UNUSED;
					g_uiBufferCount++;
					i++;
				}
				ALBuf = g_pBuffers;
			}
			else
			{
				// Find last Buffer in list
				ALBuf = g_pBuffers;
				while (ALBuf->next)
					ALBuf=ALBuf->next;
			}

			// Create all the new Buffers
			while ((ALBuf)&&(i<n))
			{
				ALBuf->next = malloc(sizeof(ALbuffer));
				if (ALBuf->next)
				{
					memset(ALBuf->next, 0, sizeof(ALbuffer));
                    puiBuffers[i] = (ALuint)ALTHUNK_ADDENTRY(ALBuf->next);
					ALBuf->next->previous = ALBuf;
					ALBuf->next->state = UNUSED;
					g_uiBufferCount++;
					i++;
					ALBuf = ALBuf->next;
				}
				else
				{
					// Out of memory
					break;
				}
			}

			// If we didn't create all the Buffers, we must have run out of memory
			if (i != n)
			{
				alSetError(AL_OUT_OF_MEMORY);
			}
		}
		else
		{
			// Pointer does not point to enough memory to write Buffer names
			alSetError(AL_INVALID_VALUE);
		}
	}

	ProcessContext(Context);

	return;
}

/*
*	alDeleteBuffers(ALsizei n, ALuint *puiBuffers)
*
*	Deletes the n AL Buffers pointed to by puiBuffers
*/
ALAPI ALvoid ALAPIENTRY alDeleteBuffers(ALsizei n, const ALuint *puiBuffers)
{
	ALCcontext *Context;
	ALbuffer *ALBuf;
	ALsizei i;
	ALboolean bFailed = AL_FALSE;

	Context = alcGetCurrentContext();
	SuspendContext(Context);

	// Check we are actually Deleting some Buffers
	if (n > 0)
	{
		if ((ALuint)n <= g_uiBufferCount)
		{
			// Check that all the buffers are valid and can actually be deleted
			for (i = 0; i < n; i++)
			{
				// Check for valid Buffer ID (can be NULL buffer)
				if (alIsBuffer(puiBuffers[i]))
				{
					// If not the NULL buffer, check that the reference count is 0
                    ALBuf = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(puiBuffers[i]));
					if (ALBuf)
					{
						if (ALBuf->refcount != 0)
						{
							// Buffer still in use, cannot be deleted
							alSetError(AL_INVALID_OPERATION);
							bFailed = AL_TRUE;
						}
					}
				}
				else
				{
					// Invalid Buffer
					alSetError(AL_INVALID_NAME);
					bFailed = AL_TRUE;
				}
			}

			// If all the Buffers were valid (and have Reference Counts of 0), then we can delete them
			if (!bFailed)
			{
				for (i = 0; i < n; i++)
				{
                    ALBuf=((ALbuffer *)ALTHUNK_LOOKUPENTRY(puiBuffers[i]));
					if (ALBuf)
					{
						if (ALBuf->previous)
							ALBuf->previous->next=ALBuf->next;
						else
							g_pBuffers = ALBuf->next;

						if (ALBuf->next)
							ALBuf->next->previous = ALBuf->previous;

						// Release the memory used to store audio data
						if (ALBuf->data)
							free(ALBuf->data);

						// Release buffer structure
                        ALTHUNK_REMOVEENTRY(puiBuffers[i]);
                        memset(ALBuf, 0, sizeof(ALbuffer));
						g_uiBufferCount--;
						free(ALBuf);
                    }
                }
			}
		}
		else
		{
			alSetError(AL_INVALID_NAME);
		}
	}

	ProcessContext(Context);

	return;

}

/*
*	alIsBuffer(ALuint uiBuffer)
*
*	Checks if ulBuffer is a valid Buffer Name
*/
ALAPI ALboolean ALAPIENTRY alIsBuffer(ALuint uiBuffer)
{
	ALCcontext *Context;
	ALboolean result=AL_FALSE;
	ALbuffer *ALBuf;
    ALbuffer *TgtALBuf;
	unsigned int i;

	Context = alcGetCurrentContext();
	SuspendContext(Context);

	if (uiBuffer)
	{
		TgtALBuf = (ALbuffer *)ALTHUNK_LOOKUPENTRY(uiBuffer);

		// Check through list of generated buffers for uiBuffer
		ALBuf = g_pBuffers;
		for (i = 0; i < g_uiBufferCount; i++)
		{
			if (ALBuf == TgtALBuf)
			{
				result = AL_TRUE;
				break;
			}

			ALBuf = ALBuf->next;
		}
	}
	else
	{
		result = AL_TRUE;
	}


	ProcessContext(Context);

	return result;
}

/*
*	alBufferData(ALuint buffer,ALenum format,ALvoid *data,ALsizei size,ALsizei freq)
*
*	Fill buffer with audio data
*/
ALAPI ALvoid ALAPIENTRY alBufferData(ALuint buffer,ALenum format,const ALvoid *data,ALsizei size,ALsizei freq)
{
	ALuint *IMAData,IMACode;
	ALCcontext *Context;
	ALint Sample,Index;
	ALint LeftSample,LeftIndex;
	ALint RightSample,RightIndex;
	ALuint LeftIMACode,RightIMACode;
	ALbuffer *ALBuf;
	ALsizei i,j,k;

	Context = alcGetCurrentContext();
	SuspendContext(Context);

	if (alIsBuffer(buffer) && (buffer != 0))
	{
        ALBuf=((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));
		if ((ALBuf->refcount==0)&&(data))
		{
			switch(format)
			{
				case AL_FORMAT_MONO8:
					if ((size%1) == 0)
					{
						// 8bit Samples are converted to 16 bit here
						// Allocate 8 extra samples (16 bytes)
						ALBuf->data=realloc(ALBuf->data,16+(size/sizeof(ALubyte))*(1*sizeof(ALshort)));
						if (ALBuf->data)
						{
							ALBuf->format = AL_FORMAT_MONO16;
							ALBuf->eOriginalFormat = AL_FORMAT_MONO8;
							for (i=0;i<(ALsizei)(size/sizeof(ALubyte));i++)
								ALBuf->data[i]=(ALshort)((((ALubyte *)data)[i]-128)<<8);
							memset(&(ALBuf->data[size/sizeof(ALubyte)]), 0, 16);
							ALBuf->size=size/sizeof(ALubyte)*1*sizeof(ALshort);
							ALBuf->frequency=freq;
						}
						else
							alSetError(AL_OUT_OF_MEMORY);
					}
					else
						alSetError(AL_INVALID_VALUE);
					break;

				case AL_FORMAT_MONO16:
					if ((size%2) == 0)
					{
						// Allocate 8 extra samples (16 bytes)
						ALBuf->data=realloc(ALBuf->data,16+(size/sizeof(ALshort))*(1*sizeof(ALshort)));
						if (ALBuf->data)
						{
							ALBuf->format = AL_FORMAT_MONO16;
							ALBuf->eOriginalFormat = AL_FORMAT_MONO16;
							memcpy(ALBuf->data,data,size/sizeof(ALshort)*1*sizeof(ALshort));
							memset(&(ALBuf->data[size/sizeof(ALshort)]), 0, 16);
							ALBuf->size=size/sizeof(ALshort)*1*sizeof(ALshort);
							ALBuf->frequency=freq;
						}
						else
							alSetError(AL_OUT_OF_MEMORY);
					}
					else
						alSetError(AL_INVALID_VALUE);
					break;

				case AL_FORMAT_STEREO8:
					if ((size%2) == 0)
					{
						// 8bit Samples are converted to 16 bit here
						// Allocate 8 extra samples (32 bytes)
						ALBuf->data=realloc(ALBuf->data,32+(size/sizeof(ALubyte))*(1*sizeof(ALshort)));
						if (ALBuf->data)
						{
							ALBuf->format = AL_FORMAT_STEREO16;
							ALBuf->eOriginalFormat = AL_FORMAT_STEREO8;
							for (i=0;i<(ALsizei)(size/sizeof(ALubyte));i++)
								ALBuf->data[i]=(ALshort)((((ALubyte *)data)[i]-128)<<8);
							memset(&(ALBuf->data[size/sizeof(ALubyte)]), 0, 32);
							ALBuf->size=size/sizeof(ALubyte)*1*sizeof(ALshort);
							ALBuf->frequency=freq;
						}
						else
							alSetError(AL_OUT_OF_MEMORY);
					}
					else
						alSetError(AL_INVALID_VALUE);
					break;

				case AL_FORMAT_STEREO16:
					if ((size%4) == 0)
					{
						// Allocate 8 extra samples (32 bytes)
						ALBuf->data=realloc(ALBuf->data,32+(size/sizeof(ALshort))*(1*sizeof(ALshort)));
						if (ALBuf->data)
						{
							ALBuf->format = AL_FORMAT_STEREO16;
							ALBuf->eOriginalFormat = AL_FORMAT_STEREO16;
							memcpy(ALBuf->data,data,size/sizeof(ALshort)*1*sizeof(ALshort));
							memset(&(ALBuf->data[size/sizeof(ALshort)]), 0, 32);
							ALBuf->size=size/sizeof(ALshort)*1*sizeof(ALshort);
							ALBuf->frequency=freq;
						}
						else
							alSetError(AL_OUT_OF_MEMORY);
					}
					else
						alSetError(AL_INVALID_VALUE);
					break;

				case AL_FORMAT_MONO_IMA4:
					// Here is where things vary:
					// nVidia and Apple use 64+1 samples per block => block_size=36 bytes
					// Most PC sound software uses 2040+1 samples per block -> block_size=1024 bytes
					if ((size%36) == 0)
					{
						// Allocate 8 extra samples (16 bytes)
						ALBuf->data=realloc(ALBuf->data,16+(size/36)*(65*sizeof(ALshort)));
						if (ALBuf->data)
						{
							ALBuf->format = AL_FORMAT_MONO16;
							ALBuf->eOriginalFormat = AL_FORMAT_MONO_IMA4;
							IMAData=(ALuint *)data;
							for (i=0;i<size/36;i++)
							{
								Sample=((ALshort *)IMAData)[0]; 
								Index=((ALshort *)IMAData)[1];

								Index=Index<0?0:Index;
								Index=Index>88?88:Index;

								ALBuf->data[i*65]=(short)Sample;

								IMAData++;

								for (j=1;j<65;j+=8)
								{
									IMACode=*IMAData;
									for (k=0;k<8;k+=2)
									{
										Sample+=((g_IMAStep_size[Index]*g_IMACodeword_4[IMACode&15])/8);
										Index+=g_IMAIndex_adjust_4[IMACode&15];
										if (Sample<-32768) Sample=-32768;
										else if (Sample>32767) Sample=32767;
										if (Index<0) Index=0;
										else if (Index>88) Index=88;
										ALBuf->data[i*65+j+k]=(short)Sample;
										IMACode>>=4;

										Sample+=((g_IMAStep_size[Index]*g_IMACodeword_4[IMACode&15])/8);
										Index+=g_IMAIndex_adjust_4[IMACode&15];
										if (Sample<-32768) Sample=-32768;
										else if (Sample>32767) Sample=32767;
										if (Index<0) Index=0;
										else if (Index>88) Index=88;
										ALBuf->data[i*65+j+k+1]=(short)Sample;
										IMACode>>=4;
									}
									IMAData++;
								}
							}
							memset(&(ALBuf->data[(size/36*65)]), 0, 16);
							ALBuf->size=size/36*65*sizeof(ALshort);
							ALBuf->frequency=freq;
						}
						else
							alSetError(AL_OUT_OF_MEMORY);
					} 
					else
						 alSetError(AL_INVALID_VALUE);
					break;

				case AL_FORMAT_STEREO_IMA4:
					// Here is where things vary:
					// nVidia and Apple use 64+1 samples per channel per block => block_size=72 bytes
					// Most PC sound software uses 2040+1 samples per channel per block -> block_size=2048 bytes
					if ((size%72) == 0)
					{
						// Allocate 8 extra samples (32 bytes)
						ALBuf->data=realloc(ALBuf->data,32+(size/72)*(2*65*sizeof(ALshort)));
						if (ALBuf->data)
						{
							ALBuf->format = AL_FORMAT_STEREO16;
							ALBuf->eOriginalFormat = AL_FORMAT_STEREO_IMA4;
							IMAData=(ALuint *)data;
							for (i=0;i<size/72;i++)
							{
								LeftSample=((ALshort *)IMAData)[0]; 
								LeftIndex=((ALshort *)IMAData)[1];

								LeftIndex=LeftIndex<0?0:LeftIndex;
								LeftIndex=LeftIndex>88?88:LeftIndex;

								ALBuf->data[i*2*65]=(short)LeftSample;

								IMAData++;

								RightSample=((ALshort *)IMAData)[0]; 
								RightIndex=((ALshort *)IMAData)[1];

								RightIndex=RightIndex<0?0:RightIndex;
								RightIndex=RightIndex>88?88:RightIndex;

								ALBuf->data[i*2*65+1]=(short)RightSample;

								IMAData++;

								for (j=2;j<130;j+=16)
								{
									LeftIMACode=IMAData[0];
									RightIMACode=IMAData[1];
									for (k=0;k<16;k+=4)
									{
										LeftSample+=((g_IMAStep_size[LeftIndex]*g_IMACodeword_4[LeftIMACode&15])/8);
										LeftIndex+=g_IMAIndex_adjust_4[LeftIMACode&15];
										if (LeftSample<-32768) LeftSample=-32768;
										else if (LeftSample>32767) LeftSample=32767;
										if (LeftIndex<0) LeftIndex=0;
										else if (LeftIndex>88) LeftIndex=88;
										ALBuf->data[i*2*65+j+k]=(short)LeftSample;
										LeftIMACode>>=4;

										RightSample+=((g_IMAStep_size[RightIndex]*g_IMACodeword_4[RightIMACode&15])/8);
										RightIndex+=g_IMAIndex_adjust_4[RightIMACode&15];
										if (RightSample<-32768) RightSample=-32768;
										else if (RightSample>32767) RightSample=32767;
										if (RightIndex<0) RightIndex=0;
										else if (RightIndex>88) RightIndex=88;
										ALBuf->data[i*2*65+j+k+1]=(short)RightSample;
										RightIMACode>>=4;

										LeftSample+=((g_IMAStep_size[LeftIndex]*g_IMACodeword_4[LeftIMACode&15])/8);
										LeftIndex+=g_IMAIndex_adjust_4[LeftIMACode&15];
										if (LeftSample<-32768) LeftSample=-32768;
										else if (LeftSample>32767) LeftSample=32767;
										if (LeftIndex<0) LeftIndex=0;
										else if (LeftIndex>88) LeftIndex=88;
										ALBuf->data[i*2*65+j+k+2]=(short)LeftSample;
										LeftIMACode>>=4;
									
										RightSample+=((g_IMAStep_size[RightIndex]*g_IMACodeword_4[RightIMACode&15])/8);
										RightIndex+=g_IMAIndex_adjust_4[RightIMACode&15];
										if (RightSample<-32768) RightSample=-32768;
										else if (RightSample>32767) RightSample=32767;
										if (RightIndex<0) RightIndex=0;
										else if (RightIndex>88) RightIndex=88;
										ALBuf->data[i*2*65+j+k+3]=(short)RightSample;
										RightIMACode>>=4;
									}
									IMAData+=2;
								}
							}
							memset(&(ALBuf->data[(size/72*2*65)]), 0, 32);
							ALBuf->size=size/72*2*65*sizeof(ALshort);
							ALBuf->frequency=freq;
						}
						else
							alSetError(AL_OUT_OF_MEMORY);
					}
					else
						alSetError(AL_INVALID_VALUE);
					break;

				default:
					alSetError(AL_INVALID_ENUM);
					break;
			}
		}
		else
		{
			// Buffer is in use, or data is a NULL pointer
			alSetError(AL_INVALID_VALUE);
		}
	}
	else
	{
		// Invalid Buffer Name
		alSetError(AL_INVALID_NAME);
	}

	ProcessContext(Context);
}


ALAPI void ALAPIENTRY alBufferf(ALuint buffer, ALenum eParam, ALfloat flValue)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (alIsBuffer(buffer) && (buffer != 0))
	{
        pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

		switch(eParam)
		{
		default:
			alSetError(AL_INVALID_ENUM);
			break;
		}
	}
	else
	{
		alSetError(AL_INVALID_NAME);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alBuffer3f(ALuint buffer, ALenum eParam, ALfloat flValue1, ALfloat flValue2, ALfloat flValue3)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (alIsBuffer(buffer) && (buffer != 0))
	{
        pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

		switch(eParam)
		{
		default:
			alSetError(AL_INVALID_ENUM);
			break;
		}
	}
	else
	{
		alSetError(AL_INVALID_NAME);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alBufferfv(ALuint buffer, ALenum eParam, const ALfloat* flValues)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (alIsBuffer(buffer) && (buffer != 0))
	{
        pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

		switch(eParam)
		{
		default:
			alSetError(AL_INVALID_ENUM);
			break;
		}
	}
	else
	{
		alSetError(AL_INVALID_NAME);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alBufferi(ALuint buffer, ALenum eParam, ALint lValue)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (alIsBuffer(buffer) && (buffer != 0))
	{
        pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

		switch(eParam)
		{
		default:
			alSetError(AL_INVALID_ENUM);
			break;
		}
	}
	else
	{
		alSetError(AL_INVALID_NAME);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alBuffer3i( ALuint buffer, ALenum eParam, ALint lValue1, ALint lValue2, ALint lValue3)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (alIsBuffer(buffer) && (buffer != 0))
	{
        pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

		switch(eParam)
		{
		default:
			alSetError(AL_INVALID_ENUM);
			break;
		}
	}
	else
	{
		alSetError(AL_INVALID_NAME);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alBufferiv(ALuint buffer, ALenum eParam, const ALint* plValues)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (alIsBuffer(buffer) && (buffer != 0))
	{
        pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

		switch(eParam)
		{
		default:
			alSetError(AL_INVALID_ENUM);
			break;
		}
	}
	else
	{
		alSetError(AL_INVALID_NAME);
	}

	ProcessContext(pContext);
}


ALAPI ALvoid ALAPIENTRY alGetBufferf(ALuint buffer, ALenum eParam, ALfloat *pflValue)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (pflValue)
	{
		if (alIsBuffer(buffer) && (buffer != 0))
		{
            pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

			switch(eParam)
			{
			default:
				alSetError(AL_INVALID_ENUM);
				break;
			}
		}
		else
		{
			alSetError(AL_INVALID_NAME);
		}
	}
	else
	{
		alSetError(AL_INVALID_VALUE);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alGetBuffer3f(ALuint buffer, ALenum eParam, ALfloat* pflValue1, ALfloat* pflValue2, ALfloat* pflValue3)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if ((pflValue1) && (pflValue2) && (pflValue3))
	{
		if (alIsBuffer(buffer) && (buffer != 0))
		{
            pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

			switch(eParam)
			{
			default:
				alSetError(AL_INVALID_ENUM);
				break;
			}
		}
		else
		{
			alSetError(AL_INVALID_NAME);
		}
	}
	else
	{
		alSetError(AL_INVALID_VALUE);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alGetBufferfv(ALuint buffer, ALenum eParam, ALfloat* pflValues)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (pflValues)
	{
		if (alIsBuffer(buffer) && (buffer != 0))
		{
            pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

			switch(eParam)
			{
			default:
				alSetError(AL_INVALID_ENUM);
				break;
			}
		}
		else
		{
			alSetError(AL_INVALID_NAME);
		}
	}
	else
	{
		alSetError(AL_INVALID_VALUE);
	}

	ProcessContext(pContext);
}


ALAPI ALvoid ALAPIENTRY alGetBufferi(ALuint buffer, ALenum eParam, ALint *plValue)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (plValue)
	{
		if (alIsBuffer(buffer) && (buffer != 0))
		{
            pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

			switch (eParam)
			{
			case AL_FREQUENCY:
				*plValue = pBuffer->frequency;
				break;

			case AL_BITS:
				*plValue= (((pBuffer->format==AL_FORMAT_MONO8)||(pBuffer->format==AL_FORMAT_STEREO8))?8:16);
				break;

			case AL_CHANNELS:
				*plValue = (((pBuffer->format==AL_FORMAT_MONO8)||(pBuffer->format==AL_FORMAT_MONO16))?1:2);
				break;

			case AL_SIZE:
				*plValue = pBuffer->size;
				break;

			default:
				alSetError(AL_INVALID_ENUM);
				break;
			}
		}
		else
		{
			alSetError(AL_INVALID_NAME);
		}
	}
	else
	{
		alSetError(AL_INVALID_VALUE);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alGetBuffer3i(ALuint buffer, ALenum eParam, ALint* plValue1, ALint* plValue2, ALint* plValue3)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if ((plValue1) && (plValue2) && (plValue3))
	{
		if (alIsBuffer(buffer) && (buffer != 0))
		{
            pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

			switch(eParam)
			{
			default:
				alSetError(AL_INVALID_ENUM);
				break;
			}
		}
		else
		{
			alSetError(AL_INVALID_NAME);
		}
	}
	else
	{
		alSetError(AL_INVALID_VALUE);
	}

	ProcessContext(pContext);
}


ALAPI void ALAPIENTRY alGetBufferiv(ALuint buffer, ALenum eParam, ALint* plValues)
{
	ALCcontext	*pContext;
	ALbuffer	*pBuffer;

	pContext = alcGetCurrentContext();
	SuspendContext(pContext);

	if (plValues)
	{
		if (alIsBuffer(buffer) && (buffer != 0))
		{
            pBuffer = ((ALbuffer *)ALTHUNK_LOOKUPENTRY(buffer));

			switch (eParam)
			{
			case AL_FREQUENCY:
			case AL_BITS:
			case AL_CHANNELS:
			case AL_SIZE:
				alGetBufferi(buffer, eParam, plValues);
				break;

			default:
				alSetError(AL_INVALID_ENUM);
				break;
			}
		}
		else
		{
			alSetError(AL_INVALID_NAME);
		}
	}
	else
	{
		alSetError(AL_INVALID_VALUE);
	}

	ProcessContext(pContext);
}


/*
*	ReleaseALBuffers()
*
*	INTERNAL FN : Called by DLLMain on exit to destroy any buffers that still exist
*/
ALvoid ReleaseALBuffers(ALvoid)
{
	ALbuffer *ALBuffer;
	ALbuffer *ALBufferTemp;
	unsigned int i;

#ifdef _DEBUG
	if (g_uiBufferCount > 0)
	{
		sprintf(szDebug, "OpenAL32 : DllMain() %d Buffer(s) NOT deleted\n", g_uiBufferCount);
		OutputDebugString(szDebug);
	}
#endif

	ALBuffer = g_pBuffers;
	for (i = 0; i < g_uiBufferCount; i++)
	{
		// Release sample data
		if (ALBuffer->data)
			free(ALBuffer->data);

		// Release Buffer structure
		ALBufferTemp = ALBuffer;
		ALBuffer = ALBuffer->next;
		memset(ALBufferTemp, 0, sizeof(ALbuffer));
		free(ALBufferTemp);
	}
}
