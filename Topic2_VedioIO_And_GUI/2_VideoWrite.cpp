/*
    Properties: https://docs.opencv.org/4.1.0/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d
    FourCC:  4-character code of codec used to compress the frames. 
             For example, VideoWriter::fourcc('P','I','M','1') is a MPEG-1 codec, 
                          VideoWriter::fourcc('M','J','P','G') is a motion-jpeg codec etc
*/

#include "VideoIO_GUI.h"

int Video_Write(){

  VideoCapture cap(0);   // Create a VideoCapture object

  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);      // get the video's property
  int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);    // get the video's property
  VideoWriter outavi("results/output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));
  VideoWriter outmp4("results/output.mp4", cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), 10, Size(frame_width, frame_height));

  //Read and save the feed from webcam until ESC is pressed.
  while(1){

    Mat frame;           // declare object "frame"
    cap >> frame;        // Capture frame-by-frame
    if (frame.empty())   // If the frame is empty, break immediately
      break;

    // Write the frame
    outavi.write(frame);
    outmp4.write(frame);

    imshow("Frame",frame);
    char c=(char)waitKey(25);
    if (c == 27)
      break;
  }

  // When everything done, release the video capture object
  cap.release();
  outavi.release();
  outmp4.release();

  // Closes all the frames
  destroyAllWindows();

  return 0;
}
