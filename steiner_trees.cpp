/* nshah213.cpp
 *  Graphics demo program for getting started with QtCreator for program 6, Steiner Trees.
 *
 * Author: Nemil R Shah
 * Date: 11/30/17
 * Class: CS 141, Fall 2017
 * System: QtCreator with Stanford graphics library, on iMac, OS Sierra
 *
 */

#include <iostream>
#include "gobjects.h"  // for all graphics objects
#include "gevents.h"   // for mouse events
#include <fstream>        // For file input
#include <cmath>
#include <ctime>
using namespace std;

// HEADER INFORMATION.
/* Program is about the graphical presentation of points with which we can interact to create a Steiner Tree.

   The program details is mentioned below:

   Program #1: Guess It
   Author: Nemil R Shah
   Lab: Tues 1am to 1:50 am
   Lab assistant: Abhinav Kumar
   Date: November 30, 2017
   System:  C++ in QT CREATOR.

   Task : The program will have a data file with various sets of points, along with reasonable known solution lengths for each set.
            It will plot the starting points on the screen as black points.  The user can:
            Press a button to add Steiner points to the graph, which will show up as blue points.
            Press a button to draw lines between points (either blue or black or mixed).
            The program keeps track of the sum of all the line lengths.  The user is responsible for keeping track of when all points are connected.
            A message indicates whether the total length is close to the known solution or not.
*/

// Global constants
const int PointRadius = 7;

// Create a graphics window, which will be used to draw graphical shapes.
// This can be a global variable, but you may not have any other global variables.
GWindow graphicsWindow;

// CLASS FOR THE POINT.
class MyPoint
{
private:
    int x;
    int y;
    string color;
    GOval *pTheOval;

public:

    // SET AND GET FUNCTIONS.
     setPoint(int X, int Y, string Color)
     {
         x=X;
         y=Y;
         color= Color;
     }

     getPointX()
     {
         return x;
     }

     getPointY()
     {
         return y;
     }

     // FUNCTION TO DRAW THE POINT ON GRAPHIC SCREEN.
    void drawPoint(GOval *TheOval)
    {


        pTheOval=TheOval;
        pTheOval = new GOval( x-PointRadius, y-PointRadius, PointRadius*2, PointRadius*2);
        pTheOval->setFilled( true);
        pTheOval->setColor( color);
        graphicsWindow.add( pTheOval);
    }
};

//  CLASS FOR THE LINE.
class MyLine
{
private:
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    GLine *line;

  public:
    // FUNCTION RETURNING THE CLOSEST POINT THE LINE DRAG.
    MyPoint close( Vector<MyPoint> &pointsArray, int mouseX, int mouseY, int number)
    {
        int length[number];
        int minlength=0;

        for(int i=0;i<number;i++)
        {
            length[i]=sqrt((pointsArray[i].getPointX()-mouseX)*(pointsArray[i].getPointX()-mouseX)+(pointsArray[i].getPointY()-mouseY)*(pointsArray[i].getPointY()-mouseY));
            minlength=length[0];
        }

        for(int i=0;i<number;i++)
        {
            if(length[i] <minlength)
            {
                minlength= length[i];
            }
        }

        for(int i=0;i<number;i++)
          {
              if(minlength==length[i])
              {
                 return pointsArray[i];
              }

          }
    }

   // ACCESSOR FUNCTIONS FOR LINE.
    void setLine(int x, int y, int XX, int YY)
    {
        xStart=x;
        yStart=y;
        xEnd=XX;
        yEnd=YY;
        cout << xStart << yStart << xEnd <<yEnd <<endl;
    }

    void setStart( int x, int y)
    {
        xStart=x;
        yStart=y;
        line->setStartPoint( xStart, yStart);
    }

    void setEnd(int x, int y)
    {
        xEnd=x;
        yEnd=y;
        line->setEndPoint( xEnd, yEnd);
    }

    // FUNCTION RETURNING THE LENGTH OF THE LINE SEGMENT.
    int lenghOfLine()
    {
        int length=0;
        int height= line->getHeight();
        int width= line->getWidth();

        //APPLY PYTHAGORS THEOREM TO RETURN LENGTH.
        length= sqrt((height*height)+(width*width));
        return length;
    }

    // FUNCTION TO DRAW THE LINE ON GRAPHIC SCREEN.
    void drawLine(GLine *Line)
     {
         line= Line;
         line = new GLine(xStart,yStart,xEnd,yEnd);  // Line to be drawn on the screen.  It starts out as a single unseen point.
         line->setLineWidth(2);
         graphicsWindow.add(line);
     }
};

//  CLASS FOR THE BOTTOM.
class MyButton
{
private:
    int ButtonX ;
    int ButtonY ;
    int ButtonWidth ;
    int ButtonHeight ;
    string Color;
    string Text;
     GRect *Rect;

public:
   // CONSTRUCTOR TO INITIALIZE ALL PRIVATE MEMBERS.
    MyButton (int x, int y, int width, int height, string color, string text)
    {
        ButtonX =x;
        ButtonY=y;
        ButtonWidth= width;
        ButtonHeight= height;
        Color= color;
        Text= text;
    }
    // FUNCTION TO DRAW THE BUTTON ON GRAPHIC SCREEN.
    void drawRectangle(GRect *Rectangle, GLabel *label )
    {
        Rect= Rectangle;
        Rect = new GRect(ButtonX, ButtonY, ButtonWidth, ButtonHeight);
        Rect->setColor( Color);
        Rect->setFilled( true);
        graphicsWindow.add( Rect);


        label = new GLabel();
        label->setText(Text);
        // Find size of text, to determine placement relative to enclosing rectangle
        int xOffset = (ButtonWidth  - label->getBounds().getWidth()) / 2;
        int yOffset = (ButtonHeight - label->getBounds().getHeight()) + 5;
        label->setLocation( ButtonX + xOffset, ButtonY + yOffset);
        graphicsWindow.add( label);
    }

    // FUNCTION TO CHECK IF THE MOUSE PRESSED BUTTON IS INSIDE THE RECTANGLE OR NOT.
    bool clickInside(int mouseX, int mouseY)
    {
        if ( Rect->contains( mouseX,mouseY))
            return true;
    }
};

//------------------------------------------------------------------------------------------
// Read in the sets of points information from the data file
// You muse make the pointsArray Vector a reference parameter so that read-in values are
// reflected back to the caller.
int readInputFile( Vector<MyPoint> &pointsArray, int &idealScore)
{
    ifstream inputFileStream;  // declare the input file stream

    // Open input file and verify.
    // You must first:
    //    1. Outside of QtCreator go into your project's resources folder, copy readme.txt into data.txt
    //    2. Edit data.txt to have the datafile information as shown on the assignment page
    //    3. Now WITHIN QtCreator right-click on the "res" folder (under "Other files") and
    //       choose "add new" and add the new data.txt file so it shows up in your environment.
    inputFileStream.open("data.txt");
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file data.txt.  Exiting..." << endl;
        exit( -1);
    }

    // Read sets of data from input file.  Each read operation reads the next set of data.
    // The first line in the input file indicates which set to use.
    int whichSetToUse;
    inputFileStream >> whichSetToUse;

    int howManyPoints;    // How many points are in the set of points
    int x, y;             // Store individual point x,y values from input file
    // Reserve some reasonable number of total points for the vector, so that
    pointsArray.ensureCapacity( 10);

    // Read in and discard some number of sets of points before we get the real set we will be using
    for( int i = 0; i < whichSetToUse-1; i++) {
        inputFileStream >> howManyPoints >> idealScore;
        for( int j = 0; j < howManyPoints; j++) {
            inputFileStream >> x >> y;    // Read in the points
        }
    }

    inputFileStream >> howManyPoints >> idealScore;

    // Read in and store the points information
    for( int i = 0; i < howManyPoints; i++)
    {
        inputFileStream >> x >> y;    // Read in the points
        // This is where you should take the points information and create points on the screen.
        // For now we will just print out the points information.
        cout << "Point " << i << " is at " << x << "," << y << endl;

        pointsArray[i].setPoint( x, y, "black");
        GOval* cir;
        pointsArray[i].drawPoint ( cir);
    }

    return howManyPoints;
}//end readInputFile()

//------------------------------------------------------------------------------------------
int main()
{
    int NumberofPoints=0;                                   // Variable for tracing number of points used.
    graphicsWindow.setCanvasSize( 400, 400);                // Set the size
    graphicsWindow.setWindowTitle(" nshah213  QtCreator Program 6");
    Vector<MyPoint> pointsArray(100);    // Store all points that are created.  //Array of objects.
    Vector<MyLine> linesArray(100);   //  Store all lines that are created.  //Array of objects
    int idealScore = 0;             // Precomputed ideal score for each puzzle.
    int idealScoreRange=0;          // To set the max limit of idealscore.
    int length=0;                   // Variable to display the length on graphics screen.
    int tempLength=0;               // Varaible to store length of one line segment.
    int displayLength=0;            // Variable to display length in graphically in contagious way.
    MyPoint closestpoint;           // Object instance of class point, which helps in finding the closest point.

    // Get the set of data to use from the input file
    NumberofPoints=readInputFile( pointsArray, idealScore);

    idealScoreRange= (idealScore*105)/100;

    // Create the Control and message components.
    // First create the messages label at the top of the window
    GLabel *messagesLabel = new GLabel();
    messagesLabel->setText("Find the minimal spanning tree, using additional points.");
    messagesLabel->setLocation( 30,20);
    graphicsWindow.add( messagesLabel);  // As with all components, after creating them you must also
                                         // add them to the graphics window, otherwise they will
                                         // exist but won't show up.

    // MESSAGE LABEL AT THE BOTTOM RIGHT TO PRINT THE LENGTH.
    GLabel *lengthLabel = new GLabel();
    lengthLabel->setText("Length: 0");
    lengthLabel->setLocation( 320,370);
    graphicsWindow.add( lengthLabel);

 //..............***************************.......................................//

    //OBJECTS INSTANCES FOR CLASS MYBUTTON.
    MyButton exitButton(250,350,50,30, "red", "Exit");
    MyButton addPointButton(10,350,70,30, "green", "Add points");
    MyButton addLineButton(90,350,70,30, "light gray", "Add lines");

    // Initialize Rectangles and labels for Button objects.
    GRect *exitRect;        GLabel *exitLabel;
    GRect *addPointRect;    GLabel *addPointLabel;
    GRect *addLineRect;    GLabel *addLineLabel;

    // DRAW ALL BUTTONS ON SCREEN.
    exitButton.drawRectangle( exitRect, exitLabel );
    addPointButton.drawRectangle(addPointRect, addPointLabel  );
    addLineButton.drawRectangle(addLineRect, addLineLabel );

  //..............***************************.......................................//

    // Make the graphics window have focus, since all interaction will be through the window
    graphicsWindow.requestFocus();

    // Set values to use for keeping track of program modes
    const int Default = 0;
    const int DrawPoints = 1;
    const int DrawLines = 2;
    int programMode = Default;  // Modes are Default, DrawPoints, DrawLines. Start in Default mode
                                // Change to the other modes when those buttons are pressed.
    bool buttonWasPressed = false;    // Set to false when a button is pressed.  Used to avoid drawing on buttons
                                      // immediately when they are pressed.

    int mouseX = -1;                    // Will store mouse position
    int mouseY = -1;
    char message[ 81];                 // Used to construct message to be displayed at top of window
    int numLines=-1;                 // Variable to count and trace the number of lines which is used for creating array of lines (objects).
    GMouseEvent e;                  // Stores the mouse event each time through the event loop
    int counter = 0;                 // Used to display debugging information, to help understand how the event loop works.
    bool flag= false;               // Bool variable to stop the event.

//....*************...MAIN WHILE LOOP STARTS..............*************................//
    // Main event loop.  Most of your code will go in here, to handle the different mouse events.
    while (true)
    {
        // Wait for a mouse event, then get the x,y coordinates

        e = waitForEvent(MOUSE_EVENT);
        // Get the current mouse x,y location
        mouseX = e.getX();
        mouseY = e.getY();

        counter++;    // Iterations counter
        // Use the sprintf string function to print into a string, which in turn will be displayed in the messages label
       // sprintf( message,"%d. Mouse at %d,%d", counter, mouseX, mouseY);  // print into a string
       // messagesLabel->setLabel(message);

        //  -------- Perform actions for different kinds of mouse events --------
        // MOUSE PRESSED.
        if (e.getEventType() == MOUSE_PRESSED)
        {
            flag= false;

        //    sprintf( message,"Mouse pressed at %d,%d", mouseX, mouseY);  // print into a string
        //   messagesLabel->setLabel(message);

            // See if Exit button was clicked by seeing if mouse press location is inside the rectangle
            if(exitButton.clickInside( mouseX, mouseY)== true)
            {
                messagesLabel->setLabel("Exiting...");
                break;
            }

            // See if ADD LINE button was clicked by seeing if mouse press location is inside the rectangle
             if((addLineButton.clickInside( mouseX, mouseY)== true))
             {
                 buttonWasPressed= false;
                 cout <<"Once the addline button is clicked you should set the programMode to Drawline.  \n";
                 programMode = DrawLines;
                 flag=true;

             }

            // SET THE PROGRAM MODE TO DRAW LINES.
            if( programMode == DrawLines)
            {
               if((addPointButton.clickInside( mouseX, mouseY)== false) && (addLineButton.clickInside( mouseX, mouseY)== false))
               {
                    numLines++;

                    linesArray[numLines].setLine(0,0,0,0);
                    GLine *pLine;
                    linesArray[numLines].drawLine(pLine);
                    sprintf( message,"Setting line start and end points to %d, %d",mouseX,mouseY);
                    messagesLabel->setLabel(message);

                    closestpoint= linesArray[numLines].close( pointsArray, mouseX,mouseY, NumberofPoints);
                    linesArray[numLines].setStart( closestpoint.getPointX(), closestpoint.getPointY());
                    linesArray[numLines].setEnd( mouseX, mouseY);
               }

            }
            //  SET THE PROGRAM MODE TO DRAW LINES.
            if( programMode == DrawPoints)
            {
               if((addPointButton.clickInside( mouseX, mouseY)== false) && (addLineButton.clickInside( mouseX, mouseY)== false))
               {
                pointsArray[NumberofPoints].setPoint( mouseX,mouseY, "blue");
                GOval* cir;
                pointsArray[NumberofPoints].drawPoint ( cir);
                NumberofPoints++;
                }
            }

            // See if ADD POINT BUTTON was clicked by seeing if mouse press location is inside the rectangle
            if((addPointButton.clickInside( mouseX, mouseY)== true) && ( buttonWasPressed ==false))
            {
                buttonWasPressed= true;
                programMode = DrawPoints;   // SET PROGRAM MODE TO DRAW POINTS.
            }

        }

        //  MOUSE RELEASED
        else if(( e.getEventType() == MOUSE_RELEASED))
        {
           if( buttonWasPressed== false && programMode == DrawLines && flag==false)
            {
               //  sprintf( message,"Mouse released at %d,%d", mouseX, mouseY);  // print into a string
                // messagesLabel->setLabel(message);
                //  cout << counter << endl;
            //     cout << "Mouse was released at : " << mouseX << "," << mouseY << endl;
                 // Depending on your program mode (DrawPoints or DrawLines), draw points or create a new line and store it somewhere,";

                 // Set the end point of the previously created line that currently has the same begin & end points
                   linesArray[numLines].setEnd( mouseX, mouseY);

                   // CODE TO SET THE END POINT OF LINE TO THE CLOSEST POINT.
                   closestpoint= linesArray[numLines].close( pointsArray, mouseX,mouseY, NumberofPoints);
                   linesArray[numLines].setEnd( closestpoint.getPointX(), closestpoint.getPointY());

                   tempLength=linesArray[numLines].lenghOfLine();
                   length= tempLength+length;
                   displayLength= length;

                   // PRINTING THE LENGTH ON GRAPHIC SCREEN.
                   sprintf( message,"Length: %d", length);  // print into a string
                   lengthLabel->setLabel(message);

                   // CONDITIONAL STATESMENTS FOR CHECKING THE USERINPUT PLOTTING LINE RESULTS.
                   if(length<idealScoreRange)
                   {
                       sprintf( message, " Great %d is less or close to %d", length, idealScore); // print into a string
                       messagesLabel->setLabel(message);
                   }
                   else
                   {
                       sprintf( message, " Sorry %d is larger than %d", length, idealScore); // print into a string
                       messagesLabel->setLabel(message);
                   }
            }
        }

        //  MOUSE DRAGGED
        else if(e.getEventType() == MOUSE_DRAGGED)
        {
            if((programMode == DrawLines) && (buttonWasPressed==false))
            {
               // cout << "Mouse dragged to " << mouseX << "," << mouseY << endl;
                // The previous MOUSE_PRESSED event created the line, though with beginning and ending points being the same.
                // Now set the end point to be at the new mouse destination.  Don't create the new line here, since we don't
                // want to do that until the MOUSE_RELEASED event.

                 linesArray[numLines].setEnd( mouseX, mouseY);
                 tempLength= linesArray[numLines].lenghOfLine();
                 sprintf( message,"Length: %d", displayLength+tempLength);  // print into a string
                 lengthLabel->setLabel(message);
            }

        }//else if( e.getEventType()...

    }//end while( true)

    // Close the windows, first the graphics window
    graphicsWindow.requestFocus();
    graphicsWindow.close();
    exitGraphics();

    return 0;
}

