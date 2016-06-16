//-----------------------------------------------------------------------------
// File: clipping.cpp
// Desc: sample shows the usage of clipping planes in OpenGL
//-----------------------------------------------------------------------------
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void idleFunc( );
void displayFunc( );
void reshapeFunc( GLsizei width, GLsizei height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void initialize( );

//-----------------------------------------------------------------------------
// global variables and #defines
//-----------------------------------------------------------------------------
#define INC_VAL     1.0f
#ifndef M_PI
#define M_PI        3.14159265359
#endif

// width and height of the window
     GLsizei g_width = 480;
     GLsizei g_height = 360;
// whether to animate
     GLboolean g_rotate = GL_TRUE;
// fill mode


GLenum g_fillmode = GL_FILL;
// light 0 position
     GLfloat g_light0_pos[4] = { 2.0f, 1.2f, 4.0f, 1.0f };
// light 1 parameters
     GLfloat g_light1_ambient[] = { .2f, .2f, .2f, 1.0f };
     GLfloat g_light1_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
     GLfloat g_light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
     GLfloat g_light1_pos[4] = { -2.0f, 0.0f, -4.0f, 1.0f };

// toggle each of 3 clipping planes
GLboolean g_clip1 = GL_FALSE;
GLboolean g_clip2 = GL_FALSE;
GLboolean g_clip3 = GL_FALSE;

// clipping planes
GLdouble eqn1[4] = { 1.0, 0.0, 0.0, 0.0 };
GLdouble eqn2[4] = { 0.0, 1.0, 0.0, 0.0 };
GLdouble eqn3[4] = { 0.0, 0.0, 1.0, 0.0 };

// modelview stuff
GLfloat g_angle_y = 32.0f;
GLfloat g_inc = 0.0f;
GLfloat g_eye_y = 0;

// translation for the clipping planes
GLfloat g_clip_x = 0.0f;
GLfloat g_clip_y = 0.0f;
GLfloat g_clip_z = 0.0f;

//-----------------------------------------------------------------------------
// Name: main( )
// Desc: entry point
//-----------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    // initialize GLUT
    glutInit( &argc, argv );
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( g_width, g_height );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow( "Clipping Planes in OpenGL" );
    // set the idle function - called when idle
    glutIdleFunc( g_rotate ? idleFunc : NULL );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
    // do our own initialization
    initialize();
    // let GLUT handle the current thread from here
    glutMainLoop();
    return 0;
}
//-----------------------------------------------------------------------------
// Name: initialize( )
// Desc: sets initial OpenGL states and initializes any application data
//-----------------------------------------------------------------------------
void initialize()
{
    // set the GL clear color - use when the color buffer is cleared
    glClearColor( 0.0f, 0.0f,0.0f, 1.0f );
    // set the shading model to 'smooth'
    glShadeModel( GL_SMOOTH );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    // set the front faces of polygons
    glFrontFace( GL_CCW );
    // set fill mode
    glPolygonMode( GL_FRONT_AND_BACK, g_fillmode );
    // enable lighting
    glEnable( GL_LIGHTING );
    // enable lighting for front
    glLightModeli( GL_FRONT, GL_TRUE );
    // material have diffuse and ambient lighting
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    // enable color
    glEnable( GL_COLOR_MATERIAL );
    // enable light 0
    glEnable( GL_LIGHT0 );
    // setup and enable light 1
    glLightfv( GL_LIGHT1, GL_AMBIENT, g_light1_ambient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, g_light1_diffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR, g_light1_specular );
    glEnable( GL_LIGHT1 );
    printf( "----------------------------------------------------\n" );
    printf( "Clipping Planes sample in OpenGL\n" );
    printf( "----------------------------------------------------\n" );
    printf( "'1' - toggle x=0 halfplane\n" );
    printf( "'2' - toggle y=0 halfplane\n" );
    printf( "'3' - toggle z=0 halfplane\n" );
    printf( "'j', 'l' - translate x=0 halfplane (when toggled)\n" );



  printf( "',', 'i' - translate y=0 halfplane (when toggled)\n" );
    printf( "'u', 'm' - translate z=0 halfplane (when toggled)\n" );
    printf( "'x', 'y', 'z' - reverse the corresponding half plane\n" );
    printf( "\n" );
    printf( "'-', '=' - rotate about y-axis\n" );
    printf( "(L/R) mouse buttons - rotate about y-axis\n" );
    printf( "'[', ']' - rotate viewpoint about x-axis\n" );
    printf( "'f' - toggle fill/wireframe drawmode\n" );
    printf( "----------------------------------------------------\n" );
    printf( "\n" );
}
//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( GLsizei w, GLsizei h )
{
    // save the new window size
    g_width = w; g_height = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( 45.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0 );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
    // position the view point


   gluLookAt( 0.0f, 3.5f * sin( g_eye_y ), 3.5f * cos( g_eye_y ),
               0.0f, 0.0f, 0.0f,
               0.0f, ( cos( g_eye_y ) < 0 ? -1.0f : 1.0f ), 0.0f );
    // set the position of the lights
    glLightfv( GL_LIGHT0, GL_POSITION, g_light0_pos );
    glLightfv( GL_LIGHT1, GL_POSITION, g_light1_pos );
}
//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
    case 'f':
        g_fillmode = ( g_fillmode == GL_FILL ? GL_LINE : GL_FILL );
        glPolygonMode( GL_FRONT_AND_BACK, g_fillmode );
    break;
    // toggle the 3 clipping planes
    case '1':
        g_clip1 = !g_clip1;
    break;
    case '2':
        g_clip2 = !g_clip2;
    break;
    case '3':
        g_clip3 = !g_clip3;
    break;
    case 'q':
        exit( 0 );
    break;


// set the rotation along the y axis
    case '-':
        g_angle_y -= INC_VAL * 4.0f;
    break;
    case '=':
        g_angle_y += INC_VAL * 4.0f;
    break;
    // move the view point up and down
    case '[':
        g_eye_y -= .1f;
    break;
    case ']':
        g_eye_y += .1f;
    break;
    // translate each clipping plane
    case 'j':
        if( g_clip1 && g_clip_x > -1.0f ) g_clip_x -= .1f;
    break;
    case 'l':
        if( g_clip1 && g_clip_x < 1.0f ) g_clip_x += .1f;
    break;
    case ',':
        if( g_clip2 && g_clip_y > -1.0f ) g_clip_y -= .1f;
    break;
    case 'i':
        if( g_clip2 && g_clip_y < 1.0f ) g_clip_y += .1f;
    break;
    case 'u':
        if( g_clip3 && g_clip_z > -1.0f ) g_clip_z -= .1f;
    break;
    case 'm':
        if( g_clip3 && g_clip_z < 1.0f ) g_clip_z += .1f;

 break;
    // reverse the half space that is removed
    case 'x':
        eqn1[0] *= -1;
    break;
    case 'y':
        eqn2[1] *= -1;
    break;
    case 'z':
        eqn3[2] *= -1;
    break;
    }
    // do a reshape since g_eye_y might have changed
    reshapeFunc( g_width, g_height );
    glutPostRedisplay( );
}
//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // rotate
        if( state == GLUT_DOWN )
            g_inc -= INC_VAL;
        else
            g_inc += INC_VAL;
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {



     if( state == GLUT_DOWN )
            g_inc += INC_VAL;
        else
            g_inc -= INC_VAL;
    }
    else
        g_inc = 0.0f;

    glutPostRedisplay( );
}
//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
}
//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
    // clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix( );
        // rotate the sphere about y axis
        glRotatef( g_angle_y += g_inc, 0.0f, 1.0f, 0.0f );
        // set up the clipping planes
        glPushMatrix( );


    glTranslatef( g_clip_x, 0.0f, 0.0f );
            glClipPlane( GL_CLIP_PLANE0, eqn1 );
        glPopMatrix( );

        glPushMatrix( );
            glTranslatef( 0.0f, g_clip_y, 0.0f );
            glClipPlane( GL_CLIP_PLANE1, eqn2 );
        glPopMatrix( );

        glPushMatrix( );
            glTranslatef( 0.0f, 0.0f, g_clip_z );
            glClipPlane( GL_CLIP_PLANE2, eqn3 );
        glPopMatrix( );

        // enable each clipping plane
        if( g_clip1 )
            glEnable( GL_CLIP_PLANE0 );
        else
            glDisable( GL_CLIP_PLANE0 );

        if( g_clip2 )
            glEnable( GL_CLIP_PLANE1 );
        else
            glDisable( GL_CLIP_PLANE1 );

        if( g_clip3 )
            glEnable( GL_CLIP_PLANE2 );
        else
            glDisable( GL_CLIP_PLANE2 );
        // draw spheres inside of spheres
        glColor3f( 0.4f, .4f, 1.0f );



   glutSolidSphere( .23, 16, 16 );
        glColor3f( 1.0f, .4f, .4f );
        glutSolidSphere( .45, 16, 16 );
        glColor3f( 1.0f, .8f, .4f );
        glutSolidSphere( .71, 16, 16 );
        glColor3f( 0.4f, 1.0f, .4f );
        glutSolidSphere( 1.0, 16, 16 );
        glPopMatrix( );
        glFlush( );
        glutSwapBuffers( );
}


