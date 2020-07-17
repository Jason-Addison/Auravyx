#include "testlib.h"

#define WAVEFILE "boom.wav"

static void iterate( void );

static ALuint movingSource = 0;

static time_t start;

static void iterate( void )
{
	if( sourceIsPlaying( movingSource ) == AL_FALSE ) {
		alSourcePlay( movingSource );
		fprintf( stderr, "have to sourceplay\n" );
	} else {
		microSleep( 10000 );
	}
}

static void init( const ALbyte *fname )
{
	ALfloat zeroes[] = { 0.0f, 0.0f, 0.0f };
	ALfloat back[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	ALfloat front[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALfloat position[] = { 0.0f, 0.0f, -4.0f };
	ALuint boom;

	start = time( NULL );

	alListenerfv( AL_POSITION, zeroes );
	alListenerfv( AL_VELOCITY, zeroes );
	alListenerfv( AL_ORIENTATION, front );

	boom = CreateBufferFromFile( fname );

	alGenSources( 1, &movingSource );

	alSourcei( movingSource, AL_BUFFER, boom );
	alSourcei( movingSource, AL_LOOPING, AL_FALSE );
	alSourcefv( movingSource, AL_POSITION, position );
	alSourcefv( movingSource, AL_VELOCITY, zeroes );
	alSourcefv( movingSource, AL_ORIENTATION, back );
	alSourcef( movingSource, AL_PITCH, 1.0f );
}

int main( int argc, char *argv[] )
{
	time_t shouldend;

	/* Initialize ALUT. */
	testInit( &argc, argv );

	init( ( const ALbyte * ) ( ( argc == 1 ) ? WAVEFILE : argv[1] ) );

	alSourcePlay( movingSource );
	while( 1 ) {
		shouldend = time( NULL );
		if( ( shouldend - start ) > 40 ) {
			break;
		}

		iterate(  );
	}

	testExit(  );

	return EXIT_SUCCESS;
}
