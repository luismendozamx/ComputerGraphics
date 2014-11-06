using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Imaging;
using System.Media;
using CSGL12;
/**
 * Blast from the past!!!!!
 * Toda esta tecnología para replicar un PONG!
 * Cortesia de @hipopotamo
 */
namespace Atari
{

    public class Handler
    {
        //Estructuras de datos
        // puntos de impacto  
        public enum collision { NO_COLLISION, FROM_RIGHT,
            FROM_LEFT, FROM_TOP, FROM_BOTTOM };
        //definir coordenadas de objetos rectangulares
        public struct RECTANGLE
        {public float left, top, right, bottom;
            public RECTANGLE(float l, float t, float r,
                             float b)
            {
                left = l;
                right = r;
                top = t;
                bottom = b;
            }
        };
        //tamaño de la ventana de juego, global para poderlo acceder desde el juego
        public int clientWidth;
        public int clientHeight;
        //variables del juego
        public int playerResult, computerResult;
        public float Xspeed, Yspeed;
        public float delta; //factor de velocidad
        public RECTANGLE ball;
        public RECTANGLE wall;
        public RECTANGLE player_1;
        //interfaz
        //marcadores
        public Label displayP, displayC;
        //posición del mouse
        public int mouse_x;
        
        public Handler()
        {
            playerResult = computerResult = 0;
            Xspeed = 1; Yspeed = 1;
            delta = 1; //mover para mas velocidad
            ball = new RECTANGLE(100, 100, 120, 120);
            player_1 = new RECTANGLE(0, 490, 40, 500);
        }

       

        public void OpenGLStarted(CSGL12Control csgl12Control)
        {
            GL gl = csgl12Control.GetGL();

            if (null == gl) { return; }

            // Basic drawing conditions
           
            // evitar parpadeos
            if (true == gl.bwglSwapIntervalEXT)
            {
                gl.wglSwapIntervalEXT(1);

            }
        }

        public void Paint(object sender, PaintEventArgs e)
        {
            if (null == sender) { return; }
            if (false == (sender is CSGL12Control)) { return; }

            //Sacar el control de GL y sus dimensiones
            CSGL12Control csgl12Control = (sender as CSGL12Control);
            GL gl = csgl12Control.GetGL();

            clientWidth = csgl12Control.ClientRectangle.Width;
            clientHeight = csgl12Control.ClientRectangle.Height;

            if (clientWidth <= 0)
            {
                clientWidth = 1;
            }

            if (clientHeight <= 0)
            {
                clientHeight = 1;
            }

            //Asignar un viewport  del tamaño del control
            gl.glViewport(0, 0, clientWidth, clientHeight);

            //Limpiar la pantalla con un color de fondo
            gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);


            //Asignar la vista del modelo y la proyeccion
            gl.glMatrixMode(GL.GL_PROJECTION);
            gl.glLoadIdentity();

            gl.gluOrtho2D(0, clientWidth, clientHeight, 0);
            

            gl.glMatrixMode(GL.GL_MODELVIEW);
            gl.glLoadIdentity();

            //Aquí va tu dibujito chido
            Render(gl); //dibujar todo el chorizo de juego
            //modificar la fisica  
            if (Test_Ball_Wall(ball, wall) == collision.FROM_RIGHT)
                Xspeed = -delta;
            if (Test_Ball_Wall(ball, wall) == collision.FROM_LEFT)
                Xspeed = delta;
            if (Test_Ball_Wall(ball, wall) == collision.FROM_TOP)
                Yspeed = delta;
            if (Test_Ball_Wall(ball, wall) == collision.FROM_BOTTOM)
                Yspeed = -delta;
            ball.left += Xspeed;
            ball.right += Xspeed;
            ball.top += Yspeed;
            ball.bottom += Yspeed;
            // Forzar el dibujado de todo y cambiar el buffer de ser necesario
            gl.wglSwapBuffers(csgl12Control.GetHDC());
        }


        public void Render(GL gl)
        {
            gl.glClear(GL.GL_COLOR_BUFFER_BIT);
            gl.glLoadIdentity();
            wall.left = wall.top = 0;
            wall.right = clientWidth;
            wall.bottom = clientHeight;

            DrawRectangle(gl, ball);
        }
        //Si, es un simple quad
        public void DrawRectangle(GL gl, RECTANGLE rect)
        {
            gl.glBegin(GL.GL_QUADS);
            gl.glVertex2f(rect.left, rect.bottom);      //Left - Bottom 
            gl.glVertex2f(rect.right, rect.bottom);
            gl.glVertex2f(rect.right, rect.top);
            gl.glVertex2f(rect.left, rect.top);
            gl.glEnd();
        }

        //Control de colisiones

        //Determinar de que lado fue la colision entre pelota y pared
        public collision Test_Ball_Wall(RECTANGLE ball, 
            RECTANGLE wall)
        {
            if (ball.right >= wall.right)
                return collision.FROM_RIGHT;
            if (ball.left <= wall.left)
                return collision.FROM_LEFT;
            if (ball.top <= wall.top)
                return collision.FROM_TOP;
            if (ball.bottom >= wall.bottom)
                return collision.FROM_BOTTOM;

            else return collision.NO_COLLISION;
        }

        //Determinar impacto de raqueta con pelota
        public bool Test_Ball_Player(RECTANGLE ball, RECTANGLE player)
        {
            return true;
           
        }

        public void MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                //mouse_x = e.X;
            }
        }

        public void MouseUp(object sender, MouseEventArgs e)
        {

        }



        public void MouseMove(object sender, MouseEventArgs e)
        {
            //servira para colocar la raqueta
            // if (e.Button == MouseButtons.Left)
            {
                mouse_x = e.X;
            }
        }

        public void MouseWheel(object sender, MouseEventArgs e)
        {
            /* double d = e.Delta * 0.01;
              */
        }

        public void KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                ((Form)(displayP.Parent)).Close(); //manera divertida de cerrar la forma
            }
        }

        public void KeyUp(object sender, KeyEventArgs e)
        {

        }
    }
}

