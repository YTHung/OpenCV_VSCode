/*
    Note:
    VideoCapture cap(args)
    Three most popular ways of reading videos using the VideoCapture Object are :

    1. Using Webcam ( Pass the argument as 0, 1, 2, ... based on the number of webcam you used)
    2. From a video File ( Specify the filename as argument)
    3. Image sequence [image_%03d.jpg]
*/
#include "VideoIO_GUI.h"

int Video_Read(){

  //VideoCapture cap("resources/chaplin.mp4");   // Create a video reader object
  VideoCapture cap(0);   // Create a video reader object

   // Check if camera opened successfully and read a frame from the object cap
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }
  
  while(1){

    Mat frame;
    cap >> frame;       // Capture frame-by-frame
    if (frame.empty())  // If the frame is empty, break immediately
      break;

    imshow( "Frame", frame );  // Display the resulting frame
    
    char c=(char)waitKey(25);  // Press ESC on keyboard to exit
    if(c==27)
      break;
  }

  cap.release();        // When everything done, release the video capture object
  destroyAllWindows();  // Closes all the frames

  return 0;
}
