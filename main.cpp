#include <iostream>
#include <cmath>
#include <thread>

using namespace std;
using namespace chrono;

#include <Windows.h>

int main() {
    DWORD d = 0;
    GetConsoleMode( GetStdHandle( STD_OUTPUT_HANDLE ), &d );
    SetConsoleMode( GetStdHandle( STD_OUTPUT_HANDLE ), d | 0x0004 );
    GetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), &d );
    SetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), d | 0x200 );

    auto tp1 = system_clock::now();
    auto tp2 = tp1;
    char s[ 80 * 30 ] = {};

    auto pfxy = []( const char *b, int x, int y ) { printf( "\x1B[%d;%dH%s", y, x, b ); };
    auto sbxy = [&]( int x, int y, char c ) { s[ y * 80 + x ] = c; };

    float t = 0.0f, f = 0.0f, tt = 0.0f; // Timing variables
    float w = 20.0f, m = 40.0f, tw = 20.0f, tm = 40.0f; // World shape variables
    float e = 1.0f, c = 40.0f; // Difficulty, Player Position
    int r = 0, h = 1000; // Player Score

    while( h > 0 ) {
        char n[ 10 ];
        // Timing
        this_thread::sleep_for( 10ms );
        tp2 = system_clock::now();
        duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        t += f = elapsedTime.count();
        tt += f;
        // Get and Handle Input
        for( int i = 0; i < 2; i++ )
            if( GetAsyncKeyState( R"(%')"[ i ] ) != 0 )
                if( !i )
                    c += 40.0f * -1 * f;
                else
                    c += 40.0f * 1 * f;
        // Update World
        if( t >= 1.5f ) {
            tw = rand() % 10 + 10;
            tm = rand() % ( 76 - ( static_cast<int>( tw ) >> 1 ) ) + 4;
        }
        e += f * 0.001f;
        w += ( tw - w ) * e * f;
        m += ( tm - m ) * e * f;
        float p = sinf( tt * e ) * m / 2.0f + 40.0f;
        // Fill Row
        for( int x = 0; x < 79; x++ )
            if( x + 1 < p - w / 2 || x > p + w / 2 ) sbxy( x, r, '.' );
            else if( x == static_cast<int>( p + ( w / 2 ) ) )
                sbxy( x, r, '#' );
            else sbxy( x, r, ' ' );
        // Scrolling Effect
        sbxy( 79, r, '\0' );
        ( r += 1 ) %= 28;
        // Draw To Screen
        for( int y = 1; y < 28; y++ )
            pfxy( &s[ ( ( y + 28 + r ) % 28 ) * 80 ], 0, y );
        pfxy( "O-V-O", c - 2, 2 );
        pfxy( " O-O ", c - 2, 3 );
        pfxy( "  V  ", c - 2, 4 );
        // Collision Checking and Health Update
        h -= s[ ( ( 4 + r ) % 28 ) * 80 + ( static_cast<int>( c ) - 2 ) ] == '.';

        // Draw HUD
        pfxy( "===============================================================================", 0, 28 );
        pfxy( "Cave Diver ~ Left Arrow / Right Arrow ~ Survive!", 2, 29 );
        _itoa_s( static_cast<int>( tt * 100.0f ), n, 10, 10 );
        pfxy( "Distance Fallen:             ", 2, 30 );
        pfxy( n, 19, 30 );

        _itoa_s( h, n, 10, 10 );
        pfxy( "Health:              ", 40, 30 );
        pfxy( n, 48, 30 );
    }

    pfxy( "Dead...\n", 70, 30 );
    system( "pause" );
    return 0;
}
