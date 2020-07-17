/*
 * High level prototypes for sound device aquisition and management.
 */
#ifndef AL_BACKENDS_ALC_BACKEND_H_
#define AL_BACKENDS_ALC_BACKEND_H_

#include "al_siteconfig.h"

#include <AL/al.h>
#include <AL/alc.h>

typedef enum
{
  ALC_OPEN_INPUT_,
  ALC_OPEN_OUTPUT_
} ALC_OpenMode;

/*
 * Various data, the details depend on the backend in question. Abstract.
 */
struct ALC_BackendPrivateData;

typedef struct ALC_BackendOpsStruct
{
  /*
   * Closes an (output or input) backend. Returns AL_TRUE if closing was
   * successful, AL_FALSE if the backend could not be closed for some
   * reason. This function is used to implement alcCloseDevice.
   */
  void (*close) (struct ALC_BackendPrivateData *privateData);

  /*
   * Informs a backend that it is about to get paused. This function is
   * currently unused, but should probably be used by alcSuspendContext.
   */
  void (*pause) (struct ALC_BackendPrivateData *privateData);

  /*
   * Informs a backend that it is about to get resumed. This function is
   * currently unused, but should probably be used by alcProcessContext.
   */
  void (*resume) (struct ALC_BackendPrivateData *privateData);

  /*
   * Sets the parameters for a backend. Because we follow a meet-or-exceed
   * policty, *deviceBufferSizeInBytes, *fmt, and *speed might be different from
   * the parameters initially passed, so the caller should check these after a
   * succesful call. Returns AL_TRUE if setting was successful, AL_FALSE if the
   * parameters could not be matched or exceeded. This function is used to
   * implement alcMakeContextCurrent(NON_NULL).
   */
  ALboolean (*setAttributes) (struct ALC_BackendPrivateData *privateData,
                              ALuint *deviceBufferSizeInBytes, ALenum *format,
                              ALuint *speed);

  /*
   * Writes a given interleaved array of sound data to an output backend. This
   * function is used to implement (a)sync_mixer_iterate.
   */
  void (*write) (struct ALC_BackendPrivateData *privateData, const void *data,
                 int bytesToWrite);

  /*
   * Captures data from an input backend into the given buffer. This function is
   * used to implement capture functionality.
   */
  ALsizei (*read) (struct ALC_BackendPrivateData *privateData, void *data,
                   int bytesToRead);

  /*
   * Returns the normalized volume for the given channel (main/PCM/CD) on an
   * output backend. This function is used to implement alcGetAudioChannel_LOKI.
   */
  ALfloat (*getAudioChannel) (struct ALC_BackendPrivateData *privateData,
                              ALuint channel);
  /*
   * Sets the normalized volume for the given channel (main/PCM/CD) on an output
   * backend. This function is used to implement alcSetAudioChannel_LOKI.
   */
  int (*setAudioChannel) (struct ALC_BackendPrivateData *privateData,
                          ALuint channel, ALfloat volume);
  /*
   * Gets the backend's name. The returned string must remain available and
   * constant through the life of the program. A valid non-empty string must be
   * returned. This function is used to fill the device specifier lists and map
   * "pretty names" to backends.
   */
  const ALCchar* (*getName)(struct ALC_BackendPrivateData *privateData);
} ALC_BackendOps;

/*
 * Returns a pointer to private backend data via the 4th argument, or NULL if no
 * such backend is available. In the former case, a pointer to a backend
 * function table suitable for reading or writing sound data is returned via the
 * 3rd argument. This function is used to implement alcOpenDevice.
 */
void alcBackendOpen_(const ALCchar *name, ALC_OpenMode mode, ALC_BackendOps **ops,
                     struct ALC_BackendPrivateData **privateData);

/*
 * Builds and returns a specifier list for the given mode (input or output).
 * This function is used to implement the various ALC device specifier strings.
 */
const ALCchar *_alcGetSpecifierList(ALC_OpenMode mode);
const ALCchar *_alcGetDefaultSpecifier(ALC_OpenMode mode);

/******************************************************************************/

void alcBackendOpenOSS_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                         struct ALC_BackendPrivateData **privateData);
void alcBackendOpenNative_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                            struct ALC_BackendPrivateData **privateData);
void alcBackendOpenALSA_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                          struct ALC_BackendPrivateData **privateData);
void alcBackendOpenARts_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                          struct ALC_BackendPrivateData **privateData);
void alcBackendOpenDMedia_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                            struct ALC_BackendPrivateData **privateData);
void alcBackendOpenESD_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                         struct ALC_BackendPrivateData **privateData);
void alcBackendOpenSDL_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                         struct ALC_BackendPrivateData **privateData);
void alcBackendOpenNull_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                          struct ALC_BackendPrivateData **privateData);
void alcBackendOpenWAVE_ (ALC_OpenMode mode, ALC_BackendOps **ops,
                          struct ALC_BackendPrivateData **privateData);

#endif /* AL_BACKENDS_ALC_BACKEND_H_ */
