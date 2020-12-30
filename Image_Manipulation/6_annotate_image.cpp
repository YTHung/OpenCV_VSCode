/*
In the previous modules we have discussed about basic image operations. 
Before going into advanced topics, let us learn about a simple but important tool in OpenCV - Image Annotation.
In this module, we will learn how to annotate an image with

	Line,
	Circle,
	Rectangle,
	Ellipse and
	Text

These are very useful for showing the output of an algorithm. 
For example after detecting a face, you need to show the detection result using a rectangle or circle. 
Similarly, after recognizing an object, you need to show what object it is by writing on top of the object in the image itself and so on..
*/

#include "ImgManipulate.h"

int Annotate_Image() {

	// Load the image
	string imageName = "img/boy.jpg";
	Mat image = imread(imageName, IMREAD_COLOR);


	//-------------------- Draw a line --------------------
	Mat imageLine = image.clone();

	// The line starts from (322,179) and ends at (400,183)
	// The color of the line is RED (Recall that OpenCV uses BGR format)
	// Thickness of line is 5px
	// Linetype is LINE_AA
	line(imageLine, Point(200, 80), Point(280, 80), Scalar(0, 255, 0), 3, LINE_AA);

	imwrite("img/results/line.jpg", imageLine);
	imshow("imageLine", imageLine);
	waitKey(0);
	destroyAllWindows();


	//-------------------- Draw a circle --------------------
	Mat imageCircle = image.clone();
	Mat imageFilledCircle = image.clone();

	circle(imageCircle, Point(250, 125), 100, Scalar(0, 0, 255), 5, LINE_AA);
	circle(imageFilledCircle, Point(250, 125), 100, Scalar(0, 0, 255), -1, LINE_AA);

	imwrite("img/results/circle.jpg", imageCircle);
	imwrite("img/results/filledCircle.jpg", imageFilledCircle);

	imshow("imageCircle", imageCircle);
	imshow("imageFilledCircle", imageFilledCircle);
	waitKey(0);
	destroyAllWindows();


	//-------------------- Draw an ellipse --------------------
	// Note: Ellipse Centers and Axis lengths must be integers
	Mat imageEllipse = image.clone();

	ellipse(imageEllipse, Point(250, 125), Point(100, 50), 0, 0, 360, Scalar(255, 0, 0), 3, LINE_AA);
	ellipse(imageEllipse, Point(250, 125), Point(100, 50), 90, 0, 360, Scalar(0, 0, 255), 3, LINE_AA);

	imwrite("img/results/ellipse.jpg", imageEllipse);
	imshow("imageEllipse", imageEllipse);
	waitKey(0);
	destroyAllWindows();


	Mat ellipseFilled = image.clone();
	// Incomplete/Open ellipse
	ellipse(ellipseFilled, Point(250, 125), Point(100, 50), 0, 180, 360, Scalar(255, 0, 0), 3, LINE_AA);
	// Filled ellipse
	ellipse(ellipseFilled, Point(250, 125), Point(100, 50), 0, 0, 180, Scalar(0, 0, 255), -2, LINE_AA);
	imwrite("img/results/ellipseFilled.jpg", ellipseFilled);
	imshow("ellipseFilled", ellipseFilled);
	waitKey(0);
	destroyAllWindows();


	//-------------------- Draw an rectangles --------------------
	Mat imageRectangle = image.clone();
	rectangle(imageRectangle, Point(170, 50), Point(300, 200), Scalar(255, 0, 255), 5, LINE_8);
	imwrite("img/results/rectangle.jpg", imageRectangle);
	imshow("imageRectangle", imageRectangle);
	waitKey(0);
	destroyAllWindows();


	//-------------------- Add Text --------------------
	// a. Put text on image
	Mat imageText = image.clone();
	string text = "I am studying";
	double fontScale = 1.5;
	int fontFace = FONT_HERSHEY_SIMPLEX;
	Scalar fontColor = Scalar(250, 10, 10);
	int fontThickness = 2;

	putText(imageText, text, Point(20, 350), fontFace, fontScale, fontColor, fontThickness, LINE_AA);
	imwrite("img/results/text.jpg", imageText);
	imshow("imageText", imageText);
	waitKey(0);
	destroyAllWindows();

	/*
		Issues with annotating text.
		First, the text looks pretty big. This happens very often because it is difficult to say before-hand what size text you should use.
		If the image is very large, fontScale=1.5 can look too small and vice-versa.
		Second, You can see the bottom part of the text is slightly clipped. 
		This happens because we dont know how big the text will look on the image.
	*/
	// b. Get font size from pixel height of text
	int pixelHeight = 20;

	// Calculate the fontScale
	fontScale = getFontScaleFromHeight(fontFace, pixelHeight, fontThickness);
	cout << "fontScale = " << fontScale;

	Mat imageTextFontScale;
	imageTextFontScale = image.clone();
	putText(imageTextFontScale, "I am studying", Point(20, 350), fontFace, fontScale, fontColor, fontThickness, LINE_AA);
	imwrite("img/results/text2.jpg", imageTextFontScale);
	imshow("imageTextFontScale", imageTextFontScale);
	waitKey(0);
	destroyAllWindows();


	// c. Get height and width of text
	Mat imageGetTextSize;
	imageGetTextSize = image.clone();
	int imageHeight = imageGetTextSize.rows;
	int imageWidth = imageGetTextSize.cols;

	// Get the text box height and width and also the baseLine
	int baseLine = 0;
	Size textSize = getTextSize("I am studying", fontFace, fontScale, fontThickness, &baseLine);

	int textWidth = textSize.width;
	int textHeight = textSize.height;

	cout << "TextWidth = " << textWidth << ", TextHeight = " << textHeight << ", baseLine = " << baseLine;

	// Get the coordinates of text box bottom left corner
	// The xccordinate will be such that the text is centered
	int xcoordinate = (imageWidth - textWidth) / 2;
	// The y coordinate will be such that the entire box is just 10 pixels above the bottom of image
	int ycoordinate = (imageHeight - baseLine - 10);

	cout << "TextBox Bottom Left = (" << xcoordinate << "," << ycoordinate << ")";

	// Draw the Canvas using a filled rectangle
	Scalar canvasColor = Scalar(255, 255, 255);
	Point canvasBottomLeft(xcoordinate, ycoordinate + baseLine);
	Point canvasTopRight(xcoordinate + textWidth, ycoordinate - textHeight);

	rectangle(imageGetTextSize, canvasBottomLeft, canvasTopRight, canvasColor, -1);

	cout << "Canvas Bottom Left = " << canvasBottomLeft << ", Top Right = " << canvasTopRight;

	// Now draw the baseline ( just for reference )
	int lineThickness = 2;
	Point lineLeft(xcoordinate, ycoordinate);
	Point lineRight(xcoordinate + textWidth, ycoordinate);
	Scalar lineColor = Scalar(0, 255, 0);

	line(imageGetTextSize, lineLeft, lineRight, lineColor, lineThickness, LINE_AA);

	// Finally Draw the text
	putText(imageGetTextSize, "I am studying", Point(xcoordinate, ycoordinate), fontFace, fontScale,
		fontColor, fontThickness, LINE_AA);
	imwrite("img/results/text3.jpg", imageGetTextSize);
	imshow("imageText", imageGetTextSize);
	waitKey(0);
	destroyAllWindows();



	return 0;
}