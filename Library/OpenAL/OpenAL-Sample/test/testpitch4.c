#include "testlib.h"

#define WAVEFILE "sample.wav"

#define MIDDLEC 523.25
#define DEFFREQ 22050

/*            a c d e f g a b C D E F G A B            */
char *musicstr = "CEG 4 CEG 4 CEG 4";

static void init( const char *fname );

static ALuint chords[3] = { 0, 0, 0 };

static time_t start;
static void *cc;		/* al context */

static void init( const char *fname )
{
	ALfloat zeroes[] = { 0.0f, 0.0f, 0.0f };
	ALfloat front[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALuint boom;
	ALshort *data;
	const int canon_max = ( ( 1 << ( 16 - 1 ) ) - 1 );
	int i;

	start = time( NULL );

	alListenerfv( AL_POSITION, zeroes );
	alListenerfv( AL_VELOCITY, zeroes );
	alListenerfv( AL_ORIENTATION, front );

	alGenBuffers( 1, &boom );

	data = malloc( DEFFREQ * sizeof *data );
	if( data == NULL ) {
		exit( EXIT_FAILURE );
	}

	/* populate data with a concert A */
	for ( i = 0; i < DEFFREQ; i++ ) {
		data[i] = canon_max * sin( MIDDLEC * 2.0 * M_PI * i / DEFFREQ );
	}

	alBufferData( boom, AL_FORMAT_MONO16, data, DEFFREQ, DEFFREQ );
	free( data );		/* openal makes a local copy of wave data */

	alGenSources( 3, chords );

	for ( i = 0; i < 3; i++ ) {
		alSourcef( chords[i], AL_GAIN, 0.10 );
		alSourcei( chords[i], AL_BUFFER, boom );
		alSourcei( chords[i], AL_LOOPING, AL_FALSE );
	}
}

int main( int argc, char *argv[] )
{
	ALCdevice *device;
	int attributeList[] = { ALC_FREQUENCY, DEFFREQ,
		0
	};
	char *musicitr = musicstr;
	ALfloat pitch = 1.0;
	int beats;
	char note;
	int i;

	device = alcOpenDevice( NULL );
	if( device == NULL ) {
		return EXIT_FAILURE;
	}

	cc = alcCreateContext( device, attributeList );
	if( cc == NULL ) {
		alcCloseDevice( device );

		return EXIT_FAILURE;
	}

	alcMakeContextCurrent( cc );

	if( argc == 1 ) {
		init( WAVEFILE );
	} else {
		init( argv[1] );
	}

	while( *musicitr ) {
		alSourceStop( chords[0] );
		alSourceStop( chords[1] );
		alSourceStop( chords[2] );

		while( *musicitr == ' ' ) {
			musicitr++;
		}

		for ( i = 0; i < 3; i++ ) {
			switch ( *musicitr ) {
			case 'c':
				pitch = 0.500010;
				break;
			case 'd':
				pitch = 0.561223;
				break;
			case 'e':
				pitch = 0.629967;
				break;
			case 'f':
				pitch = 0.667425;
				break;
			case 'g':
				pitch = 0.749164;
				break;
			case 'a':
				pitch = 0.840898;
				break;
			case 'b':
				pitch = 0.943870;
				break;
			case 'C':
				pitch = 1.0;
				break;
			case 'D':
				pitch = 1.122465;
				break;
			case 'E':
				pitch = 1.259933;
				break;
			case 'F':
				pitch = 1.339704;
				break;
			case 'G':
				pitch = 1.498309;
				break;
			case 'A':
				pitch = 1.681796;
				break;
			case 'B':
				pitch = 1.897754;
				break;
			default:
				fprintf( stderr, "unknown note %d\n",
					 *musicitr );
				break;
			}

			note = *musicitr;
			musicitr++;	/* skip note */

			fprintf( stderr, "chord[%d] = %c\n", i, note );

			alSourcef( chords[i], AL_PITCH, pitch );
		}

		alSourcePlayv( 3, chords );

		while( *musicitr == ' ' ) {
			musicitr++;
		}

		beats = ( int ) *musicitr - '0';

		musicitr++;

		fprintf( stderr, "beats %d\n", beats );

		microSleep( beats / 4.0 * 1000000 );
	}

	alcDestroyContext( cc );
	alcCloseDevice( device );

	return EXIT_SUCCESS;
}
