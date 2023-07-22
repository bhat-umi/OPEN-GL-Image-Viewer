#include<stdio.h>
#include<GL/glut.h>
#include "listImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define LR 100
#define TB 25
#define MAX 100
char *images[MAX];
int imageCount;
int curImage=0;
int cur_width;
int cur_height;
int isSlideShow;

int height,width,channels; //for image
float sx=1;
int driftX=0;
int driftY=0;
struct Rotationfilters
{
    int rotation;
    int isset;
}Rfilter;
struct ColorFilters
{
    int R:8;
    int G:8;
    int B:8;
    int BW:1;
    

}CFilter;



void reshape(int w,int h)
{
   
   cur_width=w;
   cur_height=h;
   glutPostRedisplay();

}
void init()
{
    
    glClearColor(0,0,0,0);
}
void clearFilters(char f)
{
    switch(f)
    {
        case 'C'://color filters
            CFilter.R=0xFF;
            CFilter.G=0xFF;
            CFilter.B=0xFF;
            CFilter.BW=0;
            break;
        case 'R': //rotattion filter
             Rfilter.isset=0;
             Rfilter.rotation=0;
             break;
        case 'A'://all filters
            CFilter.R=0xFF;
            CFilter.G=0xFF;
            CFilter.B=0xFF;
            CFilter.BW=0;
            Rfilter.isset=0;
            Rfilter.rotation=0;
            sx=1;
            driftX=0;
            driftY=0;

  
            

    }

    
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
   
   
    int r=0,c=0;
    unsigned char *data=stbi_load(images[curImage],&width,&height,&channels,0);
    int size =height*width*channels;
    glutSetWindowTitle(images[curImage]);
    if(data==NULL)
    {
        printf("image not loded");
    }
    //seting the view port;
    if(width<cur_width)
     glViewport((cur_width-width)/2,(cur_height-height)/2,width,height);
     else
     glViewport(0,0,cur_width,cur_height);


   
   
    //rotating
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   
    gluOrtho2D(0+driftX,width+LR+driftX,0+driftY,height+TB+driftY);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

   glTranslatef((width+LR)/2,(height+TB)/2,0);
    glRotatef(Rfilter.rotation,0,0,1);
    glTranslatef(-(width+LR)/2,-(height+TB)/2,0);
   glScalef(sx,sx,1);
   
   
    int R,G,B;
    int p=0;

    for (r=height;r>0;r--)
    {
         glBegin(GL_LINE_STRIP);
        for (c=0 ;c<width;c++)
        {
            R=*(data+p);
            G=*(data+p+1);
            B=*(data+p+2);
            if(CFilter.BW)
            {
                int avg=(R+B+G)/3;
                glColor3ub(avg,avg,avg);
            }
            else
                glColor3ub(R&CFilter.R,G&CFilter.G,B&CFilter.B);

            
                glVertex2f(LR/2+c,r);
            p+=3;
            
        }
         glEnd();
        
    }
     

         
        
    
   
    
    stbi_image_free(data);
    
    glFlush();


}

void loadImage(int count,char **value)
{
    char path[MAX_PATH]="images";
     if(count ==2)
        strcpy(path,value[1]); //path spedified


   imageCount=list_image(path,images);
    if(!imageCount)
    {
        printf("no images in the directory\n");
        exit(0);
    }
}

void SpecialInput(int key, int xx, int yy )
{
     isSlideShow=0;
     clearFilters('A');

switch(key)

    {
    case GLUT_KEY_UP:
    //do something here
    break;
    case GLUT_KEY_DOWN:
    //do something here
    break;
    case GLUT_KEY_LEFT:
        if(curImage>0)
            curImage--;
    break;
    case GLUT_KEY_RIGHT:
            if(curImage<imageCount-1)
                 curImage++;
    break;

    }
    
    glutPostRedisplay();
}
void driftU(int d)
{
    switch (d)
    {
    case 1:
         if(driftX<(sx)*width-width)
            driftX+=10;
        break;
    case 0:
        if(driftX>0)
            driftX-=10;
            break;
    case 2:
         if(driftY<(sx)*height-height)
            driftY+=10;
        break;
    case 3:
        if(driftY>0)
            driftY-=10;
            break;
  
    }
}
void keyboardListener(unsigned char key,int p ,int q)
{
    isSlideShow=0;
    switch (key)
    {
    case ' ': 
        curImage=(curImage+1)%imageCount;
        clearFilters('A');
        break;
        case 'A':
        case 'a':
            driftU(0);
            break;
        case 'D':
        case 'd':
             driftU(1);
             break;
        case 's':
        case 'S':
            driftU(2);
            break;
        case 'w':
        case 'W':
             driftU(3);
             break;
        
        case '=':
        if(sx<4)
            sx+=.5;
             driftU(1);
             
  
        break;
        case '-':
        sx-=.5;
        driftU(0);
        if(sx<=1)
        {
            sx=1;
            driftX=0;
             driftY=0;
        }
            

 
        
        break;
        default:
        return;
    }
      glutPostRedisplay();
    printf("key = %d",key);
    
}
void next(int nn)
{
    curImage=(curImage+1)%imageCount;
    clearFilters('A');
    glutPostRedisplay();
    if(isSlideShow)
    {
        glutTimerFunc(2000,next,0);
    }

}
void menu(int ch)
{
    switch(ch)
    {
        case 1:
            //enable timmer
            isSlideShow=1;
            next(0);
            break;
    }
    
}

void COLOR_FILTER(int ch)
{
    clearFilters('C');
    switch (ch)
    {
    case 1:
        CFilter.R=0;
    
     break;
    case 2:
        CFilter.G=0;
     break;
    
    case 3:
        CFilter.B=0;
     break;
    case 4:
        CFilter.BW=1;
     break;
    
    default:
        break;
    }
    glutPostRedisplay();

}
void ROTATION_FILTER(int ch)
{
    Rfilter.isset=1;

    switch(ch)
    {
        case 1:
            Rfilter.rotation+=90;
            break;
        case 2:
            Rfilter.rotation-=90;
            break;    
       
        default:
            Rfilter.rotation=0;
            Rfilter.isset=0;
            

    }
    glutPostRedisplay();

}

int main(int arc,char **argv)
{
   
    glutInit(&arc,argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowPosition(100,200);
    glutInitWindowSize(600,500);
    glutCreateWindow("Image viewer");
    loadImage(arc,argv);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(SpecialInput);
    int submenu1=glutCreateMenu(COLOR_FILTER);
    glutAddMenuEntry("     RED     ",1);
    glutAddMenuEntry("     GREEN   ",2);
    glutAddMenuEntry("     BLUE    ",3);
     glutAddMenuEntry("     BLACK&WHITE    ",4);
     glutAddMenuEntry("     CLEAR    ",99);
    clearFilters('A');
    int submenu2=glutCreateMenu(ROTATION_FILTER);
    glutAddMenuEntry(" 90  degree    ",1);
    glutAddMenuEntry(" -90 degree       ",2);
    
    glutCreateMenu(menu);
    glutAddSubMenu("  FILTER COLOR " ,submenu1);
    glutAddSubMenu("  ROTATTE " ,submenu2);
    glutAddMenuEntry("  SLIDE SHOW",1);
    
    
    
	glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
 
}